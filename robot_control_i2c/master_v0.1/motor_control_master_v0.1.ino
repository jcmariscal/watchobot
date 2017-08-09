/*
 * ***********************
 * author: J. C. Mariscal
 * MIT License
 * ***********************
 */

/*
* **********************************
* Program: motor_control_masterv0.1
* **********************************
*/
#include <Arduino.h>
#include "BasicStepperDriver.h"
#include <SoftwareSerial.h>
#include <Wire.h>

/*
 * /////////////////
 * MASTER STEPPER
 * /////////////////
 */
#define DIR PD5
#define STEP PD6
// using a 200-step motor.
#define STEPMOTOR 200
BasicStepperDriver masterStepper(STEPMOTOR, DIR, STEP);
int state =5; //non-movement state
int i = 0;

/*
 * ////////////
 * ESP8266
 * ////////////
 */
 #define DEBUG true
String ssid =  "WLAN-XAF3KZ";
String pass = "6162409967327183";
SoftwareSerial debugSerial(PD3,PD4); //for debugging serial
//SoftwareSerial espSerial(PD3, PD4);

// functions init
String sendData(String command, const int timeout, boolean debug);
void startServer();
void MasterStepperInit(unsigned int rpm, unsigned int microstep);
void moveMotor();

// SETUP
void setup() {
  //init stepper
  MasterStepperInit(1, 1);

  //init serial
  Serial.begin(115200);
  debugSerial.begin(9600);
  debugSerial.println("serial starting");

  // Start esp8266 server
  startServer();
  // Start master
  Wire.begin();        // join i2c bus (address optional for master)
  
  
}


// MAIN LOOP
void loop() {
  if ( Serial.available() ) {
    if (Serial.find("+IPD,") ) {
      String msg;
      Serial.find("?");
      msg = Serial.readStringUntil(' '); //read the whole text
      String command = msg.substring(0, 2);
      String valueStr = msg.substring(4);
      
      if (DEBUG){
        debugSerial.println(command);
        debugSerial.println(valueStr);
      }

      //move forward
      if (command == "01"){
        i = 0;
        state = 1;
      }
      // move backwards
      if (command == "02"){
        i = 0;
        state = 2;
      }
      // stop
      if (command == "03"){
        i = 0;
        state = 3;
      }
    }
  }
  moveMotor();
}
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
void MasterStepperInit(unsigned int rpm, unsigned int microstep){
  DDRB &= ~(1 << PB0);
  PORTB &= ~(1 << PB0);

  masterStepper.setRPM(rpm);
  // 1 full microstep, 2 half, etc...
  masterStepper.setMicrostep(microstep);
  //test rotation
  masterStepper.rotate(60);
  masterStepper.rotate(-60);
  masterStepper.rotate(60);
}

/*
 * ****************
 * Motor movement: uses global variable state
 * ****************
 */
void moveMotor(){
  // move motor forward
  if (state ==1) {
    Wire.beginTransmission(2); //begin transmission to device 2 (i.e. slave board)
    Wire.write(1);
    Wire.endTransmission(); 
    masterStepper.move(1);
    masterStepper.setRPM(i);
    /*
    stepper2.move(1);
    stepper2.setRPM(i);
    */
    // set acceleration
    if (i < 50){
      i++;
    }
  }

  // move motor backward
  if (state == 2) {
    Wire.beginTransmission(2); //begin transmission to device 2 (i.e. slave board)
    Wire.write(2);
    Wire.endTransmission();
    masterStepper.move(-1);
    masterStepper.setRPM(i);
    // set acceleration
    if (i < 50){
      i++;
    }
  }

  // stop
  if (state == 3){
    Wire.beginTransmission(2); //begin transmission to device 2 (i.e. slave board)
    Wire.write(3);
    Wire.endTransmission();
    masterStepper.move(0);
  }
}

/*
 * ///////////////////
 * ESP8266 FUNCTIONS
 * ///////////////////
 */
/* 

/*
 * ***************
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
 * Read web request as string after character ? and change state of motor
 * ****************************
 */
 String changeMotorState(){

 }
 

