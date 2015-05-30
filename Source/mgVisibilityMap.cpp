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

void mgVisibilityMap::CalculateVisibility(mgPoint CheckPosition)
{
	mgVector CheckDirection;
	mgRayTracer VisibilityTracer;

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

void mgVisibilityMap::CalculateVisibilityBlock(int BlockY, int BlockX)
{
	mgPoint CheckBlock[5];

	// To calculate visibility potential from a block we do a 360 raytrace from 5 points in the block.
	// The first is the center, which may be unnecessary and will possibly get removed [TODO].
	// The other four are the four corners, just to be sure we cover all possible visible areas from within
	// the block space.
	CheckBlock[0].Y = BlockY + 0.5;
	CheckBlock[0].X = BlockX + 0.5;
	CheckBlock[1].Y = CheckBlock[0].Y - MGVISIBILITYEDGEPERCISION;
	CheckBlock[1].X = CheckBlock[0].X - MGVISIBILITYEDGEPERCISION;
	CheckBlock[2].Y = CheckBlock[0].Y - MGVISIBILITYEDGEPERCISION;
	CheckBlock[2].X = CheckBlock[0].X + MGVISIBILITYEDGEPERCISION;
	CheckBlock[3].Y = CheckBlock[0].Y + MGVISIBILITYEDGEPERCISION;
	CheckBlock[3].X = CheckBlock[0].X - MGVISIBILITYEDGEPERCISION;
	CheckBlock[4].Y = CheckBlock[0].Y + MGVISIBILITYEDGEPERCISION;
	CheckBlock[4].X = CheckBlock[0].X + MGVISIBILITYEDGEPERCISION;


	for (int Iterator = 0; Iterator < 5; Iterator++)
		CalculateVisibility(CheckBlock[Iterator]);

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
