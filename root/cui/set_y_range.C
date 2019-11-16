void set_y_range(TH1* hist, Double_t par0, Double_t par1){
  hist->GetYaxis()->SetRangeUser(par0, par1);
  return;
}
