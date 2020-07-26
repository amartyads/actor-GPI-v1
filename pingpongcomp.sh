#!/bin/bash

g++ -L/home/amartya/local/lib64 LocalChannel.cpp RemoteChannel.cpp InPort.cpp OutPort.cpp Actor.cpp PingPongActor.cpp ActorGraph.cpp mainpingpong.cpp -lGPI2 -pthread -o main
