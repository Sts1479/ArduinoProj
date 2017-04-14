

#ifndef gyro_accel.h
  #define gyro_accel.h
  
  extern int accel_x_OC, accel_y_OC, accel_z_OC, gyro_x_OC ,gyro_y_OC, gyro_z_OC; // offset variables
  //extern float temp_scalled,accel_x_scalled,accel_y_scalled,accel_z_scalled,gyro_x_scalled,gyro_y_scalled,gyro_z_scalled; 
  extern int temp_scalled,accel_x_scalled,accel_y_scalled,accel_z_scalled,gyro_x_scalled,gyro_y_scalled,gyro_z_scalled;//Scaled Data variables
 // extern float Q_angle  =  0.01, Q_gyro   =  0.0003, R_angle  =  0.01, x_bias = 0, P_00 = 0, P_01 = 0, P_10 = 0, P_11 = 0, y, S, K_0, K_1, K_angle;
  
  void MPU6050_ReadData();
  void MPU6050_ResetWake();
  void MPU6050_SetDLPF(int BW);
  void MPU6050_SetGains(int gyro,int accel);
  void MPU6050_OffsetCal();
  void MPU6050_GyroCal();
  
  float kalmanCalculate(float newAngle, float newRate,int looptime);
  float Complementary2(float newAngle, float newRate,int looptime);
 
#endif
