//extern double T[3][3];
#ifndef fcal.h
  #define fcal.h
void filter1();
void MPU6050_Calib();
void MPU6050_Calib1();
//void MPU6050_Gyrocal();
void mat_multipl(double Matrix[3][3], double Vector[3]);
void mat_multipl1(double Matrix[3][3], double Vector[3]);
int sign(double x);
extern struct M{
    double Tq[3][3];
    double TVec[3];
}matrix;

#endif
