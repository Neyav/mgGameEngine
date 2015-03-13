#include <time.h>

#include "mgRandomMazeGenerator.h"
#include "mgMapDataHandler.h"
#include "mgMapElement.h"

// =------------------------------------=
// = mgRandomMazeGenerator C++ class    =
// =------------------------------------=

bool mgRandomMazeGenerator::CheckBlock(unsigned int BlockY, unsigned int BlockX)
{
	// We need to make sure that this block only has one floor adjacent to it.
	unsigned int FloorCount = 0;

	if (!Map->IsBlockClippable(BlockY - 1, BlockX))
		FloorCount++;
	if (!Map->IsBlockClippable(BlockY + 1, BlockX))
		FloorCount++;
	if (!Map->IsBlockClippable(BlockY, BlockX - 1))
		FloorCount++;
	if (!Map->IsBlockClippable(BlockY, BlockX + 1))
		FloorCount++;

	if (FloorCount > 1)
		return false;

	return true;

}

void mgRandomMazeGenerator::InitNewBlock(unsigned int BlockY, unsigned int BlockX, int NewDepth)
{
	mgMapElement *FloorBlock;

	MapCreationIndex[NewDepth].BlockY = BlockY;
	MapCreationIndex[NewDepth].BlockX = BlockX;
	MapCreationIndex[NewDepth].DirectionsValid = 4;
	MapCreationIndex[NewDepth].Direction[0] = 0;
	MapCreationIndex[NewDepth].Direction[1] = 0;
	MapCreationIndex[NewDepth].Direction[2] = 0;
	MapCreationIndex[NewDepth].Direction[3] = 0;

	// Default Floor Block Type for random maps
	FloorBlock = Map->ReturnMapBlockReference(BlockY, BlockX);
	FloorBlock->BlockType = MAP_BLOCKFLOOR;
}

void mgRandomMazeGenerator::GenerateMaze(unsigned int BlockY, unsigned int BlockX)
{
	Depth = 0;

	srand(time(NULL)); // Seed random generator

	InitNewBlock(BlockY, BlockX, Depth);

	while (Depth >= 0)
	{
		while (MapCreationIndex[Depth].DirectionsValid > 0)
		{
			DirectiontoCheck = rand() % 4;

			if (MapCreationIndex[Depth].Direction[DirectiontoCheck] == 0)
			{
				MapCreationIndex[Depth].Direction[DirectiontoCheck] = 1;
				MapCreationIndex[Depth].DirectionsValid--;

				if (DirectiontoCheck == 0) // up
				{
					// bounds check
					if (MapCreationIndex[Depth].BlockY >= 2)
						if (CheckBlock(MapCreationIndex[Depth].BlockY - 1, MapCreationIndex[Depth].BlockX))
						{
							InitNewBlock(MapCreationIndex[Depth].BlockY - 1, MapCreationIndex[Depth].BlockX, Depth + 1);
							Depth++;
						}
				}
				else if (DirectiontoCheck == 1) // down
				{
					// bounds check
					if (MapCreationIndex[Depth].BlockY != Map->mapsizeY() - 2)
						if (CheckBlock(MapCreationIndex[Depth].BlockY + 1, MapCreationIndex[Depth].BlockX))
						{
							InitNewBlock(MapCreationIndex[Depth].BlockY + 1, MapCreationIndex[Depth].BlockX, Depth + 1);
							Depth++;
						}
				}
				else if (DirectiontoCheck == 2) // east
				{
					// bounds check
					if (MapCreationIndex[Depth].BlockX >= 2)
						if (CheckBlock(MapCreationIndex[Depth].BlockY, MapCreationIndex[Depth].BlockX - 1))
						{
							InitNewBlock(MapCreationIndex[Depth].BlockY, MapCreationIndex[Depth].BlockX - 1, Depth + 1);
							Depth++;
						}
				}
				else if (DirectiontoCheck == 3) // west
				{
					// bounds check
					if (MapCreationIndex[Depth].BlockX != Map->mapsizeX() - 2)
						if (CheckBlock(MapCreationIndex[Depth].BlockY, MapCreationIndex[Depth].BlockX + 1))
						{
							InitNewBlock(MapCreationIndex[Depth].BlockY, MapCreationIndex[Depth].BlockX + 1, Depth + 1);
							Depth++;

						}
				}
			}
		}
		Depth--; // Go back a block
	}
}