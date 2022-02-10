void init_Serial() {
  Serial.begin(115200);
}

void Serial_Event() {
  if (Serial.available()) {
    Serial.flush();
    String command = "";
    String cTemp = "";
    int cTemp_cnt = 0;
    while (true)
    {
      char c;
      c = Serial.read();      
      cTemp += c;
      cTemp_cnt++;
      delay(1);
      if(cTemp[0]== '#' && c == 'n'){
        break;
      }
    
    }
    int len = cTemp_cnt;
    int Deli = cTemp.indexOf("#");
    first = cTemp.indexOf(",", Deli);
    command = cTemp.substring(0, first);
    if (command == "#CNT" && (10<=len && 16>=len)) {
      Split_Oper(cTemp,len);
    }
  }  
}
void Split_Oper(String fileName,int len) {
  if(len == 10){
    int RGB_Index = fileName.indexOf("\\", first + 1);
    RGB_Val = fileName.substring(first + 1, RGB_Index);
    Delay_Val_Int = 17;
  }
  else if(len>10&&len<=14){
  int RGB_Index = fileName.indexOf(",", first + 1);
  int Delay_Index = fileName.indexOf("\\",RGB_Index+1);
  RGB_Val = fileName.substring(first + 1, RGB_Index);
  Delay_Val_String = fileName.substring(RGB_Index+1,Delay_Index);
  Delay_Val_Int = Delay_Val_String.toInt();
  if(Delay_Val_Int==0){
    Delay_Val_Int = 17;
  }
  }
  if (RGB_Val == "R" && LED_Flag==false) {
    ON_LED(255, 0, 0);
    LED_Flag = true;
  }
  else if (RGB_Val == "G"&& LED_Flag==false) {
    ON_LED(0, 255, 0);
    LED_Flag = true;
  }
  else if (RGB_Val == "B"&& LED_Flag==false) {
    ON_LED(0, 0, 255);
    LED_Flag = true;
  }
  else if (RGB_Val == "O"&& LED_Flag==false) {
    ON_LED(255, 40, 0);
    LED_Flag = true;
  }
  else if (RGB_Val=="W"){ 
    ON_LED(255,255,255);
  }
  else if(RGB_Val == "F"){
    ON_LED(0,0,0);
  }

}
