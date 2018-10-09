//Nathan Smith, April 2016, GoBabyGo code 

  const int pwm = 3 ;  //initializing pin 3 as pwm
 const int in_a = 8 ;
 const int in_b = 9 ;
   int steer_link_Pot = analogRead(A0); //Reads value of steering linkage pot
   int joystick_steerPot = analogRead(A1); //Reads value of josticks H pot
   const int right_threshold = 700; //Sets beginning threshold
   const int left_threshold = 400; //Sets beginning threshold
   const int pot_thresholdMin = 355; //Sets stop limits
   const int pot_thresholdMax = 733; //Sets stop limits
   const int steerpot_center = 516; //provides a center point to return to
   const int steering_deadband = 30;
   int center_window = 0;
   int steerpot_window = 0;
void setup()
{
   // initialize serial communication at 9600 bits per second:
   Serial.begin(9600);
   
}
void loop()
{
steer_link_Pot = analogRead(A0);
joystick_steerPot = analogRead(A1);
delay (100);

//For Righthand or CW motion: in_a = High , in_b = Low
if ((joystick_steerPot > right_threshold)&&(steer_link_Pot < pot_thresholdMax))  {
digitalWrite(in_a,HIGH) ;
digitalWrite(in_b,LOW) ;
analogWrite(pwm,100) ;
Serial.println("Turn Right");
}

//For Lefthand or CCW motion: in_a = LOW , in_b = HIGH
if ((joystick_steerPot < left_threshold)&&(steer_link_Pot > pot_thresholdMin))  {
digitalWrite(in_a,LOW) ;
 digitalWrite(in_b,HIGH) ;
 analogWrite(pwm,100) ; 
Serial.println("Turn Left");
 
}

//States that if joystick value has not met its threshold, then do nothing
if ((joystick_steerPot > left_threshold)&(joystick_steerPot < right_threshold)) 
{
  digitalWrite(in_a,LOW) ;
 digitalWrite(in_b,LOW) ;
 analogWrite(pwm,255) ;
 Serial.println("Stop Turning 1");

}

//States that if the steering linkage potentiometer meets its limit, then do nothing (keeps it from turning too far in either direction)
else if ((steer_link_Pot < pot_thresholdMin)||(steer_link_Pot > pot_thresholdMax))
 {
  digitalWrite(in_a,LOW) ;
 digitalWrite(in_b,LOW) ;
 analogWrite(pwm,255) ;
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
      digitalWrite(in_a,LOW) ;
     digitalWrite(in_b,HIGH) ;
     analogWrite(pwm,100) ;
      Serial.println("Turn Left to Center");
    
    }
    if (steer_link_Pot < steerpot_center - steering_deadband)   // - steering_deadband
      {
        steerpot_window = 0;
        digitalWrite(in_a,HIGH) ;
       digitalWrite(in_b,LOW) ;
       analogWrite(pwm,100) ;
         Serial.println("Turn Right to Center");
      }
    if (steerpot_window == 1){
        digitalWrite(in_a,LOW) ;
     digitalWrite(in_b,LOW) ;
     analogWrite(pwm,100) ; 
       Serial.println("Stop Centered");
    }
  }
  

 // prints the analog values on the serial monitor:
   Serial.print("Pot Values  ");
   Serial.print(steer_link_Pot);
     Serial.print(" ");
   Serial.println(joystick_steerPot);
   //Serial.println(joystick_drivePot);
   delay(100);        // delay between reads for stability
 }
