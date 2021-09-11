       Program hdir
*
*     HBOOK program creating a "shared memory" 
*     area called 'TEST'
*     Routine HLIMAP replaces HLIMIT.
*     NWORDS is the amount of space requested 
*     in the shared area.
*
      parameter(nwpaw=32000000)
      common/pawc/h(nwpaw)
      call hlimit(nwpaw)
      call hropen(10,'LUN10','test.hb','n',1024,ier)
      call hbook1(1,'test1',100,-3.,3.,0.)
      call hbook2(2,'test2',30,-3.,3.,30,-3.,3.,0.)
      call hmdir('AA','S')
      call hcdir('//PAWC','S')
      call hmdir('//PAWC/AA','S')
      call hbook1(3,'test3',100,-3.,3.,0.)
      call hbook2(4,'test4',30,-3.,3.,30,-3.,3.,0.)
      call hcdir('//PAWC',' ')
      call hcdir('//LUN10',' ')
      call hrout(0,icycle,'T')
      call hrend('LUN10')
*     
      end
