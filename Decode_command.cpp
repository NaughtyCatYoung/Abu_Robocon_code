#include<bits/stdc++.h>
using namespace std;


/*
DC <index> <forward/backward>  <speed>  #control DC motor
DC_encoder <index> <angle>
Stepper <index> <forward/backward>  <step> #control stepper motor
Servo <index> <angle> #control servo
*/
vector<string> split_string(string &str) {
    vector<string> ret={};
    int len=str.length();
    int ind=0;
    string word="";
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





int main()
{
    string command;
    getline(cin,command);
    vector<string>  cmds = split_string(command);
    int sz=cmds.size();
    // cout<<sz<<"\n";
    if(sz==0)
    {
        return 0;
    }
    if(cmds[0]=="DC")
    {
        cout<<"DC motor control\n";
        if(sz!=4)
        {
            cout<<"Command error";
        }
        else
        {
            cout<<"Motor : "<<stoi(cmds[1])<<"\n";
            cout<<"Direction : "<<((cmds[2]=="Forward")? false:true )<<"\n";
            cout<<"Speed : "<<stoi(cmds[3]); 
        }
    }
    else if(cmds[0]=="DC_encoder")
    {
        cout<<"Control motor with encoder\n";
        if(sz!=3)
        {
            cout<<"Command error";
        }
        else
        {
            cout<<"Motor : "<<stoi(cmds[1])<<"\n";
            cout<<"Angle : "<<stoi(cmds[2]);
        }
    }

    else if(cmds[0]=="Stepper")
    {
        cout<<"Stepper motor control\n";
        if(sz!=4)
        {
            cout<<"Command error";
        }
        else
        {
            cout<<"Stepper : "<<stoi(cmds[1])<<"\n";
            cout<<"Direction : "<<((cmds[2]=="Forward")? false:true )<<"\n";
            cout<<"Step : "<<stoi(cmds[3]); 
        }
    }
    else if(cmds[0]=="DC_encoder")
    {
        cout<<"Control servo\n";
        if(sz!=3)
        {
            cout<<"Command error";
        }
        else
        {
            cout<<"Servo : "<<stoi(cmds[1])<<"\n";
            cout<<"Angle : "<<stoi(cmds[2]);
        }
    }

    return 0;
}