TH1* clone_with_suffix(TH1* hist, const char *suffix){
  TString str;
  Int_t num = 1;
  while (1) {
    str = Form("%s%s%d",hist->GetName(),suffix,num);
    if (gROOT->Get(str.Data())==0) {break;}
    num++;
  }
  TDirectory *save = gDirectory;
  gROOT->cd();
  TH1* htmp = (TH1*)hist->Clone(str);
  save->cd();
  return htmp;
}
