// include all libraries
#include <stdio.h>;
#include <Pixy2.h>;
#include <Servo.h>;
#include<DC_Motor.h> 

// set variables for all hardwares
Pix2 pixy;
Servo servo_claw;
Servo servo_arm;
DC_Motor motor_l; 
DC_Motor motor_r; 
//setup some other helpful variable
int state = 1; //state == 1 is line following state == 0 is grabbing


//initialization
void setup(){

	//set pin

	// Initialize pixy

	//check claw position and set it to open.

	//check camera position and set it to down

}

void loop(){
	if(state){
		//switch to color identification library

		//set camera position to up

		//continue moving until close to the object. And close claw
		
	}else{

		//check for vectors, coordinates, and interactions

		//case1 no new vector (straight)

		//case 2 new vector. (calculate movement using coordinates)

		//case 3 saw at the end of line && set state to 1
	}
	
}
