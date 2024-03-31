#include<string.h>
#include<Servo.h>
using namespace std;
/*

    Function code name

M: set speed of movement motor 0-255 for 4 motors
S: Stop all movement motor
G: Hand grip
U: Up
D: Down
s: set speed max
*/

int speedmax=100;
int speedA=0,speedB=0,speedC=0,speedD=0;

//setpin

//motor a:
int RPWMA=2;
int LPWMA=3;
//motor b:
int RPWMB=4;
int LPWMB=5;
//motor c:
int RPWMC=6;
int LPWMC=7;
//motor d:
int RPWMD=8;
int LPWMD=9;

Servo rightservo;
int rightpos=45;
Servo leftservo;
int leftpos=45;

#define stepsPerRevolution 4800
int steppin1=22;
int dirpin1=23;
int steppin2=26;
int dirpin2=27;
int speeddelay=100;

void printvalue()
{
    Serial.print(speedA);Serial.print("\t");
    Serial.print(speedB);Serial.print("\t");
    Serial.print(speedC);Serial.print("\t");
    Serial.print(speedD);Serial.print("\t");
    Serial.print(leftpos);Serial.print("\t");
    Serial.print(rightpos);Serial.print("\t");

    Serial.println("");

    return;
}

void stopmovement()
{
    speedA=0;
    speedB=0;
    speedC=0;
    speedD=0;
    return;
}

void drive_motor(int a,int b,int speedx)// a = RPWM b= LPWM
{
    if(speedx==0)
    {
        analogWrite(a,0);
        analogWrite(b,0);
    }
    else if(speedx>0)
    {
        analogWrite(a,0);
        analogWrite(b,speedx);
    }
    else {
        analogWrite(a,-speedx);
        analogWrite(b,0);
    }
}





void update_all_signal()
{

    drive_motor(RPWMA,LPWMA,-speedA); // A
    drive_motor(RPWMB,LPWMB,speedB); // B
    drive_motor(RPWMC,LPWMC,-speedC); // C
    drive_motor(RPWMD,LPWMD,speedD); // D
    leftservo.write(leftpos);
    rightservo.write(rightpos);

    return;
}

//===========================================================================================

void setup()
{
    Serial.begin(115200);
    pinMode(LPWMA,OUTPUT);
    pinMode(RPWMA,OUTPUT);
    pinMode(LPWMB,OUTPUT);
    pinMode(RPWMB,OUTPUT);
    pinMode(LPWMC,OUTPUT);
    pinMode(RPWMC,OUTPUT);
    pinMode(LPWMD,OUTPUT);
    pinMode(RPWMD,OUTPUT);

    pinMode(dirpin1,OUTPUT);
    pinMode(dirpin2,OUTPUT);
    pinMode(steppin1,OUTPUT);
    pinMode(steppin2,OUTPUT);


    leftservo.attach(A1);
    rightservo.attach(A2);
    leftservo.write(90); 
    rightservo.write(90); 

}

void loop()
{   
    // printvalue();
    String s = Serial.readStringUntil('\n');
    Serial.println(s);
    if(s.length()==0){}
    else if(s[0]=='S')
    {
        stopmovement();
    }else if(s[0]=='M')
    {
        {
        //speed 0-510
        int speedtmp[4]={0,0,0,0},ind=1;
        for(int i=0;i<4;i++)
        {
            int tmp=0;
            for(int j=0;j<3;j++)
            {
                tmp*=10;
                tmp+=s[ind]-'0';
                ind++;
            }
            speedtmp[i]=tmp;

        }
        speedA=map(speedtmp[0]-255,-255,255,-speedmax,speedmax);
        speedB=map(speedtmp[1]-255,-255,255,-speedmax,speedmax);
        speedC=map(speedtmp[2]-255,-255,255,-speedmax,speedmax);
        speedD=map(speedtmp[3]-255,-255,255,-speedmax,speedmax);
        }
    }
    else if(s[0]=='G')
    {
        int gp=60;
        if(s.length()!=2){Serial.println("No command");}
        else{
        if(s[1]=='1')
        {
            leftpos=min(105,leftpos+gp);
            leftservo.write(leftpos);
        }else
        if(s[1]=='2')
        {
            leftpos=max(45,leftpos-gp);
            leftservo.write(leftpos);
        }else
        if(s[1]=='3')
        {
            rightpos=min(105,rightpos+gp);
            rightservo.write(rightpos);
        }else 
        if(s[1]=='4')
        {
            rightpos=max(45,rightpos-gp);
            rightservo.write(rightpos);
        }
        }        
    }
    else if(s[0]=='U')
    {
        Serial.println("Going up");
        digitalWrite(dirpin1,LOW);
        digitalWrite(dirpin2,LOW);
        for (int i=0; i < stepsPerRevolution/2; i++) {
        // These four lines result in 1 step:
        digitalWrite(steppin1, HIGH);
        digitalWrite(steppin2, HIGH);
        delayMicroseconds(speeddelay);
        digitalWrite(steppin1, LOW);
        digitalWrite(steppin2, LOW);
        delayMicroseconds(speeddelay);
    }
    }
    else if(s[0]=='D')
    {
        Serial.println("Going down");
        digitalWrite(dirpin1,HIGH);
        digitalWrite(dirpin2,HIGH);
        for (int i=0; i < stepsPerRevolution/2; i++) {
        // These four lines result in 1 step:
        digitalWrite(steppin1, HIGH);
        digitalWrite(steppin2, HIGH);
        delayMicroseconds(speeddelay);
        digitalWrite(steppin1, LOW);
        digitalWrite(steppin2, LOW);
        delayMicroseconds(speeddelay);
    }
    }

    update_all_signal();

    printvalue();
    return;
}

