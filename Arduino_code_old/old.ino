#include <Vector.h>
#include <PIDController.h>
#include <string.h>
#include <util/atomic.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN 150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 600 // This is the 'maximum' pulse length count (out of 4096)

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
String storage_array[10];


//*Declare class for easy to read code
class Motor
{
private:
    int RPWM,LPWM;
public:
    int speed=0;
    Motor(int rpwm,int lpwm):RPWM(rpwm),LPWM(lpwm){;}
    void set_speed(int sp)//sp range(-4095,4095)
    {
        speed=sp;
        // Serial.println(sp);
        // pwm.setPWMFreq(1600);
        if(sp>0)
        {  
            pwm.setPWM(LPWM,0,0);
            pwm.setPWM(RPWM,0,sp);
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



//Motor  motorname(RPWM,LPWM);
Motor wheel1(0,1);
Motor wheel2(2,3);
Motor rotate1(4,5);
Motor rotate2(6,7);

/*
    2 normal DC motor and 2 DC motor with encoder
    Encoder have pin (2,26) and (3,27)
*/
PIDController pos1_pid,pos2_pid;


//Stepper_motor steppermotorname(pul,dir);
Stepper_motor Stepper1(22,23);  //Lift
Stepper_motor Stepper2(24,25);  //Left
Stepper_motor Stepper3(33,34);  //Right

Servo_Hand Left_hand(8),Right_hand(9);
//  2 Servo to control hands

int encoder_pos1=0,encoder_pos2=0;
int encoder_target1=0,encoder_target2=0;

void encoder1(){if(digitalRead(26)==HIGH)encoder_pos1++;else encoder_pos1--;}
void encoder2(){if(digitalRead(27)==HIGH)encoder_pos2++;else encoder_pos2--;}

void setup()
{
    Serial.begin(115200);
    Serial.setTimeout(10);
    pwm.begin();
    pwm.setOscillatorFrequency(27000000);
    pwm.setPWMFreq(1600);
    Wire.setClock(400000);
    
    pinMode(26,INPUT_PULLUP);
    pinMode(27,INPUT_PULLUP);

    pinMode(2,INPUT_PULLUP);
    pinMode(3,INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(2), encoder1, RISING);
    attachInterrupt(digitalPinToInterrupt(3), encoder2, RISING);

    pos1_pid.begin();
    pos1_pid.tune(200,0.00,0);
    pos1_pid.limit(-4095,4095);

    pos2_pid.begin();
    pos2_pid.tune(200,0.00,0);
    pos2_pid.limit(-4095,4095);

    for(int i=0;i<16;i++)
    {
        pwm.setPWM(i,0,0);
    }

    delay(1000);
}

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



void loop()
{
    
    String command = Serial.readStringUntil('\n');
    command.trim();
    Serial.println(command); 

    Vector<String>  cmds = split_string(command);
    /*
    DC <index> <forward/backward>  <speed>  #control DC motor
    DC_encoder <index> <angle>
    Stepper <index> <forward/backward>  <step> #control stepper motor
    Servo <index> <angle> #control servo
    */
    int sz=cmds.size();
    // Serial.println(cmds.size());
    // Serial.println(sz);
    if(sz!=0)
    {
        Serial.println("It finally works");
    if(cmds[0]=="DC")
    {
        Serial.println("DC motor control");
        if(sz!=4)
        {
            Serial.println("Command error");
        }
        else
        {
            if(cmds[1]=="1")
            {
                if(cmds[2]=="Forward")
                wheel1.set_speed(string_to_int(cmds[3]));
                else wheel1.set_speed(-string_to_int(cmds[3]));
            }
            else if(cmds[1]=="2")
            {
                if(cmds[2]=="Forward")
                wheel2.set_speed(string_to_int(cmds[3]));
                else wheel2.set_speed(-string_to_int(cmds[3]));
            }
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
            if(cmds[1]=="1")
            {
                encoder_target1=string_to_int(cmds[2]);
                pos1_pid.setpoint(-encoder_target1);
            }
            else if(cmds[1]=="2")
            {
                encoder_target2=string_to_int(cmds[2]);
                pos2_pid.setpoint(-encoder_target2);
            }
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
            if(cmds[1]=="1")
            {
                if(cmds[3]=="Forward")
                Stepper1.step(string_to_int(cmds[2]),true);
                else 
                Stepper1.step(string_to_int(cmds[2]),false);

            }
            else if(cmds[1]=="2")
            {
                if(cmds[3]=="Forward")
                Stepper2.step(string_to_int(cmds[2]),true);
                else 
                Stepper2.step(string_to_int(cmds[2]),false);
            }
            else if(cmds[1]=="3")
            {
                if(cmds[3]=="Forward")
                Stepper3.step(string_to_int(cmds[2]),true);
                else 
                Stepper3.step(string_to_int(cmds[2]),false);
            }
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
    int motor_encoder1_power=pos1_pid.compute(encoder_pos1);
    int motor_encoder2_power=-pos2_pid.compute(encoder_pos2);

    // rotate1.set_speed(motor_encoder1_power);
    // rotate2.set_speed(motor_encoder2_power);

    Serial.print(wheel1.speed);Serial.print("\t");
    Serial.print(wheel2.speed);Serial.print("\t");
    Serial.print(rotate1.speed);Serial.print("\t");
    Serial.print(rotate2.speed);Serial.print("\t");
    Serial.print(encoder_pos1);Serial.print("\t");
    Serial.print(encoder_pos2);Serial.print("\t");
    Serial.print(encoder_target1);Serial.print("\t");
    Serial.print(encoder_target2);Serial.print("\t");

}