#pragma once
#include "Settings.hpp"
#include "Instruments.hpp"
#define FILTERED_XML_DOC "filtered.xml"
class Message_Processer
{
    private:
    static char* protocol_error_process_message(char* initial_message)
    {
        std::string send_msg=MSG_PROTOCOL_ERROR;
        char* char_send_msg2=Instruments::string_to_char(send_msg);
        return char_send_msg2;
    }
    static char* train_process_message(char* initial_message,int wc)
    {
        std::string semi_filtered_msg;
            semi_filtered_msg+=initial_message;
            semi_filtered_msg.erase(0,6);
            char* temporal=Instruments::string_to_char(semi_filtered_msg);
            char char_semi_filtered_msg[wc];
            strcpy(char_semi_filtered_msg,temporal);
            std::cout<<char_semi_filtered_msg<<std::endl;
            delete temporal;
            std::string train_id;
            int train_id_length;
            for(int i=0;i<10;i++)
            {
                if(i==9)
                {
                    std::string send_msg=MSG_PROTOCOL_ERROR;
                    char* char_send_msg2=Instruments::string_to_char(send_msg);
                    return char_send_msg2;
                }
                if(char_semi_filtered_msg[i]==':')
                {
                    train_id_length=i;
                    if(train_id_length==0)
                    {
                        return protocol_error_process_message(initial_message);
                    }
                    break;
                }
                train_id+=char_semi_filtered_msg[i];
            }
            std::string filtered_msg;
            filtered_msg+=char_semi_filtered_msg;
            filtered_msg.erase(0,train_id_length+1);
            char* temp=Instruments::string_to_char(filtered_msg);
            char filtered_receive_msg[wc];
            strcpy(filtered_receive_msg,temp);
            delete temp;
            std::string send_msg;
            if(strcmp(filtered_receive_msg,"get_current_time\n")==0)
            {
                send_msg+=Instruments::get_current_time();
                char* char_send_msg2=Instruments::string_to_char(send_msg);
                return char_send_msg2;
            }
            else if(strcmp(filtered_receive_msg,"exit\n")==0)
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
                std::string temp;
                for (pugi::xml_node tren = doc.child("Trenuri").first_child(); tren; tren = tren.next_sibling())
                {
                    if(tren.attribute("Numar").value()==train_id)
                    {
                        for(pugi::xml_node elementtrasa = tren.child("Trase").child("Trasa").child("ElementTrasa");elementtrasa;elementtrasa=elementtrasa.next_sibling())
                        {
                            if(elementtrasa.attribute("DenStaOrigine").as_int()==elementtrasa.attribute("CodStaDest").as_int())
                            {
                                break;
                            }
                            send_msg+="Arriving at ";
                            temp.clear();
                            temp+=elementtrasa.attribute("OraP").value();
                            send_msg+=Instruments::time_transform(temp);
                            send_msg+=" in ";
                            send_msg+=elementtrasa.attribute("DenStaOrigine").value();
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
    static char* station_process_message(char* initial_message,int wc)
    {
        std::string semi_filtered_msg;
            semi_filtered_msg+=initial_message;
            semi_filtered_msg.erase(0,8);
            char* temporal=Instruments::string_to_char(semi_filtered_msg);
            char char_semi_filtered_msg[wc];
            strcpy(char_semi_filtered_msg,temporal);
            std::cout<<char_semi_filtered_msg<<std::endl;
            delete temporal;
            std::string station_id;
            int station_id_length;
            for(int i=0;i<10;i++)
            {
                if(i==9)
                {
                    std::string send_msg=MSG_PROTOCOL_ERROR;
                    char* char_send_msg2=Instruments::string_to_char(send_msg);
                    return char_send_msg2;
                }
                if(char_semi_filtered_msg[i]==':')
                {
                    station_id_length=i;
                    if (station_id_length==0)
                    {
                        return protocol_error_process_message(initial_message);
                    }
                    break;
                }
                station_id+=char_semi_filtered_msg[i];
            }
            std::string filtered_msg;
            filtered_msg+=char_semi_filtered_msg;
            filtered_msg.erase(0,station_id_length+1);
            char* temp=Instruments::string_to_char(filtered_msg);
            char filtered_receive_msg[wc];
            strcpy(filtered_receive_msg,temp);
            delete temp;
            std::string send_msg;
            if(strcmp(filtered_receive_msg,"get_my_next_trains\n")==0)
            {
                pugi::xml_document doc;
                std::string file_name=FILTERED_XML_DOC;
                char* char_file_name=Instruments::string_to_char(file_name);
                std::string time_est;
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
                        if((element_trasa.attribute("CodStaDest").value()==station_id) && (Instruments::s_time_to_int_time(Instruments::time_transform(element_trasa.attribute("OraS").value())))>(Instruments::s_time_to_int_time(Instruments::get_current_time())))
                        {
                            time_est=Instruments::time_estimation(Instruments::get_current_time(),Instruments::time_transform(element_trasa.attribute("OraS").value()));
                            flag=1;
                        }
                    }
                    if(flag==1)
                    {
                        send_msg+=tren.first_child().first_child().first_child().attribute("DenStaOrigine").value();
                        send_msg+='-';
                        send_msg+=tren.first_child().first_child().last_child().previous_sibling().attribute("DenStaOrigine").value();
                        send_msg+=" coming in ";
                        send_msg+=time_est;
                        send_msg+='\n';
                    }
                }
                char* char_send_msg2=Instruments::string_to_char(send_msg);
                return char_send_msg2;    
            }
            else if (strcmp(filtered_receive_msg,"get_my_trains\n")==0)
            {
                pugi::xml_document doc;
                std::string file_name=FILTERED_XML_DOC;
                char* char_file_name=Instruments::string_to_char(file_name);
                std::string s_time;
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
                        if(element_trasa.attribute("CodStaDest").value()==station_id)
                        {
                            s_time.clear();
                            flag=1;
                            s_time+=element_trasa.attribute("OraS").value();
                        }
                    }
                    if(flag==1)
                    {
                        send_msg+=tren.first_child().first_child().first_child().attribute("DenStaOrigine").value();
                        send_msg+='-';
                        send_msg+=tren.first_child().first_child().last_child().previous_sibling().attribute("DenStaOrigine").value();
                        send_msg+=" coming at ";
                        send_msg+=Instruments::time_transform(s_time);
                        send_msg+='\n';
                    }
                }
                char* char_send_msg2=Instruments::string_to_char(send_msg);
                return char_send_msg2;
            }
            else if(strcmp(filtered_receive_msg,"get_current_time\n")==0)
            {
                send_msg+=Instruments::get_current_time();
                char* char_send_msg2=Instruments::string_to_char(send_msg);
                return char_send_msg2;
            }
            else if(strcmp(filtered_receive_msg,"exit\n")==0)
            {
                send_msg=MSG_EXIT;
                char* char_send_msg2=Instruments::string_to_char(send_msg);
                return char_send_msg2;
            }
            else if(strcmp(filtered_receive_msg,"get_station_name\n")==0)
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
                    for (pugi::xml_node element_trasa = trasa.first_child(); element_trasa; element_trasa = element_trasa.next_sibling())
                    {
                        if(element_trasa.attribute("CodStaOrigine").value()==station_id)
                        {
                            send_msg+=element_trasa.attribute("DenStaOrigine").value();
                            char* char_send_msg2=Instruments::string_to_char(send_msg);
                            return char_send_msg2;
                        }
                    }
                }
                send_msg+=STATION_NOT_FOUND;
                char* char_send_msg2=Instruments::string_to_char(send_msg);
                return char_send_msg2;
            }
            else send_msg+=COMMAND_NOT_FOUND;
            /* returnam mesajul clientului */
            char* char_send_msg=Instruments::string_to_char(send_msg);
            return char_send_msg;
    }  
public:
    static char* process_message(char* initial_message,int wc)
    {
        if(strncmp(initial_message,"station:",8)==0)
        {
            return station_process_message(initial_message,wc);
        }
        else if (strncmp(initial_message,"train:",6)==0)
        {
            return train_process_message(initial_message,wc);
        }
        else
        {
            return protocol_error_process_message(initial_message);
        }
    }
};