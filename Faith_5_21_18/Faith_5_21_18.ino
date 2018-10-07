#include <DualVNH5019MotorShield.h>
DualVNH5019MotorShield md;


int head_left_pin = A3;
int head_forward_pin = A4;
int head_right_pin = A5;

int remote_forward_pin = 3;
int remote_reverse_pin = 5;
int remote_left_pin = 11;
int remote_right_pin = 13;

int parent_release_pin = 14;

int come_center_pin = 15;



void setup() {

  pinMode(head_left_pin, INPUT);
  pinMode(head_forward_pin, INPUT);
  pinMode(head_right_pin, INPUT);
  
  pinMode(remote_forward_pin, INPUT);
  pinMode(remote_reverse_pin, INPUT);
  pinMode(remote_left_pin, INPUT);
  pinMode(remote_right_pin, INPUT);
  
  pinMode(parent_release_pin, INPUT);
  

  pinMode(come_center_pin, INPUT);

  md.init();
  Serial.begin(9600);

}


bool read_head(int &left, int &forward, int &right);
bool read_remote(int &left, int &forward, int &right, int &reverse);
void head_control1();
void head_control();
void remote_control();

bool motor_start();
void motor_stop(int current_speed, String direction_fr);
void return_center(String from_direction);
void turn_left();
void turn_right();

const int FORWARD_THRESHOLD = 400;
const int RIGHT_THRESHOLD = 400;
const int LEFT_THRESHOLD = 400;
const int DELAY_MILLISECONDS = 400;
const int TURN_SPEED = 200;
const int MAX_SPEED = 125;
const int RAMP_DELAY = 5;

void loop() {

  int left, right, forward, reverse, temp;

  md.setM1Speed(0);
  
  if(read_remote(left, forward, right, reverse))
  {
    Serial.println("Remote being activated");
    Serial.println("");
    remote_control();
    
  }

  if(read_head(left, forward, right))
  {
    Serial.println("Head being activated");
    Serial.println("");
    head_control();
    Serial.println("Out of Head");
    
  }

}

bool motor_start()
{
  
}

void motor_stop(int current_speed, String direction_fr)
{
  int fr;
  
  if(direction_fr == "forward")
    fr = 1;
  else if(direction_fr == "reverse")
    fr = -1;
    
  for(int i = current_speed; i >= 0; i--)
  {
    md.setM1Speed(fr * i);
    delay(RAMP_DELAY);
  }
  
}

void turn_left()
{

}


void turn_right()
{

}


void return_center(String from_direction)
{
  if(from_direction == "right")
  {
    while(digitalRead(come_center_pin) == true)
    {
      md.setM2Speed(-1 * TURN_SPEED);
    }
    md.setM2Speed(0);
  }

  else if(from_direction == "left")
  {
    while(digitalRead(come_center_pin) == true)
    {
      md.setM2Speed(TURN_SPEED);
    }
    md.setM2Speed(0);
  }
}


bool read_head(int &left, int &forward, int &right)
{
  left = analogRead(head_left_pin);
  forward = analogRead(head_forward_pin);
  right = analogRead(head_right_pin);  

  if(left > LEFT_THRESHOLD || forward > FORWARD_THRESHOLD || right > RIGHT_THRESHOLD)
    return true;

  else
    return false;
}


bool read_remote(int &left, int &forward, int &right, int &reverse)
{
  left = digitalRead(remote_left_pin);
  forward = digitalRead(remote_forward_pin);
  right = digitalRead(remote_right_pin);
  reverse = digitalRead(remote_reverse_pin);

  if(left || forward || right || reverse)
    return true;

  else
    return false;
}


void head_control() {
  
  int forward;
  int left;
  int right;
  int temp1, temp2, temp3, temp4;
  int current_speed = 100;
  unsigned long time_last_used = millis();
  unsigned long now = millis();
  String turning_direction = "none";
  bool head_active = "true";
  
  do {
   
    if(head_active = read_head(left, forward, right)) {
      time_last_used = millis();
    }

    if(read_remote(temp1, temp2, temp3, temp4)) {
      motor_stop(0, "forward");
      return_center(turning_direction);
      return;
    }
    
    md.setM1Speed(current_speed);
    delay(RAMP_DELAY);
    
    if(left > LEFT_THRESHOLD) {
      md.setM2Speed(-1 * TURN_SPEED);
      turning_direction = "left";
    }

    else if(right > RIGHT_THRESHOLD) {
      md.setM2Speed(TURN_SPEED);
      turning_direction = "right";
    }

    else if(forward > FORWARD_THRESHOLD) {
      return_center(turning_direction);
      turning_direction = "none";
    }
        
    
    if(current_speed < MAX_SPEED) {
      current_speed++;
    }

    now = millis();
    
  } while (head_active || now - time_last_used < DELAY_MILLISECONDS);

  motor_stop(current_speed, "forward");
  return_center(turning_direction);
}



void remote_control()
{
  int left, forward, right, reverse;
  String direction_fr, direction_lr;
  read_remote(left, forward, right, reverse);
  unsigned long time_last_used = millis();
  unsigned long now = millis();
  int current_speed = 100;
  bool stopped = true;
  
  while(!digitalRead(parent_release_pin))
  {
    if(stopped == true)
      current_speed = 100;
      
    read_remote(left, forward, right, reverse);
    
    Serial.print("left: "); Serial.println(left);
    Serial.print("right: "); Serial.println(right);
    Serial.print("forward: "); Serial.println(forward);
    Serial.print("reverse: "); Serial.println(reverse);
    Serial.println("");

    //delay(500);
    
    if(forward)
    {
      stopped = false;

      for(current_speed; current_speed <= MAX_SPEED && digitalRead(remote_forward_pin) == 1; current_speed++)
      {
        md.setM1Speed(current_speed);
        delay(RAMP_DELAY);
        if(current_speed == MAX_SPEED)
          current_speed--;
      }
      
      time_last_used = millis();
      //read_remote(left, forward, right, reverse);
      Serial.print("in forward loop. Forward: ");
      Serial.println(forward);
      Serial.println("");

      direction_fr = "forward";
      direction_lr = "none";
    }

    else if(reverse)
    {
      stopped = false;

      for(current_speed; current_speed <= MAX_SPEED && digitalRead(remote_reverse_pin) == 1; current_speed++)
      {

        md.setM1Speed(-1 * current_speed);
        delay(RAMP_DELAY);
        if(current_speed == MAX_SPEED)
          current_speed--;
          
      }
      time_last_used = millis();
      //read_remote(left, forward, right, reverse);
      Serial.println("in reverse loop");
      direction_fr = "reverse";
      direction_lr = "none";
      
    }

    else if(left)
    {
      stopped = false;
      md.setM2Speed(-1 * TURN_SPEED);
      for(current_speed; current_speed <= MAX_SPEED && digitalRead(remote_left_pin) == 1; current_speed++)
      {
      
        md.setM1Speed(current_speed);
        delay(RAMP_DELAY);
      
        if(current_speed == MAX_SPEED)
          current_speed--;
      }
      time_last_used = millis();
      //turn_left();
      
      Serial.println("in left loop");
      direction_fr = "forward";
      direction_lr = "left";
     
     //return_center("left");
    }

    else if(right)
    {
      stopped = false;
      md.setM2Speed(TURN_SPEED);
      for(current_speed; current_speed <= MAX_SPEED && digitalRead(remote_right_pin) == 1; current_speed++)
      {

        md.setM1Speed(current_speed);
        delay(RAMP_DELAY);

        if(current_speed == MAX_SPEED)
          current_speed--;
      }
      time_last_used = millis();

      Serial.print("in right loop. Right: ");
      Serial.println(right);
      
      direction_fr = "forward";
      direction_lr = "right";
      
      
    }
    Serial.println("remote in use");
    Serial.println("");

    now = millis();

    if(/*now - time_last_used > DELAY_MILLISECONDS && */stopped == false)
      {
        motor_stop(current_speed, direction_fr);
        return_center(direction_lr);
        stopped = true;
      }
  }
  
}


