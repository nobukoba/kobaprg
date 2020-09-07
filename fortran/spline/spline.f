      program main
      integer n
      parameter (n=9)
      real x(0:n), a(0:n)
      data x/
     &     0.0, 1.0, 2.0,  3.0, 4.0,
     &     5.0, 6.0, 7.0, 9.0, 10.0/
      data a/
     &     2.0, 1.0, 3.0, 4.0, 5.0,
     &     4.0, 2.0, 3.0, 5.0, 9.0/
      common /comxa/ x,a
      integer i
      real xx, yy, spline
      
      do i=1,1001,1
         xx = (i-1) * 0.01
         yy = spline(xx)
         write(*,*) 'i, x, y = ', i, xx, yy
         write(20,*) xx, yy
      enddo

      do i=0,n
         write(21,*) x(i), a(i)
      enddo

      end
      
      real function spline(x_in)
      real x_in
      integer n
      parameter (n=9)
      real x(0:n), a(0:n)
      real c(0:n,3), h(0:n), al(0:n), l(0:n), mu(0:n), z(0:n)
      integer i,j,i_cur
      common /comxa/ x,a
      
      i_cur = 0
      do i=0, n
         if (x_in .lt. x(i)) then
            i_cur = i -1
            exit
         endif
      enddo
      do i=0,n-1
         h(i) = x(i+1) - x(i)
c     write (*,*) 'i,h(i)',i, h(i)
      enddo
      do i=1,n-1
         al(i) = 3.*(a(i+1)-a(i))/h(i) - 3.*(a(i)-a(i-1))/h(i-1)
c     write (*,*) al(i)
      enddo
      l(0)  = 1.
      mu(0) = 0.
      z(0)  = 0.
      do i=1,n-1
         l(i)  = 2.*(x(i+1)-x(i-1)) - h(i-1)*mu(i-1)
         mu(i) = h(i)/l(i)
         z(i)  = (al(i)-h(i-1)*z(i-1))/l(i)
c     write (*,*) l(i),mu(i),z(i)
      enddo
      l(n)   = 1.
      z(n)   = 0.
      c(n,2) = 0.
      do j=n-1,0,-1
         c(j,2) = z(j)-mu(j)*c(j+1,2)
         c(j,1) = (a(j+1)-a(j))/h(j) - h(j)*(c(j+1,2)+2.*c(j,2))/3.
         c(j,3) = (c(j+1,2)-c(j,2))/h(j)/3.
c     write (*,*) j,x(j),a(j),c(j,1),c(j,2),c(j,3)
      enddo
      
      spline = a(i_cur)
     &     + c(i_cur,1)*(x_in-x(i_cur))
     &     + c(i_cur,2)*(x_in-x(i_cur))**2
     &     + c(i_cur,3)*(x_in-x(i_cur))**3
      return
      end
      
      
      
      
