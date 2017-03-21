/*
 * 21 Mar 17 - Dane Lennon
 * 
 * This is a script writen for the Arduino Uno for the CSA Student Day and
 * uses the Multitech mDOT LoRa module running the Australian compatable AT
 * enabled firmware.
 * 
 * This program,
 *  Joins the LoRa Network
 *  Waits for motion to be sensed 
 *  Sends the following fields: 
 *    - alert: 1/0 
 *    - count: # of ticks since last reboot 
*/

/*--------------------------------------------------------------------------------------
  Includes
  --------------------------------------------------------------------------------------*/
#include <LoRaAT.h>                              //Include LoRa AT libraray

/*--------------------------------------------------------------------------------------
  Definitions
  --------------------------------------------------------------------------------------*/
LoRaAT mdot;                                     //Instantiate a LoRaAT object
int interruptPin = 2;                                 //attach the OUT pin of the PIR to pin 2 on the Uno

/*--- setup() --------------------------------------------------------------------------
  Called by the Arduino framework once, before the main loop begins.

  In the setup() routine:
   - Opens serial communication with MDOT
  --------------------------------------------------------------------------------------*/
void setup() {
  pinMode(interruptPin, INPUT_PULLUP); 

  attachInterrupt(digitalPinToInterrupt(interruptPin), updateCount, FALLING);
  
  int responseCode;                              //Response of mDot commands
  mdot.begin();                                  //Opens serial comms with MDOT

  do {
    responseCode = mdot.join();
    delay(10000);
  } while (responseCode != 0);
}

/*--- loop() ---------------------------------------------------------------------------
  Main loop called by the Arduino framework
  --------------------------------------------------------------------------------------*/
int alert = 0;
int count = 0; 
void loop() {
  //int responseCode;                              //Response code from the mdot

  char msg[15];                                  //cmd = {'a', 'l', 'e', 'r', 't', ':', '#', ',', 
                                                 //       'c', 'o', 'u', 'n', 't', ':', '#', '#', '#'}
  sprintf(msg,"alert:%d,count:%d",alert,count);
  mdot.sendPairs(msg); 
  alert = 0; 
  
  delay(10000);
}

void updateCount(){
  alert = 1; 
  ++count; 
}

