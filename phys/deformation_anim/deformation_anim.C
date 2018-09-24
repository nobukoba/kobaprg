#include "TSystem.h"
#include "TStyle.h"
#include "TH2.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGFrame.h"
#include "TGLSAViewer.h"
#include "TGLLightSet.h"
#include "TGLViewer.h"
#include "TGLCamera.h"
#include "TGLOrthoCamera.h"

void deformation_anim(){
  gStyle->SetLegoInnerR(0.);
  //gStyle->SetCanvasPreferGL(true);
  Double_t pi = 3.14159265358979323846;
  
  /* Give parameters */
  Int_t num_div = 40;
  Double_t ini_beta  = -0.8;
  Double_t fin_beta  =  0.8;
  /* End of the given parameters*/
  
  Int_t num_step = num_div * 2;
  Double_t del_beta = (fin_beta - ini_beta ) / num_div;
  Int_t len_arr  = num_step  + 1;
  Double_t *beta_arr = new Double_t[len_arr];

  Double_t beta_off0 = 0.0001;
  
  for (Int_t i = 0; i <= num_div; i++){
    beta_arr[i] = ini_beta + del_beta * i;
    if(beta_arr[i] == 0.){
      beta_arr[i] = beta_off0;
    }
  }
  for (Int_t i = num_div + 1; i <= (num_step - 1); i++){
    beta_arr[i] = ini_beta + del_beta * (num_step-i);
    if(beta_arr[i] == 0.){
      beta_arr[i] = beta_off0;
    }
  }
  
  TCanvas* c1 = new TCanvas("c1","c1");
  TH2D * h = new TH2D("h","h",21,0.,2*pi,21,0.,pi);
  TGLViewer * glViewer = 0;

  Int_t i_seq = 0;
  gSystem->Exec("mkdir -p def_anim");
  for (Int_t k = 0; k <= (num_step - 1); k++) {
    //for (Int_t k = 80; k <=80; k++) {
    Double_t beta = beta_arr[k];
    h->Reset();
    for(Int_t i = 1; i <= 21; i++){
      for(Int_t j = 1; j <= 21; j++){
	Double_t ph = h->GetXaxis()->GetBinCenter(i);
	Double_t th = h->GetYaxis()->GetBinCenter(j);
	h->Fill(ph,th,1.+beta*sqrt(5./16./pi)*(3*pow(cos(th),2)-1.));
      }
    }
    h->SetBinContent(5,10,0); /* absolute value should corresponds color*/
    h->SetMaximum(1.);
    h->SetMinimum(0.);
  
    c1->SetPhi(0.);
    c1->SetTheta(30.);
    h->Draw("surf1,sph,ah");

    if (glViewer) {glViewer->Delete();}
    glViewer = (TGLViewer*)(gPad->GetViewer3D("ogl"));
    glViewer->SetCurrentCamera(TGLViewer::kCameraOrthoZOY);

    TString fname;
    if (beta >= 0.){
      fname = Form("def_anim/beta_%03d_p%.2f.gif", i_seq, beta);
    }else{
      fname = Form("def_anim/beta_%03d_m%.2f.gif", i_seq, -beta);
    }
    glViewer->SavePictureScale(fname,.5);
    i_seq++;
    if (beta == beta_off0) {
      for (Int_t m = 0; m < num_div/4; m++) {
	fname = Form("def_anim/beta_%03d_p%.2f.gif", i_seq, beta);
	glViewer->SavePictureScale(fname,.5);
	i_seq++;
      }
    }
  }
  
  if (glViewer) {glViewer->Delete();}
  delete [] beta_arr;
  return;


}

//h->SetContour(99);
////((TGMainFrame *)(((TGLSAViewer*)glViewer)->GetFrame()))->Resize(500, 300);
//Double_t center[3] = {0.,0.,0.};
////glViewer->SetOrthoCamera(TGLViewer::kCameraOrthoZOY,10.0,10.0,center_xyz,90.,90.);
//
////glViewer->GetLightSet()->SetFrontPower(0.1);
////glViewer->SetPerspectiveCamera(TGLViewer::kCameraPerspYOZ,10.0,10.0,center_xyz,90.,90.);
//
//glViewer->SetCurrentCamera(TGLViewer::kCameraOrthoXOZ);
//////glViewer->CurrentCamera().SetExternalCenter(kTRUE);
//TGLOrthoCamera& o = glViewer->CurrentCamera();
//o.SetEnableRotate(kTRUE);
////
////glViewer->SetOrthoCamera(TGLViewer::kCameraOrthoXOZ,0.001,0.01.,center,10.,10.);
////glViewer->RefreshPadEditor(glViewer);
//
////glViewer->SetCurrentCamera(TGLViewer::kCameraOrthoZOY);
//Double_t a = -0.2;
//Double_t b = -1.;
//TGLVector3 v1=TGLVector3(-cos(a),sin(a)*cos(b),sin(a)*sin(b));
//TGLVector3 v2=TGLVector3(sin(a)*sin(b),cos(a),-sin(a)*cos(b));
//
//glViewer->ReinitializeCurrentCamera(v1,v2,1);

//TGLViewer::ECameraType camera = TGLViewer::kCameraOrthoXOZ;
//TGLViewer *v = (TGLViewer *)gPad->GetViewer3D();
//v->SetCurrentCamera(camera);
//v->CurrentCamera().SetExternalCenter(kTRUE);
//TGLOrthoCamera& o = v->CurrentCamera();
//o.SetEnableRotate(kTRUE);
//
//Double_t zoom      = 0.001;
//Double_t dolly     = 3000;
//Double_t center[3] = {0,0,0};
//Double_t hRotate   = 180.;
//Double_t vRotate   = 90.;
//
//v->SetOrthoCamera(camera, zoom, dolly, center, hRotate, vRotate);
//
//v->RequestDraw();

//glViewer->RefCamera(TGLViewer::kCameraPerspYOZ).RotateRad(30.,30.);
//glViewer->SetCurrentCamera(TGLViewer::kCameraPerspYOZ);
  
//glViewer->SetLineScale(1.);

