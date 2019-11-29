TH1* add(TH1 *hist, TH1 *hist2, Double_t c1, Double_t c2){
  gROOT->ProcessLine(".L ../cui/clone_with_suffix.C");
  TH1 *hadded = (TH1*)gROOT->ProcessLine(Form("clone_with_suffix((TH1*)%p,\"%s\");",hist,"_add"));
  hadded->SetTitle(hist->GetTitle());
  hadded->Scale(c1);
  hadded->Add(hist2,c2);
  return hadded;
}

TH1* add(TH1 *h1, TH1 *h2){
  return add(h1, h2, 1.0, 1.0);
}
