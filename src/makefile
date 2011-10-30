include makefile.rules

SERVER_EXE = ..\server\yaksrv.exe
CLIENT_EXE = ..\client\yak.exe
LIBWS232 = ..\lib\libws2_32.a

all: compile link

compile:
	$(MAKE) -C protocol
	$(MAKE) -C network
	$(MAKE) -C client
	
link:
	$(GCC) ..\lib\libprotocol.a ..\lib\libclient.a ..\lib\libnet.a $(LIBWS232) -o $(CLIENT_EXE)
	
clean:
	$(MAKE) -C protocol clean
	$(MAKE) -C client clean
