/*
 * ***********************
 * author: J. C. Mariscal
 * MIT License
 * ***********************
 */

 /*
  * *****************************
  * Program: motor_control_slave_v0.1
  * ****************************
  */
#include <Arduino.h>
#include "BasicStepperDriver.h"
#include <SoftwareSerial.h>
#include <Wire.h>
/*
 * /////////////////
 * SLAVE STEPPER
 * /////////////////
 */
#define DIR PD5
#define STEP PD6
// using a 200-step motor.
#define STEPMOTOR 200
BasicStepperDriver slaveStepper(STEPMOTOR, DIR, STEP);
int x = 0; // i2c byte sent from master
int i = 0;

void setup() {
  //init stepper
  slaveStepperInit(10, 1);

  // Start slave
  Wire.begin(2);        // join i2c bus address 2
  Wire.onReceive(receiveEvent);
}

void loop () {
  if (x == '1'){
    slaveStepper.move(1);
    slaveStepper.setRPM(i);
    // set acceleration
    if (i < 50) {
      i++;
    }
  }

  if (x == '2'){
    slaveStepper.move(-1);
    slaveStepper.setRPM(i);
    // set acceleration
    if (i < 50) {
      i++;
    }
  }

  if (x == '3'){
    slaveStepper.move(0);
  }
}

void receiveEvent(int bytes) {
  x = Wire.read();
  slaveStepper.rotate(360);
}

void slaveStepperInit(unsigned int rpm, unsigned int microstep){
  DDRB &= ~(1 << PB0);
  PORTB &= ~(1 << PB0);

  slaveStepper.setRPM(rpm);
  // 1 full microstep, 2 half, etc...
  slaveStepper.setMicrostep(microstep);
  //test rotation
  slaveStepper.rotate(60);
  slaveStepper.rotate(-60);
  slaveStepper.rotate(60);
}

