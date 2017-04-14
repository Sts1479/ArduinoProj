#ifndef sdfile_h
#define sdfile_h
#include "Arduino.h"
#include <SD.h>
#include <SPI.h>
class SdFileAll{
  public:
  void SdFileInit(short int chipSelectPin, String filename);
  void SdFileInit(short int chipSelectPin);
  void SdFileWrite();
  void SdFileWrite(String filename, int Value1, int Value2, int Value3, long Value4);
  void SdFileWriteBuff(String filename,const int L, int *Valmass1, int *Valmass2, int *Valmass3, long *Valmass4);
  void SdFileWriteBuff(String filename,const int L, int *Valmass1, int *Valmass2, int Val3, long *Valmass4);
  File File_obj;
};
#endif

