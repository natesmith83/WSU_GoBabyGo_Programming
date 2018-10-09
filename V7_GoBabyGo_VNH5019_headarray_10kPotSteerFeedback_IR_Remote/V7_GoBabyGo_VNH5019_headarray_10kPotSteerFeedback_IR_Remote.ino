//Wichita State University GoBabyGo code
//Author: Satya V P K Maddukuri: Volunteering the WSU-Go Baby Go bulid & representing IEEE-HKN
//Co-Author1: Nathan Smith : Advising ENGR 202 students for their GoBabyGo build.
//Co-Author2: Tom Mc Guire.
//Using a custom head set (Forward/Left/Right)
//Using an IR Remote (Forward/Left/Right)
//Using a DC motor with a custom built steering linkage and a 10K OHM potentiometer (for feedback) to control the steering.
//*For Public Use* 

#include "DualVNH5019MotorShield.h"
DualVNH5019MotorShield md;

int receiverpin_A = 3;                //'A' button in remote, stop remote
int receiverpin_B = 5;                //'C' button in remote, forward moment
int receiverpin_C = 11;               //'D' button in remote, left moment
int receiverpin_D = 13;               //'B' button in remote, right moment 

int headbutton_back = A2;             //'back' button near head, forward remote
int headbutton_left = A3;             //'left' button near head, left moment
int headbutton_right = A4;            //'right' button near head, right moment

int steer_Pot = A5;                   //steering pot
int check=0;  
const int numReadings = 10;           //pot smooting
int readings[numReadings];            // the readings from the analog input     //pot smooting
int readIndex = 0;                    // the index of the current reading    //pot smooting
int total = 0;                        // the running total    //pot smooting
int average = 0;                      // the average    //pot smooting
int j=0;
int i=0;


const int pot_thresholdMin = 285;     //Sets steering stop limits //275 actual left max
const int pot_thresholdMax = 546;     //Sets steering stop limits //556 actual right max
const int steerpot_center = 440;      //provides a center point to return to
const int steering_deadband = 10;     //deadband to account for sensitivity
int steerpot_window = 0;

  void _mForward()                    //Move Forward
  {
  Serial.println("before loop"); 
  int i;   
  if(j==0)
  {
   for (i = 0; i <= 150; i+=1)
  {
    if(i>45) {j=1;}
    md.setM1Speed(i); 
    delay(11);
  }
  }
  if (j==1)
  {
   md.setM1Speed(150);
  }
  
  Serial.println("after loop!");
  }

  void _mLeft()                       //Move Left
  {
   md.setM2Speed(-60);                //constant speed from datasheet
   Serial.println("go left!");
   _mForward();
  }

  void _mRight()                      //Move Right
  {
  md.setM2Speed(60);                //constant speed from datasheet
  Serial.println("go right!");
  _mForward();
  }

  void _mStop()                       //Move Stop
  {
  md.setM1Speed(0);                //Zero speed from datasheet
  Serial.println("STOP!");
  }

void setup()
{
pinMode(receiverpin_A,INPUT);             //Input Pins
pinMode(receiverpin_B,INPUT);
pinMode(receiverpin_C,INPUT);
pinMode(receiverpin_D,INPUT);
pinMode(steer_Pot,INPUT);
pinMode(headbutton_back,INPUT);
pinMode(headbutton_left,INPUT);
pinMode(headbutton_right,INPUT);

md.init();
Serial.begin(115200);
_mStop();

 for (int thisReading = 0; thisReading < numReadings; thisReading++)    //pot smooting
 {
 readings[thisReading] = 0;
 }
}

void loop() 
{
int A=digitalRead(receiverpin_A);       //Reads value from D0
int B=digitalRead(receiverpin_B);       //Reads value from D1
int C=digitalRead(receiverpin_C);       //Reads value from D2
int D=digitalRead(receiverpin_D);       //Reads value from D3
int SPot=analogRead(steer_Pot);           //Reads value from 10K pot attached to the steering linkage
int HB=analogRead(headbutton_back);     //Reads value from head back button
int HL=analogRead(headbutton_left);     //Reads value from head left button
int HR=analogRead(headbutton_right);    //Reads value from head right button

average=SPot;
//Remote activation  
  if(B==1 || C==1 || D==1)                //a dummy variable for remote loop usage
  {
    check=1;
    Serial.println("check active!");
  }
    if(check==1)
    {
    if(A==1)                              //To come out of remote loop
    {
    check=0;
    Serial.println("check IN-active!");  
    }
  
    else if(C==1)                         //For Forward movement
    { 
    _mForward();
    }
  
    else if((D==1) && (average > pot_thresholdMin) && (average < pot_thresholdMax))                         //For Left movement
    {
    j=0;
    _mLeft();
    }
  
    else if((B==1) && (average > pot_thresholdMin) && (average < pot_thresholdMax))                         //For Right movement
    {
    j=0;
    _mRight();
    }
  
    else if(A==0 && B==0 && C==0 && D==0) //To stop
    {
    j=0;
    _mStop();
    steerpot_window = 1;
    
    if (average < steerpot_center - steering_deadband)
    {
    steerpot_window = 0;
    md.setM1Speed(0);
    md.setM2Speed(50);                      //right
    Serial.println("go right to center!");
    }
    if (average > steerpot_center + steering_deadband)
    {
    steerpot_window = 0;
    md.setM1Speed(0);
    md.setM2Speed(-50);                     //left     
    Serial.println("go left to center!"); 
    }
    
    if (average>=steerpot_center - steering_deadband && average<=steerpot_center + steering_deadband)
    {
    md.setM1Speed(0);
    md.setM2Speed(0);
    Serial.println("Stop Centered");
    }
    }
    }

//Manual mode
  else if(HB==1023)                         //For Forward movement
    { 
    _mForward();
    Serial.println("-----------HB--------");Serial.print(HB);
    }
  
    else if((HL==1023) && (SPot > pot_thresholdMin) && (SPot < pot_thresholdMax))                         //For Left movement
    {
    _mLeft();
    }
  
    else if((HR==1023) && (SPot > pot_thresholdMin) && (SPot < pot_thresholdMax))                         //For Right movement
    {
    _mRight();
    }
  
    else if(HB<900 && HL<900 && HR<900) //To stop
    {
    _mStop();
        
    if (SPot < steerpot_center - steering_deadband)
    {
    steerpot_window = 0;
    md.setM1Speed(0);
    md.setM2Speed(50);                      //right
    Serial.println("go right to center!");
    }
    if (SPot > steerpot_center + steering_deadband)
    {
    steerpot_window = 0;
    md.setM1Speed(0);
    md.setM2Speed(-40);                     //left     
    Serial.println("go left to center!"); 
    }
    
    if (SPot>=steerpot_center - steering_deadband && SPot<=steerpot_center + steering_deadband)
    {
    md.setM1Speed(0);
    md.setM2Speed(0);
    Serial.println("Stop Centered");
    }
    }
}

