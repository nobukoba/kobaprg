TGraph *gr[2];

Double_t spl(Double_t *x, Double_t *p){
  return gr[0]->Eval(x[0],0,"S");
  /*return gr[0]->Eval(x[0]);*/
}
void comp(){
  gr[1] = new TGraph("fort.20");
  gr[0] = new TGraph("fort.21");
  TF1 *f  = new TF1("f",spl,0,10,0);

  f->SetNpx(100);
  f->Draw();
  gr[1]->Draw("l");
  
  return;
}
