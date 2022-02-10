#include "ntc.h"

int ADCdata;
float convertVolt;
double Rt,R_R25,ln_R;
double T;
float Temperature_Val;
  //double A1_Param= 335.4016,B1_Param=25.6524,C1_Param= 0.260597,D1_Param= 0.00632926;
  //double A1_Param= 0.003354016,B1_Param=0.000256524,C1_Param=0.00000260597,D1_Param=0.0000000632926;
double A1_Param=33540.16,B1_Param=2565.24,C1_Param=26.0597,D1_Param=0.632926;//A1,B1 = * 10000000
double GetTemp(int ch){

  ADCdata = analogRead(ch);

  convertVolt = ADCdata*5.0/1023;

  Rt = 10000.0*(double)ADCdata/(1023.0-(double)ADCdata);

  R_R25 = Rt/10000;

  ln_R = log(R_R25);

  T= A1_Param+B1_Param*ln_R+C1_Param*(pow(ln_R,2))+D1_Param*(pow(ln_R,3));
  T=T/10000000;
  T=1/T;

  Temperature_Val=T-273.15;

  return Temperature_Val;
}
