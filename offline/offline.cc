#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "command.h"

extern void hstdef_(char *);
extern void event1_(unsigned short *);
extern void hstend_();

int main(int argc,char *argv[]){
  int fd;
  int size, s;
  int  blk_endflag;
  int  ana_endflag;
  unsigned short *buf;
  unsigned short *evt, *ptr;
  blk_header_p    hdr;
  int n_evt, n_sca;
  //  unsigned long sca; // this is for 32-bit PC, nobu 20180920
  unsigned int sca; // this is for 64-bit PC, nobu 20180920
  int i, n;
  
  if (argc <=2 ){
    printf("Usage: %s dat_file_name output_root_name\n", argv[0]);
    exit(-1);
  }

  /* open the file to analyze */
  if((fd=open(argv[1], O_RDONLY))<0){
    printf("Could not open file: %s\n", argv[1]);
    exit(-1);
  }

  /* allocate memory */
  buf = (unsigned short*)malloc(BLK_SIZE*2);
  if(buf==(unsigned short*)NULL){
    printf("Could not allocate memory for buf.\n");
    exit(-1);
  }

  /* define histograms */
  hstdef_(argv[2]);

  /* start reading the file */
  ana_endflag = 0;
  n_evt = 0;
  while(!ana_endflag){
    size = 0;
    while(size<BLK_SIZE){
      s=read(fd, &((char*)buf)[size], BLK_SIZE-size);
      if(s==0){
	usleep(100000);
      }
      if(s<0){
	printf("Error in read()\n");
	perror("read");
	break;
      }
      size +=s;
    }
    if(size<BLK_SIZE) break;

    hdr = (blk_header_p)buf;
    evt = (unsigned short*)&hdr[1];
    if(hdr->header1!=BLK_HEADER1 || hdr->header2!=BLK_HEADER2){
      printf("Unknown header: %.2x:%.2x\n.", hdr->header1, hdr->header2);
      break;
    }
    switch(hdr->blk_version){
    case BLK_VERSION_1_0:
    case BLK_VERSION_1_1:
      break;
    default:
      printf("Unsupported format version: %.4x\n.", hdr->blk_version);
      exit(1);
    }
      
    switch(hdr->blk_type){
    case BLK_TYPE_START:
      printf("Start block.\n");
      printf("  Comment: %s\n", (char*)evt);
      break;
    case BLK_TYPE_DATA:
      evt = (unsigned short*)&hdr[1];
      printf("Block: %6d\n", hdr->blk_num);
      blk_endflag=0;
      while(!blk_endflag){
	switch(*evt){
	case EVENT_HEADER:
	  n_evt++;
	  event1_(evt);
	  break;
	case SCALER_HEADER:
	  n_sca++;
	  n = (evt[1]-2)/2;
          ptr = &evt[2];
	  for(i=0; i<n; i++){
	    sca = ptr[0]+ptr[1]*65536;
	    ptr += 2;
	    //printf("%10ld", sca); // This is for 32 bit
	    printf("%10d", sca);  // This is for 64 bit
	    if((i+1)%8==0)
  	      printf("\n");
	  }
	  break;
	case BLK_DELIMITER:
	  blk_endflag =1;
	  break;
	default:
	  printf("Unknown event header: %.4x\n", *evt);
	  blk_endflag = 1;
	  break;
	}
	evt = &evt[evt[1]];  /* advance the event pointer */
      }
      break;
    case BLK_TYPE_END:
      printf("End block.\n");
      printf("  Comment: %s\n", (char*)evt);
      printf("  %10d events were analyzed..\n", n_evt);
      ana_endflag = 1;
      break;
    default:
      printf("Unknown block type: %.4x\n.", hdr->blk_type);
      break;
    }
  }
  
  hstend_();
  exit(0);
}
