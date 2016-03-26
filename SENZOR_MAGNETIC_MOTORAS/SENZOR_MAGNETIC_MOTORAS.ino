#include "IRremote.h"

int RECV_PIN = 9;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  pinMode(7,OUTPUT);
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
    digitalWrite(7,HIGH);
   Serial.print("ARDUINO HAS STARTED");
   
}

void loop() {
if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
 
}








