#include "TROOT.h"
#include "TStyle.h"
#include "TF1.h"
#include "TH1.h"
#include "TLatex.h"
#include <iostream>
#include <vector>
#include <string>

double hamamoto_sph_hankel_1st(int n, double x) {
  /*same as the output of the AHNKEV subroutine in the Hamamoto's code*/
  double AHNKEV;
  double R=x;
  double RI=1.0/R;
  double EX=exp(-R);
  if (n == 0) {
    AHNKEV = EX*RI;
  }else if (n == 1){
    AHNKEV = EX*(RI+RI*RI);
  }else{
    double G_L   = EX*RI;
    double G_L_1 = EX*(RI+RI*RI);
    double G_L_2 = 0.;
    int    LL2   = (n+1)-2;
    double FL    = 1.0;
    for (int L = 1; L <= LL2; L++){
      double TWELP1 = 2.0*FL+1.0;
      G_L_2 = TWELP1*RI*G_L_1+G_L;
      G_L = G_L_1;
      G_L_1 = G_L_2;
      FL = FL+1.0;
    }
    AHNKEV = G_L_2;
  }
  return AHNKEV;
}

double hamamoto_sph_neumann(int M, double W) {
  /* Same as Hamamoto's SBSJN Fortran function*/
  double SBSYN = 0.;
  double X = W;
  double N = M;
  if (X <= 0.) {
    std::cout << "  INCORRECT ARGUMENT(S) IN SBSYN  " <<std::endl;
  }
  if (N < 0) {
    std::cout << "  INCORRECT ARGUMENT(S) IN SBSYN  " <<std::endl;
  }
  double S0 = -cos(X)/X;
  if (N != 0) {
    double S1 = (S0-sin(X))/X;
    if (N != 1) {
      double CONST = 2./X;
      double Y0 = S0;
      double Y1 = S1;
      double Y2 = 0.;
      for (int I = 2; I <= N; I++){
	Y2 = CONST*(I-.5)*Y1-Y0;
	Y0 = Y1;
	Y1 = Y2;
      }
      SBSYN = Y2;
    }else{
      SBSYN = S1;
    }
  }else{
    SBSYN = S0;
  }
  return SBSYN;
}

double hamamoto_sph_bessel(int NN, double XX) {
  /* Same as Hamamoto's SBSJN Fortran function*/
  double SBSJN;
  double FL=0.,CONST,FK[3],DSBSJN=0.;
  bool NLTX;
  double BJ[3],BBJ[3];
  /*    EQUIVALENCE (BBJ,FK) */
  double POWLOG = .11512925E+03;
  /* F(U)=(U+AB)/(ALOG(U)+1.); */
  int N = NN+1;
  if (N < 1) {
    std::cout << "  INCORRECT ARGUMENT(S) IN SBSJN"<< std::endl;
    return 0.;
  }
  double X = XX;
  if (X < 0.) {
    std::cout << "  INCORRECT ARGUMENT(S) IN SBSJN"<< std::endl;
    return 0.;
  }else if (X == 0.) {
    SBSJN=0.;
    if (N == 1) {SBSJN = 1.;}
    return SBSJN;
  }
  NLTX = false;
  BJ[1] = sin(X)/X;
  if(N == 1) {
    SBSJN = BJ[N];
    return SBSJN;
  }
  if (X < 0.5) {
    BJ[2] = (((1.-(X*X)/54.)*(X*X)/280.-.1)*(X*X)+1.)*X/3.;
  }else{
    BJ[2] = (BJ[1]-cos(X))/X;
  }
  if (N == 2) {
    SBSJN = BJ[N];
    return SBSJN;
  }
  NLTX = (N<(X+.5));
  CONST = 2.0/X;
  if (NLTX) {
    FK[1] = BJ[1];
    FK[2] = BJ[2];
    for (int I = 3; I <= N; I++) {
      FL = CONST*(I-1.5)*FK[2]-FK[1];
      FK[1] = FK[2];
      FK[2] = FL;
    }
    SBSJN = FL;
    return SBSJN;
  }
  double HEX = 1.3591409*X;
  int K;
  if (X < 251.) {
    int K10 = 1.02*X+21.;
    /* int M = std::max(K10, NN + 11 + (int)(.04*X)); */
    int M = 0;
    if (K10 > (NN + 11 + (int)(.04*X))) {
      M = K10;
    }else{
      M = (NN + 11 + (int)(.04*X));
    }
    if ((X-.99) < 0) {
      K = 172.69388/(3.6888795-log(X));
      /*K = (std::max(std::min(M,K),4)/2)*2+1;*/
      if (M < K) {K = M;}
      if (K > 4) {K = (K/2)*2+1;}
      else       {K = (4/2)*2+1;}
    }else{
      K = (M/2)*2+1;
      if (HEX > K+.5) { // label: 120
      }else{
	double AB = POWLOG/HEX;
	/* F(U)=(U+AB)/(ALOG(U)+1.); */
	double U = AB+1.;
	double F_U = (U+AB)/(log(U)+1.);
	double F_F_U  = (F_U+AB)/(log(F_U)+1.);
	/* K = std::min(K,(int)(HEX*F_F_U-.5)); */
	if (K > ((int)(HEX*F_F_U-.5))) {K = ((int)(HEX*F_F_U-.5));}
      }
    }
  }else{
    double SONST = CONST;
    BBJ[1] = 0.;
    BBJ[2] = 1.;
    K = N;
    while (true) {
      double AB = SONST*(K+.5)*BBJ[2]-BBJ[1];
      if(AB > 1.E08) {break;}
      BBJ[1] = BBJ[2];
      BBJ[2] = AB;
      K = K+1;
    }
    if (HEX > K+.5) { // label: 120
    }else{
      double AB = POWLOG/HEX;
      /* F(U)=(U+AB)/(ALOG(U)+1.); */
      double U = AB+1.;
      double F_U = (U+AB)/(log(U)+1.);
      double F_F_U  = (F_U+AB)/(log(F_U)+1.);
	/* K = std::min(K,(int)(HEX*F_F_U-.5)); */
      if (K > ((int)(HEX*F_F_U-.5))) {K = ((int)(HEX*F_F_U-.5));}
    }
  }
  if (K > N) { // label: 54
  }else{
    SBSJN=0.;
    return SBSJN;
  }
  FK[1] = 1.E-29; // label: 540
  FK[2] = 0.;
  int I;
  while (true) {
    for (I = K; I >= N; I--) {
      FL = CONST*(I+.5)*FK[1]-FK[2];
      FK[2]=FK[1];
      FK[1]=FL;
    }
    if (N == 1) {break;}
    DSBSJN = FL;
    K = N-1;
    N = 1;
  }
  I = 1;
  if (abs(BJ[1]) < abs(BJ[2])) {I = 2;}
  SBSJN = BJ[I]*(double)(DSBSJN/FK[I]);
  return SBSJN;
}

double deriv_x_sph_bessel(int n, double x) {
  /* d(j_n(x))/dx = -j_n+1(x) + (n/x)*j_n(x)
     d(x*j_n(x))/dx
     = j_n(x) + x*d(j_n(x))/dx
     = j_n(x) -x*j_n+1(x) + n*j_n(x) */
  return hamamoto_sph_bessel(n,x)
    -x*hamamoto_sph_bessel(n+1,x)
    +n*hamamoto_sph_bessel(n,x);
}

double deriv_x_sph_neumann(int n, double x) {
  /* d(y_n(x))/dx = -y_n+1(x) + (n/x)*y_n(x)
     d(x*y_n(x))/dx
     = y_n(x) + x*d(y_n(x))/dx
     = y_n(x) -x*y_n+1(x) + n*y_n(x) */
  return hamamoto_sph_neumann(n,x)
    -x*hamamoto_sph_neumann(n+1,x)
    +n*hamamoto_sph_neumann(n,x);
}

double x_sph_bessel_neumann(int n, double x, double ps) {
  return cos(ps)*x*hamamoto_sph_bessel(n,x)
    -sin(ps)*x*hamamoto_sph_neumann(n,x);
}

class WaveFunction {
public:
  WaveFunction(Int_t l2in, Int_t j2in,
               Double_t rmassin, Double_t vin, Double_t ain, Double_t Rin,
               Double_t V0in, Double_t Eljin) 
    : l1(l2in/2), l2(l2in), j1(j2in/2.), j2(j2in),
      rmass(rmassin), v(vin), a(ain),  R(Rin), 
      V0(V0in), Elj(Eljin),
      ndiv(300), rmin(-0.1), rmax(59.9), dr((rmax-rmin)/ndiv),
      //hbarc(197.3269718),
       hbarc(197.32891),/*  hbarc value in Hamamoto-san's code (main-eg.f)*/
      /*hbarc(197.11),  hbarc value in Hamamoto-san's code (cc-nilson-g.f)*/
      ls((j2*(j2+2) - l2*(l2+2) - 1*(1+2)) / 8.),
      hLambda(hbarc/(2.*rmass)), cfunit_inv(2.*rmass/(hbarc*hbarc))
  {
    calc_faclog();
    /* we use wf[1][0]-wf[1][ndiv+2]*/
    /* output : wf[1][1]-wf[1][ndiv]*/
    wf[1] = new double[ndiv+3];
    r     = new double[ndiv+3];
    for (Int_t i = 0; i <= ndiv; i++) {
      r[i] = dr * (i+0.5)+rmin;
    }
    stormer(1,ndiv+2,&(wf[0]),r);
    Double_t ps      = FindPS(ndiv);
    std::cout << "Phase shift" << ps/acos(-1.) << std::endl;

    Double_t kouter  = sqrt(2.0*rmass*Elj)/hbarc;
    Double_t normfac = sqrt(cfunit_inv/(3.141592653589793238*kouter));
    Double_t uouter  = normfac*x_sph_bessel_neumann(l1, kouter*r[ndiv], ps);
    Double_t uratio  = uouter/wf[1][ndiv];
    if (uouter*wf[1][ndiv] < 0) {
      uratio = -uratio;
    }
    for (Int_t i = 1; i <=ndiv; i++) {
      wf[1][i] = uratio*wf[1][i];
    }
    for (Int_t i = ndiv+1; i > 1; i--) {
      wf[1][i] = wf[1][i-1];
      r[i] = r[i-1];
    }
    wf[1][1] = 0.;
    r[1] = 0.;
  }
  WaveFunction(Int_t nodein, Int_t l2in, Int_t j2in,
               Double_t rmassin, Double_t vin, Double_t ain, Double_t Rin,
               Int_t opt, Double_t energy)
    : node(nodein), l1(l2in/2), l2(l2in), j1(j2in/2.), j2(j2in),
      rmass(rmassin), v(vin), a(ain), R(Rin), 
      ndiv(300), rmin(-0.1), rmax(59.9), dr((rmax-rmin)/ndiv),
      //hbarc(197.3269718),
       hbarc(197.32891),/*  hbarc value in Hamamoto-san's code (main-eg.f)*/
      /*hbarc(197.11),  hbarc value in Hamamoto-san's code (cc-nilson-g.f)*/
      ls((j2*(j2+2) - l2*(l2+2) - 1*(1+2)) / 8.),
      hLambda(hbarc/(2.*rmass)), cfunit_inv(2.*rmass/(hbarc*hbarc))
  {
    calc_faclog();
    //std::cout <<  "hLambda: " << hLambda << std::endl;
    //std::cout <<  "0.44*1.27*1.27: " << 32.*2.*hLambda*hLambda/1.27/1.27 << std::endl;
    //std::cout <<  "0.44*1.27*1.27: " << 0.44*1.27*1.27 << std::endl;
    
    wf[1] = new double[ndiv+4];
    r     = new double[ndiv+4];
    for (Int_t i = 0; i <= ndiv+3; i++) {
      r[i] = dr * (i+0.5)+rmin;
    }
    Int_t n = (Int_t) (Rin/0.2);

    Double_t kmin;
    Double_t kmax;
    if (node == 0) {
      kmin = 0.00001;
      kmax = GetZeroPosOfj(l1,0) / R;
    }else{
      kmin = GetZeroPosOfj(l1,node-1) / R;
      kmax = GetZeroPosOfj(l1,node) / R;
    }
    Double_t EljminusV0_min = pow((kmin*hbarc),2)/(2*rmass);
    Double_t EljminusV0_max = pow((kmax*hbarc),2)/(2*rmass);
    if (opt == 0) {
      Elj = energy;
      Double_t V0_min = Elj - EljminusV0_max;
      Double_t V0_max = Elj - EljminusV0_min;
      V0 = FindEne(n, opt, V0_min, V0_max);
    }else{
      V0 = energy;
      //Double_t Elj_min = EljminusV0_min + V0;
      //Double_t Elj_maxn = EljminusV0_max + V0;
      //Double_t Elj_min = 0.9*(EljminusV0_min + V0);
      //Double_t Elj_max = EljminusV0_max + V0;
      Double_t Elj_min = 0.95*(1.05*EljminusV0_min + V0);
      Double_t Elj_max = 0.95*(1.05*EljminusV0_max + V0);
      std::cout << "Elj_min = " << Elj_min << ", Elj_max = " << Elj_max << std::endl;
      if (Elj_min > 0) {
        exit (0);
        Elj_min = 0.;
      }
      if (Elj_max > 0) {Elj_max = 0.;}
      Elj = FindEne(n, opt, Elj_min, Elj_max);
    }
    //std::cout << " " << std::endl;
    std::cout << "N,l2,j2: " <<  node << "," << l2 << "," << j2 <<  std::endl;
    //std::cout << "V0 = " << V0 << " MeV" << std::endl;
    std::cout << "Elj = " << Elj << " MeV" << std::endl;

    stormer(1,n,&(wf[0]),r);
    Double_t uinner   = wf[1][n];
    stormer(ndiv+3,n,&(wf[0]),r);
    Double_t uouter   = wf[1][n];
    Double_t uratio = uinner/uouter;
    //std::cout << "uratio,uinner,uouter" << uratio<< "," << uinner<< "," << uouter << std::endl;

    for (Int_t i = n; i <= ndiv; i++) {
      wf[1][i] = uratio*wf[1][i];
    }
    Double_t sum = 0;
    for (Int_t i = ndiv; i >= 1; i--) {
      sum = sum + pow(wf[1][i],2);
    }
    Double_t norm = 1./sqrt(sum*dr);
    for (Int_t i = 1; i <=ndiv; i++) {
      wf[1][i] = wf[1][i] * norm;
    }
    for (Int_t i = ndiv+1; i > 1; i--) {
      wf[1][i] = wf[1][i-1];
      r[i] = r[i-1];
    }
    wf[1][1] = 0.;
    r[1] = 0.;
  }
  ~WaveFunction(){
    delete[] wf[1];
    delete[] r;
  }
  
private:
  Double_t FindPS(Int_t n) {
    Double_t a_outer = sqrt(2.0*rmass*Elj)/hbarc;
    Double_t x_outer = a_outer * r[n];
    Double_t slope = (8.0*(wf[1][n+1]-wf[1][n-1])-(wf[1][n+2]-wf[1][n-2]))/(12.*dr);
    Double_t Llj = r[n]*slope/wf[1][n];

    Double_t fcosps = Llj * hamamoto_sph_neumann(l1,x_outer)
      - deriv_x_sph_neumann(l1,x_outer);
    Double_t fsinps = Llj * hamamoto_sph_bessel(l1,x_outer)
      - deriv_x_sph_bessel(l1,x_outer);
    Double_t cosps = fcosps / sqrt(pow(fcosps,2.0)+pow(fsinps,2.0));
    Double_t pstmp = acos(cosps);
    if (fsinps < 0) {pstmp = -pstmp;}
    if (pstmp < 0)  {pstmp = pstmp + 3.141592653589793238;}
    return pstmp;
  }
  
  Double_t FindEne(Int_t n, Int_t opt, Double_t ene_min, Double_t ene_max) {
    if (opt == 0) {V0  = ene_min;}
    if (opt == 1) {Elj = ene_min;}
    
    stormer(ndiv+3,n-2,&(wf[0]),r);
    Double_t numerator_outer   = wf[1][n];
    std::cout << "numerator_outer = " << numerator_outer << std::endl;
    Double_t denominator_outer = (8.0*(wf[1][n+1]-wf[1][n-1])-(wf[1][n+2]-wf[1][n-2]))/(12.*dr);
    //Double_t denominator_outer = wf[1][n-1];
    stormer(1,n+2,&(wf[0]),r);
    Double_t numerator_inner   = wf[1][n];
    Double_t denominator_inner = (8.0*(wf[1][n+1]-wf[1][n-1])-(wf[1][n+2]-wf[1][n-2]))/(12.*dr);
    //Double_t denominator_inner = wf[1][n-1];
    Double_t delta0 = numerator_outer*denominator_inner
      - numerator_inner*denominator_outer;
    for (Int_t k=0; k < 50; k++){
      if (opt == 0) {V0  = 0.5*(ene_min+ene_max);}
      if (opt == 1) {Elj = 0.5*(ene_min+ene_max);}
      stormer(ndiv+3,n-2,&(wf[0]),r);
      numerator_outer   = wf[1][n];
      denominator_outer = (8.0*(wf[1][n+1]-wf[1][n-1])-(wf[1][n+2]-wf[1][n-2]))/(12.*dr);
      //denominator_outer = wf[1][n-1];      
      stormer(1,n+2,&(wf[0]),r);
      numerator_inner   = wf[1][n];
      denominator_inner = (8.0*(wf[1][n+1]-wf[1][n-1])-(wf[1][n+2]-wf[1][n-2]))/(12.*dr);
      //denominator_inner = wf[1][n-1];
      Double_t delta = numerator_outer*denominator_inner
        - numerator_inner*denominator_outer;
      if (delta0*delta > 0) {
        if (opt == 0) {ene_min = V0;}
        if (opt == 1) {ene_min = Elj;}
      }else{
        if (opt == 0) {ene_max = V0;}
        if (opt == 1) {ene_max = Elj;}
      }
    }
    if (opt == 0) {return V0;}
    if (opt == 1) {return Elj;}
    return 0;
  }

  void stormer(Int_t n1, Int_t n2, Double_t **y, Double_t xx[]) {
    Double_t pre41=19./240.;
    Double_t pre42=-2./5.;
    Double_t pre43=97./120.;
    Double_t pre44=-11./15.;
    Double_t pre45=299./240.;
    Double_t fprer[6];
    Double_t dxx_var[5] = {0.000, 0.025, 0.050, 0.1, 0.2};
    Double_t xx_var[318] = 
      {0.000,
       0.025,0.050,0.075,0.100,
       0.125,0.150,0.175,0.200,
       0.250,0.300,0.350,0.400,
       0.500,0.600,0.700,0.800,
      };
    for (Int_t i = 17;i<315;i++){
      xx_var[i] = (i-17)*0.2+1.0;
    }
    if (n1 < n2) {
      Int_t    n_start[6] = {0, 5, 9, 13, 17};
      n_start[5] = n2+16;
      Double_t fprer_stored[318];
      Double_t wf_stored[318];
      Double_t k_inner = sqrt(2.0*rmass*(Elj-V0))/hbarc;
      wf_stored[0] = 0.;
      //wf_stored[1] = (0.5/k_inner)*pow(2.*dxx_var[1]*k_inner,l1+1)*exp(faclog[l1+1]-faclog[2*(l1+1)]);
      wf_stored[1] = (0.5/k_inner)*pow(2.*dxx_var[1]*k_inner,l1+1)*exp(0.0);
      for (Int_t i=2;i<=5;i++) {
        wf_stored[i] = 2.*wf_stored[i-1]-wf_stored[i-2]+pow(dxx_var[1],2)*pot(xx_var[i-1])*wf_stored[i-1];
      }
      fprer[1]        = 0.0;
      fprer_stored[0] = fprer[1];
      if(l1==1) {
        fprer[1] = 0.6666666666*pow(dxx_var[1],2)*k_inner;
        fprer_stored[0] = fprer[1];
      }
      for (Int_t i=2;i<=5;i++) {
        fprer[i] = wf_stored[n1+i-2]*pow(dxx_var[1],2)*pot(xx_var[n1+i-2]);
        fprer_stored[i-1] = fprer[i];
      }
      for (Int_t inex=1; inex<=4; inex++){
        for (Int_t k=n_start[inex]; k<n_start[inex+1]; k++) {
          Double_t termr = pre41*fprer[1]+pre42*fprer[2]+pre43*fprer[3]+pre44*fprer[4]+pre45*fprer[5];
          if (inex!=1 && k==n_start[inex]) {
            wf_stored[k] = 2.*wf_stored[k-1]-wf_stored[k-3]+termr;
          }else{
            wf_stored[k] = 2.*wf_stored[k-1]-wf_stored[k-2]+termr;
          }
          for (Int_t i=1;i<=4;i++) {
            fprer[i]=fprer[i+1];
          }
          fprer[5]=wf_stored[k]*pow(dxx_var[inex],2)*pot(xx_var[k]);
          fprer_stored[k]=fprer[5];
        }
        if (inex == 1) {
          for (Int_t i=0;i<=4;i++) {
            fprer[i+1] = fprer_stored[i*2]*4.0;
          }
        } else if (inex == 2) {
          fprer[5] = fprer_stored[12]*4.0; /*n=12,x=0.4*/
          fprer[4] = fprer_stored[10]*4.0; /*n=10,x=0.3*/
          fprer[3] = fprer_stored[8]*16.0; /*n=8,x=0.2*/
          fprer[2] = fprer_stored[4]*16.0; /*n=4,x=0.1*/
          fprer[1] = fprer_stored[0]*16.0; /*n=0,x=0.0*/
        } else if (inex == 3) {
          fprer[5] = fprer_stored[16]*4.0;  /*n=16,x=0.8*/
          fprer[4] = fprer_stored[14]*4.0;  /*n=14,x=0.6*/
          fprer[3] = fprer_stored[12]*16.0; /*n=12,x=0.4*/
          fprer[2] = fprer_stored[8]*64.0;  /*n=8,x=0.2*/
          fprer[1] = fprer_stored[0]*64.0;  /*n=0,x=0.0*/
        }
      } 
      y[1][1] = wf_stored[8];
      y[1][2] = wf_stored[12];
      y[1][3] = wf_stored[14];
      y[1][4] = wf_stored[16];
      for (Int_t i=5;i<=n2;i++) {
        y[1][i] = wf_stored[i+12];
      }
    }else{
      Double_t k_outer = sqrt(2.0*rmass*fabs(Elj))/hbarc;
      for (Int_t i=0;i<5;i++) {
        y[1][n1-i] = r[n1-i]*hamamoto_sph_hankel_1st(l1,k_outer*r[n1-i]);
      }
      for (Int_t i=0;i<5;i++) {
        fprer[i+1] = y[1][n1-i]*pow(dr,2)*pot(xx[n1-i]);
      }
      for (Int_t k=n1-5;k>=n2;k--) {
        Double_t termr = 
          pre41*fprer[1]+
          pre42*fprer[2]+
          pre43*fprer[3]+
          pre44*fprer[4]+
          pre45*fprer[5];
        y[1][k] = 2.*y[1][k+1]-y[1][k+2]+termr;

        for (Int_t i=1;i<=4;i++) {
          fprer[i]=fprer[i+1];
        }
        fprer[5]=y[1][k]*pow(dr,2)*pot(xx[k]);
      }
    }
  }

  Double_t pot(Double_t x)
  {
    Double_t cfb = l1*(l1+1)/pow(x,2);
    Double_t tmp = (x-R)/a;
    Double_t ex, f, V, Vso,Vsum;
    ex   = exp(tmp);
    f    = 1./(1.+ex);
    V    = V0 * f;
    Vso  = -V0*v*pow(hLambda,2)/x*(-ex*pow(f,2)/a)*2.*ls;
    //Vso  = -V0*0.44*pow(1.27,2)/x*(-ex*pow(f,2)/a)*ls;
    Vsum = cfb-cfunit_inv*(Elj-V-Vso);
    return Vsum;
  }

  Double_t GetZeroPosOfj(Int_t n, Int_t i) {
    Double_t zeropos[9][6] = {
      {3.14159, 6.28319, 9.42478, 12.5664, 15.7080, 18.8496},
      {4.49341, 7.72525, 10.9041, 14.0662, 17.2208, 20.3713},
      {5.76346, 9.09501, 12.3229, 15.5146, 18.6890, 21.8539},
      {6.98793, 10.4171, 13.6980, 16.9236, 20.1218, 23.3042},
      {8.18256, 11.7049, 15.0397, 18.3013, 21.5254, 24.7276},
      {9.35581, 12.9665, 16.3547, 19.6532, 22.9046, 26.1278},
      {10.5128, 14.2074, 17.6480, 20.9835, 24.2628, 27.5079},
      {11.6570, 15.4313, 18.9230, 22.2953, 25.6029, 28.8704},
      {12.7908, 16.6410, 20.1825, 23.5913, 26.9270, 30.2173}};
    if ((n < 0)||(n > 8)||(i < 0)||(i > 5)) {
      std::cout << "error in GetZerosPosOfj" << std::endl;
      return -1;
    }
    return zeropos[n][i];
  }
  
  void calc_faclog() {
    faclog[1]=(Float_t)0.0;
    faclog[2]=(Float_t)0.0;
    Double_t FN=(Float_t)1.0;
    for (Int_t  N=3; N<=500; N++) {
      FN = FN + (Float_t)1.0;
      faclog[N]=faclog[N-1]+(Float_t)log(FN);
    }
  }

  
public:
  Double_t* Getwf(){return wf[1];}
  Double_t* GetVcenter(){
    Vcenter = new double[ndiv+1];
    for (Int_t i=1;i<=ndiv;i++) {
      Double_t tmp = (r[i]-R)/a;
      Double_t ex, f, V;
      ex   = exp(tmp);
      f    = 1./(1.+ex);
      V    = V0 * f;
      Vcenter[i] = V;
    }
    return Vcenter;
  }
  Double_t GetV(){return V0;}
  Double_t GetE(){return Elj;}
  
private:
  Int_t    node, l1, l2;
  Double_t j1;
  Int_t    j2;
  Double_t rmass, v, a, R, V0, Elj;
  Int_t    ndiv;
  Double_t rmin, rmax, dr;
  Double_t hbarc, ls, hLambda, cfunit_inv;
  Double_t faclog[501];
  Double_t *wf[2+1];
  Double_t *r;
  Double_t *Vcenter;
};

void make_hamamoto_wf() {
  const Int_t n_orb = 9;

  std::vector<std::string>  orb_str;
  orb_str.push_back("1#kern[0.1]{#it{s}_{1/2}}");
  orb_str.push_back("1#kern[0.1]{#it{p}_{3/2}}");
  orb_str.push_back("1#kern[0.1]{#it{p}_{1/2}}");
  orb_str.push_back("1#kern[0.1]{#it{d}_{5/2}}");
  orb_str.push_back("2#kern[0.1]{#it{s}_{1/2}}");
  orb_str.push_back("1#kern[0.1]{#it{d}_{3/2}}");
  orb_str.push_back("1#kern[0.1]{#it{f}_{7/2}}");
  orb_str.push_back("2#kern[0.1]{#it{p}_{3/2}}");
  orb_str.push_back("continuum");

  WaveFunction* wf[n_orb];
  wf[0] = new WaveFunction(0,0,1,(30./31.)*939.577218339,32.,0.67,1.27*pow(30,1./3.),1,-46.75);
  wf[1] = new WaveFunction(0,2,3,(30./31.)*939.577218339,32.,0.67,1.27*pow(30,1./3.),1,-46.75);
  wf[2] = new WaveFunction(0,2,1,(30./31.)*939.577218339,32.,0.67,1.27*pow(30,1./3.),1,-46.75);
  wf[3] = new WaveFunction(0,4,5,(30./31.)*939.577218339,32.,0.67,1.27*pow(30,1./3.),1,-46.75);
  wf[4] = new WaveFunction(1,0,1,(30./31.)*939.577218339,32.,0.67,1.27*pow(30,1./3.),1,-46.75);
  wf[5] = new WaveFunction(0,4,3,(30./31.)*939.577218339,32.,0.67,1.27*pow(30,1./3.),1,-46.75);
  wf[6] = new WaveFunction(0,6,7,(30./31.)*939.577218339,32.,0.67,1.27*pow(30,1./3.),1,-46.75);
  wf[7] = new WaveFunction(1,2,3,(30./31.)*939.577218339,32.,0.67,1.27*pow(30,1./3.),1,-46.75);
  wf[8] = new WaveFunction(2,3,(30./31.)*939.577218339,32.,0.67,1.27*pow(30,1./3.),-46.75,0.1);
  
  gROOT->cd();
  TH1D* h[n_orb];
  for (Int_t i =0;i<n_orb;i++) {
    h[i] = new TH1D(Form("h%d",i),orb_str[i].c_str(),300,-0.1,59.9);
    h[i]->SetContent(wf[i]->Getwf());
  }
}
