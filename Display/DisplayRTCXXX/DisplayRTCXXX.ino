#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DS1302.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(-1);  // 리셋 핀이 없음

const int CLK = 5;   // 클록 핀
const int DAT = 6;   // 데이터 핀
const int RST = 7;   // 리셋 핀
const int SWITCH_PIN = 2;  // 스위치 핀

DS1302 rtc(RST, DAT, CLK);

void setup() 
{
  pinMode(SWITCH_PIN, INPUT);  // 스위치 핀을 입력으로 설정
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  myrtc.halt(false);
  myrtc.writeProtect(false);
  
  Serial.begin(9600);
  myrtc.setDOW(WEDNESDAY);
  myrtc.setTime(17, 5, 0);
  myrtc.setDate(8, 5, 24);
}

void loop() 
{
  if (digitalRead(SWITCH_PIN) == HIGH) {  // 스위치가 눌렸는지 확인
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);

    display.print(myrtc.getDOWStr());
    display.print(" ");
    display.print(myrtc.getDateStr());
    display.print(" -- ");
    display.println(myrtc.getTimeStr());

    display.display();
  } else {
    display.clearDisplay();
    display.display();
  }
  
  delay(100);  // 짧은 지연
}
