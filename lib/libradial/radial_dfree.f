      subroutine radial_dfree(z, rms_fm, knu, m, e_ru, kp,
     & nr, r_in, g_out, f_out, phase)
      USE CONSTANTS
C
      IMPLICIT DOUBLE PRECISION (A-H,O-Z), INTEGER*4 (I-N)
      integer(4), intent(in) :: nr
      real(8), intent(in) :: m
      real(8), intent(in) :: e_ru
      real(8), intent(in) :: r_in(nr)
      real(8), intent(out) :: g_out(nr)
      real(8), intent(out) :: f_out(nr)
      real(8), intent(out) :: phase
      
      real(8) :: r_in_gau(nr)      
      
      CHARACTER fname*50
      PARAMETER (PI=3.1415926535897932D0)
      DIMENSION DR0(NDIM)
C  ****  Potential.
c       COMMON/CVPOT/Z,V0,A,IPOT
      DIMENSION R0(NDIM),RV0(NDIM)
C  ****  Output radial functions.
      COMMON/RADWF/RAD(NDIM),P(NDIM),Q(NDIM),NGP,ILAST,IER
C  ****  Coulomb wave function parameters.
      COMMON/OCOUL/RK,ETA,DELTA
C
      EXTERNAL vpot_fermi
      real(8) :: alpha = 0.0072973525693d0
      real(8) :: fm2au = 1.d-5/0.529177210903d0
      real(8) :: norm

      
      z = 1.d0*(-abs(z))
      EPS = 1e-12     
      ipot = knu
      
      fm2au = fm2au*m

      
c       write(*,42) e_ru, kp
c 42    format('# e   = ',f7.2,/,
c      &       '# kp  = ',i3)      
c       
c       write(*,43) z
c       write(*,44) rms_fm
c       write(*,46) m
c 43    format('# Z   = ',f7.2,1x)
c 44    format('# RMS = ',f7.4,1x,'[fm]')
c 46    format('# m   = ',f7.4,1x,'[ru]',/)
c 
c       write(*,45) aa, aa/fm2au, cc, cc/fm2au, tt*fm2au, tt
c 45    format ('# Parameters of the Fermi model: ',/,
c      &  '# a = ',e13.6,1x,'[au] = ',f7.4,1x,'[fm]',/,
c      &  '# c = ',e13.6,1x,'[au] = ',f7.4,1x,'[fm]',/,
c      &  '# t = ',e13.6,1x,'[au] = ',f7.4,1x,'[fm]',/)   
     
      
      do i = 1, nr
        r_in_gau(i) = r_in(i) * alpha * m
c         write(*,*) r_in(i), r_in_gau(i)
      enddo      
      
      NGP = 2048*2
      rn = 15.d0*r_in_gau(nr)

 66   CONTINUE
 
      sgrid_success = 0

      do while (sgrid_success < 1)
        
        CALL SGRID(RAD,DR0,RN,1.0D-6,1.D0,NGP,NDIM,IERS)
        if (IERS.EQ.0) then 
            sgrid_success = 1
        else
            rn = 2.d0 * rn
        end if
      end do
      
c       write(*,*) rn
      
c       IF(IERS.NE.0) STOP 'Error in SGRID.'
      
      
           
      rnucl_fm = dsqrt(5.d0/3.d0) * rms_fm
      rnucl = rnucl_fm * fm2au
      rms = rnucl / dsqrt(5.d0/3.d0)
      
      tt = 2.30d0
      aa = tt/(4.d0*dlog(3.d0))*fm2au
      cc = 5.d0/3.d0*rms**2-7.d0/3.d0*pi**2*aa**2
      cc = dsqrt(cc)
      
   
     
     
     
c       fname = 'nuclpot.dat'
c       open(13, file=fname)
c       write(13, *) '# Nuclear potential [au]'
c       write(13, *) '# r                 V_nucl*r'
      if (ipot.eq.2) then
        do i=1, NGP, 1
            r = RAD(i)
            vf = vpot_fermi(-z, cc, aa, r)
            R0(i) = r 
            RV0(i) = vf
            NV = i
c             write(13,*) r, vf
        enddo
      else if (ipot.eq.1) then
        do i=1, NGP, 1
            r = RAD(i)
            vf = z
            R0(i) = r 
            RV0(i) = vf
            NV = i
c             write(13,*) r, vf
        enddo
      endif
c       close(13)
     

c       OPEN(3, FILE=potname)
c       DO I=1,NDIM
c  1      CONTINUE
c         READ(3,*,ERR=1,END=10) R0(I),RV0(I)
c         NV=I
c       ENDDO
c  10   CONTINUE
c       CLOSE(3)

C
C  ****  Spline interpolation of the potential.
      CALL VINT(R0,RV0,NV)
c       RANGE=VRANGE()  
        

c       estimated eigenvalue [ru] (for Coulomb potential)      
c       gam = dsqrt(1.d0*kp*kp - (alpha*z)*(alpha*z))
c       e = 1.d0 / dsqrt(1.d0 + ((alpha*z)/(n - abs(kp) + gam))**2)
c       e =  (e - 1.d0)/ (alpha*alpha)
      e_au = e_ru / (alpha*alpha)
      CALL DFREE(e_au, eps, phase, kp, 1)
c       IF(IER.NE.0) STOP 'Error in DFREE.'
      IF(IER.NE.0) then
        rn = 2.0 * rn
        write(*,108) rn
        goto 66
      end if
      
 108     format(5X,'Attempt to fix: RN is extended to', E13.6)     
    
c       write(*,*) "ETA   = ", eta
c       write(*,*) "RK    = ", RK*alpha


      
      NTAB=NGP
      DO I=NGP,1,-1
        IF(ABS(P(I)).GT.1.0D-35) THEN
          NTAB=I
          GO TO 30
        ENDIF
      ENDDO
 30   CONTINUE


c     output of the program in generalized atomic units
c       fname = 'dirac.dat'
c       open(10, file = fname)
c       
c 1501  FORMAT('# Radial wave functions calculated by RADIAL.')
c 1301  FORMAT('# **** Dirac equation. ',/
c      1 '# Z = ',E13.6,/
c      2 '# Bound state: n =',I4,', kp =',I4,'  (EPS=',E8.1,')',/
c      3 '# Binding energy = ',E22.15)
c       WRITE(10,1501)
c       WRITE(10,1301) Z, n, kp, EPS, E
c 
c       WRITE(10,1502)
c 1502  FORMAT('#',3X,'R',25X,'P(R)',22X,'Q(R)')
c       DO I=1, NTAB
c C  ----  Do not print values less than 1.0D-99  ------------------------
c         IF(ABS(P(I)).LT.1.0D-99) P(I)=0.0D0
c         IF(ABS(Q(I)).LT.1.0D-99) Q(I)=0.0D0
c C  ---------------------------------------------------------------------
c       WRITE(10,'(1X,1P,3E26.16)') RAD(I), P(I), Q(I)
c       ENDDO
c       CLOSE(10)
      

      norm = dsqrt((RK*alpha)/(PI*e_au*(alpha*alpha)))
      factor = alpha * m
      sqrt_factor = factor
c     interpolation of the radial wave functions
c     to the user grid
      CALL PSIINT(RAD, P, NTAB)
c       RANGE=VRANGE()
      
      do i = 1, nr
c         xi_ru = r_in(i)
c         xi_au = xi_ru * factor
c         g_out(i) = norm*RVSPL(xi_au)
          xi = r_in_gau(i)
          g_out(i) = RVSPL(xi) * norm          
      enddo
      
      CALL PSIINT(RAD, Q, NTAB)
c       RANGE=VRANGE()
      do i = 1, nr
c         xi_ru = r_in(i)
c         xi_au = xi_ru * factor
c         f_out(i) = norm*RVSPL(xi_au)
          xi = r_in_gau(i)
          f_out(i) = RVSPL(xi) * norm        
      enddo   

     
      end subroutine     
