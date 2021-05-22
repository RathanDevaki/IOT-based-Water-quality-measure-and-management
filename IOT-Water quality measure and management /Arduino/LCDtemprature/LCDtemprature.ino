#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
 
#define ONE_WIRE_PIN 8
OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature sensors(&oneWire);
 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
 
void setup() { 
    lcd.begin(16, 2);
    lcd.print("Current Temp:");
}
 
void loop() {
    lcd.setCursor(0, 1);
    sensors.requestTemperatures();
    lcd.print(sensors.getTempCByIndex(0));
    lcd.print("C");
    delay(1000);
}
