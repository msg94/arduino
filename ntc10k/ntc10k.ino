#include <math.h>

const int trig = 2;
const int echo = 4;
bool GetT = false;
double Temp;
static unsigned int c=1;

double GetTemp(int ch){
  int ADCdata;
  float convertVolt;
  double Rt,R_R25,ln_R;
  double T;
  float Temperature_Val;
  //double A1_Param= 335.4016,B1_Param=25.6524,C1_Param= 0.260597,D1_Param= 0.00632926;
  //double A1_Param= 0.003354016,B1_Param=0.000256524,C1_Param=0.00000260597,D1_Param=0.0000000632926;
  double A1_Param=33540.16,B1_Param=2565.24,C1_Param=26.0597,D1_Param=0.632926;//A1,B1 = * 10000000
  

  
  ADCdata = analogRead(ch);
  Serial.println(ADCdata);
  convertVolt = ADCdata*3.3/1023;
  Serial.println(convertVolt);
  Rt = 10000.0*(double)ADCdata/(1023.0-(double)ADCdata);
  //Serial.println(Rt);
  R_R25 = Rt/10000;
  //Serial.println(R_R25);
  ln_R = log(R_R25);
  //Serial.println(ln_R);
  T= A1_Param+B1_Param*ln_R+C1_Param*(pow(ln_R,2))+D1_Param*(pow(ln_R,3));
  T=T/10000000;
  T=1/T;
  Serial.println(T);
  Temperature_Val=T-273.15;
  //Temp=Temperature_Val;
  //GetT=true;
  return Temperature_Val;

}


void setup() {

  Serial.begin(115200);
  
}

void loop() {
  

  Temp = GetTemp(0);
  Serial.println(Temp);
  Serial.print(c/10);
  Serial.println(" Sec");
  delay(500); 
  
    

   
  //Temp = GetTemp(0);
  //Serial.println(Temp);
  //delay(500);


}
