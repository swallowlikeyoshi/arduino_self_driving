#include <pair>
#define FORWARD true
#define BACK false

//모터의 속도를 결정함. -> 얼마나 빠르게 이동할 것인가?(0 ~ 255)
const int MOTOR_SPEED = 128; 
//이동할 시간을 결정함. -> 한번에 몇 초동안 이동할 것인가?(ms)
const int ACTIVE_PERIOD = 50; 
//loop의 지연시간을 결정함. -> 몇 초에 한번씩 조종을 할 것인가?(ms)
const int SLEEP_PERIOD = 200;

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

    void setDirection(bool Direction) // true: 전진, false: 후진
    {
        this->motorDirection = Direction;
        digitalWrite(motorPin1, Direction == FORWARD ? HIGH : LOW);
        digitalWrite(motorPin2, Direction == FORWARD ? LOW : HIGH);
        return;
    }

    void setSpeed(int speed) // sp: 0 ~ 255
    {
        if (speed >= 0 && speed <= 255)
        {
            motorSpeed = speed;
            analogWrite(motorSpeedControlPin, motorSpeed);
        }
        return;
    }

    void getSpeed(void)
    {
        return motorSpeed;
    }
};

class Car
{
protected:
    Motor leftMotor;
    Motor rightMotor;

    void setMotorSpeed(int leftSpeed, int rightSpeed)
    {
        leftMotor.setSpeed(leftSpeed);
        rightMotor.setSpeed(rightSpeed);
    }

    void setMotorDirection(bool leftDirection, bool rightDirection)
    {
        leftMotor.setDirection(leftDirection);
        rightMotor.setDirection(rightDirection);
    }

    void driveMotor(bool leftDirection, bool rightDirection)
    {
        // 모터 속도를 0으로 만들기
        setMotorSpeed(0, 0);
        // 모터 방향을 설정하기
        setMotorDirection(leftDirection, rightDirection);
        // 모터를 가동하기
        setMotorSpeed(MOTOR_SPEED, MOTOR_SPEED);
        // 가동 시간 지연
        delay(ACTIVE_PERIOD);
        // 모터 속도를 0으로 만들기
        setMotorSpeed(0, 0);
        return;
    }

public:
    Car(int i1, int i2, int i3, int i4, int leftMotorSpeed, int rightMotorSpeed)
    {
        leftMotor.setter(i1, i2, leftMotorSpeed);
        rightMotor.setter(i3, i4, rightMotorSpeed);
    }

    void moveForward(void)
    {
        driveMotor(FORWARD, FORWARD);
    }

    void moveLeft(void)
    {
        driveMotor(BACK, FORWARD);
    }

    void moveRight(void)
    {
        driveMotor(FORWARD, BACK);
    }

    pair<unsigned int, unsigned int> getMotorSpeed(void)
    {
        return make_pair(leftMotor.getSpeed(), rightMotor.getSpeed());
    }
};

class ContinuousCar : private Car 
{
    ContinuousCar(int m1, int m2, int m3, int m4, int leftMotorSpeed, int rightMotorSpeed) : Car(m1, m2, m3, m4, leftMotorSpeed, rightMotorSpeed)
    {
            
    }

    void moveStart(void)
    {
        setMotorDirection(FORWARD, FORWARD);
        setMotorSpeed(MOTOR_SPEED, MOTOR_SPEED);
    }

    void stop(void)
    {
        setMotorSpeed(0, 0);
    }

    void moveLeft(void)
    {
        pair<unsigned int, unsigned int> motorSpeed = getMotorSpeed();
        setMotorSpeed(motorSpeed.fisrt + 10, motorSpeed.second - 10);
    }

    void moveRight(void)
    {
        pair<unsigned int, unsigned int> motorSpeed = getMotorSpeed();
        setMotorSpeed(motorSpeed.fisrt - 10, motorSpeed.second + 10);
    }
};