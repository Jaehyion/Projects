#include <SoftwareSerial.h> //시리얼 통신 라이브러리 호출
 
//모터 PIN 설정
#define LEFT_A1 4 //왼쪽 바퀴
#define LEFT_B1 5 //왼쪽 바퀴
#define RIGHT_A2 6 //오른쪽 바퀴
#define RIGHT_B2 7 //오른쪽 바퀴
 
//초음파 센서 PIN 설정
#define IR_TRIG 9 //TRIG 핀 설정 (초음파 보내는 핀)
#define IR_ECHO 8 //ECHO 핀 설정 (반사된 초음파 받는 핀)
 
void setup() {
  Serial.begin(9600);   //시리얼모니터 
 
  //모터 핀모드 설정
  pinMode(LEFT_A1, OUTPUT);
  pinMode(RIGHT_A2, OUTPUT);
  pinMode(LEFT_B1, OUTPUT);
  pinMode(RIGHT_B2, OUTPUT);  
 
  //초음파 센서 핀모드 설정
  pinMode(IR_TRIG, OUTPUT);
  pinMode(IR_ECHO, INPUT);
}
 
void loop() {
  float duration, distance;
  digitalWrite(IR_TRIG, HIGH);
  delay(10);
  digitalWrite(IR_TRIG, LOW);
 
  // echoPin 이 HIGH를 유지한 시간
  duration = pulseIn(IR_ECHO, HIGH);
  // HIGH 였을 때 시간(초음파가 보냈다가 다시 들어온 시간)을 가지고 거리를 계산 한다.
  // 340은 초당 초음파(소리)의 속도, 10000은 밀리세컨드를 세컨드로, 왕복거리이므로 2로 나눠주면 거리가 cm 로 나옴
  distance = ((float)(340 * duration) / 10000) / 2;
  Serial.print("\nDIstance : ");
  Serial.println(distance);
     
  if(distance < 20) {         //장애물 감지 (20cm 이내)
     Serial.println("stop");
     stop();  //정지(1초)
     Serial.println("backward");
     backward();  //후진(0.5초)
     int rnd = random(0,2);   //장애물 감지시 좌/우회전 랜덤처리
     if(rnd == 0){
        Serial.println("right");
        right();
     }else{
        Serial.println("left");
        left();
     }
   }else{         //장애물 없음
     Serial.println("forward");
     forward();
   }
}
 
void forward(){
  digitalWrite(LEFT_A1, HIGH);
  digitalWrite(LEFT_B1, LOW);
  digitalWrite(RIGHT_A2, HIGH);
  digitalWrite(RIGHT_B2, LOW);
}
void backward(){
  digitalWrite(LEFT_A1, LOW);
  digitalWrite(LEFT_B1, HIGH);
  digitalWrite(RIGHT_A2, LOW);
  digitalWrite(RIGHT_B2, HIGH);
  delay(500);
}
void left(){
  digitalWrite(LEFT_A1, LOW);
  digitalWrite(LEFT_B1, HIGH);
  digitalWrite(RIGHT_A2, HIGH);
  digitalWrite(RIGHT_B2, LOW);
  delay(1000);
}
void right(){
  digitalWrite(LEFT_A1, HIGH);
  digitalWrite(LEFT_B1, LOW);
  digitalWrite(RIGHT_A2, LOW);
  digitalWrite(RIGHT_B2, HIGH);
  delay(1000);
}
void stop(){
  digitalWrite(LEFT_A1, LOW);
  digitalWrite(LEFT_B1, LOW);
  digitalWrite(RIGHT_A2, LOW);
  digitalWrite(RIGHT_B2, LOW);
  delay(1000);
}
