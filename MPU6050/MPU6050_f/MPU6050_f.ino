#include <Wire.h>
#include <MPU6050.h>

MPU6050 sensor;

// 낙상 감지에 사용할 임계값
const int ACC_THRESHOLD = 16000; // 가속도 임계값
const int GYRO_THRESHOLD = 300;  // 자이로 임계값
const int TIME_THRESHOLD = 10;  // 시간 임계값 (milliseconds)

// 낙상 감지에 사용할 변수
unsigned long fallTime = 0;
bool potentialFall = false;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  sensor.initialize();
  if (!sensor.testConnection()) {
    Serial.println("MPU6050 connection failed");
  } else {
    Serial.println("MPU6050 connection successful");
  }
}

void loop() {
  int16_t ax, ay, az, gx, gy, gz;
  sensor.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  int totalAcc = sqrt(ax * ax + ay * ay + az * az); // 총 가속도 계산
  int totalGyro = sqrt(gx * gx + gy * gy + gz * gz); // 총 자이로 계산

  // 데이터 출력
  Serial.print("Acc: ");
  Serial.print(totalAcc);
  Serial.print(" Gyro: ");
  Serial.println(totalGyro);

  // 가속도 또는 자이로 데이터가 임계값을 초과하는 경우
  if (totalAcc > ACC_THRESHOLD || totalGyro > GYRO_THRESHOLD) {
    if (!potentialFall) { // 최초 감지 시
      potentialFall = true;
      fallTime = millis();
    } else if (millis() - fallTime > TIME_THRESHOLD) { // 일정 시간 이상 지속되면
      Serial.println("Fall Detected!");
      potentialFall = false; // 낙상 감지 후 초기화
    }
  } else if (potentialFall) { // 임계값 미만이고, 낙상 가능성이 있었다면
    if (millis() - fallTime < TIME_THRESHOLD) {
      potentialFall = false; // 짧은 시간에 정상화된 경우, 낙상 아님으로 판단
    }
  }

  delay(50);
}
