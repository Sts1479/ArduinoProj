#include <SD.h>
#include <SPI.h>
#include <Time.h>
#include <Stream.h>
#include "mathmethods.h"
#include "sdfile.h"
/// INIT VARIABLES and CONSTANTS
const int analogInPin=A0; //analog ADC pin initilization
const int ADC_thresh=100; //lower treshold of the ADC values
// now ADC=Uin*1023/Uref; Uref=5 V;now Uin_low=0.48 V 
const int prot_int_val=40; //It's a time protect interval, 
//1 dim~ 1 ms, now 200~200 ms;
const short int chipSelectPin=53;// number of the pin SD chipselect config
// in UNO, NANO and =10;
String SDfilename="test.txt";// path of the file in a SD card
char SDfile[]="test000.txt";
int sensorValue=0;
int meansensValue=0;
int filteredValue;
long time_old=0;
long time_now=0;
const int LengthBuff=50; // Lenght of the buffer to write 
//data to the SD card
///////
short int flag_write=0;
Math Math_obj;
//Data Data_obj= new Data(LengthBuff);
int ADCBuff[LengthBuff];
int FilBuff[LengthBuff];
int ExtremBuff[LengthBuff];
long TimemsBuff[LengthBuff];
int i1=0,i2=0,i3=0;
SdFileAll SdFile_obj;
int buffcount=0;
void setup() {
 // analogReference(DEFAULT); //
  analogReference(INTERNAL2V56); // in UNO, NANO analogReference(INTERNAL)
 //or analogReference(DEFAULT)
  Serial.begin(115200);  
  Serial.println("Enter name of the file");
 
  SDfile[4]=i1+'0';SDfile[5]=i2+'0';SDfile[6]=i3+'0';
  SDfilename=SDfile;
  SdFile_obj.SdFileInit(chipSelectPin);
  //SdFile_obj.SdFileInit(chipSelectPin,SDfilename);
 
  Math_obj.timer2init();
  Math_obj.calibr(analogInPin);
}

void loop() {
   sensorValue=analogRead(analogInPin);
   Math_obj.Buffer(sensorValue);
   filteredValue=Math_obj.mean_filter(Math_obj.ADCmass);
   meansensValue= filteredValue-Math_obj.sensorMean;//mean value ofthe ADC;    
   //Serial.print("ADC out= ");
   //Serial.println(sensorValue);
   if (Math_obj.time_flag==0){ 
       Math_obj.gradient(meansensValue,ADC_thresh); }
   //for (int i=0;i<LengthBuff;i++) ExtremBuff[i]=Math_obj.Extremum;
    if (Serial.read()=='S') {
      flag_write=1;
      SdFile_obj.File_obj.println("STOP WRITING");
      SdFile_obj.File_obj.close();
      Serial.println(" File closed");
    }
    else if(flag_write==0){
        if (buffcount<LengthBuff){ 
           ADCBuff[buffcount]=sensorValue;
           FilBuff[buffcount]=meansensValue;
           ExtremBuff[buffcount]=Math_obj.Extremum;
           TimemsBuff[buffcount]=millis();
           buffcount++;
              }
        else  {buffcount=0;
               SdFile_obj.SdFileWriteBuff(SDfilename,LengthBuff,ADCBuff, FilBuff, ExtremBuff, TimemsBuff);
             }
    }

   //SdFile_obj.SdFileWriteBuff(SDfilename,LengthBuff,ADCBuff, FilBuff, ExtremBuff, TimemsBuff);
 }
        
 ISR(TIMER2_OVF_vect) {
    ++Math_obj.isr_count;
    if (Math_obj.isr_count==prot_int_val) {
      Math_obj.isr_count=0;
      Math_obj.time_flag=0;
    }
    }
   
 

