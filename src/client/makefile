include ..\makefile.rules

LIBCLIENT = ..\..\lib\libclient.a

client: compile archive

compile:
	$(GCC) $(CFLAGS) main.c
	$(GCC) $(CFLAGS) cl_engine.c
	
archive:
	ar rcs $(LIBCLIENT) main.o cl_engine.o
	del *.o

clean:
	del *.o
	del $(LIBCLIENT)
