#ifndef MyTFile_h
#define MyTFile_h

//class TFile;
#include "TFile.h"
#include "TTimer.h"

class MyTFile : public TFile {
public:
  MyTFile(const char *, Option_t *, const char *, Int_t);  
  ~MyTFile(); 
  static void ConvertDirectoryShm();
  static Option_t * foption(const char *, Option_t *);
  static Int_t extension_checker(const char *);
  static TTimer *shm_timer;
  static Int_t  hb_init_flag;
  static Int_t  shm_flag;
  static TTimer *GetShmTimer(){return shm_timer;}
  static void StartConversion();
  static void StopConversion();
  ClassDef(MyTFile,1)
};

#endif
