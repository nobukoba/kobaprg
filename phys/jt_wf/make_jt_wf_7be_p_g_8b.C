#include "Math/SpecFunc.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TF1.h"
#include "TH1.h"
#include "TLatex.h"
#include <iostream>

/*     ---------------------------------------------------------      */
void FandG(const double eta, const double krval, const int lmax,
	   double fa[26], double ga[26], double sa[26]) {
  /* implicit real*8(a-h,o-z) */
  const double gd[101] = {+0.,+12.,-360.,1260.,-1680.,1188.};
  const double cg0 =  1.223406016e+0;
  const double cg1 =  4.959570165e-2;
  const double cg2 =  8.888888889e-3;
  const double cg3 =  2.455199181e-3;
  const double cg4 =  9.108958061e-4;
  const double cg5 =  2.534684115e-4;
  const double cgp = -7.078817734e-1;
  const double cp1 =  1.728260369e-1;
  const double cp2 =  3.174603174e-4;
  const double cp3 =  3.581214850e-3;
  const double cp4 =  3.117824680e-4;
  const double cp5 =  9.073966427e-4;
  double drho=1.;
  int nr=1;
  double eps=1.0e-12;
  double eta2=eta+eta;
  double etas=eta*eta;
  int lp;
  if (lmax>12) {lp=lmax+1;}
  else         {lp=12+1;}
  double t=lp;
  double u=t*t+etas;
  double v=t/u;
  double w=eta/u;
  double x=v*v-w*w;
  double y=2.*v*w;
  u=sqrt (u);
  double sig0=eta*(log(u)-1.)+(t-0.5)*atan(eta/t);
  for (int i=1; i<=5; i++) {
    sig0=sig0-w/gd[i];
    t=v*x-w*y;
    w=v*y+w*x;
    v=t;
  }
  while (true) {
    if (lp <= lmax+1) sa[lp-1]=sig0;
    lp=lp-1;
    if (lp <= 0) break;
    t=lp;
    sig0=sig0- atan (eta/t);
  }
  double emax=pow(1.0e-5/eps,0.16666667);
  double r, g, gp;
  if (!(eta < emax)) {/* go to 200 */
    r=eta2;
    t=6.0;
    t=pow(eta,(1.0/t));
    w=eta*t;
    u=t-t*(cg2+cg4/etas)/etas;
    v=(cg1+(cg3+cg5/etas)/etas)/w;
    g=cg0*(u+v);
    t=1./t;
    w=eta*t;
    u=t+t*(cp2+cp4/etas)/etas;
    v=(cp1+(cp3+cp5/etas)/etas)/w;
    gp=cgp*(u-v);
    /*  go to 300 */
  }else{
    if (nr>1) {r=nr-1;}
    else      {r=1-1;}
    r=krval+r*drho;
    t=12.+1.4*eta;
    if (t > r) r=t;
    double fk=1.;
    double f=1.;
    double gk=0.;
    g=0.;
    double fsk=0.;
    double fp=0.;
    double gsk=1.-eta/r;
    gp=gsk;
    double epss=eps*eps;
    int n=r+r;
    for (int kp=1; kp<=n; kp++) {
      t=kp+kp;
      u=t*r;
      double ak=(t-1.)*eta/u;
      v=kp*(kp-1);
      double bk=(etas-v)/u;
      t=ak*fk-bk*gk;
      gk=ak*gk+bk*fk;
      fk=t;
      t=ak*fsk-bk*gsk-fk/r;
      gsk=ak*gsk+bk*fsk-gk/r;
      fsk=t;
      f=f+fk;
      g=g+gk;
      fp=fp+fsk;
      gp=gp+gsk;
      double test=fk*fk+gk*gk+fsk*fsk + gsk*gsk;
      if (test < epss) break;
    }
    t=r-eta*log(r+r)+sig0;
    u=cos (t);
    v=sin (t);
    g=f*u-g*v;
    gp=fp*u-gp*v;
  }
  double rs=r;
  double rho=krval;
  double f=g;
  double fp=gp;
  int is=0;
  int ir=1;
  t=r-rho;
  /*
  printf("rs: %10.20f\n", rs);
  printf("rho: %10.20f\n", rho);
  printf("f: %10.20f\n", f);
  printf("fp: %10.20f\n", fp);
  printf("t: %10.20f\n", t);
  */
  int line_number = 0;
  if (t<0.) {/* 600,700,310 */
    line_number = 600;
  }else if (t==0.){
    line_number = 700;
  }else{
    line_number = 310;
  }
  double gg=0.;
  while (true) {
    if (line_number == 310) {
      if (!(nr <= 1)) {/* go to 320 */
	is=t/drho;
	/* is=min0 (is+1,nr) */
	if ((is+1)<nr) {is=is+1;}
	else           {is=nr;}
      }
      t=is;
      rho=krval+t*drho;
      gg=rho;
      is=is+1;
      ir=is;
      rho=rho-drho; /* label: 330 */
      ir=ir-1;
      if (!(ir > 0)) {/* go to 600 */
	/* ir=max0 (is,1) */
	if (is>1) {ir=is;}
	else      {ir=1;}
	r=rs;
	rho=gg;
	f=g;
	fp=gp;
	if (ir > nr) return;
      }
      line_number = 600; continue;
    }else if (line_number == 600) {
      double h=0.5; /* label: 600 */
      w=r-eta2;
      if ((r-1.0)<0.) {/* 601,602,602 */
	h=0.5*r;
      }
      if (w<0.) {/* 603,605,605 */
	t=sqrt (-r/(w+w));
	if ((t-h)<0.) {/* 604,605,605 */
	  h=t;
	}
      }
      int last=0;
      t=rho-r;
      if (t<0.) {/* 606,700,607 */
	h=-h;
      }else if (t==0.){
	line_number = 700; continue;
      }
      u=t-h;
      if ((u*h)>=0.) {/* 608,608,609 */
	h=t;
	last=1;
      }
      u=0.0;
      t=1.0;
      double b1=0.0;
      double b2=f;
      double b3=h*fp;
      f=f+b3;
      v=0.0;
      int it=0; /* label: 610 */
      while (true) {
	v=-h*(h*b1+w*b2+u*v)/(r*t); /* label: 620 */
	fp=fp+v;
	u=t;
	t=t+1.0;
	b1=b2;
	b2=b3;
	b3=h*v/t;
	f=f+b3;
	double test=b3;
	double testp=v;
	if (w<0.) {/* 630,640,640 */
	  test=b3/f;
	  testp=v/fp;
	}
	if ((fabs(test)+fabs(testp)-eps)<0.) {/* 650,610,610 */
	  if (it <=0) {/* 660,660,670 */
	    it=1;
	    continue;
	  }else{
	    r=r+h;
	  }
	  if (last<=0) {/* 600,600,700 */
	    line_number = 600; break;
	  }else{
	    line_number = 700; break;
	  }
	}else{
	  it=0;
	  continue;
	}
      }
    }else if (line_number==700) {
      int k=lmax+1;
      x=f;
      y=fp;
      for (int j=1; j<=k; j++) {
	ga[j-1]=x;
	double al=j;
	t=j*j;
	u=t/rho+eta;
	v=sqrt (t+etas);
	w=(u*x-al*y)/v;
	y=(v*x-u*w)/al;
	x=w;
      }
      lp=rho;
      /* lp=max0 (lp+10, lmax+20) */
      if ((lp+10)>(lmax+20)) {lp=lp+10;}
      else                   {lp=lmax+20;}
      double b3=0.;
      double b2=1.0e-20;
      w=1.0/rho;
      double al=lp+1;
      v=eta/al;
      u=0.;
      double b1=0.;
      for (int j=1; j<=lp; j++) {
	k=lp+1-j;
	al=k;
	t=k+k+1;
	b1=t*(v/al+w)*b2-u*b3;
	v=eta/al;
	u=sqrt (1.0+v*v);
	b1=b1/u;
	b3=b2;
	b2=b1;
	if ((k-lmax-1)<=0) {/* 810,810,820 */
	  fa[k-1]=b1;
	}else{
	  double test=b1;
	  if ((fabs(test)-1.)>0.) {/* 840,840,830 */
	    b2=b2*1.0e-20;
	    b3=b3*1.0e-20;
	  }
	}
      }
      t=(w+eta)*b2-u*b3;
      u=1./(t*f-b1*fp);
      k=lmax+1;
      /* 
      printf("aft line 700 t: %10.20f\n", t);
      printf("aft line 700 u: %10.20f\n", u);
      printf("aft line 700 k: %d\n", k);
      */
      for (int j=1; j<=k; j++) {
	fa[j-1]=u*fa[j-1];
      }
      if ((ir-is)<0) {/* 330,340,340 */
	rho=rho-drho; /* label: 330 */
	ir=ir-1;
	if (!(ir > 0)) {/* go to 600 */
	  /* ir=max0 (is,1) */
	  if (is>1) {ir=is;}
	  else      {ir=1;}
	  r=rs;
	  rho=gg;
	  f=g;
	  fp=gp;
	  if (ir > nr) return;
	}
      }else{
	rho=rho+drho;
	ir=ir+1;
	if (ir > nr) return;
      }
      line_number = 600; continue;
    }else{
      std::cout << "Unknown line_number: " << line_number << std::endl;
    }
  }
}

/*     ---------------------------------------------------------        */
double V(const int j, const int l, const double r,
	 const double rmu, const double conno1, const double pi,
	 const double vdepth, const double wal,
	 const double wr0, const double wls, const int ib) {
  /* implicit real*8 (a-h,o-z)   
     common rmu,conno1,pi,vdepth,wal,wr0,wls,ib */
  double s=0.5;
  double rj=j/2.;
  double V_result=0.;
  if(ib==1) {
    V_result = -vdepth*exp(-r*r/wal/wal);
  }else if(ib==0) {
    double expfac=exp((r-wr0)/wal);
    V_result = -vdepth/(1.+expfac);
    double opls=(rj*(rj+1)-l*(l+1)-s*(s+1))*wls;
    double Vso=-2*opls*expfac/pow(1+expfac,2)/r/wal;
    V_result=V_result+Vso;
  }
  return V_result;
}

/*     ---------------------------------------------------------         */
void wf(double u[6603], const double h,
	const int nsteps, const double erel,
	const int j, const int l, 
	double &A, double &B,
	const double eta, /* Nobu added on 20200117 */
	const double fmuh, const double con,
	const double pi, const double vdepth,
	const double wal, const double wr0,
	const double wls, const int ib) {
  /* implicit real*8 (a-h,o-z) */
  /* double u(0:6602) */
  double Astart,Anew,Aold,Dnew,Dold,det;
  double C,Kold,Knew;
  /*     complex*16 T,S */
  double fa1[26],ga1[26],sa[26],fa2[26],ga2[26];
  /* common fmuh,con,pi */
  /*     ---------------------------------------------------------            
         erel=energy in the centre of mass frame
         fmuh=reduced mass of projectile-target system
         l=orbital angular momentum 
         h=radial integration step size
         nsteps=number of radial integration steps
         ---------------------------------------------------------        
         con is sqrt(2*[1amu]/hbar**2) in MeV**(0.5) fm**(-1)
         ---------------------------------------------------------            
         const is 2*fmuh/hbar**2, rk is centre of mass wavenumber */
  double const_=pow(con,2)*fmuh;
  double rk=sqrt(const_*erel);
  /*     print*,' k = ',rk
         ---------------------------------------------------------*/
  int i1=nsteps-2;
  double r1=i1*h;
  /* FandG(0.,rk*r1,20,fa1,ga1,sa); */
  FandG(eta,rk*r1,20,fa1,ga1,sa); /* Nobu added */
  int i2=i1-10;
  double r2=i2*h;
  /* FandG(0.,rk*r2,20,fa2,ga2,sa); */
  FandG(eta,rk*r2,20,fa2,ga2,sa); /* Nobu added */
  /*     ---------------------------------------------------------            
         Subtracted Cowell/Numerov method used for integration
         ---------------------------------------------------------                  
         starting values: at r=0 and r=h
         u(i) stores radial wavefunction at i*h
         ---------------------------------------------------------              */
  u[0]=0.;
  u[1]=pow(h,(l+1));
  /* Kold=l*(l+1)/pow(h,2)+const_*
     (V(j,l,h,fmuh,con,pi,vdepth,wal,wr0,wls,ib)-erel); */
  Kold=2.*eta/h+l*(l+1)/pow(h,2)+const_*
    (V(j,l,h,fmuh,con,pi,vdepth,wal,wr0,wls,ib)-erel); /* Nobu added */
  Aold=(1.-h*h*Kold/12.)*u[1];
  /*     Astart is analogue of Aold but at r=0 (care needed)!!*/
  Astart=0.;
  if(l==1) Astart=-2./12.*h*h;
  Dold=Aold-Astart;
  for (int i=2; i<=nsteps; i++){
    double r=i*h;
    /* *       print*,V(j,l,r) */
    Dnew=Dold+h*h*Kold*u[i-1];
    Anew=Aold+Dnew;
    /* Knew=l*(l+1)/pow(r,2)+const_*
       (V(j,l,r,fmuh,con,pi,vdepth,wal,wr0,wls,ib)-erel); */
    Knew=2.*eta/r+l*(l+1)/pow(r,2)+const_*
      (V(j,l,r,fmuh,con,pi,vdepth,wal,wr0,wls,ib)-erel);  /* Nobu added */
    u[i]=Anew/(1.-h*h*Knew/12.);
    Kold=Knew;
    Dold=Dnew;
    Aold=Anew;
  }
  /*     ---------------------------------------------------------            
         now need to match numerical wavefunction to physical
         form assumed for r > potential range
         ------ RADIUS 1  ----------------------------------------*/
  double F1=fa1[l];
  double G1=ga1[l];
  /*     ------ RADIUS 2  ----------------------------------------*/
  double F2=fa2[l];
  double G2=ga2[l];
  /*     ---------------------------------------------------------       
         solve linear equations for A and B
         so A=C*cos(delta) and B=C*sin(delta), C=sqrt(A**2+B**2)
         ---------------------------------------------------------             */
  det=F1*G2-F2*G1;
  /*
  printf("F1: %10.20f\n", F1);
  printf("F2: %10.20f\n", F2);
  printf("G1: %10.20f\n", G1);
  printf("G2: %10.20f\n", G2);*/
  A=(u[i1]*G2-u[i2]*G1)/det;
  B=(u[i2]*F1-u[i1]*F2)/det;
  C=sqrt(pow(A,2)+pow(B,2));
  A=A/C;
  B=B/C;
  /*     ---------------------------------------------------------       
	 now: A = cos(delta) and B = sin(delta)
	 having matched, re-normalise the numerical wavefunction
	 ---------------------------------------------------------             */
  for (int i=0; i<=nsteps; i++) {
    u[i]=u[i]/C;
  }
  if(A<0.) {
    B=-B;
    A=-A;
    for (int i=0; i<=nsteps; i++) {
      u[i]=-u[i];
    }
  }
  /*     ---------------------------------------------------------       
         function norm is now sin(kr...+delta) as r-> infinity               
         calculate and print various derived quantities
         ---------------------------------------------------------             
         print*,' cos(delta) = ',A
         print*,' sin(delta) = ',B
         compute partial wave T(l) = exp(i*delta)*sin(delta)
         T=(A+(0.,1.)*B)*B
         print*,' T(l) = ',T
         compute partial wave S(l) = exp(2*i*delta)
         S=(A+(0.,1.)*B)**2
         Smod=abs(S)
         print*,' S(l) = ',S,' with modulus ',Smod
         ---------------------------------------------------------      */
}

void bound(const int ia, const int ib, const int ic,
	   const int is2, const int j2, const int lmom,
	   const int nod, const int nramax, const int jhw,
	   const double cmass, const double vmass,
	   const double wzz, const double wa,
	   const double wr0, double &wls,
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
    if      (ib1 == 1) { wfcr[i]=1.0/(1.0+ex); }
    else if (ib1 == 2) { wfcr[i]=exp(-(wr/wa)*(wr/wa)); }
    else if (ib1 == 3) { wfcr[i]=exp(-wa*wr)/wr; }
    else if (ib1 == 4) { wfcr[i]=exp(-wa*wr)/(exp(-wr0*wr)-exp(-wa*wr)); }
    else if (ib1 == 5) { wfcr[i]=(1.0+yyyy)/(cosh(wr/wa)+yyyy); }
    wfsr[i]=exls/(1.0+exls)/(1.0+exls);
    if ((wr-radi)<=0.) {
      wfc[i]=0.7199262*wzz*(3.0-wr*wr/(radi*radi))/radi;
    }else{
      wfc[i]=1.4398523*wzz/wr;
    }
    wr=wr+drd[jhw];
  }
  double ramda,flns;
  double fmu,wk=0.;
  double wrhon,wrhoc,wrhoz,wrhocs,weta,wetac,wdrho,wvs,drhosq;
  double dr56,dr12,fl1,zer,ratio;
  int match=0;
  int line_number = 10;
  while (true) {
    if (line_number == 10) {
      if((ia-1)<0){
	vdepth=bengy+pow(3.1415926*(fnod+0.5*flmom1),2)/
	  (0.048228*vmass*pow(radi+drz,2)); /* label: 10 */
	/*printf("label 10 vdepth: %10.20f\n",vdepth); */
      }else{
	while (true) {
	  bengy=vdepth-pow(3.1415926*(fnod+0.5*flmom1),2)/
	    (0.048228*vmass*radz*radz);
	  /*
	  printf("incr %d\n",incr);
	  printf("bengy %f\n",bengy);
	  printf("vdepth %f\n",vdepth);
	  printf("eps7 %f\n",eps7);
	  */
	  
	  if((bengy-eps7)>0.) {
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
      match=radi/drd[jhw]+dmat;
      line_number = 80;
      continue;
    }else if (line_number == 80){
      fmu=vmass*cmass/(vmass+cmass);
      wk=0.2195376*sqrt(fmu*bengy); /* sqrt(2*amu)/hbar*c = sqrt(2*938.49410242)/197.3269804 = 0.2187351521 */
      wrhon=wk*radi;
      wrhoc=wrhon;
      wrhoz=wk*radz;
      wrhocs=wrhoc*wrhoc;
      weta=0.7199262*wzz*wk/bengy; /* e^2/2.0 = 0.7199822 */
      wetac=weta/wrhoc;
      wdrho=drd[jhw]*wk;
      if(isof!=1) wls=0.1767*vdepth;
      wvs=2.*wls*wk/(bengy*wals);
      drhosq=wdrho*wdrho;
      dr56=0.8333333333*drhosq;
      dr12=0.1*dr56;
      fl1=lmom*lmom1;
      line_number = 100;
      continue;
    }else if (line_number == 100){
      double wrho=wdrho; /* label: 100 */
      /* printf("aft label100 %10.20f\n",wdrho); */
      double wvc=vdepth/bengy;
      /* printf("aft label100 wvc %10.20f\n",wvc); */
      zer=1.0;
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
      /*
      printf("wetac %10.20f\n",wetac);
      printf("a1 %10.20f\n",a1);
      printf("b1 %10.20f\n",b1);
      printf("b2 %10.20f\n",b2);
      printf("a2 %10.20f\n",a2);
      printf("a3 %10.20f\n",a3);
      printf("wrho %10.20f\n",wrho);
      */
      
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
	if ((ffr[i][jhw]*ffr[i+1][jhw])<0.) { /* 210,215,202 */
	  nodes=nodes+2;
	}else if ((ffr[i][jhw]*ffr[i+1][jhw])==0.){
	  nodes=nodes+1;
	}
      }
      int nn=nodes/2;
      double fnn=nn;
      if((nod-nn)!=0) {/* 225,240,225 */
	korec=korec+1;
	if((korec-10)>0) {/* 228,228,226 */
	  kcheck=10;
	  std::cout << "0 subroutine bound  kcheck="
		    << kcheck << " ramda=" << ramda << std::endl;
	  bengy=10.;
	  return;
	}else{
	  double vcor=(wrhoz*wrhoz+9.86959*pow((fnod+0.5*flmom1),2))/
	    (wrhoz*wrhoz+9.86959*pow(fnn+0.5*flmom1,2));
	  vcor=sqrt(vcor);
	  if((ia-1)<0) {/* 230,235,235 */
	    vdepth=vcor*vdepth;
	    if(isof!=1) wls=0.1767*vdepth;
	    wvs=2.*wls*wk/(bengy*wals);
	    line_number = 100;
	    continue;
	  }else{
	    bengy=bengy/vcor;
	    line_number = 80;
	    continue;
	  }
	}
      }
      double dffr1=((ffr[match+3][jhw]-ffr[match-3][jhw])/60.0
		    +3.0*(ffr[match-2][jhw]-ffr[match+2][jhw])/
		    20.0+3.0*(ffr[match+1][jhw]-ffr[match-1][jhw])/4.0)/wdrho;
      /* printf("aft dffr1 %10.20f\n",dffr1); */
      korec=0;
      double rhoa=wk*drd[jhw]*(double)(nramax);
      wrho=rhoa;
      int jrho=nramax;
      while (true) { /* 325 */
	double ex=wrho+weta*log(wrho+wrho);
	ffr[jrho][jhw+2]=exp(-ex);
	if((jrho-nramax)<=0) {/* 340,340,350 */
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
	if((fabs(ffr[k][jhw+2])-test)>=0.) {/* 358,352,352 */
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
      /* printf("aft dffr2 %10.20f\n",dffr2); */
      ratio=ffr[match][jhw]/ffr[match][jhw+2];
      double tlogd1=dffr1/ffr[match][jhw];
      double tlogd2=dffr2/ffr[match][jhw+2];
      double difnce=fabs(tlogd1-tlogd2);
      /* printf("difnce %10.20f\n",difnce); */
      if((difnce-eps7)>0.) {/* 510,510,400 */
	niter=niter+1;
	if((niter-100)>0) {/* 410,410,405 */
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
	  if((i-1)<=0) {/* 420,420,430 */
	    sum1=0.0;
	    sum2=ffr[i][jhw]*ffr[i][jhw];
	    sum3=ffr[i+1][jhw]*ffr[i+1][jhw];
	  }else{
	    if((i-match)<=0) {/* 440,440,450 */
	      sum1=ffr[i-1][jhw]*ffr[i-1][jhw];
	      sum2=ffr[i][jhw]*ffr[i][jhw];
	      sum3=ffr[i+1][jhw]*ffr[i+1][jhw];
	    }else{
	      sum1=ffr[i-1][jhw+2]*ffr[i-1][jhw+2]*ratio*ratio;
	      sum2=ffr[i][jhw+2]*ffr[i][jhw+2]*ratio*ratio;
	      sum3=ffr[i+1][jhw+2]*ffr[i+1][jhw+2]*ratio*ratio;
	    }
	  }
	  if((ia-1)<0) {/* 460,470,470 */
	    if((i-1)<=0) {/* 462,462,465 */
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
	/*
	printf("bfr ramda vdepth: %10.20f\n",vdepth);
	printf("ramda: %10.20f\n",ramda);
	printf("denom: %10.20f\n",denom);
	printf("sum: %10.20f\n",sum);
	printf("wdrho: %10.20f\n",wdrho);
	*/
	while (true) {
	  ramda=1.0+ram1;
	  if((ramda-eps7)<=0.) {/* 485,485,488 */
	    ram1=0.5*ram1;
	    incr=incr+1;
	    if((incr-10)>0) {/* 482,482,486 */
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
	if((ia-1)<0) {/* 489,500,500 */
	  if((ramda-2.0)<0.0) {/* 490,495,495 */
	    vdepth=ramda*vdepth;
	    /* printf("aft ramda vdepth: %10.20f\n",vdepth); */
	    if(isof!=1) wls=0.1767*vdepth;
	    wvs=2.*wls*wk/(bengy*wals);
	    line_number = 100;
	    continue;
	  }else{
	    drz=drz-0.08*radi;
	    line_number = 10;
	    continue;
	  }
	}else{
	  bengy=bengy*ramda;
	  line_number = 80;
	  continue;
	}
      }else{
	break;
      }
    }
  }
  nr1=nramax+1;
  for (int i=match; i<=nr1; i++) {
    ffr[i][jhw]=ratio*ffr[i][jhw+2];
  }
  double sum=0.;
  for (int i=1; i<=nramax; i+=2) {
    double sum1, sum2, sum3;
    if((i-1)<=0.) {/* 540,540,550 */
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
  /* c     *****( non-local correction )***** */
  int ipnl=0;
  if (pnloc<0.) ipnl=fabs(pnloc)/dr+1;
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
    /* go to (575,580,585),ic */
    if (ic == 1) {
      ffr[i][jhw]=znorm*ffr[i][jhw]/r;
      ffr[i][3]=vdepth*wfcr[i]+wlss*wfsr[i]/r-wfc[i];
    }else if (ic == 2) {
      ffr[i][3]=vdepth*wfcr[i]+wlss*wfsr[i]/r;
      ffr[i][jhw]=znorm*ffr[i][jhw]*ffr[i][3];
    }else if (ic == 3) {
      ffr[i][3]=vdepth*wfcr[i]+wlss*wfsr[i]/r-wfc[i];
      ffr[i][jhw]=znorm*ffr[i][jhw]*ffr[i][3];
    }
    r=r+dr;
  }
  chis[jhw]=znorm*chis[jhw];
  if(ic!=1) chis[jhw]=chis[jhw]*ffr[1][3];
  return;
}
/* c     ---------------------------------------------------------         */

double WhittakerFunctionW (double k, double m, double z) {
  return exp(-z/2.0)*pow(z,m+1./2.)*ROOT::Math::conf_hypergU(1./2.+m-k, 1+2*m, z);
}

void make_jt_wf_7be_p_g_8b() {
  int    ia     = 0;      /* search well depth for ia=0, search energy for ia=1   */
  int    ic     = 1;      /* option for residual				  */
  int    ib     = 0;      /* potential type					  */
  double wr0    = 1.25;   /* potential radius					  */
  double wal    = 0.65;   /* potential diffuseness				  */
  int    nramax = 1600;   /* steps						  */
  double drx    = 0.05;   /* step						  */
  double wls    = 0.0;    /* spin orbit strength in MeV				  */
  int    isof   = 1;      /* fixed for isof = 1, hamamoto for isof = 0		  */
  double cmass  = 7.0;    /* cmass						  */
  double vmass  = 1.0;    /* vmass						  */
  double vdepth = 46.56;  /* for PRL73(1994)2029 				  */
  double bengy  = 0.1364; /* bengy						  */
  int    j2a    = 1;      /* 2*j						  */
  int    lmoma  = 1;      /* lmoma						  */
  int    nodd   = 0;      /* nodd						  */
  double wzz    = 4.0;    /* Z_c * Z_v in Sommerfeld parameter			  */
  int    is2    = 1;      /* ?							  */
  int    jhw    = 1;      /* ?							  */
  double dmat   = 0.0;    /* ?							  */
  double wrz    = wr0;    /* ?							  */
  double pnloc  = 0.0;    /* ?							  */
  double wr0ls  = wr0;    /* ?							  */
  double wals   = wal;    /* ?                                                    */
  double chis[11];
  double ffr[6603][5];
  int    kcheck;
  double u[6603], vv[6603];
  gROOT->cd();
  TH1D*  h[4];
  h[0] = new TH1D("h0","h0",1601,-0.025,80.025);
  h[1] = new TH1D("h1","h1",1601,-0.025,80.025);
  h[2] = new TH1D("h2","h2",1601,-0.025,80.025);
  h[3] = new TH1D("h3","h3",1601,-0.025,80.025);
  
  bound(ia,ib,ic,is2,j2a,lmoma,nodd,nramax,jhw,cmass,vmass,
	wzz,wal,wr0,wls,isof,ffr,vdepth,bengy,dmat,
	kcheck,wrz,chis,drx,pnloc,wr0ls,wals);
  std::cout << "vdepth: " << vdepth << std::endl;
  std::cout << "bengy: " << bengy << std::endl;
  for (int i =1; i <=nramax+1; i++) {
    h[1]->SetBinContent(i,(i-1)*drx*ffr[i-1][1]);
  }
  
  vdepth = 151.76;
  wr0    = 0.53;
  wal    = 0.65;
  bound(ia,ib,ic,is2,j2a,lmoma,nodd,nramax,jhw,cmass,vmass,
	wzz,wal,wr0,wls,isof,ffr,vdepth,bengy,dmat,
	kcheck,wrz,chis,drx,pnloc,wr0ls,wals);
  std::cout << "vdepth: " << vdepth << std::endl;
  std::cout << "bengy: " << bengy << std::endl;
  for (int i =1; i <=nramax+1; i++) {
    h[2]->SetBinContent(i,(i-1)*drx*ffr[i-1][1]);
  }
  
  vdepth = 47.91;
  wr0    = 1.25;
  wal    = 0.27;
  bound(ia,ib,ic,is2,j2a,lmoma,nodd,nramax,jhw,cmass,vmass,
	wzz,wal,wr0,wls,isof,ffr,vdepth,bengy,dmat,
	kcheck,wrz,chis,drx,pnloc,wr0ls,wals);
  std::cout << "vdepth: " << vdepth << std::endl;
  std::cout << "bengy: " << bengy << std::endl;
  for (int i =1; i <=nramax+1; i++) {
    h[3]->SetBinContent(i,(i-1)*drx*ffr[i-1][1]);
  }
  
  double fmu=vmass*cmass/(vmass+cmass);
  double wk=0.2195376*sqrt(fmu*bengy); /* sqrt(2*amu)/hbar*c = sqrt(2*938.27208816)/197.3269804 = 0.21952951919 */
  double weta=0.7199262*wzz*wk/bengy; /* e^2/2.0 = 0.7199822 */
  double fmu2 = (938.0 / 197.34) * fmu;
  double G11sq = 0.013;
  double G12sq = 0.069;
  double C11sq = G11sq * pow(fmu2, 2) / (4.*atan(1.)); /* pi = 4.*atan(1.) */
  double C12sq = G12sq * pow(fmu2, 2) / (4.*atan(1.));
  double fac = sqrt(C11sq + C12sq); 
  
  //std::cout << "ROOT::Math::sph_bessel(1,2.): " << ROOT::Math::sph_bessel(1,2.) << std::endl;
  //std::cout << "ROOT::Math::conf_hypergU(1.0, 1.0, 1.0): " << ROOT::Math::conf_hypergU(1.0, 1.5, 0.025) << std::endl;
  
  h[0]->SetBinContent(1, fac*WhittakerFunctionW(-weta, lmoma+1./2., 2*wk*drx));
  for (int i =2 ; i <=nramax+1; i++) {
    h[0]->SetBinContent(i, fac*WhittakerFunctionW(-weta, lmoma+1./2., 2*wk*(i-1)*drx));
  }

  h[3]->Draw();
  h[1]->Draw("same");
  h[2]->Draw("same");
  h[0]->Draw("same");

  
  TH1D*  h_norm[4];
  h_norm[1] = (TH1D*)h[1]->Clone("h1_norm");
  h_norm[2] = (TH1D*)h[2]->Clone("h2_norm");
  h_norm[3] = (TH1D*)h[3]->Clone("h3_norm");
  h_norm[1]->Scale(h[0]->GetBinContent(1400)/h[1]->GetBinContent(1400));
  h_norm[2]->Scale(h[0]->GetBinContent(1400)/h[2]->GetBinContent(1400));
  h_norm[3]->Scale(h[0]->GetBinContent(1400)/h[3]->GetBinContent(1400));

  h_norm[3]->Draw();
  h_norm[1]->Draw("same");
  h_norm[2]->Draw("same");
  h[0]->Draw("same");

  return;
  
  double w[6603],A,B;
  double ebe1 = 10.;
  int lfin  = 3;
  int jvals = 5;
  double conno1=0.2195376; /* sqrt(2*amu)/hbar*c = sqrt(2*938.27208816)/197.3269804 = 0.21952951919 */
  double pi=4.*atan(1.);
  double pmass=cmass+vmass;
  double rmu=cmass*vmass/pmass;
  wr0=wr0*pow(cmass,0.333333333333);

  wk=0.2195376*sqrt(fmu*ebe1); /* sqrt(2*amu)/hbar*c = sqrt(2*938.27208816)/197.3269804 = 0.21952951919 */
  weta=0.7199262*wzz*wk/ebe1; /* e^2/2.0 = 0.7199822 */
  
  wf(w,drx,nramax,ebe1,jvals,lfin,A,B,
     weta,rmu,conno1,pi,vdepth,wal,wr0,wls,ib);
  
  for (int i =1; i <=nramax; i++) {
    double r=(i-1)*drx;
    /* std::cout << r << "," << w[i-1] << std::endl; */
  }
  TH1D*  h1 = new TH1D("h1","h1",1601,-0.025,80.025);
  for (int i =1; i <=nramax+1; i++) {
    h1->SetBinContent(i,w[i-1]);
  }
  
  h1->Draw();
  
  std::cout << "A,B: " << A << "," << B << std::endl;
  printf("drx: %10.20f\n", drx);
  printf("nramax: %d\n", nramax);
  printf("ebe1: %10.20f\n", ebe1);
  printf("jvals: %d\n", jvals);
  printf("lfin: %d\n", lfin);
  printf("rmu: %10.20f\n", rmu);
  printf("conno1: %10.20f\n", conno1);
  printf("pi: %10.20f\n", pi);
  printf("vdepth: %10.20f\n", vdepth);
  printf("wal: %10.20f\n", wal);
  printf("wr0: %10.20f\n", wr0);
  printf("wls: %10.20f\n", wls);
  printf("ib: %d\n", ib);
}
