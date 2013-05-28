float GetTempurature_LM60(int port, int samples){
  float tempc;
  int tempReadings = 0;
  for(int i = 0; i < samples; i++)
  {
    tempReadings = tempReadings + map( analogRead(port), 0, 1023, 0, 5000); 
    delay(2);     
  }
  float averageReading = tempReadings / (float)samples;
  tempc = ( averageReading - 424.0)/6.25;
  return tempc;
}

float ConvertCelciusToFahrenheit(float tempCelcius){
  return (tempCelcius * 9.0)/ 5.0 + 32.0;
}
