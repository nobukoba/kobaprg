void cut_xy_sel_pad(){
  gROOT->ProcessLine(".L ./get_th2_in_sel_pad.C");
  TH2* hist = (TH2*)gROOT->ProcessLine("get_th2_in_sel_pad();");
  if(hist == 0){return;}

  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx == 0) {return;}
  TString str = gBrowserEx->OpenTGInputDialog("Range x1 x2 y1 y2: %f %f %f %f",4);
  if (str.EqualTo("")){return;}

  str.ReplaceAll(","," ");
  std::istringstream iss(str.Data());
  Double_t x1, x2, y1, y2;
  iss >> x1 >> x2 >> y1 >> y2;

  gROOT->ProcessLine(".L ../cui/cut_xy.C");
  gROOT->ProcessLine(Form("cut_xy((TH1*)%p,%f,%f,%f,%f)",hist, x1, x2, y1, y2));

  return;
}
