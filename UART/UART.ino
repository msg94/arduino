#include <Adafruit_NeoPixel.h>


char RX_Data;
volatile String TX_Data;
bool cmd_data = false;
unsigned long lastMsg = 0;
unsigned long lastOperation_time = 0;
unsigned long onoff_period = 2000;
char* cut;
char* Dcut[5];

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

bool R_flag=false;
bool G_flag=false;
bool B_flag=false;
bool Sleep_flag=false;
bool flash_flag=false;



Adafruit_NeoPixel strip = Adafruit_NeoPixel(num, pin, NEO_GRB + NEO_KHZ800);
void setup() {
  Serial.begin(115200);
  strip.begin();
  strip.clear();
  strip.setBrightness(10);
  ON_LED(50,50,50);
}

void loop() {
 /* unsigned long Time=millis();
  if ( cmd_data && (Time - lastMsg > onoff_period)) {
    cmd_data = false;
    ON_LED(50,50,50);
  }
  if(Serial.available()){
    delay(100);
    String Temp="";
    String Command="";
    String Data="";
    String Mode="";
    int Temp_cnt=0;
    while(Serial.available()){
      char ch;
      ch = Serial.read();
      Temp+=ch;
      Temp_cnt++;
    }
    if(strncmp("$VLOCK",Temp,6)==0){
      cut=strtok(Temp,",");
      for(uint8_t i=0; cut;i++){
        Dcut[i]=cut;
        cut=strtok(0,",");
      }
 
    }
    Serial.println(Dcut);
    /*
    Serial.println(Temp);
    int First_ch=Temp.indexOf(",");
    int Second_ch=Temp.indexOf(",");
    int Third_ch=Temp.indexOf("\r");
    int Length=Temp_cnt;
    Command=Temp.substring(0,First_ch);
    Serial.println(First_ch);
    Serial.println(Second_ch);
    Data=Temp.substring(First_ch+1,Second_ch-1);
    if(Command=="$VLOCK"){
      cmd_data=true;
      lastMsg=Time;
      Serial.println(Data);
      if(Data.toFloat()>=37.5){
        ON_LED(255,0,0);
      }
      else if(Data.toFloat()<=35.0){
        ON_LED(0,0,255);
      }
      else if(Data.toFloat()>35.0 && Data.toFloat()<37.5){
        ON_LED(0,255,0);
      }
      else if(Data=="BR"){
        rainbowBreathe(numLoops2, 250);
        
      }
      
    }*/
  //}
  

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



//NeoPixel에 달린 LED를 각각 주어진 인자값 색으로 채워나가는 함수
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

//모든 LED를 출력가능한 모든색으로 한번씩 보여주는 동작을 한번하는 함수
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

//NeoPixel에 달린 LED를 각각 다른색으로 시작하여 다양한색으로 5번 반복한다
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

//입력한 색으로 LED를 깜빡거리며 표현한다
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

//LED를 다양한색으로 표현하며 깜빡거린다
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     //256가지의 색을 표현
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

//255가지의 색을 나타내는 함수
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
