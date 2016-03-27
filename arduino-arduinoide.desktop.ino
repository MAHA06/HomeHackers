#include <VirtualWire.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9340.h"
#include "mario.h"

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

int WindAll=-333;
int valoare=-40;
int shown=0;

const int buzzer=3;
char ID [4];
int Humid, Temp, Wind, Rain;
char HumidC [4], TempC [4], WindC [3], RainC;
const int receive_pin = 2;
long long prev_time=0, curr_time;
int n=0,m=0,o=0;

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

locatie::loc gradina, curte;

void setup() 
{
  digitalWrite(4, LOW);
  vw_set_rx_pin(receive_pin);
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);  // Bits per sec
  vw_rx_start();       // Start the receiver PLL running
  delay(100);
  pinMode(7, INPUT);
  pinMode(3, OUTPUT);
  Serial.begin(57600);
  tft.begin();
  tell_garden();
  delay(100);
}

void loop(void) 
{
  delay(1000);
  alarma();
  ask_data();
  curr_time = millis();
  
  if (digitalRead(7) && shown==1) {shown=0; tell_garden();}
  if (digitalRead(7) && shown==0) {shown=1; tell_curte();}
  if (((curr_time-prev_time)>10000) && shown == 0) { tell_garden(); prev_time = millis();}
  if (((curr_time-prev_time)>10000) && shown == 1) { tell_curte(); prev_time = millis();}
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
 if (gradina.rain==1) {alerta_ploaie();inchidete(); sing();} else show_clock();
}


void tell_curte()
{
  tft.setRotation(0);
  static_draw();
  tft.setRotation(3);
  tft.setTextColor(ILI9340_WHITE);  tft.setTextSize(3);
  tft.setCursor(130, 10);
  tft.print("Curte");
  tft.setCursor(10, 45);
  tft.print("Umiditate:");
  tft.setCursor(190,45);
  tft.print(curte.humidity);
  tft.setCursor(250,45);
  tft.print("%");
  tft.setCursor(10, 85);
  tft.print("Temperatura:");
  tft.setCursor(220,85);
  tft.print(curte.temperature);
  tft.setCursor(280,85);
  tft.setTextSize(2);
  tft.print("*");
  tft.setTextSize(3);
  tft.print("C");
  tft.setCursor(10, 125);
  if (curte.wind == -333) {tft.print("No info about the"); tft.setCursor(10, 155); tft.print("wind.");}
  else
  {
    tft.print("Vant:");
    tft.setCursor(100,125);
    tft.print(curte.wind);
    tft.setCursor(160,125);
    tft.print("km/h");
  }
 tft.setCursor(5, 185);
 if (curte.rain==1) {alerta_ploaie();inchidete(); sing();} else show_clock();
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

        if(strcmp(ID, "GRD") == 0) 
        { 
          gradina.humidity = Humid;
          gradina.temperature = Temp;
          gradina.wind = WindAll;
          gradina.rain = Rain;
        
        }

        if(strcmp(ID, "CUR") == 0) 
        { 
          curte.humidity = Humid;
          curte.temperature = Temp;
          curte.wind = WindAll;
          curte.rain = Rain;
        }

        if (strcmp (ID, "BAL") == 0)
        {
          WindAll = Wind;
        }

        Serial.print ("ID: "); Serial.println (ID);
        Serial.print ("Humid: "); Serial.println (Humid);
        Serial.print ("Temp: "); Serial.println (Temp);
        Serial.print ("Wind: "); Serial.println (Wind);
        Serial.print ("Rain: "); Serial.println (Rain);
     }
}


void sing() 
{

 
    Serial.println(" 'Mario Theme'");
    int size = sizeof(melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {
 
      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / tempo[thisNote];
 
      buzz(3, melody[thisNote], noteDuration);
 
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
 
      // stop the tone playing:
      buzz(3, 0, noteDuration);
 
    }
}

 
void buzz(int targetPin, long frequency, long length) {
  
  long delayValue = (long)(1000000.0 / frequency / 2.0); // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = (long)(frequency * length / 1000.0); // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
}


void inchidete()

{
    int acc=1;
    int vel=2;
    int citit=1;
    int inchidere=1;
    while(inchidere==1)
    {
        citit=digitalRead(A0);
        if(citit==0)
            {
               acc=-acc;
               inchidere=0;
            }
              if(vel<200)
            {
                vel=vel+acc; if(acc<0) delay(20);
               analogWrite(5,vel);
            }
      }
      while(vel>=0)
        {
          vel=vel+acc;
            analogWrite(5,vel);
        }
        analogWrite(5,0);
}


  //thhis is a list of int variables used in this clock program
int s=0;
int sec=0;
int hrs=0;
int minutes=0;
int initialHours = 00;//variable to initiate hours
int initialMins = 0;//variable to initiate minutes
int initialSecs = 00;//variable to initiate seconds

//this method is for seconds
int seconds()
{
    s = initialHours*3600;
    s = s+(initialMins*60);
    s = s+initialSecs;
    s = s+(millis()/1000);
    return s;
}
//this method is for hours
int hours()
{
    hrs = seconds();
    hrs = hrs/3600;
    hrs = hrs%24;
    return hrs;
}
//this method is for minutes
int mins()
{
    minutes = seconds();
    minutes = minutes/60;
    minutes = minutes%60;
    return minutes;
}

int secs()
{
    sec = seconds();
    sec = sec%60;
    return sec;
}

//this loop will conintue to keep looping so that the time can go as follows

void alarma(){
  
    int current=0;
    if(Serial.available()>0)
    {
      current=Serial.read();
      n=n*10+current-48;
      current=Serial.read();
      n=n*10+current-48; 
      current=Serial.read();
      m=m*10+current-48;
      current=Serial.read();
      m=m*10+current-48;  
       current=Serial.read();
      o=o*10+current-48;
      current=Serial.read();
      o=o*10+current-48;
      
            
      }
      if((n==hours())&&(m==mins())&&(o==secs()))
      {
              
              sing();
              n=0;m=0;o=0;
              
}}


void show_clock()
{
  //hrs, minutes, sec
  tft.print(hours());
  tft.print(":");
  tft.print(mins());
  tft.print(":");
  tft.print(secs());
}

