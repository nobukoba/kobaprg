TH1* clone_with_suffix(TH1* hist, const char *suffix){
  
  TString str = hist->GetName();
  str += suffix;
  TString str_n = str;
  Int_t num = 1;
  while (gROOT->Get(str_n.Data())) {
    str_n = Form("%s%d",str.Data(),num);
    num++;
  }
  TDirectory *save = gDirectory;
  gROOT->cd();
  TH1* htmp = (TH1*)hist->Clone(str_n);
  save->cd();
  return htmp;
}
