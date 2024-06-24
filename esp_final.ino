#include <HardwareSerial.h>
#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

//Helper Token
#include "addons/TokenHelper.h"
//RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Wifi Creds
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

// Firebase API Key
#define API_KEY "AIzaSyDpiTWATXUDcDsBSOO7E8tV5hmMBJzJ8kI"

// Firebase Database URL
#define DATABASE_URL "https://esp32-c27ef-default-rtdb.firebaseio.com/" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

int analogPin = 33;  // Choose an analog pin on the ESP
int analogPin1 = 32;
const float VOLTAGE_SCALE = 5.0 / 1023.0;
unsigned long sendDataPrevMillis = 0;

int readAnalogData() 
{
  int analogData = analogRead(33);//Read data from analog pin A0,A1
  return analogData;
}
int readAnalogData1() 
{
  int analogData1 = analogRead(32);//Read data from analog pin A0,A1
  return analogData1;
}
bool signupOK = false;
#define ADC1 CH5 
#define ADC1 CH4
#define analogWrite
float power;
float energy;
const int outputPin = 33;
const int outputPin1 = 32;

void setup() {
  pinMode(analogPin, INPUT);
  pinMode(analogPin1,INPUT);
  Serial.begin(9600, SERIAL_8N1, 33, 32);
  pinMode(4, OUTPUT);  // Set the output pin to generate PWM signal
  pinMode(2, OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

//  API Key setup
  config.api_key = API_KEY;

//  Database URL setup
  config.database_url = DATABASE_URL;

//SignUP
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

//callback function for the long running token generation task
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() 
{ 
  int analogValue = analogRead(analogPin);
  int analogValue1 = analogRead(analogPin1);
  Serial.println(analogValue);
  Serial.println(analogValue1);
  delay(1000);  
  Serial.println("Message received");
  analogWrite(33, 127);
  analogWrite(32, 127);
  delay(1000);
  analogWrite(33, 255);
  analogWrite(32, 255);
  delay(1000);
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 10000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    power = readAnalogData();
    energy = readAnalogData1();
    int pwmValue = ledcRead(4);
    int pwmValue1 = ledcRead(2);
    float power = analogValue * VOLTAGE_SCALE; // Convert sensor value to voltage
    float energy = analogValue1 * VOLTAGE_SCALE;
    analogWrite(outputPin, pwmValue >= 128);
    analogWrite(outputPin1,pwmValue1>=128);

    if(Firebase.RTDB.setFloat(&fbdo, "Sensor/Power", power)){
      Serial.print("Success ");
      Serial.println(fbdo.dataPath());
    }
    else {
      Serial.print("FAILED ");
      Serial.println(fbdo.errorReason());
    }
    if(Firebase.RTDB.setFloat(&fbdo, "Sensor/Energy", energy)){
      Serial.print("Success ");
      Serial.println(fbdo.dataPath());
    }
    else {
      Serial.print("FAILED ");
      Serial.println(fbdo.errorReason());
  }
}
}
