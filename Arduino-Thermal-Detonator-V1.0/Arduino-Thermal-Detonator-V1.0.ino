/*
   Thermal Detonator V1.0

   Released on: 21st June 2019
   Author:    PropTroniX (info@proptronix.co.uk)
   Source :   https://github.com/PropTroniX/Thermal-Detonator-V1.0
   Description: Light and Sound for a 3D Printed Class-A Thermal Detonator

   Electronics Parts Available at www.proptronix.co.uk
   Thermal Detonator V1.0 STL Files are Included FREE in the GitHub.
   
   This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
   To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/.

*/

#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(7, 6); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

enum {NORMAL, SPECIAL} currentState = NORMAL;

//suffix N for Normal
byte led1StateN [] = {1, 1, 1, 1, 0, 0, 0, 1, 0};
byte led2StateN [] = {1, 0, 1, 0, 1, 1, 0, 1, 0};
byte led3StateN [] = {1, 0, 0, 1, 1, 0, 1, 0, 0};

//suffix S for Special
byte led1StateS [] = {1, 1, 1, 1, 1, 1, 1, 1, 0};
byte led2StateS [] = {1, 0, 1, 0, 1, 0, 0, 1, 0};
byte led3StateS [] = {1, 0, 0, 0, 0, 0, 1, 0, 0};

byte ledPin1 = 3;
byte ledPin2 = 4;
byte ledPin3 = 5;

int ledPinON = 2;
int triggerPin = 8;

int buttonState = 0;

unsigned long currentMillis;
unsigned long previousMillisPulse = 0;
int pulseInterval = 500;
bool pulseState = LOW;
unsigned long previousMillisBlink = 0;
int blinkInterval = 1000;

byte arrayPos = 0;
int passNumber = 1;

byte specialButtonPin = 14;

void setup()
{

  pinMode(triggerPin, INPUT);
  pinMode(ledPinON, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin2, LOW);
  pinMode(specialButtonPin, INPUT_PULLUP);


  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    while(true){
    }
  }
  
  myDFPlayer.volume(25);  //Set volume value. From 0 to 30


  myDFPlayer.play (3);
  delay (1500);

  myDFPlayer.play (2);
  delay (300);

}//setup

void loop()
{

  digitalWrite(ledPinON, HIGH);

  buttonState = digitalRead(triggerPin);

  if (buttonState == HIGH) {

    bomb ();

  } else {

    lights ();

  }
}


void lights()
{

  currentMillis = millis();
  doPulse();
  doStates();

}//lights

void doPulse()
{
  if (currentMillis - previousMillisPulse >= pulseInterval)
  {
    pulseState = !pulseState;
    digitalWrite(LED_BUILTIN, pulseState);
    previousMillisPulse = currentMillis;


  }
}

void doStates()
{

  switch (currentState)
  {
    case NORMAL:

      if (currentMillis - previousMillisBlink >= blinkInterval)
      {
        passNumber++;
        digitalWrite(ledPin1, led1StateN [arrayPos]);
        digitalWrite(ledPin2, led2StateN [arrayPos]);
        digitalWrite(ledPin3, led3StateN [arrayPos]);
        arrayPos++;
        if (arrayPos > 8)
        {
          arrayPos = 0;
        }
        previousMillisBlink = currentMillis;
      }

      if (!digitalRead(specialButtonPin))
      {
        currentState = SPECIAL;
        arrayPos = 0;
        previousMillisBlink = currentMillis;
      }

      break;

    case SPECIAL:

      if (currentMillis - previousMillisBlink >= blinkInterval)
      {
        passNumber++;
        digitalWrite(ledPin1, led1StateS [arrayPos]);
        digitalWrite(ledPin2, led2StateS [arrayPos]);
        digitalWrite(ledPin3, led3StateS [arrayPos]);
        arrayPos++;

        previousMillisBlink = currentMillis;
      }

      if (arrayPos > 8)
      {
        currentState = NORMAL;
        arrayPos = 0;
//        Serial.println(" ******* Going to NORMAL ********");
      }

      break;

  }//switch
}//doStates

void bomb ()  {

  myDFPlayer.play (1);
  delay (300);

  for (int counter = 1; counter <= 40; counter = counter + 1) {
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, HIGH);
    digitalWrite(ledPinON, LOW);
    delay(100);
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPinON, HIGH);
    delay(100);

  }
}
