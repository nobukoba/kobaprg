void get_coef_from_graphs(){
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

  TGraph gr();
  
  for (Int_t i = 0; i < number_of_peaks; i++) {
    pfy->Fit(fit_func);
    cutg->SetName(Form("CUTG_%d",i));
  }


  //TMatrixD fit_pars(number_of_peaks, degree_of_polN+1);
  //for (Int_t i = 0; i < number_of_peaks; i++) {
  //  TProfile *pfy      = (TProfile *)gROOT->FindObject(Form("pfy_%d",i));
  //  TF1      *tf1_ptr  = pfy->GetFunction("fit_func");
  //  for (Int_t j = 0; j < degree_of_polN+1; j++) {
  //    fit_pars[i][j] = tf1_ptr->GetParameter(j);
  //    std::cout << "i, j, fit_pars: " << i << ", " << j << ", " << fit_pars[i][j] << std::endl;
  //  }
  //}
  //
  //TMatrixD m(number_of_peaks, number_of_peaks);
  //for (Int_t i = 0; i < number_of_peaks; i++) {
  //  for (Int_t j = 0; j < number_of_peaks; j++) {
  //    m[i][j] = pow(fit_pars[i][0],j);
  //    std::cout << "i, j, m: " << i << ", " << j << ", " << m[i][j] << std::endl;
  //  }      
  //}
  //
  //TMatrixD m_inv(number_of_peaks, number_of_peaks);
  //m_inv = m.Invert();
  //
  //TMatrixD coef(number_of_peaks, degree_of_polN+1);
  //for (Int_t j = 0; j < degree_of_polN+1; j++) {
  //  for (Int_t i = 0; i < number_of_peaks; i++) {
  //    coef[i][j] = 0.;
  //    for (Int_t k = 0; k < number_of_peaks; k++) {
  //	coef[i][j] += m_inv[i][k] * -fit_pars[k][j];
  //    }
  //    std::cout << "i, j, coef: " << i << ", " << j << ", " << coef[i][j] << std::endl;
  //  }
  //}
  //for (Int_t i = 0; i < number_of_peaks; i++) {
  //  if (i == 1) {
  //    coef[i][0] = 1.;
  //  }else{
  //    coef[i][0] = 0.;
  //  }
  //}
  //for (Int_t j = 0; j < degree_of_polN+1; j++) {
  //  for (Int_t i = 0; i < number_of_peaks; i++) {
  //    std::cout << "0, " << i << ", " << j << ", " << coef[i][j] << std::endl;
  //  }
  //}
  //std::cout << "1, 0, 1, 1"  << std::endl;
}
