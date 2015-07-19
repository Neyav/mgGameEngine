#include <iostream>

#include "mgPathSolutionGenerator.h"
#include "mgMapDataHandler.h"
#include "mgVectorPoint.h"

// =------------------------------------=
// = mgPathSolutionGenerator C++ class  =
// =------------------------------------=

struct PathFindingNode_s mgPathSolutionGenerator::MapProcessCycle()
{
	struct PathFindingNode_s Results;
	int NewY[4], NewX[4];

	// Inital Init of Results fields.
	Results.Completed = false;
	Results.Y1 = Results.X1 = Results.Y2 = Results.X2 = 0;

	if (PathFindingCurrents.TO == 0 || PathFindingCurrents.FROM == 0)
	{		// TODO: Find a better way of handling this within the class. It essentially means there are no paths between the two points.
		std::cout << "Unsatisfactory ending" << std::endl;
		return Results;
	}

	for (int TempY = 0; TempY < MapSizeY; TempY++)
		for (int TempX = 0; TempX < MapSizeX; TempX++)
		{
			if (PathCreationMap[CordToUnifiedCord(TempY, TempX)] == PathFindingCurrents.TO || PathCreationMap[CordToUnifiedCord(TempY, TempX)] == PathFindingCurrents.FROM)
			{
				NewY[0] = TempY - 1;
				NewY[1] = TempY + 1;
				NewY[2] = NewY[3] = TempY;
				NewX[0] = NewX[1] = TempX;
				NewX[2] = TempX - 1;
				NewX[3] = TempX + 1;

				for (int Iterator = 0; Iterator < 4; Iterator++)
				{
					if (!MapReference->IsBlockClippable(NewY[Iterator], NewX[Iterator]))
					{
						if (PathCreationMap[CordToUnifiedCord(NewY[Iterator], NewX[Iterator])] == 0)
						{
							if (PathCreationMap[CordToUnifiedCord(TempY, TempX)] < 500000) // Hack number. We need a better way to do this.
							{
								PathCreationMap[CordToUnifiedCord(NewY[Iterator], NewX[Iterator])] = PathFindingCurrents.FROM + 1;
							}
							else
							{
								PathCreationMap[CordToUnifiedCord(NewY[Iterator], NewX[Iterator])] = PathFindingCurrents.TO - 1;
							}
						}
						else
						{
							if (PathCreationMap[CordToUnifiedCord(NewY[Iterator], NewX[Iterator])] > 500000 && PathCreationMap[CordToUnifiedCord(TempY, TempX)] < 500000)
							{
								Results.Y2 = NewY[Iterator];
								Results.X2 = NewX[Iterator];
								Results.Y1 = TempY;
								Results.X1 = TempX;
								Results.Completed = true;
								return Results;
							}
							else if (PathCreationMap[CordToUnifiedCord(NewY[Iterator], NewX[Iterator])] < 500000 && PathCreationMap[CordToUnifiedCord(TempY, TempX)] > 500000)
							{
								Results.Y1 = NewY[Iterator];
								Results.X1 = NewX[Iterator];
								Results.Y2 = TempY;
								Results.X2 = TempX;
								Results.Completed = true;
								return Results;
							}
						}
					}
				}

			}
		}

	//std::cout << Currents->TO << std::endl;

	PathFindingCurrents.TO--;
	PathFindingCurrents.FROM++;

	return Results;
}


void mgPathSolutionGenerator::LinkToMap(mgMapDataHandler *MapData)
{
	MapSizeY = MapData->mapsizeY();
	MapSizeX = MapData->mapsizeX();

	MapReference = MapData;
}

// Initalize all necessary factors in processing the map solution.
void mgPathSolutionGenerator::InitateMapSolution(int StartY, int StartX, int EndY, int EndX)
{
	if (MapReference == nullptr)
		return;

	if (PathMap != nullptr)
		delete[] PathMap;
	if (PathCreationMap != nullptr)
		delete[] PathCreationMap;

	PathMap = new int[MapSizeY * MapSizeX]; // Allocate the appropriate amount of memory
	PathCreationMap = new int[MapSizeY * MapSizeX];

	for (int Iterator = 0; Iterator < (MapSizeY * MapSizeX); Iterator++)
		PathMap[Iterator] = PathCreationMap[Iterator] = 0; // Initalize both pathmaps

	PathCreationMap[CordToUnifiedCord(StartY, StartX)] = 1;
	PathCreationMap[CordToUnifiedCord(EndY, EndX)] = 1000000;   // I'm still not entirely happy with this mechanic, but it does work.
	// I'll modify it in a future project for sure.

	PathFindingCurrents.TO = 1000000;
	PathFindingCurrents.FROM = 1;
}

bool mgPathSolutionGenerator::ProcessMapSolution(int CyclesToProcess)
{
	int CyclesLeft = CyclesToProcess;
	struct PathFindingNode_s Node;

	Node.Completed = false;

	while (Node.Completed == false || CyclesLeft > 0)
	{
		Node = MapProcessCycle();
		CyclesLeft--;
	}

	if (Node.Completed == false)
		return false;

	// Now we're going to start at the nodes and we're going to count backwards.

	int Value1 = PathCreationMap[CordToUnifiedCord(Node.Y1, Node.X1)];
	int Value2 = PathCreationMap[CordToUnifiedCord(Node.Y2, Node.X2)];

	PathMap[CordToUnifiedCord(Node.Y1, Node.X1)] = Value1;
	PathMap[CordToUnifiedCord(Node.Y2, Node.X2)] = Value2;

	if (Value2 < 500000 || Value1 > 500000)
	{	// Another example of there being no path between the two blocks. Need to handle this better so it doesn't close the program.
		std::cout << "bool mgPathSolutionGenerator::ProcessMapSolution(int CyclesToProcess) -> Critical Failure" << std::endl;
		exit(-1);
	}

	while (Value1 > 0)
	{
		// For Value1 we will find the next lowest block
		Value1--;
		if (PathCreationMap[CordToUnifiedCord(Node.Y1 - 1, Node.X1)] == Value1)
			Node.Y1--;
		else if (PathCreationMap[CordToUnifiedCord(Node.Y1 + 1, Node.X1)] == Value1)
			Node.Y1++;
		else if (PathCreationMap[CordToUnifiedCord(Node.Y1, Node.X1 - 1)] == Value1)
			Node.X1--;
		else if (PathCreationMap[CordToUnifiedCord(Node.Y1, Node.X1 + 1)] == Value1)
			Node.X1++;

		PathMap[CordToUnifiedCord(Node.Y1, Node.X1)] = Value1;

		// For Value2 we will find the next highest block
		Value2++;
		if (PathCreationMap[CordToUnifiedCord(Node.Y2 - 1, Node.X2)] == Value2)
			Node.Y2--;
		else if (PathCreationMap[CordToUnifiedCord(Node.Y2 + 1, Node.X2)] == Value2)
			Node.Y2++;
		else if (PathCreationMap[CordToUnifiedCord(Node.Y2, Node.X2 - 1)] == Value2)
			Node.X2--;
		else if (PathCreationMap[CordToUnifiedCord(Node.Y2, Node.X2 + 1)] == Value2)
			Node.X2++;

		PathMap[CordToUnifiedCord(Node.Y2, Node.X2)] = Value2;
	}

	delete[] PathCreationMap; // We've no need for this anymore as we have our route.

	return true;

}

bool mgPathSolutionGenerator::CordPartofPath(int CordY, int CordX)
{
	int UnifiedCord = CordToUnifiedCord(CordY, CordX);

	if (PathMap == nullptr)
		return false;

	if (PathMap[UnifiedCord] > 0)
		return true;

	return false;
}

mgPoint mgPathSolutionGenerator::NextBlock(int CordY, int CordX)
{
	mgPoint NextPathNode;

	int NewY[4], NewX[4];

	NewY[0] = CordY - 1;
	NewY[1] = CordY + 1;
	NewY[2] = NewY[3] = CordY;

	NewX[0] = NewX[1] = CordX;
	NewX[2] = CordX - 1;
	NewX[3] = CordX + 1;

	for (int Iterator = 0; Iterator < 4; Iterator++)
	{
		if (PathMap[CordToUnifiedCord(NewY[Iterator], NewX[Iterator])] > PathMap[CordToUnifiedCord(CordY, CordX)])
		{
			NextPathNode.Y = NewY[Iterator];
			NextPathNode.X = NewX[Iterator];

			return NextPathNode;
		}
	}

	NextPathNode.Y = -1;
	NextPathNode.X = -1;

	return NextPathNode;
}

mgPathSolutionGenerator::mgPathSolutionGenerator()
{
	PathMap = PathCreationMap = nullptr; // Ensure these are nullptr

	MapReference = nullptr; // This as well.
}

mgPathSolutionGenerator::~mgPathSolutionGenerator()
{
	// Clear up any internal dynamically allocated memory blocks.

	if (PathMap != nullptr)
		delete[] PathMap;

	if (PathCreationMap != nullptr)
		delete[] PathCreationMap;
}
