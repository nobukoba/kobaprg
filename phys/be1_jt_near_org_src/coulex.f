*--------------------------------------------------------------------------
*   This program calculates the electric dipole Coulomb dissociation
*   cross sections (in millibarns) for input B(E1) distributions
*--------------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,O-Z)
      CHARACTER*30 BE1,EXS,FROOT
      REAL*8 PI,AMU,ESQ,HBARC,BIND,ALPHA,ZT,AT,ZP,AP,ZB,AB,ZC,
     & AC,ECA,ECAI(4),ELAB,GAMMA,BETA,EX,SUM(4),SIG(4),
     & DB1,ACLOSE,G(4),RTOTAL,BESSK0,BESSK1,BK0,BK1,V1,
     & DELEX,XI
      LOGICAL DONE8,DONE10,DONE12,DONE14,DONE4,DONE6,DONE3
      LOGICAL DONE05,DONE1,DONE2
      INTEGER*4 I, NINI
*--------------------------------------------------------------------------
*     CONSTANTS
*--------------------------------------------------------------------------
      PI=ACOS(-1.)
      AMU=938.0
      HBARC=197.327
      ALPHA=1./137.036
      ESQ=HBARC*ALPHA
*--------------------------------------------------------------------------
*     print*,'ESQ ',ESQ
*     PROJECTILE CORE THEN VALENCE
*--------------------------------------------------------------------------
      PRINT*,' Projectile mass and separation energy'
      read*,AP,BIND
      PRINT*,' Give input file name with B(E1) distribution'
      READ(*,*)BE1
      PRINT*,' Give Target ZT  AT, and bmin'
      READ(*,*)ZT,AT,BMIN
*--------------------------------------------------------------------------
*     OVERRIDE READ BMIN VALUE WITH THE KOBAYASHI PRESCRIPTION
*     IF BMIN > 0
*--------------------------------------------------------------------------
      IF(BMIN.GT.0.0) THEN
      BMIN=1.45*(AT**.3333333333+AP**.33333333333333)
      ELSE
      BMIN=-BMIN
      ENDIF
      PRINT*,' bmin = ',BMIN
*--------------------------------------------------------------------------
      PRINT*,' Give 4 beam energies per nucleon (0s to end) '
      READ(*,*)(ECAI(ie),ie=1,4)
      print *,' B(E1) in file ',BE1
      open(3,file=be1)
      ihead = index(be1,'.')
      if(ihead.ne.0) froot = be1(1:ihead-1)
      if(ihead.eq.0) froot = be1
      k=lnblnk(froot)
      exs=froot(1:k)//'.exs'
      open(4,file=exs)
      print *,' Cross sections in file ',exs
      DO 200 ie=1,4
      ECA = ECAI(ie)
      IF(ECA.le.0.0) go to 200
      ELAB=ECA*AP
      GAMMA=ECA/AMU+1.
      BETA=SQRT(GAMMA**2-1.)/GAMMA
      NINI=2000
      rewind 3
      read(3,*) 
      DELEX = 0.0
      DONE05=.false.
      DONE1=.false.
      DONE2=.false.
      DONE3=.false.
      DONE4=.false.
      DONE6=.false.
      DONE8=.false.
      DONE10=.false.
      DONE12=.false.
      DONE14=.false.
      SIGG=0.
*     FOR SIMPSON RULE
      GG=2.
*     START LOOP ON ENERGIES
      DO I=1,NINI
       ELAST = DELEX
       read(3,*,err=99,end=99) DELEX,DB1
       STEP=DELEX-ELAST
       EX=BIND+DELEX
*--------------------------------------------------------------------------
*      CHANGED TO BMIN FOR AGREEMENT WITH TAKASHI NAKAMURA
*--------------------------------------------------------------------------
       RTOTAL=BMIN
*--------------------------------------------------------------------------
*      BERTULANI/BAUR NPA 442 (1985)739 Eq. (3.5) HAS A GAMMA FACTOR HERE
*--------------------------------------------------------------------------
       XI=EX*RTOTAL/HBARC/BETA/GAMMA
       BK0=BESSK0(XI)
       BK1=BESSK1(XI)
*      FOR SIMPSON RULE
       GG=6.-GG
*--------------------------------------------------------------------------
*      BERTULANI/BAUR FORMULAE
*--------------------------------------------------------------------------
       VX=2.*ZT**2*ALPHA/PI/BETA**2*(XI*BK0*BK1-((BETA*XI)**2)/2.*
     &          (BK1**2-BK0**2))
*--------------------------------------------------------------------------
*      OUTPUT THE VIRTUAL PHOTON SPECTRUM
*--------------------------------------------------------------------------
       write(18,*) real(DELEX),VX
*--------------------------------------------------------------------------
*      OUTPUT THE CROSS SECTION VERSUS CONTINUUM ENERGY
*--------------------------------------------------------------------------
       SIGX=VX*16.*PI**3/9.*ALPHA*DB1
       write(4,*) real(DELEX),real(10.*SIGX)
       SIGG=SIGG+GG*SIGX*STEP*10./3.
*--------------------------------------------------------------------------
*      if(abs(DELEX-0.3).lt.0.001) write(28,*) real(BMIN),10.d0*SIG(1)
*--------------------------------------------------------------------------
       if(.not.DONE05.and.DELEX.gt.0.5) THEN
       print*,'--------------------------------------------------------'
       PRINT *,'Sum to  0.5 MeV for ',real(ECA),' MeV/A:',real(SIGG)
       DONE05=.true.
       endif
       if(.not.DONE1.and.DELEX.gt.1.0) THEN
        PRINT *,'Sum to  1.0 MeV for ',real(ECA),' MeV/A:',real(SIGG)
        DONE1=.true.
       endif
       if(.not.DONE2.and.DELEX.gt.2.0) THEN
        PRINT *,'Sum to  2.0 MeV for ',real(ECA),' MeV/A:',real(SIGG)
        DONE2=.true.
       endif
       if(.not.DONE3.and.DELEX.gt.3.0) THEN
        PRINT *,'Sum to  3.0 MeV for ',real(ECA),' MeV/A:',real(SIGG)
        DONE3=.true.
       endif
       if(.not.DONE4.and.DELEX.gt.4.0) THEN
        PRINT *,'Sum to  4.0 MeV for ',real(ECA),' MeV/A:',real(SIGG)
        DONE4=.true.
       endif
       if(.not.DONE6.and.DELEX.gt.6.0) THEN
        PRINT *,'Sum to  6.0 MeV for ',real(ECA),' MeV/A:',real(SIGG)
        DONE6=.true.
       endif
       if(.not.DONE8.and.DELEX.gt.8.0) THEN
        PRINT *,'Sum to  8.0 MeV for ',real(ECA),' MeV/A:',real(SIGG)
        DONE8=.true.
       endif
       if(.not.DONE10.and.DELEX.gt.10.0) THEN
        PRINT *,'Sum to 10.0 MeV for ',real(ECA),' MeV/A:',real(SIGG)
        DONE10=.true.
       endif
       if(.not.DONE12.and.DELEX.gt.12.0) THEN
        PRINT *,'Sum to 12.0 MeV for ',real(ECA),' MeV/A:',real(SIGG)
        DONE12=.true.
       endif
       if(.not.DONE14.and.DELEX.gt.14.0) THEN
        PRINT *,'Sum to 14.0 MeV for ',real(ECA),' MeV/A:',real(SIGG)
        DONE14=.true.
       endif
      END DO
99    CONTINUE
      print*,'--------------------------------------------------------'
      PRINT *,'for Z,A,bmin',int(ZT),int(AT),real(bmin)
*     write(4,*) '# Total: ',int(SIGG)
*    +   ,' at ',int(ECA),' for Z,A,bmin',int(ZT),int(AT),real(bmin)
200   continue
      STOP
      END
! ----------------------------------------------------------------------
      FUNCTION BESSK0(X)
      REAL*8 X,BESSK0,Y,AX,P1,P2,P3,P4,P5,P6,P7,Q1,Q2,Q3,Q4,Q5,Q6,Q7,   
     +      BESSI0
      DATA P1,P2,P3,P4,P5,P6,P7/-0.57721566D0,0.42278420D0,0.23069756D0,
     + 0.3488590D-1,0.262698D-2,0.10750D-3,0.74D-5/
      DATA Q1,Q2,Q3,Q4,Q5,Q6,Q7/1.25331414D0,-0.7832358D-1,0.2189568D-1,
     + -0.1062446D-1,0.587872D-2,-0.251540D-2,0.53208D-3/
      IF(X.EQ.0.D0) THEN
      BESSK0=1.D30
      RETURN
      ENDIF
      IF(X.LE.2.D0) THEN
      Y=X*X/4.D0
      AX=-LOG(X/2.D0)*BESSI0(X)
      BESSK0=AX+(P1+Y*(P2+Y*(P3+Y*(P4+Y*(P5+Y*(P6+Y*P7))))))
      ELSE
      Y=(2.D0/X)
      AX=EXP(-X)/SQRT(X)
      BESSK0=AX*(Q1+Y*(Q2+Y*(Q3+Y*(Q4+Y*(Q5+Y*(Q6+Y*Q7))))))
      ENDIF
      RETURN
      END
! ----------------------------------------------------------------------
      FUNCTION BESSK1(X)
      REAL*8 X,BESSK1,Y,AX,P1,P2,P3,P4,P5,P6,P7,Q1,Q2,Q3,Q4,Q5,Q6,Q7,
     +  BESSI1
      DATA P1,P2,P3,P4,P5,P6,P7/1.D0,0.15443144D0,-0.67278579D0,  
     + -0.18156897D0,-0.1919402D-1,-0.110404D-2,-0.4686D-4/
      DATA Q1,Q2,Q3,Q4,Q5,Q6,Q7/1.25331414D0,0.23498619D0,-0.3655620D-1,
     + 0.1504268D-1,-0.780353D-2,0.325614D-2,-0.68245D-3/
      IF(X.EQ.0.D0) THEN
        BESSK1=1.D32
        RETURN
      ENDIF
      IF(X.LE.2.D0) THEN
      Y=X*X/4.D0
      AX=LOG(X/2.D0)*BESSI1(X)
      BESSK1=AX+(1.D0/X)*(P1+Y*(P2+Y*(P3+Y*(P4+Y*(P5+Y*(P6+Y*P7))))))
      ELSE
      Y=(2.D0/X)
      AX=EXP(-X)/SQRT(X)
      BESSK1=AX*(Q1+Y*(Q2+Y*(Q3+Y*(Q4+Y*(Q5+Y*(Q6+Y*Q7))))))
      ENDIF
      RETURN
      END
! ----------------------------------------------------------------------
      FUNCTION BESSI0(X)
      REAL *8 X,BESSI0,Y,P1,P2,P3,P4,P5,P6,P7,Q1,Q2,Q3,Q4,Q5,Q6,Q7,Q8,
     + Q9,AX,BX
      DATA P1,P2,P3,P4,P5,P6,P7/1.D0,3.5156229D0,3.0899424D0,1.2067429D0
     + ,0.2659732D0,0.360768D-1,0.45813D-2/
      DATA Q1,Q2,Q3,Q4,Q5,Q6,Q7,Q8,Q9/0.39894228D0,0.1328592D-1,  
     +  0.225319D-2,-0.157565D-2,0.916281D-2,-0.2057706D-1,  
     +  0.2635537D-1,-0.1647633D-1,0.392377D-2/
      IF(ABS(X).LT.3.75D0) THEN
      Y=(X/3.75D0)**2
      BESSI0=P1+Y*(P2+Y*(P3+Y*(P4+Y*(P5+Y*(P6+Y*P7)))))
      ELSE
      AX=ABS(X)
      Y=3.75D0/AX
      BX=EXP(AX)/SQRT(AX)
      AX=Q1+Y*(Q2+Y*(Q3+Y*(Q4+Y*(Q5+Y*(Q6+Y*(Q7+Y*(Q8+Y*Q9)))))))
      BESSI0=AX*BX
      ENDIF
      RETURN
      END
! ----------------------------------------------------------------------
      FUNCTION BESSI1(X)
      REAL *8 X,BESSI1,Y,P1,P2,P3,P4,P5,P6,P7,Q1,Q2,Q3,Q4,Q5,Q6,Q7,Q8,
     + Q9,AX,BX
      DATA P1,P2,P3,P4,P5,P6,P7/0.5D0,0.87890594D0,0.51498869D0,  
     +  0.15084934D0,0.2658733D-1,0.301532D-2,0.32411D-3/
      DATA Q1,Q2,Q3,Q4,Q5,Q6,Q7,Q8,Q9/0.39894228D0,-0.3988024D-1,  
     + -0.362018D-2,0.163801D-2,-0.1031555D-1,0.2282967D-1,       
     + -0.2895312D-1,0.1787654D-1,-0.420059D-2/
      IF(ABS(X).LT.3.75D0) THEN
      Y=(X/3.75D0)**2
      BESSI1=X*(P1+Y*(P2+Y*(P3+Y*(P4+Y*(P5+Y*(P6+Y*P7))))))
      ELSE
      AX=ABS(X)
      Y=3.75D0/AX
      BX=EXP(AX)/SQRT(AX)
      AX=Q1+Y*(Q2+Y*(Q3+Y*(Q4+Y*(Q5+Y*(Q6+Y*(Q7+Y*(Q8+Y*Q9)))))))
      BESSI1=AX*BX
      ENDIF
      RETURN
      END

