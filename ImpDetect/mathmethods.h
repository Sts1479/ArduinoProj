#ifndef mathmethods_h
#define mathmethods_h
#include "Arduino.h"
#include "math.h"
const int L_buff=10;
const int L_ext_mass=6;
class Math
{
  private:
    int sensorValue=0;
    int N=100; //buffe
   public:
    long protect_int=0;
    int time_flag=0;
    long new_millis=0;
    int isr_count=0;
    int  ADCmass[L_buff];
    int max_mass[L_ext_mass];
    int max_ext=0;
    int sensorMean=0;
    int oldValue=0,oldDelta=0,Delta=0;
    int Extremum=0;
    void timer2init();
    void time_protect();
    void calibr(int pin);
    void Buffer(int data);
    void gradient(int Value, int Thresh);
    void gradient1(int Value, int Thresh);
    int mean_filter(int *Buff);
    int RingBuff(int data, int *Mass);
};

class Data{
  public:
     int ADCBuff[];
    int FilBuff[];
    int ExtremBuff[];
    long TimemsBuff[];
   Data(int L){
    for (int i=0;i<L;i++){
    ADCBuff[L]=0;
    FilBuff[L]=0;
    ExtremBuff[L]=0;
    TimemsBuff[L]=0;
    }
    /*int ADCBuff=new int[L];
    int FilBuff=new int[L];
    int ExtremBuff=new int[L];
    long TimemsBuff=new long[L];*/
  }
};

#endif

