#include <SoftwareSerial.h>
#include "Vehicle.ino"

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
Car *vehicle;

void setup()
{
    for (int p : MOTOR_PINS)
        pinMode(p, OUTPUT);

    btSerial = new SoftwareSerial(BT_RXD, BT_TXD);
    vehicle = new Car(IN1, IN2, IN3, IN4, ENA, ENB);

    Serial.begin(9600);
    btSerial.begin(9600);
}

void loop()
{
    delay(SLEEP_PERIOD);

    if (btSerial.available())
    {
        int direction = btSerial.read();

        Serial.print("Command: '");
        Serial.write(direction);
        Serial.print("'. ");

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

    while (btSerial.available() > 0) // btSerial 버퍼를 모두 비움
        btSerial.read();
}