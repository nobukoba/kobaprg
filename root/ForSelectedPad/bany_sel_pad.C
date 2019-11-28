void bany_sel_pad(){
  gROOT->ProcessLine(".L ./get_th2_in_sel_pad.C");
  TH2* hist = (TH2*)gROOT->ProcessLine("get_th2_in_sel_pad();");
  if(hist == 0){return;}
  
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx == 0) {return;}
  TString str = gBrowserEx->OpenTGInputDialog("Range: %f %f",2);
  if (str.EqualTo("")){return;}
  
  std::istringstream iss(str.Data());
  Double_t par0, par1;
  iss >> par0 >> par1;

  gROOT->ProcessLine(".L ../cui/bany.C");
  gROOT->ProcessLine(Form("bany((TH1*)%p,%f,%f)",hist,par0,par1));
  return;
}
