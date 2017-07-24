/*
OpenCV를 이용한 색상 추적
 */
// opencv 라이브러리를 import:
import hypermedia.video.*;
OpenCV opencv; // opencv 인스턴스
int[] pixelArray; // 픽셀을 저장할 배열
color trackColor; // 찾고자 하는 색상
void setup() {
  // 윈도 초기화
  size( 640, 480 );
  // opencv 초기화
  opencv = new OpenCV( this );
  opencv.capture( width, height );
  // 찾을 색은 빨간색으로 초기화
  trackColor = color(255, 0, 0);
  // 가장자리를 부드럽게 그리도록 설정
  smooth();
}
void draw() {
  float closestMatch = 500; // 가까운 색상이 맞았음을 나타내는 값
  float colorThreshold = 10; // 색상 유사성 허용 범위
  int closestX = 0; // 가까운 색상의 수평 위치
  int closestY = 0; // 가까운 색상의 수직 위치
  // 카메라를 읽음
  opencv.read();
  // 카메라 이미지를 윈도에 그림
  image(opencv.image(), 0, 0);
  // 카메라 픽셀 배열을 복사
  pixelArray = opencv.pixels();
  // 모든 픽셀을 검사하는 루프 시작
  for (int x = 0; x < opencv.width; x++ ) {
    for (int y = 0; y < opencv.height; y++ ) {
      // 영상의 폭과 높이를 이용해서 배열에서 픽셀의 위치를 계산
      int loc = x + y*opencv.width;
      // 현재 픽셀의 색을 구한다.
      color currentColor = pixelArray[loc];
      float r1 = red(currentColor);
      float g1 = green(currentColor);
      float b1 = blue(currentColor);
      float r2 = red(trackColor);
      float g2 = green(trackColor);
      float b2 = blue(trackColor);
      // 현재 픽셀의 집합적 색상을 계산할 때 dist() 함수를 사용
      // 이 함수는 현재 픽셀의 적색, 녹색, 청색을 3차원 공간의 좌표로 가정
      // 현재 픽셀과 목적 색상의 유클리드 거리를 계산.
      // 아래 수식에서 가장 거리가 가까운 것이 색상의 유사성이 높다는 것을 의미
      float d = dist(r1, g1, b1, r2, g2, b2);
      // 만일 현재 색상이 이전에 추적했던 가장 비슷한 색상보다 비슷할시 현재 위치와 차이를 저장한다.
      if (d < closestMatch) {
        closestMatch = d;
        closestX = x;
        closestY = y;
      }
    }
  }
  // 찾아낸 색상의 거리가 색상의 허용 범위보다 작은 경우 고려
  // 색상의 정확도를 높이려면 허용 값을 작게, 색상 비교의 일치도를 낮추려면 이 값을 높게
  if (closestMatch < colorThreshold) {
    // 찾아낸 픽셀에 원을 그림
    fill(trackColor);
    strokeWeight(2.0);
    stroke(0);
    ellipse(closestX, closestY, 16, 16);
  }
}
void mousePressed() {
  // trackColor 변수에 마우스가 클릭된 위치의 색상을 저장
  int loc = mouseX + mouseY*opencv.width;
  trackColor = pixelArray[loc];
}
