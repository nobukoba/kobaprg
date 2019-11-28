void plot_graphs_clear(){
  if (!gPad) {
    std::cout << "There is no gPad." << std::endl;
    return;
  }
  TList *listofpri = gPad->GetListOfPrimitives();
  Int_t ngraphs = 0;
  TGraph *gr;
  while((gr = (TGraph*)listofpri->FindObject(Form("Graph_%d",ngraphs)))){
    ngraphs++;
    gr->Delete();
  }
  gPad->Update();
  return;
}
