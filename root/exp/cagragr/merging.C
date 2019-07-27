Int_t merging() {

  const Int_t nfiles = 1024;
  const Int_t nsets  = 4;
  TString filenames[nsets][nfiles];
  Int_t   set_runs[nsets][nfiles];
  Int_t   set_nrun[nsets] = {0};
  TString str_head = "./run";
  TString str_tail = "/hist_MakeClover_Nobu_20190727_1st.root";
  Int_t set_runs[nsets][nfiles];
  Int_t set_nrun[nsets] = {0};

  for(Int_t i = 2066; i <= 2111; i++) { set_runs[0][(set_nrun[0])++] = i; }
  for(Int_t i = 2112; i <= 2114; i++) { set_runs[1][(set_nrun[1])++] = i; }
  for(Int_t i = 2115; i <= 2118; i++) { set_runs[2][(set_nrun[2])++] = i; }
  for(Int_t i = 2119; i <= 2200; i++) { set_runs[3][(set_nrun[3])++] = i; }
  
  for (Int_t j = 0; j < nsets; j++) {
    for (Int_t i = 0; i < set_nrun[j]; i++) {
      filenames[j][i] = str_head + Form("%d",set_runs[j][i]) + str_tail; 
      std::cout << "set: " << j << ", " << filenames[j][i] << std::endl;
    }
  }
}
