/*
 Arduino Linetracer + Auto by UltraSensor
 */
#include <Servo.h>
#include <SoftwareSerial.h>

#define ENA 6 // EN1, EN2 왼쪽 바퀴 
#define EN1 7 // EN3, EN4 오른쪽 바퀴 
#define EN2 3 // ENA, ENB 속도 제어
#define ENB 5
#define EN3 4
#define EN4 2
#define Trig 9 // 초음파센서
#define Echo 10 // 초음파센서

int distance;
Servo myservo;

int Speed_PWM = 180; //속도 제어
int sensorValue1; // 왼쪽 센서
int sensorValue2; // 가운데 센서
int sensorValue3; // 오른쪽 센서

void setup() {
  myservo.attach(8); // 서보모터 핀
  myservo.write(90);
  pinMode(EN1, OUTPUT); // 방향 핀
  pinMode(EN2, OUTPUT); // 방향 핀
  //pinMode(ENA, OUTPUT); // 속도 조절
  pinMode(EN3, OUTPUT); // 방향 핀
  pinMode(EN4, OUTPUT); // 방향 핀
  pinMode(Trig,OUTPUT); // 초음파센서
  pinMode(Echo,INPUT); // 초음파센서
  //pinMode(ENB, OUTPUT); // 속도 조절
  Serial.begin(9600);
}

void insick() { // 라인 센싱 함수
  sensorValue1 = digitalRead(A0); // 라인 센싱 왼쪽
  sensorValue2 = digitalRead(A1); // 라인 센싱 가운데
  sensorValue3 = digitalRead(A2); // 라인 센싱 오른쪽
  Serial.println("sensorValue1: "+String(sensorValue1)+"  sensorValue2:"+String(sensorValue2)+"  sensorValue3:"+String(sensorValue3));
  delay(50); 
} 

int Ultra(){ // 초음파센서 거리측정
  long cm;
    digitalWrite(Trig,HIGH);
    delayMicroseconds(5);                                                                              
    digitalWrite(Trig,LOW);
    delayMicroseconds(15);
    digitalWrite(Trig,HIGH);
    cm=pulseIn(Echo,HIGH)/29/2; // 거리변환 필요함(x/29/2)
    if(cm<4||cm>1000) cm = 300; // 튀는값 필터링(더 좋은 방법이 있길 바람)
    Serial.println(cm);
   return cm;
}

void loop(){ //장애물이 없을 때는 라인을 타고, 장애물 발견시 초음파모드로 장애물이 없는 곳을 향함
  distance = Ultra();
  if (distance < 20){ // 장애물 발견시
  STOP();
  myservo.write(150); // 왼쪽
  delay(300);
  long distance1 = Ultra();
  myservo.write(90); // 가운데
  delay(300);
  long distance2 = Ultra();
  myservo.write(30); // 오른쪽
  delay(300);
  long distance3 = Ultra();
  myservo.write(90); // 서보모터 위치 초기화
    delay(300);
    distance = max(distance1, distance2); 
    long check = max(distance, distance3); // 가장 먼거리를 선택
    if (check == distance1 ) //왼쪽
    {
      LEFT();
      Serial.println("LEFT!");
      delay(200);
    } else if (check == distance3) //오른쪽
    {
      RIGHT();
      Serial.println("RIGHT!");
      delay(200);
    } else {
      GO();
      delay(200);
    }
  }


  
  insick(); // 라인 인식
  if(sensorValue1==0 && sensorValue2==0 && sensorValue3==0  || sensorValue1==0 && sensorValue2==1 && sensorValue3==0) // 라인이 없거나 중앙에 위치할 때
  { 
    GO_1();
   delay(40);
  }

 else if(sensorValue1==1 && sensorValue2==0 && sensorValue3==0) // 왼쪽 센서에 라인이 감지되었을 때
  {
    LEFT();
   delay(40);
  }
else if(sensorValue1==0 && sensorValue2==0 && sensorValue3==1) // 오른쪽 센서에 라인이 감지되었을 때
  {
    RIGHT();
   delay(40); 
 }
else 
  {
  GO_1();
  delay(40);
  }
}

// 아래는 모터제어 함수들

void GO() {
  digitalWrite(EN1, LOW);
  digitalWrite(EN2, HIGH);
  digitalWrite(EN3, HIGH);
  digitalWrite(EN4, LOW);
  analogWrite(ENA, Speed_PWM); 
  analogWrite(ENB, Speed_PWM); 
}

void GO_1() {
  digitalWrite(EN1, HIGH);
  digitalWrite(EN2, LOW);
  digitalWrite(EN3, HIGH);
  digitalWrite(EN4, LOW);
  analogWrite(ENA, 87); 
  analogWrite(ENB, 87); 
}

void BACK() {
  digitalWrite(EN1, LOW);
  digitalWrite(EN2, HIGH);
  digitalWrite(EN3, LOW);
  digitalWrite(EN4, HIGH);
  analogWrite(ENA, Speed_PWM); 
  analogWrite(ENB, Speed_PWM); 
}

void LEFT() {
  digitalWrite(EN1, LOW);
  digitalWrite(EN2, HIGH);
  digitalWrite(EN3, HIGH);
  digitalWrite(EN4, LOW);
  analogWrite(ENA, Speed_PWM); 
  analogWrite(ENB, Speed_PWM); 
}

void RIGHT() {
  digitalWrite(EN1, HIGH);
  digitalWrite(EN2, LOW);
  digitalWrite(EN3, LOW);
  digitalWrite(EN4, HIGH);
  analogWrite(ENA, Speed_PWM); 
  analogWrite(ENB, Speed_PWM); 
}

void STOP() {  
  analogWrite(ENA, 0); 
  analogWrite(ENB, 0);
}


