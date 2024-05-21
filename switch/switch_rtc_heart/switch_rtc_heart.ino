#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DS1302.h>
#include <PulseSensorPlayground.h>

// OLED 디스플레이 설정
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// RTC 설정
const int CLK = 5;  // RTC 모듈의 CLK 핀
const int DAT = 6;  // RTC 모듈의 DAT 핀
const int RST = 7;  // RTC 모듈의 RST 핀
DS1302 myrtc(RST, DAT, CLK);

// 심박수 센서 설정
const int PulseWire = A0;       // 심박수 센서의 아날로그 핀 번호
const int LED13 = 13;           // 내장 LED 핀
const int Threshold = 550;      // 심박수 임계값
PulseSensorPlayground pulseSensor;

// 스위치 설정
const int buttonPin = 2;        // 스위치가 연결된 디지털 핀 번호
int buttonState = 0;            // 현재 스위치 상태
int lastButtonState = 0;        // 마지막 스위치 상태
unsigned long lastDebounceTime = 0;  // 마지막 디바운스 체크 시간
unsigned long debounceDelay = 50;    // 디바운스 시간 지연
int buttonPressCount = 0;       // 스위치 눌림 횟수
unsigned long lastButtonPressTime = 0; // 마지막 눌림 시간
unsigned long doublePressInterval = 500; // 더블 눌림 간격 (밀리초)

void setup() {
  Serial.begin(9600);

  // OLED 디스플레이 초기화
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // 0x3C는 OLED 디스플레이의 I2C 주소
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // 무한루프
  }
  display.clearDisplay();  // 디스플레이 초기화
  display.setTextSize(2);  // 텍스트 크기 설정
  display.setTextColor(WHITE);  // 텍스트 색상 설정

  // RTC 모듈 초기화
  myrtc.halt(false);  // RTC 모듈 실행
  myrtc.writeProtect(false);  // 쓰기 보호 해제
  // myrtc.setTime(13, 25, 0);  // 필요한 경우 시간 설정 (주석 해제 시 활성화)

  // 심박수 센서 설정
  pulseSensor.analogInput(PulseWire);  // 심박수 센서의 아날로그 입력 설정
  pulseSensor.blinkOnPulse(LED13);  // 심박수가 감지되면 LED 깜빡임
  pulseSensor.setThreshold(Threshold);  // 심박수 임계값 설정
  if (pulseSensor.begin()) {  // 심박수 센서 시작
    Serial.println("Pulse Sensor 시작");
  }

  // 스위치 핀 설정
  pinMode(buttonPin, INPUT_PULLUP);  // 스위치 핀을 입력으로 설정하고 내부 풀업 저항 활성화

  Serial.println("DS1302RTC Test");
  Serial.println("---------------");
}

void loop() {
  // 스위치 상태 읽기
  int reading = digitalRead(buttonPin);

  // 디바운스 처리
  if (reading != lastButtonState) {
    lastDebounceTime = millis();  // 마지막 디바운스 체크 시간 업데이트
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {  // 디바운스 시간 지연 후
    if (reading != buttonState) {  // 스위치 상태 변경 감지
      buttonState = reading;
      if (buttonState == LOW) {  // 스위치가 눌린 경우
        unsigned long currentTime = millis();
        if (currentTime - lastButtonPressTime <= doublePressInterval) {  // 두 번 눌림 간격 내에 눌림 감지
          buttonPressCount++;
        } else {
          buttonPressCount = 1;
        }
        lastButtonPressTime = currentTime;

        Serial.print("Button pressed ");
        Serial.print(buttonPressCount);
        Serial.println(" times.");

//        if (buttonPressCount == 1) {  // 한 번 눌렀을 때
//          displayTime();
//        } 
          if (buttonPressCount == 2) {  // 두 번 눌렀을 때
            // 두 번 눌림을 감지하면 카운트 리셋
          displayHeartRate();
          buttonPressCount = 0;
        }
      }
    }
  }

  lastButtonState = reading;  // 마지막 스위치 상태 업데이트

  delay(50);  // 스위치 상태 체크 주기
}

void displayTime() {
  Serial.println("Displaying time.");
  display.clearDisplay();  // 디스플레이 초기화
  display.setCursor(15, 8);  // 커서 위치 설정
  display.setTextSize(2);  // 텍스트 크기 설정
  display.print(myrtc.getTimeStr());  // 현재 시간 출력
  display.display();  // 디스플레이에 표시
  delay(3000);  // 3초 동안 디스플레이 유지
  display.clearDisplay();  // 디스플레이 초기화
  display.display();  // 빈 화면 표시
}

void displayHeartRate() {
  Serial.println("Displaying heart rate.");
  
  // 심박수를 읽기 전에 일정 시간 동안 심박수 측정을 실행
  unsigned long startTime = millis();
  while (millis() - startTime < 2000) {  // 2초 동안 심박수 측정
    pulseSensor.getBeatsPerMinute();  // 심박수 갱신
  }

  int myBPM = pulseSensor.getBeatsPerMinute();  // 심박수 측정
  Serial.print("Measured BPM: ");
  Serial.println(myBPM);

  display.clearDisplay();  // 디스플레이 초기화
  display.setCursor(15, 20);  // 커서 위치 설정
  display.setTextSize(2);  // 텍스트 크기 설정
  display.print("BPM : ");  // "BPM : " 출력
  display.print(myBPM);  // 심박수 출력
  display.display();  // 디스플레이에 표시
  delay(3000);  // 3초 동안 디스플레이 유지
  display.clearDisplay();  // 디스플레이 초기화
  display.display();  // 빈 화면 표시
}
