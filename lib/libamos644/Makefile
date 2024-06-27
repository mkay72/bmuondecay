libamos644.a: cbsubs.o zbsubs.o machcon.o
	ar rcs libamos644.a $^ 

cbsubs.o: cbsubs.f
	$(FC) -c -O2 $^

zbsubs.o: zbsubs.f
	$(FC) -c -O2 $^

machcon.o: machcon.f
	$(FC) -c -O2 $^


