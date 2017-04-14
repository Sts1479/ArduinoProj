#include <Wire.h>
#include "gyro_accel.h"
#include "fcal.h"
// Defining constants
#define dt 3                       // time difference in milli seconds/��������� �������� � ��
#define rad2degree 57.3              // Radian to degree conversion/��������� �������� � ����
#define Filter_gain 0.7             // e.g.  angle = angle_gyro*Filter_gain + angle_accel*(1-Filter_gain)/ �����. �����-���� (����������������) �������
// *********************************************************************
//    Global Variables
// *********************************************************************
unsigned long t=0; // Time Variables
float angle_x_gyro=0,angle_y_gyro=0,angle_z_gyro=0,angle_x_accel=0,angle_y_accel=0,angle_z_accel=0,angle_x=0,angle_y=0,angle_z=0;
float angle_x_gyro_k=0,angle_y_gyro_k=0,angle_z_gyro_k=0,angle_x_k=0,angle_y_k=0,angle_z_k=0;
//float angle_x_accel_p[2]={0,0};
//float angle_y_accel_p[2]={0,0};
//float angle_z_accel_p[2]={0,0};
float angle_x_accel_0,angle_y_accel_0;
int k=0,n=0;;
double accel_vec[3], gyro_vec[3];
// *********************************************************************
// Main Code
void setup(){
  Serial.begin(115200); //Speed of the COM-port/�������� �OM-�����
  Wire.begin();
  MPU6050_ResetWake(); //Reset of the MPU/ C���� MPU
  MPU6050_SetGains(1,1);// Setting the lows scale/��������� ����� ��� ��������� � ��������������
  MPU6050_SetDLPF(0); // Setting the DLPF to inf Bandwidth for calibration/��������� ��������� ��� ������� �� ��� ����������
  //MPU6050_ReadData();
  delay(100);
  //MPU6050_ReadData();
  //MPU6050_OffsetCal();
 MPU6050_GyroCal();
  filter1();
 MPU6050_Calib1();
 //MPU6050_OffsetCal();//Calibration of the MPU/���������� MPU
 //MPU6050_SetDLPF(3); // Setting the DLPF to lowest Bandwidth 
  
 
  /*
  Serial.print("gyro_x_scalled");
  Serial.print("\tgyro_y_scalled");
  Serial.print("\tgyro_z_scalled");
  
  Serial.print("\taccel_x_scalled");
  Serial.print("\taccel_y_scalled");
  Serial.print("\taccel_z_scalled");
  
  Serial.print("\tangle_x_gyro");
  Serial.print("\tangle_y_gyro");
  Serial.print("\tangle_z_gyro");
  
  Serial.print("\tangle_x_accel");
  Serial.print("\tangle_y_accel");
  Serial.print("\tangle_z_accel");
  
  Serial.print("\tangle_x");
  Serial.print("\tangle_y");
  Serial.print("\tangle_z");
  
  Serial.println("\tLoad");
  */
  
  //gyro_x_scalled=123456;  
  t=millis(); 
}
void loop(){
  t=millis(); 
  
  MPU6050_ReadData();
  

  Serial.print(gyro_x_scalled);
  Serial.print("\t");
  Serial.print(gyro_y_scalled);
  Serial.print("\t");
  Serial.print(gyro_z_scalled);
  Serial.print("\t");


  Serial.print(accel_x_scalled);
  Serial.print("\t");
  Serial.print(accel_y_scalled);
  Serial.print("\t");
  Serial.print(accel_z_scalled);
  Serial.print("\t");

 accel_vec[0]=accel_x_scalled;
 accel_vec[1]=accel_y_scalled;
 accel_vec[2]=accel_z_scalled;
 mat_multipl1(matrix.Tq,accel_vec);
 accel_vec[0]= matrix.TVec[0];
 accel_vec[1]= matrix.TVec[1];
 accel_vec[2]= matrix.TVec[2];
 matrix.TVec[0]=0;matrix.TVec[1]=0;matrix.TVec[2]=0;
 
  Serial.print(accel_vec[0]);
  Serial.print("\t");
  Serial.print(accel_vec[1]);
  Serial.print("\t");
  Serial.print(accel_vec[2]);
  Serial.print("\t");

 gyro_vec[0]=gyro_x_scalled;
 gyro_vec[1]=gyro_y_scalled;
 gyro_vec[2]=gyro_z_scalled;
 mat_multipl1(matrix.Tq,gyro_vec);
 gyro_vec[0]= matrix.TVec[0];
 gyro_vec[1]= matrix.TVec[1];
 gyro_vec[2]= matrix.TVec[2];
 matrix.TVec[0]=0;matrix.TVec[1]=0;matrix.TVec[2]=0;

  Serial.print(gyro_vec[0]);
  Serial.print("\t");
  Serial.print(gyro_vec[1]);
  Serial.print("\t");
  Serial.print(gyro_vec[2]);
  Serial.print("\t");
/*
  if (k<3) 
  {
   
  Serial.print(matrix.Tq[k][0]);
  Serial.print("\t");
  Serial.print(matrix.Tq[k][1]);
  Serial.print("\t");
  Serial.print(matrix.Tq[k][2]);
  Serial.print("\t");
  k=k+1;
  }
  else 
  {k=0;
   Serial.print(10000);
  Serial.print("\t");}
  */
  
 
  //Serial.println(j);
  Serial.println((float)(millis()-t));
  
  while((millis()-t) < dt){ // Making sure the cycle time is equal to dt
  
  }
 

}

