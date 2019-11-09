#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TH1.h"

void add_active_histos(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TIter next(gBrowserEx->GetListOfOrderedActiveHistos());
  TH1 *hist;
  TH1 *added = 0;
  gROOT->cd();
  while((hist = (TH1 *)next())){
	if (added == 0) {
	  TString str = hist->GetName();
	  str += "_add";
	  TString str_n = str;
	  Int_t num = 1;
	  while (gROOT->Get(str_n.Data())) {
	    str_n = Form("%s%d",str.Data(),num);
	    num++;
	  }
	  added = (TH1*) hist->Clone(str_n);
	  added->SetTitle(hist->GetTitle());
	}else{
	  added->Add(hist);
	}
      }
    }
  }
  return;
}
