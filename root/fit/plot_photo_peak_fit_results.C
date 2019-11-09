#include "TPad.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TBox.h"
#include "TList.h"
#include "TGMsgBox.h"

void plot_photo_peak_fit_results(){
  std::cout << std::endl << "Macro: plot_photo_peak_fit_results.C" << std::endl;
  if (!gPad) {
    std::cout << "There is no gPad. This script is terminated." << std::endl;
    return;
  }
  TList *listofpri = gPad->GetListOfPrimitives();
  TIter next(listofpri);
  TObject *obj;
  TH1 *hist = 0;
  while ((obj = next())){
    if (obj->InheritsFrom("TH2")) {
      std::cout << "This script can not handle TH2 histograms." << std::endl;
      return;
    }
    if (obj->InheritsFrom("TH1")) {
      hist = (TH1*)obj;
      break;
    }
  }
  if(hist == 0){
    std::cout << "TH1 histogram was not found in this pad. The script is terminated." << std::endl;
    return;
  }
  
  TList *funclist = hist->GetListOfFunctions();
  if(funclist == 0){
    std::cout << "The GetListOfFunctions() is null. The script is terminated." << std::endl;
    return;
  }
  gPad->GetCanvas()->Clear();
  gPad->GetCanvas()->Divide(2,4);
  TVirtualPad *sel_pad;
  sel_pad = gPad->GetCanvas()->cd(1);
  //hist->GetXaxis()->UnZoom();
  hist->GetXaxis()->SetRangeUser(0., 2000.);
  hist->DrawCopy();
  
  Int_t j = 0;
  TF1 *funcobj = 0;
  while ((funcobj = (TF1*)funclist->FindObject(Form("photo_peak_fit_%d",j)))) {
    sel_pad = canvas->cd(j+2);

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
      //std::cout << tconst->GetTitle() << std::endl;
      pname += "= ";
      fmt = Form("%%%s #pm %%%s",ps->GetFitFormat(),ps->GetFitFormat());
      pname += Form(fmt.Data(),funcobj->GetParameter(i),funcobj->GetParError(i));
      tconst->SetTitle(pname);
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
    sel_pad->Update();
    sel_pad->Modified();
    j++;
  }  
  return;
}
