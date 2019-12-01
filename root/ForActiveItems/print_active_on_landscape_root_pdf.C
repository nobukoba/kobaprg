void print_active_on_landscape_root_pdf() {
  gROOT->ProcessLine(".L ./print_active_on_root_pdf.C");
  gROOT->ProcessLine("print_active_on_root_pdf(\"Landscape\")");
  return;
}
