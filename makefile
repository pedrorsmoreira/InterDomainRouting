CFLAGS= -O2 -Ofast -std=gnu11

default: program

program: *.c
	gcc *.c -o InterDomainRouting $(CFLAGS)

run:
	./InterDomainRouting -f LargeNetwork.txt

clean:
	rm -f *.o InterDomainRouting