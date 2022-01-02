#include "Commands/Station_Commands/Station_get_current_time.hpp"
#include "Commands/Train_Commands/Train_get_current_time.hpp"
#include "Commands/Train_Commands/Train_exit.hpp"
#include "Commands/Train_Commands/Train_report_sooner.hpp"
#include "Commands/Train_Commands/Train_report_delay.hpp"
#include "Commands/Train_Commands/Train_verify.hpp"
#include "Commands/Train_Commands/Train_get_my_stations.hpp"
#include "Commands/Train_Commands/Train_command_not_found.hpp"
#include "Commands/Station_Commands/Station_get_my_trains_update.hpp"
#include "Commands/Station_Commands/Station_get_my_trains.hpp"
#include "Commands/Station_Commands/Station_exit.hpp"
#include "Commands/Station_Commands/Station_get_station_name.hpp"
#include "Commands/Station_Commands/Station_verify.hpp"
#include "Commands/Station_Commands/Station_command_not_found.hpp"
class Message_Processer
{
    private:
    static const char* train_process_message(char* initial_message, int wc)
    {
        Command* command;
        std::string semi_filtered_msg;
        semi_filtered_msg+=initial_message;
        semi_filtered_msg.erase(0,6);
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
            if(semi_filtered_msg[i]==':')
            {
                train_id_length=i;
                if(train_id_length==0)
                {
                    return MSG_PROTOCOL_ERROR;
                }
                break;
            }
            train_id+=semi_filtered_msg[i];
        }
        std::string filtered_msg;
        filtered_msg+=semi_filtered_msg;
        filtered_msg.erase(0,train_id_length+1);
        std::string send_msg;
        if(filtered_msg.compare("get_current_time\n")==0)
        {
            command=new Train_get_current_time(train_id);
        }
        else if(filtered_msg.compare("exit\n")==0)
        {
            command=new Train_exit(train_id); 
        }
        else if (strncmp(Instruments::string_to_char(filtered_msg),"report_sooner ",14)==0)
        {
            std::string sooner_time;
            sooner_time+=filtered_msg;
            sooner_time.erase(0,14);
            int int_sooner_time=std::stoi(sooner_time);
            command=new Train_report_sooner(train_id,int_sooner_time);
        }
        else if (strncmp(Instruments::string_to_char(filtered_msg),"report_delay ",13)==0)
        {
            std::string delay_time;
            delay_time+=filtered_msg;
            delay_time.erase(0,13);
            int int_delay_time=std::stoi(delay_time);
            command=new Train_report_delay(train_id,int_delay_time);
        }
        else if(filtered_msg.compare("verify\0")==0)
        {
            command=new Train_verify(train_id);
        }
        else if(filtered_msg.compare("get_my_stations\n")==0)
        {
            command=new Train_get_my_stations(train_id);
        }
        else
        {
            command=new Train_command_not_found(train_id);
        }
        return command->execute();
    }
    static const char* station_process_message(char* initial_message,int wc)
    {
        Command* command;
        std::string semi_filtered_msg;
        semi_filtered_msg+=initial_message;
        semi_filtered_msg.erase(0,8);
        std::string station_id;
        int station_id_length;
        for(int i=0;i<10;i++)
        {
            if(i==9)
            {
                return MSG_PROTOCOL_ERROR;
            }
            if(semi_filtered_msg[i]==':')
            {
                station_id_length=i;
                if (station_id_length==0)
                {
                    return MSG_PROTOCOL_ERROR;
                }
                break;
            }
            station_id+=semi_filtered_msg[i];
        }
        std::string filtered_msg;
        filtered_msg+=semi_filtered_msg;
        filtered_msg.erase(0,station_id_length+1);
        std::string send_msg;
        if((filtered_msg.compare("get_my_next_trains\n")==0)||(filtered_msg.compare("get_my_trains_update\n")==0))
        {
            command=new Station_get_my_trains_update(station_id);
        }
        else if (filtered_msg.compare("get_my_trains\n")==0)
        {
            command=new Station_get_my_trains(station_id);
        }
        else if(filtered_msg.compare("get_current_time\n")==0)
        {
            command=new Station_get_current_time(station_id);
        }
        else if(filtered_msg.compare("exit\n")==0)
        {
            command=new Station_exit(station_id);
        }
        else if(filtered_msg.compare("get_station_name\n")==0)
        {
            command=new Station_get_station_name(station_id);
        }
        else if(filtered_msg.compare("verify\0")==0)
        {
            command=new Station_verify(station_id);
        }
        else command=new Station_command_not_found(station_id);
        return command->execute();
    }
public:  
    static const char* process_message(char* initial_message,int wc)
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
            return MSG_PROTOCOL_ERROR;
        }
    }
};