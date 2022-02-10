void init_Serial() {
  Serial.begin(115200);
  Serial.setTimeout(500);
  
}
bool Serial_flag = false;
void Serial_Event() {
  Serial.flush();
  if (Serial.available()) {
    String command = "";
    String cTemp = "";
    int cTemp_cnt = 0;
    int loop_cnt = 0;
    //while (Serial.available())
    while(true)
    {
      loop_cnt++;
      char c;
      if(Serial.available()){
      cTemp = Serial.readStringUntil('n');  
      
      cTemp_cnt = cTemp.length();
      //Serial.println(cTemp + "    "+ String(cTemp_cnt));
      //cTemp += c;
      //cTemp_cnt++;      
      }
      
      //_delay_ms(10);
      delay(2);
      //delayMicroseconds(200);
      if(cTemp[0]== '#' && cTemp[cTemp_cnt-1] == '\\'){
        Serial_flag = true;
        Serial.println("String_buffer : " + cTemp + ",  Buffer_cnt : " + (String)loop_cnt);
        break;
      }
      if(loop_cnt>=500){
        loop_cnt=0;
        Serial_flag =false;
        break;    
      }
    }
    if(Serial_flag == true){
    int len = cTemp_cnt;
    int Deli = cTemp.indexOf("#");
    first = cTemp.indexOf(",", Deli);
    command = cTemp.substring(0, first);
    if (command == "#CNT" && (10<=len && 16>=len)) {
      Split_Oper(cTemp,len);
    }
    Serial_flag=false;
  }  
  }
}
void Split_Oper(String fileName,int len) {
  if(len == 10){
    int RGB_Index = fileName.indexOf("\\", first + 1);
    RGB_Val = fileName.substring(first + 1, RGB_Index);
    Delay_Val_Int = 1700;
  }
  else if(len>10&&len<=14){
  int RGB_Index = fileName.indexOf(",", first + 1);
  int Delay_Index = fileName.indexOf("\\",RGB_Index+1);
  RGB_Val = fileName.substring(first + 1, RGB_Index);
  Delay_Val_String = fileName.substring(RGB_Index+1,Delay_Index);
  Delay_Val_Int = Delay_Val_String.toInt();
  Delay_Val_Int = Delay_Val_Int *100;
  if(Delay_Val_Int==0){
    Delay_Val_Int = 1700;
  }
  }
  if (RGB_Val == "R" && LED_Flag==false) {
    Serial.end();
    ON_LED(255, 0, 0);
    delay(Delay_Val_Int);
    ON_LED(255,255,255);
    Serial.begin(115200);
  }
  else if (RGB_Val == "G"&& LED_Flag==false) {
    Serial.end();
    ON_LED(0, 255, 0);
    delay(Delay_Val_Int);
    ON_LED(255,255,255);
    Serial.begin(115200);
  }
  else if (RGB_Val == "B"&& LED_Flag==false) {
    Serial.end();
    ON_LED(0, 0, 255);
    delay(Delay_Val_Int);
    ON_LED(255,255,255);
    Serial.begin(115200);
  }
  else if (RGB_Val == "O"&& LED_Flag==false) {
    Serial.end();
    ON_LED(255, 40, 0);
    delay(Delay_Val_Int);
    ON_LED(255,255,255);
    Serial.begin(115200);
  }
  else if (RGB_Val=="W"){ 
    ON_LED(255,255,255);
  }
  else if(RGB_Val == "F"){
    ON_LED(0,0,0);
  }

}
