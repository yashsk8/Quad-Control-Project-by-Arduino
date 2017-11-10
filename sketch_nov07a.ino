#include <Servo.h>


//Input String is of the form "XLeft YLeft XRight YRight!"
//Each of those are 3 digit values => 3 byte each
//Each string has a total of 16 characters => 16 bytes. 
//The exclamation mark is an identifer to ensure that only complete strings are considered further.
//And also to identify which value belongs to which output.


//String to recieve Bluetooth Data 
String s;

//An integer to clear buffer after every 100 loops so that
//te buffer doesn't overflow.
int n = 0;

//Values received by parsing the Bluetooth string will be
//stored in this
int throttle,roll,yaw,pitch;

//To write outputs to the microcontroller
Servo throttleServo, yawServo, rollServo, pitchServo;



void setup() {
  // put your setup code here, to run once:
  
  //Attach all the pins only at PWM supported outputs.
  //Jumper cables from this pins will go to the respective receiver pins at the microcontroller.
  throttleServo.attach(3);
  rollServo.attach(5);
  pitchServo.attach(6);
  yawServo.attach(9);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //After 100 loops, empty buffer
  if(n==100){Serial.flush();n=0;}
  n++;

  //Recieve the string via bluetooth  
  while(Serial.available()){
    char c = Serial.read();
    if(c=='!') break;
    else s+=c; 
  }

  //Check for length of string reieved(=15)
  if(s!="" && s.length()==15){

    //Map the value received (Range: 150-250) to 1000-2000
    //to be written as output in microSeconds
    
    yaw = s.substring(0,3).toInt();
    yaw = map(yaw-150,0,100,1000,2000);
    
    throttle = s.substring(4,7).toInt();
    throttle = map(throttle-150,0,100,2000,1000);
    
    roll = s.substring(8,11).toInt();
    roll = map(roll-150,0,100,1000,2000);
    
    pitch = s.substring(12).toInt();
    pitch = map(pitch-150,0,100,2000,1000);

    //Write to Output
    throttleServo.writeMicroseconds(throttle);
    rollServo.writeMicroseconds(roll);
    pitchServo.writeMicroseconds(pitch);
    yawServo.writeMicroseconds(yaw);
  }

  //Empty String
  s = "";

  //Delay
  delay(100);
}
