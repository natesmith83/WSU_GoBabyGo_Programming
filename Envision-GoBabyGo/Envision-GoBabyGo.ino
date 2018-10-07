//Wichita State University GoBabyGo code
//*For Public Use* 


#include "DualVNH5019MotorShield.h"
DualVNH5019MotorShield md;


int line_sensor = 5;                    //line hunter (center-line sensor)

int Forward = A2;                      //'forward' button on joystick, forward movement
int Left_Forward = A3;                 //'left' button on joystick, left_forward movement
int Right_Forward = A4;                //'right' button on joystick, right_forward movement
int Reverse = A5;                      //'reverse' button on joystick, reverse movement


int check = 0;
int j = 0;
int i = 0;

int turned_left = 0;                //indicates if moved left
int turned_right = 0;               //indicates if moved right
int line_sensor_value = 0;          //value comes from line hunter
bool is_stopped = false;

void _mForward()                    //Move Forward
{
  Serial.println("before loop");

 for( i = 0; i <= 230 ; i+=1)       //Start ramping speed                                                                             
  {
    if(analogRead(Forward) < 950 && analogRead(Left_Forward) < 950 && analogRead(Right_Forward) < 950)
    {
      break;
    }

    if(i > 45) {
      md.setM1Speed(i);
      delay(11);
    }

    if(i == 230)    //If at Max speed, continue at Max speed
    {
      i = 229;
    }
   }

}

void _mReverse()                    //Move Reverse
{
    Serial.println("before loop");
  
  for( i = 0; i <= 230; i+=1)
    {
      if(analogRead(Reverse) < 950)
      {  
        break;
      }

      if(i > 45) {
        //j=1;
        md.setM1Speed(-1*i);
        delay(11);
      }

      if(i == 230)
      {

        i = 229;
      }
    }

}


void _mLeft()                     //Move Left and Forward
{
  md.setM2Speed(-250);             //constant speed from datasheet
  Serial.println("go left!");
  _mForward();
}

void _mRight()                    //Move Right and Forward
{
  md.setM2Speed(250);              //constant speed from datasheet
  Serial.println("go right!");
  _mForward();
}

void _mStop()                     //Move Stop
{
  md.setM1Speed(0);               //Zero speed from datasheet
  Serial.println("STOP!");
}

void _mReturn()                   //returns steering wheels to zero (center)
{
 if(turned_left > 0)                                    //return to center by turning right
 {
  Serial.println("returning from left");              
  _mStop();
  line_sensor_value = digitalRead(line_sensor);
  while(line_sensor_value >0)
  {
    md.setM2Speed(250);
    line_sensor_value = digitalRead(line_sensor);
  }
  md.setM2Speed(0);
  turned_left = 0;
  Serial.println("returned");
 }
 if(turned_right > 0)                                  // return to center by turning left
 {
  _mStop();
  line_sensor_value = digitalRead(line_sensor);
  Serial.println("returning from right");
  while(line_sensor_value >0)
  {
    md.setM2Speed(-250);
    line_sensor_value = digitalRead(line_sensor);
  }
  md.setM2Speed(0);
  turned_right=0;
  Serial.println("returned");
 }
}
void setup()
{
  pinMode(Forward,INPUT);
  pinMode(Left_Forward,INPUT);
  pinMode(Right_Forward,INPUT);
  pinMode(Reverse,INPUT);
  pinMode(line_sensor,INPUT);

  md.init();
  Serial.begin(9600);
  _mStop();
}

void loop()
{
 
  int HB = analogRead(Forward);                 //reads value from joystick forward button
  int HL = analogRead(Left_Forward);            //reads value from joystick left button
  int HR = analogRead(Right_Forward);           //reads value joystick right button
  int REV = analogRead(Reverse);                //reads value from joystick reverse button

  Serial.println(HB);


  //checks to see if the turning happened
 
   
   if (HB > 950)                        //manual mode //forward movement
  {
    _mForward();
    Serial.println("---------HB---------");Serial.println(HB);
  }
  else if(HL>950)                       //for left movement
  {
    _mLeft();
    turned_left = 1;
    _mReturn();
  }
  else if(HR>950)                      //for right movement
  {
    _mRight();
    turned_right = 1;
    _mReturn();
  }
  else if(REV > 950)                   //for reverse movement
  {

    _mReverse();
    
  }
  else if(HB < 900 && HL < 900 && HR < 900) //to stop
  {
    _mStop();
  }
}

