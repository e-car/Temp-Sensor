//       uno  mega
//digital 10   53  :SS
//digital 11   51  :MOSI
//digital 12   50  :MISO
//digital 13   52  :SCK

#include <SPI.h>
#define SLAVE1 10
#define SLAVE2 9

unsigned int thermocouple;  // 14-Bit Thermocouple Temperature Data + 2-Bit
unsigned int internal;      // 12-Bit Internal Temperature Data + 4-Bit
float time;
float disp;                 // display value
char s[10];
char c[10];

void tmp(int x){
  digitalWrite(x,LOW);                                     //  Enable the chip
  thermocouple  = (unsigned int)SPI.transfer(0x00) << 8;   //  Read high byte thermocouple
  thermocouple |= (unsigned int)SPI.transfer(0x00);       //  Read low byte thermocouple 
  internal  = (unsigned int)SPI.transfer(0x00) << 8;       //  Read high byte internal
  internal |= (unsigned int)SPI.transfer(0x00);           //  Read low byte internal 
  digitalWrite(x, HIGH);                                  //  Disable the chip
  
  if((thermocouple & 0x0001) != 0) {
    Serial.print("ERROR: ");
    if ((internal & 0x0004) !=0)  Serial.print("Short to Vcc, ");
    if ((internal & 0x0002) !=0)  Serial.print("Short to GND, ");
    if ((internal & 0x0001) !=0)  Serial.print("Open Circuit, ");
    Serial.println();
  }
  else {
    if((thermocouple & 0x8000) == 0){ // 0℃以上   above 0 Degrees Celsius 
      disp = (thermocouple >> 2) * 0.25;
    } else {                          // 0℃未満   below zero
      disp = (0x3fff - (thermocouple >> 2) + 1)  * -0.25;
    }
    time = millis();
    time /= 1000;
    dtostrf(time, 6, 3, s);  //時間
    dtostrf(disp, 6, 2, c);  //温度
    Serial.print(s);
    Serial.print(";TMP;");
    Serial.print(c);
    Serial.print(";");
    Serial.println();    
  }
  delay(500);
}

void setup(){
  pinMode(SLAVE1, OUTPUT);
  digitalWrite(SLAVE1, HIGH);
  pinMode(SLAVE2, OUTPUT);
  digitalWrite(SLAVE2, HIGH);
  
  Serial.begin(9600);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setDataMode(SPI_MODE0);
}

void loop(){
  tmp(SLAVE1);
  tmp(SLAVE2);
}
