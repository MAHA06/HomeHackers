#include "IRremote.h"

int RECV_PIN = 9;

IRrecv irrecv(RECV_PIN);

decode_results results;
int inchidere=1;
void setup()
{
  
  pinMode(7,OUTPUT);
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
    digitalWrite(7,HIGH);
   Serial.print("ARDUINO HAS STARTED");
    pinMode(5,OUTPUT);
    pinMode(6,OUTPUT);
    analogWrite(6,0);
}
int incet=0;
int speed=0;
int citit()
{
  if ( irrecv.decode(&results)) 
 {
   Serial.println(results.value, HEX);
     irrecv.resume(); // Receive the next value
     return 1;
 }
 return 0;
}
void inchidere_fereastra()

{
 
 
  if(citit()==1)
    incet=1;
  
   
 }
  
 if(incet==1)
 {
  analogWrite(5,20);
 }
 else
 {
  analogWrite(5,40);

 }
 
  
  
}

   


void loop() {

    if(inchidere==1)
      inchidere_fereastra();
     
     

    
   
  }
 



