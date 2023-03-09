//
// begin license header
//
// This file is part of Pixy CMUcam5 or "Pixy" for short
//
// All Pixy source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Pixy source code, software and/or
// technologies under different licensing terms should contact us at
// cmucam@cs.cmu.edu. Such licensing terms are available for
// all portions of the Pixy codebase presented here.
//
// end license header
//
// This sketch is a good place to start if you're just getting started with 
// Pixy and Arduino.  This program simply prints the detected object blocks 
// (including color codes) through the serial console.  It uses the Arduino's 
// ICSP SPI port.  For more information go here:
//
// https://docs.pixycam.com/wiki/doku.php?id=wiki:v2:hooking_up_pixy_to_a_microcontroller_-28like_an_arduino-29
//
  
#include <Pixy2.h>
#include <Servo.h>; // for servo
#include <CytronMotorDriver.h>; //for motor driver


// This is the main Pixy object 
int noCan = 2;
Pixy2 pixy;
Servo servo_claw;
Servo servo_arm;


CytronMD motor_l(PWM_DIR, 3, 8);  // PWM 1 = Pin 3, DIR 1 = Pin 8.
CytronMD motor_r(PWM_DIR, 6, 7); // PWM 2 = Pin 6, DIR 2 = Pin 7.

void setup()
{
  Serial.begin(115200);
  Serial.print("Starting...\n");
  
  pixy.init();
  int8_t s = pixy.changeProg("Block");
  Serial.print(s);
  servo_claw.attach(4);
  servo_arm.attach(2);  // rangee up 100-40 down
  servo_arm.write(90);
}

void loop()
{ 
  int i; 
  // grab blocks!
  pixy.ccc.getBlocks(false, CCC_SIG1);
  
  // If there are detect blocks, print them!
  if (pixy.ccc.numBlocks && noCan>=1)
  {
    Serial.print("Detected ");
    Serial.println(pixy.ccc.numBlocks);
    for (i=0; i<pixy.ccc.numBlocks; i++)
    {
        Serial.print("(");
        Serial.print(pixy.ccc.blocks[i].m_x);
        Serial.print(",");
        Serial.print(pixy.ccc.blocks[i].m_y);
        Serial.println(")");
        Serial.println(pixy.ccc.blocks[i].m_width);
        Serial.println(pixy.ccc.blocks[i].m_height);
      if(pixy.ccc.blocks[i].m_width >= 135 && pixy.ccc.blocks[i].m_x > 150 && pixy.ccc.blocks[i].m_x < 160){
        
        servo_claw.write(0);
        delay(100);
        noCan = 0;
        
        
      }else{
        straight25();
        delay(100);
        stopp();        
      }
    }
  }else if(noCan == 0){
    
        back25(); 
  }
  else{

  }
}

void stopp(){
  motor_r.setSpeed(0);   // Motor 1 runs forward at 25% speed.
  motor_l.setSpeed(0);  // Motor 2 runs forward at 25% speed. 
}



// go stragith at 25% power
void straight25(){
  motor_r.setSpeed(64);   // Motor 1 runs forward at 25% speed.
  motor_l.setSpeed(-66);  // Motor 2 runs forward at 25% speed. 
}

void back25(){
  motor_r.setSpeed(-64);   // Motor 1 runs forward at 25% speed.
  motor_l.setSpeed(66);  // Motor 2 runs forward at 25% speed. 
}
