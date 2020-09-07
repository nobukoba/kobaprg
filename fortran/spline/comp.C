TGraph *gr[2];

Double_t spl(Double_t *x, Double_t *p){
  return gr[1]->Eval(x[0],0,"S");
  /*return gr[0]->Eval(x[0]);*/
}
void comp(){
  Double_t x[10] = {
    0.0, 1.0, 2.0,  3.0, 4.0,
    5.0, 6.0, 7.0, 9.0, 10.0};
  Double_t y[10] = {
    2.0, 1.0, 3.0, 4.0, 5.0,
    4.0, 2.0, 3.0, 5.0, 9.0};
  gr[0] = new TGraph("spline.txt");
  gr[1] = new TGraph(10, x, y);
  TF1 *f  = new TF1("f",spl,0,10,0);
  f->SetNpx(1000);
  f->Draw();
  gr[1]->Draw("*");
  gr[0]->Draw("l");
  
  return;
}
