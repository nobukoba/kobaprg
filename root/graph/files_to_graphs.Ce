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
#include "TGraphErrors.h"
#include "TGFileDialog.h"

void files_to_graphs() {
  std::cout << std::endl << "Macro: files_to_graphs.C" << std::endl;
  TBrowserEx *gBrowserEx_tmp = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx_tmp) {
    gSystem->cd((gBrowserEx_tmp->GetInitialWorkingDir()).Data());
  }else{return;}

  TString dir("."); 
  //const TString dir("."); 
  TGFileInfo fileinfo;
  char *filetypes[] = {"Text files", "*.txt",
		       "Data files", "*.dat",
		       "All files", "*",
		       0, 0};
  fileinfo.fFileTypes = (const char**)filetypes;
  //fileinfo.fFileTypes = filetypes;
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

  TList *grlist = (TList *)gROOT->GetList()->FindObject("grlist");
  if (!grlist) {
    // I need a directory for graphs. See http://root.cern.ch/root/roottalk/roottalk02/0672.html
    grlist = new TList();
    grlist->SetName("grlist");
    gROOT->Add(grlist);
  }

  Int_t nofiles = listptr->GetEntries();
  TIter next(listptr);
  TObjString *ostr;
  Int_t ifile = 0;
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
    Int_t iline = 0;
    Int_t ndata = 0;
    TGraphErrors *gr = new TGraphErrors();
    TString str_n = "gr1";
    Int_t num = 2;
    while (grlist->FindObject(str_n.Data())) {
      str_n = Form("gr%d",num);
      num++;
    }
    gr->SetName(str_n.Data());
    gr->SetTitle(basename.c_str());
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(1.0);
    grlist->Add(gr);
    while(ifs && getline(ifs, buf)){ /* getline is very slow in CINT!*/
      TString str = buf;
      str.ReplaceAll("\t"," ");
      str.ReplaceAll(","," ");
      std::stringstream ss(str.Data());
      Double_t val[4];
      if (iline == 0) {
	Int_t m;
	for (m = 0; m < 4; m++){
	  if (!(ss >> val[m])){
	    break;
	  }
	}
	ndata = m;
	if (ndata < 2) {
	  std::cout << "Num. of data is less than 2. ndata: " << ndata << std::endl;
	  std::cout << "This script is terminated." << std::endl;
	  return;
	}else{
	  std::cout << "Num. of data in the 1st line: " << ndata << std::endl;
	}
      }else{
	Int_t m;
	for (m = 0; m < 4; m++){
	  if (!(ss >> val[m])){
	    break;
	  }
	}
	if (ndata != m) {
	  std::cout << "ndata != num. of this line: " << ndata  << " != " << m << std::endl;
	  std::cout << "This script is terminated." << std::endl;
	  return;
	}
      }
      if (ndata == 2) {
	gr->SetPoint(gr->GetN(), val[0],  val[1]);
      }else if (ndata == 3) {
	Int_t ngr = gr->GetN();
	gr->SetPoint(ngr, val[0],  val[1]);
	gr->SetPointError(ngr, 0., val[2]);
      }else{
	Int_t ngr = gr->GetN();
	gr->SetPoint(ngr, val[0],  val[1]);
	gr->SetPointError(ngr, val[2], val[3]);
      }
      iline++;
    }
    ifile++;
  }
  
  if (fileinfo.fFilename) {
    delete listptr;
  }
  delete [] meas_time;
  delete [] live_time;
  return;
}
