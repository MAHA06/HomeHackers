#include <IRremote.h>



//pinii necesari
// 5 si 6 PWM pentru motor
//9 pt citit senzoru
//foloseste variabila {inchidere} pentru a declansa inchiderea trapei.El creste liniar viteza si scade cand sensoru recepteaza un stimul

decode_results results;
int inchidere=1;
void setup()
{
  

  Serial.begin(9600);
 pinMode(9,INPUT);
    
   Serial.print("ARDUINO HAS STARTED");
    pinMode(5,OUTPUT);
    pinMode(6,OUTPUT);
    analogWrite(6,0);
}



   
int acc=2;
int vel=1;
int citit=1;

void loop() {
   Serial.println(citit);
if(citit==1)
  {
    citit=digitalRead(9);
   
      if(citit==0)
      {
         acc=-acc;
         vel=vel-1;
      }
    }
 
 

    if(inchidere==1)
      {
       
      
        if((vel>0)&&(vel<=100))
        {
          vel=vel+acc;
           analogWrite(5,vel);
        }
        Serial.println(acc);
       
         
      }
 
     
     

    
   
  }
 



