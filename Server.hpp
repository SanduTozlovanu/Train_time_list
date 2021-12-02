#include <iostream>
class Server
{
    private:
    const int port=2024;
    std::string xml_file = "tree.xml";
    int get_port();
    std::string get_xml_file();
    char* process_message(char*);
    public:
    bool start_server();

};
void sighandler(int sig);