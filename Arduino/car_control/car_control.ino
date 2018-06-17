/*
 * ” Car control” 
 * Utveckla en app som autonomt kan köra en liten 4hjulig bil i ett rum och undvika alla hinder 
 * i dess väg, samt att kunna köra en rundbana känna igen banan och optimera kommande varv på banan.
 * Fas 1: att genom kommandon över bluetooth starta stoppa backa och styra höger eller 
 * vänster med variabel hastighet och variabelt styrutslag.
 * Fas 2: Integrera avståndssensorer(1-6) i kontrolloopen så bilen stannar eller väjer för hinder.
 * Fas 3: Integrera kompass och tachometer så att riktning och disans loggas 
 * Fas 5: Integrera en 2:a processor med större beräkningskapacitet och minne.
 * Fas 6: Utveckla algoritmer för att välja väg baserat på Robot SM:s regler för 
 * Folkrace Standard max vikt 1000 gram storlek bredd x längd x höjd 150 x 200 x 150
 * ej (1:87 1000 30 x 276 x 52* )
 * Roboten skall kunna ta sig runt banan på mindre än 3 minuter. 
 * Banans underlag är mestadels matt svart. Sargerna är vita och c:a 80% av klassens maxhöjd. 
 * Banans bredd varierar, men är håller generellt en bredd på 5*(klassens bredd). Dock alltid 
 * minst 3*(klassens bredd).Banan kan ha enklare hinder, t.ex. små kullar och pelare vid sargen. 
 * Pelarnas bredd är minst halva klassens bredd. Färger på banan kan även skilja sig på vissa 
 * ställen. Ju mindre klassen är desto snällare kommer banan att vara vad det gäller kullar och 
 * markfrigång. Kullarnas maximala lutning är 25° och höjd maximalt 1.5*(klassens höjd). Minsta 
 * rekommenderade markfrigång är 10% av klassens höjd.
 * Det finns två markeringar på banan (en startlinje och en mittlinje). Färgen på är 
 * ospecificerad och kan variera.
 * innerradie min 75 mm ytterradie 875 mm
 */
#include <OneWire.h>
// constants won't change. Used here to set a pin number :
const int backwardPin = 2;     // Bakåt Hög signal för bakåt
const int forwardPin =  3;     // Framåt Hög signal för framåt
const int rightPin =    4;     // Höger Hög signal för höger
const int leftPin =     5;     // Vänster Hög signal för vänster
const int selForwardRightPin =    6;     // Select VL53L0x fram höger
const int selForwardLeftPin =     7;     // Select VL53L0x fram vänster
const int selBackwardRightPin =   8;     // Select VL53L0x bak höger
const int selBackwardLeftPin =    9;     // Select VL53L0x bak vänster
const int selForwardMiddlePin =  10;     // Select VL53L0x bak höger
const int selBackwardMiddlePin = 11;     // Select VL53L0x bak vänster
const int tackoPin             = 12;     // Taco pulses 16 pulser/varv ca 8,8 mm/puls från vänster framhjul
const int startPin = 13;                 // Enable start when high
const int compassXPin =     0;     // Compass x analog value
const int compassYPin =     1;     // Compass y analog 
const int statusLedPin =    2;     // Status led 
const int LedSteady =       1;     // Status led lit
const int LedFlasingFast =  2;     // Status led blinkar snabbt
const int LedFlashingSlow = 3;     // Status led blinkar långsamt
const long interval1 = 10;           // interval at which to update motor(milliseconds)
const long interval2 = 1000;           // interval at which to upate temp reading (milliseconds)
const char forwards = 'f';             // kommands
const char backwards = 'b';
const char stopping = 's';
const char steerRight = 'r';
const char steerLeft = 'l';
const char steerStraight = 't';
const char increaseSpeed = 'i';
const char decreaseSpeed = 'd';
const char steerMoreRight = 'm';
const char steerMoreLeft = 'n';
const char steerLessRight = 'l';
const char steerLessLeft = 'k';

// Variables will change :
int carSpeed = 0;             // Backwards or Forward speed -127 to 128
int steering = 0;             // Steering angle -127 to 128
char commandMotor = 's';
char commandSteering = 't';
boolean startstate = FALSE;

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis1 = 0;        // will store last time LED was updated
unsigned long previousMillis2 = 0;        // will store last time LED was updated


void setup() {
  // put your setup code here, to run once:
  
  pinMode(tackoPin, INPUT_PULLUP);
  pinMode(startPin, INPUT);
  pinMode(compassXPin, INPUT);
  pinMode(compassYPin, INPUT);
  
  digitalWrite(forwardPin, LOW);
  digitalWrite(backwardPin, LOW);
  digitalWrite(rightPin, LOW);
  digitalWrite(leftPin, LOW);
  digitalWrite(selForwardRightPin, LOW);
  digitalWrite(selForwardLeftPin, LOW);
  digitalWrite(selBackwardRightPin, LOW);
  digitalWrite(selBackwardLeftPin, LOW);
  digitalWrite(selForwardMiddlePin, LOW);
  digitalWrite(selBackwardMiddlePin, LOW);
  digitalWrite(statusLedPin, LOW);           // Led släckt
  digitalWrite(tackoPin, HIGH);
  digitalWrite(startPin, LOW);
  
  pinMode(forwardPin, OUTPUT);
  pinMode(backwardPin, OUTPUT);
  pinMode(rightPin, OUTPUT);
  pinMode(leftPin, OUTPUT);
  pinMode(selForwardRightPin, OUTPUT);
  pinMode(selForwardLeftPin, OUTPUT);
  pinMode(selBackwardRightPin, OUTPUT);
  pinMode(selBackwardLeftPin, OUTPUT);
  pinMode(selForwardMiddlePin, OUTPUT);
  pinMode(selBackwardMiddlePin, OUTPUT);
  pinMode(statusLedPin, OUTPUT);
  
  if (digitalRead(startPin) == LOW){
    startstate = FALSE;
  } else {
    startstate = TRUE;
  }
  
  Serial.begin(9600);         // Connected to Bluetooth module HC-06 with a levelshifter 
                              // to convert 5v out from Arduino to 3,3 v in on HC-06
                              // AT+BAUD4 equals 9600 Baud(default for HC 06) Can be set to 115200 with AT+BAUD8
  if (Serial.available()) {
     Serial.read();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  // command interpreter
  if (Serial.available()) {     // If anything comes in Seria3(pins 0 & 1)
    char cmd1 = Serial.read();
        Serial.write(cmd1);
    switch (cmd1) { // read it and set command
      case forwards:     
        commandMotor = forwards;
        carSpeed = 60;
        break;
      case stopping:     
        commandMotor = stopping;
        carSpeed = 0;
        break;
      case backwards:
        commandMotor = backwards;
        carSpeed = -40;
        break;
      case steerRight:
        commandSteering = steerRight;
        steering = 60;
        break;
      case steerStraight:
        commandSteering = steerStraight;
        steering = 0;
        break;
      case steerLeft:
        commandSteering = steerLeft;
        steering = -60;
        break;
      case increaseSpeed:
        if (carSpeed < 119){
        carSpeed = carSpeed +10;
        }
        break;
      case decreaseSpeed:
        if (carSpeed > 9){
        carSpeed = carSpeed -10;
        }
        break;
      case steerMoreRight:
        if (steering < 119){
        steering = steering +10;
        }
        break;
      case steerMoreLeft:
        if (steering > -117){
        steering = steering -10;
        }
        break;
      default:
        ;
    }
            Serial.write(commandMotor);
            Serial.write(commandSteering);
  }
  // command interpreter
  if (digitalRead(startPin) == HIGH) { // do nothing if not started
    startstate = TRUE;
  switch (commandMotor) {
      case forwards:     
        motor(carSpeed);
        break;
      case stopping:     
        motor(carSpeed);
        break;
      case backwards:
        motor(carSpeed);
        break;
      default:
        ;
  }
   switch (commandSteering) {
     case steerRight:
        steer(steering);
        break;
      case steerStraight:
        steer(steering);
        break;
      case steerLeft:
        steer(steering);
        break;
      default:
        ;
  }
  }
}
void lampOn(int  pinNumber){
    digitalWrite(pinNumber, HIGH);
}
void lampOff(int pinNumber){
  digitalWrite(pinNumber, LOW);
}
void motor(int localSpeed){
  unsigned long currentMillis1 = millis();
  char test;
  if (currentMillis1 - previousMillis1 >= interval1) {
    // save the last time you checked
    previousMillis1 = currentMillis1;

    // if localSpeed is 0 do nothing if positive activate motor forward if negative motor backwards
    if (localSpeed < 0) {
      digitalWrite(forwardPin, LOW);
      delayMicroseconds(1000);
      digitalWrite(backwardPin, HIGH);
      Serial.print(millis());
      Serial.print("   ");
      Serial.println(previousMillis1);
    } else if ( localSpeed > 0 ) {
      digitalWrite(backwardPin, LOW);
      delayMicroseconds(1000);
      digitalWrite(forwardPin, HIGH);
    } else {
      digitalWrite(backwardPin, LOW);
      digitalWrite(forwardPin, LOW); 
      }
  }
}
void steer(int localSteer){
    if (localSteer < 0) {
      digitalWrite(rightPin, LOW);
      delayMicroseconds(1000);
      digitalWrite(leftPin, HIGH);
     
    } else if ( localSteer > 0 ) {
      digitalWrite(leftPin, LOW);
      delayMicroseconds(1000);
      digitalWrite(rightPin, HIGH);
    } else {
      digitalWrite(leftPin, LOW);
      digitalWrite(rightPin, LOW); 
      }
}

      
void readTemp(){
  unsigned long currentMillis2 = millis();
  
  if (currentMillis2 - previousMillis2 >= interval2) {
    // save the last time temperature was read
    previousMillis2 = currentMillis2;

  }
}
int readPot(int  pinNumber){
    int sensorValue = analogRead(pinNumber);
      int outputValue = map(sensorValue, 0, 1023, 0, 255);
      return outputValue;
}


