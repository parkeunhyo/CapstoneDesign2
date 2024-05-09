#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DS1302.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// 디스플레이 초기화 (리셋 핀이 없음을 의미하는 -1)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);  

const int CLK = 5;   // DS1302의 클록 핀
const int DAT = 6;   // DS1302의 데이터 핀
const int RST = 7;   // DS1302의 리셋 핀

DS1302 rtc(RST, DAT, CLK);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // OLED의 I2C 주소는 0x3C
  display.clearDisplay();
  display.setTextSize(1);  // 텍스트 크기 설정
  display.setTextColor(WHITE);  // 텍스트 색 설정

  rtc.halt(false);  // RTC의 시간을 멈추지 않도록 설정
  rtc.writeProtect(false);  // 쓰기 보호 비활성화
  
  Serial.begin(115200);  // 시리얼 통신 시작
  Serial.println("DS1302 RTC Test");
  Serial.println("---------------");

  // 현재 시간 설정 예시: 13시 25분 0초
  rtc.setTime(13, 25, 0);  // 시, 분, 초 설정
}

void loop() {
  display.clearDisplay();  // 디스플레이 화면 초기화
  display.setCursor(0, 10);  // 커서 위치 설정
  
  // 현재 시간 표시
  display.println(rtc.getTimeStr());
  
  display.display();  // 변경사항을 화면에 표시
  delay(1000);  // 1초마다 화면 갱신
}
