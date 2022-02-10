#include <MsTimer2.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include <DFRobot_TFmini.h>
#include <math.h>
#define VERSION 201215

//Adafruit_NeoPixel strip= Adafruit_NeoPixel(num, pin, NEO_GRB + NEO_KHZ800);
String command = "";
bool LED_Flag = false;
uint8_t LED_count = 0;
String RGB_Val = "";
String check_sum = "";
int first = 0;
uint8_t Red_i = 0, Green_i = 0, Blue_i = 0, Mode_i = 0;
bool flag = false;
bool f_flag = false;
//--------------------------------lidar-------------------------------
uint16_t distance, strength;
DFRobot_TFmini TFmini;
SoftwareSerial Lidar(8, 7);
uint16_t dist, stren;
uint16_t prev_dist;
//--------------------------------------------------------------------
//--------------------------------NTC10K------------------------------
volatile bool Temp_Flag = false;
int ADCdata;
const int numReadings = 128;
int readings[numReadings];
int readindex = 0;
uint32_t total = 0;
double average = 0;

float convertVolt;
double Rt, R_R25, ln_R;
double T;
float Temperature_Val;
//double A1_Param= 335.4016,B1_Param=25.6524,C1_Param= 0.260597,D1_Param= 0.00632926;
//double A1_Param= 0.00335401643468053,B1_Param=0.00025698501802,C1_Param=0.0000026201306709,D1_Param=0.000000063830907998;
//double A1_Param= 3354016434680530,B1_Param=256985018020000,C1_Param=2620130670900,D1_Param=63830907998; //*1000000000000000000
//double A1_Param=33540.16,B1_Param=2565.24,C1_Param=26.0597,D1_Param=0.632926;//A1,B1 = * 10000000
long long int A1_Param = 3354016434680530, B1_Param = 256985018020000, C1_Param = 2620130670900, D1_Param = 63830907998; //*1000000000000000000
double Temp;
static unsigned int c = 0;
//--------------------------------------------------------------------
//--------------------------------Neopixel----------------------------
int MinBrightness = 1;       //value 0-255
int MaxBrightness = 50;      //value 0-255

int numLoops1 = 10;
int numLoops2 = 5;
//int numLoops3 = 5;
//int numLoops4 = 3;          //add new integer and value for more color's loop if needed.

int fadeInWait = 30;          //lighting up speed, steps.
int fadeOutWait = 50;         //dimming speed, steps.
const int num = 24;
const int pin = 6;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(num, pin, NEO_GRB + NEO_KHZ800);
//--------------------------------------------------------------------
//--------------------------------Ultra_Sonic-------------------------
//--------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  TFmini.begin(Lidar);
  strip.begin();
  strip.clear();
  strip.setBrightness(127);
  ON_LED(255, 255, 255);
  for (uint8_t count = 0; count < numReadings; count++) {
    readings[count] = 0;
  }
  delay(1000);
  MsTimer2::set(100, timer_isr);
  MsTimer2::start();

}

void loop() {
  GetTemp(0);
  Get_distance();
  if (Serial.available()) {

    String command = "";
    String cTemp = "";
    int cTemp_cnt = 0;
    while (Serial.available())
    {
      char c;
      c = Serial.read();
      cTemp += c;
      cTemp_cnt++;
    }
    //Serial.println(cTemp);
    //Serial.println("cnt : " + String(cTemp_cnt));
    int len = cTemp_cnt;
    int Deli = cTemp.indexOf("#");
    first = cTemp.indexOf(",", Deli);
    command = cTemp.substring(0, first);
    if (command == "#CNT" && len == 10) {
      Split_Oper(cTemp);
    }

  }
  delay(10);
  if (LED_Flag == true) {
    LED_count++;
  }
  if (LED_count > 120 && LED_Flag == true) {
    ON_LED(255, 255, 255);
    LED_Flag = false;
    LED_count = 0;
  }
}
void timer_isr() {
  //Serial.println(Temperature_Val, 1);
  //Serial.println("ADC Val : " + String(ADCdata)+"\r\n");
  //Serial.println("Volt : " +String(convertVolt,3)+"\r\n");
  Serial.print("#CNT," + String(Temperature_Val, 1) + "," + String(dist) + "," + String(VERSION) + "\r\n");
}
void Split_Oper(String fileName) {
  int RGB_Index = fileName.indexOf("\\", first + 1);
  //int Blue_index = fileName.indexOf("\r", Green_index + 1);
  //int Mode_index=fileName.indexOf("\r",Blue_index+1);
  RGB_Val = fileName.substring(first + 1, RGB_Index);
  //Serial.println(RGB_Val);
  if (RGB_Val == "R") {
    ON_LED(255, 0, 0);
    LED_Flag = true;
  }
  else if (RGB_Val == "G") {
    ON_LED(0, 255, 0);
    LED_Flag = true;
  }
  else if (RGB_Val == "B") {
    ON_LED(0, 0, 255);
    LED_Flag = true;
  }
  else if (RGB_Val == "O") {
    ON_LED(255, 40, 0);
    LED_Flag = true;
  }
  else {
    ON_LED(255, 255, 255);
    LED_Flag = true;
  }


}
//------------------------lidar--------------------
void Get_distance() {


  while (!TFmini.measure());
  prev_dist = dist;
  dist = TFmini.getDistance();
  stren = TFmini.getStrength();
  if (dist <= 10) {
    dist = 200;
  }
  if (dist >= 200) {
    dist = 200;
  }

}
//--------------------------------------------------
//------------------------NTC10K--------------------
double GetTemp(int ch) {
  if (Temp_Flag == false) {
    int first_ADCdata = analogRead(ch);
    for (int thisReading = 0; thisReading < numReadings; thisReading++) {
      readings[thisReading] = first_ADCdata;
    }
    for (uint8_t count = 0 ; count < numReadings; count++) {
      total += readings[count];
    }
    readindex++;
    Temp_Flag = true;
  }
  else if (Temp_Flag == true) {
    int Rawinput = analogRead(ch);
    total = total - readings[readindex];
    readings[readindex] = Rawinput;
    total = total + readings[readindex];
    readindex++;
    if (readindex >= numReadings) {
      readindex = 0;
    }
  }




  
  average = (double)total / (double)numReadings;
  convertVolt = average * 5.0 / 1023;

  Rt = 10000.0 * average / (1024.0 - average);
  R_R25 = Rt / 10000;
  ln_R = log(R_R25);
  T = double(A1_Param + B1_Param * ln_R + C1_Param * (pow(ln_R, 2)) + D1_Param * (pow(ln_R, 3)));
  T = T / 1000000000000000000;
  T = 1 / T;
  Temperature_Val = T - 273.15;

}
//--------------------------------------------------
//----------------------neopixel--------------------
void ON_LED(const int R, const int G, const int B) {
  for (int j = 0; j < strip.numPixels(); j++) {
    strip.setPixelColor(j, strip.Color(R, G, B));
    strip.show();
  }
}
void rainbowBreathe(uint8_t x, uint8_t y) {
  for (int j = 0; j < x; j++) {
    for (uint8_t b = MinBrightness; b < MaxBrightness; b++) {
      strip.setBrightness(b * 255 / 255);
      for (uint8_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel(i * 256 / strip.numPixels()));
      }
      strip.show();
      delay(fadeInWait);
    }
    strip.setBrightness(MaxBrightness * 255 / 255);
    for (uint8_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(i * 256 / strip.numPixels()));
      strip.show();
      delay(y);
    }
    for (uint8_t b = MaxBrightness; b > MinBrightness; b--) {
      strip.setBrightness(b * 255 / 255);
      for (uint8_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel(i * 256 / strip.numPixels()));
      }
      strip.show();
      delay(fadeOutWait);
    }
  }
}
void rgbBreathe(uint32_t c, uint8_t x, uint8_t y) {
  for (int j = 0; j < x; j++) {
    for (uint8_t b = MinBrightness; b < MaxBrightness; b++) {
      strip.setBrightness(b * 255 / 255);
      for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
      }
      strip.show();
      delay(fadeInWait);
    }
    strip.setBrightness(MaxBrightness * 255 / 255);
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(y);
    }
    for (uint8_t b = MaxBrightness; b > MinBrightness; b--) {
      strip.setBrightness(b * 255 / 255);
      for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
      }
      strip.show();
      delay(fadeOutWait);
    }
  }
}



//NeoPixel에 달린 LED를 각각 주어진 인자값 색으로 채워나가는 함수
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

//모든 LED를 출력가능한 모든색으로 한번씩 보여주는 동작을 한번하는 함수
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//NeoPixel에 달린 LED를 각각 다른색으로 시작하여 다양한색으로 5번 반복한다
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//입력한 색으로 LED를 깜빡거리며 표현한다
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

//LED를 다양한색으로 표현하며 깜빡거린다
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   //256가지의 색을 표현
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255));
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);
      }
    }
  }
}

//255가지의 색을 나타내는 함수
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
