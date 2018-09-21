#include <string.h>
#include <stdio.h>
#include "TROOT.h"
#include "TString.h"
#include "MyTFile.h"
#include "TTimer.h"
#include "h2root_func.h"


TTimer * MyTFile::shm_timer = 0;

ClassImp(MyTFile);
MyTFile::~MyTFile(){
  if(shm_timer){
    delete shm_timer; shm_timer = 0;
  }
}

MyTFile::MyTFile(const char *name, Option_t *option,
		 const char *ftitle, Int_t compress):
  TFile(name,foption(name, option),ftitle,compress){
  Int_t ext_flag = extension_checker(name);
  if((ext_flag==1)||(ext_flag==2)){
    if (gROOT->FindObjectAny("hb_init_flag")==0){
      if(init_h2root() < 0) {
	gROOT->Add(new TNamed("hb_init_flag","hb_init_flag"));
      }else{
	printf("MyTFile: maybe init_h2root() is not defined.\n");
	printf("Please use roothb instead of root.\n");
	return;
      }
    }
  }
  if(ext_flag==1){
    if (open_file(name)) {
      printf("Warning: the hbook file could not be opened!\n");
      return;
    }
  }
  if (ext_flag==2) {
    remove(name);
    if (gROOT->FindObjectAny("shm_flag")){
      printf("Warning: shared memory has been already read!\n");
      return;
    }
    gROOT->Add(new TNamed("shm_flag","shm_flag"));
    if (open_file(name)) {
      printf("Warning: shared memory could not be opened!\n");
      return;
    }
    shm_timer = new TTimer();
    shm_timer->Connect("Timeout()", 0, 0, "convert_directory_shm()");
    shm_timer->Start(1000, kFALSE);   // 1 seconds, kFALSE-->loop
  }
  return;
}

Option_t* MyTFile::foption(const char *name, Option_t *option){
  if(extension_checker(name)==0){
    return option;
  }else{
    return "NET";
  }
}

Int_t MyTFile::extension_checker (const char *name){
  TString sname = name;
  if (sname.EndsWith(".hb")||
      sname.EndsWith(".hbk")||
      sname.EndsWith(".hbook")) {
    return 1;
  }else if((strrchr(name,'.') == NULL) &&
	   (strlen(name) <= 4) &&
	   (strlen(name) >= 1)){
    FILE *fp;
    fp = fopen(name, "w");
    if( fp == NULL ) {
      printf("roothb.cc: Error %s was not made!\n",name);
      return -1;
    }
    fprintf(fp, "Temporary file made by MyTFile in order to read shared memory!");
    fclose(fp);
    return 2;
  }else{
    return 0;
  }
}
