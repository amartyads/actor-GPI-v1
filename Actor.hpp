#include "../local/include/GASPI.h"
#include "../local/include/GASPI_Ext.h"
#include "gpi-utils.hpp"
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