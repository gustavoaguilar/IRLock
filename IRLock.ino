#include <IRremote.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <Servo.h>


//Defines the pins used
#define RECV_PIN 7
#define SERVO_PIN 9
#define BUZZER_PIN 6

#define LCD_RS_PIN 12
#define LCD_ENABLE_PIN 11
#define LCD_D4_PIN 5
#define LCD_D5_PIN 4
#define LCD_D6_PIN 3
#define LCD_D7_PIN 2

//function used to reset the screen of the LCD
void resetLCD();

//function used to produce beeps with the buzzer
void myTone(byte pin, uint16_t frequency, uint16_t duration);

//Global variables

IRrecv irrecv(RECV_PIN); //Starts the IR receiver
decode_results results; //Stores the code decoded by the IR receiver
Servo servo; //Creates a servo Object

String buff = ""; // Used to store temporarily the password that the user types 
String password = ""; // Used to store the password

uint8_t flagNewPassword = 1; // 1 when a new password needs to be set, 0 when not
uint8_t flagLocked = 0; // 1 when locked, 0 when opened

//Initialize the LCD
LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

/*
0 to 9 - NUMBERS
10 - RESET
11 - ENTER
12 - NEW PASSWORD
*/
long unsigned int keys[13] {0x207F08F7, 0x207F8877, 0x207F48B7, 0x207FC837, 0x207F28D7, 0x207FA857, 0x207F6897,
0x207FE817, 0x207F18E7, 0x207F9867, 0x207F38C7, 0x207FF807, 0x207F906F};

void setup(){
  Serial.begin(9600); //Starts a serial connection
  irrecv.enableIRIn(); //Enables the IR receiver
  servo.attach(SERVO_PIN); //Start the servo
  servo.write(10); // Position the servo in 10 degrees
  pinMode(BUZZER_PIN, OUTPUT); //Set the buzzer pin to output
  lcd.begin(16, 2); //Starts the lcd
  resetLCD();
}


void loop(){
  //if a IR signal is received
  if (irrecv.decode(&results)){
    
    myTone(6,800,50); // play a beep
    
    //checks if the code is one of the 10 numbers
    for(int i = 0; i<10; i++){
      if(results.value == keys[i]){
        lcd.print("*"); // displays a * to represent the number
        buff = buff+i; // add the number to the buffer string
        }
    }

  //Pressed delete, clears the buffer and resets the LCD
  if(results.value == keys[10]){
    buff = "";
    resetLCD();
  }

  //When Enter is pressed
  if(results.value == keys[11]){
      //New password
      if(flagNewPassword == 1){
        password = buff;
        flagNewPassword = 0;
        lcd.clear();
        lcd.setCursor(6, 0);
        lcd.print("Key");
        lcd.setCursor(4, 1);
        lcd.print("Accepted");
        delay(1000);
        resetLCD();
      }
      //Access Granted
      else if(password == buff){
        
        if(flagLocked){
          lcd.clear();
          lcd.setCursor(5, 0);
          lcd.print("Access");
          lcd.setCursor(5, 1);
          lcd.print("Granted");
          servo.write(10);
          flagLocked = 0;
        }else{
          lcd.clear();
          lcd.setCursor(5, 0);
          lcd.print("Device");
          lcd.setCursor(5, 1);
          lcd.print("Locked");
          servo.write(170);
          flagLocked = 1;
        }
        
        myTone(6,5000,50);
        myTone(6,6000,100);
        delay(2000);
        resetLCD();
      }
      //Access Denied
      else
      {
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.print("Access");
        lcd.setCursor(5, 1);
        lcd.print("Denied");

        myTone(6,1000,50);
        myTone(6,500,100);

        delay(2000);
        resetLCD();
      }
      buff = ""; //clears the buffer after Enter Pressed
  }
  //Pressed new password
  if(results.value == keys[12]){
    flagNewPassword = 1; //set the flag to up, so next time it will need to set the new password
    buff = ""; //clears the buffer
    resetLCD();
  }
    irrecv.resume();
    //Serial.println(results.value, HEX);
  }
}

//function to set the default screen
void resetLCD()
{
  //if a new password needs to be set
  if(flagNewPassword){
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Set key:");
    lcd.setCursor(0, 1);
    return;
  }
  //screen to type the password
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Type the key:");
  lcd.setCursor(0, 1);
}

/*
  Got this function from
  User @jurs 
  at
  forum.arduino.cc/index.php?topic=339625.0
 */
void myTone(byte pin, uint16_t frequency, uint16_t duration)
{ // input parameters: Arduino pin number, frequency in Hz, duration in milliseconds
  unsigned long startTime=millis();
  unsigned long halfPeriod= 1000000L/frequency/2;
  pinMode(pin,OUTPUT);
  while (millis()-startTime< duration)
  {
    digitalWrite(pin,HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(pin,LOW);
    delayMicroseconds(halfPeriod);
  }
  pinMode(pin,INPUT);
}
