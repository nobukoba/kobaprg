void proy(TH1* hist){
  TString str = hist->GetName();
  str += "_pry";
  TString str_n = str;
  Int_t num = 1;
  while (gROOT->Get(str_n.Data())) {
    str_n = Form("%s%d",str.Data(),num);
    num++;
  }

  TDirectory *save = gDirectory;
  gROOT->cd();
  TH1D *hout = new TH1D(str_n, hist->GetTitle(), hist->GetNbinsY(),
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
