TH1* sub(TH1 *hist, TH1 *hist2, Double_t c1, Double_t c2){
  gROOT->ProcessLine(".L ../cui/clone_with_suffix.C");
  TH1 *hsub = (TH1*)gROOT->ProcessLine(Form("clone_with_suffix((TH1*)%p,\"%s\");",hist,"_sub"));
  hsub->SetTitle(hist->GetTitle());
  hsub->Scale(c1);
  hsub->Add(hist2,-c2);
  return hsub;
}

TH1* sub(TH1 *h1, TH1 *h2){
  return sub(h1, h2, 1.0, 1.0);
}
