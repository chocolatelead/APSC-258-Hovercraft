
#include<PS4BT.h>
#include<usbhub.h>
#ifdef dobogusinclude
#include<spi4teensy3.h>
#endif
#include <SPI.h>
#include<Servo.h>
#include<Wire.h>

Servo batteryservo;
Servo craneservo;
Servo fanservo;
USB Usb;
//USBHub Hub1(&Usb);
BTD Btd(&Usb);
//PS4BT PS4(&Btd, PAIR);
PS4BT PS4(&Btd);

//servos
int fanpos = 0;
int cranepos = 0;
int cranepin = 3;
int fanpin = 2;

//Fans and dropping mechanism
char mosfetPin = A0;
char thrustpin = A1;

//boolean on/off variables
int dropon = 0;
int thrustOn = 0;
int mosfetOn = 0;
int motorSpeed = 0;
int fanSpeed = 0;

//motor pins
int enA = 11;  // Enable Pin for motor 1
int enB = 5;  // Enable Pin for motor 2
 
char in1 = A2;  // Control pin 1 for motor 1
char in2 = A3;  // Control pin 2 for motor 1
char in3 = A4;  // Control pin 1 for motor 2
char in4 = A5;  // Control pin 2 for motor 2

//bool batteryon = false;
void setup() {
  //servos
  pinMode(mosfetPin, OUTPUT);
  pinMode(cranepin, OUTPUT);
  pinMode(fanpin, OUTPUT);
  pinMode(thrustpin, OUTPUT);
//  batteryservo.attach(batterypin);
  craneservo.attach(cranepin);
  fanservo.attach(fanpin);
  
  //motors
  
  
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  //fans
  digitalWrite(mosfetPin, LOW);
  digitalWrite(thrustpin, LOW);
 
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
  digitalWrite(enA,LOW);
  digitalWrite(enB,LOW);

  //playstation bluetooth initialization
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
Serial.print(F("\r\nPS4 Bluetooth Library Started"));

}

void loop() {
  Usb.Task();

if (PS4.connected()){ //nothing runs before the ps4 controller is connected
 
 // Servo code all works
  fanpos = PS4.getAnalogHat(RightHatX);
  cranepos = PS4.getAnalogHat(LeftHatX);
  if (fanpos < 30) fanpos = 30; 
  if (fanpos > 225) fanpos = 225; 
  if (cranepos < 30) cranepos = 30; 
  if (cranepos > 225) cranepos = 225;
  fanservo.write(map(fanpos, 0, 255, 0,180)); //scales joystick values to servo value range
  craneservo.write(map(cranepos, 0, 255, 0, 180));

  //Motor  input 1 low input 2 high, motor goes clockwise
/*
//for controlling the thrust fans
//if pressing L2, make the fan speed equal to the value given by L2 (0-255)
  if (PS4.getAnalogButton(L2)>0){
        fanSpeed = map(PS4.getAnalogButton(L2), 0, 255, 0 ,255); 
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);                                           
  }
  else            //if not pressing L2, turn off motor
  {
    analogWrite(enB, 0);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }
  
  Serial.print(F("\r\nL2: "));
  Serial.print(PS4.getAnalogButton(L2));   
      
  analogWrite(enB, fanSpeed); // sets the back fans to specific speed 

*/
  // To move the dropping mechanism forward and backwards on the thread
  

 
  //clockwise
  if (PS4.getButtonClick(UP)) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enA, motorSpeed/4);
    motorSpeed = 255;
  }
 //counter clockwise
  if (PS4.getButtonClick(DOWN)) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enA, motorSpeed/4);
    motorSpeed = 255;
  }
  //stop
  if (PS4.getButtonClick(RIGHT)){
    if (motorSpeed == 0){
      motorSpeed = 255;
    }
    else if (motorSpeed == 255){
      motorSpeed = 0;
    
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
    analogWrite(enA,0);
    }
    
  }
 
 if(PS4.getButtonClick(LEFT)) // If the dropping mechanism isnt running, make it run, if it isnt, make it stop
  {
    if (dropon == 0){
      dropon = 1;
      
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      digitalWrite(enB, HIGH);
    }
    else if (dropon ==1){
      dropon = 0;
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
      digitalWrite(enB, LOW);
      
    }
  }
 if(PS4.getButtonClick(CROSS)) // If thrust fans arent running, make them run, if they are, make them stop
  {
    if (thrustOn == 0){
      thrustOn = 1;
      digitalWrite(thrustpin, HIGH);
    }
    else if (thrustOn ==1){
      thrustOn = 0;
      digitalWrite(thrustpin, LOW);
    }
  }
 
    

  if(PS4.getButtonClick(TRIANGLE)) // If bottom fans arent running, make them run, if they are, make them stop
  {
    if (mosfetOn ==0){
      mosfetOn = 1;
      digitalWrite(mosfetPin, HIGH); 
    }
    else if (mosfetOn == 1){
      mosfetOn = 0;
      digitalWrite(mosfetPin, LOW);
  }

}
}
}
