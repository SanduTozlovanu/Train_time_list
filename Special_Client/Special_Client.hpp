#pragma once
class Special_Client
{
private:
int port;
char* train_id;
public:
void set_train_id(char*);
char* get_train_id();
void set_port(int port);
int get_port();
bool interact_with_server(int sd,int type);
bool start_client(int argc,char* argv[]);
};