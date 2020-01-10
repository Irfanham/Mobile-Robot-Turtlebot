#include <math.h>

//encoder kiri
#define encoderKiriPinA 2
#define encoderKiriPinB 5

//encoder kanan
#define encoderKananPinA 3
#define encoderKananPinB 6

#define D 7.00 // diameter
#define L 21.2 // jarak antar roda
#define phi 3.14159
#define b 3

int ENA=10;    // enableDriverA(PWM)   
int ENB=9;    // enableDriverB  
// motor kiri
int IN1=12;    
int IN2=8;    
 //motor kanan
int IN3=4;    
int IN4=7;

//encoder

float pulsePerRev=50.0;// jumlah pulse dalam satu revolusi
float kelRoda=phi*D;
double degPerPulse = 360/pulsePerRev;
double jumlahCount=0.0; //total

volatile float encoderKiriCount =0.0;
volatile float encoderKananCount =0.0;
long lastCountKiri =0;
long lastCountKanan =0;

float bedaKanan,bedaKiri;
float x=0.0,y=0.0,theta=phi;
float errorTheta=0.0;
float gx=0.0,gy=0.0,gTheta=0.0;
float velocity=0.2,w=0.0,VR=0.0,VL=0.0;
float rSpeed=0.0,lSpeed=0.0;
float dR=0.0,dL=0.0,dist=0.0;
float kalpha = 8;
float kbeta = -1.5;
float kro = 3;
int a=0;
float point[][b];



void setup() {
  // put your setup code here, to run once:


     pinMode(ENA,OUTPUT);
     pinMode(ENB,OUTPUT);
     pinMode(IN1,OUTPUT);
     pinMode(IN2,OUTPUT);
     pinMode(IN3,OUTPUT);
     pinMode(IN4,OUTPUT);
      
     digitalWrite(ENA,HIGH);    
     digitalWrite(ENB,HIGH);    
     pinMode(encoderKiriPinA, INPUT); 
     pinMode(encoderKiriPinB, INPUT);
     pinMode(encoderKananPinA, INPUT); 
     pinMode(encoderKananPinB, INPUT);  
     
    // encoder pin on interrupt 0 (pin 2)
    attachInterrupt(0, countEncoderKiri, CHANGE);  
     
    // encoder pin on interrupt 1 (pin 3)
    attachInterrupt(1, countEncoderKanan, CHANGE);  
      Serial.begin (115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(((point[a][0]-x)>0.1)||(point[a][1]-y)0.1)
    adjustPWM(10,10);

    gTheta=atan2(point[a][1]-y,point[a][0]-x);
    Serial.print("theta goal");
    Serial.println(gTheta);

    adjustPID();
    diff();

    
    
}

void actualPosition(){
  dR=(encoderKananCount/50)*(kelRoda);
  dL=(encoderKiriCount/50)*(kelRoda);
  dist=(dR+dL)/2;
  x=x+(dist*cos(theta));
  y=y+(dist*sin(theta));
  theta = atan2(sin(theta),cos(theta));//mapping [-pi,pi]
}

float diff(){
  VR=(2*velocity+w*L)/D;
  VL=(2*velocity-w*L)/D;
}

//PID

float adjustPID(){

    errorTheta=gTheta-theta;
    errorTheta=atan2(sin(errorTheta),cos(errorTheta));
    
    w=kP*errorTheta;
    return w;
}


// left encoder counts

void countEncoderKiri(){
     if(digitalRead(encoderKiriPinB==HIGH))encoderKiriCount++;
    else encoderKiriCount--;
}\
//  right encoder counts

void countEncoderKanan(){
    if(digitalRead(encoderKananPinB==HIGH))encoderKananCount--;
    else encoderKananCount++;
    
}

void resetEnc(){
  encoder
}

// fungsi motor
void adjustPWM(float speedKanan,float speedKiri){
 
  //kanan
  if (speedKanan > 0.0) {
    //maju
    int RPWM=255;
    majuKanan(RPWM);
    
  }  else if (speedKanan < 0.0){
    //mundur
    int RPWM=255;
    mundurKanan(RPWM);
  }

  //kiri
  if (speedKiri > 0.0) {
    //maju
    int LPWM=255;
    majuKiri(LPWM);
    
  }  else if (speedKiri < 0.0){
    //mundur
    int LPWM=255;
    mundurKiri(LPWM);
  }
  
  if (speedKanan == 0.0 && speedKiri==0.0) {
   int RPWM=0;
   int LPWM=0;
   kananStop(RPWM);
   kiriStop(LPWM);

  }
}

void kananMaju(int speedKanan){
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(ENB, speedKanan);
}

void kiriMaju (int speedKiri){
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(ENA, speedKiri);
}

void kananMundur(int speedKanan){
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  analogWrite(ENB, speedKanan);

}

void kiriMundur(int speedKiri){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(ENA, speedKiri);
}

void kananStop(int speedKanan){
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  analogWrite(ENB, speedKanan);
}

void kiriStop(int speedKiri){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  analogWrite(ENA, speedKiri);
}
