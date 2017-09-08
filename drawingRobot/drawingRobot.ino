// Author: James Clare
// Year: 2017
//------------------------------------------------------------------------------------------------------------------------------------------------------------------
//The purpose of this program is to control the position of a drawing robot using the trackpad of a laptop.
//Position data will be sent to the arduino via the serial bus, this program will then interpret that data and 
//move the motors and servos to the correct positions, along with displaying a warning led to show when the device is moving.
//The position sent from a laptop running a program written in C#, the code for this program is also included in the folder.
//------------------------------------------------------------------------------------------------------------------------------------------------------------------


//include the stepper motor and servo controller libraries
#include <AccelStepper.h>
#include<Servo.h>

// Pin definitions for the first motor
#define motorPin1  3
#define motorPin2  4
#define motorPin3  5
#define motorPin4  6
// Pin definitions for the second motor
#define motorPin5  10
#define motorPin6  11
#define motorPin7  12
#define motorPin8  13
// Pin definitions for the third motor
#define motorPin9   14
#define motorPin10  15
#define motorPin11  16
#define motorPin12  17
// Define the number of half steps in the motors rotation.
// These stepper motors have 4 electromagnets, therefore they have 8 halfsteps in one rotation.
#define HALFSTEP 8
// Pin definition for the activity LED
#define ledPin 2

// Initialise the servo
Servo servo1;
//Initialise the varible used to store the input serial data.
String serialData;

// Initialize each stepper motor using the AccelStepper Library
AccelStepper stepper1(HALFSTEP, motorPin1, motorPin2, motorPin3, motorPin4);
AccelStepper stepper2(HALFSTEP, motorPin5, motorPin6, motorPin7, motorPin8);
AccelStepper stepper3(HALFSTEP, motorPin9, motorPin10, motorPin11, motorPin12);

//setup function used to initialise motor parameters
void setup() {
  //initialise parameters of motor 1
  stepper1.setMaxSpeed(1000.0);
  stepper1.setAcceleration(1000.0);
  stepper1.setSpeed(1000);
  stepper1.setCurrentPosition(0);
  stepper1.run();
  //initialise parameters of motor 2
  stepper2.setMaxSpeed(1000.0);
  stepper2.setAcceleration(1000.0);
  stepper2.setSpeed(1000);
  stepper2.setCurrentPosition(0);
  stepper2.run();
  //initialise parameters of motor 3
  stepper3.setMaxSpeed(1000.0);
  stepper3.setAcceleration(1000.0);
  stepper3.setSpeed(1000);
  stepper3.setCurrentPosition(0);
  stepper3.run();

  //set the pin of the activity LED to an output
  pinMode(ledPin, OUTPUT);

  //attach the servo signal line to a pin
  servo1.attach(8);

  //begin serial communincation at a baud of 9600 bits per second
  Serial.begin(9600);
  //set the serial timeout to 10 milliseconds to prevent the arduino being overloaded with serial data
  Serial.setTimeout(10);
}

//The most frequently called function ideal to be used for updating motor positions.
void loop() {
  //call the motor run function for each motor once per tick to produce smooth motor movement
  stepper1.run();
  stepper2.run();
  stepper3.run();

  //check to see either motors is moving, and if it is turn the activity LED on, if both motors are still, turn the LED off.
  if (stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}

//The serialEvent function is called every time the arduino receives serial data
void serialEvent() {
  //the variable serialData is set to equal the string that has been sent from the laptop via the serial port.
  serialData = Serial.readString();
  //The motors target positions set to the position indicated by the mouse on the laptop.
  //stepper1 moves the robot in the X direction.
  //stepper2 and stepper3 move the robot in the Y direction.
  //The parseData functions used here parse the relevant data from the input serial string so that it can be sent to the motors. 
  //The parseData functions can be seen further down this document.
  stepper1.moveTo(parseDataY(serialData));
  stepper2.moveTo(parseDataX(serialData));
  stepper3.moveTo(parseDataX(serialData));

  //check to see if the mouse is being clicked on the laptop, and if so move the servo holding 
  //the robot's pen to the down position. If the mouse is not being clicked, move the servo to the up position.
  if (parseDataD(serialData) == 1) {
    servo1.write(70);
  } else {
    servo1.write(10);
  }
}

//The X position, Y postion and whether the mouse is being clicked is passed to
//the arduino via the serial port as a string with the data structure of X***Y***D*
//X is a value of 0 to 180.
//Y is a value of 0 to 180.
//D is either a 1 or a 0.

//This parseDataX function removes the X axis data from the input string and returns it.
int parseDataX(String data) {
  //The X character and every character after the Y are removed from the original string
  data.remove(data.indexOf("Y"));
  data.remove(data.indexOf("X"), 1);
  //The remaining data is then converted to an integer and is then mapped to values
  //between -2000 and 2000 so that the motors can interperet it. This value is then returned by the function.
  return map(data.toInt(), 0, 180, -2000, 2000);
}

//This parseDataY function removes the Y axis data from the input string and returns it.
int parseDataY(String data) {
  //The Y character and everything before and the D character and the digit after it are removed for the original string. 
  data.remove(data.indexOf("D"));
  data.remove(0,  data.indexOf("Y") + 1);
  //The remaining data is then converted to an integer and is mapped to values between
  //-2000 and 2000 so that the motors can interperet it. This value is then returned by the function.
  return map(data.toInt(), 0, 180, -2000, 2000);
}

//This parseDataD function removes the data representing whether the mouse is currently clicked from the input string and returns it.
int parseDataD(String data) {
  //The D character and everything before it is removed from the original string.
  data.remove(0,  data.indexOf("D") + 1);
  //The remaining data is then converted to an integer and is returned.
  return data.toInt();
}


