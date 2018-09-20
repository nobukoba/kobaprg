/*  command.h ... header file for   CCNET-DAQ      */
/*  Version 1.00  20-JUN-2007  by A. Tamii         */

#define SHMKEY (0x4343444E)
#define SEMKEY (SHMKEY)

#define BLK_SIZE       0x10000
#define BLK_RED_REGION  0x0400
#define BLK_HEADER1     0x4343   /* 'CC' */
#define BLK_HEADER2     0x444E   /* 'ND' */
#define BLK_VERSION_1_0 0x0100   /* ver 1.0 */
#define BLK_VERSION_1_1 0x0101   /* ver 1.1 */
#define BLK_VERSION     (BLK_VERSION_1_1)
#define BLK_DELIMITER   0xffef
#define BLK_TYPE_START  0x0001
#define BLK_TYPE_DATA   0x0002
#define BLK_TYPE_END    0x0003

#define EVENT_HEADER    0xffdf
#define SCALER_HEADER   0xffcf

#define MAX_COMMENT_LEN 0x0100

#define MAX_N_BUF       1

typedef unsigned short blk_data_t;
typedef blk_data_t*    blk_data_p;

typedef struct blk_header{
  unsigned short   header1;    /* 'CC' */
  unsigned short   header2;    /* 'ND' */
  unsigned short   blk_version;/* = 0x0100, ver 1.0 at present */
  unsigned short   res1;       /* reserved for run number in future*/
  unsigned short   blk_num;    /* block number starts with 0 */
  unsigned short   blk_type;   /* block type, BLK_TYPE_xxx */
  unsigned short   blk_size;   /* fixed block size (unit = 256 bytes) */
  unsigned short   blk_dsize;  /* block data size (unit = 2 bytes) */
  unsigned short   byte_order1;
  unsigned short   byte_order2;
  unsigned short   time1;      /* UTC time-stamp#1 (lower word) */
  unsigned short   time2;      /* UTC time-stamp#2 (upper word) */
  unsigned short   res5;       /* reserved */
  unsigned short   res6;       /* reserved */
  unsigned short   res7;       /* reserved */
  unsigned short   res8;       /* reserved */
} blk_header_t, *blk_header_p;

#define CCND_PORT             4325   /* TCP/IP connection port */
#define TCP_FORMAT_VERSION_1 0x0100  /* TCP/IP connection format version (1.0)*/
#define TCP_FORMAT_VERSION  (TCP_FORMAT_VERSION_1)
                                     /* TCP/IP connection format version */
#define DEFAULT_HOST_NAME     "ccnetb"
#define N_SHM_BUF 10

typedef struct shm_t {
  int      exit;                 /* exit of the execture             */
  int      acq;                  /* status of the data acquisition   */
  int      cmd;                  /* command to the executer          */
  int      reply;                /* reply from the executer          */
  int      trbuf;                /* transfer a buffer                */
  int      fd_data;              /* data file descriptor             */
  char     fnam[256];            /* name of the raw data file        */
  char     comment[256];         /* comment to be written at header  */
  // This is for 32-bit PC
  //unsigned long  rawcnt;         /* written event numbers            */
  //unsigned long  anacnt;         /* analyzed event numbers           */
  //unsigned long  errcnt;         /* error event numbers              */

  // This is for 64-bit PC
  unsigned int  rawcnt;         /* written event numbers            */
  unsigned int  anacnt;         /* analyzed event numbers           */
  unsigned int  errcnt;         /* error event numbers              */

  int      blk_size;             /* block size                       */
  unsigned char buf[BLK_SIZE];
                                /* data buffer for on-line analyzer  */
} shm_t, *shm_p;


/* semaphore */
enum SEM {
  SEM_CMD = 0,  /* semaphore for the shared memory command access */
  SEM_BUF,      /* semaphore for the shared memory buffer        */
  N_SEM
};

/* command */
enum CMD {
  CMD_NONE = 0,   /* no command */
  CMD_INIT,       /* initialize */
  CMD_EXIT,       /* exit */
  CMD_OPEN,       /* open a data file */
  CMD_CLOSE,      /* close the data file */
  CMD_START,      /* start run */
  CMD_STOP,       /* stop run */
  CMD_BUFFER,     /* change buffer */
  CMD_HELP,       /* help */
  N_CMD           /* number of commands */
};

/* reply */
enum REPLY{
  REPLY_NONE = 0,      /* no reply */
  REPLY_SUCCESS,       /* success */
  REPLY_UNDER_RUN,     /* already under run (to start) */
  REPLY_NOTUNDER_RUN,  /* not under run yet (to stop)  */
  REPLY_ALREADY_OPEN,  /* file is already opened (to open)  */
  REPLY_NOT_OPEN,      /* file is not opened (to close)  */
  REPLY_OPEN_ERROR,    /* error on opening the file */
  REPLY_STOP_BEFORE_OPEN,  /* stop run before open */
  REPLY_STOP_BEFORE_CLOSE, /* stop run before close */
  REPLY_STOP_BEFORE_INIT,  /* stop run before init */
  REPLY_STOP_BEFORE_EXIT,  /* stop run before exit */
  REPLY_CLOSE_BEFORE_EXIT, /* close file before exit */
  REPLY_TIMEOUT,       /* timeout for reply */
  N_REPLY
};

static char *reply_messages[] = {
  "No reply",
  "Success",
  "Already under run",
  "Not under run",
  "File is already opened",
  "File is not opened",
  "Error on opening a file",
  "Stop run before opening a file",
  "Stop run before closing a file",
  "Stop run before initializing",
  "Stop run before exit",
  "Close file before exit",
  "Reply timeout",
};


