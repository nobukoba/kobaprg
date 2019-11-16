void set_x_range(TH1* hist, Double_t par0, Double_t par1){
  hist->GetXaxis()->SetRangeUser(par0, par1);
  return;
}
