! This file is a part of the program hfd
! for details, see Refs.
!
! [1] V. F. Bratzev, G. B. Deyneka, and I. I. Tupitsyn, 
!     Bull. Acad. Sci. USSR, Phys. Ser. 41, 173 (1977) 
!     [Izv. Acad. Nauk SSSR, Ser. Fiz. 41, 2655 (1977)].
!
! [2] I. I. Tupitsyn, V. M. Shabaev, 
!     J. R. Crespo López-Urrutia, I. Draganić, 
!     R. S. Orts, and J. Ullrich, Phys. Rev. A 68, 022511 (2003).
!
! [3] I. I. Tupitsyn, A. V. Volotka, D. A. Glazov, 
!     V. M. Shabaev, G. Plunien, J. R. Crespo López-Urrutia, 
!     A. Lapierre, and J. Ullrich, Phys. Rev. A 72, 062503 (2005).
!
! [4] I. I. Tupitsyn, N. A. Zubova, V. M. Shabaev, 
!     G. Plunien, and T. Stöhlker, Phys. Rev. A 98, 022517 (2018).
!

        subroutine vpot_fermi(znucl, rms_fm, mass,
     &  nr, r, vr)
        integer(4) :: nr
        real(8) :: znucl, rms_fm, mass
        real(8) :: rnucl_fm, rms, tt, aa, cc
        real(8) :: r(nr), vr(nr)
        real(8) :: fm2au = 1.d-5/0.529177210903d0
        real(8) :: pi = 3.1415926535897932d0        
        
        real(8) :: vpot_fermi_kernel
        external vpot_fermi_kernel
        
        
        
        fm2au = fm2au * mass
        
        rnucl_fm = dsqrt(5.d0/3.d0) * rms_fm
        rnucl = rnucl_fm * fm2au
        rms = rnucl / dsqrt(5.d0/3.d0)
        
        tt = 2.30d0
        aa = tt/(4.d0*dlog(3.d0))*fm2au
        cc = 5.d0/3.d0*rms**2-7.d0/3.d0*pi**2*aa**2
        cc = dsqrt(cc)
               
        do i = 1, nr
            vr(i) = vpot_fermi_kernel(-znucl, cc, aa, r(i))
c             write(*,*) r(i), vr(i)
        enddo
        
        end subroutine vpot_fermi
c       =================================================              
        function vpot_fermi_kernel(Z,C,A,R)
c       - - - - - - - - - - - - - - - - - - - - - - - - -
        implicit real*8 (A-H,O-Z)
        real(8) :: vpot_fermi_kernel
c       - - - - - - - - - - - - - - - - - - - - - - - - -
        parameter (pi=3.1415926535897932385d0)
c       - - - - - - - - - - - - - - - - - - - - - - - - -
        dn=1+(pi*a/c)**2-6d0*((a/c)**3)*SK(3,-c/a)
        
c       - - - - - - - - - - - - - - - - - - - - - - - - -      
        IF(R.LT.C) THEN
        B1=-6d0*((a/c)**3)*SK(3,-c/a)+
     &  6d0*((a/c)**3)*SK(3,(r-c)/a)
        B2=r/c*(3d0/2d0+((pi*a/c)**2)/
     &  2d0-3d0*((a/c)**2)*SK(2,(r-c)/a))
        B3=-((r/c)**3)/2d0
        V=Z/dn*(B1+B2+B3)
c       - - - - - - - - - - - - - - - - - - - - - - - - -      
        ELSE
c       - - - - - - - - - - - - - - - - - - - - - - - - -      
        B1=dn
        B2=3d0*((a/c)**2)*(r/c*SK(2,(c-r)/a)+
     &  2d0*a/c*SK(3,(c-r)/a))
        V=Z/dn*(B1+B2)
        END IF
c       - - - - - - - - - - - - - - - - - - - - - - - - -      	
        vpot_fermi_kernel=-V
c       - - - - - - - - - - - - - - - - - - - - - - - - -      
        RETURN
        END

c       =================================================
        function sk(k,x)
c       - - - - - - - - - - - - - - - - - - - - - - - - -
        implicit real*8 (a-h,o-z)
c       - - - - - - - - - - - - - - - - - - - - - - - - -
        parameter (error=1d-20)
c       - - - - - - - - - - - - - - - - - - - - - - - - -
        dk=K
        N=0
        sk=0.d0
c       - - - - - - - - - - - - - - - - - - - - - - - - -
10      N=N+1
        dn=N
        add=DEXP(dN*X)/(dN**dK)*(-1)**N
        if(N.eq.1) add1=add
        sk=sk+add
        if (add.eq.0d0) goto 1000
        if (dabs(add/add1).lt.error) goto 1000
        goto 10
c       - - - - - - - - - - - - - - - - - - - - - - - - -      
1000    return
        end
c       =================================================      
