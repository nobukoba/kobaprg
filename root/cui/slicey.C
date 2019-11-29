void slicey(TH1* hist){
  Int_t num = 1;
  TString str = Form("%s_sly%d_",hist->GetName(),num);
  TList *groot_list = gROOT->GetList();
  while (1) {
    TIter next(groot_list);
    TObject *obj;
    Int_t found = 0;
    while ((obj = next())) {
      if (!(obj->InheritsFrom("TH1"))) {
	continue;
      } 
      TString name = obj->GetName();
      if (name.BeginsWith(str)){
	found = 1;
	break;
      }
    }
    if (found == 1) {
      str = Form("%s_sly%d_",hist->GetName(),num);
      num++;
    }else{
      break;
    }
  }
  Int_t nbins = hist->GetNbinsX();
  Int_t ndig = 1;
  while (nbins > (TMath::Power(10, ndig)-1)) {
    ndig++;
  }
  TDirectory *save = gDirectory;
  gROOT->cd();
  for (Int_t i = 1; i <= hist->GetNbinsX(); i++) {
    TString formatstr = Form("%s%%0%dd",str.Data(),ndig);
    TString hname = Form(formatstr.Data(),i);
    TH1D *hout = new TH1D(hname, hist->GetTitle(), hist->GetNbinsY(),
			  hist->GetYaxis()->GetXmin(), hist->GetYaxis()->GetXmax());
    for (Int_t j = 0; j <= hist->GetNbinsY()+1; j++) {
      hout->Fill(hist->GetYaxis()->GetBinCenter(j),
		 hist->GetBinContent(i,j));
    }
  }
  save->cd();
  return;
}
