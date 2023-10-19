#include <SoftwareSerial.h>
#define FORWARD true
#define BACK false
const int MOTOR_SPEED = 128;  // 모터의 속도를 결정함. -> 얼마나 빠르게 이동할 것인가?(0 ~ 255)
const int ACTIVE_PERIOD = 50; // 이동할 시간을 결정함. -> 한번에 몇 초동안 이동할 것인가?(ms)
const int SLEEP_PERIOD = 200; // loop의 지연시간을 결정함. -> 몇 초에 조종을 한번씩 할 것인가?(ms)

class Motor
{
private:
    int motorPin1;
    int motorPin2;
    int motorSpeedControlPin;

    int motorSpeed;
    bool motorDirection;

public:
    void setter(int motorPin1, int motorPin2, int motorSpeedControlPin)
    {
        this->motorPin1 = motorPin1;
        this->motorPin2 = motorPin2;
        this->motorSpeedControlPin = motorSpeedControlPin;
        setSpeed(MOTOR_SPEED);
        setDirection(FORWARD);
        return;
    }

    void setDirection(bool demandedDirection) // true: 전진, false: 후진
    {
        this->motorDirection = demandedDirection;
        digitalWrite(motorPin1, demandedDirection == FORWARD ? HIGH : LOW);
        digitalWrite(motorPin2, demandedDirection == FORWARD ? LOW : HIGH);
        return;
    }

    void setSpeed(int demandedSpeed) // sp: 0 ~ 255
    {
        if (sp >= 0 && sp <= 255)
        {
            motorSpeed = demandedSpeed;
            analogWrite(motorSpeedControlPin, motorSpeed);
        }
        return;
    }
};

class Car
{
private:
    Motor leftMotor;
    Motor rightMotor;

    void setMotorSpeed(int leftDemandedSpeed, int rightDemandedSpeed)
    {
        leftMotor.setSpeed(leftDemandedSpeed);
        rightMotor.setSpeed(rightDemandedSpeed);
    }

    void setMotorDirection(bool leftDemandedDirection, bool rightDemandedDirection)
    {
        leftMotor.setDirection(leftDemandedDirection);
        rightMotor.setDirection(rightDemandedDirection);
    }

    void driveVehicle(bool leftDemandedDirection, bool rightDemandedDirection)
    {
        // 모터 속도를 0으로 만들기
        setMotorSpeed(0, 0);
        // 모터 방향을 설정하기
        setMotorDirection(leftDemandedDirection, rightDemandedDirection);
        // 모터를 가동하기
        setMotorSpeed(MOTOR_SPEED, MOTOR_SPEED);
        // 가동 시간 지연
        delay(ACTIVE_PERIOD);
        // 모터 속도를 0으로 만들기
        setMotorSpeed(0, 0);
        return;
    }

public:
    CAR(int i1, int i2, int i3, int i4, int leftMotorSpeed, int rightMotorSpeed)
    {
        leftMotor.setter(i1, i2, leftMotorSpeed);
        rightMotor.setter(i3, i4, rightMotorSpeed);
    }

    void moveForward(void)
    {
        Serial.print("Moving Forward... ");
        driveVehicle(FORWARD, FORWARD);
    }

    void moveLeft(void)
    {
        Serial.print("Moving Left... ");
        driveVehicle(BACK, FORWARD);
    }

    void moveRight(void)
    {
        Serial.print("Moving Right... ");
        driveVehicle(FORWARD, BACK);
    }
};
