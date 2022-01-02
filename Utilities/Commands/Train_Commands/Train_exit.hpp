#include "../Command.hpp"
class Train_exit : public Command
{
    protected:
    std::string train_id;
    public:
    Train_exit(std::string id)
    {
        this->train_id=id;
    }
    const char* execute()
    {
        return MSG_EXIT;
    }
};