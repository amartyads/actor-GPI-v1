#include <stdlib.h>

#include <string>

#pragma once

class Actor
{
public:
	std::string name;
	int rank;
	Actor(std::string othname, int othrank)
	{
		name = othname;
		rank = othrank;
	}
};