void cut_xy_sel_pad(){
  gROOT->ProcessLine(".L ./get_th2_in_sel_pad.C");
  TH2* hist = (TH2*)gROOT->ProcessLine("get_th2_in_sel_pad();");
  if(hist == 0){return;}

  char retstr[256];
  new TGInputDialog(gClient->GetRoot(),0,
                    "Range x1 x2 y1 y2: %f %f %f %f",
                    "0.0 1.0 0.0 1.0",retstr);
  if(retstr[0] == 0 && retstr[1] == 0){
    std::cout << "Cancel button was pushed. This script is terminated." << std::endl;
    return;
  }
  TString str = retstr;
  str.ReplaceAll(","," ");
  std::istringstream iss(str.Data());
  Double_t x1, x2, y1, y2;
  iss >> x1 >> x2 >> y1 >> y2;

  gROOT->ProcessLine(".L ../cui/cut_xy.C");
  gROOT->ProcessLine(Form("cut_xy((TH1*)%p,%f,%f,%f,%f)",hist, x1, x2, y1, y2));

  return;
}
