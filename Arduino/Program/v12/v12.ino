 
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
 #include <LiquidCrystal.h>
const int rs = 41, en = 39, d4 = 37, d5 = 35, d6 = 33, d7 = 31;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
long dropCount = 0;
int screenWidth = 16;
int screenHeight = 2;
 int sysflowrate =0;
unsigned long previousMillis = 0;        // will store last time LED was updated
int DPM=0;
// constants won't change :
const long interval = 10000;           // interval at which to blink (milliseconds)

const byte interruptPin = 2;

//String ssid="IG";    // Wifi network SSID
//String password ="a1b2c3d4e5";  // Wifi network password
//String ssid="ASK";    // Wifi network SSID
//String password ="12345678";  // Wifi network password

//String ssid="NRLimhan";    // Wifi network SSID
//String password ="qwertyuiop987";  // Wifi network password
//String ssid="Rajyog_Network";    // Wifi network SSID
//String password ="Rajyog@15022013";  // Wifi network password
//String ssid="Nitin";    // Wifi network SSID
//String password ="seven497";  // Wifi network password
String ssid="RN";    // Wifi network SSID
String password ="qwertyuiop";  // Wifi network password


boolean DEBUG=true;
int saline_pin =21;
float tempC;
int unit_id=1;
int flow_rate=0;
int pulserate=0;
int salinelevel=0;
int Econ=0;
int pushButton = 8;
int  val=0;

int AMButton = 7;
//================================================================================ setup
void setup() {                
  DEBUG=true;           // enable debug serial
  Serial.begin(9600); 
  pinMode(AMButton, INPUT);
    attachInterrupt(digitalPinToInterrupt(interruptPin), onDrop,CHANGE);
   myservo.attach(45);
  lcd.begin(16, 2);  
  Serial1.begin(115200);   
  
pinMode(saline_pin, INPUT);

 //Serial1.println("AT+RST");         // Enable this line to reset the module;
 //showResponse(1000);

//Serial1.println("AT+UART_CUR=19200,8,1,0,0");    // Enable this line to set esp8266 serial speed to 9600 bps
 //showResponse(1000);
  
  
  Serial1.println("AT+CWMODE=1");   // set esp8266 as client
  showResponse(1000);

  Serial1.println("AT+CWJAP=\""+ssid+"\",\""+password+"\"");  // set your home router SSID and password
  showResponse(5000);

   if (DEBUG)  Serial.println("Setup completed");
}

//unsigned long previousMillis = 0;  
// ====================================================================== loop  {unit_id},{flow_rate},{temperature},{pulserate},{salinelevel},{Econ}
void loop() {
 previousMillis = millis();
 salinelevel=digitalRead(saline_pin);
   int AMState = digitalRead(AMButton);
 tempC = (5.0 * analogRead(A0) * 100.0) / 1024;
  Serial.println(tempC);
   int buttonState = digitalRead(pushButton);
   
   if(buttonState==1)
   Econ=0;
   else 
   Econ=1;
  // sets the servo position according to the scaled value
  
lcd.setCursor(0,0);           //sets the cursor at row 0 column 0
  lcd.print("Temp:"); // prints 16x2 LCD MODULE
  lcd.print(tempC);
  lcd.print("C  ");


  if(salinelevel==1)
  {
   if(AMState ==0)
   {
    lcd.setCursor(0,1);           //sets the cursor at row 1 column 2
  lcd.print("DPM:");     // prints HELLO WORLD
  lcd.print(sysflowrate); 
    lcd.print("    ");  
    lcd.setCursor(8,1);            
    
     val = map(sysflowrate, 0, 250, 165, 60);  
  //  
     lcd.setCursor(8,1);            
  lcd.print("AUT MODE    ");  
   }
   else
   {
     flow_rate = analogRead(1);
   flow_rate = map(flow_rate, 110, 1010, 0, 100); 

   //flow_rate = map(flow_rate, 110, 1010, 250, 0);  //drop per min
    val = analogRead(1);            // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 160, 60);     // scale it to use it with the servo (value between 0 and 180)
                 
  


   
  lcd.setCursor(0,1);           //sets the cursor at row 1 column 2
  lcd.print("FR:");     // prints HELLO WORLD
  lcd.print(flow_rate); 
    lcd.print("%    ");  
    lcd.setCursor(8,1);            
  lcd.print("MAN MODE    ");  
    
   }
  }
  else
  {
  val=160;
   lcd.setCursor(0,1);           //sets the cursor at row 1 column 2
  lcd.print("Saline Critical  ");     // prints HELLO WORLD
  }
 myservo.write(val); 
  espdata(unit_id,DPM,tempC,pulserate,salinelevel,Econ);   //calulated drops are sent
  //espdata(unit_id,flow_rate,tempC,pulserate,salinelevel,Econ);     //calculated flow as per pot 
  delay(1000);  
  
 
    espdata_read(unit_id);
  
    
  
  delay(1000);  

  
     unsigned long currentMillis = millis();

int time1=(currentMillis - previousMillis)/1000;
  //   Serial.print("Time********:");
 //Serial.println(time1);
 DPM =(dropCount*60)/time1;
 dropCount=0;
  //Serial.print("DPM********:");
 //Serial.println(DPM);
 
}


void onDrop()
{
    
    dropCount++;
   // Serial.print("count:");
   // Serial.println(dropCount);
 
}




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
  cmd += "192.168.1.101";                               
  cmd += "\",80";
  Serial1.println(cmd);
  if (DEBUG) Serial.println(cmd);
  if(Serial1.find("Error")){
    if (DEBUG) Serial.println("AT+CIPSTART error");
    return false;
  }
   
  String getStr = "GET http://192.168.1.101/hospital/public/datalog/";   // prepare GET string
 
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

boolean espdata_read(int unit_id ){
  String cmd = "AT+CIPSTART=\"TCP\",\"";                  // TCP connection
  cmd += "192.168.1.101";                               
  cmd += "\",80";
  Serial1.println(cmd);
  if (DEBUG) Serial.println(cmd);
  if(Serial1.find("Error")){
    if (DEBUG) Serial.println("AT+CIPSTART error");
    return false;
  }  
  
 
  String getStr = "GET http://192.168.1.101/hospital/public/patientdata/";   // prepare GET string
 
  getStr += String(unit_id);
   
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  Serial1.println(cmd);
  if (DEBUG)  Serial.println(cmd);
  
  delay(100);
  if(Serial1.find(">")){
    Serial1.print(getStr);

//delay(1000);
if(Serial1.available()) // Check if there is data available
  {
    String input ="";
    while(Serial1.available()) // read the data into a variable as long as the buffer is not empty
    {
      input += (char)Serial1.read();
      delay(30);
    }
      Serial.print("data:");
      
String datai(input);
  Serial.println(datai);
   
//   input = input.replace(" ",",");
 datai.trim();
datai.remove(0,35); 
int len1=datai.length()-6;
datai.remove(len1); 
    Serial.print("data:");
   Serial.println(datai);
   
     //    Serial.println("         enddata:");
 
 Serial.print("string len:");
 
  Serial.println(datai.length());
  sysflowrate=datai.toInt(); 
  
  }
 
   
    // alert user
    if (DEBUG)   Serial.println("AT+CIPCLOSE");
    return false;
  }
  return true;
}
