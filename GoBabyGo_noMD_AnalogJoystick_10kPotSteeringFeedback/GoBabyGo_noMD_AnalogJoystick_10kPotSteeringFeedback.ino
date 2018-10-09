

//Nathan Smith, April 2016,  Wichita State University GoBabyGo code
//Advising ENGR 202 students for their GBG build.
//Using a joystick to control drivemotor (Forward/Reverse)
//Using a DC motor with a custom built steering linkage and a 10K OHM potentiometer (for feedback) 
//to control the steering.
//I am sure this could be alot better, however, as a noob, I am just happy that
//it works.
//*For Public Use* 

   const int m1_pwm = 3 ;  //initializing pin 3 as pwm steering motor
   const int m2_pwm = 5 ;  //initializing pin 5 as pwm drivemotor
   const int m1_dir = 8 ; //steering output
   //const int in_b = 9 ; //steering output
   const int m2_dir = 10 ; //drivemotor output 
   const int d2 = 7 ; // driver enable pin 
   int steer_link_Pot = analogRead(A0); //Reads value of steering linkage pot
   int joystick_steerPot = analogRead(A3); //Reads value of joysticks H pot
   int joystick_drivePot = analogRead (A5); //Reads value of joystick V pot
   const int right_threshold = 700; //Sets beginning threshold on joystick H pot
   const int left_threshold = 400; //Sets beginning threshold on joystick H pot
   const int forward_threshold = 750; //Sets beginning threshold on joystick V pot
   const int reverse_threshold = 400; //Sets beginning threshold on joystick V pot
   const int pot_thresholdMin = 335; //Sets steering stop limits
   const int pot_thresholdMax = 695; //Sets steering stop limits
   const int steerpot_center = 504; //provides a center point to return to
   const int steering_deadband = 35; //deadband to account for sensitivity
   int center_window = 0;
   int steerpot_window = 0;
   int accel = 0;
void setup()
{
   
   Serial.begin(9600); // initialize serial communication at 9600 bits per second
   
}

void loop()
{

steer_link_Pot = analogRead(A0); //Reads value of 10K pot attached to the steering linkage
joystick_steerPot = analogRead(A3); //Reads value of joystick H pot
joystick_drivePot = analogRead (A5); //Reads Value of joystick V pot
delay (100); //delay set for stability

 //For forward movement
 if(joystick_drivePot > forward_threshold) 
 {
  if(accel <=240) {
    accel = accel + 10 ;
  }
   //digitalWrite(m2_pwm,HIGH) ;
   digitalWrite(m2_dir,LOW) ;
   digitalWrite(d2, HIGH) ;
   analogWrite(m2_pwm,accel) ;
 }

   //For reverse movement
   if(joystick_drivePot < reverse_threshold) 
   {
    if(accel <=240) {
    accel = accel + 10 ;
    }
   //digitalWrite(m2_pwm,HIGH) ;
   digitalWrite(m2_dir,HIGH) ;
   digitalWrite(d2, HIGH);
   analogWrite(m2_pwm,accel) ; 
   }

   //For no forward/reverse movement
   if((joystick_drivePot > reverse_threshold)&&(joystick_drivePot < forward_threshold))
   {
   accel = 120;
   digitalWrite(m2_pwm,LOW) ;
   digitalWrite(m2_dir,LOW) ;
   digitalWrite(d2, HIGH) ;
//   analogWrite(m2_pwm,100) ;
   }

//For Righthand or CW motion: in_a = High , in_b = Low
if ((joystick_steerPot > right_threshold)&&(steer_link_Pot < pot_thresholdMax))  
{
//digitalWrite(m1_pwm,HIGH) ;
digitalWrite(m1_dir,LOW) ;
digitalWrite(d2, HIGH) ;
analogWrite(m1_pwm,100) ;
Serial.println("Turn Right");
}

//For Lefthand or CCW motion: in_a = LOW , in_b = HIGH
if ((joystick_steerPot < left_threshold)&&(steer_link_Pot > pot_thresholdMin))  
{
//digitalWrite(m1_pwm,HIGH) ;
digitalWrite(m1_dir,HIGH) ;
digitalWrite(d2, HIGH) ;
analogWrite(m1_pwm,100) ; 
Serial.println("Turn Left");
}

//States that if joystick value has not met its threshold, then do nothing
if ((joystick_steerPot > left_threshold)&(joystick_steerPot < right_threshold)) 
{
 digitalWrite(m1_pwm,LOW) ;
 digitalWrite(m1_dir,LOW) ;
 digitalWrite(d2, HIGH) ;
 //analogWrite(m1_pwm,255) ;
 Serial.println("Stop Turning 1");
}

//States that if the steering linkage potentiometer meets its limit, then do nothing (keeps it from turning too far in either direction)
else if ((steer_link_Pot < pot_thresholdMin)||(steer_link_Pot > pot_thresholdMax))
 {
 digitalWrite(m1_pwm,LOW) ;
 digitalWrite(m1_dir,LOW) ;
 digitalWrite(d2, HIGH) ;
 //analogWrite(m1_pwm,255) ;
 Serial.println("Stop Turning 2");
 }

//Moves steering to center
center_window = 1;
steerpot_window = 1;
if (joystick_steerPot > right_threshold)center_window = 0; 
if (joystick_steerPot < left_threshold)center_window = 0;

if (center_window == 1)
  {
    if (steer_link_Pot > steerpot_center + steering_deadband)
    {
      steerpot_window = 0;
     // digitalWrite(m1_pwm,HIGH) ;
      digitalWrite(m1_dir,LOW) ;
      digitalWrite(d2, HIGH) ;
      analogWrite(m1_pwm,100) ;
      Serial.println("Turn Left to Center");
    }
    if (steer_link_Pot < steerpot_center - steering_deadband)   // - steering_deadband
    {
       steerpot_window = 0;
       //digitalWrite(m1_pwm,HIGH) ;
       digitalWrite(m1_dir,HIGH) ;
       digitalWrite(d2, HIGH) ;
       analogWrite(m1_pwm,100) ;
       Serial.println("Turn Right to Center");
     }
    if (steerpot_window == 1)
    {
     digitalWrite(m1_pwm,LOW) ;
     digitalWrite(m1_dir,LOW) ;
     digitalWrite(d2, HIGH) ;
//     analogWrite(m1_pwm,100) ; 
     Serial.println("Stop Centered");
    }
  }
  

 // prints the analog values on the serial monitor
   Serial.print("Steering Pot Values:  ");
   Serial.print(steer_link_Pot);
     Serial.println(" ");
   Serial.print("Joystick H Values:  ");
   Serial.print(joystick_steerPot);
     Serial.println(" ");
   Serial.print("Joystick V Values:  ");
   Serial.print(joystick_drivePot);
     Serial.println(" ");
   delay(100);        // delay between reads for stability
 }
