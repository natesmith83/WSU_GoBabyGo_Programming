int left_input_pin = A2;
int right_input_pin = A0;
int forward_input_pin = A5;
int left_output_pin = 3;
int right_output_pin = 4;
int Forward = 8;
int f = 1;
int line_sensor_pin = 7;
int line_sensor_value = 0;
int turned_right = 0;
int turned_left = 0;
int r = 1;
int l = 1;

void setup() {
   //Serial.begin(9600);
   pinMode(left_input_pin, INPUT);
   pinMode(right_input_pin, INPUT);
   pinMode(left_output_pin, OUTPUT);
   pinMode(right_output_pin, OUTPUT);
   pinMode(forward_input_pin, INPUT);
   pinMode(Forward, OUTPUT);
   pinMode(line_sensor_pin,INPUT);
}

void settozero() {

  digitalWrite(right_output_pin, LOW);
  digitalWrite(left_output_pin, LOW);
  digitalWrite(Forward, LOW);
}

void loop() {

  r = analogRead(right_input_pin);
  l = analogRead(left_input_pin);
  f = analogRead(forward_input_pin);
   
  settozero();
  while( f > 750){

      f = analogRead(forward_input_pin);
      digitalWrite(Forward,HIGH);
      
    }

    settozero();
    
  while( l > 750){
      l = analogRead(left_input_pin);
      digitalWrite(left_output_pin,HIGH);
      digitalWrite(Forward,HIGH);
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

  while ( r > 750){
      r = analogRead(right_input_pin);
      digitalWrite(right_output_pin, HIGH);
      digitalWrite(Forward,HIGH);
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

