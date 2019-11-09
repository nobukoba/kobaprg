void copy_raw(){
  std::cout << std::endl << "Macro: copy_raw.C" << std::endl;
  TFile *fin = new TFile("hist2337.root");
  TFile *fout = new TFile("hist2337_raw.root","recreate");
  
  TDirectory *d = (TDirectory *)fin->Get("Raw");
  TList *lok_in = (TList *)d->GetListOfKeys();
  Int_t nlok_in = lok_in->GetEntries();
  TList *lok_out = new TList();
  std::cout << "nlok_in: " << nlok_in <<std::endl;
  TKey *key_in, *key_out;
  std::string key_in_name, key_out_name;
  key_in = (TKey*)lok_in->At(1);
  key_in_name = key_in->GetName();
  lok_out->Add(key_in);
  for (Int_t i = 3; i < nlok_in; i+=2) {
    key_in = (TKey*)lok_in->At(i);
    key_in_name = key_in->GetName();
    TIter next(lok_out);
    while (key_out = (TKey*)next()){ 
      key_out_name = key_out->GetName();
      //std::cout << "key_in_name "  << key_in_name << std::endl;
      //std::cout << "key_out_name " << key_out_name << std::endl;
      if(key_out_name.compare(key_in_name) > 0){
	lok_out->AddBefore(key_out,key_in);
	break;
      }
    }
    if(key_out==0){
      lok_out->Add(key_in);
    }
  }
  //lok_out->Print();
  std::cout << "nlok_out: " << lok_out->GetEntries() <<std::endl;
  TIter outnext(lok_out);
  while (key_out = (TKey*)outnext()){ 
    //std:::cout<< key_out->GetName()<< std::endl;
    TObject *obj = key_out->ReadObj();
    fout->cd();
    obj->Write();
    //delete obj;
  }
  lok_out->ls();
}
