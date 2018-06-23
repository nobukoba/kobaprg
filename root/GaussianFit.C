#include "TH1.h"
#include "TH2.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TList.h"

void GaussianFit(){
  printf("\n");
  printf("aaa\n");

  //TCanvas* canvas = gPad->GetCanvas();
  //TList* list     = canvas->GetListOfPrimitives();
  //if (list->At(0)==0) {return;}
  //
  //if (strcmp(list->At(0)->GetName(),"Canvas_1_1")==0){
  //  Int_t  cur_pad  = gPad->GetNumber();
  //  TList* selpad_list = ((TVirtualPad*)list->At(cur_pad-1))->GetListOfPrimitives();
  //  if(selpad_list->At(0)){
  //    Int_t    no_pads  = list->GetSize();
  //    Int_t    next_pad = cur_pad + 1;
  //    if (next_pad > no_pads) {next_pad = 1;}
  //    canvas->cd(next_pad);
  //  }
  //}
}
