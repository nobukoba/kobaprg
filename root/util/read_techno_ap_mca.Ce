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

void read_techno_ap_mca() {
  std::cout << std::endl << "Macro: read_techno_ap_mca.C" << std::endl;
  TBrowserEx *gBrowserEx_tmp = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx_tmp) {
    gSystem->cd((gBrowserEx_tmp->GetInitialWorkingDir()).Data());
  }else{return;}

  TString dir("."); 
  /* onst TString dir(".");  */
  TGFileInfo fileinfo;
  char *filetypes[] = {"All files", "*",
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
  Double_t *live_time_ch1 = new Double_t[nofiles];
  Double_t *live_time_ch2 = new Double_t[nofiles];
  Double_t *live_time_ch3 = new Double_t[nofiles];
  Double_t *live_time_ch4 = new Double_t[nofiles];
  Double_t *meas_time_ch1 = new Double_t[nofiles];
  Double_t *meas_time_ch2 = new Double_t[nofiles];
  Double_t *meas_time_ch3 = new Double_t[nofiles];
  Double_t *meas_time_ch4 = new Double_t[nofiles];
  TH1D    **hist_meas_ch1 = new TH1D*[nofiles];
  TH1D    **hist_meas_ch2 = new TH1D*[nofiles];
  TH1D    **hist_meas_ch3 = new TH1D*[nofiles];
  TH1D    **hist_meas_ch4 = new TH1D*[nofiles];
  TH1D    **hist_real_ch1 = new TH1D*[nofiles];
  TH1D    **hist_real_ch2 = new TH1D*[nofiles];
  TH1D    **hist_real_ch3 = new TH1D*[nofiles];
  TH1D    **hist_real_ch4 = new TH1D*[nofiles];
  TH1D    **hist_rate_ch1 = new TH1D*[nofiles];
  TH1D    **hist_rate_ch2 = new TH1D*[nofiles];
  TH1D    **hist_rate_ch3 = new TH1D*[nofiles];
  TH1D    **hist_rate_ch4 = new TH1D*[nofiles];
  
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
    TString tbuf;
    /*/ Int_t xmin, xmax; */

    while(ifs && getline(ifs, buf)){ /* getline is very slow in CINT!*/
      tbuf = buf;
      /* std::cout << tbuf << std::endl; */
      if(tbuf.BeginsWith("[Data]")){
        /* std::cout << "[Data] here\n" << std::endl; */
        TString str_n = "f1_ch1";
        Int_t num = 1;
        while (gROOT->Get(str_n.Data())) {
          num++;
          str_n = Form("f%d_ch1",num);
        }
        hist_meas_ch1[ifile] = new TH1D(Form("f%d_ch1",num),basename.c_str(),16384,0,16384);
        hist_meas_ch2[ifile] = new TH1D(Form("f%d_ch2",num),basename.c_str(),16384,0,16384);
        hist_meas_ch3[ifile] = new TH1D(Form("f%d_ch3",num),basename.c_str(),16384,0,16384);
        hist_meas_ch4[ifile] = new TH1D(Form("f%d_ch4",num),basename.c_str(),16384,0,16384);
        getline(ifs, buf);
        break;
      }
    }
    /* iline++;
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
       Int_t nbin = xmax - xmin; */
    Int_t iline = 0;
    Double_t val[5];
    while(ifs && getline(ifs, buf)){
      iline++;
      tbuf = buf;
      tbuf.ReplaceAll(","," ");
      std::istringstream iss(tbuf.Data());
      iss >> val[0] >> val[1] >> val[2] >> val[3] >> val[4];
      /* Int_t spe_line = iline - 10;
      if (spe_line <= xmax){
      std::cout<< "spe_line:" << spe_line << "val:" << val<< std::endl; */
	(hist_meas_ch1[ifile])->SetBinContent(iline,val[1]);
	(hist_meas_ch2[ifile])->SetBinContent(iline,val[2]);
	(hist_meas_ch3[ifile])->SetBinContent(iline,val[3]);
	(hist_meas_ch4[ifile])->SetBinContent(iline,val[4]);
        /*}else{
        break;
        }*/
    }
    ifile++;
  }

  return;
  /*  
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
  return; */
}
