// Sonar and Rfid Prototype Code
// ----------------------
// The orientation of the sash must be in the closed position to initialize and start the program. 
// The program will check if the sash is closed or not from the RFID, if the sash is opened then the sonar will begin measuring distance and timer will start
// When the sash is closed again sonar will stop measuring and the timer will be reset
// The program only displays the values onto the serial monitor and does not save the distance or the time the sash has been opened.
// Author: Willie Chow w3chow@ucsd.edu 

#include <MFRC522.h>
#include <SPI.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

// Constants for Sonar Sensor 
const int trigPin = 7; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 6; // Echo Pin of Ultrasonic Sensor

// defines variables for Sonar Sensor
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
long inches, cm;
long openedTime,startTime, currentTime; // variables for the timer

void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  
  // Set up Sonar Sensor
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  
  // Set up RFID
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  mfrc522.PCD_SetAntennaGain(0xFF); // Set gain to max of 48dB
  Serial.println("Beginning Sonar and RFID Sensor...");
  Serial.println();
}
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  bool RFID_present = true;
  int counter = 0;
  bool current, previous; 
  String content= "";
  openedTime=0; // reset the timer
  previous = ! mfrc522.PICC_IsNewCardPresent();
  while(RFID_present)
  {
    // check to see if RFID tag is present --> if check results is two consecutive FALSE RFID is NOT present 
    current =!mfrc522.PICC_IsNewCardPresent();
    if (current && previous)
    {
      counter++;
      previous = current;
      if (counter>2)
      {
        RFID_present = false;
        content = "";
        break;
      }
    }
    else
    {     
      //Show UID on serial monitor
      Serial.print("UID tag :");
      content= "";
      byte letter;
      for (byte i = 0; i < mfrc522.uid.size; i++) 
      {
         Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
         Serial.print(mfrc522.uid.uidByte[i], HEX);
         content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
         content.concat(String(mfrc522.uid.uidByte[i], HEX));
      }
      Serial.println();
      Serial.print("Message : ");
      content.toUpperCase();
      
      // RFID detect Tag --> sash is in closed position
      if (content.substring(1) == "36 E0 28 32" || content.substring(1) == "FC FD 65 32" || content.substring(1) == "5B 30 5A B2") //change here the UID of the card/cards that you want to give access
      {
        //Serial.println("Authorized access");
        Serial.println("Sash Closed");
        Serial.println();
        delay(3000);  // change delay to speed up checking tag process 
      }
    }
  }
  if(!RFID_present)
  {
    Serial.println("Card was removed. Enabling Sonar...");
    startTime=millis(); // get starting time for the timer
    while(! mfrc522.PICC_IsNewCardPresent()) // enable sonar until the RFID tag is detected again
    {
      // Clears the trigPin condition
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      
      // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      
      // Reads the echoPin, returns the sound wave travel time in microseconds
      duration = pulseIn(echoPin, HIGH);
      
      // Calculating the distance
      distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
      //inches = microsecondsToInches(duration);
      cm = microsecondsToCentimeters(duration);
     
      // Displays the distance on the Serial Monitor
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" cm");
      delay(500); // change delay to output and detect faster
    }
    currentTime=millis(); // get end time for the timer
    openedTime=currentTime-startTime; // get time for timer
    Serial.print("Total time opened (ms): ");
    Serial.println(openedTime);
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
} 

// function to print out distance of sonar in inches 
long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

// function to print out distance of sonar in centimeters 
long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
