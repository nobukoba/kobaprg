void slicex(TH1* hist){
  TString str = hist->GetName();
  str += "_slx_";
  TString str_n = str;
  Int_t num = 1;
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
      if (name.BeginsWith(str_n)){
	found = 1;
	break;
      }
    }
    if (found == 1) {
      str_n = Form("%s_slx%d_",hist->GetName(),num);
      num++;
    }else{
      break;
    }
  }
  Int_t nbins = hist->GetNbinsY();
  Int_t ndig = 1;
  while (nbins > (TMath::Power(10, ndig)-1)) {
    ndig++;
  }
  TString formatstr = Form("%s%%0%dd",str_n.Data(),ndig);
  /* std::cout << formatstr << std::endl;*/
  TDirectory *save = gDirectory;
  gROOT->cd();
  for (Int_t j = 1; j <= hist->GetNbinsY(); j++) {
    TString hname = Form(formatstr.Data(),j);
    TH1D *hout = new TH1D(hname, hist->GetTitle(), hist->GetNbinsX(),
			  hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
    for (Int_t i = 0; i <= hist->GetNbinsX()+1; i++) {
      hout->Fill(hist->GetXaxis()->GetBinCenter(i),
		 hist->GetBinContent(i,j));
    }
  }
  save->cd();
  return;
}
