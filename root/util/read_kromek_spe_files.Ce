#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "TROOT.h"
#include "TSystem.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TList.h"
#include "TH1.h"
#include "TList.h"
#include "TGFileDialog.h"

void read_kromek_spe_files() {
  std::cout << std::endl << "Macro: read_kromek_spe_files.C" << std::endl;
  TBrowserEx *gBrowserEx_tmp = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx_tmp) {
    gSystem->cd((gBrowserEx_tmp->GetInitialWorkingDir()).Data());
  }else{return;}

  TString dir("."); 
  /* const TString dir(".");  */
  TGFileInfo fileinfo;
  char *filetypes[] = {"hst files", "*.spe",
		       "All files", "*",
		       0, 0};
  fileinfo.fFileTypes = (const char**)filetypes;
  /* fileinfo.fFileTypes = filetypes; */
  fileinfo.fIniDir    = StrDup(dir);
  fileinfo.SetMultipleSelection(kTRUE);
  TGFileDialog* dialog = new TGFileDialog(gClient->GetRoot(),gClient->GetRoot(),kFDOpen,&fileinfo);
  if (fileinfo.fIniDir == 0) {
    std::cout << "The directory is null. Exit." << std::endl;
    return;
  }
  if ((!fileinfo.fFileNamesList)&&(!fileinfo.fFilename)) {
    std::cout << "Canceled. Exit." << std::endl;
    return;
  }
  TList *listptr;
  if (fileinfo.fFilename) {
    listptr = new TList();
    listptr->Add(new TObjString(fileinfo.fFilename));
  }else{
    listptr = (TList*)fileinfo.fFileNamesList;
  }
  
  Int_t nofiles = listptr->GetEntries();
  Double_t *live_time = new Double_t[nofiles];
  Double_t *meas_time = new Double_t[nofiles];
  TH1D    **hist_meas = new TH1D*[nofiles];
  TH1D    **hist_real = new TH1D*[nofiles];
  TH1D    **hist_rate = new TH1D*[nofiles];
  
  TIter next(listptr);
  TObjString *ostr;
  Int_t ifile = 0;
  gROOT->cd();
  while ((ostr = (TObjString*)next())){
    TString str = ostr->GetString();
    str.ReplaceAll("\"","");
    str.ReplaceAll(" ","");
    std::cout << std::endl;
    std::string filename = str.Data();
    std::cout << "Selected file: " << filename << std::endl;
    std::string basename = gSystem->BaseName(str);
    std::cout << "Base name: " << basename << std::endl;
    
    std::ifstream ifs(filename.c_str());
    std::string buf;
    Int_t iline = -1;
    Int_t xmin, xmax;
    while(ifs && getline(ifs, buf)){ /* getline is very slow in CINT!*/
      iline++;
      if (iline <= 9){
      	std::cout << "Line " << iline << ": " << buf << std::endl;
      }
      if (iline == 7){
      	std::istringstream iss(buf);
      	iss >> live_time[ifile] >> meas_time[ifile];
      }
      if (iline == 9){
      	std::istringstream iss(buf);
      	iss >> xmin >> xmax;
      	Int_t nbin = xmax - xmin;

	TString str_n = "h1";
	Int_t num = 2;
	while (gROOT->Get(str_n.Data())) {
	  str_n = Form("h%d",num);
	  num++;
	}
      	hist_meas[ifile] = new TH1D(str_n,basename.c_str(),nbin,xmin,xmax+1);
	break;
      }
    }
    Double_t val;
    while(ifs >> val){
      iline++;
      Int_t spe_line = iline - 10;
      if (spe_line <= xmax){
	//std::cout<< "spe_line:" << spe_line << "val:" << val<< std::endl;
	(hist_meas[ifile])->SetBinContent(spe_line+1,val);
      }else{
	break;
      }
    }
    ifile++;
  }
  
  for (Int_t i = 0; i < nofiles; i++) {
    TString str = (hist_meas[i])->GetName();
    str += "_real";
    TString str_n = str;
    Int_t num = 1;
    while (gROOT->Get(str_n.Data())) {
      str_n = Form("%s%d",str.Data(),num);
      num++;
    }
    (hist_real[i]) = (TH1D*)(hist_meas[i])->Clone(str_n);
    (hist_real[i])->Scale(meas_time[i]/live_time[i]);
  }
  for (Int_t i = 0; i < nofiles; i++) {
    TString str = (hist_meas[i])->GetName();
    str += "_rate";
    TString str_n = str;
    Int_t num = 1;
    while (gROOT->Get(str_n.Data())) {
      str_n = Form("%s%d",str.Data(),num);
      num++;
    }
    (hist_rate[i]) = (TH1D*)(hist_meas[i])->Clone(str_n);
    (hist_rate[i])->Scale(1./live_time[i]);
  }
  
  if (fileinfo.fFilename) {
    delete listptr;
  }
  delete [] meas_time;
  delete [] live_time;
  return;
}
