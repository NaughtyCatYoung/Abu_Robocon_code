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

class Servo_Hand
{
private:
    int pin;
public:
    Servo_Hand(int pin):pin(pin){}
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
int pos1,pos2;


void encoder1()
{
    if(digitalRead(26)==HIGH)pos1++;
    else pos1--;
}
void encoder2()
{  
    if(digitalRead(27)==HIGH)pos2++;
    else pos2--; 
}

void setup()
{
    Serial.begin(115200);
    pwm.begin();
    pwm.setOscillatorFrequency(27000000);
    pwm.setPWMFreq(1600);
    Wire.setClock(400000);

    Motor wheel1(1,2),wheel2(3,4),rotate1(5,6),rotate2(7,8);
    
    attachInterrupt(digitalPinToInterrupt(2), encoder1, RISING);
    attachInterrupt(digitalPinToInterrupt(3), encoder2, RISING);

    /*
    2 normal DC motor and 2 DC motor with encoder
    Encoder have pin (2,26) and (3,27)
    */

    Stepper_motor Stepper1(22,23);
    //Just a stepper motor

    Servo_Hand Left_hand(9),Right_hand(10);
    //2 Servo to control hands



    delay(1000);
}



Vector<String> split_string(String &str) {
    Vector<String> ret={};
    int len=str.length();
    int ind=0;
    String word="";
    while(ind<len){
        if(str[ind]==' ')
        {   
            ret.push_back(word);
            word="";
        }
        else
        {
            word+=str[ind];
        }
        ind++;
    }

    if(word!="")
    {
        ret.push_back(word);
    }
    return ret;
}



void loop()
{
    String command = Serial.readStringUntil('\n');
    Vector<String>  cmds = split_string(command);

    /*
    DC <index> <forward/backward>  <speed>  #control DC motor
    DC_encoder <index> <angle>
    Stepper <index> <forward/backward>  <step> #control stepper motor
    Servo <index> <angle> #control servo
    */


    int sz=cmds.size();
    // cout<<sz<<"\n";
    if(sz==0)
    {
        return 0;
    }
    if(cmds[0]=="DC")
    {
        Serial.println("DC motor control");
        if(sz!=4)
        {
            Serial.println("Command error");
        }
        else
        {
            ;
        }
    }
    else if(cmds[0]=="DC_encoder")
    {
        Serial.println("DC_encoder");
        if(sz!=3)
        {
            Serial.println("Command error");
        }
        else
        {
            ;
        }
    }

    else if(cmds[0]=="Stepper")
    {
        Serial.println("Stepper_motor");
        if(sz!=4)
        {
            Serial.println("Command error");
        }
        else
        {
            ;
        }
    }
    else if(cmds[0]=="Servo")
    {  
        Serial.println("Servo_control");
        if(sz!=3)
        {
            Serial.println("Command error");
        }
        else
        {
            ;
        }
    }





    







}