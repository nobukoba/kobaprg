TH1* sub(TH1 *h1, TH1 *h2, Double_t c1, Double_t c2){
  TDirectory *save = gDirectory;
  gROOT->cd();
  TString str = h1->GetName();
  str += "_sub";
  TString str_n = str;
  Int_t num = 1;
  while (gROOT->Get(str_n.Data())) {
    str_n = Form("%s%d",str.Data(),num);
    num++;
  }
  TH1* hsub   = (TH1*) h1->Clone(str_n);
  hsub->SetTitle(h1->GetTitle());
  hsub->Scale(c1);
  hsub->Add(h2,-c2);
  save->cd();
  return hsub;
}

TH1* sub(TH1 *h1, TH1 *h2){
  return sub(h1, h2, 1.0, 1.0);
}
