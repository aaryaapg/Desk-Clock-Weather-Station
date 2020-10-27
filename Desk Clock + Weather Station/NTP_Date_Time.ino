#include <Time.h>
#include <TimeLib.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

const char *ssid     = "Padhyegurjar";
const char *password = "mncb8tbbt8";

Adafruit_PCD8544 display = Adafruit_PCD8544(D4, D3, D2, D1, D0); // Nokia 5110 LCD module connections (CLK, DIN, D/C, CS, RST)
int DAY, MONTH, YEAR, SECONDS, HOURS ,MINUTES;
long int time_now;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  display.begin();
  display.clearDisplay();
  display.setContrast(50);              // Adjust for your display
  display.setTextColor(BLACK);
  display.display();
  delay(2000);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
}

void loop() {
  timeClient.update();
  time_now = timeClient.getEpochTime() + (19800); //5 hours 30 minutes = 19800s (India is GMT+5:30)
  DAY = day(time_now);
  MONTH = month(time_now);
  YEAR = year(time_now);
  HOURS = hourFormat12(time_now);
  MINUTES = minute(time_now);
  SECONDS = second(time_now);
  LCD_DISPLAY(String(DAY), String(MONTH), String(YEAR));
  display.display();
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
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(15, 40);
  display.println(DAY + ":" + MONTH + ":" + YEAR);
}
