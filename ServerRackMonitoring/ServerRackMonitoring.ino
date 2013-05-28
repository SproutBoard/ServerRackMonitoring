/*
Sproutboard Server Room Monitor
 SproutBoard.com
 
 
 Analog I/O:
 Analog 0 - CCD Light Sensor (CDS1)
 Analog 1 - Accessory Terminal Block 1 (TA1) unused
 Analog 2 - Accessory Terminal Block 2 (TA2) unused
 Analog 3 - Onboard Accessories Socket 1 (Temperature sensor on external board J9)
 Analog 4 - Onboard Accessories Socket 1 (Humidity sensor on external board J9)
 Analog 5 - Onboard Accessories Socket 2 (Sound J10)
 
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
void setup(){
  Serial.begin(9600);  
}

void loop(){
  float tempc = GetTempurature_LM60(3, 10);
  float tempf = ConvertCelciusToFahrenheit(tempc);
  Serial.print("Temperature: " );
  Serial.print( tempc);
  Serial.print( "C ");

  Serial.print(tempf);
  Serial.println( "F");
  Serial.print("Humidity: ");
  Serial.print(GetTrueRelativeHumidity(4, tempc, 10) );
  Serial.println("%" );
  delay(1000);
}



