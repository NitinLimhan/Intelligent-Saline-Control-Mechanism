void WebRequest(String request){
 //find the dividing marker between domain and path
     int slash = request.indexOf('/');
     
     //grab the domain
     String domain;
     if(slash>0){  
       domain = request.substring(0,slash);
     }else{
       domain = request;
     }

     //get the path
     String path;
     if(slash>0){  
       path = request.substring(slash);   
     }else{
       path = "/";          
     }
     
     //output domain and path to verify
     Serial.println("domain: |" + domain + "|");
     Serial.println("path: |" + path + "|");     
     
     //create start command
     String startcommand = "AT+CIPSTART=\"TCP\",\"" + domain + "\", 80"; //443 is HTTPS, still to do
     
     Serial1.println(startcommand);
     Serial.println(startcommand);
     
     
     //test for a start error
     if(Serial1.find("Error")){
       Serial.println("error on start");
       return;
     }
     
     //create the request command
     String sendcommand = "GET http://"+ domain + path + " HTTP/1.0\r\n\r\n\r\n";//works for most cases
     
     Serial.print(sendcommand);
     
     //send 
     Serial1.print("AT+CIPSEND=");
     Serial1.println(sendcommand.length());
     
     //debug the command
     Serial.print("AT+CIPSEND=");
     Serial.println(sendcommand.length());
     
     //delay(5000);
     if(Serial1.find(">"))
     {
       Serial.println(">");
     }else
     {
       Serial1.println("AT+CIPCLOSE");
       Serial.println("connect timeout");
       delay(1000);
       return;
     }
     
     //Serial.print(getcommand);
     Serial1.print(sendcommand); 
}
