include ..\makefile.rules

LIBSRV = ..\..\lib\libserver.a

server: compile archive

compile:
	$(GCC) $(CFLAGS) main.c
	$(GCC) $(CFLAGS) srv_engine.c
	
archive:
	ar rcs $(LIBSRV) main.o srv_engine.o
	del *.o

clean:
	del *.o
	del $(LIBSRV)
