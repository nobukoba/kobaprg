void get_wtdc_coef(){
  TCanvas* canvas = gPad->GetCanvas();
  if (!canvas) {
    std::cout << "There is no canvas." << std::endl;
    return;
  }
  TVirtualPad *pad = 0;
  Int_t j = 0;

  std::cout << "" << std::endl;
  std::cout << "wtdc range" << std::endl;
  std::cout << "{xlow, xup}" << std::endl;
  while ((pad = canvas->GetPad(j))) {
    TList *listofpri = pad->GetListOfPrimitives();
    if (listofpri == 0) {
      std::cout << "The pad includes nothing." << std::endl;
      return;
    }
    TIter next(listofpri);
    TObject *obj;
    TH1 *hist = 0;
    while ((obj = next())){
      if (obj->InheritsFrom("TH2")) {
        std::cout << "This macro does not handle TH2 histo." << std::endl;
        break;
      }
      if (obj->InheritsFrom("TH1")) {
        hist = (TH1*)obj;
        //std::cout << "TH1 histo was found." << std::endl;
        break;
      }
    }
    if(hist == 0){
      //std::cout << "Histogram was not found in this pad." << std::endl;
      j++;
      continue;
    }
    Double_t ymax_4 = hist->GetMaximum()/4.;
    Double_t xlow_4, xup_4;
    for (Int_t i = 1; i <= hist->GetNbinsX(); i++) {
      Double_t ytmp = hist->GetBinContent(i);
      if (ytmp > ymax_4) {
	xlow_4 = hist->GetBinCenter(i);
	break;
      }
    }
    for (Int_t i = hist->GetNbinsX(); i >= 1; i--) {
      Double_t ytmp = hist->GetBinContent(i);
      if (ytmp > ymax_4) {
	xup_4 = hist->GetBinCenter(i);
	break;
      }
    }
    Double_t xmid_4 = (xlow_4 + xup_4)/2.;
    Int_t    imid_4 = hist->FindBin(xmid_4);
    Double_t yped = hist->GetBinContent(imid_4);
    Double_t yped_10 = yped / 10.;
    
    Double_t xlow_10, xup_10;
    for (Int_t i = 1; i <= hist->GetNbinsX(); i++) {
      Double_t y2 = hist->GetBinContent(i);
      if (y2 > yped_10) {
	Double_t y1 = hist->GetBinContent(i-1);
	Double_t x1 = hist->GetBinCenter(i-1);
	Double_t x2 = hist->GetBinCenter(i);
	Double_t a = (y2-y1)/(x2-x1);
	Double_t b = (x2*y1-x1*y2)/(x2-x1);
	xlow_10 = (yped_10 -b) / a;
	break;
      }
    }
    
    for (Int_t i = hist->GetNbinsX(); i >= 1; i--) {
      Double_t y2 = hist->GetBinContent(i);
      if (y2 > yped_10) {
	Double_t y1 = hist->GetBinContent(i+1);
	Double_t x1 = hist->GetBinCenter(i+1);
	Double_t x2 = hist->GetBinCenter(i);
	Double_t a = (y2-y1)/(x2-x1);
	Double_t b = (x2*y1-x1*y2)/(x2-x1);
	xup_10 = (yped_10 -b) / a;
	break;
      }
    }
    pad->cd();
    TLine line;
    line.DrawLine(xlow_10,hist->GetMinimum(),xlow_10,hist->GetMaximum());
    line.DrawLine(xup_10,hist->GetMinimum(),xup_10,hist->GetMaximum());
    line.DrawLine(xmid_4,hist->GetMinimum(),xmid_4,hist->GetMaximum());
    Double_t xrange_min = hist->GetXaxis()->GetBinLowEdge(hist->GetXaxis()->GetFirst());
    Double_t xrange_max = hist->GetXaxis()->GetBinUpEdge(hist->GetXaxis()->GetLast());
    line.DrawLine(xrange_min,yped,xrange_max,yped);
    line.DrawLine(xrange_min,yped_10,xrange_max,yped_10);
    pad->Modified();
    pad->Update();
    std::cout << std::fixed << std::setprecision(0);
    std::cout << "{" << xlow_10 << ", " << xup_10 << "}" << std::endl;    
    j++;
  }
  return;
}
