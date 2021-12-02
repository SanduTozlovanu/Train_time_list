#include <iostream>
class Server
{
    private:
    const int port=2024;
    std::string xml_file = "tree.xml";
    bool start_server();
    int get_port();
    std::string get_xml_file();
    public:
    bool start_server();

};
void sighandler(int sig);