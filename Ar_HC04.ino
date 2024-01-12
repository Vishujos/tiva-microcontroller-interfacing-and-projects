#define echoPin 6
#define triggerPin 7
#include <TFT.h>  
#include <SPI.h>

// pin definition for Arduino UNO
#define cs   10
#define dc   9
#define rst  8


// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);
void setup() {
   pinMode(triggerPin, OUTPUT);       // Sets the trigPin as an OUTPUT
   pinMode(echoPin, INPUT);          // Sets the echoPin as an INPUT
   Serial.begin(9600);             // Serial Communication is starting with 9600 of baudrate speed
   Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
pinMode(2,OUTPUT);
   Serial.println("with Arduino UNO");
   //initialize the library
  TFTscreen.begin();

  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);
  //set the text size
  TFTscreen.setTextSize(1);
}
void loop() {
   long highPulseDuration;
   int calculatedDistanceCm;

   //Set the trigPin to low, before setting it to high for the pulse
   digitalWrite(triggerPin, LOW);
   delayMicroseconds(5);

   // Create the 10 seconds pulse on the trig pin
   digitalWrite(triggerPin, HIGH);
   delayMicroseconds(10);

   // Set the pin to low to end the pulse
   digitalWrite(triggerPin, LOW);

   // Read the duration of the high pulse on the echo pin
   highPulseDuration = pulseIn(echoPin, HIGH);

   // Calculating the distance
   calculatedDistanceCm = highPulseDuration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)

   // Displays the distance on the Serial Monitor
   Serial.print("Calculated Distance: ");
   Serial.print(calculatedDistanceCm);
   Serial.println(" cm");
   delay(1000);
   if (calculatedDistanceCm<=10){
    digitalWrite(2,HIGH);
    delay(1000);
   }
   else{
    digitalWrite(2,LOW);
    delay(1000);
   }
   int redRandom = random(50);
   // set a random font color
  TFTscreen.stroke(redRandom);
  TFTscreen.text("CalDist:", 6, 57);
  TFTscreen.print(calculatedDistanceCm);
  
}
