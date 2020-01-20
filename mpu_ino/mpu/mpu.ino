#include <Wire.h>

const int addr = 0x68; //mpu i2c address
float accAlpha, accBeta, accRho;
float gyroAlpha, gyroBeta, gyroRho;
float accErrAlpha, accErrBeta, gyroErrAlpha, gyroErrBeta, gyroErrRho;
float gyroAngleX,gyroAngleY,accAngleX,accAngleY;
float yaw,pitch,roll;
float passTime, currTime, prevTime;
int err = 0;
int Xt,Yt;
int Xa=0,Ya=0;
float theta,sudut;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin(); //init
  Wire.beginTransmission(addr); //start MPU
  Wire.write(0x6B); 
  Wire.write(0x00);//reset
  Wire.endTransmission(true);
  error();
  delay(100);
  Serial.println("Masukkan tujuan: ");
  Serial.println();
  Xt=1;
  Yt=1;
}

void loop() {
  
  // put your main code here, to run repeatedly:
      if(Serial.available()){
      String inString = "";
      String strArr[5];
      while (Serial.available()) {
        delay(2);
        char ch = Serial.read();
        inString+= ch;
      }
      int stringStart = 0;
      int arrayIndex = 0;
      for (int i=0; i < inString.length(); i++){
      
        if(inString.charAt(i) == ' ' ||inString.charAt(i) == '\n' ){
          strArr[arrayIndex] = "";
          strArr[arrayIndex] = inString.substring(stringStart, i);
          stringStart = (i+1);
          arrayIndex++;
          
        }
      }
      
      String value1 = strArr[0];
      String value2 = strArr[1];
      
      Xt = value1.toInt();
      Yt = value2.toInt();
      
        
    }
      float    z = readraw();
      sudut = tujuan(z,Xt,Yt);
      Serial.print("Alpha : ");
      Serial.print(z);
      Serial.print("|sudut : ");
      Serial.print("|X,Y : "+String(Xt)+","+String(Yt)+" | ");
      Serial.println(sudut);
}

float readraw(){
  Wire.beginTransmission(addr);
  Wire.write(0x3B); //address acc
  Wire.endTransmission(false);
  Wire.requestFrom(addr,6,true); //read 6 register-stored 2 register
  accAlpha=(Wire.read()<<8|Wire.read())/16384.0;
  accBeta=(Wire.read()<<8|Wire.read())/16384.0;
  accRho=(Wire.read()<<8|Wire.read())/16384.0;

  accAngleX = (atan(accBeta/sqrt(pow(accAlpha,2)+pow(accRho,2)))*180/3.14159)-accErrAlpha;
  accAngleY = (atan(-1*accAlpha/sqrt(pow(accBeta,2)+pow(accRho,2)))*180/3.14159)-accErrAlpha;
  
  Wire.beginTransmission(addr);
  Wire.write(0x43); //address gyro
  Wire.endTransmission(false);
  Wire.requestFrom(addr,6,true); //read 4 register-stored 2 register
  gyroAlpha=Wire.read()<<8|Wire.read();
  gyroBeta=Wire.read()<<8|Wire.read();
  gyroRho=Wire.read()<<8|Wire.read();

  gyroAlpha=(gyroAlpha/131.0)-gyroErrAlpha;
  gyroBeta=(gyroBeta/131.0)-gyroErrBeta;
  gyroRho=(gyroRho/131.0)-gyroErrRho;

  prevTime = currTime;
  currTime = millis();
  passTime = (currTime-prevTime)/1000;

  gyroAngleX=gyroAngleX+gyroAlpha*passTime;
  gyroAngleY=gyroAngleY+gyroBeta*passTime;
  yaw=yaw+gyroRho*passTime;
  roll = (0.96 * gyroAngleX)+(0.04*accAngleX);
  pitch = (0.96 * gyroAngleY)+(0.04*accAngleY);
  return yaw;
  
}

void error(){
  while (err<200){
    Wire.beginTransmission(addr);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(addr,6,true);
    accAlpha=(Wire.read()<<8|Wire.read())/16384.0;
    accBeta=(Wire.read()<<8|Wire.read())/16384.0;
    accRho=(Wire.read()<<8|Wire.read())/16384.0;
  
    accErrAlpha += ((atan(accBeta/sqrt(pow(accAlpha,2)+pow(accRho,2)))*180/3.14159));
    accErrBeta += ((atan(-1*accAlpha/sqrt(pow(accBeta,2)+pow(accRho,2)))*180/3.14159));

    err++;
  }
  accErrAlpha = accErrAlpha / 200;
  accErrBeta = accErrBeta / 200;
  err=0;

  while(err<200){
    Wire.beginTransmission(addr);
    Wire.write(0x43); //address gyro
    Wire.endTransmission(false);
    Wire.requestFrom(addr,6,true); //read 4 register-stored 2 register
    gyroAlpha=Wire.read()<<8|Wire.read();
    gyroBeta=Wire.read()<<8|Wire.read();
    gyroRho=Wire.read()<<8|Wire.read();
  
    gyroErrAlpha =gyroErrAlpha+(gyroAlpha/131.0);
    gyroErrBeta =gyroErrBeta+(gyroBeta/131.0);
    gyroErrRho =gyroErrRho+(gyroRho/131.0);
    
    err++;
  }
    gyroErrAlpha = gyroErrAlpha / 200;
    gyroErrRho = gyroErrBeta / 200;
    gyroErrRho = gyroErrRho / 200;

}

float tujuan (float alpha,int Xt,int Yt){

    theta=(atan2((Yt-Ya),(Xt-Xa)))*180/3.14159;
    sudut=theta-alpha;

    return sudut;
    
}
