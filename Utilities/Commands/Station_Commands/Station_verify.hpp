#include "../Command.hpp"
class Station_verify : public Command
{
    protected:
    std::string station_id;
    public:
    Station_verify(std::string id)
    {
        this->station_id=id;
    }
    const char* execute()
    {
        pugi::xml_document doc;
        if (!doc.load_file(OFFICIAL_XML_DOC))
        {
            return PROCESS_ERROR;
        }
        for (pugi::xml_node tren = doc.first_child().first_child().first_child().first_child().first_child(); tren; tren = tren.next_sibling())
        {
            for (pugi::xml_node element_trasa =tren.first_child().first_child().first_child();element_trasa;element_trasa=element_trasa.next_sibling())
            {
                if(element_trasa.attribute("CodStaOrigine").value()==station_id)
                {
                    return VERIFIED;  
                }
            }
        }
        return WRONG_ID;
    }
};