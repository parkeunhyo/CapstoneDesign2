/*#include <SoftwareSerial.h>
SoftwareSerial gps(11,12);
char c = ""; 
String str = ""; 
String targetStr = "GPGGA" ;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  gps.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(gps.available()){
    c=gps.read(); 
    if(c=='\n'){
      if(targetStr.equals(str.substring(1,6))){
        Serial.println(str);
        int first = str.indexOf(",");
        int two = str.indexOf(",",first+1);
        int three = str.indexOf(",",two+1);
        int four = str.indexOf(",",three+1);
        int five = str.indexOf(",",four+1);
        String Lat = str.substring(two+1,three);
        String Long = str.substring(four+1,five);

        String Lat1 = Lat.substring(0,2);
        String Lat2 = Lat.substring(2);

        String Long1 = Long.substring(0,3);
        String Long2 = Long.substring(3);

        double LatF = Lat1.toDouble()+Lat2.toDouble()/60;
        float LongF = Long1.toFloat()+Long2.toFloat()/60;

        Serial.print("Lat(위도) :");
        Serial.println(LatF,15);
        Serial.print("Long(경도) :");
        Serial.println(LongF,15);

      }
    str = "";
    }
    else{
      str+=c;
    }
  }
}*/
#include <SoftwareSerial.h>

SoftwareSerial gps(11, 12); // RX, TX
String str = "";
String targetStr = "GPGGA";

void setup() {
  Serial.begin(9600);
  gps.begin(9600);
}

void loop() {
  if (gps.available()) {
    char c = gps.read();
    if (c == '\n') {
      Serial.println(str); // 수신한 전체 문장을 출력합니다.
      
      if (targetStr.equals(str.substring(1, 6))) {
        Serial.println("GPGGA 문장 발견"); // GPGGA 문장이 발견되었음을 알립니다.

        int firstComma = str.indexOf(",");
        int secondComma = str.indexOf(",", firstComma + 1);
        int thirdComma = str.indexOf(",", secondComma + 1);
        int fourthComma = str.indexOf(",", thirdComma + 1);
        int fifthComma = str.indexOf(",", fourthComma + 1);

        String latitude = str.substring(secondComma + 1, thirdComma);
        String longitude = str.substring(fourthComma + 1, fifthComma);

        // 유효한 데이터가 있는지 확인
        if (latitude.length() > 0 && longitude.length() > 0) {
          Serial.print("위도 문자열: ");
          Serial.println(latitude);
          Serial.print("경도 문자열: ");
          Serial.println(longitude);

          String latDegrees = latitude.substring(0, 2);
          String latMinutes = latitude.substring(2);

          String lonDegrees = longitude.substring(0, 3);
          String lonMinutes = longitude.substring(3);

          Serial.print("위도(도): ");
          Serial.println(latDegrees);
          Serial.print("위도(분): ");
          Serial.println(latMinutes);
          Serial.print("경도(도): ");
          Serial.println(lonDegrees);
          Serial.print("경도(분): ");
          Serial.println(lonMinutes);

          double latitudeDecimal = latDegrees.toDouble() + latMinutes.toDouble() / 60.0;
          double longitudeDecimal = lonDegrees.toDouble() + lonMinutes.toDouble() / 60.0;

          Serial.print("Lat(위도) : ");
          Serial.println(latitudeDecimal, 15);
          Serial.print("Long(경도) : ");
          Serial.println(longitudeDecimal, 15);
        } else {
          Serial.println("유효한 GPS 데이터를 수신하지 못했습니다.");
        }
      }
      str = "";
    } else {
      str += c;
    }
  }
}
