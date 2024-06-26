#include <Vector.h>
#include <PIDController.h>
#include <string.h>
#include <util/atomic.h>
#include <Wire.h>
#include <Servo.h>


String storage_array[10];
bool update_make=false;

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
        if(speed_now!=speed)
        update_make=true;
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
        if(speed!=speed_now) 
        update_make=true;
        speed_now=speed;
        if (speed == 0)
        {
            analogWrite(PWM, 0);
            digitalWrite(DIR, LOW);
        }
        else if (speed > 0)
        {
            analogWrite(PWM, speed);
            digitalWrite(DIR, HIGH);
        }
        else
        {
            analogWrite(PWM, -speed);
            digitalWrite(DIR, LOW);
        }
    }
};
class Stepper_motor
{
private:
    int pul,dir,speeddelay=500;
public:
    Stepper_motor(int pul,int dir):pul(pul),dir(dir){
        pinMode(pul,OUTPUT);
        pinMode(dir,OUTPUT);
    }
    void step(int steps,bool direction)
    {
        update_make=true;
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
    void set_speed_delay(int new_speed)
    {
        if(new_speed<=0||new_speed>=10000)
        {
            Serial.println("Speed delay out of range");
            return;
        }
        speeddelay=new_speed;
        return;
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


// PIDController pos1_pid,pos2_pid;

// int encoder_pos1=0,encoder_pos2=0;
// int encoder_target1=0,encoder_target2=0;

MCT_HB_40A_H_Bridge wheel3(4,41,42);
MCT_HB_40A_H_Bridge wheel4(5,26,27);

Cytron_20a_motor_driver wheel1(6,29);
Cytron_20a_motor_driver wheel2(7,46);
Cytron_20a_motor_driver back_side(13,33);

Stepper_motor Stepper1(22,23);//stepper for lifting
Stepper_motor Stepper2(24,25);//stepper for control hand position

//Servo index 1 to 6 respectively
Servo ball_catcher_left; 
Servo ball_catcher_right;
Servo hand1;
Servo hand2;
Servo hand3;
Servo hand4;
//Servo desire angle
int ball_catcher_left_pos=0;
int ball_catcher_right_pos=180;
int hand1_pos=0;
int hand2_pos=60;
int hand3_pos=0;
int hand4_pos=0;
// void encoder1(){update_make=true; if(digitalRead(26)==HIGH)encoder_pos1++;else encoder_pos1--;}
// void encoder2(){update_make=true; if(digitalRead(27)==HIGH)encoder_pos2++;else encoder_pos2--;}

int Switch1=HIGH;
int Switch2=HIGH;


void setup()
{
    Serial.begin(115200);
    // Serial.setTimeout(5);

    // pinMode(26,INPUT_PULLUP);
    // pinMode(27,INPUT_PULLUP);

    // pinMode(2,INPUT_PULLUP);
    // pinMode(3,INPUT_PULLUP);
    // attachInterrupt(digitalPinToInterrupt(2), encoder1, RISING);
    // attachInterrupt(digitalPinToInterrupt(3), encoder2, RISING);

    //Switch
    pinMode(35,OUTPUT);
    pinMode(36,OUTPUT);

    digitalWrite(35,Switch1);
    digitalWrite(36,Switch2);

    // pos1_pid.begin();
    // pos1_pid.tune(25,0,0);
    // pos1_pid.limit(-150,150);

    // pos2_pid.begin();
    // pos2_pid.tune(25,0,0);
    // pos2_pid.limit(-150,150);

	ball_catcher_left.attach(44);
	ball_catcher_right.attach(45);
	hand1.attach(9);
	hand2.attach(10);
	hand3.attach(11);
	hand4.attach(12);
    ball_catcher_left.write(ball_catcher_left_pos);
    ball_catcher_right.write(ball_catcher_right_pos);
    hand1.write(hand1_pos);
    hand2.write(hand2_pos);
    hand3.write(hand3_pos);
    hand4.write(hand4_pos);
    //Stepper2.set_speed_delay(2000);
    Stepper1.set_speed_delay(200);
    update_make=true;
    delay(1000);
}



void loop()
{
    String command = Serial.readStringUntil('\n');
    
    command.trim();
    if(command.length()!=0)
    Serial.println(command); 

    Vector<String>  cmds = split_string(command);
    /*
    DC <index> <forward/backward>  <speed>  #control DC motor
    DC_encoder <index> <angle>
    Stepper <index> <step> <forward/backward>  #control stepper motor
    Servo <index> <angle> #control servo
    Switch <index> #toggle switch 
    Tune <index> <step> #tune motor encoder
    */
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
                wheel1.set_speed(-string_to_int(cmds[3])*10/13);
                else wheel1.set_speed(string_to_int(cmds[3])*10/13);
            }
            else if(cmds[1]=="2")
            {
                if(cmds[2]=="Forward")
                wheel2.set_speed(-string_to_int(cmds[3])*10/13);
                else wheel2.set_speed(string_to_int(cmds[3])*10/13);
            }
            else if(cmds[1]=="3")
            {
                if(cmds[2]=="Forward")
                wheel3.set_speed(-string_to_int(cmds[3]));
                else wheel3.set_speed(string_to_int(cmds[3]));
            }
            else if(cmds[1]=="4")
            {
                if(cmds[2]=="Forward")
                wheel4.set_speed(-string_to_int(cmds[3]));
                else wheel4.set_speed(string_to_int(cmds[3]));
            }
            else if(cmds[1]=="5")
            {
                if(cmds[2]=="Forward")
                back_side.set_speed(-string_to_int(cmds[3]));
                else back_side.set_speed(string_to_int(cmds[3]));
            }
            ;
        }
    }
    // else if(cmds[0]=="DC_encoder")
    // {
    //     Serial.println("DC_encoder");
    //     if(sz!=3)
    //     {
    //         Serial.println("Command error");
    //     }
    //     else
    //     {
    //         if(cmds[1]=="1")
    //         {
    //             encoder_target1=string_to_int(cmds[2]);
    //             if(abs(encoder_target1-encoder_pos1)<abs(encoder_target1+1200-encoder_pos1) &&abs(encoder_target1-encoder_pos1)<abs(encoder_target1-1200-encoder_pos1))
    //             pos1_pid.setpoint(encoder_target1);
    //             else if(abs(encoder_target1+1200-encoder_pos1)<abs(encoder_target1-encoder_pos1) && abs(encoder_target1+1200-encoder_pos1)<abs(encoder_target1-1200-encoder_pos1))
    //             {
    //                 pos1_pid.setpoint(encoder_target1+1200);
    //                 encoder_target1+=1200;
    //             }
    //             else 
    //             {
    //                 pos1_pid.setpoint(encoder_target1-1200);
    //                 encoder_target1-=1200;
    //             }

    //         }
    //         else if(cmds[1]=="2")
    //         {
    //             encoder_target2=string_to_int(cmds[2]);
    //             if(abs(encoder_target2-encoder_pos2)<abs(encoder_target2+900-encoder_pos2) &&abs(encoder_target2-encoder_pos2)<abs(encoder_target2-900-encoder_pos2))
    //             pos2_pid.setpoint(encoder_target2);
    //             else if(abs(encoder_target2+900-encoder_pos2)<abs(encoder_target2-encoder_pos2) && abs(encoder_target2+900-encoder_pos2)<abs(encoder_target2-900-encoder_pos2))
    //             {
    //                 pos2_pid.setpoint(encoder_target2+900);
    //                 encoder_target2+=900;
    //             }
    //             else 
    //             {
    //                 pos2_pid.setpoint(encoder_target2-900);
    //                 encoder_target2-=900;
    //             }
    //         }
    //         ;
    //     }
    // }

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
			if(cmds[1]=="1")
			{
				int angle_use=string_to_int(cmds[2]);
				if(angle_use<0||angle_use>180)
				{
					Serial.println("Wrong angle");
				}
				else 
                {
                    ball_catcher_left_pos=angle_use;
                    ball_catcher_left.write(angle_use);
                }
			}
            else if(cmds[1]=="2")
			{
				int angle_use=string_to_int(cmds[2]);
				if(angle_use<0||angle_use>180)
				{
					Serial.println("Wrong angle");
				}
				else
				{ 
					ball_catcher_right_pos=angle_use;
					ball_catcher_right.write(angle_use);
				}
			}
            else if(cmds[1]=="3")
			{
				int angle_use=string_to_int(cmds[2]);
				if(angle_use<0||angle_use>180)
				{
					Serial.println("Wrong angle");
				}
				else
				{
					hand1_pos=angle_use;
					hand1.write(angle_use);
				}
			}
            else if(cmds[1]=="4")
			{
				int angle_use=string_to_int(cmds[2]);
				if(angle_use<0||angle_use>180)
				{
					Serial.println("Wrong angle");
				}
				else 
				{
					hand2.write(angle_use);
					hand2_pos=angle_use;
				}
			}
            else if(cmds[1]=="5")
			{
				int angle_use=string_to_int(cmds[2]);
				if(angle_use<0||angle_use>180)
				{
					Serial.println("Wrong angle");
				}
				else 
                {
                    hand3_pos=angle_use;
                    hand3.write(angle_use);
                }
			}
            else if(cmds[1]=="6")
			{
				int angle_use=string_to_int(cmds[2]);
				if(angle_use<0||angle_use>180)
				{
					Serial.println("Wrong angle");
				}
				else 
                {
                    hand4_pos=angle_use;
                    hand4.write(angle_use);
                }
			}
		}
    }
    else if(cmds[0]=="Switch")
    {
        Serial.println("Switch");
        if(sz!=2)
        {
            Serial.println("Command error");
        }
        else
        {
            if(cmds[1]=="1")
            {
                Switch1^=1;
                digitalWrite(35,Switch1);
            }
            else if(cmds[1]=="2")
            {
                Switch2^=1;
                digitalWrite(36,Switch2);
            }
            ;
        }
    }
    // else if(cmds[0]=="Tune")
    // {
    //     update_make=true;
    //     Serial.println("Tune");
    //     if(sz!=3)
    //     {
    //         Serial.println("Command error");
    //     }
    //     else
    //     {
    //         if(cmds[1]=="1")
    //         {
    //             int tune_level=string_to_int(cmds[2]);
    //             if(tune_level<-100 ||tune_level>100)
    //             {
    //                 Serial.println("Command error");
    //             }
    //             else 
    //             {
    //                 encoder_pos1-=tune_level;
    //                 // encoder_target1+=tune_level;
    //                 // pos1_pid.setpoint(encoder_target1);
    //             }

    //         }
    //         else if(cmds[1]=="2")
    //         {
    //             int tune_level=string_to_int(cmds[2]);
    //             if(tune_level<-100 ||tune_level>100)
    //             {
    //                 Serial.println("Command error");
    //             }
    //             else 
    //             {
    //                 encoder_pos2-=tune_level;
    //                 // encoder_target2+=tune_level;
    //                 // pos2_pid.setpoint(encoder_target2);
    //             }
    //         }
    //         ;
    //     }
    // }
    }
    // int motor_encoder1_power=pos1_pid.compute(encoder_pos1);
    // int motor_encoder2_power=-pos2_pid.compute(encoder_pos2);
	
	hand1.write(hand1_pos);
    hand2.write(hand2_pos);
    hand3.write(hand3_pos);
    hand4.write(hand4_pos);
    ball_catcher_left.write(ball_catcher_left_pos);
    ball_catcher_right.write(ball_catcher_right_pos);


    
    if(update_make){
    Serial.print("Motor ");
    Serial.print(wheel1.speed_now);Serial.print("\t");
    Serial.print(wheel2.speed_now);Serial.print("\t");
    Serial.print(wheel3.speed_now);Serial.print("\t");
    Serial.print(wheel4.speed_now);Serial.print("\t");
    Serial.print(back_side.speed_now);Serial.print("\t");
    
    Serial.print("Servo ");
    Serial.print(ball_catcher_left.read());Serial.print("\t");
    Serial.print(ball_catcher_right.read());Serial.print("\t");
    Serial.print(hand1.read());Serial.print("\t");
    Serial.print(hand2.read());Serial.print("\t");
    Serial.print(hand3.read());Serial.print("\t");
    Serial.print(hand4.read());Serial.print("\t");

    // Serial.println("");
    // Serial.println("");
    update_make=false;
    }
    
    return;
}
