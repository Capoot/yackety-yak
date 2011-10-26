include ..\makefile.rules

client: compile archive clean

compile:
	$(GCC) $(CFLAGS) main.c
	
archive:
	ar rcs ..\..\lib\libclient.a main.o
	
clean:
	del *.o