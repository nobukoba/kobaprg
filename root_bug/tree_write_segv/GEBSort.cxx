
/* sorter for data from the GEB tab (or a file) */
/* totally universal in its scope */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <signal.h>
#include <time.h>
#include <stddef.h>
#include <zlib.h>

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

#include "TMapFile.h"

#include "GEBSort.h"

PARS Pars;

//#define MAXFLOAT 3.40282347e+38
#define MAXINT   2147483647

int GEBacq (char *ChatFileName)
{
  int sup_dgs ();
  int bin_dgs ();

  Pars.f1 = new TFile (ChatFileName, "RECREATE");
  sup_dgs();
  for (Int_t i =0; i <100000000; i++){
    bin_dgs();
  }
  Pars.f1->Write();
  Pars.f1->Close();
  return (0);  
}

/*----------------------------------------------------*/

int main (int argc, char **argv)
{
  GEBacq ("test.root");
  return (0);
}
