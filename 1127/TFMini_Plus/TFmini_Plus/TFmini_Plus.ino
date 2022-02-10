#include <DFRobot_TFmini.h>

#include <Adafruit_NeoPixel.h>

#include <math.h>


DFRobot_TFmini TFmini;
SoftwareSerial Lidar(8,7);
//Adafruit_NeoPixel strip= Adafruit_NeoPixel(num, pin, NEO_GRB + NEO_KHZ800);
//--------------------------------lidar-------------------------------
uint16_t dist,str;

//--------------------------------------------------------------------
//--------------------------------NTC10K------------------------------
int ADCdata;
float convertVolt;
double Rt,R_R25,ln_R;
double T;
float Temperature_Val;
  //double A1_Param= 335.4016,B1_Param=25.6524,C1_Param= 0.260597,D1_Param= 0.00632926;
  //double A1_Param= 0.003354016,B1_Param=0.000256524,C1_Param=0.00000260597,D1_Param=0.0000000632926;
double A1_Param=33540.16,B1_Param=2565.24,C1_Param=26.0597,D1_Param=0.632926;//A1,B1 = * 10000000
double Temp;
static unsigned int c=1;
//--------------------------------------------------------------------
//--------------------------------Neopixel----------------------------
//--------------------------------------------------------------------
//--------------------------------Ultra_Sonic-------------------------
//--------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  TFmini.begin(Lidar);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
   if (TFmini.measure()) {                     
    dist = TFmini.getDistance();      
    str = TFmini.getStrength();
    if(dist <=10){
      dist = 200;       
    }
    Serial.print("#CNT,0,"+String(dist)+",0\r\n");
    //Serial.print("Distance = ");
    //Serial.print(dist);
    //Serial.println("cm");
    //Serial.print("Strength = ");
    //Serial.println(str);
    
  }
  delay(100);
  
}
