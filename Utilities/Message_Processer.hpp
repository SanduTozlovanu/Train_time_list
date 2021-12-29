#pragma once
#include "Settings.hpp"
#include "Instruments.hpp"
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
                std::cout<<"am intrat aici"<<std::endl;
                send_msg=MSG_EXIT;
                char* char_send_msg2=Instruments::string_to_char(send_msg);
                return char_send_msg2;
            }
            else if (strncmp(filtered_receive_msg,"report_sooner ",14)==0)
            {
                std::string sooner_time;
                sooner_time+=filtered_receive_msg;
                sooner_time.erase(0,14);
                int int_sooner_time=std::stoi(sooner_time);
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
                int OraS;
                int current_time;
                for (pugi::xml_node tren = doc.child("Trenuri").first_child(); tren; tren = tren.next_sibling())
                {
                    if(tren.attribute("Numar").value()==train_id)
                    {
                        int flag=0;
                        for(pugi::xml_node element_trasa = tren.child("Trase").child("Trasa").child("ElementTrasa");element_trasa;element_trasa=element_trasa.next_sibling())
                        {   
                            if(element_trasa.attribute("DenStaOrigine").as_int()==element_trasa.attribute("CodStaDest").as_int())
                            {
                               break;
                            }
                            OraS=Instruments::s_time_to_int_time(Instruments::time_transform(element_trasa.attribute("OraS").value()));
                            current_time=Instruments::s_time_to_int_time(Instruments::get_current_time());
                            if(OraS>current_time)
                            {
                                std::cout<<"Modificam acum : "<<element_trasa.attribute("OraS").value()<<std::endl;
                                if(flag=0)
                                {
                                    flag=1;
                                }
                                else
                                {
                                    int int_OraP=std::stoi(element_trasa.attribute("OraP").value());
                                    int_OraP-=int_sooner_time*60;
                                    element_trasa.attribute("OraP").set_value(Instruments::string_to_char(std::to_string(int_OraP)));
                                }
                                element_trasa.attribute("Ajustari").set_value(Instruments::string_to_char(std::to_string(element_trasa.attribute("Ajustari").as_int()-int_sooner_time)));
                                int int_OraS=std::stoi(element_trasa.attribute("OraS").value());
                                std::cout<<int_OraS<<std::endl;
                                int_OraS-=int_sooner_time*60;
                                std::cout<<int_OraS<<std::endl;
                                element_trasa.attribute("OraS").set_value(Instruments::string_to_char(std::to_string(int_OraS)));
                            }
                        }
                    }
                }
                bool saveSucceeded = doc.save_file(FILTERED_XML_DOC);
                send_msg=CHANGE_SUCCESSFUL;
                char* char_send_msg2=Instruments::string_to_char(send_msg);
                return char_send_msg2;
            }
            else if (strncmp(filtered_receive_msg,"report_delay ",13)==0)
            {
                std::string delay_time;
                delay_time+=filtered_receive_msg;
                delay_time.erase(0,13);
                int int_delay_time=std::stoi(delay_time);
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
                int OraS;
                int current_time;
                for (pugi::xml_node tren = doc.child("Trenuri").first_child(); tren; tren = tren.next_sibling())
                {
                    if(tren.attribute("Numar").value()==train_id)
                    {
                        int flag=0;
                        for(pugi::xml_node element_trasa = tren.child("Trase").child("Trasa").child("ElementTrasa");element_trasa;element_trasa=element_trasa.next_sibling())
                        {   
                            if(element_trasa.attribute("DenStaOrigine").as_int()==element_trasa.attribute("CodStaDest").as_int())
                            {
                               break;
                            }
                            OraS=Instruments::s_time_to_int_time(Instruments::time_transform(element_trasa.attribute("OraS").value()));
                            current_time=Instruments::s_time_to_int_time(Instruments::get_current_time());
                            if(OraS>current_time)
                            {
                                if(flag=0)
                                {
                                    flag=1;
                                }
                                else
                                {
                                    int int_OraP=std::stoi(element_trasa.attribute("OraP").value());
                                    int_OraP+=int_delay_time*60;
                                    element_trasa.attribute("OraP").set_value(Instruments::string_to_char(std::to_string(int_OraP)));
                                }
                                element_trasa.attribute("Ajustari").set_value(Instruments::string_to_char(std::to_string(element_trasa.attribute("Ajustari").as_int()+int_delay_time)));
                                int int_OraS=std::stoi(element_trasa.attribute("OraS").value());
                                std::cout<<int_OraS<<std::endl;
                                int_OraS+=int_delay_time*60;
                                std::cout<<int_OraS<<std::endl;
                                element_trasa.attribute("OraS").set_value(Instruments::string_to_char(std::to_string(int_OraS)));
                            }
                        }
                    }
                }
                bool saveSucceeded = doc.save_file(FILTERED_XML_DOC);
                send_msg=CHANGE_SUCCESSFUL;
                char* char_send_msg2=Instruments::string_to_char(send_msg);
                return char_send_msg2;
            }
            else if(strcmp(filtered_receive_msg,"verify\0")==0)
            {
                pugi::xml_document doc;
                std::string file_name=OFFICIAL_XML_DOC;
                char* char_file_name=Instruments::string_to_char(file_name);
                if (!doc.load_file(char_file_name))
                {
                    std::string error_msg;
                    error_msg+=PROCESS_ERROR;
                    char * error = Instruments::string_to_char(error_msg);
                    return error;
                }
                delete char_file_name;
                for (pugi::xml_node tren = doc.first_child().first_child().first_child().first_child().first_child(); tren; tren = tren.next_sibling())
                {
                    if(tren.attribute("Numar").value()==train_id)
                    {
                        send_msg+=VERIFIED;
                        char* char_send_msg=Instruments::string_to_char(send_msg);
                        return char_send_msg;    
                    }
                }
                send_msg+=WRONG_ID;
                char* char_send_msg=Instruments::string_to_char(send_msg);
                return char_send_msg;    
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
            if((strcmp(filtered_receive_msg,"get_my_next_trains\n")==0)||(strcmp(filtered_receive_msg,"get_my_trains_update\n")==0))
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
                    int delay=0,flag=0,sooner=0;
                    pugi::xml_node trasa=tren.child("Trase").child("Trasa");
                    if((trasa.attribute("CodStatieFinala").value()==station_id)&&(Instruments::s_time_to_int_time(Instruments::time_transform(trasa.last_child().previous_sibling().attribute("OraS").value()))>(Instruments::s_time_to_int_time(Instruments::get_current_time()))))
                    {
                        flag=1;
                        time_est=Instruments::time_estimation(Instruments::get_current_time(),Instruments::time_transform(trasa.last_child().previous_sibling().attribute("OraS").value()));
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
                            if(element_trasa.attribute("Ajustari").as_int()>0)
                            {
                                delay=1;
                            }
                            if(element_trasa.attribute("Ajustari").as_int()<0)
                            {
                                sooner=1;
                            }
                        }
                    }
                    if(flag==1)
                    {
                    
                        send_msg+='[';send_msg+=tren.attribute("CategorieTren").value();send_msg+=']';
                        send_msg+="[";send_msg+=tren.attribute("Numar").value();send_msg+=']';
                        send_msg+=tren.first_child().first_child().first_child().attribute("DenStaOrigine").value();
                        send_msg+='-';
                        send_msg+=tren.first_child().first_child().last_child().previous_sibling().attribute("DenStaOrigine").value();
                        send_msg+=" coming in ";
                        send_msg+=time_est;
                        if(delay) {send_msg+=" (DELAYED)";}
                        if(sooner) {send_msg+=" (SOONER)";}
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
                        if(element_trasa.attribute("DenStaOrigine").as_int()==element_trasa.attribute("CodStaDest").as_int())
                        {
                            flag=0;
                            break;
                        }
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
                    if(flag==1 && s_time.length()!=0)
                    {
                        send_msg+='[';send_msg+=tren.attribute("CategorieTren").value();send_msg+=']';
                        send_msg+="[";send_msg+=tren.attribute("Numar").value();send_msg+=']';
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
            else if(strcmp(filtered_receive_msg,"verify\0")==0)
            {
                pugi::xml_document doc;
                std::string file_name=OFFICIAL_XML_DOC;
                char* char_file_name=Instruments::string_to_char(file_name);
                if (!doc.load_file(char_file_name))
                {
                    std::string error_msg;
                    error_msg+=PROCESS_ERROR;
                    char * error = Instruments::string_to_char(error_msg);
                    return error;
                }
                delete char_file_name;
                for (pugi::xml_node tren = doc.first_child().first_child().first_child().first_child().first_child(); tren; tren = tren.next_sibling())
                {
                    for (pugi::xml_node element_trasa =tren.first_child().first_child().first_child();element_trasa;element_trasa=element_trasa.next_sibling())
                    {
                        if(element_trasa.attribute("CodStaOrigine").value()==station_id)
                        {
                            send_msg+=VERIFIED;
                            char* char_send_msg=Instruments::string_to_char(send_msg);
                            return char_send_msg;    
                        }
                    }
                }
                send_msg+=WRONG_ID;
                char* char_send_msg=Instruments::string_to_char(send_msg);
                return char_send_msg;    
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