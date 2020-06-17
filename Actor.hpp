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
	Actor(std::string othname, int othsrno, int othrank)
	{
		name = othname;
		rank = othrank;
		srno = othsrno;
		globID = (rank << 10) | srno;
	}
	Actor(int srno, int rank) : Actor("A-"+std::to_string(rank)+"-"+std::to_string(srno), srno, rank) { }

	static int encodeGlobID(int actNo, int procNo);
	static std::pair<int,int> decodeGlobID(int inpGlobId);
};