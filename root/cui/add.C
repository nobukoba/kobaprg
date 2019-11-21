TH1* add(TH1 *h1, TH1 *h2, Double_t c1, Double_t c2){
  TDirectory *save = gDirectory;
  gROOT->cd();
  TString str = h1->GetName();
  str += "_add";
  TString str_n = str;
  Int_t num = 1;
  while (gROOT->Get(str_n.Data())) {
    str_n = Form("%s%d",str.Data(),num);
    num++;
  }
  TH1* hadded = (TH1*) h1->Clone(str_n);
  hadded->SetTitle(h1->GetTitle());
  hadded->Scale(c1);
  hadded->Add(h2,c2);
  save->cd();
  return hadded;
}

TH1* add(TH1 *h1, TH1 *h2){
  return add(h1, h2, 1.0, 1.0);
}
