#include "TROOT.h"
#include "TStyle.h"
#include "TF1.h"
#include "TH1.h"
#include "TLatex.h"
#include <iostream>

void bound(const int ia, const int ib, const int ic,
	   const int is2, const int j2, const int lmom,
	   const int nod, const int nramax, const int jhw,
	   const double cmass, const double vmass,
	   const double wzz, const double wa,
	   const double wr0, double wls,
	   const int isof,
	   double ffr[6603][5], double &vdepth,
	   double &bengy, const double dmat,
	   int &kcheck, const double wrz, double chis[11],
	   const double drx, const double pnloc,
	   const double wr0ls, const double wals) {  
  /*  implicit real*8(a-h,o-z) */
  double drd[5];
  double wfcr[6603], wfsr[6603], wfc[6603];
  /*      real*8 cmass,vmass ,wzz,wa,wr0,wls,ffr,vdepth,bengy,dmat,wrz,
	  1 chis,drx
	  common/so/isof */
  int korec=0;
  for (int i=1; i<=6602; i++) {
    ffr[i][jhw]=0.;
  }
  int niter=0;
  int incr=0;
  double eps7=1.e-6;
  drd[jhw]=drx;
  int lmom1=lmom+1;
  double test=1.0e+20;
  double lmom2=lmom1+1;
  double drz=wrz;
  double fnod=nod;
  double flmom=lmom;
  double flmom1=lmom1;
  double radi=wr0*pow(cmass,0.333333333);
  double radls=wr0ls*pow(cmass,0.333333333);
  double yyyy = cosh(radi/wa);
  double radz=radi;
  double wr=drd[jhw];
  int    nr1=nramax+1;
  for (int i=1; i<=nramax+10; i++) {
    double yyy=(wr-radi)/wa;
    double zzz=(wr-radls)/wals;
    if(yyy>90.) yyy=90.;
    if(zzz>90.) zzz=90.;
    double ex=exp(yyy);
    double exls=exp(zzz);
    int ib1=ib+1;
    switch (ib1) {
    case 1: wfcr[i]=1.0/(1.0+ex);                           break;
    case 2: wfcr[i]=1.0/(1.0+ex);                           break;
    case 3: wfcr[i]=exp(-(wr/wa)*(wr/wa));                  break;
    case 4: wfcr[i]=exp(-wa*wr)/wr;                         break;
    case 5: wfcr[i]=exp(-wa*wr)/(exp(-wr0*wr)-exp(-wa*wr)); break;
    case 6: wfcr[i]=(1.0+yyyy)/(cosh(wr/wa)+yyyy);          break;
    }
    wfsr[i]=exls/(1.0+exls)/(1.0+exls);
    if ((wr-radi)<=0.) {
      wfc[i]=0.7199262*wzz*(3.0-wr*wr/(radi*radi))/radi;
    }else{
      wfc[i]=1.4398523*wzz/wr;
    }
    wr=wr+drd[jhw];
  }
  double ramda=0.0;
 label10:;
  if((ia-1)<0){
    vdepth=bengy+pow(3.1415926*(fnod+0.5*flmom1),2)/
      (0.048228*vmass*pow(radi+drz,2)); // label: 10
    printf("label 10 vdepth: %10.20f\n",vdepth);
  }else{
    while (true){
      bengy=vdepth-pow(3.1415926*(fnod+0.5*flmom1),2)/
	(0.048228*vmass*radz*radz);
      if((bengy-eps7)<=0.) {
	break;
      }else{
	radz=radz+drz;
	incr=incr+1;
	if((incr-20)<=0) {
	  continue;
	}else{
	  kcheck=11;
	  std::cout << "0 subroutine bound  kcheck="
		    << kcheck << " ramda=" << ramda << std::endl;
	  bengy=10.;
	  return;
	}
      }
    }
  }
  int is2mn=abs(j2-2*lmom);
  int is2mx=j2+2*lmom;
  double flns;
  if(!((is2<is2mn)||(is2>is2mx))) {
    double fjs=0.5*(double)(j2);
    double fis=0.5*(double)(is2);
    flns=fjs*(fjs+1.)-flmom*(flmom+1.)-fis*(fis+1.);
  }else{
    int lsq=j2-2*lmom;
    if(lsq<0){
      flns=-lmom1;
    }else if (lsq==0){
      flns=0.;
    }else{
      flns=lmom;
    }
  }
  int match=radi/drd[jhw]+dmat;
 label80:;
  double fmu=vmass*cmass/(vmass+cmass);
  double wk=0.2195376*sqrt(fmu*bengy);
  double wrhon=wk*radi;
  double wrhoc=wrhon;
  double wrhoz=wk*radz;
  double wrhocs=wrhoc*wrhoc;
  double weta=0.7199262*wzz*wk/bengy;
  double wetac=weta/wrhoc;
  double wdrho=drd[jhw]*wk;
  if(isof!=1) wls=0.1767*vdepth;
  double wvs=2.*wls*wk/(bengy*wals);
  double drhosq=wdrho*wdrho;
  double dr56=0.8333333333*drhosq;
  double dr12=0.1*dr56;
  double fl1=lmom*lmom1;
 label100:;
  double wrho=wdrho; // label: 100
  printf("aft label100 %10.20f\n",wdrho);
  double wvc=vdepth/bengy;
  double zer=1.0;
  for (int j=1; j<=lmom2; j++){ 
    double a1=-wvs*flns*wfsr[j]/(flmom1+flmom1);
    double b1=1.0-wvc*wfcr[j]+3.0*wetac;
    double b2=wvs*flns*wfsr[j];
    double a2=(b1-b2*a1)/(4.0*flmom1+2.0);
    double a3=(b1*a1-b2*a2)/(6.0*flmom1+6.0);
    double wrhosq=wrho*wrho;
    double b3=weta/(wrhoc*wrhocs);
    double a4=(b1*a2-b2*a3-b3)/(8.0*flmom1+12.0);
    double a5=(b1*a3-b2*a4-b3*a1)/(10.0*flmom1+20.0);
    double a6=(b1*a4-b2*a5-b3*a2)/(12.0*flmom1+30.0);
    ffr[j][jhw]=pow(wrho,lmom1)*
      (1.0+a1*wrho+a2*wrho*wrho+a3*wrhosq*wrho+
       a4*wrhosq*wrhosq+a5*wrho*wrhosq*wrhosq+a6*pow(wrhosq,3));
    wrho=wrho+wdrho;
  }
  int mat1=match+1;
  double x1=wdrho*flmom1;
  double x2=x1+wdrho;
  double x3=x2+wdrho;
  for(int i=lmom1; i<=mat1; i++){
    double fac1=1.0-dr12*
      (fl1/(x1*x1)+1.0-wvc*wfcr[i]-wvs*flns*wfsr[i]/x1+wfc[i]/bengy);
    double fac2=2.0+dr56*
      (fl1/(x2*x2)+1.0-wvc*wfcr[i+1]-wvs*flns*wfsr[i+1]/x2+wfc[i+1]/bengy);
    double fac3=1.0-dr12*
      (fl1/(x3*x3)+1.0-wvc*wfcr[i+2]-wvs*flns*wfsr[i+2]/x3+wfc[i+2]/bengy);
    ffr[i+2][jhw]=(ffr[i+1][jhw]*fac2-ffr[i][jhw]*fac1)/fac3;
    if((fabs(ffr[i+2][jhw])-test)>=0.) {
      int ip2=i+2;
      for (int ip=1; ip <=ip2;ip++){
	ffr[ip][jhw]=ffr[ip][jhw]/test;
      }
      zer=zer/test;
    }else{
      x1=x2;
      x2=x3;
      x3=x3+wdrho;
    }
  }
  int nodes=0;
  for (int i=lmom1; i <= match; i++) {
    if ((ffr[i][jhw]*ffr[i+1][jhw])<0.) { // 210,215,202
      nodes=nodes+2;
    }else if ((ffr[i][jhw]*ffr[i+1][jhw])==0.){
      nodes=nodes+1;
    }
  }
  int nn=nodes/2;
  double fnn=nn;
  if((nod-nn)!=0) {//225,240,225
    korec=korec+1;
    if((korec-10)>0) {//228,228,226
      kcheck=10;
      std::cout << "0 subroutine bound  kcheck="
		<< kcheck << " ramda=" << ramda << std::endl;
      bengy=10.;
      return;
    }else{
      double vcor=(wrhoz*wrhoz+9.86959*pow((fnod+0.5*flmom1),2))/
	(wrhoz*wrhoz+9.86959*pow(fnn+0.5*flmom1,2));
      vcor=sqrt(vcor);
      if((ia-1)<0) {//230,235,235
	vdepth=vcor*vdepth;
	if(isof!=1) wls=0.1767*vdepth;
	wvs=2.*wls*wk/(bengy*wals);
	goto label100;
      }else{
	bengy=bengy/vcor;
	goto label80;
      }
    }
  }
  double dffr1=((ffr[match+3][jhw]-ffr[match-3][jhw])/60.0
		+3.0*(ffr[match-2][jhw]-ffr[match+2][jhw])/
		20.0+3.0*(ffr[match+1][jhw]-ffr[match-1][jhw])/4.0)/wdrho;
  printf("aft dffr1 %10.20f\n",dffr1);
  korec=0;
  double rhoa=wk*drd[jhw]*(double)(nramax);
  wrho=rhoa;
  int jrho=nramax;
  while (true) { // 325
    double ex=wrho+weta*log(wrho+wrho);
    ffr[jrho][jhw+2]=exp(-ex);
    if((jrho-nramax)<=0) {//340,340,350
      jrho=jrho+1;
      wrho=wrho+wdrho;
    }else{
      break;
    }
  }
  x1=rhoa-wdrho;
  x2=rhoa;
  x3=x2+wdrho;
  double imax=nramax-match+3;
  for (int i=1; i<=imax; i++){
    int k=nramax-i;
    double fac1=1.0-dr12*
      (fl1/(x1*x1)+1.0-wvc*wfcr[k]-wvs*flns*wfsr[k]/x1+wfc[k]/bengy);
    double fac2=2.0+dr56*
      (fl1/(x2*x2)+1.0-wvc*wfcr[k+1]-wvs*flns*wfsr[k+1]/x2+wfc[k+1]/bengy);
    double fac3=1.0-dr12*
      (fl1/(x3*x3)+1.0-wvc*wfcr[k+2]-wvs*flns*wfsr[k+2]/x3+wfc[k+2]/bengy);
    ffr[k][jhw+2]=(ffr[k+1][jhw+2]*fac2-ffr[k+2][jhw+2]*fac3)/fac1;
    if((fabs(ffr[k][jhw+2])-test)>=0.) {//358,352,352
      int nrten=nramax-k+2;
      for (int iten=1; iten<=nrten; iten++){
	int kten=iten+k-1;
	ffr[kten][jhw+2]=ffr[kten][jhw+2]/test;
      }
    }
    x3=x2;
    x2=x1;
    x1=x1-wdrho;
  }
  double dffr2=((ffr[match+3][jhw+2]-ffr[match-3][jhw+2])/
		60.0+3.0*(ffr[match-2][jhw+2]-ffr[match+2][jhw+2])/20.0
		+3.0*(ffr[match+1][jhw+2]-ffr[match-1][jhw+2])/4.0)/wdrho;
  printf("aft dffr2 %10.20f\n",dffr2);
  double ratio=ffr[match][jhw]/ffr[match][jhw+2];
  double tlogd1=dffr1/ffr[match][jhw];
  double tlogd2=dffr2/ffr[match][jhw+2];
  double difnce=fabs(tlogd1-tlogd2);
  printf("difnce %10.20f\n",difnce);
  if((difnce-eps7)>0.) {//510,510,400
    niter=niter+1;
    if((niter-100)>0) {//410,410,405
      kcheck=12;
      std::cout << "0 subroutine bound  kcheck="
		<< kcheck << " ramda=" << ramda << std::endl;
      bengy=10.;
      return;
    }
    double fnum=ffr[match][jhw+2]*dffr2*ratio*ratio-ffr[match][jhw]*dffr1;
    double sum=0.0;
    for (int i=1; i<=nramax; i+=2) {
      double sum1, sum2, sum3;
      if((i-1)<=0) {//420,420,430
	sum1=0.0;
	sum2=ffr[i][jhw]*ffr[i][jhw];
	sum3=ffr[i+1][jhw]*ffr[i+1][jhw];
      }else{
	if((i-match)<=0) {//440,440,450
	  sum1=ffr[i-1][jhw]*ffr[i-1][jhw];
	  sum2=ffr[i][jhw]*ffr[i][jhw];
	  sum3=ffr[i+1][jhw]*ffr[i+1][jhw];
	}else{
	  sum1=ffr[i-1][jhw+2]*ffr[i-1][jhw+2]*ratio*ratio;
	  sum2=ffr[i][jhw+2]*ffr[i][jhw+2]*ratio*ratio;
	  sum3=ffr[i+1][jhw+2]*ffr[i+1][jhw+2]*ratio*ratio;
	}
      }
      if((ia-1)<0) {//460,470,470
	if((i-1)<=0) {//462,462,465
	  sum1=0.0;
	}else{
	  sum1=-sum1*wfcr[i-1]*wvc;
	}
	sum2=-sum2*wfcr[i]*wvc;
	sum3=-sum3*wfcr[i+1]*wvc;
      }
      sum=sum+sum1+4.0*sum2+sum3;
    }
    double denom=sum*wdrho/3.0;
    incr=0;
    double ram1=fnum/denom;
    printf("bfr ramda vdepth: %10.20f\n",vdepth);
    printf("ramda: %10.20f\n",ramda);
    printf("denom: %10.20f\n",denom);
    printf("sum: %10.20f\n",sum);
    printf("wdrho: %10.20f\n",wdrho);
    while (true) {
      ramda=1.0+ram1;
      if((ramda-eps7)<=0.) {//485,485,488
	ram1=0.5*ram1;
	incr=incr+1;
	if((incr-10)>0) {//482,482,486
	  kcheck=13;
	  std::cout << "0 subroutine bound  kcheck="
		    << kcheck << " ramda=" << ramda << std::endl;
	  bengy=10.;
	  return;
	}
      }else{
	break;
      }
    }
    if((ia-1)<0) {//489,500,500
      if((ramda-2.0)<0.0) {//490,495,495
	vdepth=ramda*vdepth;
	printf("aft ramda vdepth: %10.20f\n",vdepth);
	if(isof!=1) wls=0.1767*vdepth;
	wvs=2.*wls*wk/(bengy*wals);
	goto label100;
      }else{
	drz=drz-0.08*radi;
	goto label10;
      }
    }else{
      bengy=bengy*ramda;
      goto label80;
    }
  }
  nr1=nramax+1;
  for (int i=match; i<=nr1; i++) {
    ffr[i][jhw]=ratio*ffr[i][jhw+2];
  }
  double sum=0.;
  for (int i=1; i<=nramax; i+=2) {
    double sum1, sum2, sum3;
    if((i-1)<=0.) {//540,540,550
      sum1=0.0;
    }else{
      sum1=ffr[i-1][jhw]*ffr[i-1][jhw];
    }
    sum2=ffr[i][jhw]*ffr[i][jhw];
    sum3=ffr[i+1][jhw]*ffr[i+1][jhw];
    sum=sum+sum1+4.0*sum2+sum3;
  }
  sum=sum*drd[jhw]/3.0;
  double znorm=1.0/sqrt(sum);
  double wlss=2.*wls*flns/wals;
  double r=drd[jhw];
  double dr=r;
  //c     *****( non-local correction )*****
  int ipnl=0;
  if(pnloc<0.) ipnl=fabs(pnloc)/dr+1;
  double fact=0.048196758*fmu*pow(pnloc,2)/8.0;
  sum=0.0;
  for (int i=1; i<=nr1; i++) {
    ffr[i][jhw]=znorm*ffr[i][jhw];
    if(!(fact<1.e-10)) {
      ffr[i][3]=vdepth*wfcr[i]+wlss*wfsr[i]/r-wfc[i];
      ffr[i][jhw]=ffr[i][jhw]*exp(-fact*ffr[i][3]);
      if(i<ipnl) ffr[i][jhw]=0.0;
      sum=sum+pow(ffr[i][jhw],2);
    }
    r=r+dr;
  }
  chis[jhw]=znorm*zer*pow(wk,lmom1);
  znorm=1.0;
  if(!(fact<1.e-10)) {
    chis[jhw]=chis[jhw]*exp(-fact*ffr[1][3]);
    if(ipnl<0) chis[jhw]=0.0;
    znorm=1.0/sqrt(sum*dr);
  }
  r=dr;
  for (int i=1; i<=nr1; i++) {
    switch (ic) { //go to (575,580,585),ic
    case 1:
      ffr[i][jhw]=znorm*ffr[i][jhw]/r;
      ffr[i][3]=vdepth*wfcr[i]+wlss*wfsr[i]/r-wfc[i];
      break;
    case 2:
      ffr[i][3]=vdepth*wfcr[i]+wlss*wfsr[i]/r;
      ffr[i][jhw]=znorm*ffr[i][jhw]*ffr[i][3];
      break;
    case 3:
      ffr[i][3]=vdepth*wfcr[i]+wlss*wfsr[i]/r-wfc[i];
      ffr[i][jhw]=znorm*ffr[i][jhw]*ffr[i][3];
      break;
    }
    r=r+dr;
  }
  chis[jhw]=znorm*chis[jhw];
  if(ic!=1) chis[jhw]=chis[jhw]*ffr[1][3];
  return;
}
//c     ---------------------------------------------------------         


void make_jt_wf() {
  int    ia     = 0;      // search well depth for ia=0, search energy for ia=1
  int    ic     = 1;      // option for residual
  int    ib     = 0;      // potential type
  double wr0    = 1.2434; // potential radius
  double wal    = 0.7;    // potential diffuseness
  int    nramax = 1600;   // steps
  double drx    = 0.05;   // step
  double wls    = 6.0;    // spin orbit strength in MeV
  int    isof   = 0;      // fixed for isof = 1, hamamoto for isof = 0
  double cmass  = 28;     // cmass
  double vmass  = 1;      // vmass
  double bengy  = 0.01;  // bengy
  int    j2a    = 3;      // 2*j
  int    lmoma  = 2;      // lmoma
  int    nodd   = 2;      // nodd
  double wzz    = 0.0;    // ?
  int    is2    = 1;      // ?
  int    jhw    = 1;      // ?
  double dmat   = 0.;     // ?
  double wrz    = wr0;    // ?
  double pnloc  = 0.;     // ?
  double wr0ls  = wr0;    // ?
  double wals   = wal;    // ?
  double chis[11];
  double ffr[6603][5];
  double vdepth;
  int    kcheck;
  bound(ia,ib,ic,is2,j2a,
	lmoma,nodd,nramax,jhw,
	cmass,vmass,
	wzz,wal,wr0,wls,
	isof,
	ffr,vdepth,bengy,dmat,
	kcheck,wrz,chis,drx,
	pnloc,wr0ls,wals);
  double u[6603],vv[6603];
  std::cout << "vdepth" << vdepth << std::endl;
  u[1]=0.;
  vv[1]=0.;
  for (int i =1; i <=nramax; i++) {
    u[i+1]  = ffr[i][1];
    vv[i+1] = ffr[i][3];
  }
  for (int i =1; i <=nramax+1; i++) {
    double r=(i-1)*drx;
    u[i] = r*u[i];
    std::cout << r << "," << u[i] << std::endl;
  }
  gROOT->cd();
  TH1D*  h = new TH1D("h0","h0",1601,-0.025,80.025);
  for (int i =1; i <=nramax+1; i++) {
    h->SetBinContent(i,u[i]);
  }
  h->Draw();
  
  
}
