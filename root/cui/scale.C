void scale(TH1* hist, Double_t par){
  gROOT->ProcessLine(".L ./clone_with_suffix.C");
  TH1 *scl = (TH1*)gROOT->ProcessLine(Form("clone_with_suffix((TH1*)%p,\"%s\");",hist,"_scl"));
  scl->Scale(par);
  save->cd();
  return;
}
