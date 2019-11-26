void test_histos(){
  TDirectory *save = gDirectory;
  gROOT->cd();
  TH1D * h1 = new TH1D("h1","h1", 30, -4, 4);
  TH1D * h2 = new TH1D("h2","h2", 30, -4, 4);
  TH1D * h3 = new TH1D("h3","h3", 30, -4, 4);
  TH2D * h4 = new TH2D("h4","h4", 40, -4, 4, 40, -4, 4);
  TDirectory * dir = gROOT->mkdir("histos_dir");
  dir->cd();
  TH1D * h5 = new TH1D("h5","h5", 50, -4, 4);
  TH1D * h6 = new TH1D("h6","h6", 60, -4, 4);
  TH2D * h7 = new TH2D("h7","h7", 70, -4, 4, 70, -4, 4);
  save->cd();
  Double_t x1, x2, x3, x4, x5, x6;
  for (Int_t i=0; i < 1000; i++) {
      gRandom->Rannor(x1,x2);
      gRandom->Rannor(x3,x4);
      gRandom->Rannor(x5,x6);
      h1->Fill(x1);
      h2->Fill(x2);
      h3->Fill(x3);
      h4->Fill(x3,x4);
      h5->Fill(x5);
      h6->Fill(x6);
      h7->Fill(x5,x6);
  }
  
  return;
}
