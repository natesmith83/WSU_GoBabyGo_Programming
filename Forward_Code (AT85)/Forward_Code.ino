int ForwardReverse = A2;
int Forward = 1;
int Reverse = 2;
int y = 1;

void setup() {

  //Serial.begin(9600);
  pinMode(ForwardReverse,INPUT);
  pinMode(Forward, OUTPUT);
  pinMode(Reverse, OUTPUT);
  
}

void settozero(){

  digitalWrite(Forward, LOW);
  digitalWrite(Reverse, LOW);
  
}

void loop() {

  y = analogRead(ForwardReverse);

    settozero();
    while( y < 250){

      y = analogRead(ForwardReverse);
      digitalWrite(Forward,HIGH);
      
    }
 
    settozero();
    while (y > 900 ){
      
      y = analogRead(ForwardReverse);
    digitalWrite(Reverse, HIGH);
    
    }
    
    
}

