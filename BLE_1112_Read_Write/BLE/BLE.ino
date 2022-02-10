#include <DFRobot_TFmini_Nano.h>

#include "TcTimer5.h"
#include <ArduinoBLE.h>
#include <Adafruit_NeoPixel.h>
#include <math.h>
#include <string.h>
#define VERSION 201112

//Adafruit_NeoPixel strip= Adafruit_NeoPixel(num, pin, NEO_GRB + NEO_KHZ800);
bool BLE_Connect_flag = false;
bool flag = false;
bool f_flag = false;
//---------------------------------timer------------------------------
volatile uint32_t Counter_timer=0;
//--------------------------------lidar-------------------------------
uint16_t distance, strength;
DFRobot_TFmini TFmini;
//--------------------------------------------------------------------
//--------------------------------NTC10K------------------------------
int ADCdata;
float convertVolt;
double Rt, R_R25, ln_R;
double T;
float Temperature_Val;
//double A1_Param= 335.4016,B1_Param=25.6524,C1_Param= 0.260597,D1_Param= 0.00632926;
//double A1_Param= 0.003354016,B1_Param=0.000256524,C1_Param=0.00000260597,D1_Param=0.0000000632926;
double A1_Param = 33540.16, B1_Param = 2565.24, C1_Param = 26.0597, D1_Param = 0.632926; //A1,B1 = * 10000000
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
const int num = 20;
const int pin = 7;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(num, pin, NEO_GRB + NEO_KHZ800);
//--------------------------------------------------------------------
//--------------------------------Ultra_Sonic-------------------------
//--------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
// BLE UUIDs
//----------------------------------------------------------------------------------------------------------------------

#define BLE_UUID_TEST_SERVICE               "b337339a-78f7-4ac0-b903-8943d83fa378"
#define BLE_UUID_WRITE                      "b337339b-78f7-4ac0-b903-8943d83fa378"
#define BLE_UUID_READ                       "b337339c-78f7-4ac0-b903-8943d83fa378"

//----------------------------------------------------------------------------------------------------------------------
// BLE
//----------------------------------------------------------------------------------------------------------------------

BLEService testService( BLE_UUID_TEST_SERVICE );
BLEStringCharacteristic CT_WRITE( BLE_UUID_WRITE, BLEWrite, 40 );
BLEStringCharacteristic CT_READ( BLE_UUID_READ, BLERead|BLENotify, 40 );
long previousMillis = 0;
String fileName = "";
String message = "";
String command="";
String Red="",Green="",Blue="",Mode="";
uint8_t Red_i=0,Green_i=0,Blue_i=0,Mode_i=0;
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT); 
  Serial.begin( 115200 );
  //while ( !Serial );
  Serial.println("Start");

  BLE.begin();

  // set advertised local name and service UUID:
  BLE.setDeviceName( "C&T Arduino" );
  BLE.setLocalName( "C&T Arduino" );
  BLE.setAdvertisedService( testService );

  // BLE add characteristics
  testService.addCharacteristic( CT_READ );
  testService.addCharacteristic( CT_WRITE );

  // add service
  BLE.addService( testService );

  // set the initial value for the characeristic:
  //fileNameCharacteristic.writeValue( fileName );
  BLE.setEventHandler(BLEConnected, BLE_connect_event);
  BLE.setEventHandler(BLEDisconnected, BLE_disconnect_event);
  CT_WRITE.setEventHandler(BLEWritten, BLE_RX_event);
  // start advertising
  BLE.advertise();
  TFmini.begin();
  TcTimer5::begin();
  TcTimer5::start(NULL);
  strip.begin();
  strip.clear();
  strip.setBrightness(10);
  ON_LED(0,0,0);
  strip.show();
  Serial.println("Start");
}

void loop()
{
  BLE.poll();
  if (BLE_Connect_flag == true) {
    //ON_LED(50,50,50);
    if (TcTimer5::get(0) >= 10) {
      //Serial.println("event1");
      TcTimer5::reset(0);
      distance = Get_distance();
      Temp = GetTemp(0);
      Counter_timer++;
    }
    if(Counter_timer>=30){
      //Serial.println("event2");
      CT_READ.writeValue("#CNT,"+String(Temp)+","+String(distance)+","+String(VERSION)+"\r\n");
      Counter_timer=0;
    }
    delay(1);
  }
}
void BLE_connect_event(BLEDevice central) {
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
  digitalWrite(LED_BUILTIN, HIGH);
  ON_LED(50,50,50);
  BLE_Connect_flag = true;
}
void BLE_disconnect_event(BLEDevice central) {
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
  digitalWrite(LED_BUILTIN, LOW);
  ON_LED(0,0,0);
  BLE_Connect_flag = false;
}
void BLE_RX_event(BLEDevice central, BLECharacteristic characteristic) {
  Serial.print("Characteristic event, written: ");
  if ( CT_WRITE.written() )
  {
    fileName = CT_WRITE.value();
    Serial.print( fileName + "\r\n" );    
  }
  Split_Oper(fileName);
  
  //fileNameCharacteristic.writeValue("echo: " + String(distance) + "\r\n");
}
void Split_Oper(String fileName){
  int first=fileName.indexOf(",");
  int Red_index=fileName.indexOf(",",first+1);
  int Green_index=fileName.indexOf(",",Red_index+1);
  int Blue_index=fileName.indexOf(",",Green_index+1);
  int Mode_index=fileName.indexOf("\r",Blue_index+1);
  command = fileName.substring(0,first);
  Red=fileName.substring(first+1,Red_index);
  Red_i=Red.toInt();
  Green=fileName.substring(Red_index+1,Green_index);
  Green_i=Green.toInt();
  Blue=fileName.substring(Green_index+1,Blue_index);
  Blue_i=Blue.toInt();
  Mode=fileName.substring(Blue_index+1,Mode_index);
  Mode_i=Mode.toInt();
  ON_LED(Red_i,Green_i,Blue_i);
  Serial.println(command + "  " + Red +"  "+Green+"  "+Blue+"  "+Mode); 
}
//--------------------------------event_isr--------
void event_isr(){
  Counter_timer++;  
}
//------------------------lidar--------------------
uint16_t Get_distance() {
  uint16_t dist, stre;

  if (TFmini.measure()) {
    dist = TFmini.getDistance();
    stre = TFmini.getStrength();
    return dist;
  }

}
//--------------------------------------------------
//------------------------NTC10K--------------------
double GetTemp(int ch) {
  ADCdata = analogRead(ch);
  //Serial.println(ADCdata);
  convertVolt = ADCdata * 3.3 / 1023;
  //Serial.println(convertVolt);
  Rt = 10000.0 * (double)ADCdata / (1023.0 - (double)ADCdata);
  //Serial.println(Rt);
  R_R25 = Rt / 10000;
  //Serial.println(R_R25);
  ln_R = log(R_R25);
  //Serial.println(ln_R);
  T = A1_Param + B1_Param * ln_R + C1_Param * (pow(ln_R, 2)) + D1_Param * (pow(ln_R, 3));
  T = T / 10000000;
  T = 1 / T;
  //Serial.println(T);
  Temperature_Val = T - 273.15;
  //Temp=Temperature_Val;
  //GetT=true;
  return Temperature_Val;
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


/*BLEDevice central = BLE.central();

  if ( central )
  {
  Serial.print( "Connected to central: " );
  Serial.println( central.address() );

  while ( central.connected() )
  {
    long currentMillis = millis();
    if ( fileNameCharacteristic.written() )
    {
      fileName = fileNameCharacteristic.value();
      Serial.print( fileName+"\r\n" );
    }
    if (currentMillis - previousMillis >= 500) {
      previousMillis = currentMillis;
      fileNameCharacteristic.writeValue(fileName);
    }

  }

  Serial.print( F( "Disconnected from central: " ) );
  Serial.println( central.address() );
  }*/
