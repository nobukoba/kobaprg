void swap_xy(TH1* hist){
  gROOT->ProcessLine(".L ../cui/clone_with_suffix.C");
  TH2 *hout = (TH2*)gROOT->ProcessLine(Form("clone_with_suffix((TH1*)%p,\"%s\");",hist,"_swp"));
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
  gPad->Modified();
  gPad->Update();
  gPad->GetFrame()->SetBit(TBox::kCannotMove);
  return;
}
