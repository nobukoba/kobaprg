void shaping_digitizer_traces(){
  std::cout << "Macro: shaping digitizer_traces"<< std::endl;
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx) {
    gSystem->cd((gBrowserEx->GetInitialWorkingDir()).Data());
  }else{return;}
  std::cout << "gSystem->pwd(): " << gSystem->pwd() << std::endl;

  TFile *f = TFile::Open("./kobaprg/root/exp/qst/run0377.root");
  f->ls();

  TList *grlist = new TList();
  grlist->SetName("grlist");
  gROOT->Add(grlist);
  
  TH1* h = (TH1 *)f->FindObjectAny("bo0_ch8_ev0");
  //std::cout << "gr->GetN(): "<< gr->GetN() <<std::endl;
  //for (Int_t i = 0; i < 2; i ++) {
  //  std::cout << "gr->GetX(): "<< gr->GetX()[i] <<std::endl;
  //  std::cout << "gr->GetY(): "<< gr->GetY()[i] <<std::endl;
  //}
<<<<<<< HEAD
=======

  TList * grlist = new TList();
  grlist->SetName("grlist");
  gROOT->Add(grlist);
>>>>>>> c4785975daa3469f6cfdba21d5cba95466997792
  
  TGraph *V_in = new TGraph();
  V_in->SetName("V_in");
  grlist->Add(V_in);
  Int_t k = 0;
  //for (Int_t i = 1; i <= h->GetNbinsX(); i++) {
<<<<<<< HEAD
  //for (Int_t i = 400000; i <= 700000; i++) {
    for (Int_t i = 657000; i <= 658000; i++) {
=======
  //for (Int_t i = 600000; i <= h->GetNbinsX(); i++) {
  for (Int_t i = 657000; i <= 658000; i++) {
>>>>>>> c4785975daa3469f6cfdba21d5cba95466997792
    V_in->SetPoint(k,i,h->GetBinContent(i));
    k++;
  }

  
  //for (Int_t i = 0; i < 400.; i++) {
  //  Double_t x = i;
  //  Double_t y = 0.;
  //  if (x < 180.) {
  //    y = -400.*exp(-x/2000.0)+1000.;
  //  }else{
  //    y = -1000.*exp(-x/2000.0)+1000.;
  //    //y = 200.;
  //  }
  //  V_in->SetPoint(i,x,y);
  //}
  
  TGraph *dV_in_dt = new TGraph();
  dV_in_dt->SetName("dV_in_dt");
  grlist->Add(dV_in_dt);
  Double_t y_prev = 0.;
  Double_t x_prev = -2.;
  for (Int_t i = 0; i < V_in->GetN(); i++) {
    Double_t x = V_in->GetX()[i];
    Double_t y = V_in->GetY()[i];
    Double_t dy = y - y_prev;
    if (x - x_prev > 1.1) {
      dy = V_in->GetY()[i+1] - V_in->GetY()[i];
    }
    dV_in_dt->SetPoint(i,x,dy);
    x_prev = x;
    y_prev = y;
  }
  dV_in_dt->SetMarkerStyle(20);
  dV_in_dt->Draw("ap");
  V_in->Draw("ap");
  gPad->Update();
  //return;
  Double_t C_D  = 1.0;
  Double_t R_D  = 2.0;
  Double_t R_I  = 1.0;
<<<<<<< HEAD
  Double_t C_I  = 2.5;
=======
  Double_t C_I  = 3.0;
>>>>>>> c4785975daa3469f6cfdba21d5cba95466997792
  Double_t R_pz = 23.0;
  Double_t dt = 1.; /* in us*/

  Double_t C_prime  = C_D*(R_pz/(R_pz+R_D));
  Double_t tau_prime = C_prime*R_D;
  Double_t tau_I = C_I*R_I;
  
  std::cout << "C_D:        " << C_D         <<std::endl;
  std::cout << "R_D:        " << R_D         <<std::endl;
  std::cout << "R_pz:       " << R_pz        <<std::endl;
  std::cout << "dt:         " << dt          <<std::endl;
  std::cout << "C_prim:     " << C_prime     <<std::endl;
  std::cout << "tau_prim:   " << tau_prime   <<std::endl;
  
  TGraph *i_t    = new TGraph();
  TGraph *i_pz_t = new TGraph();
  TGraph *V_mid  = new TGraph();
  V_mid->SetName("V_mid");
  grlist->Add(V_mid);
  Double_t i_t_prev = 0.;
  Double_t i_pz_t_prev = 0.;
  for (Int_t i = 0; i < dV_in_dt->GetN(); i ++) {
    Double_t x = dV_in_dt->GetX()[i];
    Double_t y = dV_in_dt->GetY()[i];
    //if (x - x_prev > 1.1) {
    if (i==0) {
      //std::cout << "x here"<<std::endl;
      i_t_prev = y*tau_prime/R_D;
      //i_t_prev = 0.0;
      i_pz_t_prev = 0.;
    }
    Double_t i_t_cur  =  i_t_prev + dt*(y/R_D - (1./tau_prime)*i_t_prev);
    Double_t i_pz_t_cur = i_pz_t_prev + dt*(1./(C_D*R_pz))*i_t_prev;
    Double_t V_mid_cur = R_D*(i_t_cur+i_pz_t_cur);
    
    //std::cout << "i_t_cur:     " << i_t_cur     <<std::endl;
    //std::cout << "i_t_prev:   " << i_t_prev   <<std::endl;
    //std::cout << "x:     " << x     <<std::endl;
    //std::cout << "x_prev:   " << x_prev   <<std::endl;
    
    i_t->SetPoint(i,x,i_t_cur);
    i_pz_t->SetPoint(i,x,i_pz_t_cur);
    V_mid->SetPoint(i,x,V_mid_cur);
    i_t_prev = i_t_cur;
    i_pz_t_prev = i_pz_t_cur;
    x_prev = x;
  }
  i_t->SetMarkerStyle(20);
  i_t->SetMarkerSize(0.5);
  i_pz_t->SetMarkerStyle(20);
  i_pz_t->SetMarkerSize(0.5);
  V_mid->SetMarkerStyle(20);
  V_mid->SetMarkerSize(0.5);
  i_t->Draw("ap");
  V_mid->Draw("ap");
  gPad->Update();
  //return;
  
  TGraph *dV_mid_dt = new TGraph();
  dV_mid_dt->SetName("dV_mid_dt");
  grlist->Add(dV_mid_dt);
  y_prev = 0.;
  for (Int_t i = 0; i < V_mid->GetN(); i++) {
    Double_t x = V_mid->GetX()[i];
    Double_t y = V_mid->GetY()[i];
    Double_t dy = y - y_prev;
    if (i==0) {
      dy = V_mid->GetY()[1] - V_mid->GetY()[0];
    }
    dV_mid_dt->SetPoint(i,x,dy);
    y_prev = y;
  }
  dV_mid_dt->SetMarkerStyle(20);
  dV_mid_dt->SetMarkerSize(0.5);
  dV_mid_dt->Draw("ap");
  gPad->Update();
  //return;
  
  TGraph *i_I_t = new TGraph();
  TGraph *V_out = new TGraph();
  V_out->SetName("V_out");
  grlist->Add(V_out);
  Double_t i_I_t_prev = 0.;
  x_prev = -2.;
  for (Int_t i = 0; i < dV_mid_dt->GetN(); i ++) {
    Double_t x = dV_mid_dt->GetX()[i];
    Double_t y = dV_mid_dt->GetY()[i];
    //if (x - x_prev > 1.1) {
    if (i==0) {
      std::cout << "x here"<<std::endl;
      i_I_t_prev = 0.0;
      y = 0.0;
    }
    Double_t i_I_t_cur  =  i_I_t_prev + dt*(y/R_I - (1./tau_I)*i_I_t_prev);
    Double_t V_out_cur = V_mid->GetY()[i]-i_I_t_cur*R_I;
    
    i_I_t->SetPoint(i,x,i_I_t_cur);
    V_out->SetPoint(i,x,V_out_cur);
    i_I_t_prev = i_I_t_cur;
    x_prev = x;
  }
  V_out->SetMarkerStyle(20);
  V_out->SetMarkerSize(0.5);

  i_I_t->Draw("ap");
  V_out->Draw("ap");
  gPad->Update();  
  //return;
  
  TGraph *V_out_ave = new TGraph();
  V_out_ave->SetName("V_out_ave");
  grlist->Add(V_out_ave);
  const Int_t n_arr = 5;
  Double_t y_arr[n_arr];
  for (Int_t i = 0; i < V_out->GetN(); i++) {
    Double_t x = V_out->GetX()[i];
    if(i==0){
      y_arr[0] = V_out->GetY()[i];
      y_arr[1] = V_out->GetY()[i];
      y_arr[2] = V_out->GetY()[i];
      y_arr[3] = V_out->GetY()[i+1];
      y_arr[4] = V_out->GetY()[i+2];
    }
    if(i < (V_out_ave->GetN()-2)) {
      y_arr[4] = V_out->GetY()[i+2];
    }else{
      y_arr[4] = V_out->GetY()[i];
    }
    
    Double_t V_out_ave_cur = 0.;
    for (Int_t j = 0; j < n_arr; j++){
      V_out_ave_cur += y_arr[j];
    }
    V_out_ave->SetPoint(i,x,V_out_ave_cur/n_arr);
    for (Int_t j = 0; j < n_arr-1; j++){
      y_arr[j] = y_arr[j+1];
    }
  }
  V_out_ave->SetMarkerStyle(20);
  V_out_ave->SetMarkerSize(0.5);
  V_out_ave->Draw("al");
  gPad->Update();  
  return;
}
