void copy_with_cutg(){
  TCanvas* canvas = gPad->GetCanvas();
  if (canvas == 0) {
    std::cout << "There is no canvas." << std::endl;
    return;
  }
  TVirtualPad *sel_pad = canvas->GetPad(gPad->GetNumber());
  if (sel_pad == 0) {
    std::cout << "There is no sel_pad." << std::endl;
    return;
  }
  TList *listofpri = sel_pad->GetListOfPrimitives();
  if (listofpri == 0) {
    std::cout << "The pad includes nothing." << std::endl;
    return;
  }
  TIter next(listofpri);
  TObject *obj;
  TH2 *hist = 0;
  while (obj = next()){
    if (obj->InheritsFrom("TH2")) {
      hist = (TH2*)obj;
      std::cout << "TH2 hist was found." << std::endl;
      break;
    }
  }
  if(hist == 0){
    std::cout << "TH2 histogram was not found in this pad." << std::endl;
    return;
  }

  TCutG *cutg = (TCutG*)listofpri->FindObject("CUTG");
  if (cutg != 0){
    cutg->Delete();
  }
  cutg  = (TCutG*)sel_pad->WaitPrimitive("CUTG","CutG");

  gROOT->cd();
  TString str = hist->GetName();
  str += "_cut";
  TString str_n = str;
  Int_t num = 1;
  while (gROOT->Get(str_n.Data())) {
    str_n.Form("%s%d",str.Data(),num);
    num++;
  }
  TH2 *hout = (TH2*)hist->Clone(str_n);
  //hout->Reset();
  Double_t xx, yy;
  for (Int_t i = 0; i <= hist->GetNbinsX()+1; i++) {
    for (Int_t j = 0; j <= hist->GetNbinsY()+1; j++) {
      xx = hist->GetXaxis()->GetBinCenter(i);
      yy = hist->GetYaxis()->GetBinCenter(j);
      if (cutg->IsInside(xx,yy) != 1) {
	hout->SetBinContent(i,j,0);
      }
    }
  }
  hout->Draw("colz");
  canvas->Update();
  return;
}
