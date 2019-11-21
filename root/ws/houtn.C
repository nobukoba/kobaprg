#include <iostream>
#include <iomanip>
#include <fstream>
#include "TPad.h"
#include "TCanvas.h"
#include "TList.h"
#include "TH1.h"
#include "TGFileDialog.h"

void houtn(const Int_t n_hist) {
  TCanvas* canvas = gPad->GetCanvas();
  TList* list     = canvas->GetListOfPrimitives();
  if (list->At(0)==0) {return;}
  Int_t no_pads  = list->GetSize();
  if (no_pads != n_hist) {
    std::cout << "Number of pads should be " << n_hist << ". Stopped now." << std::endl;
    return;
  }
  
  TH1 **hist = new TH1*[n_hist];
  for (Int_t i=0; i < n_hist; i++){
    hist[i] = 0;
    TVirtualPad *sel_pad = canvas->GetPad(i+1);
    TList *listofpri = sel_pad->GetListOfPrimitives();
    if (listofpri == 0) {
      std::cout << "The pad includes nothing." << std::endl;
      return;
    }
    TIter next(listofpri);
    TObject *obj;
    while ((obj = next())){
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
  for (Int_t i=1; i<n_hist; i++){
    if(nbinsx != hist[i]->GetNbinsX()) {
      std::cout << "The number of bins is not same as others." << std::endl;
      return;
    }
  }
  TString dir("../../../"); 
  //const TString dir("."); 
  TString fname("temp.hst");
  TGFileInfo fileinfo;
  char *filetypes[] = {"hst files", "*.hst",
		       "All files", "*",
		       0, 0};
  fileinfo.fFileTypes = (const char**)filetypes;
  //fileinfo.fFileTypes = filetypes;
  fileinfo.fIniDir    = StrDup(dir);
  fileinfo.fFilename  = StrDup(fname);
  TGFileDialog* dialog = new TGFileDialog(gClient->GetRoot(),gClient->GetRoot(),kFDSave,&fileinfo);
  if (fileinfo.fIniDir == 0) {
    std::cout << "The directory is null." << std::endl;
    return;
  }
  if (fileinfo.fFilename == 0) {
    //std::cout << "The file name is null." << std::endl;
    std::cout << "Canceled." << std::endl;
    return;
  }
  
  ifstream ifs(fileinfo.fFilename);
  if((!ifs.fail())&&(!fileinfo.fOverwrite)) {
    std::cout << "The file " << fileinfo.fFilename << " exits and fOverwrite is false!" << std::endl;
    return;
  }
  ifs.close();
  
  ofstream ofs(fileinfo.fFilename);
  ofs << std::fixed << std::showpoint;
  for (Int_t i = 1; i <= 1024; i++) {
    for (Int_t j = 0; j < n_hist; j++) {
      ofs << " "
	  << std::setw(7)
	  << std::setprecision(0);
      if (i <= nbinsx) {
	ofs << hist[j]->GetBinContent(i);
      }else{
	ofs << 0.;
      }
    }
    ofs << std::endl;
  }
  ofs.close();
  std::cout << "The file " << fileinfo.fFilename << " was created." << std::endl;
  delete [] hist;
  return;
}

void houtn() {
  houtn(4);
  return;
}
