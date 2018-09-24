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

  Double_t beta_arr[32];
  Double_t dbeta = 0.1;
  Double_t fbeta = -0.8;
  for (Int_t i = 0; i <= 16; i++){
    beta_arr[i] = fbeta + dbeta * i;
    if(beta_arr[i] == 0.){
      beta_arr[i] = 0.001;
    }
  }
  for (Int_t i = 17; i <= 31; i++){
    beta_arr[i] = fbeta + dbeta * (32-i);
    if(beta_arr[i] == 0.){
      beta_arr[i] = 0.001;
    }
  }
  
  TCanvas* c1 = new TCanvas("c1","c1");
  TH2D * h = new TH2D("h","h",21,0.,2*pi,21,0.,pi);
  //for (Int_t k = 0; k <=31; k++) {
  for (Int_t k = 16; k <=16; k++) {
    h->Reset();
    Double_t beta = beta_arr[k];
    for(Int_t i = 1; i<=21; i++){
      for(Int_t j = 1; j<=21; j++){
	Double_t ph = h->GetXaxis()->GetBinCenter(i);
	Double_t th = h->GetYaxis()->GetBinCenter(j);
	h->Fill(ph,th,1+beta*sqrt(5./16./pi)*(3*pow(cos(th),2)-1.));
      }
    }
    h->SetBinContent(5,10,0);
    h->SetMaximum(0.8);
    h->SetMinimum(0.);
  
    c1->SetPhi(0.);
    c1->SetTheta(30.);
    //h->SetContour(99);
    //h->Draw("glsurf1 sph");
    //top->Draw("gl same");
    //h->Draw("surf1,sph,ah,fb,bb");
    h->Draw("surf1,sph,ah");
    //h->Draw("surf1");
    //h->Draw("surf1,sph,lc");
    TGLViewer * glViewer = (TGLViewer*)(gPad->GetViewer3D("ogl"));
    glViewer->SetCurrentCamera(TGLViewer::kCameraOrthoZOY);
    //TString fname;
    //if (beta >= 0.){
    //  fname = Form("def_nuc/beta_%02d_p%.2f.gif",k,beta);
    //}else{
    //  fname = Form("def_nuc/beta_%02d_m%.2f.gif",k,-beta);
    //}
    ////((TGMainFrame *)(((TGLSAViewer*)glViewer)->GetFrame()))->Resize(500, 300);
    //glViewer->SavePictureScale(fname,.5);
    //glViewer->Delete();
  }
  
  return;
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

}

