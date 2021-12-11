#pragma once
class Simple_Client
{
private:
int port;
char* station_id;
public:
void set_station_id(char* station_id);
char* get_station_id();
void set_port(int port);
int get_port();
bool id_verifier(int sd);
bool interact_with_server(int sd);
bool start_client(int argc,char* argv[]);
};