#include <iostream>
#include <string>
#include "TMath.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TPad.h"
#include "TList.h"
#include "TString.h"
#include "TVirtualFitter.h"
#include "TFitResult.h"
#include "TGMsgBox.h"

extern Double_t fit_photo_peak_PolyBg(Double_t *dim, Double_t *par, Int_t order);
extern Double_t fit_photo_peak_StepFunction(Double_t *dim, Double_t *par);
extern Double_t fit_photo_peak_StepBG(Double_t *dim, Double_t *par);
extern Double_t fit_photo_peak_Gaus(Double_t *dim, Double_t *par);
extern Double_t fit_photo_peak_SkewedGaus(Double_t *dim,Double_t *par);
extern Double_t fit_photo_peak_PhotoPeak(Double_t *dim,Double_t *par);
extern Double_t fit_photo_peak_PhotoPeakBG(Double_t *dim,Double_t *par);

Double_t fit_photo_peak_PhotoPeakBG(Double_t *dim,Double_t *par) {
  // - dim[0]: channels to fit
  // - par[0]: height of peak
  // - par[1]: cent of peak
  // - par[2]: sigma
  // - par[3]: relative height of skewed gaus to gaus
  // - par[4]: "skewedness" of the skewed gaussin
  // - par[5]: size of stepfunction step.

  // - par[6]: base bg height.
  // - par[7]: slope of bg.
  
  double spar[4];
  spar[0] = par[0];
  spar[1] = par[1];
  spar[2] = par[2];
  spar[3] = par[5];  //stepsize;
  return fit_photo_peak_Gaus(dim,par) + fit_photo_peak_SkewedGaus(dim,par) + fit_photo_peak_StepFunction(dim,spar) + fit_photo_peak_PolyBg(dim,par+6,0);
}

Double_t fit_photo_peak_PolyBg(Double_t *dim, Double_t *par, Int_t order) {
  Double_t result = 0.0;
  Int_t j = 0;
  for(Int_t i = 0; i <= order; i++) {
    result += *(par+j) *TMath::Power(dim[0],i);
    j++;
  }
  return result;
}

Double_t fit_photo_peak_Const(Double_t *dim, Double_t *par) {
  return par[0];
}


Double_t fit_photo_peak_StepFunction(Double_t *dim, Double_t *par) {
  //  -dim[0]: channels to fit
  //  -par[0]: height of peak
  //  -par[1]: centroid of peak
  //  -par[2]: sigma of peak
  //  -par[3]: size of step in step function.
  
  Double_t x       = dim[0];
  
  Double_t height  = par[0];
  Double_t cent    = par[1];
  Double_t sigma   = par[2];
  //Double_t R       = par[4];
  Double_t step    = par[3];
  
  //return TMath::Abs(step)*height/100.0*TMath::Erfc((x-cent)/(TMath::Sqrt(2.0)*sigma));
  return height*(step/100.0) *TMath::Erfc((x-cent)/(TMath::Sqrt(2.0)*sigma));
}

Double_t fit_photo_peak_StepBG(Double_t *dim, Double_t *par) {
  return fit_photo_peak_StepFunction(dim,par) + fit_photo_peak_PolyBg(dim,(par+4),0);
}

Double_t fit_photo_peak_Gaus(Double_t *dim, Double_t *par) {
  // - dim[0]: channels to fit
  // - par[0]: height of peak
  // - par[1]: cent of peak
  // - par[2]: sigma
  // - par[3]: relative height of skewed gaus to gaus

  Double_t x      = dim[0];
  Double_t height = par[0];
  Double_t cent   = par[1];
  Double_t sigma  = par[2];
  Double_t R      = par[3];

  return height*(1.0-R/100.0)*TMath::Gaus(x,cent,sigma);
}

Double_t fit_photo_peak_SkewedGaus(Double_t *dim,Double_t *par) {
  // StepFunction(dim,par) + PolyBg
  // - par[0]: height of peak
  // - par[1]: cent of peak
  // - par[2]: sigma
  // - par[3]: relative height of skewed gaus to gaus
  // - par[4]: "skewedness" of the skewed gaussin

  Double_t x      = dim[0]; //channel number used for fitting
  Double_t height = par[0]; //height of photopeak
  Double_t cent   = par[1]; //Peak Centroid of non skew gaus
  Double_t sigma  = par[2]; //standard deviation of gaussian
  Double_t R      = par[3]; //relative height of skewed gaussian
  Double_t beta   = par[4]; //"skewedness" of the skewed gaussian

  double scaling = R*height/100.0;
  //double x_rel = (x - cent)/sigma;

  double fterm = (x-cent)/(sigma*TMath::Sqrt(2.));
  double sterm = sigma /  (beta *TMath::Sqrt(2.));

  return scaling * TMath::Exp((x-cent)/beta) * TMath::Erfc(fterm + sterm); 
}

Double_t fit_photo_peak_PhotoPeak(Double_t *dim,Double_t *par) {
  return fit_photo_peak_Gaus(dim,par) + fit_photo_peak_SkewedGaus(dim,par);
}

void fit_photo_peak_InitParams(TH1 *fithist, TF1 *fitfunc){
  if(!fithist){
    printf("No histogram is associated yet, no initial guesses made\n");
    return;
  }
  //printf("%s called.\n",__PRETTY_FUNCTION__); fflush(stdout);
  //Makes initial guesses at parameters for the fit. Uses the histogram to
  Double_t xlow,xhigh;
  fitfunc->GetRange(xlow,xhigh);

  //Int_t bin = fithist->GetXaxis()->FindBin(GetParameter("centroid"));
  Int_t binlow = fithist->GetXaxis()->FindBin(xlow);
  Int_t binhigh = fithist->GetXaxis()->FindBin(xhigh);

  Double_t highy  = fithist->GetBinContent(binlow);
  Double_t lowy   = fithist->GetBinContent(binhigh);
  for(int x=1;x<5;x++) {
    highy += fithist->GetBinContent(binlow-x);
    lowy  += fithist->GetBinContent(binhigh+x);
  }
  highy = highy/5.0;
  lowy = lowy/5.0;

//  Double_t yhigh  = fithist->GetBinContent(binhigh);
//  Double_t ylow   = fithist->GetBinContent(binlow);
  if(lowy>highy){
    //std::swap(lowy,highy);
    Double_t tmp = lowy;
    lowy = highy;
    highy = tmp;
  }

  double largestx=0.0;
  double largesty=0.0;
  int i = binlow;
  for(;i<=binhigh;i++) {
    if(fithist->GetBinContent(i) > largesty) {
      largesty = fithist->GetBinContent(i);
      largestx = fithist->GetXaxis()->GetBinCenter(i);
    }
  }

  // - par[0]: height of peak
  // - par[1]: cent of peak
  // - par[2]: sigma
  // - par[3]: R:    relative height of skewed gaus to gaus
  // - par[4]: beta: "skewedness" of the skewed gaussin
  // - par[5]: step: size of stepfunction step.

  // - par[6]: base bg height.

  //limits.
  fitfunc->SetParLimits(0,0,largesty*2);
  fitfunc->SetParLimits(1,xlow,xhigh);
  fitfunc->SetParLimits(2,0.1,xhigh-xlow);
  fitfunc->SetParLimits(3,0.0,40);
  fitfunc->SetParLimits(4,0.01,5);
  //double step = ((highy-lowy)/largesty)*50;
  double step = (highy-lowy)/largesty*50;

  //TF1::SetParLimits(5,step-step*.1,step+.1*step);
  //printf(" highy = %.02f \t lowy = %.02f \t step = %.02f\n",highy,lowy,step); fflush(stdout);
  
  fitfunc->SetParLimits(0,0,largesty*2);
  fitfunc->SetParLimits(1,xlow,xhigh);
  fitfunc->SetParLimits(2,0.1,xhigh-xlow);
  fitfunc->SetParLimits(3,0.0,40);
  //fitfunc->SetParLimits(3,0.0,100);
  fitfunc->SetParLimits(5,0.0,step+step);

  //double slope  = (yhigh-ylow)/(xhigh-xlow);
  //double offset = yhigh-slope*xhigh;
  double offset = lowy;
  fitfunc->SetParLimits(6,offset-0.5*offset,offset+offset);
  //TF1::SetParLimits(7,-2*slope,2*slope);

  //Make initial guesses
  fitfunc->SetParameter(0,largesty);         //fithist->GetBinContent(bin));
  fitfunc->SetParameter(1,largestx);         //GetParameter("centroid"));
  fitfunc->SetParameter(2,(largestx*.01)/2.35);                    //2,(xhigh-xlow));     //2.0/binWidth); //
  fitfunc->SetParameter(3,5.);
  fitfunc->SetParameter(4,1.);
  fitfunc->SetParameter(5,step);
  fitfunc->SetParameter(6,offset);
  //TF1::SetParameter(7,slope);

  fitfunc->SetParError(0,0.10 * largesty);
  fitfunc->SetParError(1,0.25);
  fitfunc->SetParError(2,0.10 *((largestx*.01)/2.35));
  fitfunc->SetParError(3,5);
  fitfunc->SetParError(4,0.5);
  fitfunc->SetParError(5,0.10 * step);
  fitfunc->SetParError(6,0.10 * offset);
  return;
}

void fit_photo_peak_InitNames(TF1 *fitfunc){
  fitfunc->SetParName(0,"Height");
  fitfunc->SetParName(1,"centroid");
  fitfunc->SetParName(2,"sigma");
  fitfunc->SetParName(3,"R");
  fitfunc->SetParName(4,"beta");
  fitfunc->SetParName(5,"step");
  fitfunc->SetParName(6,"bg_offset");
}

void fit_photo_peak_DoFit(TH1 *fithist, TF1 *fitfunc, Option_t *opt) {
  TString options = opt;
  TVirtualFitter::SetMaxIterations(100000);

  bool verbose = !options.Contains("Q");
  bool noprint =  options.Contains("no-print");
  if(noprint) {
    options.ReplaceAll("no-print","");
  }

  if(fithist->GetSumw2()->fN!=fithist->GetNbinsX()+2)
    fithist->Sumw2();
  
  //TFitResultPtr fitres = fithist->Fit(fitfunc,Form("%sLRSME",options.Data()));
  TFitResultPtr fitres = fithist->Fit(fitfunc,Form("%sLRSME",options.Data()));
  
  //fitres.Get()->Print();
  printf("chi^2/NDF = %.02f\n",fitfunc->GetChisquare()/(double)fitfunc->GetNDF());

  if(!fitres.Get()->IsValid()) {
    printf("fit has failed, trying refit... ");
    //SetParameter(3,0.1);
    //SetParameter(4,0.01);
    //SetParameter(5,0.0);
    fithist->GetListOfFunctions()->Last()->Delete();
    fitres = fithist->Fit(fitfunc,Form("%sLRSME",options.Data())); //,Form("%sRSM",options.Data()))
    if( fitres.Get()->IsValid() ) {
      printf(" refit passed!\n");
    } else {
      printf(" refit also failed :( \n");
    }
  }

  //Double_t binwidth = fithist->GetBinWidth(GetParameter("centroid"));
  //Double_t width    = TF1::GetParameter("sigma");
  Double_t xlow,xhigh;
  //Double_t int_low,int_high;
  fitfunc->GetRange(xlow,xhigh);
  //int_low  = xlow - 5.*width;
  //int_high = xhigh +5.*width;

  TF1 *fbg = new TF1("fit_photo_peak_bg", fit_photo_peak_StepBG, xlow, xhigh, 6);
  fbg->SetParameter(0,fitfunc->GetParameter(0));
  fbg->SetParameter(1,fitfunc->GetParameter(1));
  fbg->SetParameter(2,fitfunc->GetParameter(2));
  fbg->SetParameter(3,fitfunc->GetParameter(5));
  fbg->SetParameter(4,fitfunc->GetParameter(6));
  
  double fArea = fitfunc->Integral(xlow,xhigh) / fithist->GetBinWidth(1);
  double bgArea = fbg->Integral(xlow,xhigh) / fithist->GetBinWidth(1);
  fArea -= bgArea;
  delete fbg;

  if(xlow>xhigh){
    //std::swap(xlow,xhigh);
    Double_t tmp = xlow;
    xlow = xhigh;
    xhigh = tmp;
  }

  double fSum = fithist->Integral(fithist->GetXaxis()->FindBin(xlow),
                           fithist->GetXaxis()->FindBin(xhigh));// * fithist->GetBinWidth(1);
  printf("sum between markers: %02f\n",fSum);
  double fDSum = TMath::Sqrt(fSum);
  fSum -= bgArea;
  printf("sum after subtraction: %02f\n",fSum);

  if(!verbose) {
    printf("hist: %s\n",fithist->GetName());
    //Print();
    /*
    printf("BG Area:         %.02f\n",bgArea);
    printf("GetChisquared(): %.4f\n", fitfunc->GetChisquare());
    printf("GetNDF():        %i\n",   fitfunc->GetNDF());
    printf("GetProb():       %.4f\n", fitfunc->GetProb());*/
    //TF1::Print();
  }

  //printf("fithist->GetListOfFunctions()->FindObject(this) = 0x%08x\n",fithist->GetListOfFunctions()->FindObject(GetName()));
  //fflush(stdout);
  //Copy(*fithist->GetListOfFunctions()->FindObject(GetName()));
  //  fithist->GetListOfFunctions()->Add(&fBGFit); //use to be a clone.
  //fithist->GetListOfFunctions()->Add(fBGFit.Clone()); //use to be a clone.

  //SetParent(0); //fithist);

  //delete tmppeak;
  return;
}

void fit_photo_peak(Double_t x0, Double_t x1){
  TCanvas* canvas;
  if (!(canvas = gPad->GetCanvas())) {
    std::cout << "There is no canvas." << std::endl;
    return;
  }
  TList* listofpri = gPad->GetListOfPrimitives();
  TH1* hist = 0;
  TIter next(listofpri); TObject *obj;
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
  if (!hist) {
    std::cout << "TH1 histogram was not found in this pad." << std::endl;
    return;
  }

  if (x0 > x1){
    Double_t tmp;
    tmp = x1;
    x1 = x0;
    x0 = tmp;
  }

  Int_t j = 0;
  while(hist->GetListOfFunctions()->FindObject(Form("fit_photo_peak_%d",j))){
    j++;
  }
  TF1 *f = new TF1(Form("fit_photo_peak_%d",j), fit_photo_peak_PhotoPeakBG, x0, x1, 7);
  f->SetLineWidth(2);
  
  fit_photo_peak_InitNames(f);
  fit_photo_peak_InitParams(hist, f);
  //fit_photo_peak_DoFit(hist,f,"Q+");

  
  if (j==0) {
    hist->Fit(f,"R");
    hist->GetListOfFunctions()->Last()->Delete();
    hist->GetListOfFunctions()->Add(f->Clone());
  }else{
    hist->Fit(f,"R+");
  }

  
  TF1 *fbg = new TF1(Form("fit_photo_peak_bg_%d",j), fit_photo_peak_StepBG, x0, x1, 5);
  fbg->SetLineWidth(1);
  fbg->SetLineColor(1);
  fbg->SetLineStyle(kDashed);
  fbg->SetParameter(0,f->GetParameter(0));
  fbg->SetParameter(1,f->GetParameter(1));
  fbg->SetParameter(2,f->GetParameter(2));
  fbg->SetParameter(3,f->GetParameter(5));
  fbg->SetParameter(4,f->GetParameter(6));

  TF1 *fgaus = new TF1(Form("fit_photo_peak_gaus_%d",j), fit_photo_peak_Gaus, x0, x1, 4);
  fgaus->SetLineWidth(1);
  fgaus->SetLineColor(1);
  fgaus->SetLineStyle(kDashed);
  fgaus->SetParameter(0,f->GetParameter(0));
  fgaus->SetParameter(1,f->GetParameter(1));
  fgaus->SetParameter(2,f->GetParameter(2));
  fgaus->SetParameter(3,f->GetParameter(3));

  TF1 *fsg = new TF1(Form("fit_photo_peak_skewedgaus_%d",j), fit_photo_peak_SkewedGaus, x0, x1, 5);
  fsg->SetLineWidth(1);
  fsg->SetLineColor(1);
  fsg->SetLineStyle(kDashed);
  fsg->SetParameter(0,f->GetParameter(0));
  fsg->SetParameter(1,f->GetParameter(1));
  fsg->SetParameter(2,f->GetParameter(2));
  fsg->SetParameter(3,f->GetParameter(3));
  fsg->SetParameter(4,f->GetParameter(4));

  TF1 *fconst = new TF1(Form("fit_photo_peak_const_%d",j), fit_photo_peak_Const, x0, x1, 1);
  fconst->SetLineWidth(1);
  fconst->SetLineColor(1);
  fconst->SetLineStyle(kDashed);
  fconst->SetParameter(0,f->GetParameter(6));


  hist->GetListOfFunctions()->Add(fbg->Clone());
  hist->GetListOfFunctions()->Add(fgaus->Clone());
  hist->GetListOfFunctions()->Add(fsg->Clone());
  hist->GetListOfFunctions()->Add(fconst->Clone());
  gPad->Update();
  gPad->Modified();
  return;
}

void fit_photo_peak(){
  TCanvas* canvas;
  if (!(canvas = gPad->GetCanvas())) {
    std::cout << "There is no canvas." << std::endl;
    return;
  }
  gPad->SetCrosshair();
  TMarker *mk = (TMarker*)canvas->WaitPrimitive("TMarker","Marker");
  Double_t x0 = mk->GetX();
  delete mk;
  TList* listofpri =gPad->GetListOfPrimitives();
  TH1* hist = 0;
  TIter next(listofpri); TObject *obj;
  while (obj = next()){
    if (obj->InheritsFrom("TH2")) {
      std::cout << "This script can not handle TH2 histograms." << std::endl;
      gPad->SetCrosshair(0);
      return;
    }
    if (obj->InheritsFrom("TH1")) {
      hist = (TH1*)obj;
      break;
    }
  }
  if (!hist) {
    std::cout << "TH1 histogram was not found in this pad." << std::endl;
    gPad->SetCrosshair(0);
    return;
  }
  TLine line;
  line.DrawLine(x0,hist->GetMinimum(),x0,hist->GetMaximum());
  mk = (TMarker*)canvas->WaitPrimitive("TMarker","Marker");
  Double_t x1 = mk->GetX();
  line.DrawLine(x1,hist->GetMinimum(),x1,hist->GetMaximum());
  delete mk;
  gPad->SetCrosshair(0);
  fit_photo_peak(x0, x1);
  return;
}
