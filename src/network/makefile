include ..\makefile.rules

LIBNET = ..\..\lib\libnet.a

client: compile archive

compile:
	$(GCC) $(CFLAGS) network_wsockimpl.c
	
archive:
	ar rcs $(LIBNET) network_wsockimpl.o
	del *.o

clean:
	del *.o
	del $(LIBNET)
