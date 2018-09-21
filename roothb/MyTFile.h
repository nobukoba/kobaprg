#ifndef MyTFile_h
#define MyTFile_h

//class TFile;
#include "TFile.h"
#include "TTimer.h"

class MyTFile : public TFile {
public:
  MyTFile(const char *, Option_t *, const char *, Int_t);  
  ~MyTFile(); 
  static Option_t * foption(const char *, Option_t *);
  static Int_t extension_checker(const char *);
  static TTimer *shm_timer;
  static TTimer *GetShmTimer(){return shm_timer;}
  ClassDef(MyTFile,1)
};

#endif
