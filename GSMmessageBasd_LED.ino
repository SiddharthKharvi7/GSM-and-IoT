/* This code is to Controll a LED using Messaging
 *  
 *  Insert a Sim on GSM800l Module power a 4V 2A Powersupply 
 *  and begin with following connections
 *  
 *  Module Connections
 *  
 *  GSM 800l--------Arduino
 *   TX >>-------->> RX 10
 *   RX >>-------->> TX 11
 *   Gnd ----------- Gnd
 *  
 *  LED --- D13 pin of Arduino 
 * 
 * On a Phone text ON / OFF to the Sim inserted to Module
 * The LED will React to the commeands
 * 
 * Author :Siddharth Kharvi
 * Created Date :11/07/2019
 * Updated Date :28/07/2019
 * 
 */
#include <SoftwareSerial.h> //include libraries
SoftwareSerial mySerial(10,11); // 10RX 11TX

char incomingByte; 
String incomingData;
bool atCommand = true;
int led = 13;              //LED will be Conected to pin no 13

int index = 0;
String number = "";
String message = "";


void setup() 
{
    Serial.begin(9600);
    mySerial.begin(9600);
    pinMode(led, OUTPUT); 
    
    
    while(!mySerial.available()){
      mySerial.println("AT");
      delay(1000); 
      Serial.println("connecting....");
    }
    
    Serial.println("Connected..");  
    mySerial.println("AT+CMGF=1");  //Set SMS Text Mode 
    delay(1000);  
    mySerial.println("AT+CNMI=1,2,0,0,0");  //procedure, how to receive messages from the network
    delay(1000);
    mySerial.println("AT+CMGL=\"REC UNREAD\""); // Read unread messages
    Serial.println("Ready to received Commands..");  
 }

void loop()
{ 
  if(mySerial.available()){
      delay(100);
      
      // Serial buffer
      while(mySerial.available()){
        incomingByte = mySerial.read();    ///Get Incomming Data
        incomingData += incomingByte; 
       }
        
        delay(10); 
        if(atCommand == false){
          receivedMessage(incomingData);
        }else{
          atCommand = false;
         
        }
        
        //ete messages to save memory
        if (incomingData.indexOf("OK") == -1){
          mySerial.println("AT+CMGDA=\"DEL ALL\"");
          delay(1000);
          atCommand = true;
        }
        
        incomingData = "";
  }
}

void receivedMessage(String inputString){
  
  //Get The number of the sender
  index = inputString.indexOf('"')+1;
  inputString = inputString.substring(index);
  index = inputString.indexOf('"');
  number = inputString.substring(0,index);
  Serial.println("Number: " + number);

  //Get The Message of the sender
  index = inputString.indexOf("\n")+1;
  message = inputString.substring(index);
  message.trim();
  Serial.println("Message: " + message);
        
  message.toUpperCase(); // uppercase the message received

  //turn LED ON or OFF
  if (message.indexOf("ON") > -1){
      digitalWrite(led, HIGH);
      Serial.println("Command: LED Turn On.");
   }
          
  if (message.indexOf("OFF") > -1){
        digitalWrite(led, LOW);
        Serial.println("Command: LED Turn Off.");
   }
          
   delay(50);
  }
