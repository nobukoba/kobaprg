/// \file
/// \ingroup tutorial_http
///
///  The objects are filled with some random numbers and saved on a in-memory file.
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
#include <TROOT.h>
#include <TSystem.h>
#include <TBenchmark.h>
#include <TInterpreter.h>
#include <THttpServer.h>

#include "TMessage.h" /* Nobu --> */
#include "TSocket.h"
#include "TError.h" /* --> Nobu added 2020/04/30 */

void display_client2_multi_shm(const char * shm_names = "TEST",
			       const char * roothost = "localhost",
			       const Int_t  rootport = 9090,
			       const Int_t  httpport = 8080,
			       const char*  jobname = "job1",
			       Long64_t     maxcnt = 0) {
  // Open connection to server
   Int_t status, version, kind;
   TSocket *sock = new TSocket(roothost, rootport);
   if (!sock->IsValid()) {
      Error("display_client2","Could not establish a connection with the server %s:%d.",rootname,rootport);
      return;
   }

   // Wait till we get the start message
   // server tells us who we are
   sock->Recv(status, kind);
   if (kind != 0 /* kStartConnection */)
   {
      Error("display_client2","Unexpected server message: kind=%d status=%d\n",kind,status);
      delete sock;
      return;
   }
   sock->Recv(version, kind);
   if (kind != 1 /* kStartConnection */)
   {
      Fatal("display_client2","Unexpected server message: kind=%d status=%d\n",kind,status);
   } else {
      Info("display_client2","Connected to fastMergeServer version %d\n",version);
   }

   TMessage::EnableSchemaEvolutionForAll(kFALSE);

   // http server with port 8080, use jobname as top-folder name
   THttpServer* serv = new THttpServer(Form("http:%d?top=%s", httpport, jobname));
   
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

   TString str_shm_names = shm_names;
   TList shm_name_list;
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
     std::cout << ostr->GetString().Data() << std::endl;
   }

   TMemFile *transient = 0;
   TList file_list;
   TMessage mess(kMESS_OBJECT);
   TObjString * ostr2;
   while (1){
     TIter next2(&shm_name_list);
     while ((ostr2 = (TObjString*)next2())){
       for (Int_t i=0;i<1000;i++) {
	 if (gSystem->ProcessEvents()) return;
	 gSystem->Sleep(1); /* Sleep for 1 ms */ 
       }
       mess.Reset(kMESS_ANY);              /* re-use TMessage object */
       mess.WriteTString(ostr2->GetString());
       sock->Send(mess);
       TMessage *mess2;
       sock->Recv(mess2);
       Int_t shm_result;
       mess2->ReadInt(shm_result);
       if(shm_result){
	 Info("ana_read2","Shared memory read succeeded %d", shm_result);
       }else{
	 Info("ana_read2","Shared memory read failed! %d", shm_result);
	 continue;
       }
       sock->Recv(mess2);
       Long64_t length;
       TString filename;
       mess2->ReadTString(filename);
       mess2->ReadLong64(length); /* '*mess >> length;' is broken in CINT for Long64_t. */
       
       Info("s2root_server2","Receive input from client for %s",filename.Data());
       TObject * tmp = file_list.FindObject(filename.Data());
       if (tmp) {
	 file_list.Remove(tmp);
	 delete tmp;
       }
       transient = new TMemFile(filename,mess2->Buffer() + mess2->Length(),length);
       file_list.Add(transient);
       delete mess2; 
       transient->ls();
     }
   }
   sock->Send("Finished");          /* tell server we are finished */
   gBenchmark->Show("hclient");
   /* Close the socket */
   sock->Close();
   gBenchmark->Show(jobname);
}
