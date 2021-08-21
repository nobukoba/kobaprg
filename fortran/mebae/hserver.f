       Program hserver
*
*     HBOOK program creating a "shared memory" 
*     area called 'TEST'
*     Routine HLIMAP replaces HLIMIT.
*     NWORDS is the amount of space requested 
*     in the shared area.
*
      parameter(nwords=32000000)
      
      integer val(205)

      
      call hlimap(nwords,'MEBA')
      call hbook1(1,'test1',100,-4.,4.,0.)
      call hcopy(1,2,'test2')
      call hcopy(1,3,'test3')
      call hbook2(4,'test4',30,-3.,3.,30,-3.,3.,0.)
      call hbook2(5,'test5',30,-3.,3.,30,-3.,3.,0.)
      call hbook2(6,'mebae',205,1,206,51,1,52,0.)
      call hbook2(7,'mebae2',205,1,206,51,1,52,0.)
      
      open(100, FILE='mebae.dat')
      do j = 1, 51
         read(100, '(205(I1))'), val
         write(*,'(205(I1))') (val(i), i=1,205)
         do i = 1, 205
            if (val(i) .eq. 1) then
               call hf2(6,float(i),float(52-j),1.)
            endif
         enddo
      enddo

      do while (.true.)
         r = 1
         do i=1,50000
            call rannor(a,b)
            call hf1(1,erf(a),1.)
            call hf1(2,b,1.)
            call hf1(3,a**2+b**2,1.)
            call hf2(4,a,b,1.)
            call hf2(5,a+1.,b+1.,1.)
            call hrndm2(6,rx,ry)
            call hf2(7,rx,ry,1.)

            if (mod(i,1*int(r)) .eq. 0) then
               print *,' hserver in loop index ',i
               r = 1.3*r
               print *, r
               call sleep(1)
            endif
         enddo
         call hreset(1,' ')
         call hreset(2,' ')
         call hreset(3,' ')
         call hreset(4,' ')
         call hreset(5,' ')
         call hreset(7,' ')
      enddo
*     
      end
