#include <Adafruit_PCD8544.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <SPI.h>
#include <DHT.h>        // DHT11 temperature and humidity sensor Library

# define DHTPIN D5      // DHTPIN : I have connected DHT11 Sensor to pin D5 (ie, GPIO0 of ESP8266
# define DHTTYPE DHT11  // We are using DHT11. (Can use DHT22.. or any other from this series

Adafruit_PCD8544 display = Adafruit_PCD8544(D4, D3, D2, D1, D0); // Nokia 5110 LCD module connections (CLK, DIN, D/C, CS, RST)
DHT dht = DHT(DHTPIN, DHTTYPE);   // DHT dht = DHT(DHTPIN,Sensor_type), "dht" is the name of the sensor (variable)

void setup(void)
{ 
  display.begin(); // initialize the display
  display.setContrast(50); // You can change the contrast around to adapt the display
  display.clearDisplay();  /* Clear the screen and buffer so that garbage value isnt't printed on the sceen when it starts */
  //display.drawFastHLine(0, 20, display.width(), BLACK); // Draw Horizontal line
  display.setTextSize(1); // Size can be 1 or 2 or 3 etc
  display.setTextColor(BLACK, WHITE); // To Display inverted text: display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 0); //(6, 0)
  display.print("TEMP.:");
  display.setCursor(0, 10); //(15, 28)
  display.print("HUMI.:");
  display.display();
  
  dht.begin();          //Initialize the sensor
  Serial.begin(9600);   //Begin serial communication at a baud rate of 9600
  Serial.println("Humidity and temperature\n\n");
  delay(100);
}
void loop() {
    int h = dht.readHumidity();
    int tc = dht.readTemperature();   
    float tf = dht.readTemperature(true);   // Get Temperature in F   
    float hif = dht.computeHeatIndex(tf, h); // Heat index in F, Default
    float hic = dht.computeHeatIndex(tc, h, false);  // Heat index in C
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

    // To print temperature (in °C)
    display.setCursor(35, 0); //(18,12);
    display.printf(" %02u.%1u C", (tc)%100, tc % 10); // print degree symbol ( ° ): %02u.%1u C
 
    // To print humidity (in %)
    display.setCursor(40, 10); //(24, 40);
    display.printf("%02u.%1u %%", (h)%100, h % 10); // print degree symbol ( % ): %02u.%1u %%
 
    display.display(); // To update the display
    delay(2000); // Delay of 2 seconds between readings
}
