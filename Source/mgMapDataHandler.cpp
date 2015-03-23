#include <math.h>
#include <stdlib.h>

#include "mgMapDataHandler.h"

// =------------------------------------=
// = mgMapDataHandler C++ class         =
// =------------------------------------=

bool mgMapDataHandler::PositionBoundsCheck(int PosY, int PosX)
{
	if (PosY < 0)
		return false;
	if (PosX < 0)
		return false;
	if (PosY >= MapSizeY)
		return false;
	if (PosX >= MapSizeX)
		return false;

	return true;
}

void mgMapDataHandler::InitalizeMapData(int SizeY, int SizeX)
{
	mgPoint PositionReference = { 0, 0 };

	if (InternalMapData != NULL)
		delete InternalMapData; 

	InternalMapData = new mgMapElement[SizeY * SizeX];
	MapSizeY = SizeY;
	MapSizeX = SizeX;

	for (int Iterator = 0; Iterator < (SizeY * SizeX); Iterator++)
	{	// Set the defaults for each block in the map.
		InternalMapData[Iterator].BlockType = MAP_BLOCKWALL;
		InternalMapData[Iterator].Position = PositionReference;

		PositionReference.X++;
		if (PositionReference.X >= SizeX)
		{
			PositionReference.X = 0;
			PositionReference.Y++;
		}
	}
}

void mgMapDataHandler::CloneMapIntoHost(mgMapDataHandler *MapToClone)
{
	InitalizeMapData(MapToClone->mapsizeY(), MapToClone->mapsizeX());

	for (int TempY = 0; TempY < MapSizeY; TempY++)
	{
		for (int TempX = 0; TempX < MapSizeX; TempX++)
		{
			mgMapElement *From, *To;
			From = MapToClone->ReturnMapBlockReference(TempY, TempX);
			To = ReturnMapBlockReference(TempY, TempX);

			*To = *From; // While correct, looks hilarious. Reads as such: Butthole To = Butthole From. The poop goes back in again!
		}
	}
}

bool mgMapDataHandler::IsBlockClippable(int PosY, int PosX)
{
	int UnifiedPosition = (PosY * MapSizeX) + PosX; // Our map blocks are stored in a one dimensional dynamically allocated array.

	//std::cout << "Block: " << PosY << ", " << PosX << std::endl;

	if (!PositionBoundsCheck(PosY, PosX))
		return true; // If it lands outside our map area, treat it as a wall.

	// Do any map based clipping tests here, right now that simply is the test for a wall.
	if (InternalMapData[UnifiedPosition].BlockType == MAP_BLOCKWALL)
		return true;

	return false;
}

bool mgMapDataHandler::WillObjectFit(double PosY, double PosX, float ObjectSize)
{
	int UnifiedPosition = (floor(PosY) * MapSizeX) + floor(PosX);

	if (!PositionBoundsCheck(floor(PosY), floor(PosX)))
		return false;

	if (InternalMapData[UnifiedPosition].BlockType == MAP_BLOCKWALL)
		return false;

	// We've exausted all simple methods of ruling this position out, time to work on the more complicated ones.
	double TestY[4]; // 0 = Up, 1 = Down, 2 = Left, 3 = Right
	double TestX[4];

	TestY[2] = TestY[0] = PosY + ObjectSize;
	TestY[3] = TestY[1] = PosY - ObjectSize;
	TestX[0] = TestX[1] = PosX + ObjectSize;
	TestX[2] = TestX[3] = PosX - ObjectSize;

	for (int Iterator = 0; Iterator < 4; Iterator++)
	{
		UnifiedPosition = (floor(TestY[Iterator]) * MapSizeX) + floor(TestY[Iterator]);

		if (InternalMapData[UnifiedPosition].BlockType == MAP_BLOCKWALL)
			return false;
	}

	return true;
}

mgMapElement *mgMapDataHandler::ReturnMapBlockReference(int PosY, int PosX)
{
	int UnifiedPosition = (PosY * MapSizeX) + PosX;

	if (!PositionBoundsCheck(PosY, PosX))
		return NULL; 

	return &InternalMapData[UnifiedPosition];
}

int mgMapDataHandler::mapsizeY(void)
{
	return MapSizeY;
}

int mgMapDataHandler::mapsizeX(void)
{
	return MapSizeX;
}

mgMapDataHandler::mgMapDataHandler()
{
	InternalMapData = NULL;
}

mgMapDataHandler::~mgMapDataHandler()
{
	if (InternalMapData != NULL)
		delete InternalMapData;
}