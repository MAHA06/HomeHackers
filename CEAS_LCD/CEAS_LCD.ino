#include <LiquidCrystal.h>
LiquidCrystal lcd(7,6,5,4,3,2);

void setup() { 
  Serial.begin(9600);

  delay(1000);
  lcd.begin(16, 2);  
  lcd.setCursor(0, 0);
    lcd.print("ceasu pulii");
  // print out the date
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
void loop(){
    digitalClockDisplay();
}
void printDigits(byte digits){
    if(digits < 10)
        lcd.print('0');
    lcd.print(digits);
}
char sep()
{
    s = millis()/1000;
    if(s%2==0)
    {
        lcd.print(":");
    }
    else {
        lcd.print(" ");
    }  
}
void digitalClockDisplay(){
    lcd.setCursor(0,1);
    
    printDigits(
    hours());
    sep();
    printDigits(mins());
    sep();
    printDigits(secs());
}
