#include <stdio.h>
int *iq, *lq;
float *q;
int lcont, lcid;

#define PAWC_SIZE 2000000
#define pawc pawc_
#define quest quest_
#define hcbits hcbits_
#define hcbook hcbook_
#define rzcl rzcl_
extern "C" int pawc_[PAWC_SIZE];
extern "C" int quest_[100];
extern "C" int hcbits_[37];
extern "C" int hcbook_[51];
extern "C" int rzcl_[11];

/*  Define the names of the Fortran subroutine and functions for the different OSs */
/*  Define the names of the Fortran subroutine and functions for the different OSs*/
# define hlimit  hlimit_
# define hropen  hropen_
# define hrin    hrin_
# define hnoent  hnoent_
# define hgive   hgive_
# define hgiven  hgiven_
# define hgnpar  hgnpar_
# define hgnf    hgnf_
# define hgnt    hgnt_
# define rzink   rzink_
# define hdcofl  hdcofl_
# define hdelet  hdelet_
# define hntvar2 hntvar2_
# define hbnam   hbnam_
# define hi      hi_
# define hie     hie_
# define hif     hif_
# define hij     hij_
# define hix     hix_
# define hijxy   hijxy_
# define hije    hije_
# define hcdir   hcdir_
# define zitoh   zitoh_
# define uhtoc   uhtoc_
# define hlimap  hlimap_ /* nobu added */
# define hidall  hidall_ /* nobu added */
# define hrin    hrin_   /* nobu added */
extern "C" void hlimit_(const int&);
extern "C" void hropen_(const int&,const char*,const char*,const char*,const int&,const int&,const int,const int,const int);
extern "C" void hrin_(const int&,const int&,const int&);
extern "C" void hnoent_(const int&,const int&);
extern "C" void hgive_(const int&,const char*,const int&,const float&,const float&,const int&,const float&,const float&,const int&,const int&,const int);
extern "C" void hgiven_(const int&,const char*,const int&,const char*,const float&,const float&,const int,const int);
extern "C" void hntvar2_(const int&,const int&,const char*,const char*,const char*,int&,int&,int&,int&,int&,const int,const int, const int);
extern "C" void hbnam_(const int&,const char*,const int&,const char*,const int&,const int, const int);
extern "C" void hprntu_(const int&);
extern "C" void hgnpar_(const int&,const char *,const int);
extern "C" void hgnf_(const int&,const int&,const float&,const int&);
extern "C" void hgnt_(const int&,const int&,const int&);
extern "C" void rzink_(const int&,const int&,const char *,const int);
extern "C" void hdcofl_();
extern "C" void hmaxim_(const int&,const float&);
extern "C" void hminim_(const int&,const float&);
extern "C" void hdelet_(const int&);
extern "C" void hix_(const int&,const int&,const float&);
extern "C" void hijxy_(const int&,const int&,const int&,const float&,const float&);
extern "C" float hi_(const int&,const int&);
extern "C" float hie_(const int&,const int&);
extern "C" float hif_(const int&,const int&);
extern "C" float hij_(const int&,const int&,const int&);
extern "C" float hije_(const int&,const int&,const int&);
extern "C" void hcdir_(const char*,const char* ,const int,const int);
extern "C" void zitoh_(const int&,const int&,const int&);
extern "C" void uhtoc_(const int&,const int&,const char*,int&,const int);
extern "C" void hlimap_(const int&,const char*, const int); /* Nobu added */
extern "C" void hidall_(const int*, const int&);            /* Nobu added */
extern "C" void hrin_(const int&,const int&,const int&);   /* nobu added */
extern "C" void hrend_(const char*,const int);              /* nobu added */

int main () {
  lq = &pawc[9];
  iq = &pawc[17];
  void *qq = iq;
  q = (float*)qq;
  int pawc_size = PAWC_SIZE;
  printf("%d\n",pawc_size);
  hlimit_(pawc_size);
  printf("toy_c 1\n");
  hlimap_(0,"TEST",4);
  printf("toy_c 1\n");
  int idvec[1000000];
  int noh;
  hrin_(0,9999,0);
  hidall_(idvec,noh);
  return 0;
}
