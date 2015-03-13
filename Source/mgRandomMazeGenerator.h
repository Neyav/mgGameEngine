#ifndef MGRANDOMMAZEGENERATORH
#define MGRANDOMMAZEGENERATORH

#include <stdlib.h>

#include "mgMapDataHandler.h"

struct MapCreationIndex_s
{
	int BlockY;
	int BlockX;
	int Direction[4];
	int DirectionsValid;
};

// =------------------------------------=
// = mgRandomMazeGenerator C++ class    =
// =------------------------------------=
//
// Simply put, takes a mgMapDataHandler type and uses its functions to generate a completely random maze
// of 1x1 corridors. It starts wherever you call GenerateMaze on, and isn't guaranteed to make an exit anywhere
// nor make completely efficent use of the space allocated to it. Some handiwork may be required to make an exit and
// an entrace, but any open space WILL be connected to any other open space via some means.
class mgRandomMazeGenerator
{
private:
	int Depth;
	int DirectiontoCheck;

	MapCreationIndex_s MapCreationIndex[100000]; // 100,000 ceiling for the Creation Index. A waste of memory, for sure, since it is
	// 6x5x100,000 = 3,000,000. So we use about 3MB of ram for this structure alone.
	// In the realm of 16GB RAM chips I say, whateva, atm.
	// Interesting lesson here: This size prevented me from doing anything but dynamically allocating the memory
	// for this class, as a strict definition of a variable as this type would overflow the stack by default.
public:

	mgMapDataHandler *Map = NULL;

	bool CheckBlock(unsigned int BlockY, unsigned int BlockX);
	void InitNewBlock(unsigned int BlockY, unsigned int BlockX, int NewDepth);
	void GenerateMaze(unsigned int BlockY, unsigned int BlockX);
};
#endif