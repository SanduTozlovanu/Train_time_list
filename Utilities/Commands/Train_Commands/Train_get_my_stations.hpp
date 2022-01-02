#include "../Command.hpp"
class Train_get_my_stations : public Command
{
    protected:
    std::string train_id;
    public:
    Train_get_my_stations(std::string id)
    {
        this->train_id=id;
    }
    const char* execute()
    {
        std::string send_msg;
        pugi::xml_document doc;
        if (!doc.load_file(FILTERED_XML_DOC))
        {
            return PROCESS_ERROR;
        }
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
};