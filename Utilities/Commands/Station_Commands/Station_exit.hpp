#include "../Command.hpp"
class Station_exit : public Command
{
    protected:
    std::string station_id;
    public:
    Station_exit(std::string id)
    {
        this->station_id=id;
    }
    const char* execute()
    {
        return MSG_EXIT;
    }
};