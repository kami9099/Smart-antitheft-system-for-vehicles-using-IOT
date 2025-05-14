#include <SoftwareSerial.h>
#include <TinyGPS++.h>

SoftwareSerial GSM_Module(8,9);
SoftwareSerial GPS(3,4);  //// The serial connection to the GPS device
TinyGPSPlus gps;        // The TinyGPS++ object


#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
int push=13;
double longitude;
double latitude;
String msg;

SoftwareSerial *mySerial;

void setup()
{
  GSM_Module.begin(115200);   // Setting the baud rate of GSM Module  
  GPS.begin(9600);
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  Serial.println ("SIM900A Ready");
  delay(100);
  //while (GPS.available() == 0)
  //{}
     
  pinMode(push , INPUT);
  digitalWrite(push, INPUT_PULLUP);
  gps_data();
  lcd.begin();
  lcd.backlight();
  lcd.print("Welcome :)");
  delay(1000);
  Scrolling_Display("System Is Ready :) ");
  SwitchSerial(0);
  lcd.clear();
  Scrolling_Display("Sending Start Message :) ");
  SendMessage1("Your Car Has Been Started (_)");
  lcd.setCursor(0,1);
  lcd.print("Message Sent :) ");
  mySerial=&GPS;
  delay(1000);
}


void loop()
{       
    if (mySerial->available() > 0)
{
    gps.encode(mySerial->read());
    if (gps.location.isUpdated())
  {   
      lcd.clear();
      Serial.print("Latitude= "); 
      latitude=gps.location.lat();
      Serial.print(latitude);
      lcd.setCursor(0,0);
      lcd.print("Latitude=");
      lcd.setCursor(10,0);
      lcd.print(latitude);
      Serial.print("    Longitude= "); 
      longitude=gps.location.lng();
      Serial.println(longitude);
      lcd.setCursor(0,1);
      lcd.print("Longitude=");
      lcd.setCursor(10,1);
      lcd.print(longitude);
      delay(100);
      Serial.println ("Type s to send message or r to receive message");
   }
}
if (Serial.available()>0)
     SwitchSerial(0);
   switch(Serial.read())
  {
    case 's':{
      lcd.clear();
      lcd.setCursor(0,0);
      Scrolling_Display("Sending Message.... :) ");
      SendMessage();
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("Message Sent :)");
      delay(1000);
      break;
      }
  }
  
  if ( digitalRead(push) == 1)
        SendMessage();
  
}
 void SendMessage()
{
  mySerial=&GSM_Module;
  lcd.clear();
  Scrolling_Display ("Sending Message... :) ");
  mySerial->println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  Serial.println ("Set SMS Number");
  mySerial->println("AT+CMGS=\"+923123674156\"\r"); //Mobile phone number to send message
  delay(1000);
  mySerial->println ("YOUR Car's Location is:");
  mySerial->print ("Longitude:"); 
  mySerial->print (longitude);
  mySerial->print ("    Latitude:");
  mySerial->println (latitude);// Messsage content
  delay(100);
  mySerial->println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Scrolling_Display ("Message has been sent :) ");
  mySerial=&GPS;
}

void SendMessage1(String message)
{
 Serial.println ("Sending Message");
  mySerial->println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  Serial.println ("Set SMS Number");
  mySerial->println("AT+CMGS=\"+923123674156\"\r"); //Mobile phone number to send message
  delay(1000);
  Serial.println ("Set SMS Content");
  mySerial->println(message);// Messsage content
  delay(100);
  Serial.println ("Finish");
  mySerial->println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.println("Message Sent");
  lcd.print("Car Started :)");
}
    
  void SwitchSerial(int hardware) {
  if (hardware == 1) {
    mySerial = &GPS;
  } else {
    mySerial = &GSM_Module;
  }
}
void Scrolling_Display(String message)
{
   int  length = message.length();
  for(int i = 0; i < length; i++)
  {  
    delay(10);
    ClearRow(0);
    lcd.print(message.substring(i,i+16));
    delay(100);
  }
}

void ClearRow(int rowNum)
{
  lcd.setCursor(0,rowNum);
  lcd.print("                 ");
  lcd.setCursor(0,rowNum);
}

 void ReadMessage()
{ mySerial=&GSM_Module;
  mySerial->println("AT+CMGF=1");
  delay(100);  
  Serial.println ("SIM900A Incoming SMS");
  delay (100);
  mySerial->println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
  delay(1000);
  if (mySerial->available()>0)
  SendMessage();
  msg = mySerial->readString();
  mySerial->println(msg);
  delay(2000);
  Serial.println ("Unread Message done");
  delay(2000);

 }  

 void gps_data()
{
   if (mySerial->available() > 0)
 {
    gps.encode(mySerial->read());
    if (gps.location.isUpdated())
  {   latitude=gps.location.lat();
      longitude=gps.location.lng();
      }
  }
}