void prox_sel_pad(){
  gROOT->ProcessLine(".L ./get_th2_in_sel_pad.C");
  TH2* hist = (TH2*)gROOT->ProcessLine("get_th2_in_sel_pad();");
  if(hist == 0){return;}
  
  gROOT->ProcessLine(".L ../cui/prox.C");
  gROOT->ProcessLine(Form("prox((TH1*)%p)",hist));

  return;
}
