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

//double A1_Param= 335.4016,B1_Param=25.6524,C1_Param= 0.260597,D1_Param= 0.00632926;
//double A1_Param= 0.00335401643468053,B1_Param=0.00025698501802,C1_Param=0.0000026201306709,D1_Param=0.000000063830907998;
//double A1_Param= 3354016434680530,B1_Param=256985018020000,C1_Param=2620130670900,D1_Param=63830907998; //*1000000000000000000
//double A1_Param=33540.16,B1_Param=2565.24,C1_Param=26.0597,D1_Param=0.632926;//A1,B1 = * 10000000
long long int A1_Param = 3354016434680530, B1_Param = 256985018020000, C1_Param = 2620130670900, D1_Param = 63830907998; //*1000000000000000000
double Temp;
static unsigned int c = 0;

void init_array(){
    for (uint8_t count = 0; count < numReadings; count++) {
    readings[count] = 0;
  }
  
}

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
