/*
	Programmer's name: BedSecured
	Programmer: Jaycee Alipio
	Date: 2/20/2023
	Version: 2.0.3
	Description:  Uses ultrasonic sensor to detect change in movement and sends signal to phone using wifi
*/

//Defines variables for ultrasonic sensor
int trigPin = 19;
const int echoPin = 18;
#define MAX_DISTANCE 700
//timeOut= 2*MAX_DISTANCE /100 /340 * 1000000 = MAX_DISTANCE*58.8
float timeOut = MAX_DISTANCE * 60;
int soundVelocity = 340; //define sound speed = 340 m/s

int latchPin = 2;
int clockPin = 4;
int dataPin = 15;
int comPin[] = {25,26,27,14}; //Comm pin (anode) of 4 digit 7-segment display
int detectionCount = 0;
int seconds = 0, minutes = 0;

//Defines the encoding of characters 0-F of the common-anode 7-Segment Display
byte num[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  for (int i=0; i < 4; i++) {
    pinMode(comPin[i], OUTPUT);
  }
}


void loop() {
  for (int i=0; i < 4; i++) {
    //Select a single 7-segment display
    electDigitalDisplay (i);
    //Send data to 74HC595
    writeData(num[i]);
    delay(5);
    //Clear the display content
    writeData(0xff);
  }
  
  if (getSonar() < 70.0 ) {
    detectionCount = detectionCount + 1;
    Serial.print(". Detected ");
    Serial.print(detectionCount);
    Serial.print(" times!");
    delay(10000);
  }

    Serial.print(" Distance: ");
    Serial.print(getSonar()); //Send ping, get distance in cm and print
    Serial.println(" cm ");
}

void electDigitalDisplay(byte com) {
  //Close all single 7-segment display
  for (int i=0; i <4; i++) {
    digitalWrite(comPin[i], LOW);
  }
  digitalWrite(comPin[com], HIGH);
}


void writeData(int value) {
 // Make latchPin output low level
 digitalWrite(latchPin, LOW);
 // Send serial data to 74HC595
 shiftOut(dataPin, clockPin, LSBFIRST, value); // Make latchPin output high level
// Make latchPin output high level, then 74HC595 will update data to parallel output
 digitalWrite(latchPin, HIGH);
}


float getSonar() {
  unsigned long pingTime;
  float distance;
  //make trigPin output high level lasting for 10us to trigger HC_SR04
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  //Wait HC-SR04 returning to the high level and measure out this waiting time
  pingTime = pulseIn(echoPin, HIGH, timeOut); 
  // calculate the distance according to the time
  distance = (float)pingTime * soundVelocity / 2 / 10000; 
  return distance; // return the distance value

}