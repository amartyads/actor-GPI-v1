#include <stdlib.h>
#include <utility>

#include <string>

#pragma once

class Actor
{
public:
	std::string name;
	int rank;
	int srno;
	int globID;
	Actor(std::string othname, int othrank, int othsrno)
	{
		name = othname;
		rank = othrank;
		srno = othsrno;
		globID = (rank << 10) | srno;
	}
	Actor(int rank, int srno) : Actor("A-"+std::to_string(rank)+"-"+std::to_string(srno), rank, srno) { }

	static int encodeGlobID(int procNo, int actNo);
	static std::pair<int,int> decodeGlobID(int inpGlobId);
};