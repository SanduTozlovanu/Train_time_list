#include "../Utilities/Settings.hpp"
#include "../Utilities/Message_Processer.hpp"
#include "../Utilities/Instruments.hpp"
#define OFFICIAL_XML_DOC "../Utilities/sntfc-cfr-cltori-s.a.-1232-trenuri_2021.xml"
#include "Server.hpp"
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
                int wc1,wc2;
                bzero (receive_msg, RECEIVE_LIMIT);
                printf ("[server]Waiting for message...\n");
                fflush (stdout);
                if (read (client, &wc1, sizeof(int)) <= 0)
                {
                    perror ("[server]Error at read() from the client.\n");
                    close (client);	/* inchidem conexiunea cu clientul */
                    goto cnt;
                }
                if (read (client, receive_msg, sizeof(char)*wc1+1) <= 0)
                {
                    perror ("[server]Error at read() from the client.\n");
                    close (client);	/* inchidem conexiunea cu clientul */
                    goto cnt;
                }
                    /* continuam sa ascultam */
                char* send_message=Message_Processer::process_message(receive_msg,wc1);
                if ((strcmp(send_message,MSG_EXIT)==0)||(strcmp(send_message,MSG_PROTOCOL_ERROR)==0)||(strcmp(send_message,PROCESS_ERROR)==0))
                {
                    wc2=strlen(send_message);
                    if (write (client,&wc2, sizeof(int)) <= 0)
                    {
                        perror ("[server]Error at write() towards client.\n");
                        close (client);
                        goto cnt;
                    }
                    else if (write (client,send_message, sizeof(char)*wc2+1) <= 0)
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
                    wc2=strlen(send_message);
                    if (write (client,&wc2, sizeof(int)) <= 0)
                    {
                        perror ("[server]Error at write() towards client.\n");
                        close (client);
                        goto cnt;
                    }
                    else if (write (client,send_message, sizeof(char)*wc2+1) <= 0)
                    {
                        perror ("[server]Error at write() towards client.\n");
                        close (client);
                        goto cnt;
                    }
                    else
                    {
                    delete send_message;
                    printf ("[server]The message was successfully sent.\n");
                    }
                        /* am terminat cu acest client, inchidem conexiunea */
                }
            }
        }       
    }	
}
int main()
{
    Server Main_Server;
    Main_Server.start_server();
}