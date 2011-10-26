include makefile.rules

SERVER_EXE = ..\server\yaksrv.exe
CLIENT_EXE = ..\client\yak.exe

all: compile link

compile:
	$(MAKE) -C protocol
	$(MAKE) -C client
	
link:
	$(GCC) ..\lib\libprotocol.a ..\lib\libclient.a -o $(CLIENT_EXE)
	
clean:
	$(MAKE) -C protocol clean