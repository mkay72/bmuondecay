libradial.a: radial.o radial_dbound.o radial_dfree.o v_fermi.o
	ar rcs libradial.a $^ 

radial.o: radial.f
	$(FC) -c -std=legacy -O2 $^

radial_dbound.o: radial_dbound.f
	$(FC) -c -std=legacy -O2 $^
	
radial_dfree.o: radial_dfree.f
	$(FC) -c -std=legacy -O2 $^
	
v_fermi.o: v_fermi.f
	$(FC) -c -std=legacy -O2 $^
