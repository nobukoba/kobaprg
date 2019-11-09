#include "TPad.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TBox.h"
#include "TList.h"
#include "TGMsgBox.h"

void multi_fit_photo_peak_for_active_results(){
  std::cout << std::endl << "Macro: multi_fit_photo_peak_for_active_results.C" << std::endl;
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx) {
    gSystem->cd((gBrowserEx->GetInitialWorkingDir()).Data());
  }else{return;}
  std::cout << "gSystem->pwd(): " << gSystem->pwd() << std::endl;
  if (!gPad) {
    std::cout << "There is no gPad. Exit." << std::endl;
    return;
  }
  TCanvas *canvas = gPad->GetCanvas();
  TIter next(gBrowserEx->GetListOfOrderedActiveHistos());
  TH1 * hist;
  while((hist = (TH1*)next())){
    TList *funclist = hist->GetListOfFunctions();
    if(funclist == 0){
      std::cout << "The GetListOfFunctions() is null. The script is terminated." << std::endl;
      return;
    }
    gPad->GetCanvas()->Clear();
    gPad->GetCanvas()->Divide(2,2);
    TVirtualPad *sel_pad;
    sel_pad = gPad->GetCanvas()->cd(1);
    //hist->GetXaxis()->UnZoom();
    hist->GetXaxis()->SetRangeUser(0., 3000.);
    hist->DrawCopy();
    
    //return;
    
    Int_t j = 0;
    TF1 *funcobj = 0;
    std::cout << userdata->GetName() << " center par: ";
    while (funcobj = (TF1*)funclist->FindObject(Form("fit_photo_peak_%d",j))) {
      std::cout << funcobj->GetParameter(1) << ", ";
      sel_pad = gPad->GetCanvas()->cd(j+2);
      Double_t xmin, xmax, xrange;
      funcobj->GetRange(xmin, xmax);
      xrange = xmax - xmin;
      hist->GetXaxis()->SetRangeUser(xmin - xrange / 3., xmax + xrange*(2./3.));
      TH1* histcopy = hist->DrawCopy();
      sel_pad->Update();
      TPaveStats *ps = (TPaveStats*)sel_pad->GetPrimitive("stats");
      if (ps == 0) {
	std::cout << "There is no Pave Stats. The script is terminated." << std::endl;
	return;
      }
      ps->SetName("mystats");
      TList *listOfLines = ps->GetListOfLines();
      
      TText *tconst = 0;
      TString pname, fmt;
      for (Int_t i = 0;i < funcobj->GetNpar(); i++) {
	pname = funcobj->GetParName(i);
	pname += " "; /*In order to distinguish 'R' from 'RMS'*/
	tconst = ps->GetLineWith(pname);
	pname += "= ";
	fmt = Form("%%%s #pm %%%s",ps->GetFitFormat(),ps->GetFitFormat());
	pname += Form(fmt.Data(),funcobj->GetParameter(i),funcobj->GetParError(i));
	if (tconst == 0){
	  break;
	}
	tconst->SetTitle(pname);
      }
      if (tconst == 0){
	break;
      }
      tconst = ps->GetLineWith("#chi^{2} / ndf");
      fmt = Form("#chi^{2} / ndf = %%%s / %%%s",ps->GetFitFormat(),ps->GetFitFormat());
      tconst->SetTitle(Form(fmt.Data(),funcobj->GetChisquare(),(Double_t)funcobj->GetNDF()));
      
      // Add a new line in the stat box.
      // Note that "=" is a control character
      Double_t fit_integ   = funcobj->Integral(xmin, xmax);
      //Double_t fit_integ_e = funcobj->IntegralError(xmin, xmax);
      //TString fmt2 = Form("Fit Integ. = %%%s #pm %%%s",ps->GetFitFormat(),ps->GetFitFormat());
      TString fmt2 = Form("Fit Integ. = %%%s",ps->GetFitFormat());
      //TLatex *myt = new TLatex(0,0,Form(fmt2.Data(),fit_integ,fit_integ_e));
      TLatex *myt = new TLatex(0,0,Form(fmt2.Data(),fit_integ));
      myt->SetTextSize(ps->GetTextSize());
      listOfLines->Add(myt);
      // the following line is needed to avoid that the automatic redrawing of stats
      histcopy->SetStats(0);
      gPad->Update();
      gPad->Modified();
      j++;
    }
    std::cout << std::endl;
    cur_ListTreeItem = gBrowserEx->NextItem(cur_ListTreeItem);
  }
  return;
}
