#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

MPU6050 accelgyro;

int16_t ax, ay, az, gx, gy, gz;

double timeStep, time, timePrev;
double arx, ary, arz, grx, gry, grz, gsx, gsy, gsz, rx, ry, rz;

int i;
double gyroScale = 131;

void setup() {

  Wire.begin();
  Serial.begin(9600);
  accelgyro.initialize();

  time = millis();

  i = 1;

}

int ok=0;     //variabilele globale de care am nevoie
 int first_z;
 int first_x;
 int first_y;
void procesare_coordonate(int arx,int ary,int arz)
{
  if(ok==0)
  {
   
    first_z=arz;
     first_x=arx;
     first_y=ary;
  
    
    ok=1;
  }
  else
  {
     Serial.print("Diferenta de unghi pe x  este:");
    Serial.println(arx-first_x);
     Serial.print("Diferenta de unghi pe y  este:");
    Serial.println(ary-first_y);
    
    Serial.print("Diferenta de unghi pe z  este:");
    Serial.println(arz-first_z);
    Serial.print("VITEZA VANTULUI=");
    Serial.println(abs(arx-first_x));
    
    
  }
}
void loop() {

  // set up time for integration
  timePrev = time;
  time = millis();
  timeStep = (time - timePrev) / 1000; // time-step in s

  // collect readings
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // apply gyro scale from datasheet
  gsx = gx/gyroScale;   gsy = gy/gyroScale;   gsz = gz/gyroScale;

  // calculate accelerometer angles
  arx = (180/3.141592) * atan(ax / sqrt(square(ay) + square(az))); 
  ary = (180/3.141592) * atan(ay / sqrt(square(ax) + square(az)));
  arz = (180/3.141592) * atan(sqrt(square(ay) + square(ax)) / az);

  // set initial values equal to accel values
  if (i == 1) {
    grx = arx;
    gry = ary;
    grz = arz;
  }
  // integrate to find the gyro angle
  else{
    grx = grx + (timeStep * gsx);
    gry = gry + (timeStep * gsy);
    grz = grz + (timeStep * gsz);
  }  

  // apply filter
  rx = (0.1 * arx) + (0.9 * grx);
  ry = (0.1 * ary) + (0.9 * gry);
  rz = (0.1 * arz) + (0.9 * grz);

  // print result
//  Serial.print(i);   Serial.print("\t");
//  Serial.print(timePrev);   Serial.print("\t");
//  Serial.print(time);   Serial.print("\t");
//  Serial.print(timeStep, 5);   Serial.println("\t\t");
  Serial.print("Angle on x:");
  Serial.println(arx);   
    Serial.print("Angle on y:");
  Serial.println(ary);   
    Serial.print("Angle on z:");
  Serial.println(arz);   
//  Serial.print(grx);   Serial.print("\t");
//  Serial.print(gry);   Serial.print("\t");
//  Serial.print(grz);   Serial.print("\t\t");
//  Serial.print(rx);   Serial.print("\t");
//  Serial.print(ry);   Serial.print("\t");
//  Serial.println(rz);
Serial.print("\n");


procesare_coordonate(arx,ary,arz);

  i = i + 1;
  delay(500);

}
