#include "Actor.hpp"

class PingPongActor: public Actor
{
public:
    void act();
    bool finished();
    PingPongActor(uint64_t rank, uint64_t srno);
};