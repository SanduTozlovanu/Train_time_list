#pragma once
#include "Settings.hpp"
#include "Instruments.hpp"
#define FILTERED_XML_DOC "filtered.xml"
class Message_Processer
{
public:
    static char* process_message(char* initial_message)
    {
        if(strncmp(initial_message,"station:",8)==0)
        {
            std::string semi_filtered_msg;
            semi_filtered_msg+=initial_message;
            semi_filtered_msg.erase(0,8);
            char* temporal=Instruments::string_to_char(semi_filtered_msg);
            char char_semi_filtered_msg[RECEIVE_LIMIT];
            strcpy(char_semi_filtered_msg,temporal);
            std::cout<<char_semi_filtered_msg<<std::endl;
            delete temporal;
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
                if(char_semi_filtered_msg[i]==':')
                {
                    station_id_length=i;
                    break;
                }
                station_id+=char_semi_filtered_msg[i];
            }
            std::string filtered_msg;
            filtered_msg+=char_semi_filtered_msg;
            filtered_msg.erase(0,station_id_length+1);
            char* temp=Instruments::string_to_char(filtered_msg);
            char filtered_receive_msg[RECEIVE_LIMIT];
            strcpy(filtered_receive_msg,temp);
            delete temp;
            std::string send_msg;
            if(strcmp(filtered_receive_msg,"get_my_trains\n")==0)
            {
                pugi::xml_document doc;
                std::string file_name=FILTERED_XML_DOC;
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
                std::string file_name=FILTERED_XML_DOC;
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
        else if (strncmp(initial_message,"train:",6)==0)
        {
            std::string semi_filtered_msg;
            semi_filtered_msg+=initial_message;
            semi_filtered_msg.erase(0,6);
            char* temporal=Instruments::string_to_char(semi_filtered_msg);
            char char_semi_filtered_msg[RECEIVE_LIMIT];
            strcpy(char_semi_filtered_msg,temporal);
            std::cout<<char_semi_filtered_msg<<std::endl;
            delete temporal;
            std::string train_id;
            int train_id_length;
            for(int i=0;i<10;i++)
            {
                if(i==9)
                {
                    std::string send_msg="Protocol error ...\n";
                    char* char_send_msg2=Instruments::string_to_char(send_msg);
                    return char_send_msg2;
                }
                if(char_semi_filtered_msg[i]==':')
                {
                    train_id_length=i;
                    break;
                }
                train_id+=char_semi_filtered_msg[i];
            }
            std::string filtered_msg;
            filtered_msg+=char_semi_filtered_msg;
            filtered_msg.erase(0,train_id_length+1);
            char* temp=Instruments::string_to_char(filtered_msg);
            char filtered_receive_msg[RECEIVE_LIMIT];
            strcpy(filtered_receive_msg,temp);
            delete temp;
            std::string send_msg;
            if(strcmp(filtered_receive_msg,"exit\n")==0)
            {
                send_msg=MSG_EXIT;
                char* char_send_msg2=Instruments::string_to_char(send_msg);
                return char_send_msg2;
            }
            else if(strcmp(filtered_receive_msg,"get_my_stations\n")==0)
            {
                pugi::xml_document doc;
                std::string file_name=FILTERED_XML_DOC;
                char* char_file_name=Instruments::string_to_char(file_name);
                if (!doc.load_file(char_file_name))
                {
                    std::string error_msg;
                    error_msg+=PROCESS_ERROR;
                    char * error = Instruments::string_to_char(error_msg);
                    return error;
                }
                delete char_file_name;
                std::string first_station_name;
                std::string second_station_name;
                for (pugi::xml_node tren = doc.child("Trenuri").first_child(); tren; tren = tren.next_sibling())
                {
                    if(tren.attribute("Numar").value()==train_id)
                    {
                        for(pugi::xml_node elementtrasa = tren.child("Trase").child("Trasa").child("ElementTrasa");elementtrasa;elementtrasa=elementtrasa.next_sibling())
                        {
                            first_station_name.clear();
                            second_station_name.clear();
                            first_station_name+=elementtrasa.attribute("DenStaOrigine").value();
                            second_station_name+=elementtrasa.attribute("DenStaDestinatie").value();
                            if(first_station_name.compare(second_station_name)==0)
                            {
                                break;
                            }
                            send_msg+=elementtrasa.attribute("DenStaOrigine").value();
                            send_msg+=" going to ";
                            send_msg+=elementtrasa.attribute("DenStaDestinatie").value();
                            send_msg+='\n';
                        }
                    }
                }
                char* char_send_msg2=Instruments::string_to_char(send_msg);
                return char_send_msg2;
            }
            else
            {
                send_msg+=COMMAND_NOT_FOUND;
                /* returnam mesajul clientului */
                char* char_send_msg=Instruments::string_to_char(send_msg);
                return char_send_msg;    
            }
        }
    }
};