// Optical HR/Pulse monitor
// Embedded systems project April 2022
//Susan Tunkkari NITS19K 
//Version 1.2 - Final version


#define USE_ARDUINO_INTERRUPTS true // Set-up low-level interrupts for most acurate BPM. 
#include <PulseSensorPlayground.h> //Include PulseSensorPlayground Library. 
#include <LiquidCrystal.h> // Include LiquidCrystal Library.


LiquidCrystal lcd(10, 9, 3, 4, 7, 8); // lcd(RS,E,D4,D5,D6,D7).


const int PulseSensorWire = A3; // PulseSensor connected to A3
const int LED_green = 5; //green LED connected to pin 5
const int LED_red = 6; //red LED connected to pin 6
const int Buzzer = 2; // Buzzer connected to pin 2
int Threshold = 550; //Determine which Signal to count as a beat - default value for green LED 550- may need adjusting for sensitivity


//Determine which pixels to turn on and off to create heart
byte heart1[8] = {B00011, B00111, B01111, B11111, B11111, B11111, B11111, B01111};
byte heart2[8] = {B11000, B11100, B11110, B11111, B11111, B11111, B11111, B11111};
byte heart3[8] = {B00011, B00111, B01111, B11111, B11111, B11111, B11111, B11111};
byte heart4[8] = {B11000, B11100, B11110, B11111, B11111, B11111, B11111, B11110};
byte heart5[8] = {B00011, B00001, B00000, B00000, B00000, B00000, B00000, B00000};
byte heart6[8] = {B11111, B11111, B11111, B11111, B01111, B00111, B00011, B00001};
byte heart7[8] = {B11111, B11111, B11111, B11111, B11110, B11100, B11000, B10000};
byte heart8[8] = {B11000, B10000, B00000, B00000, B00000, B00000, B00000, B00000};

                               
PulseSensorPlayground pulseSensor; //Create instance of the PulseSensorPlayground 


void setup() {   
  Serial.begin(9600);// Set Serial Communication at 9600 baud (bits per second).
  lcd.begin(16, 2); // Initialize interface to LCD screen - set dimentions for screen (width and height)
     
  //create char segments for heart
  lcd.createChar(1, heart1);
  lcd.createChar(2, heart2);
  lcd.createChar(3, heart3);
  lcd.createChar(4, heart4);
  lcd.createChar(5, heart5);
  lcd.createChar(6, heart6);
  lcd.createChar(7, heart7);
  lcd.createChar(8, heart8);

  //set cursor to correct part of screen and print message
  lcd.setCursor(0,0);
  lcd.print("HR/Pulse Monitor"); 
  //set cursor to beginning of next line and print message to it
  lcd.setCursor(0,1);
  lcd.print(" Welcome! ");
  delay(2000);

 
  pulseSensor.analogInput(PulseSensorWire);  
  pulseSensor.blinkOnPulse(LED_green); // blink LED with heartbeat.
  pulseSensor.setThreshold(Threshold);
  pinMode(LED_red, OUTPUT);
 

  if (pulseSensor.begin()) {
    lcd.setCursor(0,1);
    lcd.print("Lets get started"); // prints once upon power up/reset
  }
  
  delay(2000);
  lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("Place finger ");
    lcd.setCursor(0,1);
    lcd.print(" on the sensor  "); 
    delay(3000);
    lcd.clear();
    delay(500);
}

void loop() {
  int myBPM = pulseSensor.getBeatsPerMinute(); // Calls function of the pulse senor object. This works out the HR - BPM is now held as myBPM. 
 
  if (pulseSensor.sawStartOfBeat()) { // if start of beat found 
      tone(Buzzer,700, 10); // pin variable, tone , length of tone
 
   // print heart segments
    lcd.setCursor(0,0);
    lcd.write(byte(1));
    lcd.setCursor(1,0);
    lcd.write(byte(2));
    lcd.setCursor(2,0);
    lcd.write(byte(3));
    lcd.setCursor(3,0);
    lcd.write(byte(4));
    lcd.setCursor(0,1);
    lcd.write(byte(5));
    lcd.setCursor(1,1);
    lcd.write(byte(6));
    lcd.setCursor(2,1);
    lcd.write(byte(7));
    lcd.setCursor(3,1);
    lcd.write(byte(8));


  //print Heart Rate as BPM 
  //Set paramaters 
  if(myBPM >100){
    digitalWrite(LED_red, HIGH);// turn on red light if >100BPM
    lcd.setCursor(5,0);
    lcd.print("Heart Rate");
    lcd.setCursor(5,1);
    lcd.print(myBPM);
    lcd.print("  BPM HI  "); //Indicate in display HI along with BPM
  }
    else if( myBPM < 50){
    digitalWrite(LED_red, HIGH);// turn on red light if <50BPM
    lcd.setCursor(5,0);
    lcd.print("Heart Rate");
    lcd.setCursor(5,1);
    lcd.print(myBPM);
    lcd.print("  BPM Lo  ");//Indicate Lo in display along with BPM
  }
   else{
    digitalWrite(LED_red, LOW);// turn off when within paramaters
    lcd.setCursor(5,0);
    lcd.print("Heart Rate");
    lcd.setCursor(5,1);
    lcd.print(myBPM);
    lcd.print("  BPM    ");//Display BPM 
    }
    
  }

  delay(20); 
}
