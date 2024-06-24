float vrms;
float irms;
float power;
float counter=0;
float total_time=5.003;
float energy;
float last_energy=0;
void setup() 
{
 Serial.begin(9600);
}

void loop() 
{
 vrms = getVRMS();
 irms = getIRMS();
 power= vrms*irms;
 last_energy=last_energy+1.73;
 if(power>1)
 {
 Serial.println("Device is ON");

 Serial.print("Vrms value is : ");
 Serial.println("Volts", vrms);
 delay(1000);
 
 Serial.print("Irms value is : ");
 Serial.println("Amps", irms);
 delay(1000);

 Serial.print("Power value is : ");
 Serial.println("Watt",power);
 delay(1000);
 
 energy=power*total_time;
 last_energy=last_energy+energy;
 
 Serial.print("Total energy is : ");
 Serial.println("Ws",last_energy);
 }

 else
 {
  Serial.println("Device is OFF");
  Serial.print("Energy consumed was : ");
  Serial.println("Ws",last_energy);
 }
}

float getIRMS()
{
  float peaktopeaki;
  float readValuei;                // value read from the sensor
  float maxValuei = 0;             // store max value here
  float minValuei = 1024;          // store min value here
  float rmsvalue1i;
  float rmsvaluei ;
   uint32_t start_timei = millis();
   while((millis()-start_timei) < 1000) //sample for 1 Sec
   {
       readValuei = analogRead(A1);
       // see if you have a new maxValue
       if (readValuei > maxValuei) 
       {
           /*record the maximum sensor value*/
           maxValuei = readValuei;
       }
       if (readValuei< minValuei) 
       {
           /*record the minimum sensor value*/
           minValuei = readValuei;
       }
   }
   
   // Subtract min from max
   peaktopeaki = (maxValuei - minValuei);
   rmsvalue1i = peaktopeaki/ sqrt(2);  
   rmsvaluei = abs((rmsvalue1i-2.12)/21.37);
   return rmsvaluei;
 }

 float getVRMS()
{
  float rmsvaluev;
  float peaktopeakv;
  int readValuev;                // value read from the sensor
  int maxValuev = 0;             // store max value here
  int minValuev = 1024;          // store min value here
   uint32_t start_timev = millis();
   while((millis()-start_timev) < 1000) //sample for 1 Sec
   {
       readValuev = analogRead(A0);
       // see if you have a new maxValue
       if (readValuev > maxValuev) 
       {
           /*record the maximum sensor value*/
           maxValuev = readValuev;
       }
       if (readValuev < minValuev) 
       {
           /*record the minimum sensor value*/
           minValuev = readValuev;
       }
   }
   
   // Subtract min from max
   peaktopeakv = (maxValuev - minValuev);
   rmsvaluev = peaktopeakv/sqrt(2);   
   return rmsvaluev;
 }
