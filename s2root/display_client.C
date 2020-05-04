/// \file
/// \ingroup tutorial_http
///  This program creates :
///    - a one dimensional histogram
///    - a two dimensional histogram
///    - a profile histogram
///    - a memory-resident ntuple
///
///  These objects are filled with some random numbers and saved on a in-memory file.
///  All objects can be seen in web browser is open url:
/// ~~~
///      http://localhost:8080
/// ~~~
///
/// \macro_code
///
/// \author Sergey Linev

#include <TFile.h>
#include <TMemFile.h>
#include <TNtuple.h>
#include <TH2.h>
#include <TProfile.h>
#include <TCanvas.h>
#include <TFrame.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TRandom3.h>
#include <TBenchmark.h>
#include <TInterpreter.h>
#include <THttpServer.h>

#include "TMessage.h" /* Nobu --> */
#include "TSocket.h"
#include "TTree.h"
#include "TError.h" /* --> Nobu added 2020/04/30 */
#include <time.h> /* Nobu 2020/04/15 */

void display_client(const char* jobname = "job1", Long64_t maxcnt = 0)
{
  
//  TString filename = Form("%s.root", jobname);
//   TFile *hfile = new TMemFile(filename,"RECREATE","Demo ROOT file with histograms");
//
//   // Create some histograms, a profile histogram and an ntuple
//   TH1F *hpx = new TH1F("hpx","This is the px distribution",100,-4,4);
//   hpx->SetFillColor(48);
//   TH2F *hpxpy = new TH2F("hpxpy","py vs px",40,-4,4,40,-4,4);
//   TProfile *hprof = new TProfile("hprof","Profile of pz versus px",100,-4,4,0,20);
//   TNtuple *ntuple = new TNtuple("ntuple","Demo ntuple","px:py:pz:random:i");
//   hfile->Write();

         // Open connection to server
   Int_t status, version, kind;
   TSocket *sock = new TSocket("localhost", 9090);
   //TSocket *sock = new TSocket("miho-1.rcnp.osaka-u.ac.jp", 9090);
   //printf("here1\n");
   //TSocket *sock = new TSocket("koba-desktop.rcnp.osaka-u.ac.jp", 9090);
   //printf("here2\n");
   if (!sock->IsValid()) {
      Error("treeClient","Could not establish a connection with the server %s:%d.","localhost",9090);
      return;
   }

   // Wait till we get the start message
   // server tells us who we are
   sock->Recv(status, kind);
   if (kind != 0 /* kStartConnection */)
   {
      Error("treeClient","Unexpected server message: kind=%d status=%d\n",kind,status);
      delete sock;
      return;
   }
   sock->Recv(version, kind);
   if (kind != 1 /* kStartConnection */)
   {
      Fatal("treeClient","Unexpected server message: kind=%d status=%d\n",kind,status);
   } else {
      Info("treeClient","Connected to fastMergeServer version %d\n",version);
   }

   int idx = status;


   TMessage::EnableSchemaEvolutionForAll(kFALSE);
   TMessage mess(kMESS_OBJECT);

   // http server with port 8080, use jobname as top-folder name
   THttpServer* serv = new THttpServer(Form("http:8080?top=%s", jobname));

   // fastcgi server with port 9000, use jobname as top-folder name
   // THttpServer* serv = new THttpServer(Form("fastcgi:9000?top=%s_fastcgi", jobname));

   // dabc agent, connects to DABC master_host:1237, works only when DABC configured
   // THttpServer* serv = new THttpServer(Form("dabc:master_host:1237?top=%s_dabc", jobname));

   // when read-only mode disabled one could execute object methods like TTree::Draw()
   serv->SetReadOnly(kFALSE);

   // One could specify location of newer version of JSROOT
   // serv->SetJSROOT("https://root.cern.ch/js/latest/");
   // serv->SetJSROOT("http://jsroot.gsi.de/latest/");

   gBenchmark->Start(jobname);

   // Fill histograms randomly
   //TRandom3 random;
   //Float_t px, py, pz;
   //const Int_t kUPDATE = 1000;
   //Long64_t i = 0;

   // Fill histogram randomly
   const int kUPDATE = 1000;
   Long64_t i = 0;
   TMemFile *transient = 0;
   while (true) {
     i++;
     if (i &&(i%kUPDATE)==0) {
       TMessage *mess2;
       sock->Send("test");
       //mess.Reset();
       //sock->Recv(&mess);
       sock->Recv(mess2);
       
       Long64_t length;
       TString filename;
       Int_t clientId;
       mess2->ReadInt(clientId);
       mess2->ReadTString(filename);
       mess2->ReadLong64(length); // '*mess >> length;' is broken in CINT for Long64_t.

       //mess.ReadInt(clientId);
       //mess.ReadTString(filename);
       //mess.ReadLong64(length); // '*mess >> length;' is broken in CINT for Long64_t.

       printf("here1\n");
       Info("fastMergeServer","Receive input from client %d for %s",clientId,filename.Data());
       printf("here2\n");
       delete transient;
       printf("here3\n");
       transient = new TMemFile(filename,mess2->Buffer() + mess2->Length(),length);
       //transient = new TMemFile(filename,mess.Buffer() + mess.Length(),length);
       //mess2->SetBufferOffset(mess2->Length()+length);
       delete mess2; 
       printf("here4\n");
       transient->ls();
       /*sleep(1);*/
       printf("here5\n");
     }
     if (gSystem->ProcessEvents()) break;
   }
   sock->Send("Finished");          // tell server we are finished
   
   
   gBenchmark->Show("hclient");
   
   // Close the socket
   sock->Close();
   
   
   //while (true) {
   //
   //  TDirectory *cursav = gDirectory;
   //  
   //  random.Rannor(px,py);
   //   pz = px*px + py*py;
   //   Float_t rnd = random.Rndm(1);
   //   hpx->Fill(px);
   //   hpxpy->Fill(px,py);
   //   hprof->Fill(px,pz);
   //   // fill only first 25000 events in NTuple
   //   if (i<25000) ntuple->Fill(px,py,pz,rnd,i);
   //   if (i && (i%kUPDATE) == 0) {
   //      if (i == kUPDATE) hpx->Draw();
   //      c1->Modified();
   //      c1->Update();
   //      if (i == kUPDATE) hfile->Write();
   //
   //      if (gSystem->ProcessEvents()) break;
   //   }
   //   i++;
   //   if ((maxcnt>0) && (i>=maxcnt)) break;
   //
   //}

   gBenchmark->Show(jobname);
}
