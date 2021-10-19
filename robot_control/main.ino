/*
 * ----------------------------------
 * Copyright (c) J. C. Mariscal
 * MIT License
 * ----------------------------------
 */

/*
* ----------------------------------
* Program: watchobot motor control
* ---------------------------------
*/
#include <Arduino.h>
#include "BasicStepperDriver.h"
#include <SoftwareSerial.h>

/** ******************
 * STEPPERS
 * *******************/
#define DIR 5 // PD5
#define STEP 6 // PD6
#define DIR2 9         // PB1
#define STEP2 10       // PB2
// using a 200-step motor.
#define STEPMOTOR 200
int g_RPM = 30;
int g_microstep = 16;

BasicStepperDriver masterStepper(STEPMOTOR, DIR, STEP);
BasicStepperDriver stepper2(STEPMOTOR, DIR2, STEP2);
int g_motorState =3; //non-movement state
int i = 0;
int g_j = 0;

/* *************
 * ESP8266
 * *************/
#define DEBUG true
String ssid =  "SSID"; // add ssid of the wifi here
String pass = "pass"; // add password of the wifi here
SoftwareSerial debugSerial(PD3,PD4); //for debugging serial

/********************
 * FUNCTIONS INITS
 *********************/
String sendData(String command, const int timeout, boolean debug);
void startServer();
void stepperInit(unsigned int rpm, unsigned int microstep);
void moveMotor();
void checkUserCommand(String command);

/***************************************************************************/

// SETUP
void setup() {
  //init stepper
  stepperInit(g_RPM, g_microstep);

  //init serial
  Serial.begin(115200);
  debugSerial.begin(9600);
  debugSerial.println("serial starting");

  // Start esp8266 server
  startServer();
}

/***************************************************************************/

// MAIN LOOP
void loop() {
  if ( Serial.available() ) {
    if (Serial.find("+IPD,") ) {
      String msg;
      Serial.find("?");
      msg = Serial.readStringUntil(' '); //read the whole text
      String commandReceived = msg.substring(0, 2);
      String valueStr = msg.substring(4);
      
      if (DEBUG){
        debugSerial.println(commandReceived);
        debugSerial.println(valueStr);
      }
      /*
      String newRPM = msg.substring(0, 3);
      if (newRPM.charAt(0) == "a"){
        newRPM = newRPM.substring(1,3);
        g_RPM = newRPM.toInt();
        stepper2.setRPM(g_RPM);
        masterStepper.setRPM(g_RPM);
      }
      */
      
      checkUserCommand(commandReceived);
      }

    }
  moveMotor(g_motorState);
}

/****************************************************************************/
/*
 * //////////////
 * MOTOR FUNCTIONS
 * //////////////
 */

/*
 * ***********************
 * Init for master stepper
 * ***********************
 */
void stepperInit(unsigned int rpm, unsigned int microstep){
  DDRB &= ~(1 << PB0);
  PORTB &= ~(1 << PB0);
  DDRB &= ~(1 << PD7);
  PORTB &= ~(1 << PD7);

  masterStepper.setRPM(rpm);
  // 1 full microstep, 2 half, etc...
  masterStepper.setMicrostep(microstep);
  //test rotation

  stepper2.setRPM(rpm);
  stepper2.setMicrostep(microstep);
  masterStepper.rotate(10);
  masterStepper.rotate(-10);
  stepper2.rotate(10);
  stepper2.rotate(-10);
}

/*
 * ****************
 * Motor movement: uses global variable state
 * ****************
 */
void moveMotor(int motorState){
  // move motor forward
  if (motorState ==1) {
    masterStepper.move(1);
    masterStepper.setRPM(i);
    //stepper2 -1 is forward
    stepper2.move(-1);
    stepper2.setRPM(i);
    ++g_j;
    
    // set acceleration
    if (i < g_RPM && (g_j % 100 == 0)){
      i++;
    }
  }

  // move motor backward
  if (motorState == 2) {
    masterStepper.move(-1);
    masterStepper.setRPM(i);
    // stepper2, move 1 is backwards
    stepper2.move(1);
    stepper2.setRPM(i);
    ++g_j;
    // set acceleration
    if (i < g_RPM && (g_j % 100 == 0)){
      i++;
    }
  }

  // stop
  if (motorState == 3){
    masterStepper.move(0);
  }

  // move right
  if (motorState == 4){
    masterStepper.move(1);
    masterStepper.setRPM(2);
    stepper2.move(-1);
    stepper2.setRPM(i);
    // set acceleration
    if (i < g_RPM){
      i++;
    }
  }

  // move left
  if (motorState == 5){
    masterStepper.move(1);
    masterStepper.setRPM(i);
    stepper2.move(-1);
    stepper2.setRPM(2);

    // set acceleration
    if (i < g_RPM){
      i++;
    }
  }
  // pivot right
  if (motorState == 6){
    masterStepper.move(-1);
    masterStepper.setRPM(1);
    stepper2.move(-1);
    stepper2.setRPM(i);
    // set acceleration
    if (i < g_RPM){
      i++;
    }
  }

  // pivot left
  if (motorState == 7){
    masterStepper.move(1);
    masterStepper.setRPM(i);
    stepper2.move(1);
    stepper2.setRPM(1);
    // set acceleration
    if (i < g_RPM){
      i++;
    }
  }

}

/*
 * ///////////////////
 * ESP8266 FUNCTIONS
 * ///////////////////
 */
/* 

/*
 * *****************
 * sendData function: Sends command to the ESP8266 and returns response.
 * *****************
 */
String sendData(String command, const int timeout, boolean debug)
{
    debugSerial.println("sending data");
    String response = "";
    
    Serial.print(command); // send the read character to the mySerial
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(Serial.available())
      {
        
        // The esp has data so display its output to the serial window 
        char c = Serial.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      debugSerial.print(response);
    }
    
    return response;
}

 /*  ****************
 *  esp8266 server: uses global variable ssid, pass.
 *  ****************
 */
void startServer(){
  debugSerial.println("starting server");
  // reset module
  sendData("AT+RST\r\n",2000,DEBUG);
  // configure as access point
  sendData("AT+CWMODE=1\r\n",1000,DEBUG);
  // connect to wifi
  sendData("AT+CWJAP=\"" + ssid +"\",\"" + pass + "\"", 1000, DEBUG);
  // get ip address
  sendData("AT+CIFSR\r\n",1000,DEBUG);
  // configure multiple connections
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG);
  // use port 80 and turn on the server
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG);
}


/*
 * ****************************
 * WATCHOBOT COMMANDS FUNCTION: set event according to string command.
 * ****************************
 */
void checkUserCommand(String command){
  //move forward
  if (command == "01"){
    i = 1;
    g_motorState = 1;
    g_j = 0;
  }
  // move backwards
  if (command == "02"){
    i = 1;
    g_motorState = 2;
    g_j = 0;
  }
  // stop
  if (command == "03"){
    i = 1;
    g_motorState = 3;
  }
  // move right
  if (command == "04"){
    i = 1;
    g_motorState = 4;
  }
  // move left
  if (command == "05"){
    i = 1;
    g_motorState = 5;
  }
  // increase RPM
  if (command == "06"){
    ++g_RPM;
    if (g_RPM == 4000){
      g_RPM = 3;
    }
  }

  // decrease RPM
  if (command == "07"){
    --g_RPM;
    if (g_RPM == 1){
      g_RPM = 1;
    }
  }

  // reset RPM default
  if (command == "08"){
    g_RPM = 3;
    masterStepper.setMicrostep(16);
    stepper2.setMicrostep(16);
    
  }
  // full step mode
  if (command == "09"){
    masterStepper.setMicrostep(1);
    stepper2.setMicrostep(1);
    
  }
  // 1/2 microstep
  if (command == "10"){
    masterStepper.setMicrostep(2);
    stepper2.setMicrostep(2);
  }
  // 1/4 microstep
  if (command == "11"){
    masterStepper.setMicrostep(4);
    stepper2.setMicrostep(4);
  }
  // 1/8 microstep
  if (command == "12"){
    masterStepper.setMicrostep(8);
    stepper2.setMicrostep(8);
  }

  if (command == "13"){
    g_motorState = 6;
    i = 0;
  }
  
  if (command == "14"){
    g_motorState = 7;
    i = 0;
  }
}

