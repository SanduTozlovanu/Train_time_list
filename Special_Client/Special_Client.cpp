#include "../Utilities/Settings.hpp"
#include "../Utilities/Instruments.hpp"
#include "Special_Client.hpp"
void Special_Client::set_port(int port)
{
    this->port=port;
}
char* Special_Client::get_train_id()
{
    return this->train_id;
}
void Special_Client::set_train_id(char * id_statie)
{
    this->train_id=id_statie;
}
int Special_Client::get_port()
{
    return this->port;
}
bool Special_Client::interact_with_server(int sd,int type)
{
    char send_message[SEND_LIMIT];
    char receive_message[RECEIVE_LIMIT];
    bzero (send_message, SEND_LIMIT);
    bzero (receive_message, RECEIVE_LIMIT);
    if(type==1)
    {
        read (0, send_message, RECEIVE_LIMIT);
    }
    else
    {
        std::string verify;
        verify+="verify";
        for(int i=0;i<verify.length();i++)
        {
            send_message[i]=verify[i];
        }
    }
    std::cout<<"msg is :"<<send_message<<std::endl;
    std::string protocol_message;
    protocol_message+="train:";
    protocol_message+=get_train_id();
    protocol_message+=':';
    protocol_message+=send_message;
    char* protocolled_send_message=Instruments::string_to_char(protocol_message);
    int wc1=strlen(protocolled_send_message);
    int wc2;
    /* trimiterea mesajului la server */
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
    printf ("[client]Received message is: %s\n", receive_message);
    if (strcmp(receive_message,MSG_EXIT)==0||strcmp(receive_message,WRONG_ID)==0)
    {
        close (sd);
        return false;
    }
    return true;
}
bool Special_Client::start_client(int argc,char* argv[])
{
    int sd;			// descriptorul de socket
    struct sockaddr_in server;	// structura folosita pentru conectare 	

    /* exista toate argumentele in linia de comanda? */
    if (argc != 4)
        {
        printf ("Sintax: %s <server_adress> <port> <train_id>\n", argv[0]);
        return -1;
        }

    /* stabilim portul */
    set_port(atoi (argv[2]));
    set_train_id(argv[3]);
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
    if(!interact_with_server(sd,2))
    {
        return false;
    }
    printf ("List of commands:\n");
    std::cout<<"get_my_stations\n";
    std::cout<<"report_sooner (number of minutes)\n";
    std::cout<<"report_delay (number of minutes)\n";
    std::cout<<"get_current_time\n";
    std::cout<<"exit\n";
    fflush (stdout);
    /* citirea mesajului */
    bool response=true;
    while(response==true)
    {
        response=interact_with_server(sd,1);
    }
    return true;
    /* inchidem conexiunea, am terminat */
}
int main (int argc, char *argv[])
{
    Special_Client c1;
    c1.start_client(argc,argv);
}
