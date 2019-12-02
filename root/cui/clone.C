void clone(TH1* hist){
  gROOT->ProcessLine(".L ../cui/clone_with_suffix.C");
  gROOT->ProcessLine(Form("clone_with_suffix((TH1*)%p,\"%s\");",hist,"_cln"));
  return;
}
