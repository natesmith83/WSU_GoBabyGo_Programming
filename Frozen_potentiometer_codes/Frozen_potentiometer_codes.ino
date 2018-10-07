


// the setup routine runs once when you press reset:
void setup() {
   // initialize serial communication at 9600 bits per second:
   Serial.begin(9600);
   
}

// the loop routine runs over and over again forever:
void loop() {

   int steer_link_Pot = analogRead(A0);
   int joystick_steerPot = analogRead(A1);
   int joystick_drivePot = analogRead (A2);
   int drivemotorPin = 6;
   const int threshold = 750;
  

   if(joystick_drivePot > threshold) {
    digitalWrite(drivemotorPin, HIGH);
   } else {
    digitalWrite(drivemotorPin, LOW); 
   }
   //}
   // print out the value you read:
   Serial.println(steer_link_Pot);
   Serial.println(joystick_steerPot);
   Serial.println(joystick_drivePot);
   delay(100);        // delay in between reads for stability
}
