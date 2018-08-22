void get_coef_y_cor_from_graphs(){
  TCanvas* canvas = gPad->GetCanvas();
  if (canvas == 0) {
    std::cout << "There is no canvas." << std::endl;
    return;
  }
  TVirtualPad *sel_pad = canvas->GetPad(gPad->GetNumber());
  if (sel_pad == 0) {
    std::cout << "There is no sel_pad." << std::endl;
    return;
  }
  TList *listofpri = sel_pad->GetListOfPrimitives();
  if (listofpri == 0) {
    std::cout << "The pad includes nothing." << std::endl;
    return;
  }
  Int_t ngraphs = 0;
  while(listofpri->FindObject(Form("Graph_%d",ngraphs))){
    ngraphs++;
  }
  if (ngraphs == 0) {
    std::cout << "Graph_n was not found in the current pad." << std::endl;
    return;
  }
  
  const Int_t degree_of_polN  = 4;
  TMatrixD fit_pars(ngraphs, degree_of_polN+1);
  for (Int_t i = 0; i < ngraphs; i++) {
    TGraph *gr = (TGraph *)listofpri->FindObject(Form("Graph_%d",i));
    if (degree_of_polN+1 > gr->GetN()) {
      std::cout << "Warning: the number of parameters of polN: " << degree_of_polN+1 << " is larger than the number of the points of " << Form("Graph_%d",i) << ": " << gr->GetN() << std::endl;
    }
    TF1 *fit_func = new TF1("fit_func", Form("pol%d",degree_of_polN), 0., 1.);
    gr->Fit(fit_func);
    for (Int_t j = 0; j < degree_of_polN+1; j++) {
      fit_pars[i][j] = fit_func->GetParameter(j);
      std::cout << "i, j, fit_pars: " << i << ", " << j << ", " << fit_pars[i][j] << std::endl;
    }
    delete fit_func;
  }
  
  TMatrixD m(ngraphs, ngraphs);
  for (Int_t i = 0; i < ngraphs; i++) {
    for (Int_t j = 0; j < ngraphs; j++) {
      m[i][j] = pow(fit_pars[i][0],j);
      std::cout << "i, j, m: " << i << ", " << j << ", " << m[i][j] << std::endl;
    }      
  }
  
  TMatrixD m_inv(ngraphs, ngraphs);
  m_inv = m.Invert();
  
  TMatrixD coef(ngraphs, degree_of_polN+1);
  for (Int_t j = 0; j < degree_of_polN+1; j++) {
    for (Int_t i = 0; i < ngraphs; i++) {
      coef[i][j] = 0.;
      for (Int_t k = 0; k < ngraphs; k++) {
  	coef[i][j] += m_inv[i][k] * -fit_pars[k][j];
      }
      std::cout << "i, j, coef: " << i << ", " << j << ", " << coef[i][j] << std::endl;
    }
  }
  for (Int_t i = 0; i < ngraphs; i++) {
    if (i == 1) {
      coef[i][0] = 1.;
    }else{
      coef[i][0] = 0.;
    }
  }
  for (Int_t j = 0; j < degree_of_polN+1; j++) {
    for (Int_t i = 0; i < ngraphs; i++) {
      std::cout << "1, " << j << ", " << i << ", " << coef[i][j] << std::endl;
    }
  }
  std::cout << "0, 1, 0, 1"  << std::endl;
}
