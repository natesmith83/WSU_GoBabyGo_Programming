//Nathan Smith
int leftright_joystick_input_pin = A2;
int left_output_pin = 1;
int right_output_pin = 2;
int joystick_value = 1;
int line_sensor_pin = 0;
int line_sensor_value = 0;
int turned_right = 0;
int turned_left = 0;

void setup() {
   //Serial.begin(9600);
   pinMode(leftright_joystick_input_pin, INPUT);
   pinMode(left_output_pin, OUTPUT);
   pinMode(right_output_pin, OUTPUT);
   pinMode(line_sensor_pin,INPUT);
}

void settozero() {

  digitalWrite(right_output_pin, LOW);
  digitalWrite(left_output_pin, LOW);
}

void loop() {

  joystick_value = analogRead(leftright_joystick_input_pin);
   
  settozero();
    
  while(joystick_value < 250){
      joystick_value = analogRead(leftright_joystick_input_pin);
      digitalWrite(left_output_pin,HIGH);
      turned_left=1;
      }

  settozero();
  //Serial.print("turn left value:");
 //Serial.println(turned_left);       
  if(turned_left > 0)
  {
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

  settozero();

  while ( joystick_value > 900){
      joystick_value = analogRead(leftright_joystick_input_pin);
      digitalWrite(right_output_pin, HIGH);
      turned_right=1;
    }

  settozero();

  if(turned_right > 0)
  {
    line_sensor_value = digitalRead(line_sensor_pin);
    while(line_sensor_value > 0)
    {
      digitalWrite(left_output_pin, HIGH);
      line_sensor_value = digitalRead(line_sensor_pin);
    }
    turned_right = 0;
  }
}    

