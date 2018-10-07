#include <NewPing.h>  

int PING_PIN = 12;  // Arduino pin tied to both trigger and echo pins on the ultrasonic sensor. 
# define MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
int ForwardReverse = A2; // Analog value from joystick. 
int Forward = 3; 
int Reverse = 4;
int y = 1;
int leftright_joystick_input_pin = A5;
int left_output_pin = 5;
int right_output_pin = 6;
int joystick_value = 1;
int line_sensor_pin = 7;
int line_sensor_value = 0;
int turned_right = 0;
int turned_left = 0;

NewPing sonar(PING_PIN, PING_PIN, MAX_DISTANCE); // NewPing setup of pin and maximum distance.
unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).

void setup() {

  //Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  Serial.begin(9600);
  pinMode(ForwardReverse,INPUT);
  pinMode(Forward, OUTPUT);
  pinMode(Reverse, OUTPUT);
  pinMode(leftright_joystick_input_pin, INPUT);
  pinMode(left_output_pin, OUTPUT);
  pinMode(right_output_pin, OUTPUT);
  pinMode(line_sensor_pin,INPUT);
  
}

void settozero(){

  digitalWrite(Forward, LOW);
  digitalWrite(Reverse, LOW);
  digitalWrite(right_output_pin, LOW);
  digitalWrite(left_output_pin, LOW);
  
}

void loop() {
delay(250);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings. 
uS = sonar.ping();              // Send ping, get ping time in microseconds (uS).
y = analogRead(ForwardReverse);
joystick_value = analogRead(leftright_joystick_input_pin);

settozero();


if (y < 380){  // stick position go forward 
  
  digitalWrite(Forward,HIGH);
  if (MAX_DISTANCE < uS )  digitalWrite(Forward,LOW);

}

if (y >= 380 && y <= 750 ){ // medum rang no go 
    // put the parking break on ??? 
  }

if (y > 750 ){ // reverse

  digitalWrite(Reverse, HIGH);
}

while (joystick_value < 250) {
   joystick_value = analogRead(leftright_joystick_input_pin);
   digitalWrite (left_output_pin, HIGH);
   turned_left = 1;
  settozero(); 
}

   
   if(turned_left > 0){
   line_sensor_value = digitalRead(line_sensor_pin);
      while(line_sensor_value > 0)
      {
        digitalWrite(right_output_pin, HIGH);
        line_sensor_value = digitalRead(line_sensor_pin);
        //Serial.println(digitalRead(line_sensor_pin));
        //Serial.println("adjusting Right");
      }
    turned_left = 0;
   }
   


while(joystick_value > 900) {
   joystick_value = analogRead(leftright_joystick_input_pin);
   digitalWrite (right_output_pin, HIGH);
   turned_right = 1;
   settozero();
}


 if (turned_right > 0){
    line_sensor_value = digitalRead(line_sensor_pin);
    while(line_sensor_value > 0)
    {
      digitalWrite(left_output_pin, HIGH);
      line_sensor_value = digitalRead(line_sensor_pin);
      //Serial.println(digitalRead(line_sensor_pin));
      //Serial.println("adjusting Left");
    }
    turned_right = 0;
   }
     


//Serial.print ("LeftRight = ");
//Serial.println (joystick_value);
//Serial.print ("ForwardReverse = ");
//Serial.println (y);  
}















//  while(joystick_value < 250){
//      joystick_value = analogRead(leftright_joystick_input_pin);
//      digitalWrite(left_output_pin,HIGH);
//      turned_left=1;
//      }
//
//  settozero();
  //Serial.print("turn left value:");
 //Serial.println(turned_left);       
//  if(turned_left > 0)
//  {
//    line_sensor_value = digitalRead(line_sensor_pin);
//    while(line_sensor_value > 0)
//    {
//      digitalWrite(right_output_pin, HIGH);
//      line_sensor_value = digitalRead(line_sensor_pin);
//      //Serial.println(digitalRead(line_sensor_pin));
//      //Serial.println("adjusting Right");
//    }
//    turned_left = 0;
//  }
//
//  settozero();

//  while ( joystick_value > 900){
//      joystick_value = analogRead(leftright_joystick_input_pin);
//      digitalWrite(right_output_pin, HIGH);
//      turned_right=1;
//    }
//
//  settozero();

//  if(turned_right > 0)
//  {
//    line_sensor_value = digitalRead(line_sensor_pin);
//    while(line_sensor_value > 0)
//    {
//      digitalWrite(left_output_pin, HIGH);
//      line_sensor_value = digitalRead(line_sensor_pin);
//    }
//    turned_right = 0;
//  }  
  
//  }
//}
