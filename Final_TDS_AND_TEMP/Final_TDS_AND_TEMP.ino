#include <OneWire.h>
#define TdsSensorPin A0
#include <LiquidCrystal_I2C.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include <Wire.h>
#include "GravityTDS.h"

LiquidCrystal_I2C lcd(0x27,20,4); //0X27-adress, 20-columns, 4-rows

#define ONE_WIRE_BUS 8
//Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS);
GravityTDS gravityTds;
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

float tempC = 0;
float tdsValue = 0;
float volt;
float ntu;
int sensorPin = A1;

void setup() {
  
  sensors.begin();
  lcd.init();          // initialize the lcd 
 // lcd.init();
 // Turn on the blacklight and print a message.
 lcd.backlight();
 // lcd.setCursor(3,0); //a patra coloana, prima linie 
  
 // lcd.setCursor(2,1);
  
  // lcd.setCursor(0,2);
 
  // lcd.setCursor(2,3); 
   gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
    gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
    gravityTds.begin();
 
  Serial.begin(9600);
  
}

void loop() {
  // call sensors.requestTemperatures() to issue a global temperature 
 // request to all devices on the bus 
 // Send the command to get temperature reading
  sensors.requestTemperatures();
  // You can have more than one DS18B20 on the same bus.  
   // 0 refers to the first IC on the wire 
  tempC = sensors.getTempCByIndex(0);
  gravityTds.update();  //sample and calculate 
    tdsValue = gravityTds.getTdsValue();
  delay(1000);
  
  Serial.println(tempC);
  lcd.setCursor(0,0);
  lcd.print("TEMP:");
  lcd.print(tempC);
  lcd.print(" C");
  lcd.setCursor(0,1);
  lcd.print("TDS:");
  lcd.print(tdsValue);
  lcd.print(" ppm");

//tensiunea de iesire variaza destul de mult si e greu de masurat
//incadram in 800 de citiri si luam valoarea medie pentru citire.
volt = 0;
    for(int i=0; i<800; i++)
    {
        volt += ((float)analogRead(sensorPin)/1023)*5;
    }
    volt = volt/800;
   // volt = round_to_dp(volt,2);
    if(volt < 2.5){
      ntu = 3000;
    }else{
   //convertim analog voltage in NTU
   ntu = -1120.4*square(volt)+5742.3*volt-4353.8; 
    }

Serial.println(volt);
Serial.println(" V");
Serial.println(ntu);
Serial.println(" NTU");
}
/*    float round_to_dp( float in_value, int decimal_place )
{
  float multiplier = powf( 10.0f, decimal_place );
  in_value = roundf( in_value * multiplier ) / multiplier;
  return in_value;
} */
