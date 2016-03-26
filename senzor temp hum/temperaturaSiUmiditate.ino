
void setup(){
  Serial.begin(9600);
}

void loop(){
  Serial.print("Temperatura: ");
  int temperatura = readTempInCelsius(10,5);
  Serial.print(temperatura);
  Serial.print("  ");
  Serial.write(176); // caracter grade celsius 
  Serial.println("C");

  Serial.print("Humidity ");

  int senzorHumidity = analogRead(A0);
  
  Serial.print(relativeHumidityPercentage(temperatura, senzorHumidity));
  Serial.println("%");
  
  delay(500);
}

float readTempInCelsius(int count, int pin) {
  float temperaturaMediata = 0;
  float sumaTemperatura = 0;
  for (int i =0; i < count; i++) {
    int reading = analogRead(pin);
    float voltage = reading * 5.0;
    voltage /= 1024.0;
    float temperatureCelsius = (voltage - 0.5) * 100 ;
    sumaTemperatura = sumaTemperatura + temperatureCelsius;
  }
  return sumaTemperatura / (float)count;
}

const long int tabelValori[10][9] = { // temperatura / valori senzor de umiditate ==> RH %
  //RH 10%    20%    30%   40%  50%   60%  70%   90%  
    {350000, 40000, 7200, 1800, 510, 165, 56, 21, 8}, // 0 grade
    {150000, 22000, 4600, 1200, 360, 118, 41, 15, 6}, // 5 grade
    {95000, 15000, 3300, 860, 260, 86, 30, 11, 4},//10 grade
    {65000, 10000, 2100, 590, 190, 63, 22, 9, 4},// 15 grade
    {43000, 6900, 1600, 460, 145, 45, 16, 7, 3},
    {32000, 5000, 1200, 340, 110, 33, 12, 5, 2},
    {24000, 3900, 910, 260, 83, 26, 10, 4, 1},//30 grade
    {18000, 3000, 710, 210, 63, 20, 8, 3, 1},
    {13000, 2400, 580, 163, 47, 16, 7, 3, 1},
    {9800, 1900, 470, 128, 37, 13, 5, 3, 1}// 45 grade
  };

int long relativeHumidityPercentage(int temperature, int sensorHumidity)
{
 int long rowTabel = temperature / 5;
 int j;
 int RelativeHumidity;

 for(j = 0; j < 10 - 1; j++) 
  {
    if(tabelValori[rowTabel][j] > sensorHumidity && sensorHumidity >  tabelValori[rowTabel][j + 1])
    {
      int long var_1_la_suta = (tabelValori[rowTabel][j] - tabelValori[rowTabel][j + 1]) / 10;

      int count = 0;
      while(sensorHumidity > tabelValori[rowTabel][j + 1])
      {
        sensorHumidity -= var_1_la_suta;
        ++count;
      }

      RelativeHumidity = ((j + 2) * 10 ) - count;
      break;
    }
   }

   return RelativeHumidity;
}

