void print_root_pdf() {
  TString sprinter  = gEnv->GetValue("Print.Printer", "");
  TString sprintCmd = gEnv->GetValue("Print.Command", "lpr -o raw");
  TString fn = "../../root.pdf";
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
  gSystem->Exec(cmd);
  return;
}
