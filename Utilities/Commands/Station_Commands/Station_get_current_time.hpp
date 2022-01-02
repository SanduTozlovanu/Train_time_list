#include "../Command.hpp"
class Station_get_current_time : public Command
{
    protected:
    std::string station_id;
    public:
    Station_get_current_time(std::string id)
    {
        this->station_id=id;
    }
    const char* execute()
    {
        return Instruments::string_to_char(Instruments::get_current_time());
    }
};