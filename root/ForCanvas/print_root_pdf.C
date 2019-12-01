void print_root_pdf() {
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx) {
    gSystem->cd((gBrowserEx->GetInitialWorkingDir()).Data());
  }else{return;}
  std::cout << "gSystem->pwd(): " << gSystem->pwd() << std::endl;
  
  TString sprinter  = gBrowserEx->GetPrinter();
  TString sprintCmd = gBrowserEx->GetPrintCmd();
  Int_t ret = 0;
  Bool_t pname = kTRUE;
  char *printer, *printCmd;
  
  if (sprinter == "")
    printer = StrDup(gEnv->GetValue("Print.Printer", ""));
  else
    printer = StrDup(sprinter);
  if (sprintCmd == "")
    printCmd = StrDup(gEnv->GetValue("Print.Command", ""));
  else
    printCmd = StrDup(sprintCmd);
  
  new TGPrintDialog((TRootBrowser*)gClient->GetDefaultRoot(),
		    (TRootCanvas*)gPad->GetCanvas()->GetCanvasImp(),
		    400, 150,
		    &printer, &printCmd, &ret);
  if (ret) {
    sprinter  = printer;
    sprintCmd = printCmd;
    gBrowserEx->SetPrinter(sprinter.Data());
    gBrowserEx->SetPrintCmd(sprintCmd.Data());
    
    if (sprinter == "")
      pname = kFALSE;
      
    TString fn = "./root.pdf";
    TString cmd = sprintCmd;
    if (cmd.Contains("%p"))
      cmd.ReplaceAll("%p", sprinter);
    else if (pname) {
      cmd += " "; cmd += sprinter; cmd += " ";
    }
    if (cmd.Contains("%f"))
      cmd.ReplaceAll("%f", fn);
    else {
      cmd += " "; cmd += fn; cmd += " ";
    }
    gSystem->Exec(cmd);
  }
  delete [] printer;
  delete [] printCmd;
  return;
}
