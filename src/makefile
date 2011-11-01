include makefile.rules

SERVER_EXE = ..\server\yaksrv.exe
CLIENT_EXE = ..\client\yak.exe
LIBWS232 = ..\lib\libws2_32.a
LIBWSOCK32 = ..\lib\libwsock32.a

all: compile link

compile:
	$(MAKE) -C protocol
	$(MAKE) -C network
	$(MAKE) -C client
	
link:
	$(GCC) -o $(CLIENT_EXE) ..\lib\libprotocol.a ..\lib\libclient.a ..\lib\libnet.a $(LIBWS232)
#$(LIBWSOCK32) -lws2_32
	
clean:
	$(MAKE) -C protocol clean
	$(MAKE) -C client clean
