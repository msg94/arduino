#include <Adafruit_NeoPixel.h>
#include <DFRobot_TFmini.h>
#include <math.h>


//Adafruit_NeoPixel strip= Adafruit_NeoPixel(num, pin, NEO_GRB + NEO_KHZ800);
bool flag=false;
bool f_flag=false;
//--------------------------------lidar-------------------------------
uint16_t distance,strength;
DFRobot_TFmini TFmini;
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
static unsigned int c=0;
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
const int num = 20;
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
  strip.setBrightness(10);
    // initialize timer1 
  Serial.begin(115200);
  noInterrupts();           // disable all interrupts
  TCCR1A = 0; 
  TCCR1B = 0; 
  TCNT1  = 0; 
  OCR1A = 6249;       // compare match register 
  TCCR1B |= (1 << WGM12);   // CTC mode 
  TCCR1B |= (1 << CS12);    // 256 prescaler  
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt 
  /*TCCR1B = 0x00;  //stop the counter
  TCCR1A = 0x00;  
  TIMSK1 = 0x01;  //setting timer overflow interrupt enable
  TCNT1H = 0xff;
  TCNT1L = 0x05;  //for count 125 cycle
  TCCR1B = 0x03;  //enable the counter

  /*TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = 34286;            // preload timer 65536-16MHz/256/2Hz
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer compare interrupt*/
  interrupts();             // enable all interrupts
  }

void loop() {
  // put your main code here, to run repeatedly:
  if(flag==true){
    Temp=GetTemp(0);
    Serial.println(Temp);
    Get_distance();  
    flag=false; 
  }
  /*else if(flag==false&&f_flag==true){
    Get_distance();
    f_flag=false;
    
  }
  /*if(c%10==0){
      Temp = GetTemp(0);
      Serial.println(Temp);
      delay(50);
  }
  Get_distance();*/
}
ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{ 
  c++;
  //Serial.println(c);
  if(c>3){
    flag=true;
    c=0;
  }
  /* 
  c++;
  if(c>=30){
    flag=true;
    c=0;
  }
  //if(c==0)
  //flag=false;
  TCNT1H = 0xff;
  TCNT1L = 0x05;
  TIFR1 = 0x00;
*/


  
  //Serial.println(c);
}
//------------------------lidar--------------------
void Get_distance(){
  uint16_t dist,str;

 while(!TFmini.measure());                   
    dist = TFmini.getDistance();      
    str = TFmini.getStrength();   
    Serial.print("Distance = ");
    Serial.print(dist);
    Serial.print("cm");
    Serial.print("      Strength = ");
    Serial.println(str);    
  

}
//--------------------------------------------------
//------------------------NTC10K--------------------
double GetTemp(int ch){
  ADCdata = analogRead(ch);
  //Serial.println(ADCdata);
  convertVolt = ADCdata*5.0/1023;
  //Serial.println(convertVolt);
  Rt = 10000.0*(double)ADCdata/(1023.0-(double)ADCdata);
  //Serial.println(Rt);
  R_R25 = Rt/10000;
  //Serial.println(R_R25);
  ln_R = log(R_R25);
  //Serial.println(ln_R);
  T= A1_Param+B1_Param*ln_R+C1_Param*(pow(ln_R,2))+D1_Param*(pow(ln_R,3));
  T=T/10000000;
  T=1/T;
  //Serial.println(T);
  Temperature_Val=T-273.15;
  //Temp=Temperature_Val;
  //GetT=true;
  return Temperature_Val;
}
//--------------------------------------------------
//----------------------neopixel--------------------
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
