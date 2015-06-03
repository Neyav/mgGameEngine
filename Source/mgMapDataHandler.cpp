#include <math.h>
#include <stdlib.h>

#include "mgMapDataHandler.h"

_mgGameEngineVersionstruct mgGameEngineVersion = { 0, 5, 0 };

// =------------------------------------=
// = mgMapDataHandler C++ class         =
// =------------------------------------=

void mgMapDataHandler::CleanHouseProtocol(void)
{
	if (InternalMapData == nullptr)
		return;

	// Clean up the blocks inside each element, and each element itself.
	for (int Iterator = 0; Iterator < (MapSizeY * MapSizeX); Iterator++)
		delete InternalMapData[Iterator].MapBlock;

	delete[] InternalMapData;
}

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
	mgPoint PositionReference;

	if (InternalMapData != nullptr)
		CleanHouseProtocol();

	InternalMapData = new mgMapElementArray[SizeY * SizeX];
	MapSizeY = SizeY;
	MapSizeX = SizeX;

	for (int Iterator = 0; Iterator < (SizeY * SizeX); Iterator++)
	{	// Set the defaults for each block in the map.
		InternalMapData[Iterator].MapBlock = new mgMapElement;
		InternalMapData[Iterator].MapBlock->BlockType = MAP_BLOCKWALL;
		InternalMapData[Iterator].MapBlock->Position = PositionReference;

		PositionReference.X++;
		if (PositionReference.X >= SizeX)
		{
			PositionReference.X = 0;
			PositionReference.Y++;
		}
	}

	// This section adds another 2 seconds on the stresstest timer. It's definitely not the fastest way I could have done it, but all things considered it's among the 
	// cleanest and most straight forward. This isn't a section of code I consider "time sensitive". mgMapDataHandler's are NOT toilet paper. We do not wipe our ripe and
	// then swipe another.
	for (int Y = 0; Y < SizeY; Y++)
	{
		for (int X = 0; X < SizeX; X++)
		{
			mgMapElement *BlockReference = ReturnMapBlockReference(Y, X);
			BlockReference->Adjacent[ELEMENT_NORTH] = ReturnMapBlockReference(Y - 1, X);
			BlockReference->Adjacent[ELEMENT_EAST] = ReturnMapBlockReference(Y, X + 1);
			BlockReference->Adjacent[ELEMENT_WEST] = ReturnMapBlockReference(Y, X - 1);
			BlockReference->Adjacent[ELEMENT_SOUTH] = ReturnMapBlockReference(Y + 1, X);
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

	// Connect adjacent block property to its new neighbor.
	for (int Y = 0; Y < MapSizeY; Y++)
	{
		for (int X = 0; X < MapSizeX; X++)
		{
			mgMapElement *BlockReference = ReturnMapBlockReference(Y, X);
			BlockReference->Adjacent[ELEMENT_NORTH] = ReturnMapBlockReference(Y - 1, X);
			BlockReference->Adjacent[ELEMENT_EAST] = ReturnMapBlockReference(Y, X + 1);
			BlockReference->Adjacent[ELEMENT_WEST] = ReturnMapBlockReference(Y, X - 1);
			BlockReference->Adjacent[ELEMENT_SOUTH] = ReturnMapBlockReference(Y + 1, X);
		}
	}
}

bool mgMapDataHandler::IsBlockClippable(int PosY, int PosX)
{
	int UnifiedPosition = (PosY * MapSizeX) + PosX; // Our map blocks are stored in a one dimensional dynamically allocated array.

	if (!PositionBoundsCheck(PosY, PosX))
		return true; // If it lands outside our map area, treat it as a wall.

	// Do any map based clipping tests here, right now that simply is the test for a wall.
	if (InternalMapData[UnifiedPosition].MapBlock->BlockType == MAP_BLOCKWALL)
		return true;

	return false;
}

bool mgMapDataHandler::WillObjectFit(double PosY, double PosX, float ObjectSize)
{
	int UnifiedPosition = (floor(PosY) * MapSizeX) + floor(PosX);

	if (!PositionBoundsCheck(floor(PosY), floor(PosX)))
		return false;

	if (InternalMapData[UnifiedPosition].MapBlock->BlockType == MAP_BLOCKWALL)
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

		if (InternalMapData[UnifiedPosition].MapBlock->BlockType == MAP_BLOCKWALL)
			return false;
	}

	return true;
}

mgMapElement *mgMapDataHandler::ReturnMapBlockReference(int PosY, int PosX)
{
	int UnifiedPosition = (PosY * MapSizeX) + PosX;

	if (!PositionBoundsCheck(PosY, PosX))
		return nullptr; 

	return InternalMapData[UnifiedPosition].MapBlock;
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
	InternalMapData = nullptr;
}

mgMapDataHandler::~mgMapDataHandler()
{
	if (InternalMapData != nullptr)
		CleanHouseProtocol();
}