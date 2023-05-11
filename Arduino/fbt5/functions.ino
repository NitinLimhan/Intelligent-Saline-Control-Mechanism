void SubmitHttpRequest(int unit_id)
{




 
  String cmd = "AT+CIPSTART=\"TCP\",";                  // TCP connection
  cmd += "192.168.1.101";                               
  cmd += "\",80\r\n";
  Serial1.println(cmd);
  if (DEBUG) Serial.println(cmd);
  if(Serial1.find("Error")){
    if (DEBUG) Serial.println("AT+CIPSTART error");
    return false;
  }
 // http://hospital.ig/datalog
  delay(5000);
 // String getStr = "GET /ethernet/data.php?temperature=";   // prepare GET string
  String getStr = "GET  http://192.168.1.101/patientdata/1 HTTP/1.0\r\n";   // prepare GET string
 
 // getStr += String(unit_id);
    
   
  
  // getStr +="&field3=";
  // getStr += String(value3);
  // ...
 //etStr += "  HTTP/1.0\r\n\r\n";

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
String cadena="";
void fun()
{
   String getStr = "GET http://192.168.1.101/patientdata/1";   // prepare GET string
 
  String cmd = "AT+CIPSTART=\"TCP\",\"";                  // TCP connection
  cmd += "192.168.1.101";                               
  cmd += "\",80";
  
   Serial1.println(cmd);
  //connecting to server
  if(Serial1.find("OK")) {
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println("Server connection successful...");
    String peticionHTTP="GET / http://192.168.1.2/hospital/public/patientdata/1";   // prepare GET string
    //Sending the length of the HTTP request
    Serial1.print("AT+CIPSEND=");
    Serial1.println(peticionHTTP.length());
    //waiting for ">" for sending HTTP request
    if(Serial1.find(">")) {
      //we can send the HTTP request when > is displayed
      Serial.println("Sending HTTP request. . .");
      Serial1.println(peticionHTTP);
      if(Serial1.find("SEND OK")) {
        Serial.println("HTTP request sent...:");
        Serial.println();
        Serial.println("On stand by...");
        boolean fin_respuesta=false;
        long tiempo_inicio=millis();
        cadena="";
        
        while(fin_respuesta==false) {
          while(Serial1.available()>0) {
        char c=Serial1.read();
        Serial.write(c);
        cadena.concat(c); //store the request string on "cadena"
        if (cadena.length() > 50) cadena = "";
      }
          //terminate if "cadena" length is greater than  3500
          if(cadena.length()>5000) {
            Serial.println("The request exceeded the maximum length...");
            Serial1.println("AT+CIPCLOSE");
            if( Serial1.find("OK"))
              Serial.println("Connection terminated...");
            fin_respuesta=true;
          }
          if((millis()-tiempo_inicio)>10000) {
            //Terminate if connection time exceeded the maximum
            Serial.println("Connection time exceeded...");
            Serial1.println("AT+CIPCLOSE");
            if( Serial1.find("OK"))
              Serial.println("Connection terminated");
            fin_respuesta=true;
          }
          if(cadena.indexOf("CLOSED")>0) {
            Serial.println();
            Serial.println("String OK, connection terminated");
            fin_respuesta=true;
          }
        }
      } else {
        Serial.println("error on HTTP request.....");
      }
    }
  } else {
    Serial.println("Unable to find the server....");
  }
  delay(1000); //1 second delay before new loop
}


void ShowSerialData()
{
  while(Serial1.available()!=0)
    Serial.write(char (Serial1.read()));
    delay(5);
}
