#include <Blynk.h>

//ths will work
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

// Data wire is plugged into digital pin 2 on the Arduino
#define ONE_WIRE_BUS 8
//ph 
#define SensorPin A5 //pH meter Analog output to Arduino Analog Input 0

#define Offset 0.00 //deviation compensate

#define LED 13

#define samplingInterval 20

#define printInterval 100

#define ArrayLenth 40 //times of collection

// float phValue = -5.70 * voltage + 21.34;

int pHArray[ArrayLenth]; //Store the average value of the sensor feedback

int pHArrayIndex = 0;

//lcd
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);  

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

void setup(void)
{
  sensors.begin();  // Start up the library
    lcd.begin(16, 2);
  Serial.begin(9600);
  lcd.print("  WATER QUALITY");
  lcd.setCursor(0,1);
  lcd.print("   MEASUREMENT ");
  delay(2000);
  lcd.clear();

  pinMode(LED, OUTPUT);
}

void loop(void)
{ 
  // Send the command to get temperatures
  sensors.requestTemperatures(); 

  //print the temperature in Celsius
  Serial.print("\nTEMPERATURE: ");
  Serial.print(sensors.getTempCByIndex(0));
  Serial.print("Degree ");//shows degrees character
  Serial.print("C  |  ");
  //print the temperature in Fahrenheit
  Serial.print((sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0);
  Serial.print("Degree ");//shows degrees character
  Serial.println("F\n");
//lcd print
lcd.setCursor(0,0);
lcd.print("TEMPERATURE:");
lcd.setCursor(0,1);
float temp1=sensors.getTempCByIndex(0);
lcd.print(temp1);
lcd.print((char)223);     // degree symbol
lcd.print("C");
delay(1000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("TEMPERATURE");

lcd.setCursor(0,1);
lcd.print((sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0);
lcd.print((char)223);
lcd.print("F");
delay(1000);
lcd.clear();
//
  int sensorValue = analogRead(A0);// read the input on analog pin 0:
  float voltage = sensorValue * (5.0 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  Serial.println("\nTurbidity");
  Serial.println(voltage); // print out the value you read:
  lcd.setCursor(0,0);
  lcd.print("TURBIDITY LEVEL");
  lcd.setCursor(0,1);
   lcd.print(voltage);
  delay(1000);
  lcd.clear();

//ph cont..
 static unsigned long samplingTime = millis();

  static unsigned long printTime = millis();

  static float pHValue, voltagePH;

  if (millis() - samplingTime > samplingInterval) {

    pHArray[pHArrayIndex++] = analogRead(SensorPin);

    if (pHArrayIndex == ArrayLenth)pHArrayIndex = 0;

    voltagePH = avergearray(pHArray, ArrayLenth) * 5.0 / 1024;

    pHValue = 3.5 * voltagePH + Offset;

    samplingTime = millis();

  }
  if (millis() - printTime > printInterval) { //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
    Serial.println("\npH METER"); //Test the serial monitor
    Serial.print("Voltage:");

    Serial.print(voltagePH, 2);

    Serial.print(" pH value: ");

    Serial.println(pHValue, 2);
    
    digitalWrite(LED, digitalRead(LED) ^ 1);

    printTime = millis();
   //voltage lcd
    lcd.setCursor(0,0);
    lcd.print("VOLTAGE");
    lcd.setCursor(0,1);
    lcd.print(voltagePH);
    delay(1000);
    lcd.clear();
  //ph lcd
     lcd.setCursor(0,0);
    lcd.print("PH LEVEL");
    lcd.setCursor(0,1);
    lcd.print(pHValue);
    //ph lcd
    delay(1000);
    lcd.clear();
    
  }

}

double avergearray(int* arr, int number) {

  int i;

  int max, min;

  double avg;

  long amount = 0;

  if (number <= 0) {

    Serial.println("Error number for the array to avraging!/n");

    return 0;

  }

  if (number < 5) { //less than 5, calculated directly statistics

    for (i = 0; i < number; i++) {

      amount += arr[i];

    }

    avg = amount / number;

    return avg;

  }

  else {

    if (arr[0] < arr[1]) {

      min = arr[0]; max = arr[1];

    }

    else {

      min = arr[1]; max = arr[0];

    }

    for (i = 2; i < number; i++) {

      if (arr[i] < min) {

        amount += min; //arr<min

        min = arr[i];

      }

      else {

        if (arr[i] > max) {

          amount += max; //arr>max

          max = arr[i];

        }

        else {

          amount += arr[i]; //min<=arr<=max

        }

      }

    }
    avg = (double)amount / (number - 2);

  }
  return avg;
}
  
