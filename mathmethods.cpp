#include "mathmethods.h"
#include "Arduino.h"

void Math::calibr(int pin){
  long sum=0;
  for (int i=0;i<N;i++){ 
  sensorValue = analogRead(pin);
  sum=sum+sensorValue; }
  sensorMean=sum/N;
}

void Math::Buffer(int data){
  int L=sizeof(ADCmass)/2;
    for (int k=1;k<L;k++) ADCmass[L-k]=ADCmass[L-k-1]; 
    ADCmass[0]=data;  
}

int Math::RingBuff(int data, int *Mass){
  int L=sizeof(Mass)/2;
    for (int k=1;k<L;k++) Mass[L-k]=Mass[L-k-1]; 
    Mass[0]=data;  
  return *Mass;
}

/*
void Math::Buffer_in(int data, int i){
  int L=sizeof(Math::ADCmass);
  if (Math::ADCmass[0]==0){
  Math::ADCmass[i]=data;
  }
  else{
  Math::ADCmass[i+1]=Math::ADCmass[i];
   Math::ADCmass[0]=data;
  }
}
*/

int Math::mean_filter(int *Buff){
  int sum_mean=0;
  //int Win=sizeof(Buff)/2;
  for (int i=0;i<L_buff;i++) sum_mean=Buff[i]+sum_mean;
  return sum_mean/(L_buff);
}
void Math::gradient(int Value, int Thresh){
  if (abs(Value)>Thresh)
  {
    Delta=oldValue-Value;
     if (oldDelta*Delta<0)
    {
      //Serial.print("Extremum value= ");
      //Serial.println(Value);
      Extremum=Value;
      time_flag=1;
      isr_count=0;
      TCNT2=0;
    }
    oldValue=Value;
    oldDelta=Delta;
  }  
}

void Math::gradient1(int Value, int Thresh){
  int max_Value=0;
  if (abs(Value)>Thresh)
  {
    Delta=oldValue-Value;
     if (oldDelta*Delta<0)
    {
      if (max_ext<L_ext_mass){
      max_mass[max_ext]=Value;
      ++max_ext;
      }
      else{
     // Serial.print("Extremum value= ");
      for (int i=0;i<L_ext_mass;i++)  max_Value=max(max_mass[i],max_Value);
      //Serial.println(max_Value);
     // for (int i=0;i<L_ext_mass;i++) // Serial.println(max_mass[i]);
      //Serial.print("time ");
     // Serial.println(millis()-new_millis);
     Extremum=max_Value;
      //new_millis=millis();
      time_flag=1;
      isr_count=0;
      max_ext=0;
      max_Value=0;
      TCNT2=0;
      }
    }
    oldValue=Value;
    oldDelta=Delta;
  }  
}


void Math::time_protect(){
  if (time_flag==1){
  ++protect_int;}
  if (protect_int==100){
    time_flag=0;
    protect_int=0;
  }
}

void Math::timer2init(){
  TCCR2A = 0;
  TCCR2B = 1<<CS22 | 0<<CS21 | 0<<CS20;
  TIMSK2 = 1<<TOIE2;
}




