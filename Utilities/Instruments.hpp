#pragma once
#include "Settings.hpp"
#define FILTERED_XML_DOC "filtered.xml"
class Instruments
{
    private:
    static char* get_random_dayset()
    {
        std::string dayset;
        int random_number;
        for(int i=0;i<7;i++)
        {
            random_number=rand() % 2;
            if(random_number==0) dayset+='0';
            else dayset+='1';
        }
        char* char_dayset=string_to_char(dayset);
        return char_dayset;
    }
    public:
    static char* string_to_char(std::string str)
    {
        char *char_str = new char[str.length() + 1];
        strcpy(char_str,str.c_str());
        char_str[str.length()] = '\0';
        return char_str;
    }
    static std::string get_current_time()
    {
        time_t today=time(0);
        struct tm * time;
        time=localtime(&today);
        mktime(time);
        std::string current_time;
        current_time+=std::to_string(time->tm_hour);
        current_time+=':';
        current_time+=std::to_string(time->tm_min);
        current_time+=':';
        current_time+=std::to_string(time->tm_sec);
        return current_time;
    }
    static int what_date_is_today()
    {
        time_t today=time(0);
        struct tm * time;
        time=localtime(&today);
        mktime(time);
        int year=time->tm_year+1900,month=time->tm_mon+1,day=time->tm_mday;
        int int_date=(((year*100)+month)*100)+day;
        return int_date;
    }
    static int day_of_week()
    {
        time_t today=time(0);
        struct tm * time;
        time=localtime(&today);
        mktime(time);
        int week_day=time->tm_wday;
        return week_day;
    }
    static std::string time_transform(std::string time)
    {   
        std::cout<<"am ajuns aici1"<<time<<std::endl;
        int seconds = std::stoi(time);
        std::cout<<"am ajuns aici2"<<std::endl;
        seconds=seconds/60;
        int mins=0;
        int hours=0;
        do
        {
            if(mins==60)
            {
                hours++;
                mins=0;
            }
            seconds=seconds-60;
            mins++;

        } while (seconds>60);
        std::string str_time;
        str_time+=std::to_string(hours);
        str_time+=':';
        str_time+=std::to_string(mins);
        str_time+=':';
        str_time+=std::to_string(seconds);
        return str_time;     
    }
    static pugi::xml_document filter_xml (std::string official_name)
    {
        srand (time(NULL));
        std::cout<<"We start filtering the xml"<<std::endl;
        pugi::xml_document filtered_doc;
        auto declarationNode = filtered_doc.append_child(pugi::node_declaration);
        declarationNode.append_attribute("version")= "1.0";
        declarationNode.append_attribute("encoding")= "UTF-8";
        auto root = filtered_doc.append_child("Trenuri");
        pugi::xml_document doc;
        char* official_char_name=string_to_char(official_name);
        pugi::xml_parse_result result = doc.load_file(official_char_name);
        delete official_char_name;
        if (!result)
        {
            std::cout << "Parse error: " << result.description()
                << ", character pos= " << result.offset;
        }
        pugi::xml_node trains=doc.first_child().first_child().first_child().first_child();

        for(pugi::xml_node train = trains.first_child();train;train=train.next_sibling())
        {
            int flag=0;
            pugi::xml_node restrictii_tren=train.child("RestrictiiTren");
            for(pugi::xml_node calendar_tren=restrictii_tren.first_child();calendar_tren;calendar_tren=calendar_tren.next_sibling())
            {
                calendar_tren.attribute("Zile").set_value(Instruments::get_random_dayset());
                std::string week_day=calendar_tren.attribute("Zile").value();
                if((calendar_tren.attribute("DeLa").as_int()<=what_date_is_today())&&(calendar_tren.attribute("PinaLa").as_int()>=what_date_is_today())&&(week_day[Instruments::day_of_week()]=='1'))
                {
                    flag=1;
                }
            }
            if(flag==1)
            {
                filtered_doc.child("Trenuri").append_copy(train);
            }
        }
        bool saveSucceeded = filtered_doc.save_file(FILTERED_XML_DOC);
        return filtered_doc;
    }
};