#include "Actor.hpp"
#include "Channel.hpp"
class OutPort
{
public:
    Actor *parentActor;
    Channel *connChannel;
    double** data;
};