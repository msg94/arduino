#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(20, 6, NEO_GRB + NEO_KHZ800);
#include "Arduino.h"
#include "neopixel.h"

int MinBrightness = 1;       //value 0-255
int MaxBrightness = 50;      //value 0-255

int numLoops1 = 10;
int numLoops2 = 5;
//int numLoops3 = 5;
//int numLoops4 = 3;          //add new integer and value for more color's loop if needed.

int fadeInWait = 30;          //lighting up speed, steps.
int fadeOutWait = 50;         //dimming speed, steps.
const int num = 20;
const int pin = 6;
  

void neopixcel_init(){
  
  strip.begin();
  strip.clear();
  strip.setBrightness(255);

}

void ON_LED(const int R, const int G, const int B){
  for (int j =0; j < strip.numPixels(); j++) {
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
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

//?????? LED??? ??????????????? ??????????????? ????????? ???????????? ????????? ???????????? ??????
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//NeoPixel??? ?????? LED??? ?????? ??????????????? ???????????? ?????????????????? 5??? ????????????
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { 
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//????????? ????????? LED??? ??????????????? ????????????
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//LED??? ?????????????????? ???????????? ???????????????
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     //256????????? ?????? ??????
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0); 
        }
    }
  }
}

//255????????? ?????? ???????????? ??????
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
