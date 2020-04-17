#include <string.h>
#include "TROOT.h"
#include "TRint.h"
#include "TApplication.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TString.h"
#include "TPluginManager.h"


void create_temp_file(int argc, char **argv){
  for (int i = 1; i < argc; i++) {
    TString str = argv[i];
    if((strrchr(str,'.') == NULL) &&
       (str.BeginsWith("-") == 0) &&
       (strlen(str) <= 4) &&
       (strlen(str) >= 1)){
      FILE *fp;
      fp = fopen(str, "w");
      if( fp == NULL ) {
	printf("roothb.cc: Error %s was not made!\n",str.Data());
	return;
      }
      fprintf(fp, "Temporary file made by roothb in order to read shared memory!");
      fclose(fp);
    }
  }
  return;
}

void put_prefix(TObjArray* objarr){
  TIter next(objarr);
  while (TObjString* fileObj = (TObjString*)next()) {
    TString str = fileObj->String();
    if (str.EndsWith(".hb")||
	str.EndsWith(".hbk")||
	str.EndsWith(".hbook")) {
      str = "file:" + str;
      fileObj->SetString(str.Data());
    }else if((strrchr(str,'.') == NULL) &&
	     (str.BeginsWith("-") == 0) &&
	     (strlen(str) <= 4) &&
	     (strlen(str) >= 1)){
      str = "file:" + str;
      fileObj->SetString(str.Data());
    }
  }
  return;
}

int main(int argc, char **argv) {
  create_temp_file(argc, argv);
  TRint *theApp = new TRint("Rint", &argc, argv);

  put_prefix(theApp->InputFiles());
  gROOT->GetListOfSpecials()->Add(theApp);
  gPluginMgr->AddHandler("TFile","^file:","MyTFile","MyTFile", "MyTFile(const char*,Option_t*,const char*,Int_t)");
  theApp->Run();
  delete theApp;
  return 0;
}
