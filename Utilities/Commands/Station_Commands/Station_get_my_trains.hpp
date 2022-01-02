#include "../Command.hpp"
class Station_get_my_trains : public Command
{
    protected:
    std::string station_id;
    public:
    Station_get_my_trains(std::string id)
    {
        this->station_id=id;
    }
    const char* execute()
    {
        pugi::xml_document doc;
        std::string send_msg;
        std::string s_time;
        if (!doc.load_file(FILTERED_XML_DOC))
        {
            return PROCESS_ERROR;
        }
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
};