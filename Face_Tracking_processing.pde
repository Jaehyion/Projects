import processing.video.*;     
import java.awt.*;
import hypermedia.video.*;
import processing.serial.*;                       //헤더파일들 입니다.
 
Capture video;
OpenCV cam;
 
int w = 800;
int h = 600;                               //영상이 모니터에 보일때의 해상도를 정해주는 부분입니다. 
 
int xstart = 0; 
int ystart = 0; 
Serial port;

char servoTiltPosition = 125;               //모터값의 초기화 값입니다. 
char servoPanPosition = 65;                //정면을 보았을 때의 모터값이 다음과 같습니다. 

char tiltChannel = 1;                      //가로와 세로로 회전하는 모터값들인데 동시에 두 모터값을 
char panChannel = 0;                       //보내주어야 하기 때문에 채널값으로 구분하여 보내주게 됩니다. 

int midFaceY=0;
int midFaceX=0;

int midScreenY = (h/2);                     //화면의 중앙좌표값을 의미하게 됩니다. 
int midScreenX = (w/2);                       //나중에 얼굴의 좌표값과 비교하여 모터값에 영향을 주게 됩니다. 
int midScreenWindow = 10;               
 
boolean firstTime = true;                 //아두이노와 원활한 소통을 위해 처음 3초를 기다려 주기 위한 세팅입니다.
void setup()
{
  size(w,h);                                 //화면의 크기를 정해주는 부분입니다. 
  video = new Capture(this, width, height);   //웹캠을 통해 들어오는 영상값을 변수에 저장하는 부분입니다. 
  video.start();                             
  loadPixels();
  cam = new OpenCV(this);
  cam.allocate(video.width, video.height);
  cam.copy(video);
  cam.cascade(OpenCV.CASCADE_FRONTALFACE_ALT);    //받은 영상값을 Open CV에 넘겨주는 부분입니다. 
  println(Serial.list()); 

  port = new Serial(this, Serial.list()[1], 57600);   //아두이노와 통신하기 위한 세팅해주는 부분입니다. 

  port.write(tiltChannel);  
  port.write(servoTiltPosition);  
  port.write(panChannel);      
  port.write(servoPanPosition);                //아두이노로 채널값과 모터값을 보냄으로써 초기화하는 부분입니다. 
}
 
void draw()
{
  if (firstTime) {                          //참으로 설정해 주었던 firstTime이기 때문에 
    delay(3000);                              //3초를 기다리게 되며
    firstTime = false;                       //값을 false로 바꿔줘 다음번에는 실행하지 않게 합니다. 
  }
  
  if(video.available())                  //영상값을 읽어들이는 부분입니다. 
  {
    video.read();  
    video.loadPixels();
    cam.copy(video);
    background(0);
    cam.read();
  }
 
  loadPixels();
  int vloc = 0;
  color c = color(0, 0, 0);
  for(int i = 0; i<width;i++)
  {
    for(int j = 0; j<height;j++)
    {
      int loc = i + j*width;
     
      c = 0;
     
      if(i<(video.width+xstart-1) && i>=xstart && j<(video.height+ystart-1)&&j>=ystart)
      {
        vloc = i-xstart + (j-ystart)*video.width;
        float r = red(video.pixels[vloc]);
        float g = green(video.pixels[vloc]);
        float b = blue(video.pixels[vloc]);
     
        c = color(r,g,b);
      }
     
      pixels[loc] = c;
    }  
 
  }

  updatePixels();
 
  Rectangle[] faces = cam.detect(1.2, 2, OpenCV.HAAR_DO_CANNY_PRUNING, 40,40);
  noFill();
  stroke(0,255,255);

  for(int i = 0; i 0){
    midFaceY = faces[0].y + (faces[0].height/2);                     //얼굴의 중앙좌표값을 계산하는 부분입니다. 
    midFaceX = faces[0].x + (faces[0].width/2);
  
    float Kx=0.02, Ky=0.02;                                 //상수를 이용해서 얼굴과 중앙의 좌표차이에 비례하게
    
    if(servoTiltPosition >= 80 || servoTiltPosition <= 170)  //모터값을 정해주는 부분입니다. 
      servoTiltPosition += (char)( Ky * (midFaceY - midScreenY) );
    
    if(servoPanPosition >= 15 && servoPanPosition <= 135)
      servoPanPosition += (char)( Kx * (midFaceX -  midScreenX) ); 
      
    else if(servoPanPosition < 15 || servoPanPosition > 135)
      servoPanPosition = (char)65;                         //모터가 너무 많이 돌아가게 되며 정 중앙으로 오게하는 
  }                                                        //부분입니다. 
  
  port.write(tiltChannel);      
  port.write(servoTiltPosition);
  port.write(panChannel);                             //아두이노로 계산한 모터값을 보내주는 부분입니다. 
  port.write(servoPanPosition);  
  delay(1);
}