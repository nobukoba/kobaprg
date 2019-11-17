#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TList.h"
#include "TKey.h"
#include "TH1.h"

void count_in_range(TH1 *hist){
  std::cout << hist->GetName() << ": ";
  std::cout << hist->Integral(hist->GetXaxis()->GetFirst(),
                              hist->GetXaxis()->GetLast()) << std::endl;
  return;
}
