#include "../Command.hpp"
class Train_command_not_found : public Command
{
    protected:
    std::string train_id;
    public:
    Train_command_not_found(std::string id)
    {
        this->train_id=id;
    }
    const char* execute()
    {
        return COMMAND_NOT_FOUND;   
    }
};