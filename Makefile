#fisier folosit pentru compilarea serverului&clientului TCP concurent

all:
	g++  -g Server/Server.cpp Utilities/pugixml.cpp -o Server/Server
	g++  -g Simple_Client/Simple_Client.cpp -o Simple_Client/Simple_Client
	g++  -g Special_Client/Special_Client.cpp -o Special_Client/Special_Client
clean:
	rm -f Server/Server Simple_Client/Simple_Client Special_Client/Special_Client