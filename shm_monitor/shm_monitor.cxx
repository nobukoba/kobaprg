// @(#)root/main:$Id$
// Author: Rene Brun   20/09/96
/////////////////////////////////////////////////////////////////////////
//      Program to convert an HBOOK in the shared memoty into a ROOT file
//                      Author: Rene Brun
//
//  This program is invoked via:
//     s2root shared_memory_name root_file_name  compress tolower
//  if the second parameter root_file_name is missing the name will be
//  automatically generated from the hbook file name. Example:
//       s2root SHMN
//     is identical to
//       s2root SHMN SHMN.root
//  if compress is missing (or = 1)the ROOT file will be compressed
//  if compress = 0 the ROOT file will not be compressed.
//  if tolower is missing (or = 1) ntuple column names are converted to lower case
//                but the first character is converted to upper case.
//  if tolower = 2 same as tolower=1 except that the first character is also
//                convertex to lower case
/////////////////////////////////////////////////////////////////////////
//
// Modified by Nobu on Jan. 28, 2018
// Version 1.0 08-May-2020 Nobu Kobayashi
//
/////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h> /* Nobu 2020/04/14 4:31AM */
#include <sstream> /* Nobu 2020/05/10 4:31AM */
#include <string> /* Nobu 2020/05/10 4:31AM */

#include "Riostream.h"
#include "TFile.h"
#include "TDirectoryFile.h"
#include "TTree.h"
#include "TLeafI.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TGraph.h"
#include "TMath.h"

/* for shm_monitor */
#include "TMemFile.h"
#include "TSystem.h"
#include "THttpServer.h"
#include "TServerSocket.h"
#include "TString.h"
#include "TStyle.h"

//  Define the names of the Fortran common blocks for the different OSs
// Note that with gcc3.4 or above the size of PAWC_SIZE must be the same 
// as in the Fortran definition in hbook.f and zebra
// Also, the arrays must be declared extern like on Windows
#ifndef WIN32
#define PAWC_SIZE 32000000
#  define bigbuf bigbuf_
#  define pawc pawc_
#  define quest quest_
#  define hcbits hcbits_
#  define hcbook hcbook_
#  define rzcl rzcl_
//int pawc[PAWC_SIZE];
//int quest[100];
//int hcbits[37];
//int hcbook[51];
//int rzcl[11];
extern "C" char bigbuf[PAWC_SIZE];
extern "C" int pawc[PAWC_SIZE];
extern "C" int quest[100];
extern "C" int hcbits[37];
extern "C" int hcbook[51];
extern "C" int rzcl[11];
#else
// on windows /pawc/ must have the same length as in libPacklib.a !!
#define PAWC_SIZE 32000000
#  define bigbuf BIGBUF
#  define pawc   PAWC
#  define quest  QUEST
#  define hcbits HCBITS
#  define hcbook HCBOOK
#  define rzcl   RZCL
extern "C" int bigbuf[PAWC_SIZE];
extern "C" int pawc[PAWC_SIZE];
extern "C" int quest[100];
extern "C" int hcbits[37];
extern "C" int hcbook[51];
extern "C" int rzcl[11];
#endif

int *iq, *lq;
float *q;
char idname[128];
int nentries;
char chtitl[128];
int ncx,ncy,nwt,idb;
int lcont, lcid, lcdir;
float xmin,xmax,ymin,ymax;
const Int_t kMIN1 = 7;
const Int_t kMAX1 = 8;

#if defined __linux
//On linux Fortran wants this, so we give to it!
int xargv=0;
int xargc=0;
void MAIN__() {}
#endif

//  Define the names of the Fortran subroutine and functions for the different OSs

#ifndef WIN32
# define hlimit  hlimit_
# define hropen  hropen_
# define hrin    hrin_
# define hnoent  hnoent_
# define hgive   hgive_
# define hgiven  hgiven_
# define hprntu  hprntu_
# define hgnpar  hgnpar_
# define hgnf    hgnf_
# define hgnt    hgnt_
# define rzink   rzink_
# define hdcofl  hdcofl_
# define hmaxim  hmaxim_
# define hminim  hminim_
# define hdelet  hdelet_
# define hntvar2 hntvar2_
# define hbname  hbname_
# define hbnamc  hbnamc_
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

# define hlimap  hlimap_ // nobu added
# define hidall  hidall_ // nobu added
# define hrin2   hrin2_ // nobu added

# define type_of_call
# define DEFCHAR  const char*
# define PASSCHAR(string) string
#else
# define hlimit  HLIMIT
# define hropen  HROPEN
# define hrin    HRIN
# define hnoent  HNOENT
# define hgive   HGIVE
# define hgiven  HGIVEN
# define hprntu  HPRNTU
# define hgnpar  HGNPAR
# define hgnf    HGNF
# define hgnt    HGNT
# define rzink   RZINK
# define hdcofl  HDCOFL
# define hmaxim  HMAXIM
# define hminim  HMINIM
# define hdelet  HDELET
# define hntvar2 HNTVAR2
# define hbname  HBNAME
# define hbnamc  HBNAMC
# define hbnam   HBNAM
# define hi      HI
# define hie     HIE
# define hif     HIF
# define hij     HIJ
# define hix     HIX
# define hijxy   HIJXY
# define hije    HIJE
# define hcdir   HCDIR
# define zitoh   ZITOH
# define uhtoc   UHTOC
# define type_of_call  _stdcall
# define DEFCHAR  const char*, const int
# define PASSCHAR(string) string, strlen(string)
#endif

extern "C" void  type_of_call hlimit(const int&);
#ifndef WIN32
extern "C" void  type_of_call hropen(const int&,DEFCHAR,DEFCHAR,DEFCHAR,
                        const int&,const int&,const int,const int,const int);
#else
extern "C" void  type_of_call hropen(const int&,DEFCHAR,DEFCHAR,DEFCHAR,
                        const int&,const int&);
#endif

extern "C" void  type_of_call hrin(const int&,const int&,const int&);
extern "C" void  type_of_call hnoent(const int&,const int&);
#ifndef WIN32
extern "C" void  type_of_call hgive(const int&,DEFCHAR,const int&,const float&,const float&,
   const int&,const float&,const float&,const int&,const int&,const int);
#else
extern "C" void  type_of_call hgive(const int&,DEFCHAR,const int&,const float&,const float&,
   const int&,const float&,const float&,const int&,const int&);
#endif

#ifndef WIN32
extern "C" void  type_of_call hgiven(const int&,DEFCHAR,const int&,DEFCHAR,
   const float&,const float&,const int,const int);
#else
extern "C" void  type_of_call hgiven(const int&,DEFCHAR,const int&,DEFCHAR,
   const float&,const float&);
#endif

#ifndef WIN32
extern "C" void  type_of_call hntvar2(const int&,const int&,DEFCHAR,DEFCHAR,DEFCHAR,int&,int&,int&,int&,int&,const int,const int, const int);
#else
extern "C" void  type_of_call hntvar2(const int&,const int&,DEFCHAR,DEFCHAR,DEFCHAR,int&,int&,int&,int&,int&);
#endif

#ifndef WIN32
extern "C" void  type_of_call hbnam(const int&,DEFCHAR,const int&,DEFCHAR,const int&,const int, const int);
#else
extern "C" void  type_of_call hbnam(const int&,DEFCHAR,const int&,DEFCHAR,const int&);
#endif

extern "C" void  type_of_call hprntu(const int&);
extern "C" void  type_of_call hgnpar(const int&,const char *,const int);
extern "C" void  type_of_call hgnf(const int&,const int&,const float&,const int&);
extern "C" void  type_of_call hgnt(const int&,const int&,const int&);
extern "C" void  type_of_call rzink(const int&,const int&,const char *,const int);
extern "C" void  type_of_call hdcofl();
extern "C" void  type_of_call hmaxim(const int&,const float&);
extern "C" void  type_of_call hminim(const int&,const float&);
extern "C" void  type_of_call hdelet(const int&);
extern "C" void  type_of_call hix(const int&,const int&,const float&);
extern "C" void  type_of_call hijxy(const int&,const int&,const int&,const float&,const float&);

extern "C" void  type_of_call hlimap(const int&,const char*, const int); // Nobu added
extern "C" void  type_of_call hidall(const int*, const int&); // Nobu added
extern "C" void  type_of_call hrin2(const int&,const int&,const int&); // nobu added

#ifndef R__B64BUG
extern "C" float type_of_call hi(const int&,const int&);
extern "C" float type_of_call hie(const int&,const int&);
extern "C" float type_of_call hif(const int&,const int&);
extern "C" float type_of_call hij(const int&,const int&,const int&);
extern "C" float type_of_call hije(const int&,const int&,const int&);
#else
extern "C" double type_of_call hi(const int&,const int&);
extern "C" double type_of_call hie(const int&,const int&);
extern "C" double type_of_call hif(const int&,const int&);
extern "C" double type_of_call hij(const int&,const int&,const int&);
extern "C" double type_of_call hije(const int&,const int&,const int&);
#endif

#ifndef WIN32
extern "C" void  type_of_call hcdir(DEFCHAR,DEFCHAR ,const int,const int);
#else
extern "C" void  type_of_call hcdir(DEFCHAR,DEFCHAR);
#endif

extern "C" void  type_of_call zitoh(const int&,const int&,const int&);
#ifndef WIN32
extern "C" void  type_of_call uhtoc(const int&,const int&,DEFCHAR,int&,const int);
#else
extern "C" void  type_of_call uhtoc(const int&,const int&,DEFCHAR,int&);
#endif

extern void convert_directory(const char*);
extern void convert_1d(Int_t id);
extern void convert_2d(Int_t id);
extern void convert_profile(Int_t id);
extern void convert_cwn(Int_t id);
extern void convert_rwn(Int_t id);

Int_t golower  = 1;
Int_t bufsize  = 64000;
Int_t optcwn = 1;
int main(int argc, char **argv)
{
   //Program to convert an HBOOK in shm into a ROOT file
   if (argc < 2) {
      printf("******Error in invoking shm_monitor\n");
      printf("===>  shm_monitor port [shm_name_list] [compress] [tolower] [lrecl] [bufsize] [optcwn] \n");
      printf("      i.e., shm_monitor 8080 TEST,FRED\n");
      printf("      port for THttpServer\n");
      printf("      shm_name_list should be separated by comma with no space. If no value is given, all shared memories will be read.\n");
      printf("      compress = 1 by default (use 0 for no compression)\n");
      printf("      tolower  = 1 by default (use 0 to keep case of column names)\n");
      printf("      lrecl =0 by default (must be specified if >8092)\n");
      printf("      bufsize = 8000 by default (branch buffer size)\n");
      printf("      for cwn ntuple only: optcwn  = 1 (default)  1-byte int -> char, 2-byte int -> short, (use 0 to keep 4-byte int) \n");
      return 1;
   }
   lq = &pawc[9];
   iq = &pawc[17];
   void *qq = iq;
   q = (float*)qq;

   Int_t compress = 1;
   int ier=0, record_size=0;
   const char* shm_names = "";
   Int_t  port = 8080;
   
   if (argc > 7) {
      optcwn = atoi(argv[7]);
   }
   if (argc > 6) {
      bufsize = atoi(argv[6]);
   }
   if (argc > 5) {
      record_size = atoi(argv[5]);
   }
   if (argc > 4) {
      golower = atoi(argv[4]);
   }
   if (argc > 3) {
      compress = atoi(argv[3]);
   }
   if (argc > 2) {
     shm_names = argv[2];
   }
   port = atoi(argv[1]);

#if defined(_HIUX_SOURCE) && !defined(__GNUC__)
   hf_fint((char *)NULL);
#endif


   int pawc_size = PAWC_SIZE;
   hlimit(pawc_size);

   int lun = 10;
   /* c/o Nobu 2018/01/27 23:40:23 -->
#ifndef WIN32
   hropen(lun,PASSCHAR("example"),PASSCHAR(file_in),PASSCHAR("px"),record_size,ier,7,strlen(file_in),2);
#else
   hropen(lun,PASSCHAR("example"),PASSCHAR(file_in),PASSCHAR("px"),record_size,ier);
#endif
--> End */

   TString str_shm_names = shm_names;
   Int_t all_read_flag = 0;
   if (str_shm_names.Length() == 0) {
     all_read_flag = 1;
   }
   TList shm_name_list;
   if (all_read_flag == 0){
     while(1){
       Int_t pos = str_shm_names.First(',');
       if(pos < 0){break;}
       TString substr = str_shm_names(0,pos);
       shm_name_list.Add(new TObjString(substr.Data()));
       str_shm_names=str_shm_names(pos+1,str_shm_names.Length());
     }
     shm_name_list.Add(new TObjString(str_shm_names.Data()));

     TIter next(&shm_name_list);
     TObjString * ostr;
     std::cout << "List of shared memory names:" << std::endl;
     while ((ostr = (TObjString*)next())){
       std::cout << "  " <<  ostr->GetString().Data() << std::endl;
     }
     std::cout << std::endl;
   }

   // http server with port, use jobname as top-folder name
   TString thttpserver_str;
   if (all_read_flag == 1) {
     thttpserver_str = Form("http:%d?top=job_all_pid%d_at_%s", port, gSystem->GetPid(), gSystem->HostName());
   }else{
     thttpserver_str = Form("http:%d?top=job_pid%d_at_%s", port, gSystem->GetPid(), gSystem->HostName());
   }
   
   std::cout << "Check availability of the port: " << port << std::endl;
   TServerSocket *ssocket = new TServerSocket(port);
   if(!ssocket->IsValid()){
     std::cout << "TServerSocket::GetErrorCode(): " << ssocket->GetErrorCode() << std::endl;
     std::cout << "Probably port: " << port << " is already in use." << std::endl;
     std::cout << "Exit."<< std::endl;
     delete ssocket;
     return 1;
   }else{
     std::cout << "The port is free." << std::endl;
     delete ssocket;
   }
   
   THttpServer* serv = new THttpServer(thttpserver_str.Data());
   // when read-only mode disabled one could execute object methods like TTree::Draw()
   serv->SetReadOnly(kFALSE);
   
   TMemFile *transient = 0;
   TList file_list;
   TList file_name_list;
   TObjString * ostr2;
   while (1){
     if(all_read_flag == 1){
       shm_name_list.Delete();
       file_name_list.Delete();
       TString cmd = "ipcs -m | perl -alne 'if($.>3){@arr = @F[0] =~ /.{2}/g; foreach(@arr){$_ =~ s/00//;}printf(\"%s \",pack(\"H*\", @arr[4].@arr[3].@arr[2].@arr[1]));}'";
       TString cmd_out = gSystem->GetFromPipe(cmd.Data());
       std::stringstream ss(cmd_out.Data());
       std::string std_str;
       while (ss >> std_str){
	 shm_name_list.Add(new TObjString(std_str.c_str()));
       }
     }
     TIter next2(&shm_name_list);
     while ((ostr2 = (TObjString*)next2())){
       TString shm_name = ostr2->GetString();
       hlimap(0,shm_name.Data(),shm_name.Length());
       hrin2(0,9999,0);
       hdelet(0);
       if (quest[0]) {
	 printf("Warning: cannot open the shared memory: %s\n",shm_name.Data());
	 continue;
       }
       TString filename = Form("%s",shm_name.Data());
       if(all_read_flag == 1){
	 TString cmd = Form("ipcs -m | grep `perl -e '$str=sprintf(\"%%-4s\",\"%s\");@arr=$str=~/.{1}/g;printf(\"0x%%s\",unpack(\"H*\",@arr[3].@arr[2].@arr[1].@arr[0]));'` | perl -anle '{printf(\"%%s\", @F[2])}'", shm_name.Data());
	 TString cmd_out = gSystem->GetFromPipe(cmd.Data());
	 filename = Form("%s_%s",shm_name.Data(),cmd_out.Data());
       }
       file_name_list.Add(new TObjString(filename.Data()));
       TMemFile * tmp = (TMemFile *)file_list.FindObject(filename.Data());
       if (tmp) {
	 file_list.Remove(tmp);
	 delete tmp;
       }
       transient = new TMemFile(filename.Data(),"RECREATE",filename.Data());
       if (!transient) {
	 printf("Error: can't open the TMemFile: %s \n",filename.Data());
	 return 1;
       }
       file_list.Add(transient);
       convert_directory("//example");
       transient->Write();
       if (gSystem->ProcessEvents()) {
	 printf("Error: gSystem->ProcessEvents() is null\n");
	 return 1;
       }
       transient->ls();
     }
     TIter next3(&file_list);
     while ((transient = (TMemFile*)next3())){
       if (!file_name_list.FindObject(transient->GetName())) {
	 file_list.Remove(transient);
	 delete transient;
       }
     }
     gSystem->Sleep(1000); /* Sleep for 1000 ms */ 
   }
   return 0;
}

//____________________________________________________________________________
void convert_directory(const char *dir)
{
   //convert a directory
  /* Nobu c/o 2020/04/29
    printf(" Converting directory %s\n",dir); */
     
/* c/o Nobu 2018/01/27 23:13:15 -->
   Int_t id;
//  Int_t nastycase=0;
//  Int_t nastyprint=0;
//  Int_t idold = 0;
   for (Int_t key=1;key<1000000;key++) {
      int z0 = 0;
      rzink(key,z0,"S",1);
      printf("key %d, quest[0] %d\n",key, quest[0]);
      if (quest[0]) break;
      if (quest[13] & 8) {
         continue;
//        if (!nastyprint) {
//           printf("Found nasty Hbook case!! You had an Hbook error message\n");
//           printf(" when creating the file (too many records)\n");
//           printf(" Hbook file should have been created with a bigger LRECL\n");
//           printf(" ROOT will try to recover\n");
//           nastyprint = 1;
//        }
//        nastycase  = 1;
      }
      id = quest[20];
//     if (id == idold && nastycase) continue;
//     nastycase  = 0;
//     idold = id;
--> End */

   Int_t idvec[1000000];
   Int_t noh;
   hrin2(0,9999,0);
   hidall(idvec,noh);
   hdelet(0);
   for (Int_t i=0;i<noh;i++) {
     Int_t id = idvec[i];
      int i999 = 999;
      hrin2(id,i999,0);
      if (quest[0]) {
         printf("Error cannot read ID = %d\n",id);
         //break;
      }
      hdcofl();
      lcid  = hcbook[10];
      lcont = lq[lcid-1];
      if (hcbits[3]) {
         if (iq[lcid-2] == 2) convert_rwn(id);
         else                 convert_cwn(id);
         hdelet(id);
         continue;
      }
      if (hcbits[0] && hcbits[7]) {
         convert_profile(id);
         hdelet(id);
         continue;
      }
      if (hcbits[0]) {
         convert_1d(id);
         hdelet(id);
         continue;
      }
      if (hcbits[1] || hcbits[2]) {
         convert_2d(id);
         hdelet(id);
      }
   }

// converting subdirectories of this directory
   const Int_t kKLS = 26;
   const Int_t kKNSD = 23;
   lcdir = rzcl[2];
   Int_t ls = iq[lcdir+kKLS];
   Int_t ndir = iq[lcdir+kKNSD];
   Int_t nch=16;
   Int_t ihdir[4];
   Int_t ncw = 4;
   TDirectory *cursav = gDirectory;
   Int_t i;
   char chdir[17];
   char hbookdir[17];
   for (Int_t k=0;k<ndir;k++) {
      lcdir = rzcl[2];
      zitoh(iq[lcdir+ls+7*k],ihdir[0],ncw);
      for (i=0;i<17;i++) chdir[i] = 0;
#ifndef WIN32
      uhtoc(ihdir[0],ncw,chdir,nch ,16);
#else
      uhtoc(ihdir[0],ncw,chdir,16,nch);
#endif
      //do not process directory names containing a slash
      if (strchr(chdir,'/')) {
         printf("Sorry cannot convert directory name %s because it contains a slash\n",chdir);
         continue;
      }   
      strlcpy(hbookdir,chdir,17);
      for (i=16;i>0;i--) {
         if (chdir[i] == 0) continue;
         if (chdir[i] != ' ') break;
         chdir[i] = 0;
      }
#ifndef WIN32
      hcdir(PASSCHAR(hbookdir),PASSCHAR(" "),16,1);
#else
      hcdir(PASSCHAR(hbookdir),PASSCHAR(" "));
#endif
      TDirectoryFile *newdir = new TDirectoryFile(chdir,chdir);
      newdir->cd();
      convert_directory(chdir);
#ifndef WIN32
      hcdir(PASSCHAR("\\"),PASSCHAR(" "),1,1);
#else
      hcdir(PASSCHAR("\\"),PASSCHAR(" "));
#endif
   /* To avoid memory leak, newdir may NOT be written and deleted here */
   /*   newdir->Write(); */
      cursav->cd();
   }
}

//____________________________________________________________________________
void convert_1d(Int_t id)
{
   //convert 1d histogram
   if (id > 0) snprintf(idname,128,"h%d",id);
   else        snprintf(idname,128,"h_%d",-id);
   hnoent(id,nentries);
#ifndef WIN32
   hgive(id,chtitl,ncx,xmin,xmax,ncy,ymin,ymax,nwt,idb,80);
#else
   hgive(id,chtitl,80,ncx,xmin,xmax,ncy,ymin,ymax,nwt,idb);
#endif
   chtitl[4*nwt] = 0;
   TH1F *h1;
   Int_t i;
   /* Nobu added May 9, 2020*/
   TString idname_title = Form("%s; %s",idname,chtitl);
   while (idname_title.EndsWith(" ")){
     idname_title.Remove(TString::kTrailing, ' ');
   }
   if (hcbits[5]) {
      Int_t lbins = lq[lcid-2];
      Double_t *xbins = new Double_t[ncx+1];
      for (i=0;i<=ncx;i++) xbins[i] = q[lbins+i+1];
      /* h1 = new TH1F(idname,chtitl,ncx,xbins); */
      h1 = new TH1F(idname_title.Data(),chtitl,ncx,xbins);
      delete [] xbins;
   } else {
     /* h1 = new TH1F(idname,chtitl,ncx,xmin,xmax); */
      h1 = new TH1F(idname_title.Data(),chtitl,ncx,xmin,xmax);
   }
   if (hcbits[8]) h1->Sumw2();
   TGraph *gr = 0;
   if (hcbits[11]) {
      gr = new TGraph(ncx);
      h1->GetListOfFunctions()->Add(gr);
   }

   /*Float_t x,yx;*/
   Float_t x;
   for (i=0;i<=ncx+1;i++) {
      x = h1->GetBinCenter(i);
      /*For ROOT v6, the error calculation is wrong.
	2020/05/01 Nobu
	yx = hi(id,i);
	h1->Fill(x,yx); */
      h1->SetBinContent(i,hi(id,i));
      if (hcbits[8]) h1->SetBinError(i,hie(id,i));
      if (gr && i>0 && i<=ncx) gr->SetPoint(i,x,hif(id,i));
   }
   Float_t yymin, yymax;
   if (hcbits[19]) {
      yymax = q[lcid+kMAX1];
      h1->SetMaximum(yymax);
   }
   if (hcbits[20]) {
      yymin = q[lcid+kMIN1];
      h1->SetMinimum(yymin);
   }
   h1->SetEntries(nentries);
   /* To avoid memory leak, h1 should NOT be written and deleted here */
   /* h1->Write(); */
   /* delete h1;*/
}

//____________________________________________________________________________
void convert_2d(Int_t id)
{
   //convert 2d histogram
   if (id > 0) snprintf(idname,128,"h%d",id);
   else        snprintf(idname,128,"h_%d",-id);
   hnoent(id,nentries);
#ifndef WIN32
   hgive(id,chtitl,ncx,xmin,xmax,ncy,ymin,ymax,nwt,idb,80);
#else
   hgive(id,chtitl,80,ncx,xmin,xmax,ncy,ymin,ymax,nwt,idb);
#endif
   chtitl[4*nwt] = 0;
   /* Nobu added May 9, 2020*/
   /* TH2F *h2 = new TH2F(idname,chtitl,ncx,xmin,xmax,ncy,ymin,ymax); */
   TString idname_title = Form("%s; %s",idname,chtitl);
   while (idname_title.EndsWith(" ")){
     idname_title.Remove(TString::kTrailing, ' ');
   }
   TH2F *h2 = new TH2F(idname_title.Data(),chtitl,ncx,xmin,xmax,ncy,ymin,ymax);
   Float_t offsetx = 0.5*(xmax-xmin)/ncx;
   Float_t offsety = 0.5*(ymax-ymin)/ncy;
   Int_t lw = lq[lcont];
   if (lw) h2->Sumw2();

   Float_t x = 0.0, y = 0.0;
   for (Int_t j=0;j<=ncy+1;j++) {
      for (Int_t i=0;i<=ncx+1;i++) {
      /*For ROOT v6, the error calculation is wrong.
	2020/05/01 Nobu
         hijxy(id,i,j,x,y);
         h2->Fill(x+offsetx,y+offsety,hij(id,i,j));*/
         h2->SetBinContent(i,j,hij(id,i,j));
         if (lw) {
            Double_t err2 = hije(id,i,j);
            h2->SetCellError(i,j,err2);
         }
      }
   }
   h2->SetEntries(nentries);
   /* Nobu added May 9, 2020*/
   h2->SetOption("colz");
   /* To avoid memory leak, h2 should NOT be written and deleted here */
   /* h2->Write();*/
   /* delete h2; */
}

//____________________________________________________________________________
void convert_profile(Int_t id)
{
// the following structure is used in Hbook
//    lcid points to the profile in array iq
//    lcont = lq(lcid-1)
//    lw    = lq(lcont)
//    ln    = lq(lw)
//      if option S jbyt(iq(lw),1,2) = 1
//      if option I jbyt(iq(lw),1,2) = 2

   if (id > 0) snprintf(idname,128,"h%d",id);
   else        snprintf(idname,128,"h_%d",-id);
   hnoent(id,nentries);
   Int_t lw = lq[lcont];
   Int_t ln = lq[lw];
#ifndef WIN32
   hgive(id,chtitl,ncx,xmin,xmax,ncy,ymin,ymax,nwt,idb,80);
#else
   hgive(id,chtitl,80,ncx,xmin,xmax,ncy,ymin,ymax,nwt,idb);
#endif
   chtitl[4*nwt] = 0;
   const char *option= " ";
   if (iq[lw] == 1) option = "S";
   if (iq[lw] == 2) option = "I";
   TProfile *p = new TProfile(idname,chtitl,ncx,xmin,xmax,ymin,ymax,option);

   const Int_t kCON1 = 9;
   Int_t i;
   for (i=1;i<=ncx;i++) {
      Int_t n = Int_t(q[ln+i]);
      p->SetBinEntries(i,n);
      Float_t content = q[lcont+kCON1+i];
      Float_t error   = TMath::Sqrt(q[lw+i]);
      p->SetBinContent(i,content);
      p->SetBinError(i,error);
   }
   p->SetEntries(nentries);
   p->Write();
   delete p;
}

//____________________________________________________________________________
void convert_rwn(Int_t id)
{
   //convert row wise ntuple
   const int kNchar=9;
   int nvar;
   int ier=0;
   int i,j;
   char *chtag_out;
   float *x;
   float rmin[1000], rmax[1000];

   if (id > 0) snprintf(idname,128,"h%d",id);
   else        snprintf(idname,128,"h_%d",-id);
   hnoent(id,nentries);
   printf(" Converting RWN with ID= %d, nentries = %d\n",id,nentries);
   nvar=0;
#ifndef WIN32
   hgiven(id,chtitl,nvar,PASSCHAR(""),rmin[0],rmax[0],80,0);
#else
   hgiven(id,chtitl,80,nvar,PASSCHAR(""),rmin[0],rmax[0]);
#endif

   chtag_out = new char[nvar*kNchar+1];
   x = new float[nvar];

   chtag_out[nvar*kNchar]=0;
   for (i=0;i<80;i++)chtitl[i]=0;
#ifndef WIN32
   hgiven(id,chtitl,nvar,chtag_out,rmin[0],rmax[0],80,kNchar);
#else
   hgiven(id,chtitl,80,nvar,chtag_out,kNchar,rmin[0],rmax[0]);
#endif
   hgnpar(id,"?",1);
   char *name = chtag_out;
   for (i=80;i>0;i--) {if (chtitl[i] == ' ') chtitl[i] = 0; }
   TTree *tree = new TTree(idname,chtitl);
   Int_t first,last;
   for(i=0; i<nvar;i++) {
      name[kNchar-1] = 0;
      first = last = 0;
      // suppress traling blanks
      for (j=kNchar-2;j>0;j--) {
         if(golower) name[j] = tolower(name[j]);
         if (name[j] == ' ' && last == 0) name[j] = 0;
         else last = j;
      }
      if (golower == 2) name[0] = tolower(name[0]);

      // suppress heading blanks
      for (j=0;j<kNchar;j++) {
         if (name[j] != ' ') break;
         first = j+1;
      }
      tree->Branch(&name[first],&x[i],&name[first],bufsize);
      name += kNchar;
   }
   for(i=1;i<=nentries;i++) {
      hgnf(id,i,x[0],ier);
      tree->Fill();
   }
   tree->Write();
   delete tree;
   delete [] x;
   delete [] chtag_out;
}

//____________________________________________________________________________
void convert_cwn(Int_t id)
{
   //convert column wise ntuple
   const int kNchar=9;
   int nvar;
   int ier=0;
   int i,j;
   int nsub,itype,isize,ielem;
   char *chtag_out;
   float *x;
   float rmin[1000], rmax[1000];

   if (id > 0) snprintf(idname,128,"h%d",id);
   else        snprintf(idname,128,"h_%d",-id);
   hnoent(id,nentries);
   printf(" Converting CWN with ID= %d, nentries = %d\n",id,nentries);
   nvar=0;
#ifndef WIN32
   hgiven(id,chtitl,nvar,PASSCHAR(""),rmin[0],rmax[0],80,0);
#else
   hgiven(id,chtitl,80,nvar,PASSCHAR(""),rmin[0],rmax[0]);
#endif


   chtag_out = new char[nvar*kNchar+1];
   Int_t *charflag = new Int_t[nvar];
   Int_t *lenchar  = new Int_t[nvar];
   Int_t *boolflag = new Int_t[nvar];
   Int_t *lenbool  = new Int_t[nvar];
   UChar_t *boolarr = new UChar_t[10000];
   x = new float[nvar];

   chtag_out[nvar*kNchar]=0;
   for (i=0;i<80;i++)chtitl[i]=0;
#ifndef WIN32
   hgiven(id,chtitl,nvar,chtag_out,rmin[0],rmax[0],80,kNchar);
#else
   hgiven(id,chtitl,80,nvar,chtag_out,kNchar,rmin[0],rmax[0]);
#endif
   Long_t add= (Long_t)&bigbuf[0];
#ifndef WIN32
   hbnam(id,PASSCHAR(" "),add,PASSCHAR("$CLEAR"),0,1,6);
#else
   hbnam(id,PASSCHAR(" "),add,PASSCHAR("$CLEAR"),0);
#endif

   Int_t bufpos = 0;
   Int_t isachar = 0;
   Int_t isabool = 0;
   char fullname[1024];
   char name[512];
   char block[512];
   char oldblock[512];
   Int_t nbits = 0;
   strlcpy(oldblock,"OLDBLOCK",512);
   Int_t oldischar = -1;
   for (i=80;i>0;i--) {if (chtitl[i] == ' ') chtitl[i] = 0; }
   TTree *tree = new TTree(idname,chtitl);
   for(i=0; i<nvar;i++) {
      memset(name,' ',sizeof(name));
      name[sizeof(name)-1] = 0;
      memset(block,' ',sizeof(block));
      block[sizeof(block)-1] = 0;
      memset(fullname,' ',sizeof(fullname));
      fullname[sizeof(fullname)-1]=0;
#ifndef WIN32
      hntvar2(id,i+1,PASSCHAR(name),PASSCHAR(fullname),PASSCHAR(block),nsub,itype,isize,nbits,ielem,512,1024,512);
#else
      hntvar2(id,i+1,PASSCHAR(name),PASSCHAR(fullname),PASSCHAR(block),nsub,itype,isize,nbits,ielem);
#endif

      for (j=510;j>0;j--) {
         if(golower) name[j] = tolower(name[j]);
         if (name[j] == ' ') name[j] = 0;
      }
      if (golower == 2) name[0] = tolower(name[0]);

      for (j=1022;j>0;j--) {
         if(golower && fullname[j-1] != '[') fullname[j] = tolower(fullname[j]);
         // convert also character after [, if golower == 2
         if (golower == 2) fullname[j] = tolower(fullname[j]);
         if (fullname[j] == ' ') fullname[j] = 0;
      }
      // convert also first character, if golower == 2
      if (golower == 2) fullname[0] = tolower(fullname[0]);
      for (j=510;j>0;j--) {
         if (block[j] == ' ') block[j] = 0;
         else break;
      }
      if (itype == 1) {
         if( isize == 4 )     strlcat(fullname,"/F",1024);
         else if( isize == 8) strlcat(fullname,"/D",1024);
      }


      // add support for 1-byte (Char_t) and 2-byte (Short_t) integers
      // Int_t nBytesUsed = 4; // default for integers

      if( itype == 2 ) {
         if( optcwn == 1 ) {
            if( nbits > 16 ) {
               strlcat(fullname,"/I",1024);
            } else {
               if( nbits > 8 ) {
                  strlcat(fullname,"/S",1024);
                  //nBytesUsed = 2;
               } else {
                  strlcat(fullname,"/B",1024);
                  //nBytesUsed = 1;
               }
            }
         } else {
            strlcat(fullname,"/I",1024);
         }
      }

      // add support for 1-byte (UChar_t) and 2-byte (UShort_t) integers
      if ( itype == 3 ) {
         if(  optcwn == 1 ) {
            if( nbits > 16) {
               strlcat(fullname,"/i",1024);
            } else {
               if( nbits > 8 ) {
                  strlcat(fullname,"/s",1024);
                  //nBytesUsed = 2;
               } else {
                  strlcat(fullname,"/b",1024);
                  //nBytesUsed = 1;
               }
            }
         } else {
            strlcat(fullname,"/i",1024);
         }
      }




//     if (itype == 4) strlcat(fullname,"/i",1024);
      if (itype == 4) strlcat(fullname,"/b",1024);
      if (itype == 5) strlcat(fullname,"/C",1024);
      printf("Creating branch:%s, block:%s, fullname:%s, nsub=%d, itype=%d, isize=%d, ielem=%d\n",name,block,fullname,nsub,itype,isize,ielem);
      Int_t ischar;
      if (itype == 5) ischar = 1;
      else            ischar = 0;
      if (ischar != oldischar || strcmp(oldblock,block) != 0) {
         strlcpy(oldblock,block,512);
         oldischar = ischar;
         Int_t lblock   = strlen(block);
         add= (Long_t)&bigbuf[bufpos];
#ifndef WIN32
         hbnam(id,PASSCHAR(block),add,PASSCHAR("$SET"),ischar,lblock,4);
#else
         hbnam(id,PASSCHAR(block),add,PASSCHAR("$SET"),ischar);
#endif

      }
      TBranch *branch = tree->Branch(name,(void*)&bigbuf[bufpos],fullname,bufsize);
      boolflag[i] = -10;
      charflag[i] = 0;
      if (itype == 4) {isabool++; boolflag[i] = bufpos; lenbool[i] = ielem;}
      bufpos += isize*ielem;
      if (ischar) {isachar++; charflag[i] = bufpos-1; lenchar[i] = isize*ielem;}
      TObjArray *ll= branch->GetListOfLeaves();
      TLeaf *leaf = (TLeaf*)ll->UncheckedAt(0);
      if (!leaf) continue;
      TLeafI *leafcount = (TLeafI*)leaf->GetLeafCount();
      if (leafcount) {
         if (leafcount->GetMaximum() <= 0) leafcount->SetMaximum(ielem);
      }
   }
   Int_t cf,l;
   for(i=1;i<=nentries;i++) {
      hgnt(id,i,ier);
      if (isabool) { // if column is boolean
         for (j=0;j<nvar;j++) {
            cf = boolflag[j];
            if (cf >-1) {
               for (l=0;l<lenbool[j];l++) {
#ifdef R__BYTESWAP
                  boolarr[l] = (UChar_t)bigbuf[cf+4*l];
#else
                  boolarr[l] = (UChar_t)bigbuf[cf+4*l+3];
#endif
               }
               memcpy(&bigbuf[cf],boolarr,lenbool[j]);
            }
         }
      }
      if (isachar) { // if column is character, set terminator
         for (j=0;j<nvar;j++) {
            cf = charflag[j];
            if (cf) {
               bigbuf[cf] = '\0';
               if (bigbuf[cf-1] != ' ') continue;
               bigbuf[cf-1] = '\0';
               if (bigbuf[cf-2] != ' ') continue;
               bigbuf[cf-2] = '\0';
            }
         }
      }

      // if optimizing cwn ntuple then look up bufpos and adjust integers to be shorts or chars
      if(  optcwn == 1 ) {
         bufpos = 0;
         for(int k=0; k<nvar;k++) {
#ifndef WIN32
            hntvar2(id,k+1,PASSCHAR(name),PASSCHAR(fullname),PASSCHAR(block),nsub,itype,isize,nbits,ielem,32,64,32);
#else
            hntvar2(id,k+1,PASSCHAR(name),PASSCHAR(fullname),PASSCHAR(block),nsub,itype,isize,nbits,ielem);
#endif

            Int_t nBytesUsed = 4; // default for integers

            if ( itype == 2 || itype == 3) {
               if( nbits > 16) {
                    // do nothing for integers of 4 byte
               } else {
                  if( nbits > 8 ) nBytesUsed = 2;
                  else            nBytesUsed = 1;
               }
            }

            if(nBytesUsed == 1) {
               for(Int_t index = 0; index < ielem; index++) {
                  // shift all chars with data to be one after another
                  bigbuf[bufpos + index*nBytesUsed ] =  bigbuf[bufpos + index * isize];
               }
            } else {
               if(nBytesUsed == 2) {
                  for(Int_t index = 0; index < ielem; index++) {
                     // shift all shorts ( 2 chars) with data to be one after another
                     bigbuf[bufpos + index*nBytesUsed ] =  bigbuf[bufpos + index * isize];
                     bigbuf[bufpos + index*nBytesUsed+1 ] =  bigbuf[bufpos + index * isize+1];
                  }
               }
            }
            bufpos += isize*ielem;
            
         }
      }

      tree->Fill();
   }
   tree->Print();
   tree->Write();
   delete tree;
   delete [] x;
   delete [] charflag;
   delete [] lenchar;
   delete [] boolflag;
   delete [] lenbool;
   delete [] boolarr;
   delete [] chtag_out;
}
