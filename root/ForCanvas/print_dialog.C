void print_dialog(){
  gStyle->SetPaperSize(20,26);
  ((TRootCanvas*)gPad->GetCanvas()->GetCanvasImp())->PrintCanvas(); 
  return;
}
