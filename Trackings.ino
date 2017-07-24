#include <servo.h>
char tiltChannel=1, panChannel=0;

Servo servoTilt, servoPan;

char serialChar= 0;
int inValue = 0;

void setup() {
  servoTilt.attach(3);   //모터를 초기화하여 중앙보게하는 부분
  servoPan.attach(2);
  servoTilt.write(125);
  servoPan.write(65);

  Serial.begin(57600);

}

void loop() {
  while(Serial.available() & lt;=0 );  // 채널값을 읽고 모터값을 변수에 저장
  serialChar = Serial.read();
  if(serialChar == tiltChannel){
    while(Serial.available()&lt;=0);
    inValue = Serial.read();
    servoTilt.write(inValue);
  }

  else if (serialChar == panChannel){   // 채널값에따라 모터를 조종
    while(Serial.available()&lt;=0);
    inValue = Serial.read();
    servoPan.write(inValue);
  
  }


}
