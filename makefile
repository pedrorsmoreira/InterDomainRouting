CFLAGS= -O2 -Ofast -std=gnu11

default: program

program: *.c
	gcc *.c -o InterDomainRouting $(CFLAGS)

debug: *.c
	gcc -Wall -pg *.c -o InterDomainRouting-debug $(CFLAGS)

run:
	./InterDomainRouting -f LargeNetwork.txt

run-debug:
	./InterDomainRouting-debug -f LargeNetwork.txt; gprof InterDomainRouting-debug gmon.out > debug.txt

clean:
	rm -f *.o InterDomainRouting InterDomainRouting-debug gmn.out