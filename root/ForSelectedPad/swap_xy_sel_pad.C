void swap_xy_sel_pad(){
  gROOT->ProcessLine(".L ./get_th2_in_sel_pad.C");
  TH2* hist = (TH2*)gROOT->ProcessLine("get_th2_in_sel_pad();");
  if(hist == 0){return;}

  gROOT->ProcessLine(".L ../cui/swap_xy.C");
  gROOT->ProcessLine(Form("swap_xy((TH1*)%p)",hist));

  return;
}
