void init_Serial() {
  Serial.begin(115200);
}

void Serial_Event() {
  if (Serial.available()) {
    Serial.flush();
    String command = "";
    String cTemp = "";
    int cTemp_cnt = 0;
    while (Serial.available())
    {
      char c;
      c = Serial.read();
      cTemp += c;
      cTemp_cnt++;
    }
    int len = cTemp_cnt;
    int Deli = cTemp.indexOf("#");
    first = cTemp.indexOf(",", Deli);
    command = cTemp.substring(0, first);
    if (command == "#CNT" && len == 10) {
      Split_Oper(cTemp);
    }
  }  
}
void Split_Oper(String fileName) {
  int RGB_Index = fileName.indexOf("\\", first + 1);
  RGB_Val = fileName.substring(first + 1, RGB_Index);
  if (RGB_Val == "R") {
    ON_LED(255, 0, 0);
    LED_Flag = true;
  }
  else if (RGB_Val == "G") {
    ON_LED(0, 255, 0);
    LED_Flag = true;
  }
  else if (RGB_Val == "B") {
    ON_LED(0, 0, 255);
    LED_Flag = true;
  }
  else if (RGB_Val == "O") {
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
