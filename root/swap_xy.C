Bool_t GetSelectedTH2(TH1*& hist, TCanvas*& canvas,
		      TVirtualPad*& sel_pad, TList*& listofpri){
  if (!(canvas = gPad->GetCanvas())) {
    std::cout << "GetSelectedTH1: There is no canvas." << std::endl;
    return false;
  }
  if (!(sel_pad = canvas->GetPad(gPad->GetNumber()))) {
    std::cout << "GetSelectedTH1: There is no selected pad." << std::endl;
    return false;
  }
  if (!(listofpri = sel_pad->GetListOfPrimitives())) {
    std::cout << "GetSelectedTH1: There is nothing in this pad." << std::endl;
    return false;
  }
  TIter next(listofpri); TObject *obj;
  hist = 0;
  while (obj = next()){
    if (obj->InheritsFrom("TH2")) {
      hist = (TH1*)obj;
      break;
    }
  }
  if (!hist) {
    std::cout << "GetSelectedTH2: TH2 histogram was not found in this pad." << std::endl;
    return false;
  }
  return true;
}

void swap_xy(){
  std::cout << std::endl << "Macro: swap_xy.C" << std::endl;
  TH2* hist; TCanvas* canvas; TVirtualPad* sel_pad; TList* listofpri;
  if (!GetSelectedTH2(hist, canvas, sel_pad, listofpri)) {
    std::cout << "This script is terminated." << std::endl;
    return;
  }
  TString str = hist->GetName();
  str += "_swp";
  TString str_n = str;
  Int_t num = 1;
  while (gROOT->Get(str_n.Data())) {
    str_n = Form("%s%d",str.Data(),num);
    num++;
  }
  gROOT->cd();
  TH2 *hout = (TH2*)hist->Clone(str_n);
  hout->SetTitle(hist->GetTitle());
  hout->Reset();
  hout->SetBins(hist->GetNbinsY(),hist->GetYaxis()->GetXmin(),hist->GetYaxis()->GetXmax(),
		hist->GetNbinsX(),hist->GetXaxis()->GetXmin(),hist->GetXaxis()->GetXmax());
  for (Int_t i=0; i <= hist->GetNbinsX()+1; i++){
    for (Int_t j=0; j <= hist->GetNbinsY()+1; j++){
      hout->SetBinContent(j,i,hist->GetBinContent(i,j));
    }
  }
  hout->SetEntries(hist->GetEntries());
  hout->Draw("colz");
  canvas->Modified();
  canvas->Update();
  return;
}
