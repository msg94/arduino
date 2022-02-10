#include <MsTimer2.h>
#include <SoftwareSerial.h>
#include <math.h>
#define VERSION 210121
uint16_t dist, stren;
float Temperature_Val;
bool start_flag=false;
int start_count=0;
String command = "";
volatile bool LED_Flag = false;
uint64_t LED_count = 0;
String RGB_Val = "";
String Delay_Val_String = "";
int Delay_Val_Int = 0;
int first = 0;


void setup() {  
 
  init_Serial();
  init_neopixel();
   ON_LED(0,0,0);
  init_Lidar();
  init_array();
  ///delay(1000);
  MsTimer2::set(100, timer_isr);
  MsTimer2::start();
}

void loop() { 
  GetTemp(0);
  Get_distance();
  Serial_Event(); 
  delay(10);
}
void timer_isr() {
  //Serial.println(Temperature_Val, 1);
  Serial.print("#CNT," + String(Temperature_Val, 1) + "," + String(dist) + "," + String(VERSION) + "\r\n");
  if (LED_Flag == true) {
    LED_count++;
  }  
  if (LED_count > Delay_Val_Int && LED_Flag == true) {
    ON_LED(255, 255, 255);
    LED_Flag = false;
    LED_count = 0;
  }  
}
