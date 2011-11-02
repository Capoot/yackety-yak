include ..\makefile.rules

LIBSRV = ..\..\lib\libserver.a

server: compile archive

compile:
	$(GCC) $(CFLAGS) main.c
	
archive:
	ar rcs $(LIBSRV) main.o
	del *.o

clean:
	del *.o
	del $(LIBSRV)
