#pragma once
class Server
{
    private:
    const int port=PORT_NUMBER;
    int get_port();
    public:
    bool start_server();

};
void sighandler(int sig);