#include <MsTimer2.h>
#include <SoftwareSerial.h>
#include <math.h>
#define VERSION 210215
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
int timer_count = 0;

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
  delay(1);  
  Serial_Event(); 
  delay(1);
}
void timer_isr() {
  //Serial.println(Temperature_Val, 1);
  Serial.print("#CNT," + String(Temperature_Val, 1) + "," + String(dist) + "," + String(VERSION) + "\r\n");
}
