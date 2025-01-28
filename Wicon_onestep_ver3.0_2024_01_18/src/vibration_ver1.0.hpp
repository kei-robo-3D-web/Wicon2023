#include <Arduino.h>
class vibration
{
private:

public:
    uint8_t vibMotorPin = 0;
    void declaration(const uint8_t);
    void pattern1();
    void pattern2();
    void pattern3();
    void pattern4();
    void test();
};

void vibration::declaration(uint8_t vibMotor){
    vibMotorPin = vibMotor;
    pinMode(vibMotorPin,OUTPUT);
}

void vibration::pattern1(){
    digitalWrite(vibMotorPin,HIGH);
    delay(1000);
    digitalWrite(vibMotorPin,LOW);
    delay(1000);
}

void vibration::pattern2(){
    digitalWrite(vibMotorPin,HIGH);
    delay(500);
    digitalWrite(vibMotorPin,LOW);
    delay(1000);
}

void vibration::pattern3(){
    digitalWrite(vibMotorPin,HIGH);
    delay(3000);
    digitalWrite(vibMotorPin,LOW);
    delay(3000);
}

void vibration::pattern4(){
    digitalWrite(vibMotorPin,HIGH);
    delay(500);
    digitalWrite(vibMotorPin,LOW);
    delay(500);
    digitalWrite(vibMotorPin,HIGH);
    delay(500);
    digitalWrite(vibMotorPin,LOW);
    delay(500);
}

void vibration::test(){
    digitalWrite(vibMotorPin,HIGH);
    delay(1000);
    digitalWrite(vibMotorPin,LOW);
    delay(1000);
    digitalWrite(vibMotorPin,HIGH);
    delay(500);
    digitalWrite(vibMotorPin,LOW);
    delay(500);
}
