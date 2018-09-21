#ifndef h2root_func_h
#define h2root_func_h

#ifndef __CINT__
#ifdef __cplusplus
extern "C" {
#endif
#endif
  class TDirectoryFile;
extern int  init_h2root();
extern int  open_file(const char *);
extern void convert_directory_shm();
extern void convert_directory(const Int_t, TDirectoryFile *);
extern void convert_1d(Int_t, TDirectoryFile *);
extern void convert_2d(Int_t, TDirectoryFile *);
extern void convert_profile(Int_t, TDirectoryFile *);
extern void convert_rwn(Int_t, TDirectoryFile *);
extern void convert_cwn(Int_t, TDirectoryFile *);

#ifndef __CINT__
#ifdef __cplusplus
}
#endif
#endif
#endif
