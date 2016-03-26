#include <VirtualWire.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9340.h"

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

// These are the pins used for the UNO
// for Due/Mega/Leonardo use the hardware SPI pins (which are different)
#define _sclk 13
#define _miso 12
#define _mosi 11
#define _cs 10
#define _dc 9
#define _rst 8

Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _rst);

int valoare=-40;
int shown=0;

const int buzz=3;
char ID [4];
int Humid, Temp, Wind, Rain;
char HumidC [4], TempC [4], WindC [3], RainC;
const int receive_pin = 2;
long long prev_time=0, curr_time;

namespace locatie
{
  typedef struct
  {
    int humidity;
    int temperature;
    int wind;
    int rain;
  } loc;
}

locatie::loc gradina, sera;

void setup() 
{
  vw_set_rx_pin(receive_pin);
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);  // Bits per sec
  vw_rx_start();       // Start the receiver PLL running
  delay(100);
  pinMode(7, INPUT);
  Serial.begin(57600);
  tft.begin();
  delay(100);
}

void loop(void) 
{
  ask_data();
  curr_time = millis();
  
  if (digitalRead(7) && shown==1) {shown=0; tell_garden();}
  if (digitalRead(7) && shown==0) {shown=1; tell_sera();}
  if (((curr_time-prev_time)>10000) && shown == 0) { tell_garden(); prev_time = millis();}
  if (((curr_time-prev_time)>10000) && shown == 1) { tell_sera(); prev_time = millis();}
}

void static_draw()
{
  tft.fillScreen(ILI9340_BLACK);
  tft.drawFastVLine(0,0,320, ILI9340_WHITE);
  tft.drawFastVLine(1,0,320, ILI9340_WHITE);
  tft.drawFastHLine(0,0,240, ILI9340_WHITE);
  tft.drawFastHLine(0,1,240, ILI9340_WHITE);
  tft.drawFastVLine(238,0,320, ILI9340_WHITE);
  tft.drawFastVLine(239,0,320, ILI9340_WHITE);
  tft.drawFastHLine(0,318,240, ILI9340_WHITE);
  tft.drawFastHLine(0,319,240, ILI9340_WHITE);
}

void alerta_ploaie()
{
  tft.setTextColor(ILI9340_RED);  tft.setTextSize(4);
  tft.print("It's raining!");
  
}

void tell_garden()
{
  tft.setRotation(0);
  static_draw();
  tft.setRotation(3);
  tft.setTextColor(ILI9340_WHITE);  tft.setTextSize(3);
  tft.setCursor(100, 10);
  tft.print("Gradina");
  tft.setCursor(10, 45);
  tft.print("Umiditate:");
  tft.setCursor(190,45);
  tft.print(gradina.humidity);
  tft.setCursor(250,45);
  tft.print("%");
  tft.setCursor(10, 85);
  tft.print("Temperatura:");
  tft.setCursor(220,85);
  tft.print(gradina.temperature);
  tft.setCursor(280,85);
  tft.setTextSize(2);
  tft.print("*");
  tft.setTextSize(3);
  tft.print("C");
  tft.setCursor(10, 125);
  if (gradina.wind == -333) {tft.print("No info about the"); tft.setCursor(10, 155); tft.print("wind.");}
    else
    {
      tft.print("Vant:");
      tft.setCursor(100,125);
      tft.print(gradina.wind);
      tft.setCursor(160,125);
      tft.print("km/h");
    }
 tft.setCursor(5, 185);
 if (gradina.rain==1) {alerta_ploaie(); tone(buzz, 5000, 300);}
}


void tell_sera()
{
  tft.setRotation(0);
  static_draw();
  tft.setRotation(3);
  tft.setTextColor(ILI9340_WHITE);  tft.setTextSize(3);
  tft.setCursor(130, 10);
  tft.print("Sera");
  tft.setCursor(10, 45);
  tft.print("Umiditate:");
  tft.setCursor(190,45);
  tft.print(sera.humidity);
  tft.setCursor(250,45);
  tft.print("%");
  tft.setCursor(10, 85);
  tft.print("Temperatura:");
  tft.setCursor(220,85);
  tft.print(sera.temperature);
  tft.setCursor(280,85);
  tft.setTextSize(2);
  tft.print("*");
  tft.setTextSize(3);
  tft.print("C");
  tft.setCursor(10, 125);
  if (sera.wind == -333) {tft.print("No info about the"); tft.setCursor(10, 155); tft.print("wind.");}
  else
  {
    tft.print("Vant:");
    tft.setCursor(100,125);
    tft.print(sera.wind);
    tft.setCursor(160,125);
    tft.print("km/h");
  }
 tft.setCursor(5, 185);
 if (sera.rain==1) {alerta_ploaie(); tone(buzz, 5000, 300);}
}


void ask_data()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;



    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
        int i;

        ID [0] = buf [0];
        ID [1] = buf [1];
        ID [2] = buf [2];
        ID [3] = 0;

        HumidC [0] = buf [3];
        HumidC [1] = buf [4];
        HumidC [2] = buf [5];
        HumidC [3] = 0;

        TempC [0] = buf [6];
        TempC [1] = buf [7];
        TempC [2] = buf [8];
        TempC [3] = 0;

        WindC [0] = buf [9];
        WindC [1] = buf [10];
        WindC [2] = 0;

        RainC = buf [11];

        if (strcmp (HumidC, "XXX") != 0)
          Humid = atoi (HumidC);
        else
          Humid = -333;
        if (strcmp (TempC, "XXX") != 0)
        {
          Temp = atoi (TempC + 1);
          if (TempC [0] == '1')
          {
            Temp = -Temp;
          }
        }
        else
        {
          Temp = -333;
        }
        if (strcmp (WindC, "XX") != 0)
          Wind = atoi (WindC);
        else
          Wind = -333;

        if (RainC != 'X')
          Rain = RainC - '0';
         else
          Rain = -333;

        if(!strcmp(ID, "GRD")) 
        { 
          gradina.humidity = Humid;
          gradina.temperature = Temp;
          gradina.wind = Wind;
          gradina.rain = Rain;
        
        }

        if(!strcmp(ID, "SER")) 
        { 
          sera.humidity = Humid;
          sera.temperature = Temp;
          sera.wind = Wind;
          sera.rain = Rain;
        
        }

        Serial.print ("ID: "); Serial.println (ID);
        Serial.print ("Humid: "); Serial.println (Humid);
        Serial.print ("Temp: "); Serial.println (Temp);
        Serial.print ("Wind: "); Serial.println (Wind);
        Serial.print ("Rain: "); Serial.println (Rain);
     }
}
