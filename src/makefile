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
	$(MAKE) -C server
	$(MAKE) -C util
	
link:
	$(GCC) -o $(CLIENT_EXE) ..\lib\libclient.a ..\lib\libprotocol.a ..\lib\libnet.a ..\lib\libutil.a $(LIBWS232)
	$(GCC) -o $(SERVER_EXE) ..\lib\libserver.a ..\lib\libprotocol.a ..\lib\libnet.a ..\lib\libutil.a $(LIBWS232)
	
clean:
	$(MAKE) -C protocol clean
	$(MAKE) -C network clean
	$(MAKE) -C client clean
	$(MAKE) -C server clean
	$(MAKE) -C util clean
