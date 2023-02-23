// include all libraries
#include <stdio.h>;
#include <Pixy2.h>;
#include <Servo.h>;
#include <CytronMotorDriver.h>;

// set variables for all hardwares
Pix2 pixy;
Servo servo_claw;
Servo servo_arm;

// motor l is attached backward and negative value is forward.
// motor r is attached normally and positive value is forward.
// eg. 
// motor_l.setSpeed(-128);
// motor_r.setSpeed(128);
// will move both motor forward at 50% power.

CytronMD motor_l(PWM_DIR, 3, 8);  // PWM 1 = Pin 3, DIR 1 = Pin 8.
CytronMD motor_r(PWM_DIR, 6, 7); // PWM 2 = Pin 6, DIR 2 = Pin 7.

// setup some other helpful variable
bool state = true; //state == 1 is line following state == 0 is grabbing
bool firstrun = true;

// initialization
void setup(){
	//start serial monitor
	Serial.begin(9600);
  	Serial.print("Starting...\n");
	
	// Initialize pixy
  	pixy.init();
	
	//set pin
	servo_claw.attach(4); // range open 60-0 close
	servo_arm.attach(5);  // rangee up 100-40 down
	servo_claw.write(90) //set open angle to 90 degrees.
	servo_arm.write(0) //set camera arm angle at 0 degree.
	
	
	//set claw position to open.
	char buffer[40];
	sprintf(buffer, "The starting angle of claw servo is %d degree", servo_claw.read());
	Serial.println(buffer);
	sprintf(buffer, "The starting angle of camera servo is %d degree", servo_arm.read());
	Serial.println(buffer);
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
		
		//<pixy.line.numVectors> is
		// Member variables for pixy.line(var type :uint8_t)
		// it keep tracks of: The number of lines in the vectors variable.

		//<pixy.line.vecotrs> is
		// Member variables for pixy.line(var type :array of Vector(struct))
		// This array contains either all of the detected lines if getAllFeatures() is called or the Vector if getMainFeatures() is called.
		
		// here are some helpful info for Vector Struct
		
		// uint8_t m_x0
		// This variable contains the x location of the tail of the Vector or line. The value ranges between 0 and frameWidth (79) 3)
		
		// uint8_t m_y0
		// This variable contains the y location of the tail of the Vector or line. The value ranges between 0 and frameWidth (52).
		
		// uint8_t m_x1
		// This variable contains the x location of the head (arrow end) of the Vector or line. The value ranges between 0 and frameWidth (79).
		
		// uint8_t m_y1
		//This variable contains the y location of the head (arrow end) of the Vector or line. The value ranges between 0 and frameWidth (52).


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
