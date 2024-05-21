#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DS1302.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int CLK_PIN = 5;   // DS1302의 클록 핀
const int DATA_PIN = 6;  // DS1302의 데이터 핀
const int RST_PIN = 7;   // DS1302의 리셋 핀

DS1302 rtc(RST_PIN, DATA_PIN, CLK_PIN);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // OLED의 I2C 주소는 0x3C
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);

  rtc.halt(false);  // RTC의 시간을 멈추지 않도록 설정
  rtc.writeProtect(false);  // 쓰기 보호 비활성화

  Serial.begin(9600);  // 시리얼 통신 시작
  Serial.println("DS1302 RTC Test");
  Serial.println("---------------");

  // 현재 시간 설정: 13시 25분 0초
  rtc.setTime(13, 25, 0);
}

void loop() {
  display.clearDisplay();
  display.setCursor(0, 0);  // 표시할 위치를 (0, 0)으로 설정
  display.println(rtc.getTimeStr());  // DS1302에서 시간을 가져와 출력
  display.display();  // 화면에 내용을 표시
  delay(1000);  // 다음 업데이트까지 1초 대기
}
