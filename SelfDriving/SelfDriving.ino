#include <SoftwareSerial.h>
#include "Vehicle.h"

#define BT_RXD 13
#define BT_TXD 12
#define ENA 5
#define IN1 6
#define IN2 7
#define IN3 8
#define IN4 9
#define ENB 10
const int MOTOR_PINS[] = {IN1, IN2, IN3, IN4, ENA, ENB};

SoftwareSerial *btSerial;
ContinuousCar *vehicle;

void setup()
{
    for (int p : MOTOR_PINS)
        pinMode(p, OUTPUT);

    btSerial = new SoftwareSerial(BT_RXD, BT_TXD);
    vehicle = new ContinuousCar(IN1, IN2, IN3, IN4, ENA, ENB);
    vehicle->moveStart();

    Serial.begin(9600);
    btSerial.begin(9600);
}

void loop()
{
    int awaitCommandDeliveryPeriodAfterBufferClearing = 10; //너무 짧을 경우 버퍼 초기화 후 새로운 명령이 전달되지 않을 가능성이 있음.(ms)
    delay(SLEEP_PERIOD - awaitCommandDeliveryPeriodAfterBufferClearing);
    
    while (btSerial.available() > 0) //btSerial 버퍼를 모두 비움
        btSerial.read();

    delay(awaitCommandDeliveryPeriodAfterBufferClearing)

    if (btSerial.available())
    {
        int direction = btSerial.read();

        switch (direction)
        {
        case '1':
            Serial.println("Go Right.");
            vehicle.moveRight();
            break;
        case '2':
            Serial.println("Go Left.");
            vehicle.moveLeft();
            break;
        default:
            Serial.println("Stop.");
            vehicle.stop();
            break;
        }
    }
}