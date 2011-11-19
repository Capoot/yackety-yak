include ..\makefile.rules

LIBUTIL = ..\..\lib\libutil.a

server: compile archive

compile:
	$(GCC) $(CFLAGS) util.c
	
archive:
	ar rcs $(LIBUTIL) util.o
	del *.o

clean:
	del *.o
	del $(LIBUTIL)
