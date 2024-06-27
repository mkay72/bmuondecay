hfd: fermi.o cgc.o
	ar rcs libhfd.a $^ 

cgc.o: cgc.f90
	$(FC) -c $^
	
fermi.o: fermi.f
	$(FC) -c $^

