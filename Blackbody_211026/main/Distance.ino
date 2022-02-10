#include <DFRobot_TFmini.h>
uint16_t distance, strength;
DFRobot_TFmini TFmini;
SoftwareSerial Lidar(8, 7);

uint16_t prev_dist;
bool connect_Flag=false;
void init_Lidar(){
  TFmini.begin(Lidar); 
  connect_Flag=true;
}

void Get_distance() {  
  if(TFmini.measure()){
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
  else{
    dist = -1;        
  }

}
