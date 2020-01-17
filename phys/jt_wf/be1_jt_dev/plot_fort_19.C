void plot_fort_19(){
  TGraph *g = new TGraph("fort.19");
  TH1D *h1 = new TH1D("fort_19","fort_19",1601, -0.025, 80.025);
  for (int i=0; i<g->GetN();i++) {
    h1->SetBinContent(i+1,g->GetY()[i]);
  }
  delete g;
}
