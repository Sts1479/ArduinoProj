#include <math.h>
#include <Arduino.h>
//#include "fcal.h"
#include "gyro_accel.h"
#include "fcal.h"
M matrix;
int sign (double x)
{
    if (x>0) return 1;
    if (x==0) return 0;
    if (x<0) return -1;
}
void MPU6050_Calib()
{
    double vx,vy,vz;
    double norm,vxn,vyn,vzn;
    double c1[3];
    double quat[4];
    double q_sum0,q_sum1,q_sum2,q_sum3;
    vx=(accel_x_scalled);vy=(accel_y_scalled);vz=(accel_z_scalled);
    norm=sqrt(vx*vx+vy*vy+vz*vz);
    vxn=vx/norm;
    vyn=vy/norm;
    vzn=vz/norm;
    c1[0]=(vyn*sqrt(1-vxn*vxn-vzn*vzn)+vxn*vzn*sqrt(1-vyn*vyn-vzn*vzn))/(1-vzn*vzn);
    c1[1]=(vxn*sqrt(1-vyn*vyn-vzn*vzn)+vyn*vzn*sqrt(1-vxn*vxn-vzn*vzn))/(1-vzn*vzn);
    c1[2]=vzn;

    q_sum0=1+c1[0]+c1[1]+c1[2];
    q_sum1=1+c1[0]-c1[1]-c1[2];
    q_sum2=1-c1[0]+c1[1]-c1[2];
    q_sum3=1-c1[0]-c1[1]+c1[2];

    if (q_sum0<0) q_sum0=-q_sum0;
    if (q_sum1<0) q_sum1=-q_sum1;
    if (q_sum2<0) q_sum2=-q_sum2;
    if (q_sum3<0) q_sum3=-q_sum3;

    quat[0]=0.5*sqrt(q_sum0);
    quat[1]=0.5*sqrt(q_sum1);
    quat[2]=0.5*sqrt(q_sum2);
    quat[3]=0.5*sqrt(q_sum3);

    //signification
    if ((sign(vxn)==1)&&(sign(vyn)==1)&&(sign(vzn)==1)) quat[1]=-quat[1];

    if ((sign(vxn)==-1)&&(sign(vyn)==1)&&(sign(vzn)==1)) quat[1]=-quat[1];

    if ((sign(vxn)==1)&&(sign(vyn)==1)&&(sign(vzn)==-1)) quat[1]=-quat[1];

    if ((sign(vxn)==-1)&&(sign(vyn)==1)&&(sign(vzn)==-1)) quat[1]=-quat[1];

    if ((sign(vxn)==1)&&(sign(vyn)==-1)&&(sign(vzn)==1)) quat[3]=-quat[3];

    if ((sign(vxn)==-1)&&(sign(vyn)==-1)&&(sign(vzn)==1)) quat[3]=-quat[3];

    if ((sign(vxn)==1)&&(sign(vyn)==-1)&&(sign(vzn)==1)) quat[3]=-quat[3];

    if ((sign(vxn)==-1)&&(sign(vyn)==-1)&&(sign(vzn)==-1)) quat[3]=-quat[3];


    matrix.Tq[0][0]=quat[0]*quat[0]+quat[1]*quat[1]-quat[2]*quat[2]-quat[3]*quat[3];
    matrix.Tq[0][1]=2*(quat[1]*quat[2]-quat[3]*quat[0]);
    matrix.Tq[0][2]=2*(quat[3]*quat[1]+quat[2]*quat[0]);
    matrix.Tq[1][0]=2*(quat[2]*quat[1]+quat[3]*quat[0]);
    matrix.Tq[1][1]=quat[0]*quat[0]-quat[1]*quat[1]+quat[2]*quat[2]-quat[3]*quat[3];
    matrix.Tq[1][2]=2*(quat[2]*quat[3]-quat[1]*quat[0]);
    matrix.Tq[2][0]=2*(quat[3]*quat[1]-quat[2]*quat[0]);
    matrix.Tq[2][1]=2*(quat[2]*quat[3]+quat[1]*quat[0]);
    matrix.Tq[2][2]=quat[0]*quat[0]-quat[1]*quat[1]-quat[2]*quat[2]+quat[3]*quat[3];
 
 //matrix.Tq[0][0]=1;
}

void MPU6050_Calib1()
{
    double vx,vy,vz;
    double norm,vxn,vyn,vzn;
    double c1[3];
    double quat[4];
    double q_sum0,q_sum1,q_sum2,q_sum3;
    vx=(accel_x_scalled);vy=(accel_y_scalled);vz=(accel_z_scalled);
    if (accel_y_scalled<0) vx=-vx; 
    norm=sqrt(vx*vx+vy*vy+vz*vz);
    vxn=vx/norm;
    vyn=vy/norm;
    vzn=vz/norm;
    c1[0]=(vyn*sqrt(1-vxn*vxn-vzn*vzn)-vxn*vzn*sqrt(1-vyn*vyn-vzn*vzn))/(1-vzn*vzn);
    c1[1]=-(vxn*sqrt(1-vyn*vyn-vzn*vzn)-vyn*vzn*sqrt(1-vxn*vxn-vzn*vzn))/(1-vzn*vzn);
    c1[2]=vzn;

    q_sum0=1+c1[0]+c1[1]+c1[2];
    q_sum1=1+c1[0]-c1[1]-c1[2];
    q_sum2=1-c1[0]+c1[1]-c1[2];
    q_sum3=1-c1[0]-c1[1]+c1[2];

    if (q_sum0<0) q_sum0=-q_sum0;
    if (q_sum1<0) q_sum1=-q_sum1;
    if (q_sum2<0) q_sum2=-q_sum2;
    if (q_sum3<0) q_sum3=-q_sum3;

    quat[0]=0.5*sqrt(q_sum0);
    quat[1]=0.5*sqrt(q_sum1);
    quat[2]=0.5*sqrt(q_sum2);
    quat[3]=-0.5*sqrt(q_sum3);

   
    matrix.Tq[0][0]=quat[0]*quat[0]+quat[1]*quat[1]-quat[2]*quat[2]-quat[3]*quat[3];
    matrix.Tq[0][1]=2*(quat[1]*quat[2]-quat[3]*quat[0]);
    matrix.Tq[0][2]=2*(quat[3]*quat[1]+quat[2]*quat[0]);
    matrix.Tq[1][0]=2*(quat[2]*quat[1]+quat[3]*quat[0]);
    matrix.Tq[1][1]=quat[0]*quat[0]-quat[1]*quat[1]+quat[2]*quat[2]-quat[3]*quat[3];
    matrix.Tq[1][2]=2*(quat[2]*quat[3]-quat[1]*quat[0]);
    matrix.Tq[2][0]=2*(quat[3]*quat[1]-quat[2]*quat[0]);
    matrix.Tq[2][1]=2*(quat[2]*quat[3]+quat[1]*quat[0]);
    matrix.Tq[2][2]=quat[0]*quat[0]-quat[1]*quat[1]-quat[2]*quat[2]+quat[3]*quat[3];
 
 //matrix.Tq[0][0]=1;
}

void mat_multipl(double Matrix[3][3], double Vector[3])
{
    double sum=0;
    for (int j=0;j<3;j++)
    {
for (int i=0;i<3;i++) sum=Matrix[i][j]*Vector[i]+sum;
matrix.TVec[j]=sum;sum=0;
    }
}

void mat_multipl1(double Matrix[3][3], double Vector[3])
{
    double sum=0;
    for (int i=0;i<3;i++)
    {
for (int j=0;j<3;j++) sum=Matrix[i][j]*Vector[j]+sum;
matrix.TVec[i]=sum;sum=0;
    }
}


void filter1()
{
  int x=0,y=0,z=0;
for (int i=1;i<=700;i++)
  {
    MPU6050_ReadData();
    x=(x+accel_x_scalled)/2;
    y=(y+accel_y_scalled)/2;
    z=(z+accel_z_scalled)/2;
  }
  accel_x_scalled=x;
  accel_y_scalled=y;
  accel_z_scalled=z;  
}


