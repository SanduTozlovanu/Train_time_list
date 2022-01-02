#include "../Command.hpp"
class Station_get_station_name : public Command
{
    protected:
    std::string station_id;
    public:
    Station_get_station_name(std::string id)
    {
        this->station_id=id;
    }
    const char* execute()
    {
        std::string send_msg;
        pugi::xml_document doc;
        if (!doc.load_file(FILTERED_XML_DOC))
        {
            return PROCESS_ERROR;
        }
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
        return STATION_NOT_FOUND;
    }
};          