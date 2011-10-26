include ..\makefile.rules

protocol: compile archive clean

compile:
	$(GCC) $(CFLAGS) message_factory.c
	$(GCC) $(CFLAGS) message_utility.c
	
archive:
	ar rcs ..\..\lib\libprotocol.a message_factory.o message_utility.o
	
clean:
	del *.o