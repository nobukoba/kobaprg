void fit_graph(){
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
  TGraph *gr_pnt;
  while(gr_pnt = (TGraph*)listofpri->FindObject(Form("Graph_%d",ngraphs))){
    std::cout << "Graph_" << ngraphs << " was found." << std::endl;
    ngraphs++;
  }
  if (ngraphs == 0) {
    std::cout << "Graph_* was not found in the current pad." << std::endl;
    return;
  }
  gr_pnt = (TGraph*)listofpri->FindObject(Form("Graph_%d",ngraphs-1));  

  Int_t degree_of_polN = gr_pnt->GetN()-1;
  TF1 *fit_func = new TF1("fit_func", Form("pol%d",degree_of_polN), 0., 1.);
  gr_pnt->Fit(fit_func);
  fit_func->Draw("C,SAME");
  if(degree_of_polN == 1){
    Double_t b = fit_func->GetParameter(0);
    Double_t a = fit_func->GetParameter(1);
    std::cout << "y = "   << a
	      << "*x + (" << b
	      << ")" << std::endl;
    std::cout << "x = "   << 1/a
	      << "*y + (" << -(b/a)
	      << ")" << std::endl;
  }
  sel_pad->Modified();
  sel_pad->Update();
  fit_func->Delete();
  
  return;
}
