void print_root_pdf() {
  HistBrowser *pHistBrowser_tmp = (HistBrowser *)gROOT->ProcessLine("pHistBrowser;");
  if (pHistBrowser_tmp) {
    gSystem->cd((pHistBrowser_tmp->GetInitialWorkingDir()).Data());
  }else{return;}
  std::cout << "gSystem->pwd(): " << gSystem->pwd() << std::endl;

  TString sprinter  = gEnv->GetValue("Print.Printer", "");
  TString sprintCmd = gEnv->GetValue("Print.Command", "lpr -o raw");
  TString fn = "./root.pdf";
  TString cmd = sprintCmd;
  if (cmd.Contains("%p"))
    cmd.ReplaceAll("%p", sprinter);
  else if (sprinter) {
    cmd += " "; cmd += sprinter; cmd += " ";
  }
  
  if (cmd.Contains("%f"))
    cmd.ReplaceAll("%f", fn);
  else {
    cmd += " "; cmd += fn; cmd += " ";
  }
  std::cout << "Command: " << cmd << std::endl;
  gSystem->Exec(cmd);
  return;
}
