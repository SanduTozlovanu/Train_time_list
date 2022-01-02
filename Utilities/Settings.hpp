#pragma once
#include <sys/types.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include "pugixml.hpp"
#define MAX_QUEUE_CLIENT 20
#define PORT_NUMBER 2024
#define IP_ADDRESS "127.0.0.1"
#define SEND_LIMIT 12000
#define RECEIVE_LIMIT 12000
#define CHANGE_SUCCESSFUL "The documented has been updated successfully\n"
#define MSG_EXIT "Quitting the client ...\n"
#define MSG_PROTOCOL_ERROR "Protocol error ...\n"
#define PROCESS_ERROR "Error...\n"
#define WRONG_ID "Entered ID is wrong ... \n"
#define VERIFIED "ID-Authentification success\n"
#define COMMAND_NOT_FOUND "Command not found...\n"
#define STATION_NOT_FOUND "Station couldn't be found...\n"
#define FILTERED_XML_DOC "filtered.xml"
#define OFFICIAL_XML_DOC "../Utilities/sntfc-cfr-cltori-s.a.-1232-trenuri_2021.xml"