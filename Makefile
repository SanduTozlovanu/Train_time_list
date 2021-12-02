#fisier folosit pentru compilarea serverului&clientului TCP iterativ

all:
	g++  -g Server.cpp pugixml.cpp -o Server
	g++  -g Simple_Client.cpp -o Simple_Client
clean:
	rm -f cliTcpIt servTcpIt