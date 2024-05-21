#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PulseSensorPlayground.h>

// OLED 디스플레이 설정
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // 무한루프
  }

  // 심박수 센서 설정
  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED13);
  pulseSensor.setThreshold(Threshold);

  // 심박수 센서 시작
  if (pulseSensor.begin()) {
    Serial.println("Pulse Sensor 시작");
  }

  // 스위치 핀 설정
  pinMode(buttonPin, INPUT_PULLUP);  // 내부 풀업 저항 활성화

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
}

void loop() {
  // 스위치 상태 읽기
  int reading = digitalRead(buttonPin);

  // 디바운스 처리
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        unsigned long currentTime = millis();
        if (currentTime - lastButtonPressTime <= doublePressInterval) {
          buttonPressCount++;
        } else {
          buttonPressCount = 1;
        }
        lastButtonPressTime = currentTime;

        if (buttonPressCount == 2) {
          buttonPressCount = 0;  // 두 번 눌림을 감지하면 카운트 리셋
          displayHeartRate();
        }
      }
    }
  }

  lastButtonState = reading;

  delay(50);  // 스위치 상태 체크 주기
}

void displayHeartRate() {
  int myBPM = pulseSensor.getBeatsPerMinute();  // 심박수 측정

  display.clearDisplay();

  // 심박수 출력
  display.setCursor(15, 20);
  display.setTextSize(2);  // 글자 크기를 키움
  display.print("BPM : ");
  display.print(myBPM);

  display.display();
}
