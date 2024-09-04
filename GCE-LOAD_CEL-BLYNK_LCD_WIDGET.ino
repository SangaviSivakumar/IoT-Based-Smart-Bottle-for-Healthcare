//iotecemit@gmail.com
#include <ESP8266WiFi.h>
#include "HX711.h"
#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
///#include <LiquidCrystal_I2C.h>
//LiquidCrystal_I2C lcd(0x27, 16, 2);
#define BLYNK_PRINT Serial
 
const char *ssid = "iotlab"; // replace with your wifi ssid and wpa2 key
const char *pass = "password12345";
char auth[] = " cATOZXcv6jMw9IZIptYlD9kQdpSdur6h";    // You should get Auth Token in the Blynk App.
 WidgetLCD lcd(V9);
WiFiClient client;
 
HX711 scale(D5, D6);
 
int rbutton = D4; // this button will be used to reset the scale to 0.
float weight;
float calibration_factor = -515598.91; // for me this vlaue works just perfect 419640 
 int smokeA0 = A0;

int sensorThres = 350;
void setup() 
{
  Serial.begin(115200);
  pinMode(rbutton, INPUT_PULLUP); 
  pinMode(D8, OUTPUT); 
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
  long zero_factor = scale.read_average(); //Get a baseline reading
  Blynk.begin(auth, ssid, pass);
  Wire.begin(D2, D1);
//  lcd.begin();
  //lcd.setCursor(6,0);
//  lcd.print("IOT");
//  lcd.setCursor(1,1);
  lcd.print(0,0, "GAS CYLINDER-IoT ");
  lcd.print(0,1, "  MONITORING      ");
  delay(3000);
  lcd.clear();
 
  lcd.print(0,0,"Connecting Wifi");
  
  WiFi.begin(ssid, pass);
  {
  delay(1000);
  Serial.print(".");
  lcd.clear();
  }
  Serial.println("");
  Serial.println("WiFi connected");
  lcd.clear();
  //lcd.print("WiFi connected");
  lcd.print(0, 0, "WiFi connected");
  delay(2000);
  lcd.clear();
}
 
void loop() 
 
{
 
  Blynk.run();
  smoke();
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
 
  weight = scale.get_units(5); 
 
 // lcd.setCursor(0, 0);
  //lcd.print("Measured Weight");
  //lcd.setCursor(0, 1);
 // lcd.print(weight);
 // lcd.print(" KG  ");
 lcd.print(0, 0, "Measured Weight");
 lcd.print(3, 1, weight);
 lcd.print(10, 1, "KG");
  Blynk.virtualWrite(V3, weight);
  delay(2000);
  lcd.clear();
  if(weight<=0.10)
   {
     Blynk.notify("T==> Gas cylinder Low- G1100302-kk nagar");  
     lcd.print(0, 0, "Gas cylinder Low G11032-KK Nagar");
     delay(2000);
  lcd.clear();
   }
    

  
  
  
  Serial.print("Weight: ");
  Serial.print(weight);
  Serial.println(" KG");
  Serial.println();
  
 
  if ( digitalRead(rbutton) == LOW)
{
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
}
 
}



void smoke(void)
{
int analogSensor = analogRead(smokeA0);

 Serial.print("Pin A0: ");
 Serial.println(analogSensor);
 
 // Checks if it has reached the threshold value
 if (analogSensor > sensorThres)
 {
   
    digitalWrite(D8, HIGH);
     lcd.clear();
  lcd.print(0, 0, "  GAS LEAKAGE     Emergency   ");
   delay(1000);
  //  Blynk.notify("T==> OVER GAS-BE ALERT");  
   
   Blynk.notify(" Gas leakage- Emergency  ");  
    lcd.clear();
 }
 else
 {
  
  digitalWrite(D8, LOW);
    
 }
}
