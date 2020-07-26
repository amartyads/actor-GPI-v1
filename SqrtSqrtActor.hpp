#include "Actor.hpp"
#include <string>
#include <vector>

class SqrtSqrtActor: public Actor
{
public:
    void act();
    int numDigsSent, numDigsToSend;
    bool writeSuccess;
    std::vector<double> inData;
    SqrtSqrtActor(std::string name, uint64_t rank, uint64_t srno, int numDigsToSend);
};