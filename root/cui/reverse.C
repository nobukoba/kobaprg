void reverse(TH1* hist){
  TDirectory *save = gDirectory;
  gROOT->cd();
  TString str = hist->GetName();
  str += "_rev";
  TString str_n = str;
  Int_t num = 1;
  while (gROOT->Get(str_n.Data())) {
    str_n = Form("%s%d",str.Data(),num);
    num++;
  }
  TH1 *rev = (TH1*) hist->Clone(str_n);
  rev->Scale(-1);
  save->cd();
  return;
}
