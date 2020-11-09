#include <Adafruit_PCD8544.h> //Library for Nokia 5110 LCD
#include <Adafruit_GFX.h> //Adafruit Graphics Library
#include <gfxfont.h>
#include <SPI.h> //For communication between NodeMCU and LCD
#include <DHT.h> // DHT11 temperature and humidity sensor Library
#include <Time.h> 
#include <TimeLib.h>
#include <NTPClient.h> //Library for NTP Protocolj
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define DHTPIN D5      // DHTPIN : I have connected DHT11 Sensor to pin D5 (ie, GPIO0 of ESP8266
#define DHTTYPE DHT11  // We are using DHT11. (Can use DHT22.. or any other from this series)
#define RAINPIN A0     // FC-37 Analog Output pin is connected to A0

Adafruit_PCD8544 display = Adafruit_PCD8544(D4, D3, D2, D1, D0); // Nokia 5110 LCD module connections (CLK, DIN, D/C, CS, RST)
DHT dht = DHT(DHTPIN, DHTTYPE);   // DHT dht = DHT(DHTPIN,Sensor_type), "dht" is the name of the sensor (variable)

const char *ssid     = "SSID";
const char *password = "Password";
int DAY, MONTH, YEAR, SECONDS, HOURS ,MINUTES, WEEKDAY;
long int time_now; //Epoch Time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void setup() {
  Serial.begin(9600); //Begin serial communication at a baud rate of 9600
  pinMode(RAINPIN, INPUT);
  dht.begin(); //Initialize the sensor
  WiFi.begin(ssid, password);
  display.begin(); //Initialize the display
  display.clearDisplay();
  display.setContrast(50); // Adjust for your display
  display.setTextColor(BLACK, WHITE);
  display.display();
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  timeClient.begin();
}

void loop() {
  int h = dht.readHumidity();
  int tc = dht.readTemperature();   
  float tf = dht.readTemperature(true);   // Get Temperature in F   
  float hif = dht.computeHeatIndex(tf, h); // Heat index in F, Default
  float hic = dht.computeHeatIndex(tc, h, false);  // Heat index in C
  // This is just for the serial Monitor
  Serial.print("Humidity = ");
  Serial.print(h);
  Serial.print("% |");
  Serial.print("Temperature = ");
  Serial.print(tc); 
  Serial.print("\xC2\xB0 C ie, ");    // Serial.print(" \xC2\xB0"); is used to print the degree symbol in the serial monitor
  Serial.print(tf);
  Serial.print("\xC2\xB0 F |");
  Serial.print("Heat index = ");
  Serial.print(hic); 
  Serial.print("\xC2\xB0 C ie, ");
  Serial.print(hif);
  Serial.println("\xC2\xB0 F |");
  
  // To print Temperature (in Celcius)on LCD
  display.setCursor(0, 0); //(18,12); (35,0)
  display.printf("%02u.%1u C", (tc)%100, (tc)%10); // print degree symbol ( C ): %02u.%1u C
  display.setCursor(35, 0);
  display.printf(",");
  
  // To print humidity (in %) on LCD
  display.setCursor(43, 0); //(24, 40); (40, 10)
  display.printf("%02u.%1u %%", (h)%100, (h)%10); // print degree symbol ( % ): %02u.%1u %%
  display.display(); // To update the display

  //To print whether it is raining or not
  int raining = analogRead(RAINPIN);
  if(raining < 700){
    Serial.println("Water Detected!"); //Serial Monitor
    display.setCursor(20,12); 
    display.printf("Raining!"); //LCD
    display.display();
  }
  else {
    Serial.println("No Water Detected!"); //Serial Monitor
    display.setCursor(12,12);
    display.printf("Not raining!"); //LCD
    display.display();
  }

  //Using the NTP Protocol
  timeClient.update();
  time_now = timeClient.getEpochTime() + (19800); //5 hours 30 minutes = 19800s (India is GMT+5:30)
  DAY = day(time_now);
  MONTH = month(time_now);
  YEAR = year(time_now);
  HOURS = hourFormat12(time_now);
  MINUTES = minute(time_now);
  SECONDS = second(time_now);
  WEEKDAY = weekday(time_now);
  LCD_DISPLAY(String(DAY), String(MONTH), String(YEAR));
  delay(1000);
}
void LCD_DISPLAY(String DAY, String MONTH, String YEAR)
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(13,25); //(0,5)
  if (HOURS > 9){
    if(isPM(time_now)) 
        display.println(String(HOURS) + ":" + String(MINUTES) + ":" + String(SECONDS) + " " + "PM"); 
    else
        display.println(String(HOURS) + ":" + String(MINUTES) + ":" + String(SECONDS) + " " + "AM"); 
  }
  else if (HOURS < 10){
    if(isPM(time_now)) 
        display.println("0" + String(HOURS) + ":" + String(MINUTES) + ":" + String(SECONDS) + " " + "PM");
    else
        display.println("0" + String(HOURS) + ":" + String(MINUTES) + ":" + String(SECONDS) + " " + "AM");
  }
  display.setCursor(0, 40);
  //Printing the weekday
  switch(WEEKDAY)
  {
    case 1:  display.println("Sun,"); break;
    case 2:  display.println("Mon,"); break;
    case 3:  display.println("Tue,"); break;
    case 4:  display.println("Wed,"); break;
    case 5:  display.println("Thu,"); break;
    case 6:  display.println("Fri,"); break;
    default: display.println("Sat,");
  }
  display.setCursor(27, 40);
  display.println(DAY + "-" + MONTH + "-" + YEAR);
}
