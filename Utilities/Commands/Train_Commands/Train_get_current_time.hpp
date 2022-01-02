#include "../Command.hpp"
class Train_get_current_time : public Command
{
    protected:
    std::string train_id;
    public:
    Train_get_current_time(std::string id)
    {
        this->train_id=id;
    }
    const char* execute()
    {
        return Instruments::string_to_char(Instruments::get_current_time());
    }
};