      PROGRAM HSIMPLE
      PARAMETER (NWPAWC=20000)
      COMMON/PAWC/H(NWPAWC)
*.------------------------------------------------------------
      CALL HLIMIT(NWPAWC)
      call hbook1(1,'test1',10,-5.,5.,0.)
      call hcopy(1,2,'test2')
      call hcopy(1,3,'test3')
      call hbook2(4,'test4',10,-5.,5.,10,-5.,5.,0.)
*
      do 10 i=1,1000000
         call rannor(a,b)
         call hf1(1,a,1.)
*         call hf1(2,b,1.)
*         call hf1(3,a**2+b**2,1.)
         call hf2(4,a,b,1.)
         if(mod(i,100000).eq.0)
     X   print *,' hserver in loop index ',i
  10  continue
*
      call hfill(2,-5.,0.,1.)
      call hfill(2,-4.,0.,2.)
      call hfill(2,-3.,0.,3.)
      call hfill(2,-2.,0.,4.)
      call hfill(2,-1.,0.,8.)
      call hfill(2, 0.,0.,16.)

      call hfill(3,-5.,0.,1.)
      call hfill(3,-4.,0.,2.)
      call hfill(3,-3.,0.,3.)
      call hfill(3,-2.,0.,4.)
      call hfill(3,-1.,0.,8.)
      call hfill(3, 0.,0.,16.)

      CALL HRPUT(0,'HSIMPLE.HBOOK','N')
      CALL HPRINT(0)
      END
