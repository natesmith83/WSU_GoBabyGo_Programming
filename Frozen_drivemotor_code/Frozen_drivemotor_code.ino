
//Nathan Smith, April 2016, GoBabyGo code

int steer_link_Pot = analogRead(A0);
int joystick_steerPot = analogRead(A1);
int joystick_drivePot = analogRead (A2);
//int drivemotorPin = 6;
const int forward_threshold = 750;
const int reverse_threshold = 400;
const int pwm = 5 ;  //initializing pin 5 as pwm (ENB)
const int in_c = 10 ;
const int in_d = 11 ;

// the setup routine runs once when you press reset:

void setup() {
   // initialize serial communication at 9600 bits per second:
   Serial.begin(9600);
   
}

// the loop routine runs over and over again forever:
void loop() {
 steer_link_Pot = analogRead(A0);
 joystick_steerPot = analogRead(A1);
 joystick_drivePot = analogRead (A2);
 delay (250);

   if(joystick_drivePot > forward_threshold) {
   digitalWrite(in_c,HIGH) ;
   digitalWrite(in_d,LOW) ;
   analogWrite(pwm,255) ;
   }

   if(joystick_drivePot < reverse_threshold) {
   digitalWrite(in_c,LOW) ;
   digitalWrite(in_d,HIGH) ;
   analogWrite(pwm,255) ; 
   }

   if((joystick_drivePot > reverse_threshold)&&(joystick_drivePot < forward_threshold)){
   digitalWrite(in_c,LOW) ;
   digitalWrite(in_d,LOW) ;
   analogWrite(pwm,100) ;
   }
   
   // digitalWrite(drivemotorPin, HIGH);
   //} else {
    //digitalWrite(drivemotorPin, LOW); 
   //}
   //}
   
   // print out the value you read:
   Serial.println(steer_link_Pot);
   Serial.println(joystick_steerPot);
   Serial.println(joystick_drivePot);
   delay(100);        // delay in between reads for stability
}
