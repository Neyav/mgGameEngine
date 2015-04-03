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
#ifdef USEBINARYTREE
	mgPoint TestPosition;

	TestPosition.Y = CheckY;
	TestPosition.X = CheckX;

	return VisibilityTree.IsElementPresent(TestPosition);
#else
	mgPoint ListObject;

	VisibilityList.ResetIterator();	

	while (!VisibilityList.IteratorAtEnd())
	{
		ListObject = VisibilityList.ReturnElement();

		if (ListObject.Y == CheckY && ListObject.X == CheckX)
			return true; // The block is marked as visible
	}

	return false; // It isn't marked in this list
#endif
}

void mgVisibilityMap::AddVisiblePoint(int AddY, int AddX)
{
#ifdef USEBINARYTREE
	mgPoint NewPoint;

	NewPoint.Y = AddY;
	NewPoint.X = AddX;
	VisibilityTree.AddElement(NewPoint);
#else
	if (!IsMarkedVisible(AddY, AddX))
	{	// It's not in our list, add it to our list.
		mgPoint NewPoint;

		NewPoint.Y = AddY;
		NewPoint.X = AddX;
		VisibilityList.AddElement(NewPoint);
	}
#endif
}

void mgVisibilityMap::AddVisiblePoint(mgPoint Point)
{
	AddVisiblePoint(floor(Point.Y), floor(Point.X)); // Seems a little rediculous but whatever.
}

int mgVisibilityMap::MappedItems(void)
{
#ifdef USEBINARYTREE
	return VisibilityTree.Elements();
#else
	return VisibilityList.NumberOfElements();
#endif
}

mgPoint mgVisibilityMap::SequentialObtainItem(void)
{
#ifdef USEBINARYTREE
	mgPoint t;
	t.Y = 0;
	t.X = 0;

	return t;
#else
	mgPoint ReturnValues;

	if (VisibilityList.IteratorAtEnd())
	{
		std::cout << "mgPoint mgVisibilityMap::SequentialObtainItem(void) -- Asked for object past end of list." << std::endl;
		exit(-1);
	}

	ReturnValues = VisibilityList.ReturnElement();

	return ReturnValues;
#endif
}

void mgVisibilityMap::SequentialMarkerReset(void)
{
#ifdef USEBINARYTREE

#else
	VisibilityList.ResetIterator();
#endif
}

// Merge another map into this one.
void mgVisibilityMap::MergeMaps(mgVisibilityMap *OtherMap)
{
#ifdef USEBINARYTREE

#else
	int ElementsToCopy = 0;

	if (OtherMap == NULL)
		return;

	OtherMap->SequentialMarkerReset();

	ElementsToCopy = OtherMap->MappedItems();

	while (ElementsToCopy > 0) // Sequentially grab and copy each item
	{
		AddVisiblePoint(OtherMap->SequentialObtainItem());
		ElementsToCopy--;
	}
#endif
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
}

mgVisibilityMap::mgVisibilityMap()
{
	MapReference = NULL;
	VisiblePoints = 0;
}

mgVisibilityMap::~mgVisibilityMap()
{
	
}
