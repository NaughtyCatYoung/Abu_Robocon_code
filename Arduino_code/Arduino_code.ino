#include <Vector.h>
#include <PIDController.h>
#include <string.h>
#include <util/atomic.h>
#include <Wire.h>

String storage_array[10];


class MCT_HB_40A_H_Bridge
{
private:
    int PWM,IN1,IN2;
    bool break_enable=false;
public:
    int speed_now=0;
    MCT_HB_40A_H_Bridge(int pwm,int in1,int in2):PWM(pwm),IN1(in1),IN2(in2){
        pinMode(PWM,OUTPUT);
        pinMode(IN1,OUTPUT);
        pinMode(IN2,OUTPUT);
    };
    void set_speed(int speed)
    {
        if (speed > 255 || speed < -255)  // Fixed the condition for out-of-range speed
        {
            Serial.println("Speed is out of range");
            return;
        }
        speed_now=speed;
        if(speed==0)
        {
            digitalWrite(IN1,LOW);
            digitalWrite(IN2,LOW);

            if(break_enable)analogWrite(PWM,255);
            else analogWrite(PWM,0);
        }
        else if(speed>0)
        {
            digitalWrite(IN1,HIGH);
            digitalWrite(IN2,LOW);
            analogWrite(PWM,speed);
        }
        else 
        {
            digitalWrite(IN1,LOW);
            digitalWrite(IN2,HIGH);
            analogWrite(PWM,-speed);
        }

        return;
    }
};  
class Cytron_20a_motor_driver
{
private:
    int PWM, DIR;
public:
    int speed_now=0;
    Cytron_20a_motor_driver(int pwm, int dir) : PWM(pwm), DIR(dir)
    {
        pinMode(PWM, OUTPUT);
        pinMode(DIR, OUTPUT);
    }

    void set_speed(int speed)
    {
        if (speed > 255 || speed < -255)  // Fixed the condition for out-of-range speed
        {
            Serial.println("Speed is out of range");
            return;
        }
        
        if (speed == 0)
        {
            analogWrite(PWM, 0);
            digitalWrite(DIR, LOW);
        }
        else if (speed > 0)
        {
            analogWrite(PWM, speed);
            digitalWrite(DIR, LOW);
        }
        else
        {
            analogWrite(PWM, -speed);
            digitalWrite(DIR, HIGH);
        }
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

Vector<String> split_string(String &str) {
    //Split string into Vector of string
    Vector<String> ret={};
    ret.setStorage(storage_array);
    int len=str.length();
    int ind=0;
    String word="";
    while(ind<len){
        if(str[ind]==' ')
        {   
            if(word.length()>0)
            {
                ret.push_back(word);
                word="";
            }
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

int string_to_int(String s)
{   
    if(s.length()==0)
    return 0;
    int ans=0;
    for(auto i : s)
    {
        if(i=='-')continue;
        ans*=10;
        ans+=i-'0';
    }
    if(s[0]=='-')ans=-ans;
    return ans;
}


PIDController pos1_pid,pos2_pid;

int encoder_pos1=0,encoder_pos2=0;
int encoder_target1=0,encoder_target2=0;

void encoder1(){if(digitalRead(26)==HIGH)encoder_pos1++;else encoder_pos1--;}
void encoder2(){if(digitalRead(27)==HIGH)encoder_pos2++;else encoder_pos2--;}

void setup()
{
    Serial.begin(115200);
    Serial.setTimeout(10);

    pinMode(26,INPUT_PULLUP);
    pinMode(27,INPUT_PULLUP);

    pinMode(2,INPUT_PULLUP);
    pinMode(3,INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(2), encoder1, RISING);
    attachInterrupt(digitalPinToInterrupt(3), encoder2, RISING);

    pos1_pid.begin();
    pos1_pid.tune(10,0.00,0);
    pos1_pid.limit(-255,255);

    pos2_pid.begin();
    pos2_pid.tune(10,0.00,0);
    pos2_pid.limit(-255,255);

    delay(1000);
}



void loop()
{
    String command = Serial.readStringUntil('\n');
    command.trim();
    Serial.println(command); 

    Vector<String>  cmds = split_string(command);

    int sz=cmds.size();
    if(sz!=0)
    {
        Serial.println("==================================");
        for (int i = 0; i < sz; i++)
        {
            Serial.println(cmds[i]);
        }
        Serial.println("==================================");
    }
    return;
}