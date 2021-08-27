      program toy
      parameter (nwpaw=12000000)
      common/pawc/paw(nwpaw)
      
      integer idvec(1000000)
      integer noh
      call hlimit(nwpaw)
      call hlimap(0,'TEST')
      
      call hrin(0,9999,0)
      call hidall(idvec,noh)
      
*      do i = 1, noh
*         id = idvec(i)
*         call hprint(id)
*         call hdelet(id)
*      enddo
      
      end
      
