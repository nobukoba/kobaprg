void transform_1d(){
  if (!gPad) {
    std::cout << "There is no gPad. This script is terminated." << std::endl;
    return;
  }
  TVirtualPad *sel_pad = gROOT->GetSelectedPad();
  TList *listofpri = sel_pad->GetListOfPrimitives();
  TIter next(listofpri);
  TObject *obj;
  TH2 *hist = 0;
  while (obj = next()){
    if (obj->InheritsFrom("TH2")) {
      std::cout << "This script can not handle TH2 histograms. This script is terminated." << std::endl;
      return;
    }
    if (obj->InheritsFrom("TH1")) {
      hist = (TH1*)obj;
      break;
    }
  }
  if(hist == 0){
    std::cout << "TH1 histogram was not found in this pad. This script is terminated." << std::endl;
    return;
  }
  
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
  hout->Draw();
  gPad->Update();
  gPad->GetFrame()->SetBit(TBox::kCannotMove);
  return;
}
