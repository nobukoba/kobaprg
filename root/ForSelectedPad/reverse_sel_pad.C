void reverse_sel_pad(){
  gROOT->ProcessLine(".L ./get_th_in_sel_pad.C");
  TH1* hist = (TH1*)gROOT->ProcessLine("get_th_in_sel_pad();");
  if(hist == 0){return;}

  gROOT->ProcessLine(".L ../cui/reverse.C");
  gROOT->ProcessLine(Form("reverse((TH1*)%p)",hist));

  return;
}
