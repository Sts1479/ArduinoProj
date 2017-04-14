
#include <Arduino.h>
#include <Wire.h>
#include "gyro_accel.h"


// Defining the important registers

#define MPU6050_address 0x68

// ------ SELF Test Trim Factors ----------------------
#define MPU6050_self_test_x 13       // R/W
#define MPU6050_self_test_y 14       // R/W
#define MPU6050_self_test_z 15       // R/W
#define MPU6050_self_test_A 16       // R/W 

// ----- Sample Divider -------------------------------
#define MPU6050_sample_div 25        // R/W
/*       Sample Divider Discription
  Sample rate = 8/(1 + Sample rate divider) [kHz]  if DLPF is disabled
  Sample rate = 1/(1 + Sample rate divider) [kHz]  if DLPF is enabled
*/

// ----- Configration ---------------------------------
#define MPU6050_config 26            // R/W
#define MPU6050_gyro_config 27       // R/W
#define MPU6050_accel_config 28      // R/W

// ----- Data ---------------------------------------------
#define MPU6050_data_start 59



// ----- Power Management --------------------------------- 
#define MPU6050_PWR1 107
#define MPU6050_PWR2 108

// ----- Defining Constant --------------------------------
#define g 9.81                       // Gravitational accelration




// Variables 
int temp=0, accel_x=0, accel_y=0, accel_z=0, gyro_x=0, gyro_y=0, gyro_z=0; // Raw values varaibles
int accel_x_OC=0, accel_y_OC=0, accel_z_OC=0, gyro_x_OC=0 ,gyro_y_OC=0, gyro_z_OC=0; // offset variables
//float temp_scalled,accel_x_scalled,accel_y_scalled,accel_z_scalled,gyro_x_scalled,gyro_y_scalled,gyro_z_scalled; //Scalled Data varaibles
int temp_scalled,accel_x_scalled,accel_y_scalled,accel_z_scalled,gyro_x_scalled,gyro_y_scalled,gyro_z_scalled; //Scalled Data varaibles
float accel_scale_fact = 1, gyro_scale_fact = 1; // Scale factor variables
float Q_angle=0.01, Q_gyro=0.0003, R_angle  =  0.01, x_bias = 0, P_00 = 0, P_01 = 0, P_10 = 0, P_11 = 0, y, S, K_0, K_1, K_angle;
float c_angle2C,x1,y1,x2;

// **************************************************************************
//          Functions for MPU6050
// **************************************************************************


void MPU6050_ReadData(){
  
  
  Wire.beginTransmission(MPU6050_address);
  Wire.write(MPU6050_data_start);
  Wire.endTransmission();

  int read_bytes = 14;

  Wire.requestFrom(MPU6050_address,read_bytes);

  if(Wire.available() == read_bytes){
  
    accel_x = Wire.read()<<8 | Wire.read();
    accel_y = Wire.read()<<8 | Wire.read();
    accel_z = Wire.read()<<8 | Wire.read();
  
    temp = Wire.read()<<8 | Wire.read();
  
    gyro_x = Wire.read()<<8 | Wire.read();
    gyro_y = Wire.read()<<8 | Wire.read();
    gyro_z = Wire.read()<<8 | Wire.read();

  
  }
  
  
  
  accel_x_scalled = (int)(accel_x-accel_x_OC); 
  accel_y_scalled = (int)(accel_y-accel_y_OC);
  accel_z_scalled = (int)(accel_z-accel_z_OC);
  
  gyro_x_scalled = (int)(gyro_x-gyro_x_OC);
  gyro_y_scalled = (int)(gyro_y-gyro_y_OC);
  gyro_z_scalled = (int)(gyro_z-gyro_z_OC);
  
  /*
  accel_x_scalled = (float)(accel_x-accel_x_OC)*accel_scale_fact/1000; // divided by 1000 as the Scale factor is in milli units
  accel_y_scalled = (float)(accel_y-accel_y_OC)*accel_scale_fact/1000;
  accel_z_scalled = (float)(accel_z-accel_z_OC)*accel_scale_fact/1000;
  
  gyro_x_scalled = (float)(gyro_x-gyro_x_OC)*gyro_scale_fact/1000;
  gyro_y_scalled = (float)(gyro_y-gyro_y_OC)*gyro_scale_fact/1000;
  gyro_z_scalled = ((float)(gyro_z-gyro_z_OC)*gyro_scale_fact/1000);
  //temp_scalled = (float)temp/340+36.53;
  */
}



// ------------------------------------------------
//       Reset and wake up the MPU6050
// ------------------------------------------------
void MPU6050_ResetWake(){
  
 // Serial.println("Resetting MPU6050 and waking it up.....");
  Wire.beginTransmission(MPU6050_address);
  Wire.write(MPU6050_PWR1);
  Wire.write(0b10000000);
  Wire.endTransmission();
  
  delay(100); // Waiting for the reset to complete
   
  Wire.beginTransmission(MPU6050_address);
  Wire.write(MPU6050_PWR1);
 
  Wire.write(0b00000000);
  Wire.endTransmission();
  
}



// ------------------------------------------------
//       Setting up the DLFP
// ------------------------------------------------

void MPU6050_SetDLPF(int BW)
  {
    if (BW < 0 || BW > 6){
      BW = 0;
    }
  Wire.beginTransmission(MPU6050_address);
  Wire.write(MPU6050_config); // Address to the configuration register
/*       config Discription ---- x x 0 0 0 F2 F1 F0 
  I am only intrested in the Digital Low Pass Filter (DLPF)
  F2 F1 F0    Bandwidth [Hz]
  0  0  0        
  0  0  1      184
  0  1  0      94
  0  1  1      44
  1  0  0      21  
  1  0  1      10  
  1  1  0      5
*/

  Wire.write(BW);
  Wire.endTransmission();
  }
  
// ------------------------------------------------
//       Setting up the Accelrometer and Gyro Gains
// ------------------------------------------------  

void MPU6050_SetGains(int gyro,int accel)
  {
    byte gyro_byte,accel_byte;
    
    // Setting up Gyro
    Wire.beginTransmission(MPU6050_address);
    Wire.write(MPU6050_gyro_config); // Address to the configuration register
    if (gyro==0)
    {
      gyro_scale_fact =(float)250*0.0305; // each data is of 16 bits that means, 250 is divided along 2^(15)-1 = 32767 so for milli degree/s 0.0305 = 1000/32767
      gyro_byte = 0b00000000;
    }else if (gyro == 1)
    {
      gyro_scale_fact = 500*0.0305; // each data is of 16 bits that means, 500 is divided along 2^(15)-1 = 32767 so for milli degree/s 0.0305 = 1000/32767
      gyro_byte = 0b00001000;
    }else if (gyro == 2)
    {
      gyro_scale_fact = 1000*0.0305;// each data is of 16 bits that means, 1000 is divided along 2^(15)-1 = 32767 so for milli degree/s 0.0305 = 1000/32767
      gyro_byte = 0b00010000;
    }else if (gyro == 3)
    {
      gyro_scale_fact = 2000*0.0305;  // each data is of 16 bits that means, 2000 is divided along 2^(15)-1 = 32767 so for milli degree/s 0.0305 = 1000/32767
      gyro_byte = 0b00011000;
    }
    {
      gyro_scale_fact = 1;
    }  
      
    Wire.write(gyro_byte);
    Wire.endTransmission();
    //Serial.print("The gyro scale is set to ");
    //Serial.print(gyro_scale_fact);
    //Serial.println(" milli Degree/s");
    
    
    // Setting up Accel
    Wire.beginTransmission(MPU6050_address);
    Wire.write(MPU6050_accel_config); // Address to the configuration register
    if (accel==0)
    {
      accel_scale_fact =(float)2*g*0.0305; // each data is of 16 bits that means, 2g is divided along 2^(15)-1 = 32767 so for milli m/s^2 0.0305 = 1000/32767       
      accel_byte = 0b00000000;
    }else if (accel == 1)
    {
      accel_scale_fact = 4*g*0.0305; // each data is of 16 bits that means, 4g is divided along 2^(15)-1 = 32767 so for milli m/s^2 0.0305 = 1000/32767
      accel_byte = 0b00001000;
    }else if (accel == 2)
    {
      accel_scale_fact = 8*g*0.0305;// each data is of 16 bits that means, 8g is divided along 2^(15)-1 = 32767 so for milli m/s^2 0.0305 = 1000/32767
      accel_byte = 0b00010000;
    }else if (accel == 3)
    {
      accel_scale_fact = 16*g*0.0305; // each data is of 16 bits that means, 16g is divided along 2^(15)-1 = 32767 so for milli m/s^2 0.0305 = 1000/32767
      accel_byte = 0b00011000;
    }else
    {
      accel_scale_fact = 1;
    }  
      
    Wire.write(accel_byte);
    Wire.endTransmission();
   // Serial.print("The accel scale is set to ");
   // Serial.print(accel_scale_fact);
    //Serial.println(" milli m/s^2");
    
  }
  
  
// ------------------------------------------------
//       offset calibration
// ------------------------------------------------  

void MPU6050_OffsetCal(){
 // Serial.println("Calibrating gyroscope .... dont move the hardware ..........");
  
  int x=0,y=0,z=0,i;
  int gyro_x1=0, gyro_y1=0, gyro_z1=0; // the first dementions gyro
  //int gyro_x0=0, gyro_y0=0, gyro_z0=0;
  
  MPU6050_ReadData();
  //delay(100);
  MPU6050_ReadData();
  
  // Gyro Offset Calculation
  //x=gyro_x;
  //y=gyro_y;
  //z=gyro_z;
  //gyro_x1=gyro_x;
  //gyro_y1=gyro_y;
  //gyro_z1=gyro_z;
    
  for (i=1;i<=1000;i++){
    MPU6050_ReadData();
    // demph  the signal
    //gyro_x=gyro_x/4+gyro_x1-gyro_x1/4;
    //gyro_y=gyro_y/4+gyro_y1-gyro_y1/4;
    //gyro_z=gyro_z/4+gyro_z1-gyro_z1/4;
    x=(x+gyro_x)/2;
    y=(y+gyro_y)/2;
    z=(z+gyro_z)/2;
   // Serial.print(".");
  }
 // Serial.println(".");
  gyro_x_OC=x;
  gyro_y_OC=y;
  gyro_z_OC=z;
  
  //Serial.print("gyro_x register offset = ");
 // Serial.println(x);
 
  
  //Serial.print("gyro_y register offect = ");
  //Serial.println(y);
 
  
 // Serial.print("gyro_z register offset = ");
  //Serial.println(z);
 
  
  
  // Accel Offset Calculation
  //Serial.println("Calibrating accelrometer .... dont move the hardware ..........");
  //x=accel_x;
  //y=accel_y;
  //z=accel_z;
  x=0;y=0;z=0;  
  for (i=1;i<=1000;i++){
    MPU6050_ReadData();
    x=(x+accel_x)/2;
    y=(y+accel_y)/2;
    z=(z+accel_z)/2;
    //Serial.print(".");
  }
  //Serial.println(".");
  accel_x_OC=x;
  accel_y_OC=y;
  accel_z_OC=z-(float)g*1000/accel_scale_fact;
  
  //Serial.print("Accel_x register offset = ");
  //Serial.println(x);
  
  
  //Serial.print("Accel_y register offect = ");
  //Serial.println(y);
  
  
  //Serial.print("Accel_z register offset = ");
  //Serial.println(z);
  
  
  

}
float kalmanCalculate(float newAngle, float newRate,int looptime)
{
float dt = float(looptime)/1000;
K_angle += dt * (newRate - x_bias);
P_00 +=  - dt * (P_10 + P_01) + Q_angle * dt;
P_01 +=  - dt * P_11;
P_10 +=  - dt * P_11;
P_11 +=  + Q_gyro * dt;

y = newAngle - K_angle;
S = P_00 + R_angle;
K_0 = P_00 / S;
K_1 = P_10 / S;

K_angle +=  K_0 * y;
x_bias  +=  K_1 * y;
P_00 -= K_0 * P_00;
P_01 -= K_0 * P_01;
P_10 -= K_1 * P_00;
P_11 -= K_1 * P_01;

return K_angle;
}

float Complementary2(float newAngle, float newRate,int looptime) {
float st=10;
float dtc2=float(looptime)/1000.0;

x1 = (newAngle - c_angle2C)*st*st;
y1 = dtc2*x1 + y1;
x2 = y1 + (newAngle - c_angle2C)*2*st + newRate*2;
c_angle2C = dtc2*x2 + c_angle2C;

return c_angle2C;
}


// function to calibrate the coordinate frame 

void MPU6050_GyroCal(){
  
  int x=0,y=0,z=0,i;
  int gyro_x1=0, gyro_y1=0, gyro_z1=0; // the first dementions gyro
  
  MPU6050_ReadData();
  //delay(100);
  MPU6050_ReadData();
   
  for (i=1;i<=1000;i++){
    MPU6050_ReadData();
    x=(x+gyro_x)/2;
    y=(y+gyro_y)/2;
    z=(z+gyro_z)/2;
  }
  gyro_x_OC=x;
  gyro_y_OC=y;
  gyro_z_OC=z;
 }

