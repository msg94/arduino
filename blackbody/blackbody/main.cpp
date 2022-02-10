#include "Arduino.h"
#include "main.h"
#include "ntc.h"
#include <SoftwareSerial.h>
#include <MsTimer2.h>
#include <math.h>
unsigned int c=1;
double Temp;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  MsTimer2::set(100,count);
  MsTimer2::start();
}

void loop() {
  if(c%10==0){
    Temp = GetTemp(0);
    Serial.println(Temp);
    Serial.print(c/10);
    Serial.println(" Sec");
    delay(500); 
  }
  // put your main code here, to run repeatedly:

}
void count(){
  if(c>36000)
    c=1;
  c++;  
}
