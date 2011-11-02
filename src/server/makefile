include ..\makefile.rules

LIBSRV = ..\..\lib\libserver.a

server: compile archive

compile:
	$(GCC) $(CFLAGS) main.c
	$(GCC) $(CFLAGS) comline.c
	
archive:
	ar rcs $(LIBSRV) main.o comline.o
	del *.o

clean:
	del *.o
	del $(LIBSRV)
