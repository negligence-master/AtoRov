// include all libraries
#include <stdio.h>; // for basic functionality
#include <math.h>; // for trigs
#include <Pixy2.h>; // for pixy duhhh
#include <Servo.h>; // for servo
#include <CytronMotorDriver.h>; //for motor driver

// set variables for all hardwares
Pixy2 pixy;
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
bool state = true; // state == 1 is line following state == 0 is grabbing
bool firstrun = true;
int vector_count = 0;
int occurance = 0; // of times that rover had chosen straight 

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
  servo_claw.write(60); //set open angle to 90 degrees.
  servo_arm.write(40); //set camera arm angle at 0 degree.
  
  
  //set claw position to open.
  char buffer[40]; //for string formatting ...DO NOT REMOVE...
  sprintf(buffer, "The starting angle of claw servo is %d degree", servo_claw.read());
  Serial.println(buffer);
  sprintf(buffer, "The starting angle of camera servo is %d degree", servo_arm.read());
  Serial.println(buffer);
}

void loop(){
  if(state){
    
    if(firstrun){
      pixy.changeProg("line"); //changing to linefollowing library
      firstrun = false; //make sure it only run in the first time.
    }
    
    
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
    // This variable contains the y location of the head (arrow end) of the Vector or line. The value ranges between 0 and frameWidth (52).


    int8_t i; // for loop indexing ...DO NOT REMOVE...
    
    char buf[128]; // for string formating ...DO NOT REMOVE...

    pixy.line.getAllFeatures();

    // detect if new vector found
    if (pixy.line.numVectors >= vector_count){
      occurance = 0;
      vector_count = pixy.line.numVectors; // update vector count
      float turn_angle = calc_turn_angle(pixy.line.vectors[vector_count-1]); // calculate turn angle by passing the newest vector to a function.
                             // note that 1st vector has index 0, 2nd vector has index 1....
                             // therefore the newest vector will always be index len-1
      int direction = 1; // default to turn right
      if(turn_angle < 0){ // adjust if it is actually left.
        turn_angle = turn_angle * -1;
        direction = -1;
      }
      float dur = turn_angle*2.6125;
      spin(direction, dur);
      
    }else{
      occurance += 1;
      if(occurance == 1){
        straight100(); // start fast
      }else if(occurance == 2){
        straight50(); // gradually slow down
      }else{
        straight25(); // slow down to 25%
      }
      
    }
    
// sample code for pixy library
//    for (i=0; i<pixy.line.numVectors; i++) 
//    {
//      sprintf(buf, "line %d: ", i);
//      Serial.print(buf);
//      pixy.line.vectors[i].print();
//    }

//    // print all intersections
//    for (i=0; i<pixy.line.numIntersections; i++)
//    {
//      sprintf(buf, "intersection %d: ", i);
//      Serial.print(buf);
//      pixy.line.intersections[i].print();
//    }
    
    

    //case1 no new vector (straight)

    //case 2 new vector. (calculate movement using coordinates)

    //case 3 saw at the end of line && set state to 1
    
  }else{
    
    if(firstrun){
      pixy.changeProg("video"); //changing to color identification library
      firstrun = false; //make sure it only run in the first time.
    }
    //switch to color identification library

    //set camera position to up

    //continue moving until close to the object. And close claw
    
  }
  
}

void reverseServo(Servo s){
  if(s.read() == 100){
    s.write(40);
  }else if(s.read() == 60){
    s.write(0);
  }else if(s.read() == 40){
    s.write(100);
  }else if(s.read() == 0){
    s.write(60);
  }
}

// go stragith at 25% power
void straight25(){
  motor_r.setSpeed(64);   // Motor 1 runs forward at 25% speed.
  motor_l.setSpeed(-64);  // Motor 2 runs forward at 25% speed. 
}


// go stragith at 50% power
void straight50(){
  motor_r.setSpeed(128);
  motor_l.setSpeed(-128);
}

// go stragith at 100% power
void straight100(){
  motor_r.setSpeed(256);
  motor_l.setSpeed(-256);   
}

// spin in _ direction. (direction should be int +1 for right and -1 for left.)
// for set ammount of time in millisecond

void spin(int direction, int dur){
        motor_r.setSpeed(256*direction);  // Motor 1 runs forward/backward at 25% speed.
    motor_l.setSpeed(256*direction);  // Motor 2 runs backward/forward at 25% speed.
    delay(dur);
    straight25(); //go straight after time passed.
}

// take in a Vector v and spit out an angle in float.
float calc_turn_angle(struct Vector v){
  double delta_x = v.m_x1 - v.m_x0; // right is positive
  double delta_y = v.m_y0 - v.m_y1; // up is positive
  double angle = atanf(delta_y/delta_x)/6.28*360;
  if(delta_x > 0 && delta_y > 0){
    return 90-angle;
  }else if(delta_x > 0 && delta_y < 0){
    return -1*angle+90;
  }else if(delta_x < 0 && delta_y > 0){
    return -1*angle-90;
  }else if(delta_x < 0 && delta_y < 0){
    return angle+90;
  }
  return 0;
}
