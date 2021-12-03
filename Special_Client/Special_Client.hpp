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
bool start_client(int argc,char* argv[]);
};