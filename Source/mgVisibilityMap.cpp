#include <math.h>
#include <stdlib.h>
#include <iostream>

#include "mgVisibilityMap.h"
#include "mgVectorPoint.h"
#include "mgMapDataHandler.h"
#include "mgRayTracer.h"
#include "mgLinkedList.h"

// =------------------------------------=
// = mgVisibilityMap C++ class			=
// =------------------------------------=

// Returns true if the block is marked as visible in our list, false otherwise
bool mgVisibilityMap::IsMarkedVisible(int CheckY, int CheckX)
{
	mgPoint TestPosition;

	TestPosition.Y = CheckY;
	TestPosition.X = CheckX;

	return VisibilityTree.IsElementPresent(TestPosition);
}

void mgVisibilityMap::AddVisiblePoint(int AddY, int AddX)
{
	mgPoint NewPoint;

	NewPoint.Y = AddY;
	NewPoint.X = AddX;
	VisibilityTree.AddElement(NewPoint);
}

void mgVisibilityMap::AddVisiblePoint(mgPoint Point)
{
	AddVisiblePoint(floor(Point.Y), floor(Point.X)); // Seems a little rediculous but whatever.
}

int mgVisibilityMap::MappedItems(void)
{
	return VisibilityTree.Elements();
}

void mgVisibilityMap::LinkToMapHandler(mgMapDataHandler *LinkMap)
{
	MapReference = LinkMap;
}

void mgVisibilityMap::CalculateVisibility(int BlockY, int BlockX)
{
	mgPoint CheckPosition;
	mgVector CheckDirection;
	mgRayTracer VisibilityTracer;

	CheckPosition.Y = BlockY + 0.5;
	CheckPosition.X = BlockX + 0.5;

	VisibilityTracer.MapReference = MapReference;
	VisibilityTracer.ListPositions = true;

	// Spin in a 360 degree angle checking visibility for this block against all those around it
	for (int CheckAngle = 0; CheckAngle < 360; CheckAngle++)
	{
		CheckDirection.VectorFromDegrees(CheckAngle);

		// Uses the PositionsChecked list to determine results, and therefore doesn't need the information stored in mgTraceResults
		VisibilityTracer.OccluderPoint(CheckPosition, CheckDirection);
		VisibilityTracer.PositionsChecked.ResetIterator(); // Make sure we're starting at the beginning.

		while (!VisibilityTracer.PositionsChecked.IteratorAtEnd())
		{
			mgPoint CheckedListItem;

			CheckedListItem = VisibilityTracer.PositionsChecked.ReturnElement();

			AddVisiblePoint(CheckedListItem);
		}
	}
}

void mgVisibilityMap::CalculateAdjacentVisibility(int BlockY, int BlockX)
{
	// Calculate visibility for adjacent blocks, do this by calling the CalculateVisibility function on each block beside.
	int testY[4], testX[4];

	testY[0] = BlockY - 1;
	testY[1] = BlockY + 1;
	testY[2] = testY[3] = BlockY;
	testX[0] = testX[1] = BlockX;
	testX[2] = BlockX - 1;
	testX[3] = BlockX + 1;

	for (int Iterator = 0; Iterator < 4; Iterator++)
		CalculateVisibility(testY[Iterator], testX[Iterator]);

#ifdef BINARYTREEDUMP
	VisibilityTree.DumpTreeStructure("TreeOutput.txt");
#endif
}

mgVisibilityMap::mgVisibilityMap()
{
	MapReference = nullptr;
	VisiblePoints = 0;
}

mgVisibilityMap::~mgVisibilityMap()
{
	
}
