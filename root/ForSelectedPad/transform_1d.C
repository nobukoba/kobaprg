void transform_1d(){
  gROOT->ProcessLine(".L ./get_th1_in_sel_pad.C");
  TH1* hist = (TH1*)gROOT->ProcessLine("get_th1_in_sel_pad();");
  if(hist == 0){return;}
  
  char retstr[256];
  new TGInputDialog(gClient->GetRoot(),0,
		    "Enter parameters for p0 + p1*x + p2*x^2 + ...\nFormat: %f %f %f ...",
                    "0.0 1.0",retstr);
  if(retstr[0] == 0 && retstr[1] == 0){
    std::cout << "Cancel button was pushed. This script is terminated." << std::endl;
    return;
  }
  TString str1 = retstr;
  str1.ReplaceAll(","," ");
  std::istringstream iss1(str1.Data());
  Double_t par;
  TString str_tf1 = "";
  Int_t npar = 0;
  while (iss1 >> par) {
    if(npar == 0) {
      str_tf1 = Form("(%g)",par);
    }else{
      str_tf1 += Form("+(%g)*pow(x,%d)",par,npar);
    }
    npar++;
  }
  TF1 *trs_func = new TF1("trs_func", str_tf1, 0., 1.);
  //while (iss1 >> par) {
  //  if(npar == 0) {
  //    str_tf1 = "[0]";
  //  }else{
  //    str_tf1 += Form("+[%d]*TMath::Power(x,%d)",npar,npar);
  //  }
  //  npar++;
  //}
  //TF1 *trs_func = new TF1("trs_func", str_tf1, 0., 1.);
  //std::istringstream iss2(str);
  //for (Int_t i=0; i < npar; i++) {
  //  iss2 >> par;
  //  trs_func->SetParameter(i,par);
  //}
  //trs_func->Draw();
  
  new TGInputDialog(gClient->GetRoot(),0,
		    "Output histogram's nbin, xmin, xmax\nFormat: %d %f %f",
                    "100 0.0 1.0",retstr);
  if(retstr[0] == 0 && retstr[1] == 0){
    std::cout << "Cancel button was pushed. This script is terminated." << std::endl;
    return;
  }
  TString str2 = retstr;
  str2.ReplaceAll(","," ");
  std::istringstream iss2(str2.Data());
  Int_t nbin = 100;
  Double_t xmin = 0., xmax = 1.;
  iss2 >> nbin >> xmin >> xmax;
  
  gROOT->cd();
  TString str = hist->GetName();
  str += "_trs";
  TString str_n = str;
  Int_t num = 1;
  while (gROOT->Get(str_n.Data())) {
    str_n = Form("%s%d",str.Data(),num);
    num++;
  }
  TH1D *hout = new TH1D(str_n, hist->GetTitle(), nbin, xmin, xmax);
  
  for (Int_t i = 1; i <= hist->GetNbinsX(); i++) {
    Double_t xlow   = hist->GetXaxis()->GetBinLowEdge(i);
    Double_t xup    = hist->GetXaxis()->GetBinUpEdge(i);
    Double_t xlow_t = trs_func->Eval(xlow);
    Double_t xup_t  = trs_func->Eval(xup);
    if (xlow_t > xup_t) {
      Double_t xtmp = xlow_t;
      xlow_t = xup_t;
      xup_t = xtmp;
    }
    Double_t xrange_t = xup_t - xlow_t;
    Double_t yin = hist->GetBinContent(i);
    Double_t yin_per_x = yin / xrange_t;
    Int_t ilow_t = hout->FindBin(xlow_t);
    Int_t iup_t  = hout->FindBin(xup_t);
    for (Int_t ifill = ilow_t; ifill <= iup_t; ifill++) {
      Double_t xwid = hout->GetXaxis()->GetBinWidth(ifill);
      if (ifill == ilow_t) {
	xwid = xwid - (xlow_t - hout->GetXaxis()->GetBinLowEdge(ifill));
      }
      if (ifill == iup_t) {
	xwid = xwid - (hout->GetXaxis()->GetBinUpEdge(ifill) - xup_t);
      }
      hout->Fill(hout->GetBinCenter(ifill), yin_per_x * xwid);
    }
  }
  hout->SetBinContent(0, hist->GetBinContent(0));
  hout->SetBinContent(hist->GetNbinsX()+1, hist->GetBinContent(hist->GetNbinsX()+1));
  hout->SetEntries(hist->GetEntries());
  hout->Draw();
  gPad->Update();
  gPad->GetFrame()->SetBit(TBox::kCannotMove);
  return;
}
