#ifndef my_cfortran_h
#define my_cfortran_h

//  Define the names of the Fortran common blocks for the different OSs
// Note that with gcc3.4 or above the size of PAWC_SIZE must be the same 
// as in the Fortran definition in hbook.f and zebra
// Also, the arrays must be declared extern like on Windows
#define PAWC_SIZE 32000000
#define bigbuf bigbuf_
#define pawc pawc_
#define quest quest_
#define hcbits hcbits_
#define hcbook hcbook_
#define rzcl rzcl_
extern "C" char bigbuf[PAWC_SIZE];
extern "C" int pawc[PAWC_SIZE];
extern "C" int quest[100];
extern "C" int hcbits[37];
extern "C" int hcbook[51];
extern "C" int rzcl[11];

#if defined __linux
//On linux Fortran wants this, so we give to it!
//int xargv=0;
//int xargc=0;
//void MAIN__() {}
#endif

//  Define the names of the Fortran subroutine and functions for the different OSs

#define hlimit  hlimit_
#define hropen  hropen_
#define hrin    hrin_
#define hnoent  hnoent_
#define hgive   hgive_
#define hgiven  hgiven_
#define hprntu  hprntu_
#define hgnpar  hgnpar_
#define hgnf    hgnf_
#define hgnt    hgnt_
#define rzink   rzink_
#define hdcofl  hdcofl_
#define hmaxim  hmaxim_
#define hminim  hminim_
#define hdelet  hdelet_
#define hntvar2 hntvar2_
#define hbname  hbname_
#define hbnamc  hbnamc_
#define hbnam   hbnam_
#define hi      hi_
#define hie     hie_
#define hif     hif_
#define hij     hij_
#define hix     hix_
#define hijxy   hijxy_
#define hije    hije_
#define hcdir   hcdir_
#define zitoh   zitoh_
#define uhtoc   uhtoc_
#define hlimap  hlimap_ // nobu added
#define hidall  hidall_ // nobu added
#define hrin2   hrin2_ // nobu added
#define hrend   hrend_ // nobu added

extern "C" void hlimit(const int&);
extern "C" void hropen(const int&,const char*,const char*,const char*,const int&,const int&,const int,const int,const int);
extern "C" void hrin(const int&,const int&,const int&);
extern "C" void hnoent(const int&,const int&);
extern "C" void hgive(const int&,const char*,const int&,const float&,const float&,const int&,const float&,const float&,const int&,const int&,const int);
extern "C" void hgiven(const int&,const char*,const int&,const char*,const float&,const float&,const int,const int);
extern "C" void hntvar2(const int&,const int&,const char*,const char*,const char*,int&,int&,int&,int&,int&,const int,const int, const int);
extern "C" void hbnam(const int&,const char*,const int&,const char*,const int&,const int, const int);
extern "C" void hprntu(const int&);
extern "C" void hgnpar(const int&,const char *,const int);
extern "C" void hgnf(const int&,const int&,const float&,const int&);
extern "C" void hgnt(const int&,const int&,const int&);
extern "C" void rzink(const int&,const int&,const char *,const int);
extern "C" void hdcofl();
extern "C" void hmaxim(const int&,const float&);
extern "C" void hminim(const int&,const float&);
extern "C" void hdelet(const int&);
extern "C" void hix(const int&,const int&,const float&);
extern "C" void hijxy(const int&,const int&,const int&,const float&,const float&);
extern "C" float hi(const int&,const int&);
extern "C" float hie(const int&,const int&);
extern "C" float hif(const int&,const int&);
extern "C" float hij(const int&,const int&,const int&);
extern "C" float hije(const int&,const int&,const int&);
extern "C" void hcdir(const char*,const char* ,const int,const int);
extern "C" void zitoh(const int&,const int&,const int&);
extern "C" void uhtoc(const int&,const int&,const char*,int&,const int);
extern "C" void hlimap(const int&,const char*, const int); // Nobu added
extern "C" void hidall(const int*, const int&); // Nobu added
extern "C" void hrin2(const int&,const int&,const int&); // nobu added
extern "C" void hrend(const char*,const int); // nobu added
#endif
