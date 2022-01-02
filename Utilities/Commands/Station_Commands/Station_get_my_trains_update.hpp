#include "../Command.hpp"
class Station_get_my_trains_update : public Command
{
    protected:
    std::string station_id;
    public:
    Station_get_my_trains_update(std::string id)
    {
        this->station_id=id;
    }
    const char* execute()
    {
        std::string send_msg;
        pugi::xml_document doc;
        std::string time_est;
        if (!doc.load_file(FILTERED_XML_DOC))
        {
            return PROCESS_ERROR;
        }
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
        const char* char_send_msg2=Instruments::string_to_char(send_msg);
        return char_send_msg2;    
    }
};