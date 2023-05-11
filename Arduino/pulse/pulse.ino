//Number of pulses, used to measure energy.
#include <Servo.h>
#include <LiquidCrystal.h>
const int rs = 41, en = 39, d4 = 37, d5 = 35, d6 = 33, d7 = 31;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
long pulseCount = 0;
Servo myservo;  // create servo object to control a servo

unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change :
const long interval = 10000;           // interval at which to blink (milliseconds)

const byte interruptPin = 2;
void setup()
{lcd.begin(16, 2);  
    Serial.begin(9600);

    // KWH interrupt attached to IRQ 1 = pin3
    attachInterrupt(digitalPinToInterrupt(interruptPin), onPulse,CHANGE);
      myservo.write(140);        
}

void loop()
{
    myservo.attach(45);
int flow_rate = analogRead(1);

int  val = analogRead(1);            // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 160, 60);     // scale it to use it with the servo (value between 0 and 180)
  myservo.write(val);    
          //sets the cursor at row 1 column 2
  flow_rate = map(flow_rate, 110, 1010, 100, 0); 
  lcd.setCursor(0,0);  
   lcd.print("FR:");     // prints HELLO WORLD
  lcd.print(flow_rate); 
    lcd.print("% ");  
lcd.setCursor(8,0);  
   lcd.print("DPM:");     // prints HELLO WORLD
  lcd.print(val); 
    lcd.print("  ");  

     unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
       previousMillis = currentMillis;
 //      Serial.print("count:");
   //    Serial.println(pulseCount);
 int DPM =pulseCount*6;
 pulseCount=0;
  lcd.setCursor(0,1);  
   lcd.print("DPM:");     // prints HELLO WORLD
  lcd.print(DPM); 
    lcd.print("   ");
      Serial.println(DPM);

  }

 
}

// The interrupt routine
void onPulse()
{
    
    pulseCount++;
 Serial.print("count:");
       Serial.println(pulseCount);
 
}
