include ..\makefile.rules

LIBCLIENT = ..\..\lib\libclient.a

client: compile archive

compile:
	$(GCC) $(CFLAGS) main.c
	$(GCC) $(CFLAGS) comline.c
	
archive:
	ar rcs $(LIBCLIENT) main.o comline.o
	del *.o

clean:
	del *.o
	del $(LIBCLIENT)
