#include "Actor.hpp"
#include <string>

class SqrtSourceActor: public Actor
{
public:
    void act();
    int numDigsSent, numDigsToSend;
    SqrtSourceActor(std::string name, uint64_t rank, uint64_t srno, int numDigsToSend);
}