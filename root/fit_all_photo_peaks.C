void fit_all_photo_peaks(){
  std::cout << std::endl << "Macro: kobamac/root/fit_all_photo_peaks.C" << std::endl;
  TFile *fin = new TFile("hist2335_calibrated.root");
  TFile *fout = new TFile("hist2335_calibrated_fit.root","recreate");
  
  TList *lok_in = (TList *)fin->GetListOfKeys();
  Int_t nlok_in = lok_in->GetEntries();
  TList *lok_out = new TList();
  std::cout << nlok_in <<std::endl;
  lok_in->ls();
  TKey *key_in;
  std::string key_in_name, key_out_name;
  TIter next(lok_in);
  while (key_in = (TKey*)next()){ 
    TObject *obj = key_in->ReadObj();
    if (obj->InheritsFrom("TH1") == 0) {
      continue;    
    }
    std::cout << "obj->GetName() " << obj->GetName() <<std::endl;
    obj->Draw();
    gROOT->ProcessLine(".x photo_peak_fit.C(1150.,1190.)");
    gROOT->ProcessLine(".x plot_fit_results.C(\"photo_peak_fit_0\")");
  }
}
