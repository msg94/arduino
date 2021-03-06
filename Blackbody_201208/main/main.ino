#include <MsTimer2.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include <DFRobot_TFmini.h>
#include <math.h>
#define VERSION 201124

//Adafruit_NeoPixel strip= Adafruit_NeoPixel(num, pin, NEO_GRB + NEO_KHZ800);
String command = "";
String Red = "", Green = "", Blue = "", Mode = "";
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
int ADCdata;
int EMA_S=0;
float EMA_a=0.6;
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
  strip.setBrightness(255);
  ON_LED(255, 255, 255);
  MsTimer2::set(200, timer_isr);
  MsTimer2::start();
  EMA_S=analogRead(0);
}

void loop() {
  GetTemp(0);
  Get_distance();
  delay(10);
  if (Serial.available()) {
    String command = "";
    String cTemp;
    int cTemp_cnt;
    String Red = "", Green = "", Blue = "", Mode = "";
    uint8_t Red_i = 0, Green_i = 0, Blue_i = 0, Mode_i = 0;

    while (Serial.available())
    {
      char c;
      c = Serial.read();
      cTemp += c;
      cTemp_cnt++;
    }
    int len = cTemp_cnt;
    Split_Oper(cTemp);

  }
}
void timer_isr() {
  //Serial.println(Temperature_Val,1);
  //Serial.println("ADC Val : " + String(ADCdata)+"\r\n");
  //Serial.println("Volt : " +String(convertVolt,3)+"\r\n");
  Serial.println("#CNT," + String(Temperature_Val,1) + "," + String(dist) + "," + String(VERSION) + "\r\n");
}
void Split_Oper(String fileName) {
  int Deli = fileName.indexOf("#");
  int first = fileName.indexOf(",", Deli);
  int Red_index = fileName.indexOf(",", first + 1);
  int Green_index = fileName.indexOf(",", Red_index + 1);
  int Blue_index = fileName.indexOf("\r", Green_index + 1);
  //int Mode_index=fileName.indexOf("\r",Blue_index+1);
  command = fileName.substring(0, first);
  Red = fileName.substring(first + 1, Red_index);
  Red_i = Red.toInt();

  Green = fileName.substring(Red_index + 1, Green_index);
  Green_i = Green.toInt();
  Blue = fileName.substring(Green_index + 1, Blue_index);
  Blue_i = Blue.toInt();
  //Mode=fileName.substring(Blue_index+1,Mode_index);
  //Mode_i=Mode.toInt();
  if (Red_i <= 0 || Red_i > 255 || Blue_i <= 0 || Blue_i > 255 || Green_i <= 0 || Green_i > 255) {
    Red_i = 255;
    Blue_i = 255;
    Green_i = 255;
  }
  ON_LED(Red_i, Green_i, Blue_i);
  delay(1500);
  ON_LED(255, 255, 255);
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
  ADCdata = analogRead(ch);
  EMA_S=(EMA_a*ADCdata)+((1-EMA_a)*EMA_S);
  convertVolt = EMA_S * 5.0 / 1024;
  
  Rt = 10000.0 * (double)ADCdata / (1024.0 - (double)ADCdata);
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



//NeoPixel??? ?????? LED??? ?????? ????????? ????????? ????????? ??????????????? ??????
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

//?????? LED??? ??????????????? ??????????????? ????????? ???????????? ????????? ???????????? ??????
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

//NeoPixel??? ?????? LED??? ?????? ??????????????? ???????????? ?????????????????? 5??? ????????????
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

//????????? ????????? LED??? ??????????????? ????????????
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

//LED??? ?????????????????? ???????????? ???????????????
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   //256????????? ?????? ??????
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

//255????????? ?????? ???????????? ??????
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
