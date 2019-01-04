*----------------------------------------------------------------------
*     calculation of BE1 distribution for Coulex code
*     version for any j,l value bound state with sum over jf,lf 
*----------------------------------------------------------------------
      implicit real*8 (a-h,o-z)
      real*8 chis(10),ffr(6602,4),u(6602),bs(6602),vv(6602),bs2(6602)
      real*8 w(0:6602),A,B
      common rmu,conno1,pi,vdepth,wal,wr0,wls,ib
      common/so/isof
      common/clebma/faclog(500)
      print*,'dB(E1)/dE distribution in the two-cluster model'
      print*
*     search well depth for ia=0, search energy for ia=1
      ia=0
      ic=1
*     read stuff
*     print*,'potential type'
      print*,'0 = Woods-Saxon'
      print*,'1 = Gauss'
*     print*,'2 = Yukawa'
*     print*,'3 = Hulthen'
*     print*,'4 = Cosh'
      read*,ib
*---------------------------------------------------------------------
      call factor
      p1=4.d0*atan(1.d0)
      conno1=0.2195376d0
      esq=1.44d0
      amu=938.0d0 
      hbarc=197.34d0
      hbarc2=hbarc*hbarc
      pi=4.d0*datan(1.d0)
      wzz=0.0d0
*---------------------------------------------------------------------
*     conno1=0.2196079d0
*---------------------------------------------------------------------
*     lots of bound state parameters
*---------------------------------------------------------------------
      print*,'potential radius and diffuseness, steps and step'
      read *, wr0,wal,nramax,drx
      print*,'spin orbit strength and if fixed (1)'
      read*,wls,isof
      print*,'cmass and vmass'
      read*,cmass,vmass
      print*,'core charge '
      read*,zc
      print*,'bengy'
      read*,bengy
      print*,'maximum relative energy'
      read*,e1enmax
      print*,'2*j lmoma and nodd'
      read*,j2a,lmoma,nodd
* -------------------------------------------------------------------
*     print*,'rmscore'
*     read*,rmscore
*     print*,'dmat '
*     read*,dmat
*--------------------------------------------------------------------
      rmscore=0.d0
      dmat=0.d0
      is2=1
      spin=is2/2.d0
      rlmoma=lmoma
      rja=j2a/2.d0
      pmass=cmass+vmass
      rmu=cmass*vmass/pmass
      rma=rmu*amu              
      jhw=1
      pnloc=0.d0
      wr0ls=wr0
      wals=wal
      wrz=wr0
*------------------------------------------------------------------------
      bsalfa=sqrt(2.d0*rma*abs(bengy))/hbarc
      dee0=hbarc2/(2.d0*rma)*sqrt(8.d0*pi*bsalfa)
      print*,' Yukawa |D0| value for this binding = ',dee0
*------------------------------------------------------------------------
      call bound(ia,ib,ic,is2,j2a,lmoma,nodd,nramax,
     1 jhw,cmass,vmass,wzz,wal,wr0,wls,ffr,vdepth,bengy,dmat,kcheck,
     2 wrz,chis,drx,pnloc,wr0ls,wals)
*------------------------------------------------------------------------
      write(*,21) vdepth,bengy,wal,wr0,wzz,wls,ia,ib,ic,
     1 lmoma,nodd,jhw
   21 format('  vdepth= ',f10.5,'  bengy= ',f10.5,'  wa= ',f10.5,
     + //,'  wr0= ',f10.5,'  wzz= ',f10.5,'  wls= ',f10.5,//,
     +   '  ia= ',i5,'  ib= ',i5,'  ic= ',i5,'  lmom= ',i5,
     +   '  nod= ',i5,'  jhw= ',i5)
*------------------------------------------------------------------------
*     wave function at the origin
*------------------------------------------------------------------------
      u(1)=0.d0
      vv(1)=0.d0
*------------------------------------------------------------------------
      do 44 i=1,nramax
      r=(i-1)*drx
      u(i+1)= ffr(i,1)
      vv(i+1)= ffr(i,3)
   44 continue
*------------------------------------------------------------------------
*     check normalizations and calculate rms relative and of composite
*------------------------------------------------------------------------
      do 86 i=1,nramax
      r=(i-1)*drx
      u(i)=+r*u(i)
      write(19,*) r,u(i)
*------------------------------------------------------------------------
*     for zero range check
*------------------------------------------------------------------------
*     u(i)=sqrt(2*bsalfa)*exp(-bsalfa*r)
*------------------------------------------------------------------------
      bs(i)=u(i)**2
   86 continue
      call sim(bs,rnorm,1,nramax,drx)
      print*
      print *,'  wave function normalization =',rnorm 
      print*
*------------------------------------------------------------------------
*     comment out rms radius computation
*------------------------------------------------------------------------
*     do 90 i=1,nramax
*     r=(i-1)*drx
*     bs(i)=bs(i)*r*r
*  90 continue
*     call sim(bs,rnorm,1,nramax,drx)
*     print*
*     print *,'  wave function relative rms =',sqrt(rnorm) 
*     rmsproj=sqrt(cmass/pmass*rmscore**2+cmass*vmass/pmass/pmass
*    +  *rnorm)
*     print *,'  projectile rms =',rmsproj 
*------------------------------------------------------------------------
*     remove comment for plane wave final state
*------------------------------------------------------------------------
*     vdepth=0.d0
*     wls=0.d0
*------------------------------------------------------------------------
      print*,' final state potentials '
      print*,vdepth,wls,real(wr0),real(wal)
      wr0=wr0*cmass**(0.333333333333d0)
      print*
      write(21,*) 0.d0,0.d0
*     write(22,*) 0.d0,0.d0
*     write(23,*) 0.d0,0.d0
*------------------------------------------------------------------------
*     calculate B(E1) distribution
*------------------------------------------------------------------------
*     constant multiplier for dB(E1)/dE
*------------------------------------------------------------------------
      rcj=(zc/pmass)**2*(rmu*amu/hbarc/hbarc)*2.d0/p1
      ieb=0
      llmin=lmoma-1
      llmax=lmoma+1

* tmp nobu -->
*      lfin  = 3;
*      jvals = 5;
*      ebe1 = 0.45;
*      call wf(w,drx,nramax,ebe1,jvals,lfin,A,B) 
*      do i=1,nramax
*         r=(i-1)*drx
**         write(*,*) r,w(i-1)
*      enddo
*      write(*,*) 'A,B: ',A,B
*      write(*,*) 'drx', drx
*      write(*,*) 'nramax', nramax
*      write(*,*) 'ebe1', ebe1
*      write(*,*) 'jvals', jvals
*      write(*,*) 'lfin', lfin
*      write(*,*) 'rmu', rmu
*      write(*,*) 'conno1', conno1
*      write(*,*) 'pi', pi
*      write(*,*) 'vdepth', vdepth
*      write(*,*) 'wal', wal
*      write(*,*) 'wr0', wr0
*      write(*,*) 'wls', wls
*      write(*,*) 'ib', ib
*      return
* --> temp nobu end

      if(llmin.lt.0) llmin=llmax
      do ebe1=0.01d0,e1enmax,0.01d0
*      do ebe1=0.45d0,e1enmax,100.01d0
       qbe1=sqrt((0.2195376d0**2*rmu)*ebe1)
       be1dist=0.d0
       ieb=ieb+1
       do lfin=llmin,llmax,2
        do jstep=-1,1,2
        rlfin=lfin
        jvals=2*lfin+jstep
        rjval=jvals/2.d0
        if(rjval.lt.0) goto 888
*------------------------------------------------------------------------
*       these lines for j-version only
*------------------------------------------------------------------------
        cgc=cleb(rjval,spin,rja,-spin,1.d0,0.d0)*stat2(rjval)
        if(abs(cgc).lt.1.d-10) goto 888
        cgc=cgc*cgc/4.d0/p1
        if(ieb.eq.1) print*,' ang mom ',real(rlfin),real(rjval),cgc
*------------------------------------------------------------------------
*       these lines for ell only
*------------------------------------------------------------------------
*       cgc=cleb(rlmoma,0.d0,1.d0,0.d0,rlfin,0.d0)
*       if(abs(cgc).lt.1.d-10) goto 888
*       cgc=cgc*cgc*3.d0/(4.d0*p1)
*       if(ieb.eq.1) print*,' ang mom ',real(lfin),cgc
*------------------------------------------------------------------------
*       analytic result in zero range s-state case
*------------------------------------------------------------------------
*       rcc=3.d0*hbarc**2/(p1*p1*rmu*amu)*(zc/pmass)**2
*       rcc=sqrt(bengy)*rcc*(ebe1)**1.5d0/(ebe1+bengy)**4
*------------------------------------------------------------------------
*       solve radial differential equation for this partial wave
*------------------------------------------------------------------------
        call wf(w,drx,nramax,ebe1,jvals,lfin,A,B) 
*       print*,A,B
*       delta=atan2(B,A)
*       print*,lfin,jvals,'  delta (deg) = ',delta/pi*180.d0
        do 87 i=1,nramax
        r=(i-1)*drx
*     ---------------------------------------------------------------------
*       BE1 transform integrand
*     ---------------------------------------------------------------------
        bs2(i)=r*u(i)*w(i-1)/qbe1
*       write(33,*) r,w(i-1)/qbe1,r*bes1(qbe1*r)
*     ---------------------------------------------------------------------
*       zero-range limit (numerically)
*     ---------------------------------------------------------------------
*       bs (i)=r*r*bes1(qbe1*r)*sqrt(2*bsalfa)*exp(-bsalfa*r)
   87   continue
        call sim(bs2,rnorm2,1,nramax,drx)
*       call sim(bs,rnorm,1,nramax,drx)
*       print*,rnorm2
        be1dist=be1dist+qbe1*(rnorm2**2)*rcj*cgc
  888   continue
        enddo
       enddo
       write(21,*) ebe1,be1dist
      enddo
      end
*------------------------------------------------------------------------
      function bes0(x)
*------------------------------------------------------------------------
      implicit real*8(a-h,o-z)
      bes0=1.d0
      if(x.gt.0.d0) bes0=sin(x)/x 
      return
      end
*------------------------------------------------------------------------
      function bes1(x)
*------------------------------------------------------------------------
      implicit real*8(a-h,o-z)
      bes1=0.d0
      if(x.gt.0.d0) bes1=sin(x)/x/x-cos(x)/x
      return
      end
*------------------------------------------------------------------------
      real*8 function bes2(x)
      implicit real*8(a-h,o-z)
      bes2=0.d0
      if(x.gt.0.d0) bes2=3.d0*bes1(x)/x-bes0(x)
      return
      end
*------------------------------------------------------------------------
      real*8 function bes3(x)
      implicit real*8(a-h,o-z)
      bes3=0.d0
      if(x.gt.0.d0) bes3=5.d0*bes2(x)/x-bes1(x)
      return
      end
*------------------------------------------------------------------------
      real*8 function bes4(x)
      implicit real*8(a-h,o-z)
      bes4=0.d0
      if(x.gt.0.d0) bes4=7.d0*bes3(x)/x-bes2(x)
      return
      end
***********************************************************************
      subroutine bound(ia,ib,ic,is2,j2,lmom,nod,nramax,jhw,cmass,vmass,
     1wzz,wa,wr0,wls,ffr,vdepth,bengy,dmat,kcheck,wrz,chis,drx,pnloc,
     1wr0ls,wals)
      implicit real*8(a-h,o-z)
      dimension ffr(6602,4),chis(10),drd(4)
      dimension wfcr(6602),wfsr(6602),wfc(6602)
      real*8 cmass,vmass ,wzz,wa,wr0,wls,ffr,vdepth,bengy,dmat,wrz,
     1 chis,drx
      common/so/isof
      korec=0
      do 6000 i=1,6602
      ffr(i,jhw)=0.0
 6000 continue
      niter=0
      incr=0
      eps7=1.d-6
      drd(jhw)=drx
      lmom1=lmom+1
      test=1.0e+20
      lmom2=lmom1+1
      drz=wrz
      fnod=nod
      flmom=lmom
      flmom1=lmom1
      radi=wr0*(cmass)**0.333333333d0
      radls=wr0ls*(cmass)**0.333333333d0
      yyyy = cosh(radi/wa)
      radz=radi
      wr=drd(jhw)
      nr1=nramax+1
      do 8 i=1,nramax+10
      yyy=(wr-radi)/wa
      zzz=(wr-radls)/wals
      if(yyy.gt.90.d0) yyy=90.d0
      if(zzz.gt.90.d0) zzz=90.d0
      ex=exp(yyy)
      exls=exp(zzz)
      ib1=ib+1
      go to (6100,6200,6300,6350,6355),ib1
 6100 wfcr(i)=1.0/(1.0+ex)
      go to 6400
 6200 wfcr(i)=exp(-(wr/wa)*(wr/wa))
      go to 6400
 6300 wfcr(i)=exp(-wa*wr)/wr
      go to 6400
 6350 wfcr(i)=exp(-wa*wr)/(exp(-wr0*wr)-exp(-wa*wr))
      go to 6400
 6355 wfcr(i) = (1.0+yyyy)/(cosh(wr/wa)+yyyy)
 6400 wfsr(i)=exls/(1.0+exls)/(1.0+exls)
      if(wr-radi) 3,3,4
    3 wfc(i)=0.7199262*wzz*(3.0-wr*wr/(radi*radi))/radi
      go to 5
    4 wfc(i)=1.4398523*wzz/wr
    5 wr=wr+drd(jhw)
    8 continue
      if(ia-1) 10,20,20
   10 vdepth=bengy+(3.1415926*(fnod+0.5*flmom1))**2/(0.048228*vmass*
     1 (radi+drz)**2)
      write(*,*) 'vdepth: ', vdepth 
      go to 30
   20 bengy=vdepth-(3.1415926*(fnod+0.5*flmom1))**2/(0.048228*vmass
     1*radz*radz)
      if(bengy-eps7) 25,25,30
   25 radz=radz+drz
      incr=incr+1
      if(incr-20) 20,20,27
   27 kcheck=11
      go to 7400
   30 is2mn=abs(j2-2*lmom)
      is2mx=j2+2*lmom
      if(is2.lt.is2mn.or.is2.gt.is2mx) go to 40
      fjs=0.5*real(j2)
      fis=0.5*real(is2)
      flns=fjs*(fjs+1.)-flmom*(flmom+1.)-fis*(fis+1.)
      go to 70
   40 lsq=j2-2*lmom
      if(lsq) 42,44,46
   42 flns=-lmom1
      go to 70
   44 flns=0.
      go to 70
   46 flns=lmom
   70 match=radi/drd(jhw)+dmat
   80 fmu=vmass*cmass/(vmass+cmass)
      wk=0.2195376d0*sqrt(fmu*bengy)
      wrhon=wk*radi
      wrhoc=wrhon
      wrhoz=wk*radz
      wrhocs=wrhoc*wrhoc
      weta=0.7199262*wzz*wk/bengy
      wetac=weta/wrhoc
      wdrho=drd(jhw)*wk
      if(isof.ne.1) wls=0.1767*vdepth
      wvs=2.*wls*wk/(bengy*wals)
      drhosq=wdrho*wdrho
      dr56=0.8333333333*drhosq
      dr12=0.1*dr56
      fl1=lmom*lmom1
  100 wrho=wdrho
*      write(*,*) 'aft label100', wdrho
      wvc=vdepth/bengy
      zer=1.0
      do 180 j=1,lmom2
      a1=-wvs*flns*wfsr(j)/(flmom1+flmom1)
      b1=1.0-wvc*wfcr(j)+3.0*wetac
      b2=wvs*flns*wfsr(j)
      a2=(b1-b2*a1)/(4.0*flmom1+2.0)
      a3=(b1*a1-b2*a2)/(6.0*flmom1+6.0)
      wrhosq=wrho*wrho
      b3=weta/(wrhoc*wrhocs)
      a4=(b1*a2-b2*a3-b3)/(8.0*flmom1+12.0)
      a5=(b1*a3-b2*a4-b3*a1)/(10.0*flmom1+20.0)
      a6=(b1*a4-b2*a5-b3*a2)/(12.0*flmom1+30.0)
      ffr(j,jhw)=(wrho**lmom1)*(1.0+a1*wrho+a2*wrho*wrho+a3*wrhosq*wrho
     1+a4*wrhosq*wrhosq+a5*wrho*wrhosq*wrhosq+a6*wrhosq**3)
  180 wrho=wrho+wdrho
      mat1=match+1
      x1=wdrho*flmom1
      x2=x1+wdrho
      x3=x2+wdrho
      do 200 i=lmom1,mat1
      fac1=1.0-dr12*(fl1/(x1*x1)+1.0-wvc*wfcr(i)-wvs*flns*wfsr(i)/x1
     1+wfc(i)/bengy)
      fac2=2.0+dr56*(fl1/(x2*x2)+1.0-wvc*wfcr(i+1)-wvs*flns*wfsr(i+1)/x2
     1+wfc(i+1)/bengy)
      fac3=1.0-dr12*(fl1/(x3*x3)+1.0-wvc*wfcr(i+2)-wvs*flns*wfsr(i+2)/x3
     1+wfc(i+2)/bengy)
      ffr(i+2,jhw)=(ffr(i+1,jhw)*fac2-ffr(i,jhw)*fac1)/fac3
      if(dabs(ffr(i+2,jhw))-test) 195,185,185
  185 ip2=i+2
      do 190 ip=1,ip2
      ffr(ip,jhw)=ffr(ip,jhw)/test
  190 continue
      zer=zer/test
  195 x1=x2
      x2=x3
      x3=x3+wdrho
  200 continue
      nodes=0
      do 202 i=lmom1,match
      if(ffr(i,jhw)*ffr(i+1,jhw)) 210,215,202
  210 nodes=nodes+2
      go to 202
  215 nodes=nodes+1
  202 continue
      nn=nodes/2
      fnn=nn
      if(nod-nn) 225,240,225
  225 korec=korec+1
      if(korec-10) 228,228,226
  226 kcheck=10
      go to 7400
  228 vcor=(wrhoz*wrhoz+9.86959*(fnod+0.5*flmom1)**2)/(wrhoz*wrhoz
     1+9.86959*(fnn+0.5*flmom1)**2)
      vcor=sqrt(vcor)
      if(ia-1) 230,235,235
  230 vdepth=vcor*vdepth
      if(isof.ne.1) wls=0.1767*vdepth
      wvs=2.*wls*wk/(bengy*wals)
      go to 100
  235 bengy=bengy/vcor
      go to 80
  240 dffr1=((ffr(match+3,jhw)-ffr(match-3,jhw))/60.0+3.0*(ffr(match-2,
     1jhw)-ffr(match+2,jhw))/20.0+3.0*(ffr(match+1,jhw)-ffr(match-1,jhw)
     2)/4.0)/wdrho
*      write(*,*) 'aft dffr1', dffr1
      korec=0
      rhoa=wk*drd(jhw)*real(nramax)
      wrho=rhoa
      jrho=nramax
  325 ex=wrho+weta*log(wrho+wrho)
      ffr(jrho,jhw+2)=exp(-ex)
      if(jrho-nramax) 340,340,350
  340 jrho=jrho+1
      wrho=wrho+wdrho
      go to 325
  350 x1=rhoa-wdrho
      x2=rhoa
      x3=x2+wdrho
      imax=nramax-match+3
      do 360 i=1,imax
      k=nramax-i
      fac1=1.0-dr12*(fl1/(x1*x1)+1.0-wvc*wfcr(k)-wvs*flns*wfsr(k)/x1
     1+wfc(k)/bengy)
      fac2=2.0+dr56*(fl1/(x2*x2)+1.0-wvc*wfcr(k+1)-wvs*flns*wfsr(k+1)/x2
     1+wfc(k+1)/bengy)
      fac3=1.0-dr12*(fl1/(x3*x3)+1.0-wvc*wfcr(k+2)-wvs*flns*wfsr(k+2)/x3
     1+wfc(k+2)/bengy)
      ffr(k,jhw+2)=(ffr(k+1,jhw+2)*fac2-ffr(k+2,jhw+2)*fac3)/fac1
      if(dabs(ffr(k,jhw+2))-test) 358,352,352
  352 nrten=nramax-k+2
      do 356 iten=1,nrten
      kten=iten+k-1
      ffr(kten,jhw+2)=ffr(kten,jhw+2)/test
  356 continue
  358 x3=x2
      x2=x1
      x1=x1-wdrho
  360 continue
      dffr2=((ffr(match+3,jhw+2)-ffr(match-3,jhw+2))/60.0+3.0*(ffr(match
     1-2,jhw+2)-ffr(match+2,jhw+2))/20.0+3.0*(ffr(match+1,jhw+2)
     2-ffr(match-1,jhw+2))/4.0)/wdrho
*      write(*,*) 'aft dffr2', dffr2
      ratio=ffr(match,jhw)/ffr(match,jhw+2)
      tlogd1=dffr1/ffr(match,jhw)
      tlogd2=dffr2/ffr(match,jhw+2)
      difnce=dabs(tlogd1-tlogd2)
*      write(*,*) 'difnce', difnce
      if(difnce-eps7) 510,510,400
  400 niter=niter+1
      if(niter-100) 410,410,405
  405 kcheck=12
      go to 7400
  410 fnum=ffr(match,jhw+2)*dffr2*ratio*ratio-ffr(match,jhw)*dffr1
      sum=0.0
      do 480 i=1,nramax,2
      if(i-1) 420,420,430
  420 sum1=0.0
      go to 445
  430 if(i-match) 440,440,450
  440 sum1=ffr(i-1,jhw)*ffr(i-1,jhw)
  445 sum2=ffr(i,jhw)*ffr(i,jhw)
      sum3=ffr(i+1,jhw)*ffr(i+1,jhw)
      go to 455
  450 sum1=ffr(i-1,jhw+2)*ffr(i-1,jhw+2)*ratio*ratio
      sum2=ffr(i,jhw+2)*ffr(i,jhw+2)*ratio*ratio
      sum3=ffr(i+1,jhw+2)*ffr(i+1,jhw+2)*ratio*ratio
  455 if(ia-1) 460,470,470
  460 if(i-1) 462,462,465
  462 sum1=0.0
      go to 467
  465 sum1=-sum1*wfcr(i-1)*wvc
  467 sum2=-sum2*wfcr(i)*wvc
      sum3=-sum3*wfcr(i+1)*wvc
  470 sum=sum+sum1+4.0*sum2+sum3
  480 continue
      denom=sum*wdrho/3.0
      incr=0
      ram1=fnum/denom
      write (*,*) 'bfr ramda vdepth: ', vdepth
      write (*,*) 'ramda: ', ramda
      write (*,*) 'denom: ', denom
      write (*,*) 'sum: ', sum
      write (*,*) 'wdrho: ', wdrho
  482 ramda=1.0+ram1
      if(ramda-eps7) 485,485,488
  485 ram1=0.5*ram1
      incr=incr+1
      if(incr-10) 482,482,486
  486 kcheck=13
      go to 7400
  488 if(ia-1) 489,500,500
  489 if(ramda-2.0) 490,495,495
  490 vdepth=ramda*vdepth
      write (*,*) 'aft ramda vdepth: ', vdepth
      if(isof.ne.1) wls=0.1767*vdepth
      wvs=2.*wls*wk/(bengy*wals)
      go to 100
  495 drz=drz-0.08*radi
      go to 10
  500 bengy=bengy*ramda
      go to 80
  510 nr1=nramax+1
      do 520 i=match,nr1
      ffr(i,jhw)=ratio*ffr(i,jhw+2)
  520 continue
      sum=0.0
      do 570 i=1,nramax,2
      if(i-1) 540,540,550
  540 sum1=0.0
      go to 560
  550 sum1=ffr(i-1,jhw)*ffr(i-1,jhw)
  560 sum2=ffr(i,jhw)*ffr(i,jhw)
      sum3=ffr(i+1,jhw)*ffr(i+1,jhw)
      sum=sum+sum1+4.0*sum2+sum3
  570 continue
      sum=sum*drd(jhw)/3.0
      znorm=1.0/sqrt(sum)
      wlss=2.*wls*flns/wals
      r=drd(jhw)
      dr=r
c     *****( non-local correction )*****
      ipnl=0
      if(pnloc.lt.0.0) ipnl=dabs(pnloc)/dr+1
      fact=0.048196758*fmu*pnloc**2/8.0
      sum=0.0
      do 599 i=1,nr1
      ffr(i,jhw)=znorm*ffr(i,jhw)
      if(fact.lt.1.e-10) go to 599
      ffr(i,3)=vdepth*wfcr(i)+wlss*wfsr(i)/r-wfc(i)
      ffr(i,jhw)=ffr(i,jhw)*exp(-fact*ffr(i,3))
      if(i.lt.ipnl) ffr(i,jhw)=0.0
      sum=sum+(ffr(i,jhw))**2
  599 r=r+dr
      chis(jhw)=znorm*zer*wk**lmom1
      znorm=1.0
      if(fact.lt.1.e-10) go to 8611
      chis(jhw)=chis(jhw)*exp(-fact*ffr(1,3))
      if(ipnl.gt.0) chis(jhw)=0.0
      znorm=1.0/sqrt(sum*dr)
 8611 r=dr
      do 600 i=1,nr1
      go to (575,580,585),ic
  575 ffr(i,jhw)=znorm*ffr(i,jhw)/r
      ffr(i,3)=vdepth*wfcr(i)+wlss*wfsr(i)/r-wfc(i)      
      go to 600
  580 ffr(i,3)=vdepth*wfcr(i)+wlss*wfsr(i)/r
      go to 590
  585 ffr(i,3)=vdepth*wfcr(i)+wlss*wfsr(i)/r-wfc(i)
  590 ffr(i,jhw)=znorm*ffr(i,jhw)*ffr(i,3)
  600 r=r+dr
      chis(jhw)=znorm*chis(jhw)
      if(ic.ne.1) chis(jhw)=chis(jhw)*ffr(1,3)
      go to 8000
 7400 write (*,7500) kcheck,ramda
      bengy=10.d0
 7500 format(19h0 subroutine bound ,8h kcheck=,i3,7h ramda=,f10.6)
 8000 return
      end
*     ---------------------------------------------------------         
      subroutine wf(u,h,nsteps,erel,j,l,A,B)
      implicit real*8 (a-h,o-z)
      real*8 u(0:6602)
      real*8 Astart,Anew,Aold,Dnew,Dold,det,V
      real*8 A,B,C,Kold,Knew
*     complex*16 T,S
      real*8 fa1(0:25),ga1(0:25),sa(0:25),fa2(0:25),ga2(0:25)
      common fmuh,con,pi
*     ---------------------------------------------------------            
*     erel=energy in the centre of mass frame
*     fmuh=reduced mass of projectile-target system
*     l=orbital angular momentum 
*     h=radial integration step size
*     nsteps=number of radial integration steps
*     ---------------------------------------------------------        
*     con is sqrt(2*[1amu]/hbar**2) in MeV**(0.5) fm**(-1)
*     ---------------------------------------------------------            
*     const is 2*fmuh/hbar**2, rk is centre of mass wavenumber
      const=con**2*fmuh
      rk=sqrt(const*erel)
*     print*,' k = ',rk
*     ---------------------------------------------------------
      i1=nsteps-2
      r1=i1*h
      call FandG(0.d0,rk*r1,20,fa1,ga1,sa)
      i2=i1-10
      r2=i2*h
      call FandG(0.d0,rk*r2,20,fa2,ga2,sa)        
*     ---------------------------------------------------------            
*     Subtracted Cowell/Numerov method used for integration
*     ---------------------------------------------------------                  
*     starting values: at r=0 and r=h
*     u(i) stores radial wavefunction at i*h
*     ---------------------------------------------------------              
      u(0)=0.d0
      u(1)=h**(l+1)
      Kold=l*(l+1)/h**2+const*(V(j,l,h)-erel)                
      Aold=(1.d0-h*h*Kold/12.d0)*u(1)
*     Astart is analogue of Aold but at r=0 (care needed)!!
      Astart=0.d0
      if(l.eq.1) Astart=-2.d0/12.d0*h*h
      Dold=Aold-Astart
      do 999 i=2,nsteps
         r=i*h
*        print*,V(j,l,r)
         Dnew=Dold+h*h*Kold*u(i-1)
         Anew=Aold+Dnew
         Knew=l*(l+1)/r**2+const*(V(j,l,r)-erel)                
         u(i)=Anew/(1.d0-h*h*Knew/12.d0)
         Kold=Knew
         Dold=Dnew
         Aold=Anew
  999 continue
*     ---------------------------------------------------------            
*     now need to match numerical wavefunction to physical
*     form assumed for r > potential range
*     ------ RADIUS 1  ----------------------------------------
      F1=fa1(l)
      G1=ga1(l)
*     ------ RADIUS 2  ----------------------------------------
      F2=fa2(l)
      G2=ga2(l)
*     ---------------------------------------------------------       
*     solve linear equations for A and B
*     so A=C*cos(delta) and B=C*sin(delta), C=sqrt(A**2+B**2)
*     ---------------------------------------------------------             
      det=F1*G2-F2*G1
*      write(*,*) 'F1', F1
*      write(*,*) 'F2', F2
*      write(*,*) 'G1', G1
*      write(*,*) 'G2', G2
      A=(u(i1)*G2-u(i2)*G1)/det
      B=(u(i2)*F1-u(i1)*F2)/det
      C=sqrt(A**2+B**2)
      A=A/C
      B=B/C
*     ---------------------------------------------------------       
*     now: A = cos(delta) and B = sin(delta)
*     having matched, re-normalise the numerical wavefunction
*     ---------------------------------------------------------             
      do 222 i=0,nsteps
      u(i)=u(i)/C
  222 continue
      if(A.lt.0.d0) then
      B=-B
      A=-A
      do i=0,nsteps
      u(i)=-u(i)
      enddo
      endif
*     ---------------------------------------------------------       
*     function norm is now sin(kr...+delta) as r-> infinity               
*     calculate and print various derived quantities
*     ---------------------------------------------------------             
*     print*,' cos(delta) = ',A
*     print*,' sin(delta) = ',B
*     compute partial wave T(l) = exp(i*delta)*sin(delta)
*     T=(A+(0.d0,1.d0)*B)*B
*     print*,' T(l) = ',T
*     compute partial wave S(l) = exp(2*i*delta)
*     S=(A+(0.d0,1.d0)*B)**2
*     Smod=abs(S)
*     print*,' S(l) = ',S,' with modulus ',Smod
*     ---------------------------------------------------------      
      return
      end
*     ---------------------------------------------------------      
      subroutine FandG(eta,krval,lmax,fa,ga,sa)
      implicit real*8(a-h,o-z)
      real*8 fa(0:25),ga(0:25),sa(0:25),gd(100),krval
      data cg0, cg1, cg2, cg3, cg4, cg5/
     +1.223406016d+0,4.959570165d-2,8.888888889d-3,
     +2.455199181d-3,9.108958061d-4,2.534684115d-4/
      data cgp,cp1,cp2,cp3,cp4,cp5 /
     +-7.078817734d-1,1.728260369d-1,3.174603174d-4,
     + 3.581214850d-3,3.117824680d-4,9.073966427d-4/
      data gd(1),gd(2),gd(3),gd(4),gd(5) / 
     +12.d0,-360.d0,1260.d0,-1680.d0,1188.d0/
      drho=1.d0
      nr=1
      eps=1.0d-12
      eta2=eta+eta
      etas=eta*eta
      lp=max0(lmax,12)+1
      t=lp
      u=t*t+etas
      v=t/u
      w=eta/u
      x=v*v-w*w
      y=2.*v*w
      u=sqrt (u)
      sig0=eta*(dlog(u)-1.)+(t-0.5)*atan(eta/t)
      do 20 i=1,5
      sig0=sig0-w/gd(i)
      t=v*x-w*y
      w=v*y+w*x
      v=t
   20 continue
   30 if (lp .le. lmax+1) sa(lp-1)=sig0
      lp=lp-1
      if (lp .le. 0) go to 100
      t=lp
      sig0=sig0- datan (eta/t)
      go to 30
  100 emax=(1.0e-5/eps)**0.16666667
      if (eta .lt. emax) go to 200
      r=eta2
      t=6.0
      t=eta**(1.0/t)
      w=eta*t
      u=t-t*(cg2+cg4/etas)/etas
      v=(cg1+(cg3+cg5/etas)/etas)/w
      g=cg0*(u+v)
      t=1./t
      w=eta*t
      u=t+t*(cp2+cp4/etas)/etas
      v=(cp1+(cp3+cp5/etas)/etas)/w
      gp=cgp*(u-v)
      go to 300
  200 r=max0 (nr,1)-1
      r=krval+r*drho
      t=12.+1.4*eta
      if (t .gt. r) r=t
      fk=1.
      f=1.
      gk=0.
      g=0.
      fsk=0.
      fp=0.
      gsk=1.-eta/r
      gp=gsk
      epss=eps*eps
      n=r+r
      do 210 kp=1,n
      t=kp+kp
      u=t*r
      ak=(t-1.)*eta/u
      v=kp*(kp-1)
      bk=(etas-v)/u
      t=ak*fk-bk*gk
      gk=ak*gk+bk*fk
      fk=t
      t=ak*fsk-bk*gsk-fk/r
      gsk=ak*gsk+bk*fsk-gk/r
      fsk=t
      f=f+fk
      g=g+gk
      fp=fp+fsk
      gp=gp+gsk
      test=fk*fk+gk*gk+fsk*fsk + gsk*gsk
      if (test .lt. epss) go to 220
  210 continue
  220 t=r-eta*dlog(r+r)+sig0
      u=cos (t)
      v=sin (t)
      g=f*u-g*v
      gp=fp*u-gp*v
  300 rs=r
      rho=krval
      f=g
      fp=gp
      is=0
      ir=1
      t=r-rho
*      write(*,*) 'rs', rs
*      write(*,*) 'rho', rho
*      write(*,*) 'f', f
*      write(*,*) 'fp', fp
*      write(*,*) 't', t
      if (t) 600,700,310
  310 if (nr .le. 1) go to 320
      is=t/drho
      is=min0 (is+1,nr)
  320 t=is
      rho=krval+t*drho
      gg=rho
      is=is+1
      ir=is
  330 rho=rho-drho
      ir=ir-1
*      write(*,*) 'in line 310 ir', ir
      if (ir .gt. 0) go to 600
      ir=max0 (is,1)
      r=rs
      rho=gg
      f=g
      fp=gp
      go to 350
  340 rho=rho+drho
      ir=ir+1
  350 if (ir .gt. nr) return
  600 h=0.5
*      write(*,*) 'aft line 600 r', r
*      write(*,*) 'aft line 600 rho', rho
*      write(*,*) 'aft line 600 f', f
*      write(*,*) 'aft line 600 fp', fp
*      write(*,*) 'aft line 600 t', t
      w=r-eta2
      if (r-1.0) 601,602,602
  601 h=0.5*r
  602 if (w) 603,605,605
  603 t=sqrt (-r/(w+w))
      if (t-h) 604,605,605
  604 h=t
  605 last=0
      t=rho-r
      if (t) 606,700,607
  606 h=-h
  607 u=t-h
      if (u*h) 608,608,609
  608 h=t
      last=1
  609 u=0.0
      t=1.0
      b1=0.0
      b2=f
      b3=h*fp
      f=f+b3
      v=0.0
  610 it=0
  620 v=-h*(h*b1+w*b2+u*v)/(r*t)
      fp=fp+v
      u=t
      t=t+1.0
      b1=b2
      b2=b3
      b3=h*v/t
      f=f+b3
      test=b3
      testp=v
      if (w) 630,640,640
  630 test=b3/f
      testp=v/fp
  640 if (abs(test)+abs(testp)-eps) 650,610,610
  650 if (it) 660,660,670
  660 it=1
      go to 620
  670 r=r+h
      if (last) 600,600,700
  700 k=lmax+1
      x=f
      y=fp
      do 710 j=1,k
      ga(j-1)=x
      al=j
      t=j*j
      u=t/rho+eta
      v=sqrt (t+etas)
      w=(u*x-al*y)/v
      y=(v*x-u*w)/al
      x=w
  710 continue
      lp=rho
      lp=max0 (lp+10, lmax+20)
      b3=0.d0
      b2=1.0d-20
      w=1.0d0/rho
      al=lp+1
      v=eta/al
      u=0.
      do 840 j=1,lp
      k=lp+1-j
      al=k
      t=k+k+1
      b1=t*(v/al+w)*b2-u*b3
      v=eta/al
      u=sqrt (1.0+v*v)
      b1=b1/u
      b3=b2
      b2=b1
      if (k-lmax-1) 810,810,820
  810 fa(k-1)=b1
      go to 840
  820 test=b1
      if (abs(test)-1.) 840,840,830
  830 b2=b2*1.0d-20
      b3=b3*1.0d-20
  840 continue
      t=(w+eta)*b2-u*b3
      u=1./(t*f-b1*fp)
      k=lmax+1
*      write(*,*) 'aft line 700 t:', t
*      write(*,*) 'aft line 700 u:', u
*      write(*,*) 'aft line 700 k:', k
      do 850 j=1,k
      fa(j-1)=u*fa(j-1)
  850 continue
      if (ir-is) 330,340,340
      end
*     ---------------------------------------------------------        
      real*8 function V(j,l,r)
      implicit real*8 (a-h,o-z)   
      common rmu,conno1,pi,vdepth,wal,wr0,wls,ib
      s=0.5d0
      rj=j/2.d0
      if(ib.eq.1) then
       V = -vdepth*exp(-r*r/wal/wal)
      else if(ib.eq.0) then
       expfac=exp((r-wr0)/wal)
       V = -vdepth/(1.d0+expfac)
       opls=(rj*(rj+1)-l*(l+1)-s*(s+1))*wls
       Vso=-2*opls*expfac/(1+expfac)**2/r/wal
       V=V+Vso
      endif
      return
      end
*     ---------------------------------------------------------        
      subroutine sim(fa,res,m,n,h)
      implicit real*8(a-h,o-z)
      dimension fa(6602),dq(6602)
      do 90 i=m,n
      dq(i)=fa(i)
   90 continue
      rq1=dq(m+1)
      rq2=dq(m+2)
      i=m+3
   98 continue
      if(i.ge.n) go to 99
      rq1=rq1+dq(i)
      rq2=rq2+dq(i+1)
      i=i+2
      go to 98
   99 continue
      res=0.33333333333d0*h*(dq(m)+4.d0*rq1+2.d0*rq2-dq(n))
      return
      end
c     ****************************************************************
      real*8 function stat2(x)
      implicit real*8(a-h,o-z)
      stat2=sqrt(2.d0*x+1.d0)
      return
      end
c     ****************************************************************
      real*8 function cleb(ria,rid,rib,rie,ric,rif)
      implicit real*8(a-h,o-z)
      common/clebma/faclog(500)
      ia=2.d0*(ria+.0001d0)
      ib=2.d0*(rib+.0001d0)
      ic=2.d0*(ric+.0001d0)
      id=int(sign(1.d0,rid)*2.d0*(abs(rid)+.0001d0))
      ie=int(sign(1.d0,rie)*2.d0*(abs(rie)+.0001d0))
      if=int(sign(1.d0,rif)*2.d0*(abs(rif)+.0001d0))
      wwww=-1.0d0
      cleb=0.0d0
      if(id+ie-if) 7000,105,7000
  105 k1=ia+ib+ic
      if((-1)**k1) 7000,107,107
  107 if(.not.((id.eq.0).and.(ie.eq.0))) go to 110
      k1=k1/2
      if((-1)**k1) 7000,110,110
  110 k1=ia+ib-ic
      k2=ic-iabs(ia-ib)
      k3=min0(k1,k2)
      if(k3) 7000,130,130
  130 if((-1)**(ib+ie)) 7000,7000,140
  140 if((-1)**(ic+if)) 7000,7000,150
  150 if(ia-iabs (id)) 7000,152,152
  152 if(ib-iabs (ie)) 7000,154,154
  154 if(ic-iabs (if)) 7000,160,160
  160 if(ia) 7000,175,165
  165 if(ib) 7000,175,170
  170 if(ic) 7000,180,250
  175 cleb=1.0d0
      go to 7000
  180 fb=float(ib+1)
      cleb=((wwww)**((ia-id)/2))/sqrt(fb)
      go to 7000
  250 fc2=ic+1
      iabcp=(ia+ib+ic)/2+1
      iabc=iabcp-ic
      icab=iabcp-ib
      ibca=iabcp-ia
      iapd=(ia+id)/2+1
      iamd=iapd-id
      ibpe=(ib+ie)/2+1
      ibme=ibpe-ie
      icpf=(ic+if)/2+1
      icmf=icpf-if
      vvv=0.5d0
      sqfclg=vvv*(log(fc2)-faclog(iabcp+1)
     1      +faclog(iabc)+faclog(icab)+faclog(ibca)
     2      +faclog(iapd)+faclog(iamd)+faclog(ibpe)
     3      +faclog(ibme)+faclog(icpf)+faclog(icmf))
      nzmic2=(ib-ic-id)/2
      nzmic3=(ia-ic+ie)/2
      nzmi= max0(0,nzmic2,nzmic3)+1
      nzmx= min0(iabc,iamd,ibpe)
      if(nzmx.lt.nzmi) go to 7000
      s1=(wwww)**(nzmi-1)
      do 400 nz=nzmi,nzmx
      nzm1=nz-1
      nzt1=iabc-nzm1
      nzt2=iamd-nzm1
      nzt3=ibpe-nzm1
      nzt4=nz-nzmic2
      nzt5=nz-nzmic3
      termlg=sqfclg-faclog(nz)-faclog(nzt1)-faclog(nzt2)
     1           -faclog(nzt3)-faclog(nzt4)-faclog(nzt5)
      ssterm=s1*exp (termlg)
      cleb=cleb+ssterm
  400 s1=-s1
 7000 return
      end
c     ****************************************************************
      subroutine factor
      implicit real*8(a-h,o-z)
      common/clebma/faclog(500)
      faclog(1)=0.0d0
      faclog(2)=0.0d0
      fn=1.0d0
      do 200 i=3,500
      fn=fn+1.0d0
  200 faclog(i)=faclog(i-1)+log(fn)
      return
      end
c     *******************************************************************
      real*8 function rac(ria,rib,ric,rid,rie,rif)
c     -------------------------------------------------------------------
c     subroutine calculates the racah coefficient w(abcd;ef) defined
c     according to the convention of brink and satchler.
c     the arguments are real and are the actual values of the angular
c     momenta, ( i.e. they can take half integer values )
c     -------------------------------------------------------------------
      implicit real*8(a-h,o-z)
      common/clebma/faclog(500)
      dimension lt(6)
      rac=0.0d0
      ia=2.d0*(ria+.0001d0)
      ib=2.d0*(rib+.0001d0)
      ic=2.d0*(ric+.0001d0)
      id=2.d0*(rid+.0001d0)
      ie=2.d0*(rie+.0001d0)
      if=2.d0*(rif+.0001d0)
      k1=ia+ib-ie
      k2=ie-iabs (ia-ib)
      k3=ic+id-ie
      k4=ie-iabs (ic-id)
      k5=ia+ic-if
      k6=if-iabs (ia-ic)
      k7=ib+id-if
      k8=if-iabs(ib-id)
      k9= min0 (k1,k2,k3,k4,k5,k6,k7,k8)
      if(k9) 7000,20,20
   20 k2=k1-2*(k1/2)
      k4=k3-2*(k3/2)
      k6=k5-2*(k5/2)
      k8=k7-2*(k7/2)
      if(max0(k2,k4,k6,k8)) 7000,25,7000
   25 ltmin=min0(ia,ib,ic,id,ie,if)
      if(ltmin) 7000,30,150
   30 lt(1)=ia
      lt(2)=ib
      lt(3)=ic
      lt(4)=id
      lt(5)=ie
      lt(6)=if
      ltmin=lt(1)
      kmin=1
      do 40 n=2,6
      if(lt(n)-ltmin) 35,40,40
   35 ltmin=lt(n)
      kmin=n
   40 continue
      s1=1.0d0
      f1=ie
      f2=if
      go to (55,55,55,55,45,50),kmin
   45 f1=ia
      f2=ic
      s1=(-1.d0)**(k5/2)
      go to 55
   50 f1=ia
      f2=ib
      s1=(-1.d0)**(k1/2)
   55 rac=s1/dsqrt((f1+1.d0)*(f2+1.d0))
      go to 7000
  150 iabep=(ia+ib+ie)/2+1
      icdep=(ic+id+ie)/2+1
      iacfp=(ia+ic+if)/2+1
      ibdfp=(ib+id+if)/2+1
      iabe=iabep-ie
      ieab=iabep-ib
      ibea=iabep-ia
      icde=icdep-ie
      iecd=icdep-id
      idec=icdep-ic
      iacf=iacfp-if
      ifac=iacfp-ic
      icfa=iacfp-ia
      ibdf=ibdfp-if
      ifbd=ibdfp-id
      idfb=ibdfp-ib
      iabcd1=(ia+ib+ic+id+4)/2
      iefmad=(ie+if-ia-id)/2
      iefmbc=(ie+if-ib-ic)/2
      nzmax=min0(iabe,icde,iacf,ibdf)
      nzmi1=-iefmad
      nzmi2=-iefmbc
      nzmin=max0(0,nzmi1,nzmi2)+1
      if(nzmax.lt.nzmin) go to 7000
      sqlog=faclog(iabe)+faclog(ieab)+faclog(ibea)+faclog(icde)+faclog(i
     1ecd)+faclog(idec)+faclog(iacf)+faclog(ifac)+faclog(icfa)+faclog(ib
     2df)+faclog(ifbd)+faclog(idfb)-faclog(iabep+1)-faclog(icdep+1)-facl
     3og(iacfp+1)-faclog(ibdfp+1)
      sqlog=0.5d0*sqlog
      do 200 nz=nzmin,nzmax
      nzm1=nz-1
      k1=iabcd1-nzm1
      k2=iabe-nzm1
      k3=icde-nzm1
      k4=iacf-nzm1
      k5=ibdf-nzm1
      k6=nz
      k7=iefmad+nz
      k8=iefmbc+nz
      sslog=sqlog+faclog(k1)-faclog(k2)-faclog(k3)-faclog(k4)
     1           -faclog(k5)-faclog(k6)-faclog(k7)-faclog(k8)
      ssterm=((-1.d0)**nzm1)*dexp(sslog)
      rac=rac+ssterm
  200 continue
 7000 return
      end
c     ********************************************************************
      real*8 function u9(ra,rb,rc,rd,re,rf,rg,rh,ri)
c     -------------------------------------------------------------------
c     nine-j symbol. definition as in brink and satchler.
c     -------------------------------------------------------------------
      implicit real*8(a-h,o-z)
      u9=0.0d0
      k1=idint(2.d0*dabs(ra-ri)+0.01d0)
      k2=idint(2.d0*dabs(rb-rf)+0.01d0)
      k3=idint(2.d0*dabs(rd-rh)+0.01d0)
      minrda=max0(k1,k2,k3)
      k1=idint(2.d0*(ra+ri)+0.01d0)
      k2=idint(2.d0*(rb+rf)+0.01d0)
      k3=idint(2.d0*(rd+rh)+0.01d0)
      maxrda=min0(k1,k2,k3)
      if(minrda-maxrda) 30,30,20
   30 do 50 n1=minrda,maxrda,2
      r1=float(n1)/2.d0
      ramda2=n1
      y9=(ramda2+1.d0)*rac(ra,ri,rd,rh,r1,rg)*rac(rb,rf,rh,rd,r1,re)
      u9=u9+y9*rac(ra,ri,rb,rf,r1,rc)
   50 continue
   20 return
      end
c     ****************************************************************
      real*8 function threej(ria,rid,rib,rie,ric,rif)
c     -------------------------------------------------------------------
c     three-j symbol. definition as in brink and satchler.
c     -------------------------------------------------------------------
      implicit real*8(a-h,o-z)
      common/clebma/faclog(500)
      threej=cleb(ria,rid,rib,rie,ric,-rif)
      ipas=idint(dabs(ria-rib-rif)+0.01d0)
      threej=dble(float((-1)**ipas))*threej/stat2(ric)
      return
      end
c     *******************************************************************
      real*8 function sixj(ria,rib,rie,rid,ric,rif)
c     -------------------------------------------------------------------
c     subroutine calculates six-j coefficient defined
c     according to the convention of brink and satchler.
c     the arguments are real and are the actual values of the angular
c     momenta, ( i.e. they can take half integer values )
c     -------------------------------------------------------------------
      implicit real*8(a-h,o-z)
      common/clebma/faclog(500)
      i=abs(ria+rib+ric+rid)+0.1d0
      sixj=(-1)**i*rac(ria,rib,ric,rid,rie,rif)
      return
      end
c     ******************************************************************
      real*8 function rmat(j,m,n)
      implicit real*8(a-h,o-z)
c     ------------------------------------------------------------------
c     small d rotation matrices for integer j. brink and satchler
c     formula. rotation of beta about y-axis.
c     sc=sin(beta/2), dc=cos(beta/2).
c     ------------------------------------------------------------------
      common/clebma/faclog(500)
      common/dmats/sc,dc
      integer t
      t=0
      rmat=1.d0
      if(iabs(m).gt.j.or.iabs(n).gt.j) go to 888
      if(j.eq.0) return
      ind1=j+m+1
      ind2=j-n+1
      ind3=n-m+1
      rnum=(faclog(ind1)+faclog(j-m+1)+faclog(ind2)+faclog(j+n+1))
      rnum=rnum/2.d0
      rmat=0.d0
      go to 997
   88 den=faclog(ind1)+faclog(ind2)+faclog(ind3)+faclog(t+1)
      rmat=rmat+(-1.d0)**t*dexp(rnum-den)*dc**(2*(j-t)+m-n)
     1     *sc**(2*t+n-m)
   99 t=t+1
      ind1=ind1-1
      ind2=ind2-1
      ind3=ind3+1
  997 if((ind1.le.0).or.(ind2.le.0)) return
      if(ind3.le.0) go to 99
      go to 88
  888 rmat=0.d0
      return
      end
