#include <SoftwareSerial.h>                    // SoftwareSerial 헥사파일 선언

​

SoftwareSerial GPS (13 , 12);                 // GPS 시리얼 통신 핀설정 (TX , RX)

​

void setup() 

{

  Serial.begin(9600);                                // 시리얼통신 설정 (보드레이트 9600)

  GPS.begin(9600);                                  // GPS 시리얼통신 설정 (보드레이트 9600)

}

​

void loop() 

{

  if(Serial.available())                               // Serial에 입력이 되면

  {

    Serial.write(Serial.read());                   // Serial에 입력된 값을 시리얼 모니터에 출력

  }

  if(GPS.available())                                 // GPS에 입력이 되면

  {

    Serial.write(GPS.read());                     // GPS에 입력된 값을 시리얼 모니터에 출력

  }

}
[출처] 아두이노 NEO-6M GPS 모듈 사용 예제|작성자 도매키트 아두몰
