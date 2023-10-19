#include <SoftwareSerial.h>

#define BT_RXD 13
#define BT_TXD 12
#define ENA 5
#define IN1 6
#define IN2 7
#define IN3 8
#define IN4 9
#define ENB 10
const int motorPins[] = {IN1,IN2,IN3,IN4,ENA,ENB};


const int wheelSpeed = 128; //모터의 속도를 결정함. -> 얼마나 빠르게 이동할 것인가?(0 ~ 255)
//const int differentialRatio = 2; //회전 시 양쪽 바퀴의 속도 차를 결정함. 회전 시 내측의 바퀴가 (wheelSpeed / differentialRatio) 의 속도로 회전함.
const int delayPeriod = 50; //이동할 시간을 결정함. -> 한번에 몇 초동안 이동할 것인가?(ms)
const int sleepPeriod = 200; //loop의 지연시간을 결정함. -> 몇 초에 조종을 한번씩 할 것인가?(ms)


class MOTOR
{
    private:
        int pinNum[3];
        int currentSpeed;
        bool currentDirection;
    public:
        int getCurrentSpeed(void)
        {
            return currentSpeed;
        }

        void setDirection(bool dir) //true: 전진, false: 후진
        {
            currentDirection = dir;
            digitalWrite(pinNum[0], dir == true? HIGH : LOW);
            digitalWrite(pinNum[1], dir == true? LOW : HIGH);
            return;
        }

        void setSpeed(int sp) //sp: 0 ~ 255
        {
            if (sp < 0 || sp > 255)
            {
                Serial.println("Invalid motor speed!!!");
                return;
            }

            currentSpeed = sp;
            analogWrite(pinNum[2], currentSpeed);
            return;
        }

        void setter(int pin1, int pin2, int pwm)
        {
            pinNum[0] = pin1;
            pinNum[1] = pin2;
            pinNum[2] = pwm;
            currentSpeed = wheelSpeed;
            currentDirection = true;
            setDirection(currentDirection);
        }
};

class CAR
{
    private:
        MOTOR leftMotor;
        MOTOR rightMotor;

        void setSpeed(int a, int b)
        {
            Serial.print("Set speed to ");
            Serial.print(a);
            Serial.print(", ");
            Serial.print(b);
            Serial.println(".");

            leftMotor.setSpeed(a);
            rightMotor.setSpeed(b);
        }
    public:
        CAR(int i1, int i2, int i3, int i4, int speedA, int speedB)
        {
            Serial.println("CAR init...");

            leftMotor.setter(i1, i2, speedA);
            rightMotor.setter(i3, i4, speedB);

            Serial.print("CAR init complete. ");
        }

        void moveForward(void)
        {
            Serial.print("Moving Forward... ");
            setSpeed(wheelSpeed, wheelSpeed);
            
            delay(delayPeriod); //delayPeriod만큼 이동 후 정지
            stop();
        }

        void moveLeft(void)
        {
            Serial.print("Moving Left... ");
            //setSpeed(wheelSpeed / differentialRatio, wheelSpeed);
            setSpeed(0, wheelSpeed);

            delay(delayPeriod); //delayPeriod만큼 이동 후 정지
            stop();
        }

        void moveRight(void)
        {
            Serial.print("Moving Right... ");
            //setSpeed(wheelSpeed, wheelSpeed / differentialRatio);
            setSpeed(wheelSpeed, 0);

            delay(delayPeriod); //delayPeriod만큼 이동 후 정지
            stop();
        }

        void stop(void)
        {
            Serial.print("Stoping... ");
            setSpeed(0, 0);
        }
};

SoftwareSerial* bluetooth;
CAR* RC;

void setup()
{
  Serial.begin(9600);
  Serial.println("init...");

  for (int p : motorPins) //모터 제어를 위한 핀 모드 설정
  {
      pinMode(p, OUTPUT);
  }

  bluetooth = new SoftwareSerial(BT_RXD, BT_TXD);
  RC = new CAR(IN1, IN2, IN3, IN4, ENA, ENB);

  bluetooth.begin(9600);

  Serial.println("initiation complete.");
}


void loop()
{
  delay(sleepPeriod); //sleepPeriod 만큼 정지
 
  if (bluetooth.available())
  {
    int direction = bluetooth.read();

    Serial.print("Command: '")
    Serial.write(direction);
    Serial.print("'. ");

    switch(direction)
    {
      case '1': Serial.println("Go Right."); RC.moveRight(); break;
      case '2': Serial.println("Go Left."); RC.moveLeft(); break;
      default: Serial.println("Stop."); RC.stop(); break;
    }

    while(bluetooth.available() > 0) //bluetooth 버퍼를 모두 비움
    {
        bluetooth.read();
    }
  }

}

