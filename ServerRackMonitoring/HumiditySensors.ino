/*
for honeywell part #HIH-5031-001 @ 5 volts
*/
double GetNonTempuratureAdjustedHumidity(int port, int samples){
  int VoutSamples = 0;
   for(int i = 0; i < samples; i++)
  {
    VoutSamples = VoutSamples + map( analogRead(port), 0, 1023, 0, 5000); 
    delay(2);     
  }
  double averageVout = VoutSamples / (double)samples;
  double RH = (( averageVout / 5000.0 ) - 0.1515) / 0.00636;
  return RH;
}

/*
for honeywell part #HIH-5031-001 @ 5 volts
*/
double GetTrueRelativeHumidity(int port, float tempC, int samples){
  double RH = GetNonTempuratureAdjustedHumidity(port, samples);
  double TrueRH = RH / (1.0546 - 0.00216 * tempC);
  return TrueRH;
}
