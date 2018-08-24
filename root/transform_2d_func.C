#include <iostream>
#include <iomanip>
#include "TH2D.h"
#include "TProfile.h"
#include "TF1.h"
#include "TF2.h"
#include "TGraph.h"
#include "TFile.h"
#include "TCutG.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TMatrixD.h"
#include "TStyle.h"
#include "TROOT.h"

void FillByTrapezoidAreaInACell(TH2* hout,
				const Double_t a,    const Double_t b,
				const Double_t xlow, const Double_t xup,
				const Double_t ylow, const Double_t yup,
				const Int_t    icol, const Int_t    jrow,
				const Double_t density) {
  Double_t area = 0.;
  if (a == 0.) {
    area = (xup-xlow) * (yup-ylow);
  }else{
    Double_t x_at_ylow = (ylow-b) / a;
    Double_t x_at_yup  = (yup -b) / a;
    area = ((xup - x_at_yup) + (xup - x_at_ylow)) * (yup - ylow) / 2.;
  }
  if (area <= 0) { return; }
  Double_t weight = area * density;
  Double_t cont = hout->GetBinContent(icol,jrow);
  hout->SetBinContent(icol, jrow, weight + cont);
  return;
}

void FillByTrapezoidInAColumn(TH2* hout,
			      const Double_t a1,   const Double_t b1,
			      const Double_t a2,   const Double_t b2,
			      const Double_t xlow, const Double_t xup,
			      const Int_t    icol,
			      const Double_t density) {
  if (xup <= xlow) {return;}
  Double_t xmid = (xlow + xup) / 2.;
  Double_t y1_at_xmid = a1 * xmid + b1;
  Double_t y2_at_xmid = a2 * xmid + b2;
  Double_t a[3], b[3];
  a[1] = 0.; b[1] = 0.;
  if (y1_at_xmid < y2_at_xmid) { a[0] = a1; b[0] = b1; a[2] = a2; b[2] = b2; }
  else                         { a[0] = a2; b[0] = b2; a[2] = a1; b[2] = b1; }
  if (a[0] > 0) { b[0] = 2.*a[0]*xmid+b[0]; a[0] = -a[0]; }
  if (a[2] < 0) { b[2] = 2.*a[2]*xmid+b[2]; a[2] = -a[2]; }
  Double_t ylow[3], yup[3];
  ylow[0] = a[0]*xup +b[0]; yup[0]  = a[0]*xlow+b[0];
  ylow[2] = a[2]*xlow+b[2]; yup[2]  = a[2]*xup +b[2];
  ylow[1] = yup[0];         yup[1]  = ylow[2];
  for (Int_t k=0; k<3; k++){
    if (((k==0)&&(a[0]==0.))||((k==2)&&(a[2]==0.))) { continue; }
    Int_t jlow = hout->GetYaxis()->FindBin(ylow[k]);
    Int_t jup  = hout->GetYaxis()->FindBin(yup[k]);
    Double_t ylow_tmp, yup_tmp;
    for (Int_t j=jlow; j<=jup; j++){
      if (j == jlow) { ylow_tmp = ylow[k]; }
      else           { ylow_tmp = hout->GetYaxis()->GetBinLowEdge(j); }
      if (j == jup)  { yup_tmp  = yup[k];  }
      else           { yup_tmp  = hout->GetYaxis()->GetBinUpEdge(j); }
      FillByTrapezoidAreaInACell(hout,a[k],b[k],xlow,xup,ylow_tmp,yup_tmp,icol,j,density);
    }
  }
  return;
}

void FillByTrapezoidArea(TH2* hout,
			 const Double_t a1, const Double_t b1,
			 const Double_t a2, const Double_t b2,
			 const Double_t x1, const Double_t x2,
			 const Double_t density) {
  Double_t density_with_sign;
  Double_t xlow, xup;
  if (x1 == x2) {
    return;
  }else if (x1 < x2) {
    xlow = x1;
    xup  = x2;
    density_with_sign = density;
  }else{
    xlow = x2;
    xup  = x1;
    density_with_sign = -density;
  }
  Int_t ilow = hout->GetXaxis()->FindBin(xlow);
  Int_t iup  = hout->GetXaxis()->FindBin(xup);
  for (Int_t i = ilow; i <= iup; i++) {
    Double_t xlow_tmp, xup_tmp;
    if (i == ilow) { xlow_tmp = xlow; }
    else           { xlow_tmp = hout->GetXaxis()->GetBinLowEdge(i); }
    if (i == iup)  { xup_tmp  = xup; }
    else           { xup_tmp  = hout->GetXaxis()->GetBinUpEdge(i); }
    Double_t x_intersec = 0.;
    if (a1 != a2)  { x_intersec = -(b1-b2)/(a1-a2); }
    if ((a1 != a2) && (x_intersec > xlow_tmp) && (x_intersec < xup_tmp)) {
      FillByTrapezoidInAColumn(hout,a1,b1,a2,b2,xlow_tmp,x_intersec,i,density_with_sign);
      FillByTrapezoidInAColumn(hout,a1,b1,a2,b2,x_intersec,xup_tmp,i,density_with_sign);
    }else{
      FillByTrapezoidInAColumn(hout,a1,b1,a2,b2,xlow_tmp,xup_tmp,i,density_with_sign);
    }
  }
  return;
}

void FillByQuadrangleArea(TH2* hout,
			  const Double_t *xin, const Double_t *yin,
			  const Double_t weight){
  Double_t xmin   = xin[0];
  Int_t    i_xmin = 0;
  for (Int_t i=1; i<4; i++) {
    if (xin[i] < xmin) {
      xmin   = xin[i];
      i_xmin = i;
    }
  }
  Double_t x[4], y[4];
  Int_t j = i_xmin;
  for (Int_t i=0; i<4; i++) {
    x[i] = xin[j];
    y[i] = yin[j];
    j++;
    if (j == 4) {j = 0;}
  }
  if(x[3] < x[1]) {
    Double_t tmp;
    tmp = x[3]; x[3] = x[1]; x[1] = tmp;
    tmp = y[3]; y[3] = y[1]; y[1] = tmp;
  }
  Double_t a[4], b[4];
  for (Int_t i=0; i<4; i++) {
    Int_t k = i + 1;
    if (k == 4) {k = 0;}
    a[i] = (y[k]-y[i])/(x[k]-x[i]);
    b[i] = y[i] - a[i]*x[i];
  }
  Double_t area_of_quadrangle = (1./2.)*((x[0]*y[1] + x[1]*y[2] + x[2]*y[3] + x[3]*y[0])-
					 (y[0]*x[1] + y[1]*x[2] + y[2]*x[3] + y[3]*x[0]));
  if (area_of_quadrangle == 0.) {
    //std::cout << "area_of_quadrangle is 0!!" << std::endl;
    return;
  }
  /*
    std::cout<< "area_of_quadrangle"<< area_of_quadrangle << std::endl;
    Double_t xx[4],yy[4];
    xx[0] = x[0]; yy[0] = a[0]*x[0]+b[0]; xx[1] = x[1];  yy[1] = a[0]*x[1]+b[0];
    xx[2] = x[1]; yy[2] = a[3]*x[1]+b[3]; xx[3] = xx[2]; yy[3] = yy[2];
    xx[0] = x[1]; yy[0] = a[1]*x[1]+b[1]; xx[1] = x[1];  yy[1] = a[3]*x[1]+b[3];
    xx[2] = x[3]; yy[2] = a[3]*x[3]+b[3]; xx[3] = x[3];  yy[3] = a[1]*x[3]+b[1];
    xx[0] = x[2]; yy[0] = a[2]*x[2]+b[2]; xx[1] = x[2];  yy[1] = a[3]*x[2]+b[3];
    xx[2] = x[3]; yy[2] = a[3]*x[3]+b[3]; xx[3] = xx[2]; yy[3] = yy[2];
    Double_t area_of_quadrangle2 = (1./2.)*((xx[0]*yy[1] + xx[1]*yy[2] + xx[2]*yy[3] + xx[3]*yy[0])-
    (yy[0]*xx[1] + yy[1]*xx[2] + yy[2]*xx[3] + yy[3]*xx[0]));
    std::cout << "area_of_quadrangle2" << area_of_quadrangle2 << std::endl;
  */
  if (area_of_quadrangle < 0.) {
    area_of_quadrangle = -area_of_quadrangle;
  }
  Double_t density = weight / area_of_quadrangle;
  if (x[3] < x[2]) {
    FillByTrapezoidArea(hout,a[0],b[0],a[3],b[3],x[0],x[1],density);
    FillByTrapezoidArea(hout,a[1],b[1],a[3],b[3],x[1],x[3],density);
    FillByTrapezoidArea(hout,a[1],b[1],a[2],b[2],x[3],x[2],density);
  }else{
    FillByTrapezoidArea(hout,a[0],b[0],a[3],b[3],x[0],x[1],density);
    FillByTrapezoidArea(hout,a[1],b[1],a[3],b[3],x[1],x[2],density);
    FillByTrapezoidArea(hout,a[2],b[2],a[3],b[3],x[2],x[3],density);
  }
  return;
}

void doTransformationTH2D(const TH2* hin, TH2* hout,
			  const TF2* fx,   const TF2* fy) {
  Double_t xprim[4], yprim[4];
  for (Int_t i = 1; i <= hin->GetNbinsX(); i++) {
    Double_t xlow = hin->GetXaxis()->GetBinLowEdge(i);
    Double_t xup  = hin->GetXaxis()->GetBinUpEdge(i);
    for (Int_t j = 1; j <= hin->GetNbinsY(); j++) {
      Double_t ylow = hin->GetYaxis()->GetBinLowEdge(j);
      Double_t yup  = hin->GetYaxis()->GetBinUpEdge(j);
      xprim[0] = fx->Eval(xlow, ylow); yprim[0] = fy->Eval(xlow, ylow);
      xprim[1] = fx->Eval(xup,  ylow); yprim[1] = fy->Eval(xup,  ylow);
      xprim[2] = fx->Eval(xup,  yup);  yprim[2] = fy->Eval(xup,  yup);
      xprim[3] = fx->Eval(xlow, yup);  yprim[3] = fy->Eval(xlow, yup);
      Double_t weight = hin->GetBinContent(i,j);
      if (weight == 0.) {continue;}
      FillByQuadrangleArea(hout, xprim, yprim, weight);
    }
  }
  return;
}

void  getInverseXYofPoint(const Int_t nbinx_out, const Int_t nbiny_out,
			  const Int_t i, const Int_t j, const Double_t x, const Double_t y,
			  const Double_t *xtri,  const Double_t *ytri,
			  const Double_t *fxtri, const Double_t *fytri,
			  Double_t *xinv, Double_t *yinv, Bool_t *binv){
  Double_t v0x = x - fxtri[1];
  Double_t v1x = fxtri[0] - fxtri[1];
  Double_t v2x = fxtri[2] - fxtri[1];
  Double_t v0y = y - fytri[1];
  Double_t v1y = fytri[0] - fytri[1];
  Double_t v2y = fytri[2] - fytri[1];
  Double_t deno = v1x*v2y - v2x*v1y;
  if (deno == 0) {std::cout << "deno ==0" << std::endl; return;}
  Double_t coef1 = (1./deno) * ( v2y * v0x - v2x * v0y);
  Double_t coef2 = (1./deno) * (-v1y * v0x + v1x * v0y);
  Double_t sum = coef1 + coef2;
  if ((coef1 >= 0.) && (coef2 >= 0.) && (sum >= 0.) && (sum < 1.)) {
    Int_t pnt = (i-1) * (nbiny_out+1) + j;
    xinv[pnt] = coef1 * (xtri[0] - xtri[1]) + coef2 * (xtri[2] - xtri[1]) + xtri[1];
    yinv[pnt] = coef1 * (ytri[0] - ytri[1]) + coef2 * (ytri[2] - ytri[1]) + ytri[1];
    binv[pnt] = 1;
  }
  return;
}

void  getInverseXYofPoints(const Int_t nbinx_out, const Double_t xmin_out, const Double_t xmax_out,
			   const Int_t nbiny_out, const Double_t ymin_out, const Double_t ymax_out,
			   const Double_t *xtri,  const Double_t *ytri,
			   const Double_t *fxtri, const Double_t *fytri,
			   Double_t *xinv, Double_t *yinv, Bool_t *binv){
  Double_t xmin = fxtri[0], xmax = fxtri[0];
  Double_t ymin = fytri[0], ymax = fytri[0];
  for (Int_t i = 1; i < 3; i++) {
    if (fxtri[i] < xmin) {xmin = fxtri[i];}
    if (fxtri[i] > xmax) {xmax = fxtri[i];}
    if (fytri[i] < ymin) {ymin = fytri[i];}
    if (fytri[i] > ymax) {ymax = fytri[i];}
  }
  Double_t dx = (xmax_out - xmin_out) / nbinx_out;
  Double_t dy = (ymax_out - ymin_out) / nbiny_out;
  Int_t imin = ((int) ((xmin-xmin_out) / dx)) + 1;
  Int_t imax = ((int) ((xmax-xmin_out) / dx)) + 1;
  Int_t jmin = ((int) ((ymin-ymin_out) / dy)) + 1;
  Int_t jmax = ((int) ((ymax-ymin_out) / dy)) + 1;
  if (imax > (nbinx_out+1)) {imax = nbinx_out+1;}
  if (jmax > (nbiny_out+1)) {jmax = nbiny_out+1;}
  if (imin < 1) {imin = 1;}
  if (jmin < 1) {jmin = 1;}
  for (Int_t i = imin; i <= imax; i++) {
    Double_t x = (i-1) * dx  + xmin_out;
    for (Int_t j = jmin; j <= jmax; j++) {
      Double_t y = (j-1) * dy + ymin_out;
      getInverseXYofPoint(nbinx_out, nbiny_out, i, j, x, y,
			  xtri, ytri, fxtri, fytri,
			  xinv, yinv, binv);
    }
  }
  return;
}

void  getInverseXY(const Int_t nbinx_in,  const Double_t xmin_in,  const Double_t xmax_in,
		   const Int_t nbiny_in,  const Double_t ymin_in,  const Double_t ymax_in,
		   const Int_t nbinx_out, const Double_t xmin_out, const Double_t xmax_out,
		   const Int_t nbiny_out, const Double_t ymin_out, const Double_t ymax_out,
		   const TF2* fx, const TF2*  fy,
		   Double_t *xinv, Double_t *yinv, Bool_t *binv){
  Double_t xtri[3],  ytri[3], fxtri[3], fytri[3];
  Double_t dx = (xmax_in - xmin_in) / nbinx_in;
  Double_t dy = (ymax_in - ymin_in) / nbiny_in;
  for (Int_t i = 1; i <= nbinx_in; i++) {
    Double_t xlow = dx * (i-1) + xmin_in;
    Double_t xup  = dx * i     + xmin_in;
    for (Int_t j = 1; j <= nbiny_in; j++) {
      Double_t ylow = dy * (j-1) + ymin_in;
      Double_t yup  = dy * j     + ymin_in;
      xtri[0] = xup;  ytri[0] = ylow;
      xtri[1] = xlow; ytri[1] = ylow;
      xtri[2] = xup;  ytri[2] = yup;
      fxtri[0] = fx->Eval(xtri[0], ytri[0]); fytri[0] = fy->Eval(xtri[0], ytri[0]);
      fxtri[1] = fx->Eval(xtri[1], ytri[1]); fytri[1] = fy->Eval(xtri[1], ytri[1]);
      fxtri[2] = fx->Eval(xtri[2], ytri[2]); fytri[2] = fy->Eval(xtri[2], ytri[2]);
      getInverseXYofPoints(nbinx_out, xmin_out, xmax_out,
			   nbiny_out, ymin_out, ymax_out,
			   xtri, ytri, fxtri, fytri,
			   xinv, yinv, binv);
      xtri[0] = xup;  ytri[0] = yup;
      xtri[2] = xlow; ytri[2] = yup;
      fxtri[0] = fxtri[2];                   fytri[0] = fytri[2];
      fxtri[2] = fx->Eval(xtri[2], ytri[2]); fytri[2] = fy->Eval(xtri[2], ytri[2]);
      getInverseXYofPoints(nbinx_out, xmin_out, xmax_out,
			   nbiny_out, ymin_out, ymax_out,
			   xtri, ytri, fxtri, fytri,
			   xinv, yinv, binv);
    }
  }
  return;
}

void drawInverseFunc(const TH2* hin, const TH2* hout,
		     const TF2* fx,   const TF2* fy) {
  //Int_t    nbinx_in  = hin->GetNbinsX();
  Int_t    nbinx_in  = 50;
  Double_t xmin_in   = hin->GetXaxis()->GetXmin();
  Double_t xmax_in   = hin->GetXaxis()->GetXmax();
  //Int_t    nbiny_in  = hin->GetNbinsY();
  Int_t    nbiny_in  = 50;
  Double_t ymin_in   = hin->GetYaxis()->GetXmin();
  Double_t ymax_in   = hin->GetYaxis()->GetXmax();
  //Int_t    nbinx_out = hout->GetNbinsX();
  Int_t    nbinx_out = 50;
  Double_t xmin_out  = hout->GetXaxis()->GetXmin();
  Double_t xmax_out  = hout->GetXaxis()->GetXmax();
  //Int_t    nbiny_out = hout->GetNbinsY();
  Int_t    nbiny_out = 50;
  Double_t ymin_out  = hout->GetYaxis()->GetXmin();
  Double_t ymax_out  = hout->GetYaxis()->GetXmax();

  Double_t *xinv = new Double_t[(nbinx_out+2)*(nbiny_out+2)];
  Double_t *yinv = new Double_t[(nbinx_out+2)*(nbiny_out+2)];
  Bool_t   *binv = new Bool_t  [(nbinx_out+2)*(nbiny_out+2)];

  for (Int_t i = 0; i < (nbinx_out+2)*(nbiny_out+2); i++) {
    xinv[i] = 0.;
    yinv[i] = 0.;
    binv[i] = 0;
  }

  getInverseXY(nbinx_in,  xmin_in,  xmax_in,
	       nbiny_in,  ymin_in,  ymax_in,
	       nbinx_out, xmin_out, xmax_out,
	       nbiny_out, ymin_out, ymax_out,
	       fx, fy, xinv, yinv, binv);
  TGraph *g1 = new TGraph();
  TGraph *g2 = new TGraph();
  g1->SetLineWidth(1);
  g2->SetLineWidth(1);
  Int_t di = 1;
  Int_t dj = 1;  
  for (Int_t i = 1; i <= nbinx_out+1; i+=di) {
  Int_t k = 0;
    for (Int_t j = 1; j <= nbiny_out+1; j+=dj) {
      Int_t pnt = (i-1) * (nbiny_out+1) + j;
      if (binv[pnt]) {
	g1->SetPoint(k++, xinv[pnt], yinv[pnt]);
      }
    }
    if (g1->GetN() > 0) { 
      g1->DrawGraph(g1->GetN(),g1->GetX(),g1->GetY(),"l");
    }
    g1->Set(0);
  }
  for (Int_t j = 1; j <= nbiny_out+1; j+=dj) {
    Int_t k = 0;
    for (Int_t i = 1; i <= nbinx_out+1; i+=di) {
      Int_t pnt = (i-1) * (nbiny_out+1) + j;
      if (binv[pnt]) {
	g2->SetPoint(k++, xinv[pnt], yinv[pnt]);
      }
    }
    if (g2->GetN() > 0) { 
      g2->DrawGraph(g2->GetN(),g2->GetX(),g2->GetY(),"l");
    }
    g2->Set(0);
  }
  delete [] xinv;
  delete [] yinv;
  delete [] binv;
  return;
}

void transform_2d_func(){
  std::cout<< "The functions in this macro is used from tranfrom_2d.C." << std::endl;
}
