#include "sdfile.h"
//File File_obj;
void SdFileAll::SdFileInit(short int chipSelectPin, String filename){
   pinMode(chipSelectPin, OUTPUT);
   if (!SD.begin(chipSelectPin)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
 // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
   File_obj = SD.open(filename, FILE_WRITE);
    if (File_obj) {
    Serial.println("Writing to ");
    Serial.print(filename);
    Serial.println(" If you want to stop writing to a file, enter 'S'");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening");
    Serial.println(filename);
  }
  
}

void SdFileAll::SdFileInit(short int chipSelectPin){
 
 String filename1="";
 String prob="";
 char Nchar[50];
   pinMode(chipSelectPin, OUTPUT);
   if (!SD.begin(chipSelectPin)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
    Serial.println("Enter name of the file");
 // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
 
  while (Serial.readString().equals(NULL)){
     filename1=Serial.readString();
     if(!filename1.equals(NULL)) break;
  }
 filename1.toCharArray(Nchar,filename1.length());
  Serial.println(Nchar);
   File_obj = SD.open(Nchar, FILE_WRITE);
    if (File_obj) {
    Serial.println("Writing to ");
    Serial.print(Nchar);
    Serial.println(" If you want to stop writing to a file, enter 'S'");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening");
    Serial.println(Nchar);
    return;
  }
  
}

void SdFileAll::SdFileWrite(String filename, int Value1, int Value2, int Value3, long Value4){
   File_obj = SD.open(filename, FILE_WRITE);
    File_obj.print(Value1);
    File_obj.print("\t");
    File_obj.print(Value2);
    File_obj.print("\t");
    File_obj.print(Value3);
    File_obj.print("\t");
    File_obj.println(Value4);
    File_obj.close();
    return;
}

void SdFileAll::SdFileWriteBuff(String filename,const int L, int *Valmass1, int *Valmass2, int *Valmass3, long *Valmass4){
   //File_obj = SD.open(filename, FILE_WRITE);
    for (int i=0;i<L;i++){
    File_obj.print(Valmass1[i]);
    File_obj.print("\t");
    File_obj.print(Valmass2[i]);
    File_obj.print("\t");
    File_obj.print(Valmass3[i]);
    File_obj.print("\t");
    File_obj.println(Valmass4[i]);
    }
    //File_obj.close();
    return;
}

void SdFileAll::SdFileWriteBuff(String filename,const int L, int *Valmass1, int *Valmass2, int Val3, long *Valmass4){
   //File_obj = SD.open(filename, FILE_WRITE);
    for (int i=0;i<L;i++){
    File_obj.print(Valmass1[i]);
    File_obj.print("\t");
    File_obj.print(Valmass2[i]);
    File_obj.print("\t");
    File_obj.print(Val3);
    File_obj.print("\t");
    File_obj.println(Valmass4[i]);
    }
    //File_obj.close();
    //return;
}



void SdFileAll::SdFileWrite(){
   //File_obj = SD.open(filename, FILE_WRITE);
    File_obj.println("testing 1, 2, 3.");
   // File_obj.close();
  //  return;
   // Serial.println("testing 1, 2, 3.");
}
