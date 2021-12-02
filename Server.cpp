#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include "Server.hpp"
#include "pugixml.hpp"
#include "Instruments.hpp"
#define RECEIVE_LIMIT 100
#define SEND_LIMIT 1000
#define MAX_QUEUE_CLIENT 20
#define OFFICIAL_XML_DOC "sntfc-cfr-cltori-s.a.-1232-trenuri_2021.xml"
#define MSG_EXIT "Quitting the client ...\n"
#define MSG_PROTOCOL_ERROR "Protocol error ...\n"
#define PROCESS_ERROR "Error...\n"
#define COMMAND_NOT_FOUND "Command not found...\n"

void sighandler (int sig)
{
      while (0 < waitpid(-1,NULL,WNOHANG))
  {
  }
}
int Server::get_port()
{
    return this->port;
}
std::string Server::get_xml_file()
{
    return this->xml_file;
}
char* Server::process_message(char* initial_message)
{
    std::string station_id;
    int station_id_length;
    for(int i=0;i<10;i++)
    {
        if(i==9)
        {
            std::string send_msg="Protocol error ...\n";
            char* char_send_msg2=Instruments::string_to_char(send_msg);
            return char_send_msg2;
        }
        if(initial_message[i]==':')
        {
            station_id_length=i;
            break;
        }
        station_id+=initial_message[i];
    }
    std::string filtered_msg;
    filtered_msg+=initial_message;
    filtered_msg.erase(0,station_id_length+1);
    char* temp=Instruments::string_to_char(filtered_msg);
    char filtered_receive_msg[RECEIVE_LIMIT];
    strcpy(filtered_receive_msg,temp);
    std::string send_msg;
    if(strcmp(filtered_receive_msg,"get_my_trains\n")==0)
    {
        pugi::xml_document doc;
        std::string file_name=get_xml_file();
        char* char_file_name=Instruments::string_to_char(file_name);
        if (!doc.load_file(char_file_name))
        {
            std::string error_msg;
            error_msg+=PROCESS_ERROR;
            char * error = Instruments::string_to_char(error_msg);
            return error;
        }
        delete char_file_name;
        for (pugi::xml_node tren = doc.child("Trenuri").first_child(); tren; tren = tren.next_sibling())
        {
            pugi::xml_node trasa=tren.child("Trase").child("Trasa");
            int flag=0;
            if((trasa.attribute("CodStatieFinala").value()==station_id)||(trasa.attribute("CodStatieInitiala").value()==station_id))
            {
                flag=1;
            }
            for (pugi::xml_node element_trasa = trasa.first_child(); element_trasa; element_trasa = element_trasa.next_sibling())
            {
                if(flag==1)
                {
                    break;
                }
                if((element_trasa.attribute("CodStaDest").value()==station_id)||(element_trasa.attribute("CodStaOrigine").value()==station_id))
                {
                    flag=1;
                }
            }
            if(flag==1)
            {
                send_msg+=tren.attribute("Numar").value();
                send_msg+='\n';
            }
        }
        char* char_send_msg2=Instruments::string_to_char(send_msg);
        return char_send_msg2;

    }
    if(strcmp(filtered_receive_msg,"exit\n")==0)
    {
        send_msg=MSG_EXIT;
        char* char_send_msg2=Instruments::string_to_char(send_msg);
        return char_send_msg2;
    }
    else if (strcmp(filtered_receive_msg,"getinfo\n")==0)
    {
        pugi::xml_document doc;
        std::string file_name=get_xml_file();
        char* char_file_name=Instruments::string_to_char(file_name);
        if (!doc.load_file(char_file_name))
        {
            std::string error_msg;
            error_msg+=PROCESS_ERROR;
            char * error = Instruments::string_to_char(error_msg);
            return error;
        }
        delete char_file_name;
        pugi::xml_node trains = doc.child("schedule");

        for (pugi::xml_node train = trains.first_child(); train; train = train.next_sibling())
        {
            std::string nume="IR1";
            if(train.first_attribute().value()==nume)
            {
                send_msg+="train:";
                send_msg+=train.first_attribute().name();
                send_msg+=" ";
                send_msg+=train.first_attribute().value();
                send_msg+="\n";
                for (pugi::xml_node station=train.first_child();station;station=station.next_sibling())
                {
                    for (pugi::xml_attribute attr = station.first_attribute(); attr; attr = attr.next_attribute())
                    {
                        send_msg+=" ";
                        send_msg+=attr.name();
                        send_msg+="=";
                        send_msg+=attr.value();
                    }
                    send_msg+="\n";
                }
            }
        }
    }
    else send_msg+=COMMAND_NOT_FOUND;
    /* returnam mesajul clientului */
    char* char_send_msg=Instruments::string_to_char(send_msg);
    return char_send_msg;
}
bool Server::start_server()
{
    struct sockaddr_in server;	// structura folosita de server
    struct sockaddr_in from;	
    char receive_msg[RECEIVE_LIMIT];		//mesajul primit de la client 
    int sd;			//descriptorul de socket 

    /* crearea unui socket */
    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
        {
        perror ("[server]socket() error\n");
        return errno;
        }

    /* pregatirea structurilor de date */
    bzero (&server, sizeof (server));
    bzero (&from, sizeof (from));
    
    /* umplem structura folosita de server */
    /* stabilirea familiei de socket-uri */
        server.sin_family = AF_INET;	
    /* acceptam orice adresa */
        server.sin_addr.s_addr = htonl (INADDR_ANY);
    /* utilizam un port utilizator */
        server.sin_port = htons (get_port());
    
    /* atasam socketul */
    int flag = 1;  
    if (-1 == setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag))) {  
        perror("setsockopt fail");  
    }  
    if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
        {
        perror ("[server]Error at bind().\n");
        return errno;
        }

    /* punem serverul sa asculte daca vin clienti sa se conecteze */
    if (listen (sd, MAX_QUEUE_CLIENT) == -1)
        {
        perror ("[server]Error at listen().\n");
        return errno;
        }
    int client;
    socklen_t length = sizeof (from);
    std::string official_name=OFFICIAL_XML_DOC;
    pugi::xml_document ceva=Instruments::filter_xml(official_name);
    /* servim in mod iterativ clientii... */
    while (1)
        {
        cnt:;
        printf ("[server]Waiting at port %d...\n",get_port());
        fflush (stdout);

        /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
        /* eroare la acceptarea conexiunii de la un client */
        client = accept (sd, (struct sockaddr *) &from, &length);
        if (client < 0)
            {
            perror ("[server]Errror at accept().\n");
            goto cnt;
            }
        int pid=fork();
        if (pid != 0)
        {
            close(client);
            if (signal(SIGCHLD,sighandler) == SIG_ERR)
            {
            perror("signal()");
            return 1;
            }
            goto cnt;
        }
        else
        {
        /* s-a realizat conexiunea, se astepta mesajul */
            while(1)
            {
                bzero (receive_msg, RECEIVE_LIMIT);
                printf ("[server]Waiting for message...\n");
                fflush (stdout);
                if (read (client, receive_msg, RECEIVE_LIMIT) <= 0)
                {
                    perror ("[server]Error at read() from the client.\n");
                    close (client);	/* inchidem conexiunea cu clientul */
                    goto cnt;
                }
                    /* continuam sa ascultam */
                char* send_message=process_message(receive_msg);
                if ((strcmp(send_message,MSG_EXIT)==0)||(strcmp(send_message,MSG_PROTOCOL_ERROR)==0)||(strcmp(send_message,PROCESS_ERROR)==0))
                {
                    if (write (client,send_message, SEND_LIMIT) <= 0)
                    {
                        perror ("[server]Error at write() towards client.\n");
                        close (client);
                        goto cnt;
                    }
                    else
                    {
                        delete send_message;
                        printf ("[server]The message was successfully sent.\n");
                        std::cout<<"Client Shutdown..."<<std::endl;
                        close (client);
                        goto cnt; 
                    } 
                }
                else
                {
                    printf("[server]Sending message back...%s\n",send_message);
                    if (write (client,send_message, SEND_LIMIT) <= 0)
                    {
                        perror ("[server]Error at write() toward client.\n");
                        delete send_message;
                        goto cnt;		/* continuam sa ascultam */
                    }
                    else
                    delete send_message;
                    printf ("[server]The message was successfully sent.\n");
                        /* am terminat cu acest client, inchidem conexiunea */
                }
            }
        }       
    }	
}
int main()
{
    Server server1;
    server1.start_server();
}