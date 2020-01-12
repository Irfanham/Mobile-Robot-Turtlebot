#include <Wire.h>

const int addr = 0x68; //mpu i2c address
float accAlpha, accBeta, accRho;
float gyroAlpha, gyroBeta, gyroRho;
float accErrAlpha, accErrBeta, gyroErrAlpha, gyroErrBeta, gyroErrRho;
int c = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin(); //init
  mpu_register();
  //error();
  delay(100);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  mpu_read();

  Serial.print("accAlpha=");
  Serial.print(accAlpha);
  Serial.print(" | accBeta");
  Serial.print(accBeta);
  Serial.print(" | accRho=");
  Serial.print(accRho);
  Serial.print(" | gyroAlpha=");
  Serial.print(gyroAlpha);
  Serial.print(" | gyroBeta");
  Serial.print(gyroBeta);
  Serial.print(" | gyroRho=");
  Serial.print(gyroRho);
  Serial.println();
  

}

void mpu_register(){
  Wire.beginTransmission(addr); //start MPU
  Wire.write(0x6B); 
  Wire.write(0x00);//reset
  Wire.endTransmission();

  //configure acc(+/-2g)
  Wire.beginTransmission(addr);
  Wire.write(0x3B); //address acc
  Wire.endTransmission();
 // Wire.requestFrom(addr,6,true); //read 6 register-stored 2 register

  //configure gyro(+/-250 deg/s)
  Wire.beginTransmission(addr);
  Wire.write(0x43); //address gyro
  Wire.endTransmission();
 // Wire.requestFrom(addr,6,true); //read 4 register-stored 2 register
}

void mpu_read(){
  Wire.beginTransmission(addr);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(addr,14);
  
  while(Wire.available()<14);
  accAlpha=Wire.read()<<8|Wire.read();
  accBeta=Wire.read()<<8|Wire.read();
  accRho=Wire.read()<<8|Wire.read();
 
  temp = Wire.read()<<8|Wire.read();
  
  gyroAlpha=Wire.read()<<8|Wire.read();
  gyroBeta=Wire.read()<<8|Wire.read();
  gyroRho=Wire.read()<<8|Wire.read();
}
