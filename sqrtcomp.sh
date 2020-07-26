#!/bin/bash

g++ -L/home/amartya/local/lib64 LocalChannel.cpp RemoteChannel.cpp InPort.cpp OutPort.cpp Actor.cpp SqrtSourceActor.cpp SqrtSqrtActor.cpp SqrtSinkActor.cpp ActorGraph.cpp mainsqrt.cpp -lGPI2 -pthread -o main
