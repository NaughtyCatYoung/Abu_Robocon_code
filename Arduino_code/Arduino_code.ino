#include <Vector.h>
#include <PIDController.h>
#include<string.h>
#include <util/atomic.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
long SERVOMIN =150; // This is the 'minimum' pulse length count (out of 4096)
long SERVOMAX =600; // This is the 'maximum' pulse length count (out of 4096)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


class Motor
{
private:
    int RPWM,LPWM;
public:
    Motor(int RPWM,int LPWM):RPWM(RPWM),LPWM(LPWM){;}
    void setspeed(int sp)//sp range(0,4095)
    {
        pwm.setPWMFreq(1600);
        if(sp>0)
        {   
            pwm.setPWM(RPWM,0,sp);
            pwm.setPWM(LPWM,0,0);
        }
        else if(sp<0)
        {
            pwm.setPWM(RPWM,0,0);
            pwm.setPWM(LPWM,0,-sp);
        }
        else {
            pwm.setPWM(RPWM,0,0);
            pwm.setPWM(LPWM,0,0);
        }
    }
};

class Servo
{
private:
    int pin;
public:
    Servo(int pin):pin(pin){}
    void turn(float degree)
    {
        pwm.setPWMFreq(50);
        int pulse=map(0,180,SERVOMIN,SERVOMAX,degree);
        pwm.setPWM(pin,0,pulse);
    }
};

class Stepper_motor
{
private:
    int pul,dir,speeddelay=100;
public:
    Stepper_motor(int pul,int dir):pul(pul),dir(dir){
        pinMode(pul,OUTPUT);
        pinMode(dir,OUTPUT);
    }
    void step(int steps,bool direction)
    {
        if(direction)
        {
            Serial.println("Going up");
            digitalWrite(dir,LOW);
            for (int i=0; i < steps; i++) {
                digitalWrite(pul, HIGH);
                delayMicroseconds(speeddelay);
                digitalWrite(pul, LOW);
                delayMicroseconds(speeddelay);
            }
        }
        else
        {
            Serial.println("Going Down");
            digitalWrite(dir,HIGH);
            for (int i=0; i < steps; i++) {
                digitalWrite(pul, HIGH);
                delayMicroseconds(speeddelay);
                digitalWrite(pul, LOW);
                delayMicroseconds(speeddelay);
            }
        }
    }

};


void setup()
{
    Serial.begin(115200);
    pwm.begin();
    pwm.setOscillatorFrequency(27000000);
    pwm.setPWMFreq(1600);
    Wire.setClock(400000);
}


void loop()
{




    ;


}