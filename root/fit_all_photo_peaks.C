void fit_all_photo_peaks(){
  std::cout << std::endl << "Macro: kobamac/root/fit_all_photo_peaks.C" << std::endl;
  TFile *fin = new TFile("hist2335_calibrated.root");
  
  TList *lok_in = (TList *)fin->GetListOfKeys();
  Int_t nlok_in = lok_in->GetEntries();
  std::cout << nlok_in <<std::endl;
  lok_in->ls();
  TKey *key_in;
  TIter next(lok_in);
  while (key_in = (TKey*)next()){ 
    TObject *obj = key_in->ReadObj();
    if (obj->InheritsFrom("TH1") == 0) {
      continue;    
    }
    std::cout << "obj->GetName() " << obj->GetName() <<std::endl;
    obj->Draw();
    gROOT->ProcessLine(".x photo_peak_fit.C(1150., 1190.)");
    gROOT->ProcessLine(".x photo_peak_fit.C(1300., 1350.)");
    gROOT->ProcessLine(".x plot_photo_peak_fit_results.C");
     new TGMsgBox(gClient->GetRoot(),0, "wait", "wait", kMBIconAsterisk, kMBYes);
  }
}
