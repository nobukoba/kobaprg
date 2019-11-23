#include "TRootCanvas.h"
#include "TEnv.h"
#include "TStyle.h"

void print(){
  //Int_t ret;
  //char *pri="aa";
  //char *cmd="bb";
  //new TGPrintDialog(gClient->GetDefaultRoot(),0,400,150,&pri,&cmd,&ret);
  
  //Int_t ret;
  //char *pri="aa";
  //char *cmd="bb";
  //new TGPrintDialog(gClient->GetDefaultRoot(),0,400,150,&pri,&cmd,&ret);

  TRootCanvas * rc = ((TGFrameElement*)((TRootBrowser*)gBrowserEx->GetBrowserImp())->GetTabRight()->GetTabContainer(0)->GetList()->First())->fFrame;
  printf("rc %s\n",rc->GetName());
  gEnv->SetValue("Print.Printer", "-Pa3c4476");
  gEnv->SetValue("Print.Command","lpr");
  gEnv->SetValue("Print.FileType","ps");
  gStyle->SetPaperSize(20,26);
  rc->PrintCanvas();
  return;
}
