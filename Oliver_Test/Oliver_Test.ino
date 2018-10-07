int Forward_input = A3; 
int Reverse_input = A2; // Analog value from joystick. 
int Forward = 3; 
int Reverse = 4;
int joystick_y = 1;
int right_input = A5;
int left_input = A4; 
int left_output_pin = 5;
int right_output_pin = 6;
int joystick_x = 1;
int line_sensor_pin = 7;
int line_sensor_value = 0;
int turned_right = 0;
int turned_left = 0;
char currentstate = 0;
int speed_pin = 9;
int ramp = 0;
int deceleration = 0;
int LED = 13;

//limit for deadzone
const int right_limit = 900;
const int left_limit = 900;
const int forward_limit = 900;
const int reverse_limit = 900;
const int speed_value = 100;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(Forward_input, INPUT);
  pinMode(Reverse_input, INPUT);
  pinMode(Forward, OUTPUT);
  pinMode(Reverse, OUTPUT);
  pinMode(left_input, INPUT);
  pinMode(right_input, INPUT);
  pinMode(left_output_pin, OUTPUT);
  pinMode(right_output_pin, OUTPUT);
  pinMode(line_sensor_pin,INPUT);
  pinMode (speed_pin, OUTPUT);
}

//reset
void settozero(){
  //digitalWrite(Forward, LOW);
  //digitalWrite(Reverse, LOW);
  digitalWrite(right_output_pin, LOW);
  digitalWrite(left_output_pin, LOW);
  analogWrite(speed_pin, 0);


}
void drivemode(char mode) {
  switch (mode){
    case 'F':
		settozero();
		digitalWrite(Forward,HIGH);
    analogWrite (speed_pin,ramp++);
    if(ramp>speed_value){ramp=speed_value;}
    deceleration=1; 
    
    break;
    case 'B':
		settozero();
		digitalWrite(Reverse,HIGH);
    analogWrite (speed_pin,ramp++);
    if(ramp>speed_value){ramp=speed_value;}
    deceleration=1;
    
    break;
    case 'L':
		settozero();
		digitalWrite(left_output_pin,HIGH);
	
    break;
    case 'R':
		settozero();
		digitalWrite(right_output_pin,HIGH);
	
    break;
    case 1:
		settozero();
    digitalWrite(Forward,HIGH);
    digitalWrite(left_output_pin,HIGH);
    analogWrite (speed_pin,ramp++);
    if(ramp>speed_value){ramp=speed_value;}
    deceleration=1; 
 
    break;
    case 2:
    settozero();
    digitalWrite(Forward,HIGH);
    digitalWrite(right_output_pin,HIGH);
    analogWrite (speed_pin,ramp++);
    if(ramp>speed_value){ramp=speed_value;}
    deceleration=1; 

    break;
    case 3:
    settozero();
    digitalWrite(Reverse,HIGH);
    digitalWrite(left_output_pin,HIGH);
    analogWrite (speed_pin,ramp++);
    if(ramp>speed_value){ramp=speed_value;}
    deceleration=1; 
    
    break;
    case 4:
    settozero();
    digitalWrite(Reverse,HIGH);
    digitalWrite(right_output_pin,HIGH);
    analogWrite (speed_pin,ramp++);
    if(ramp>speed_value){ramp=speed_value;}
    deceleration=1; 
    
    break;
    case 0:
    settozero();
		if (currentstate == 1 || currentstate == 'L' || currentstate == 3) {
			while ( line_sensor_value > 0 ) {
				digitalWrite(right_output_pin, HIGH);	
        line_sensor_value = digitalRead(line_sensor_pin);
			}
     settozero();
     currentstate = 0;
		}
		if (currentstate == 2 || currentstate == 'R' || currentstate == 4) {
			while ( line_sensor_value > 0 ) {
				digitalWrite(left_output_pin, HIGH);	
        line_sensor_value = digitalRead(line_sensor_pin);
        Serial.println("what here");
			}
     settozero();
     currentstate = 0;
		}

    analogWrite(speed_pin,0);
    ramp = 0;
    if (deceleration > 0) {
      digitalWrite(LED,HIGH);
      delay(750);
      digitalWrite(LED,LOW);
    }
    digitalWrite(Forward, LOW);
    digitalWrite(Reverse, LOW);
    deceleration = 0;
    break;

  }  
  currentstate = mode;
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(25);
  joystick_y = analogRead(Forward_input, Reverse_input);
  joystick_x = analogRead(right_input, left_input);
  line_sensor_value = digitalRead(line_sensor_pin);
  
  if (joystick_y < forward_limit) {
   if ( joystick_x < left_limit) {
	   drivemode(1);
   }
   else if ( joystick_x >= left_limit && joystick_x <= right_limit){
	   drivemode('F');
   }
	   else if (joystick_x > right_limit ) {
		   drivemode(2);
   }
  }
   
   if ( joystick_y >= forward_limit && joystick_y <= reverse_limit) {
   if ( joystick_x < left_limit) {
	   drivemode('L');
   }
   else if ( joystick_x >= left_limit && joystick_x <= right_limit){
	   drivemode(0);
   }
	   else if (joystick_x > right_limit ) {
		   drivemode('R');
   }
  }

	if ( joystick_y > reverse_limit) {
	if ( joystick_x < left_limit) {
	   drivemode(3);
	}
		else if ( joystick_x >= left_limit && joystick_x <= right_limit){
	   drivemode('B');
		}
		else if (joystick_x > right_limit ) {
			drivemode(4);
   }
  }  
}
