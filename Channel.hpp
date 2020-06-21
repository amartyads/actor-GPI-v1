#include "connection-type-util.hpp"
class Channel
{
public:
    ActorConnectionType currConnectionType;

    virtual void pushData();
    virtual void pullData();
};