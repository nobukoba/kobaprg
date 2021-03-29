#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#include "Rtypes.h"
#include "TROOT.h"
#include "TFile.h"
#include "TRandom.h"
#include "TH1.h"
#include "TH2.h"
#include "TObjArray.h"
#include "TObject.h"
#include "TKey.h"
#include "TSystem.h"
#include "TCutG.h"
#include "TTree.h"
#include "GEBSort.h"

extern PARS Pars;

TTree *tree;
Int_t id;

int sup_dgs () {
  tree = new TTree("tree","tree");
  tree->Branch("id",&id,"id/I");
  return (0);

}

int bin_dgs (){
  tree->Fill();
  tree->AutoSave();
  return (0);
}
