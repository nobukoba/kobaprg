TGraph *gr[2];

Double_t dcs_fuc(Double_t *x, Double_t *p){
    return gr[0]->Eval(x[0],0,"S");
    /*return gr[0]->Eval(x[0]);*/
}
void plot_dcs(){
    gr[0] = new TGraph("dcs_pZr90E80_num.dat");
    gr[1] = new TGraph("dcs_pZr94E80_num.dat");
    gr[0]->Draw("a*");
    TF1 *f = new TF1("f",dcs_fuc,0,180,0);
    f->SetNpx(1800);
    f->Draw("same,l");
    return;
}
