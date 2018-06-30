#include <iostream>
#include "TPad.h"
#include "TCanvas.h"
#include "TList.h"
#include "TH1.h"
#include "TGFileDialog.h"

void hout4() {
  TCanvas* canvas = gPad->GetCanvas();
  TList* list     = canvas->GetListOfPrimitives();
  if (list->At(0)==0) {return;}
  Int_t no_pads  = list->GetSize();
  if (no_pads != 4) {
    std::cout << "Number of pads should be 4. Stopped now." << std::endl;
    return;
  }
  TH1 *hist[4];
  for (Int_t i=0; i<4; i++){
    hist[i] = 0;
    TVirtualPad *sel_pad = canvas->GetPad(i+1);
    TList *listofpri = sel_pad->GetListOfPrimitives();
    if (listofpri == 0) {
      std::cout << "The pad includes nothing." << std::endl;
      return;
    }
    TIter next(listofpri);
    TObject *obj;
    while (obj = next()){
      if (obj->InheritsFrom("TH2")) {
	std::cout << "This script can not handle TH2 histograms." << std::endl;
	return;
      }
      if (obj->InheritsFrom("TH1")) {
	hist[i] = (TH1*)obj;
	break;
      }
    }
    if(hist[i] == 0){
      std::cout << "TH1 histogram was not found for i = "<< i << std::endl;
      return;
    }
  }
  
  Int_t nbinsx = hist[0]->GetNbinsX();
  for (Int_t i=1; i<4; i++){
    if(nbinsx != hist[i]->GetNbinsX()) {
      std::cout << "The number of bins is not same as others." << std::endl;
      return;
    }
  }
  static TString dir("../../"); 
  TGFileInfo fileinfo;
  fileinfo.fIniDir="temp.hst";
  const char *filetypes[] = {"hst files", "*.hst",
			     "All files", "*",
			     0, 0};
  fileinfo.fFileTypes = filetypes;
  fileinfo.fIniDir    = StrDup(dir);
  TGFileDialog* dialog = new TGFileDialog(gClient->GetRoot(),0,kFDSave,&fileinfo);
  //cout << "selected file is " << fileinfo.fFilename << endl;
  printf("Save file: %s (dir: %s)\n", fileinfo.fFilename, fileinfo.fIniDir); 
  dir = fileinfo.fIniDir; 
  //if (fileinfo.fFilename) 
  //  SaveFile(fileinfo.fFilename);

}
