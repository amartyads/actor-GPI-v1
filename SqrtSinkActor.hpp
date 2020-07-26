#include "Actor.hpp"
#include <string>

class SqrtSinkActor: public Actor
{
public:
    void act();
    int numDigsGot, numDigsToGet;
    SqrtSinkActor(std::string name, uint64_t rank, uint64_t srno, int numDigsToGet);
};