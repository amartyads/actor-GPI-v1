#include "Actor.hpp"
#include "Channel.hpp"
class InPort
{
public:
    Actor *parentActor;
    Channel *connChannel;
    double** data;
};