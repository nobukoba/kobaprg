void bany_sel_pad(){
  gROOT->ProcessLine(".L ./get_th2_in_sel_pad.C");
  TH2* hist = (TH2*)gROOT->ProcessLine("get_th2_in_sel_pad();");
  if(hist == 0){return;}

  char retstr[256];
  new TGInputDialog(gClient->GetRoot(),0,
                    "Range: %f %f",
                    "0.0 1.0",retstr);
  if(retstr[0] == 0 && retstr[1] == 0){
    std::cout << "Cancel button was pushed. This script is terminated." << std::endl;
    return;
  }
  TString str = retstr;
  str.ReplaceAll(","," ");
  std::istringstream iss(str.Data());
  Double_t par0, par1;
  iss >> par0 >> par1;

  gROOT->ProcessLine(".L ../cui/bany.C");
  gROOT->ProcessLine(Form("bany((TH1*)%p,%f,%f)",hist,par0,par1));
  return;
}
