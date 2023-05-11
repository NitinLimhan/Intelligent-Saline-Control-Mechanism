 
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
 #include <LiquidCrystal.h>
const int rs = 41, en = 39, d4 = 37, d5 = 35, d6 = 33, d7 = 31;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int screenWidth = 16;
int screenHeight = 2;
 

String ssid="IG";//"NRLimhan";    // Wifi network SSID
String password ="a1b2c3d4e5";//"qwertyuiop987";  // Wifi network password

boolean DEBUG=true;

//======================================================================== showResponce
void showResponse(int waitTime){
    long t=millis();
    char c;
    while (t+waitTime>millis()){
      if (Serial1.available()){
        c=Serial1.read();
        if (DEBUG) Serial.print(c);
      }
    }
                   
}

//========================================================================
boolean espdata(int unit_id,int flow_rate,float temperature,int pulserate,int salinelevel,int Econ){
  String cmd = "AT+CIPSTART=\"TCP\",\"";                  // TCP connection
  cmd += "192.168.1.28";                               
  cmd += "\",80";
  Serial1.println(cmd);
  if (DEBUG) Serial.println(cmd);
  if(Serial1.find("Error")){
    if (DEBUG) Serial.println("AT+CIPSTART error");
    return false;
  }
  http://hospital.ig/datalog
  
 // String getStr = "GET /ethernet/data.php?temperature=";   // prepare GET string
  String getStr = "GET http://192.168.1.28/datalog/";   // prepare GET string
 
  getStr += String(unit_id);
    
  getStr += String(',');
getStr += String(flow_rate);
    
  getStr += String(',');
  
getStr += String(temperature);
    
  getStr += String(',');


getStr += String(pulserate);
    
  getStr += String(',');
   
getStr += String(salinelevel);
    
  getStr += String(',');
  
getStr += String(Econ);
    
  
  // getStr +="&field3=";
  // getStr += String(value3);
  // ...
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  Serial1.println(cmd);
  if (DEBUG)  Serial.println(cmd);
  
  delay(100);
  if(Serial1.find(">")){
    Serial1.print(getStr);
    if (DEBUG)  Serial.print(getStr);
  }
  else{
    Serial1.println("AT+CIPCLOSE");
    // alert user
    if (DEBUG)   Serial.println("AT+CIPCLOSE");
    return false;
  }
  return true;
}
//================================================================================ setup
void setup() {                
  DEBUG=true;           // enable debug serial
  Serial.begin(9600); 
  
   myservo.attach(45);
  lcd.begin(16, 2);  
  Serial1.begin(115200);  // enable software serial
                          // Your esp8266 module's speed is probably at 115200. 
                          // For this reason the first time set the speed to 115200 or to your esp8266 configured speed 
                          // and upload. Then change to 9600 and upload again
  
  //Serial1.println("AT+RST");         // Enable this line to reset the module;
  //showResponse(1000);

  //Serial1.println("AT+UART_CUR=9600,8,1,0,0");    // Enable this line to set esp8266 serial speed to 9600 bps
  //showResponse(1000);
  
  

  Serial1.println("AT+CWMODE=1");   // set esp8266 as client
  showResponse(1000);

  Serial1.println("AT+CWJAP=\""+ssid+"\",\""+password+"\"");  // set your home router SSID and password
  showResponse(5000);

   if (DEBUG)  Serial.println("Setup completed");
}
float tempC;
int unit_id=1;
int flow_rate=0;
int pulserate=0;
int salinelevel=0;
int Econ=0;
int pushButton = 8;

// ====================================================================== loop  {unit_id},{flow_rate},{temperature},{pulserate},{salinelevel},{Econ}
void loop() {
 int buttonState = digitalRead(pushButton);
 if(buttonState==1)
 Econ=0;
 else 
 Econ=1;
  // sets the servo position according to the scaled value
flow_rate = analogRead(1);
  flow_rate = map(flow_rate, 110, 1010, 100, 0); 
int  val = analogRead(1);            // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 0, 160);     // scale it to use it with the servo (value between 0 and 180)
  myservo.write(val);                
  tempC = (5.0 * analogRead(A0) * 100.0) / 1024;
    Serial.println(tempC);
           espdata(unit_id,flow_rate,tempC,pulserate,salinelevel,Econ);                                      // Write values to thingspeak
     
  
    
  // thingspeak needs 15 sec delay between updates,     
  delay(1000);  

   lcd.setCursor(0,0);           //sets the cursor at row 0 column 0
  lcd.print("Temp:"); // prints 16x2 LCD MODULE
  lcd.print(tempC);
lcd.print("C  ");
  lcd.setCursor(0,1);           //sets the cursor at row 1 column 2
  lcd.print("FR:");     // prints HELLO WORLD
  lcd.print(flow_rate); 
    lcd.print("% ");  
}




