include ..\makefile.rules

LIBNET = ..\..\lib\libnet.a

client: compile archive

compile:
	$(GCC) $(CFLAGS) network.c
	
archive:
	ar rcs $(LIBNET) network.o
	del *.o

clean:
	del *.o
	del $(LIBNET)
