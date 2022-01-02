#include "../Command.hpp"
class Station_command_not_found : public Command
{
    protected:
    std::string station_id;
    public:
    Station_command_not_found(std::string id)
    {
        this->station_id=id;
    }
    const char* execute()
    {
        return COMMAND_NOT_FOUND;
    }
};