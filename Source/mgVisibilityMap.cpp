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
	mgPoint *ListObject;

	VisibilityList.ResetIterator();

	ListObject = VisibilityList.ReturnElement();

	while (ListObject != NULL)
	{
		if (ListObject->Y == CheckY && ListObject->X == CheckX)
			return true; // The block is marked as visible

		ListObject = VisibilityList.ReturnElement();
	}

	return false; // It isn't marked in this list
}

void mgVisibilityMap::AddVisiblePoint(int AddY, int AddX)
{
	if (!IsMarkedVisible(AddY, AddX))
	{	// It's not in our list, add it to our list.
		mgPoint NewPoint;

		NewPoint.Y = AddY;
		NewPoint.X = AddX;
		VisibilityList.AddElement(NewPoint);
	}
}

void mgVisibilityMap::AddVisiblePoint(mgPoint Point)
{
	AddVisiblePoint(floor(Point.Y), floor(Point.X)); // Seems a little rediculous but whatever.
}

int mgVisibilityMap::MappedItems(void)
{
	return VisibilityList.NumberOfElements();
}

mgPoint mgVisibilityMap::SequentialObtainItem(void)
{
	mgPoint ReturnValues;
	mgPoint *PositionReturned;

	PositionReturned = VisibilityList.ReturnElement();

	if (PositionReturned == NULL)
	{
		ReturnValues.Y = -1;
		ReturnValues.X = -1;

		return ReturnValues;
	}

	ReturnValues.Y = PositionReturned->Y;
	ReturnValues.X = PositionReturned->X;

	return ReturnValues;
}

void mgVisibilityMap::SequentialMarkerReset(void)
{
	VisibilityList.ResetIterator();
}

// Merge another map into this one.
void mgVisibilityMap::MergeMaps(mgVisibilityMap *OtherMap)
{
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

}

void mgVisibilityMap::LinkToMapHandler(mgMapDataHandler *LinkMap)
{
	MapReference = LinkMap;
}

void mgVisibilityMap::CalculateVisibility(int BlockY, int BlockX)
{
	mgPoint CheckPoint; // HAH.... wait, yes, okay, lolololololol.. ahahaha... Okay, I'm calm.
	mgPoint CheckPosition;
	mgVector CheckDirection;
	mgPoint *CheckedListItem;
	mgRayTracer VisibilityTracer;

	CheckPosition.Y = BlockY + 0.5;
	CheckPosition.X = BlockX + 0.5;

	VisibilityTracer.MapReference = MapReference;
	VisibilityTracer.ListPositions = true;

	// Spin in a 360 degree angle checking visibility for this block against all those around it
	for (int CheckAngle = 0; CheckAngle < 360; CheckAngle++)
	{
		CheckDirection.VectorFromDegrees(CheckAngle);

		//std::cout << "Trace: " << CheckAngle << std::endl;

		CheckPoint = VisibilityTracer.OccluderPoint(CheckPosition, CheckDirection);
		VisibilityTracer.PositionsChecked.ResetIterator(); // Make sure we're starting at the beginning.

		//AddVisiblePoint(round(CheckPoint.Y), round(CheckPoint.X));

		CheckedListItem = VisibilityTracer.PositionsChecked.ReturnElement();

		while (CheckedListItem != NULL)
		{
			AddVisiblePoint(*CheckedListItem);
			CheckedListItem = VisibilityTracer.PositionsChecked.ReturnElement();
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
