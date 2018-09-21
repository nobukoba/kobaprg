#include <string.h>
#include "TROOT.h"
#include "TRint.h"
#include "TString.h"
#include "TPluginManager.h"

void put_prefix(int argc, char **argv, char **argvm){
  for (int i = 1; i < argc; i++) {
    TString str = argv[i];
    if (str.EndsWith(".hb")||
	str.EndsWith(".hbk")||
	str.EndsWith(".hbook")) {
      str = "file:" + str;
      strcpy(argvm[i],str.Data());
    }else if((strrchr(str,'.') == NULL) &&
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
      
      str = "file:" + str;
      strcpy(argvm[i],str.Data());
    }else{
      strcpy(argvm[i],str.Data());
    }
    printf("argvm[%d] %s\n",i,argvm[i]);
  }
  return;
}

int main(int argc, char **argv) {
  char ** argvm = new char*[argc];
  for (int i=0;i<argc;i++){argvm[i] = new char[1024];}
  put_prefix(argc, argv, argvm);
  TRint *theApp = new TRint("Rint", &argc, argvm);
  for (int i=0;i<argc;i++){delete argvm[i];}
  delete [] argvm;

  gROOT->GetListOfSpecials()->Add(theApp);
  gPluginMgr->AddHandler("TFile","^file:","MyTFile","MyTFile", "MyTFile(const char*,Option_t*,const char*,Int_t)");
  theApp->Run();
  delete theApp;
  return 0;
}
