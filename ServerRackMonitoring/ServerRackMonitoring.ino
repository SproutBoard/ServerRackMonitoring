#include <EEPROM.h>

#include <SPI.h>
#include <Ethernet.h>
#include <Time.h>

#include <Bounce.h>
#include "ReedSwitches.h"

/*
Sproutboard Server Room Monitor
 SproutBoard.com
 
 
 Analog I/O:
 Analog 0 - CCD Light Sensor (CDS1)
 Analog 1 - Accessory Terminal Block 1 (TA1) unused
 Analog 2 - Accessory Terminal Block 2 (TA2) unused
 Analog 3 - Onboard Accessories Socket 1 (Temperature sensor on external board J9)
 Analog 4 - Onboard Accessories Socket 1 (Humidity sensor on external board J9)
 Analog 5 - Accessory Terminal Block 3 (TA5)
 
 Digital I/O:
 Digital 0 - Not used
 Digital 1 - Serial Terminal (TS1)
 Digital 2 - Onboard Switch (SW1)
 Digital 3 - Onboard Peizo Speaker (SP1)
 Digital 4 - LED 1 (LED 1)
 Digital 5 - LED 2 (LED 2)
 Digital 6 - Accessory Terminal Block 1 (DA1) unused
 Digital 7 - Accessory Terminal Block 2 (DA2) unused
 Digital 8 - Accessory Terminal Block 3 (DA3) unused
 Digital 9 - Accessory Terminal Block 4 (DA4) unused
 Digital 10 - Reserved For Additional Shield (Ethernet shield pins correspond)
 Digital 11 - Reserved For Additional Shield (Ethernet shield pins correspond)
 Digital 12 - Reserved For Additional Shield (Ethernet shield pins correspond)
 Digital 13 - Reserved For Additional Shield (Ethernet shield pins correspond)
 */



//Analog IO
const int TemperatureSensorPin = 3;
const int HumiditySensorPin = 4;
const int AnalogTerminalBlock1 = 1;
const int AnalogTerminalBlock2 = 2;
const int AnalogTerminalBlock3 = 5;

//Digital IO
const int LED1 = 4;
const int LED2 = 5;
const int DigitalTerminalBlock1 = 6;
const int DigitalTerminalBlock2 = 7;
const int DigitalTerminalBlock3 = 8;
const int DigitalTerminalBlock4 = 9;

ReedSwitches reedSwitches = ReedSwitches();

byte mac[] = {  
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };


IPAddress ip(192,168,1,177);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

float _tempc;
float _tempf;
double _trueRH;
unsigned long time;
unsigned long timePrevious;

void setup(){
  Serial.begin(9600);  
  pinMode(LED1, OUTPUT); 
  pinMode(LED2, OUTPUT); 
  time = millis();
  timePrevious = time;
  
  reedSwitches.AddPin( DigitalTerminalBlock1);
  if (Ethernet.begin(mac) == 0) {

    Serial.print("Failed to configure Ethernet using DHCP, using static IP ");
    Serial.println(ip);
    Ethernet.begin(mac, ip);
  }
  Serial.print("My IP address: ");
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  Serial.println();
  
}

void loop(){
  _tempc = GetTempurature_LM60(TemperatureSensorPin, 10);
  _tempf = ConvertCelciusToFahrenheit(tempc);
  _trueRH = GetTrueRelativeHumidity(HumiditySensorPin, tempc, 10);
  reedSwitches.Poll();
  
  time = millis();
  
  if(time - timePrevious >= 500) //To slow down Serial output so it's readable
  {
    PrintTemperatureSerial(tempc, tempf);
    PrintHumiditySerial(trueRH);
    reedSwitches.PrintSerial();
    timePrevious = time;
  }
  
  WebServerResponse();

}

void WebServerResponse(){  
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each sensor

          PrintTemperatureWeb(client);
          PrintHumidityWeb(client);
          reedSwitches.PrintWeb(client);
          
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

void PrintTemperatureWeb(EthernetClient client){
  client.print("Temperature is ");
  client.print(_tempc);
  client.print("&deg; C");
  client.println("<br />"); 

  client.print("Temperature is ");
  client.print(_tempf);
  client.print("&deg; F ");
  client.println("<br />");
}

void PrintHumidityWeb(EthernetClient client){
  client.print("Humidity is ");
  client.print(_trueRH);
  client.print("%");
  client.println("<br />"); 
}

void PrintTemperatureSerial(float tempc, float tempf){
  Serial.print("Temperature: " );
  Serial.print( tempc);
  Serial.print( "C ");

  Serial.print(tempf);
  Serial.println( "F");
}

void PrintHumiditySerial(double trueRH){
  Serial.print("Humidity: ");
  Serial.print(trueRH);
  Serial.println("%" );

}




