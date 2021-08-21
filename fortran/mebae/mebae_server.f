      Program mebae_server
*     
*     HBOOK program creating a "shared memory" 
*     area called 'TEST'
*     Routine HLIMAP replaces HLIMIT.
*     NWORDS is the amount of space requested 
*     in the shared area.
*     
      parameter(nwords=32000000)
      
      call hlimap(nwords,'MEB2')
      call hropen(1,'read','com.hb',' ',1024,ISTAT)
      call hrin(0,9999,0)
      
      call hbook1(101,'test1',100,-4.,4.,0.)
      call hcopy(101,102,'test2')
      call hcopy(101,103,'test3')
      call hbook2(104,'test4',30,-3.,3.,30,-3.,3.,0.)
      call hbook2(105,'test5',30,-3.,3.,30,-3.,3.,0.)

      call hcopy(1,11,'histo1')
      call hcopy(2,12,'histo2')
      call hcopy(3,13,'histo3')
      call hcopy(4,14,'histo4')
      
      call hreset(11,' ')
      call hreset(12,' ')
      call hreset(13,' ')
      call hreset(14,' ')
      
      do while (.true.)
         do i=1,100000
            call rannor(a,b)
            call hf1(101,erf(a),1.)
            call hf1(102,b,1.)
            call hf1(103,a**2+b**2,1.)
            call hf2(104,a,b,1.)
            call hf2(105,a+1.,b+1.,1.)
            
            rx = hrndm1(1)
            call hf1(11,rx,1.)
            call hrndm2(2,rx,ry)
            call hf2(12,rx,ry,1.)
            call hrndm2(3,rx,ry)
            call hf2(13,rx,ry,1.)
            call hrndm2(4,rx,ry)
            call hf2(14,rx,ry,1.)
            
            if (mod(i,1000) .eq. 0) then
               print *,' hserver in loop index ',i
               call sleep(1)
            endif
         enddo
         
         call hreset(101,' ')
         call hreset(102,' ')
         call hreset(103,' ')
         call hreset(104,' ')
         call hreset(105,' ')
         call hreset(11,' ')
         call hreset(12,' ')
         call hreset(13,' ')
         call hreset(14,' ')
      enddo
*     
      end
