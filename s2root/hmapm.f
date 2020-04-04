*
* $Id: hmapm.F,v 1.3 2001/10/02 09:06:15 couet Exp $
*
* $Log: hmapm.F,v $
* Revision 1.3  2001/10/02 09:06:15  couet
* - The following program didn't work. It could not access the shared memory
*   area named 'ASA'.
*
*        Program shm_read
*        Common /PAWC/ A(100000)
*        Call Hlimit(100000)
*        Call Hlimap(0,'ASA')
*        End
*
*   To make it work the shared memory name had to be put in a character
*   variable, ie:
*
*        Program shm_read
*        Common /PAWC/ A(100000)
*        Character*8 C
*        Call Hlimit(100000)
*        C = 'ASA'
*        Call Hlimap(0,C)
*        End
*
* Revision 1.2  1996/05/14 10:28:52  couet
* - Mods for ALPHA OSF
*
* Revision 1.1.1.1  1996/01/16 17:08:09  mclareni
* First import
*
*
*CMZ :  4.21/08 05/01/94  09.03.21  by  Rene Brun
*-- Author :    Fons Rademakers   20/03/91
*-- Modified:   Wojtek Burkot     01/03/92
      INTEGER FUNCTION HMAPM(MFILE, IBASE, IOFFST)
************************************************************************
*                                                                      *
* HMAPM                                                                *
*                                                                      *
* Create a global section. This routine causes the pages at ICOMAD     *
* to be mapped from file MFILE. MFILE must have been created in the    *
* /tmp directory. It returns in IOFFST the offset between ICOMAD and   *
* a base common IBASE (typically the PAWC). On successful completion   *
* this function returns 0. In case of an error -ERRNO is returned.     *
*                                                                      *
* HMAPM is an interface routine that calls the C routine HMAPI.        *
*                                                                      *
* After a global section has been created by HCREATEM, other           *
* processes can use the data in the global section via this function   *
* and HFREEM.                                                          *
*                                                                      *
************************************************************************
*
      CHARACTER*(*) MFILE
      INTEGER       ICOMAD(1), IBASE(1), HMAPI
      SAVE ICOMAD
*
************************************************************************
*  modification: set ICOMAD in input to required starting address of   *
*  mapping. Actual mapping address will be returned in IOFFST.         *
*  This requires also a change in C routine HMAPI.                     *
*                                 02/03/1992    W.B.                   *
************************************************************************
      CHARACTER*(128) MFILE2
      PARAMETER (MFEN=100000000)

      MFILE2 = MFILE
      CALL UCTOH(MFILE2,MKEY,4,4)
      ICOMAD(1) = 0
C this is for Intels IA64 architectures (data are in 0x6000000000000000)
C on AMD64 this yields 0 and will not change ICOMAD(1)
C c/o Nobu 2018/01/26 20:12:47-->
C      ICOMAD(1) = LOC (ICOMAD(1)) - LOCB(ICOMAD(1))
C --> Eed
C this should be a reasonable range for IA64 architectures
C Added Nobu 2018/01/26 20:13:07 -->
      ICOMAD(1) = ICOMAD(1) + 2**30
C --> End
      HMAPM = HMAPI(MKEY, ICOMAD)
      IOFFST = ICOMAD(1) - LOCF(IBASE(1))
*
      END
