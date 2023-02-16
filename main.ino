// include all libraries
#include <stdio.h>;
#include <Pixy2.h>;
#include <Servo.h>;
#include "CytronMotorDriver.h"

// set variables for all hardwares
Pix2 pixy;
Servo servo_claw;
Servo servo_cam;
CytronMD motor_l(PWM_DIR, 3, 4);  // PWM 1 = Pin 3, DIR 1 = Pin 4.
CytronMD motor_r(PWM_DIR, 9, 10); // PWM 2 = Pin 9, DIR 2 = Pin 10.

//setup some other helpful variable
bool state = true; //state == 1 is line following state == 0 is grabbing
bool firstrun = true;

//initialization
void setup(){
	//start serial monitor
	Serial.begin(9600);
  	Serial.print("Starting...\n");
	//set pin
	servo_claw.attach(1);
	servo_arm.attach(2);
	// Initialize pixy
  	pixy.init();
	//set claw position to open.
	servo_claw.write(90) //set open angle to 90 degrees.
	//check camera position and set it to down
	servo_cam.write(0) //set camera arm angle at 0 degree.
}

void loop(){
	if(state){
		
		if(firstrun){
			pixy.changeProg("line"); //changing to linefollowing library
			firstrun = False; //make sure it only run in the first time.
		}
		int8_t i;
		char buf[128];

		pixy.line.getAllFeatures();

		// print all vectors
		for (i=0; i<pixy.line.numVectors; i++)
		{
			sprintf(buf, "line %d: ", i);
			Serial.print(buf);
			pixy.line.vectors[i].print();
		}

		// print all intersections
		for (i=0; i<pixy.line.numIntersections; i++)
		{
			sprintf(buf, "intersection %d: ", i);
			Serial.print(buf);
			pixy.line.intersections[i].print();
		}
		
		//check for vectors, coordinates, and interactions

		//case1 no new vector (straight)

		//case 2 new vector. (calculate movement using coordinates)

		//case 3 saw at the end of line && set state to 1
		
	}else{
		
		if(firstrun){
			pixy.changeProg("video"); //changing to color identification library
			firstrun = False; //make sure it only run in the first time.
		}
		//switch to color identification library

		//set camera position to up

		//continue moving until close to the object. And close claw
		
	}
	
}

void moveMotor() {
  motor1.setSpeed(128);   // Motor 1 runs forward at 50% speed.
  motor2.setSpeed(-128);  // Motor 2 runs backward at 50% speed.
  delay(1000);
  
  motor1.setSpeed(255);   // Motor 1 runs forward at full speed.
  motor2.setSpeed(-255);  // Motor 2 runs backward at full speed.
  delay(1000);

  motor1.setSpeed(0);     // Motor 1 stops.
  motor2.setSpeed(0);     // Motor 2 stops.
  delay(1000);

  motor1.setSpeed(-128);  // Motor 1 runs backward at 50% speed.
  motor2.setSpeed(128);   // Motor 2 runs forward at 50% speed.
  delay(1000);
  
  motor1.setSpeed(-255);  // Motor 1 runs backward at full speed.
  motor2.setSpeed(255);   // Motor 2 runs forward at full speed.
  delay(1000);

  motor1.setSpeed(0);     // Motor 1 stops.
  motor2.setSpeed(0);     // Motor 2 stops.
  delay(1000);
} 
