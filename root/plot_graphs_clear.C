void plot_graphs_clear(){
  TCanvas* canvas = gPad->GetCanvas();
  if (canvas == 0) {
    std::cout << "There is no canvas." << std::endl;
    return;
  }
  TVirtualPad *sel_pad = canvas->GetPad(gPad->GetNumber());
  if (sel_pad == 0) {
    std::cout << "There is no selected pad." << std::endl;
    return;
  }
  TList *listofpri = sel_pad->GetListOfPrimitives();
  if (listofpri == 0) {
    std::cout << "The pad includes nothing." << std::endl;
    return;
  }
  Int_t ngraphs = 0;
  TGraph *gr;
  while(gr = (TGraph*)listofpri->FindObject(Form("Graph_%d",ngraphs))){
    ngraphs++;
    gr->Delete();
  }
  canvas->Update();
  return;
}
