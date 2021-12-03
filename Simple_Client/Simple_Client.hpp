#pragma once
class Simple_Client
{
private:
int port;
char* station_id;
public:
void set_station_id(char*);
char* get_station_id();
void set_port(int port);
int get_port();
bool start_client(int argc,char* argv[]);
};