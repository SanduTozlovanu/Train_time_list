#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include "Simple_Client.hpp"
#include "Instruments.hpp"
#define MSG1 "Quitting the client ...\n"
#define SEND_LIMIT 100
#define RECEIVE_LIMIT 1000
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
bool Simple_Client::start_client(int argc,char* argv[])
{
    int sd;			// descriptorul de socket
    struct sockaddr_in server;	// structura folosita pentru conectare 
    char send_message[SEND_LIMIT];
    char receive_message[RECEIVE_LIMIT];	// mesajul trimis

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

    /* citirea mesajului */
    while(1)
    {
        bzero (send_message, SEND_LIMIT);
        bzero (receive_message, RECEIVE_LIMIT);
        printf ("[client]Introduce a command: ");
        fflush (stdout);
        read (0, send_message, RECEIVE_LIMIT);
        std::cout<<"msg is :"<<send_message<<std::endl;
        std::string protocol_message;
        protocol_message+=get_station_id();
        protocol_message+=':';
        protocol_message+=send_message;
        char* protocolled_send_message=Instruments::string_to_char(protocol_message);
        
        /* trimiterea mesajului la server */
        if (write (sd, protocolled_send_message, SEND_LIMIT) <= 0)
            {
            perror ("[client]Error at write() toward server.\n");
            return errno;
            }

        /* citirea raspunsului dat de server 
            (apel blocant pina cind serverul raspunde) */
        if (read (sd, receive_message, RECEIVE_LIMIT) < 0)
            {
            perror ("[client]Error at read() toward server.\n");
            return errno;
            }
        /* afisam mesajul primit */
        printf ("[client]Received message is: %s\n", receive_message);
        if (strcmp(receive_message,MSG1)==0)
        {
            close (sd);
            return true;
        }
    }
    /* inchidem conexiunea, am terminat */
}
int main (int argc, char *argv[])
{
    Simple_Client c1;
    c1.start_client(argc,argv);
}
