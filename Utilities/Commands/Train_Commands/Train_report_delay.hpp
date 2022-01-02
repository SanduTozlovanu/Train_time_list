#include "../Command.hpp"
class Train_report_delay : public Command
{
    protected:
    std::string train_id;
    int delay_time;
    public:
    Train_report_delay(std::string id,int delay_time)
    {
        this->train_id=id;
        this->delay_time=delay_time;
    }
    const char* execute()
    {
        pugi::xml_document doc;
        if (!doc.load_file(FILTERED_XML_DOC))
        {
            return PROCESS_ERROR;
        }
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
                            int_OraP+=this->delay_time*60;
                            element_trasa.attribute("OraP").set_value(Instruments::string_to_char(std::to_string(int_OraP)));
                        }
                        element_trasa.attribute("Ajustari").set_value(Instruments::string_to_char(std::to_string(element_trasa.attribute("Ajustari").as_int()+this->delay_time)));
                        int int_OraS=std::stoi(element_trasa.attribute("OraS").value());
                        std::cout<<int_OraS<<std::endl;
                        int_OraS+=this->delay_time*60;
                        std::cout<<int_OraS<<std::endl;
                        element_trasa.attribute("OraS").set_value(Instruments::string_to_char(std::to_string(int_OraS)));
                    }
                }
            }
        }
        bool saveSucceeded = doc.save_file(FILTERED_XML_DOC);
        return CHANGE_SUCCESSFUL;   
    }
};