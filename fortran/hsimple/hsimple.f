      PROGRAM HSIMPLE
      PARAMETER (NWPAWC=20000)
      COMMON/PAWC/H(NWPAWC)
*.------------------------------------------------------------
      CALL HLIMIT(NWPAWC)
      call hbook1(1,'test1',100,-3.,3.,0.)
      call hcopy(1,2,'test2')
      call hcopy(1,3,'test3')
      call hbook2(4,'test4',30,-3.,3.,30,-3.,3.,0.)
*
      do 10 i=1,1000000
         call rannor(a,b)
         call hf1(1,a,1.)
         call hf1(2,b,1.)
         call hf1(3,a**2+b**2,1.)
         call hf2(4,a,b,1.)
         if(mod(i,100000).eq.0)
     X   print *,' hserver in loop index ',i
  10  continue
*
      CALL HRPUT(0,'HSIMPLE.HBOOK','N')
      CALL HPRINT(0)
      END
