#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DS1302.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(-1);  // -1 은 리셋 핀이 없음을 의미

const int CLK = 5;
const int DAT = 6;
const int RST = 7;

DS1302 myrtc(RST, DAT, CLK);
void setup() 
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // DM30 I2C 주소는 일반적으로 0x3C
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);

  myrtc.halt(false);
  myrtc.writeProtect(false);
  
  Serial.begin(9600);
  Serial.println("DS1302RTC Test");
  Serial.println("---------------");

 // myrtc.setDOW(WEDNESDAY);
  myrtc.setTime(13, 25, 0);
  //myrtc.setDate(8, 5, 24);
}

void loop() 
{
  display.clearDisplay();
  display.setCursor(15,8);
  
  //display.print(myrtc.getDOWStr());
  //display.print(" ");
  //display.print(myrtc.getDateStr());
  //display.print(" -- ");
  display.println(myrtc.getTimeStr());
  
  display.display();  // 화면에 내용을 보여줍니다.
  
  delay(1000);  // 1초 간격으로 업데이트
}
