#include "TFile.h"


#define TAPE 0
#define NET  1
#define DISK 2
#define GEB  3

#define LONGLEN 8000
#define SHORTLEN 1024
#define RBUFSIZE 500000
#define MAXPAYLOADSIZE 10000
#define STRLEN 256
#define MAXDETPOS 120
#define MAXCRYSTALNO 3
#define RATELEN 16384
#define DTBTEVLEN 1000
#define MAXNOSEG 9
#define MAXDETNO 200
#define NUMSEGS 36
#define MAXSEGNO MAXDETNO*NUMSEGS

#define MAXGEBS 200

#define MAX_GAMMA_RAYS 40
#define GEB_BITS GEB_HEADER_BYTES*8

/* max values for # of bits */

#define M14BITS 0x3fff 
#define M13BITS 0x1fff
#define M12BITS 0x0fff
#define M11BITS 0x07ff
#define M10BITS 0x03ff

/* basic spectra lengths */

#define L14BITS  M14BITS+1
#define L13BITS  M13BITS+1
#define L12BITS  M12BITS+1
#define L11BITS  M11BITS+1
#define L10BITS  M10BITS+1

typedef struct PARS
{
  char ROOTFile[STRLEN];
  int nEvents;
  char ROOTFileOption[STRLEN];
  char GTSortInputFile[STRLEN];
  int UseShareMemFile;
  unsigned int StartMapAddress;
  char ShareMemFile[STRLEN];
  int InputSrc;
  int HaveRootFileName;
  int WeWereSignalled;
  int UseRootFile, SizeShareMemFile;
  int UpdateRootFile;
  char spname[STRLEN];
  int firstEvent;
  int GSudpPort;
  int NumToPrint;
  int DumpEvery;
  TFile *f1;
  TList *wlist;
  long long int curTS;
  long long int dTS;
  long long int nbytes;
  int CurEvNo;
  char pHost[16];
  int grouping;
  int type;
  float CCcal_gain[MAXDETNO+1];
  float CCcal_offset[MAXDETNO+1];
  float SEGcal_gain[MAXDETPOS + 1][MAXCRYSTALNO + 1];
  float SEGcal_offset[MAXDETPOS + 1][MAXCRYSTALNO + 1];
  float timeout;
  float crmat[MAXDETPOS + 1][MAXCRYSTALNO + 1][4][4]; 
  float detpolang[MAXDETPOS+1];
  float beta;
  int GGMAX;
  int modwrite;
  int tsnumwrites;
  float fomlo[MAXNOSEG];
  float fomhi[MAXNOSEG];
  int ndetlimlo;
  int ndetlimhi;
  float beamdir[3];
  int nocrystaltoworldrot;
  int multlo;
  int multhi;
  int RunNo; //E.I. 16Sep19
} PARS;


/* structure for the tracked gamma rays */
/* written to the output with geb ID GEB_TYPE_TRACK==3 */
