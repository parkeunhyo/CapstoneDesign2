#include <PulseSensorPlayground.h>

// Constants
const int PulseWire = A0;       // Pulse Sensor Signal 연결된 핀
const int LED13 = 13;           // 내장 LED 연결된 핀
const int Threshold = 550;      // 임계값 (센서 값의 기준)

// Variables
PulseSensorPlayground pulseSensor;

void setup() {
  // Serial Communication 시작
  Serial.begin(9600);

  // PulseSensor 설정
  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED13);       // 심박이 감지되면 내장 LED 깜빡임
  pulseSensor.setThreshold(Threshold);

  // PulseSensor 시작
  if (pulseSensor.begin()) {
    Serial.println("Pulse Sensor 시작");
  }
}

void loop() {
  int myBPM = pulseSensor.getBeatsPerMinute();  // 심박수 측정

  // 심박수가 측정되면 시리얼 모니터에 출력
  if (pulseSensor.sawStartOfBeat()) {
    Serial.println();

    Serial.print("♥ BPM: ");
    Serial.println(myBPM);
  }

  // Delay for a bit to avoid serial monitor flooding
  delay(20);
}
