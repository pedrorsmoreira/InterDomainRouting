CFLAGS= -O3 -Ofast -std=gnu11

default: program

program: *.c
	gcc *.c -o InterDomainRouting $(CFLAGS)

debug: *.c
	gcc -Wall -pg *.c -o InterDomainRouting-debug $(CFLAGS)

run:
	./InterDomainRouting -f files/LargeNetwork.txt

run-debug:
	./InterDomainRouting-debug -f files/LargeNetwork.txt; gprof InterDomainRouting-debug gmon.out > debug.txt

clean:
	rm -f *.o InterDomainRouting InterDomainRouting-debug gmn.out
