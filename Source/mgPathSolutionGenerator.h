#ifndef MGPATHSOLUTIONGENERATORH
#define MGPATHSOLUTIONGENERATORH

#include "mgMapDataHandler.h"
#include "mgVectorPoint.h"

struct PathFindingNode_s
{
	int Y1;
	int X1;
	int Y2;
	int X2;
	bool Completed;
};

struct PathFindingCurrents_s
{
	int FROM;
	int TO;
};

// =------------------------------------=
// = mgPathSolutionGenerator C++ class  =
// =------------------------------------=
//
// The meat and gravy of this particular experiment. I had originally designed all of it to test a new method for
// shortest path mapping. I had a new idea, experimented with it, and it works. It's fast, I'm not sure if it's fast
// enough though. A lot of optimization could be put into it for sure, but truth be told I'll probably just rewrite
// it better next time with the lessons learned from this iteration. What I can tell you for certain is that it works.
class mgPathSolutionGenerator
{
private:
	mgMapDataHandler *MapReference;
	int *PathMap;			// Contains the final path
	int *PathCreationMap;	// Used during the creation of the map
	int MapSizeY, MapSizeX;
	struct PathFindingCurrents_s PathFindingCurrents;

	inline int CordToUnifiedCord(int CordY, int CordX)
	{
		return (CordY * MapSizeX) + CordX;
	}
	struct PathFindingNode_s MapProcessCycle();

public:
	void LinkToMap(mgMapDataHandler *MapData);
	void InitateMapSolution(int StartY, int StartX, int EndY, int EndX);
	bool ProcessMapSolution(int CyclesToProcess);
	bool CordPartofPath(int CordY, int CordX);
	mgPoint NextBlock(int CordY, int CordX);

	mgPathSolutionGenerator();  // Constructor
	~mgPathSolutionGenerator(); // Destructor
};

#endif