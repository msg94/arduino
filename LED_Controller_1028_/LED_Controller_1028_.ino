#include <Adafruit_NeoPixel.h>
//#ifdef __AVR__
//#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
//#endif


#define PIN        4//d2
#define NUMPIXELS 29 // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define PIN2       0//d3
#define NUMPIXELS2 10 // Popular NeoPixel ring size
Adafruit_NeoPixel pixels2(NUMPIXELS2, PIN2, NEO_GRB + NEO_KHZ800);

#define PIR        5//d1
#define TRIG       13//d5
#define ECHO       12//d6

unsigned long lastMsg = 0;
unsigned long lastOperation_time = 0;
unsigned long onoff_period = 2000;
int messageState = 0;
int PIRState = 0;
bool cmd_data = false;

void setup() {
//#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
//  clock_prescale_set(clock_div_1);
//#endif
  pixels.begin();
  pixels2.begin();
  delay(1000);
  pixels.clear();
  pixels2.clear();
  pixels.show();
  pixels2.show();
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(50, 50, 50));
  }
  for (int i = 0; i < NUMPIXELS2; i++) {
    pixels2.setPixelColor(i, pixels2.Color(50, 50, 50));
  }
  pixels.show();
  pixels2.show();
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(PIR, INPUT);
  Serial.begin(9600);
  Serial.println("Start");
}

void loop() {  //#AL,33.5,\r\n red  37.5>=  blue 32<=
  unsigned long now = millis();
  if ( cmd_data && (now - lastMsg > onoff_period)) {
    cmd_data = false;
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(50, 50, 50));
    }
    pixels.show();
  }
  if (Serial.available()) {
    delay(100);
    String cTemp = "";
    String sCommand = "";
    String data = "";
    int cTemp_cnt = 0;
    while (Serial.available())
    {
      char c;
      c = Serial.read();
      cTemp += c;
      cTemp_cnt++;
    }
    int first = cTemp.indexOf(",");
    int data_end = cTemp.indexOf("\r");
    int len = cTemp_cnt;
    sCommand = cTemp.substring(0, first);
    data = cTemp.substring(first + 1, data_end - 1);
    if (sCommand == "#AL") {
      cmd_data = true;
      lastMsg = now;
      if (data.toFloat() >= 37.5) {
        for (int i = 0; i < NUMPIXELS; i++) {
          pixels.setPixelColor(i, pixels.Color(250, 5, 5));
        }
        pixels.show();
      }
      else if (data.toFloat() <= 32) {
        for (int i = 0; i < NUMPIXELS; i++) {
          pixels.setPixelColor(i, pixels.Color(30, 30, 250));
        }
        pixels.show();
      }
      else {
        for (int i = 0; i < NUMPIXELS; i++) {
          pixels.setPixelColor(i, pixels.Color(30, 250, 30));
        }
        pixels.show();
      }
    }
    else delay(10);
  }

  PIRState = digitalRead(PIR);

  if (PIRState) {
    if (now - lastOperation_time >= 100) {
      lastOperation_time = now;
      long duration, distance;
      digitalWrite(TRIG, LOW);
      delayMicroseconds(2);
      digitalWrite(TRIG, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG, LOW);
      duration = pulseIn(ECHO, HIGH);
      distance = duration * 170 / 1000;
      String Msg = "#AL,";
      Msg += String(distance);
      Msg += ",\r\n";
      Serial.print(Msg);
    }
  }
}
