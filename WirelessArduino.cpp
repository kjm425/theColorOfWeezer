#include <Arduino.h>
#include <SPI.h>
#include "RF24.h"
int latchPin = 5;
int clockPin = 6;
int dataPin = 4;
int outputEnablePin = 3;
int lvl = 0;
 byte addresses[][6] = {"1Node","2Node"};
 /***      Set this radio as radio number 0 or 1 0 for led arduino        ***/
bool radioNumber = 1;
// Used to control whether this node is sending or receiving 0 for led arduino
bool role = 1;
struct dataStruct{
  unsigned long _micros;
  float value;
}myData;
RF24 radio(7,8);
byte leds = 0;
void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, leds);
   digitalWrite(latchPin, HIGH);
}
 
void setBrightness(byte brightness) // 0 to 255
{
  analogWrite(outputEnablePin, 255-brightness);
}
 

void setup() 
{
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(outputEnablePin, OUTPUT);
  Serial.begin(115200);
  radio.begin();
  myData.value = 0;
  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
  
  // Start the radio listening for data
  radio.startListening();
  setBrightness(255);
  leds = 0;
  updateShiftRegister();
  delay(500);
    for (int i = 0; i < 8; i++)
  {
    bitSet(leds, i);
    updateShiftRegister();
    delay(5);
  }
}
 
void loop() {
  if (role == 1)  {
    
    radio.stopListening();
                                       // First, stop listening so we can talk.
    myData.value = Serial.parseInt();      
     if (!radio.write( &myData, sizeof(myData) ) ){
       Serial.println(F("failed"));
     }else{
     }
    
    radio.startListening();                                                   // Set up a variable to indicate if a response was received or not

  }



/****************** Pong Back Role ***************************/

   if ( role == 0 )
  {
    
    if( radio.available()){
                                                           // Variable for the received timestamp
      while (radio.available()) {                          // While there is data ready
        radio.read( &myData, sizeof(myData) );             // Get the payload
      }
     
      radio.stopListening();                               // First, stop listening so we can talk  
                                      // Increment the float value
      setBrightness(myData.value);              // Send the final one back.      
      radio.startListening();                              // Now, resume listening so we catch the next packets.     
   }
 }
    
    
}
