#include "../Command.hpp"
class Train_verify : public Command
{
    protected:
    std::string train_id;
    public:
    Train_verify(std::string id)
    {
        this->train_id=id;
    }
    const char * execute()
    {
        pugi::xml_document doc;
        if (!doc.load_file(OFFICIAL_XML_DOC))
        {
            return PROCESS_ERROR;
        }
        for (pugi::xml_node tren = doc.first_child().first_child().first_child().first_child().first_child(); tren; tren = tren.next_sibling())
        {
            if(tren.attribute("Numar").value()==train_id)
            {
                return VERIFIED;   
            }
        }
        return WRONG_ID; 
    }
};