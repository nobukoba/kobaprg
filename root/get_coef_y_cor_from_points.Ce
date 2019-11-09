void get_coef_y_cor_from_points(){
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
  Int_t degree_of_polN  = 0;
  TGraph *gr_pnt;
  while(gr_pnt = (TGraph*)listofpri->FindObject(Form("Graph_%d",ngraphs))){
    if (ngraphs == 0) {
      degree_of_polN = gr_pnt->GetN()-1;
    }else{
      if (degree_of_polN != (gr_pnt->GetN()-1)){
	std::cout << "Number of points of Graph_" << ngraphs << " is not the same as the previous Graph. Exit." << std::endl;
	return;
      }
    }
    ngraphs++;
  }
  if (ngraphs == 0) {
    std::cout << "Graph_* was not found in the current pad." << std::endl;
    return;
  }

  if (ngraphs == 1) {
    TGraph *gr = (TGraph *)listofpri->FindObject("Graph_0");
    TF1 *fit_func = new TF1("fit_func", Form("pol%d",degree_of_polN), 0., 1.);
    gr->Fit(fit_func);
    std::cout << "1, 0, 1, 1"  << std::endl;
    for (Int_t i = 1; i < degree_of_polN+1; i++) {
      std::cout << "1, " << i << ", 0, " << -fit_func->GetParameter(i) << std::endl;
    }
    std::cout << "0, 1, 0, 1"  << std::endl;
    delete fit_func;
    return;
  }

  TMatrixD fit_pars(degree_of_polN+1, ngraphs);
  TMatrixD m((degree_of_polN+1)*ngraphs, (degree_of_polN+1)*ngraphs);
  Int_t k = 0;
  for (Int_t i = 0; i < degree_of_polN+1; i++) {
    for (Int_t j = 0; j < ngraphs; j++) {
      TGraph *gr = (TGraph *)listofpri->FindObject(Form("Graph_%d",j));
      TF1 *fit_func = new TF1("fit_func", Form("pol%d",degree_of_polN), 0., 1.);
      gr->Fit(fit_func);
      fit_pars[i][j] = fit_func->GetParameter(0);
      std::cout << "i, j, fit_pars: " << i << ", " << j << ", " << fit_pars[i][j] << std::endl;
      Double_t x, y;
      gr->GetPoint(i, x, y);
      for (Int_t i2 = 0; i2 < degree_of_polN+1; i2++) {
	for (Int_t j2 = 0; j2 < ngraphs; j2++) {
	  m[k][i2*ngraphs+j2] = pow(x,i2)*pow(y,j2);
	  std::cout << "i2, j2, k, i2*ngraphs+j2, m: " << i2 << ", " << j2 << ", " << k << ", " << i2*ngraphs+j2  << ", " << m[k][i2*ngraphs+j2] << std::endl;
	}
      }
      k++;
      delete fit_func;
    }
  }
  
  TMatrixD m_inv((degree_of_polN+1)*ngraphs, (degree_of_polN+1)*ngraphs);
  m_inv = m.Invert();
  
  TMatrixD coef(degree_of_polN+1, ngraphs);
  k = 0;
  for (Int_t i = 0; i < degree_of_polN+1; i++) {
    for (Int_t j = 0; j < ngraphs; j++) {
      for (Int_t i2 = 0; i2 < degree_of_polN+1; i2++) {
	for (Int_t j2 = 0; j2 < ngraphs; j2++) {
	  coef[i][j] += m_inv[k][i2*ngraphs+j2] * fit_pars[i2][j2];
	}
      }
      k++;
      std::cout << "i, j, coef: " << i << ", " << j << ", " << coef[i][j] << std::endl;
    }
  }
  
  for (Int_t i = 0; i < degree_of_polN+1; i++) {
    for (Int_t j = 0; j < ngraphs; j++) {
      std::cout << "1, " << i << ", " << j << ", " << coef[i][j] << std::endl;
    }
  }
  std::cout << "0, 1, 0, 1"  << std::endl;
}
