void scale(TH1* hist, Double_t par){
  TDirectory *save = gDirectory;
  gROOT->cd();
  TString str = hist->GetName();
  str += "_scl";
  TString str_n = str;
  Int_t num = 1;
  while (gROOT->Get(str_n.Data())) {
    str_n = Form("%s%d",str.Data(),num);
    num++;
  }
  TH1 *scl = (TH1*) hist->Clone(str_n);
  scl->Scale(par);
  save->cd();
  return;
}
