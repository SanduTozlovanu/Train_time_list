#include "../Utilities/Settings.hpp"
#include "../Utilities/Instruments.hpp"
#include "Simple_Client.hpp"
void Simple_Client::set_port(int port)
{
    this->port=port;
}
char* Simple_Client::get_station_id()
{
    return this->station_id;
}
void Simple_Client::set_station_id(char * id_statie)
{
    this->station_id=id_statie;
}
int Simple_Client::get_port()
{
    return this->port;
}
bool Simple_Client::id_verifier(int sd)
{
    std::string protocol_message;
    char receive_message[RECEIVE_LIMIT];
    int wc2;
    protocol_message+="station:";
    protocol_message+=get_station_id();
    protocol_message+=":verify";
    char *protocolled_send_message=Instruments::string_to_char(protocol_message);
    int wc1=strlen(protocolled_send_message);
    if (write (sd,&wc1, sizeof(int)) <= 0)
    {
        perror ("[client]Error at write() toward server.\n");
        return errno;
    }

    if (write (sd, protocolled_send_message, sizeof(char)*wc1+1) <= 0)
    {
        perror ("[client]Error at write() toward server.\n");
        return errno;
    }
    /* citirea raspunsului dat de server 
        (apel blocant pina cind serverul raspunde) */
    if (read (sd,&wc2,sizeof(int)) < 0)
    {
        perror ("[client]Error at read() toward server.\n");
        return errno;
    }
    if (read (sd, receive_message, sizeof(char)*wc2+1) < 0)
    {
        perror ("[client]Error at read() toward server.\n");
        return errno;
    }
    std::cout<<receive_message<<std::endl;
    if (strcmp(receive_message,MSG_EXIT)==0||strcmp(receive_message,WRONG_ID)==0)
    {
        close (sd);
        return false;
    }
    return true;
}
bool Simple_Client::interact_with_server(int sd)
{
    char send_message[SEND_LIMIT];
    char receive_message[RECEIVE_LIMIT];
    std::string protocol_message;
    int wc1;
    int wc2;
    std::string pressed;
    int bad_command=0;
    do
    {
        bad_command=0;
        bzero (send_message, SEND_LIMIT);
        bzero (receive_message, RECEIVE_LIMIT);
        printf ("Introduce a number:\n");
        fflush (stdout);
        protocol_message.clear();
        protocol_message+="station:";
        protocol_message+=get_station_id();
        protocol_message+=':';
        int signal=0;
        char* protocolled_send_message;
        cnt1:;
        std::cout<<"1 : Get the next incoming trains real-time"<<std::endl;
        std::cout<<"2 : Get all the trains that come/came today"<<std::endl;
        std::cout<<"3 : Get the name of the station"<<std::endl;
        std::cout<<"4 : Get current time"<<std::endl;
        std::cout<<"5 : Exit"<<std::endl;
        std::cin>>pressed;
        if((pressed.compare("1")==0)||(pressed.compare("2")==0)||(pressed.compare("3")==0)||(pressed.compare("4")==0)||(pressed.compare("5")==0))
        {
            int int_pressed=stoi(pressed);
            switch(int_pressed) 
            {
                case 1: {protocol_message+="get_my_trains_update\n";protocolled_send_message=Instruments::string_to_char(protocol_message);wc1=strlen(protocolled_send_message);signal=1;break;}
                case 2: {protocol_message+="get_my_trains\n";protocolled_send_message=Instruments::string_to_char(protocol_message);wc1=strlen(protocolled_send_message);break;}
                case 3: {protocol_message+="get_station_name\n";protocolled_send_message=Instruments::string_to_char(protocol_message);wc1=strlen(protocolled_send_message);break;}
                case 4: {protocol_message+="get_current_time\n";protocolled_send_message=Instruments::string_to_char(protocol_message);wc1=strlen(protocolled_send_message);break;}
                case 5: {protocol_message+="exit\n";protocolled_send_message=Instruments::string_to_char(protocol_message);wc1=strlen(protocolled_send_message);break;}
            }
        }
        else
        {
            system("clear");
            std::cout<<"Inserted value should be from 1 to 5"<<std::endl;bad_command=1;
        }
        /* trimiterea mesajului la server */
        if(!bad_command)
        {
            cnt2:;
            if (write (sd,&wc1, sizeof(int)) <= 0)
                {
                    perror ("[client]Error at write() toward server.\n");
                    return errno;
                }

            if (write (sd, protocolled_send_message, sizeof(char)*wc1+1) <= 0)
                {
                    perror ("[client]Error at write() toward server.\n");
                    return errno;
                }

            /* citirea raspunsului dat de server 
                (apel blocant pina cind serverul raspunde) */
            if (read (sd,&wc2,sizeof(int)) < 0)
                {
                    perror ("[client]Error at read() toward server.\n");
                    return errno;
                }
            if (read (sd, receive_message, sizeof(char)*wc2+1) < 0)
                {
                    perror ("[client]Error at read() toward server.\n");
                    return errno;
                }
            /* afisam mesajul primit */
            system("clear");
            std::cout<<receive_message<<std::endl;
            if (strcmp(receive_message,MSG_EXIT)==0)
            {
                close (sd);
                return true;
            }
            else if (signal)
            {
                sleep(5);
                goto cnt2;

            }
        }
    } while(1);
    return true;
}
bool Simple_Client::start_client(int argc,char* argv[])
{
    int sd;			// descriptorul de socket
    struct sockaddr_in server;	// structura folosita pentru conectare 

    /* exista toate argumentele in linia de comanda? */
    if (argc != 4)
        {
        printf ("Sintax: %s <server_adress> <port> <station_id>\n", argv[0]);
        return -1;
        }

    /* stabilim portul */
    set_port(atoi (argv[2]));
    set_station_id(argv[3]);
    int Port = get_port();
    /* cream socketul */
    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
        {
        perror ("Error at socket().\n");
        return errno;
        }
    /* umplem structura folosita pentru realizarea conexiunii cu serverul */
    /* familia socket-ului */
    server.sin_family = AF_INET;
    /* adresa IP a serverului */
    server.sin_addr.s_addr = inet_addr(argv[1]);
    /* portul de conectare */
    server.sin_port = htons (Port);
    
    /* ne conectam la server */
    if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
        {
        perror ("[client]Error at connect().\n");
        return errno;
        }
    if(!id_verifier(sd)) return false;
    if(interact_with_server(sd)) return true;
    else return false;
}
int main (int argc, char *argv[])
{
    Simple_Client c1;
    c1.start_client(argc,argv);
}
