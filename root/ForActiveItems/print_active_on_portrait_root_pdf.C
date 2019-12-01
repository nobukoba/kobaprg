void print_active_on_portrait_root_pdf() {
  gROOT->ProcessLine(".L ./print_active_on_root_pdf.C");
  gROOT->ProcessLine("print_active_on_root_pdf(\"Portrait\")");
  return;
}
