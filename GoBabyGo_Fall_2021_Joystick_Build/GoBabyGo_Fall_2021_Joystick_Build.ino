//GoBabyGo Wichita State University
//Fall 2021
//code written by Everett Kernen
//electronics done by Everett Kernen and Jackson Truitt
//if you have any questions, try not to; I've spent too much time commenting extensively
//but if you do still have questions, email me at everettk2020@gmail.com with the subject line reading "GoBabyGo" along with whatever else
//I'll try to respond in a semi-timely manner

#include <Servo.h>


//PINS USED IN ARDUINO
  //either plug wires into the pins we've already written or write in the pins you're using
  //it appears that a number is equal to its digital counterpart
  //i.e. digital pin 10 --> 10
  //for analog, use prefix A, i.e. analog pin 5 --> A5
  const int JFWDBWDPIN = A1;  //pin number for forward and backwards input on the joystick
            //brown wire
  const int JSIDEWAYSPIN = A3;//pin number for left and right input on the joystick
            //yellow wire
  const int GAS = 10;         //pin number for the Gas Relay input
            //green signal wire
  const int FWD = 9;          //orange relay
            //blue signal wire
  const int BWD = 8;          //blue relay
            //yellow signal wire
  const int RIGHTMOTOR = 6;   //signal in from the remote that you are turning right
  const int LEFTMOTOR = 5;    //signal in from the remote that you are turning left
  const int ENCODER = A5;     //encoder signal pin
  const int STEERMOTOR = 3;   //motor controller output pin

//CONSTANT VALUES
  //these values should already work; however you may need to change them
  //if joystick is initializing too quickly or too late
  //or if steering motor activates too often
  //or if the steering isn't centered after "returning to center"

  //if you do need to change these values, I recommend you use the Serial Monitor to check the values
  //as you turn the joystick, what values are you seeing and what values do you need to be seeing?
  //you can see the Serial Monitor by going to Tools->Serial Monitor

  //CONSTANT VALUES TO CHECK AGAINST JOYSTICK VALUES
    //joystick initialization values
    //change these if joystick initializes too quickly or too late
    //middle is ~520 or so

    //as mentioned below with the encoder values, we see JOYSTICKMIDDLE differently when the arduino is not connected to the computer
    //we started with 520, we moved to 630 (+110 again)
    //see lines 65-68 for more info
    const int JOYSTICKMIDDLE = 630;
    const int JOYSTICKDEADZONE = 200;
    const int FWDVALUE = JOYSTICKMIDDLE - JOYSTICKDEADZONE;   //used to check "if joystick forward value is above FWDVALUE, go forward"
    const int BWDVALUE = JOYSTICKMIDDLE + JOYSTICKDEADZONE;   //same as above, just if below BWDVALUE, go backwards
    const int LEFTVALUE = JOYSTICKMIDDLE - JOYSTICKDEADZONE;  //same as above, just left
    const int RIGHTVALUE = JOYSTICKMIDDLE + JOYSTICKDEADZONE; //same as above, just right

  //CONSTANT VALUES TO CHECK AGAINST ENCODER VALUE
    //steering centering values
    //change these if steering doesn't return to center properly
    //values go from 0-1023, clockwise turning moves the number up
    //you WILL need to modify ENCODERCENTER to begin with, it depends on how you install the encoder
    //make sure you install the encoder such that when you turn, it doesn't loop around from for example 800 to 1023->0 to 200
    //that will view to you as the edges are 800 and 200, just make sure that the middle is actually 500 and not 1023

    //when you unplug the usb, the steering goes off center
    //this is just due to processing differences between computer and arduino (we think)
    //the processing differences are irrelevant to you as the user, all you need to know is
    //you'll have to play with ENCODERCENTER to fix this issue. We went from 590 to 700
    const int ENCODERCENTER = 700;
    //modify these two only if the center range is too broad or too narrow for your taste
    const int ENCODERFINE = 30;
    const int ENCODERCOARSE = 70;
    //IF THE ENCODER IS INSTALLED BACKWARDS OR THE STEERING RETURNS TO CENTER THE WRONG WAY
    //switch this from 1 to -1 or the reverse
    //please don't have this be anything other than 1 or -1
    //this actually might not work, if you're having this issue you may need to do some testing oops
    const int CLOCKWISETURNSRIGHT = 1;


  //SPEED VALUES USED FOR STEERING
    //how fast the steering turns
    //for our motor controller, the arduino views it as a servo
    //as such, we use writeMicroseconds to control the speed
    //this is only relevant if you don't use the same motor controller
    //if you don't or you just want more information, visit https://www.arduino.cc/en/Reference/ServoWriteMicroseconds or the arduino main reference page
    //anyways, for our motor controller, full speed one way is 2500; full speed the other way is 500
    //1500 is middle (no turning at all)

    //if the joystick turns the wrong way, switch the right and left values
    const int FULLSPEEDRIGHT = 750;
    const int PARTSPEEDRIGHT = 1250;
    const int ZEROSPEED = 1500;
    const int PARTSPEEDLEFT = 1750;
    const int FULLSPEEDLEFT = 2250;

//-----DO NOT MODIFY PAST THIS LINE UNLESS YOU KNOW WHAT YOU'RE DOING-----


//INTEGERS USED TO READ VALUES FROM JOYSTICK
int JFWDBWD;                
int JLEFTRIGHT;

//INTEGER TO READ VALUE FROM ENCODER (piece attached to steering shaft)
int ENCODERVAL;

//Motor Controller (viewed by arduino as a servo)
Servo STEER;

//-------------------FUNCTIONS START HERE--------------------------------

// put your setup code here, to run once:
void setup() {
  
  Serial.begin(9600);   //tells arduino how fast to interact with the Serial Monitor

  //set input and output pins for arduino
  pinMode(JFWDBWDPIN, INPUT);    
  pinMode(JSIDEWAYSPIN, INPUT);
  pinMode(ENCODER, INPUT);
  pinMode(FWD, OUTPUT);      
  pinMode(BWD, OUTPUT);
  pinMode(GAS, OUTPUT);   
  pinMode(RIGHTMOTOR, INPUT);
  pinMode(LEFTMOTOR, INPUT);
  STEER.attach(STEERMOTOR, 500, 2500);
  //pinMode(LED_BUILTIN, OUTPUT);   //set up built in led (used for troubleshooting)

  //safekeeping measure so relays are set to the car not running to the races on startup
  //also, the steering motor is set to not do anything
  digitalWrite(GAS, LOW);         
  digitalWrite(FWD, LOW);
  digitalWrite(BWD, LOW);
  STEER.writeMicroseconds(ZEROSPEED);
}

// put your main code here, to run repeatedly:
void loop() {
  
  //take in analog input to read what values the joystick is sending
  JFWDBWD = analogRead(JFWDBWDPIN);    
  JLEFTRIGHT = analogRead(JSIDEWAYSPIN);
  ENCODERVAL = analogRead(ENCODER);
  //print out values to the serial monitor so that we can look at them
  Serial.print("Yval: ");
  Serial.println(JFWDBWD);
  Serial.print("Xval: ");
  Serial.println(JLEFTRIGHT);
  Serial.print("Steering: ");
  Serial.println(ENCODERVAL);
  Serial.println();

  
  if (JFWDBWD < FWDVALUE) {
    //if joystick is forward, go forward
    digitalWrite(FWD, HIGH);
    digitalWrite(GAS, HIGH);
    digitalWrite(BWD, LOW);
  }
  else if (JFWDBWD > BWDVALUE){
    //if joystick is backward, go backward
    digitalWrite(BWD, HIGH);
    digitalWrite(GAS, HIGH);
    digitalWrite(FWD, LOW);
  }
  else {
    //"if joystick y value is centered"
    //set relays so nothing is on
    digitalWrite(FWD, LOW);
    digitalWrite(BWD, LOW);
    digitalWrite(GAS, LOW);
  }

  
  if (JLEFTRIGHT > RIGHTVALUE || digitalRead(RIGHTMOTOR) == HIGH) {
    //turn right
    STEER.writeMicroseconds(FULLSPEEDRIGHT);
  }
  else if (JLEFTRIGHT < LEFTVALUE || digitalRead(LEFTMOTOR) == HIGH) {
    //turn left
    STEER.writeMicroseconds(FULLSPEEDLEFT);
  }
  else{
    //"if joystick x value is centered"
    if (ENCODERVAL > (ENCODERCENTER + (ENCODERFINE * CLOCKWISETURNSRIGHT))){
      //if the steering is turned to the left
      if (ENCODERVAL > (ENCODERCENTER + (ENCODERCOARSE * CLOCKWISETURNSRIGHT))){
        //if it's turned all the way to the left, turn right full speed
        STEER.writeMicroseconds(FULLSPEEDRIGHT);
      }
      else{
        //if it's turned only a little to the left, turn right at a lower speed
        STEER.writeMicroseconds(PARTSPEEDRIGHT);
      }
    }
    else if (ENCODERVAL < (ENCODERCENTER - (ENCODERFINE * CLOCKWISETURNSRIGHT))){
      //if the steering is turned to the right
      if (ENCODERVAL < (ENCODERCENTER - (ENCODERCOARSE * CLOCKWISETURNSRIGHT))){
        //if it's turned all the way to the right, turn left full speed
        STEER.writeMicroseconds(FULLSPEEDLEFT);
      }
      else{
        //if it's turned only a little to the right, turn left at a lower speed
        STEER.writeMicroseconds(PARTSPEEDLEFT);
      }
    }
    else {
      //if the joystick is centered and so is the steering, do nothing
      STEER.writeMicroseconds(ZEROSPEED);
    }
  }
  
}
