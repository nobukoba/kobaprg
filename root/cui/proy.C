void proy(TH1* hist){
  char suffix[] = "_pry";
  Int_t num = 1;
  TString str;
  while (1) {
    str = Form("%s%s%d",hist->GetName(),suffix,num);
    if (gROOT->Get(str.Data())==0) {break;}
    num++;
  }
  TDirectory *save = gDirectory;
  gROOT->cd();
  TH1D *hout = new TH1D(str, hist->GetTitle(), hist->GetNbinsY(),
			hist->GetYaxis()->GetXmin(), hist->GetYaxis()->GetXmax());
  save->cd();
  for (Int_t i = 1; i <= hist->GetNbinsX(); i++) {
    for (Int_t j = 0; j <= hist->GetNbinsY()+1; j++) {
      hout->Fill(hist->GetYaxis()->GetBinCenter(j),
		 hist->GetBinContent(i,j));
    }
  }
  hout->SetEntries(hist->GetEntries());
  hout->Draw();
  gPad->Update();
  gPad->GetFrame()->SetBit(TBox::kCannotMove);
  return;
}
