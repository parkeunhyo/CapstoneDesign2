#include <Wire.h>
#include <MPU6050.h>

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

void setup() {
  Wire.begin();
  Serial.begin(38400);
  
  Serial.println("Initialize MPU6050");
  while (!accelgyro.testConnection()) {
    Serial.println("MPU6050 connection failed");
    delay(500);
  }

  accelgyro.initialize();
  Serial.println("MPU6050 ready");
}

void loop() {
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  Serial.print("a/g: ");
  Serial.print(ax); Serial.print(" ");
  Serial.print(ay); Serial.print(" ");
  Serial.print(az); Serial.print(" ");
  Serial.print(gx); Serial.print(" ");
  Serial.print(gy); Serial.print(" ");
  Serial.println(gz);
  delay(100);
}
