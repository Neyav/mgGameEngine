#include <math.h>
#include <stdlib.h>
#include <iostream>

#include "mgRayTracer.h"
#include "mgLineSegment.h"
#include "mgMapDataHandler.h"
#include "mgVectorPoint.h"

void PrintCoord(mgPoint DisplayPoint)
{
	std::cout << "=[" << DisplayPoint.Y << ", " << DisplayPoint.X << "]=";
}

// =------------------------------------=
// = mgRayTracer C++ class              =
// =------------------------------------=
//

mgLinkedList<mgLineSegment> *mgRayTracer::BuildOccluderLines(mgPoint Position)
{
	mgLinkedList<mgLineSegment> *OccluderLineList;

	OccluderLineList = new mgLinkedList < mgLineSegment > ;

	for (int CheckY = floor(Position.Y) - 1; CheckY < floor(Position.Y) + 2; CheckY++)
	{
		for (int CheckX = floor(Position.X) - 1; CheckX < floor(Position.X) + 2; CheckX++)
		{
			mgLinkedList<mgLineSegment> *BlockLineList;
			mgMapElement *BlockReference;

			BlockReference = MapReference->ReturnMapBlockReference(CheckY, CheckX);
			if (BlockReference == NULL) // No mapblock here.
				continue;
			
			BlockLineList = BlockReference->BlockGeometry(); 
			if (BlockLineList != NULL) // Empty blocks don't contain geometry, make sure this isn't one of those before attempting
			{							// to add that geometry to our occluder list.
				BlockLineList->ResetIterator();

				for (int Iterator = 0; Iterator < BlockLineList->NumberOfElements(); Iterator++)
				{
					mgLineSegment *CopiedLine;

					CopiedLine = BlockLineList->ReturnElementReference();

					// False because we do not own the reference, reference is owned by the MapElement.
					OccluderLineList->AddElementReference(CopiedLine, false);
				}
			}

			// TODO: Add object line data here checking here.
			//		Add mgMapObject reference to the LineList so objects can be refrenced when a matching line is found.
		}
	}

	return OccluderLineList;
}

// Determine if any objects or walls are occluding our ray tracer
mgTraceResults mgRayTracer::OccluderPoint(mgPoint Origin, mgVector Direction)
{
	mgPoint Occluder, OccluderBlock;
	double distance = 1000;
	int Scans = 0;
	mgTraceResults Results;

	Results.ImpactLine = NULL;
	Results.ImpactPoint.Y = 0;
	Results.ImpactPoint.X = 0;
	Results.RayDistance = 0;
	Results.CompleteScan = false;

	TracerOrigin = TracerPosition = Origin;
	TracerDirection = Direction;

	// If there is a list, clear it out.
	PositionsChecked.ClearList();

	// Scans only exists to prevent infinite length lines.
	while (Scans < 100)
	{
		mgPoint MapBlock;
		mgLinkedList<mgLineSegment> *LineSegmentList;
		mgLineSegment *LineReference;
		mgLineSegment TestLine;

		Scans++;

		TestLine.ImportLine(TracerPosition, Direction, 1); // We are testing one block at a time, so move one blocks distance per check.

		MapBlock.Y = floor(TracerPosition.Y);
		MapBlock.X = floor(TracerPosition.X);

		// If we're maintaining a list, add this point to the list.
		if (ListPositions)
			PositionsChecked.AddElement(MapBlock);

		//std::cout << "[-Checking block " << BlockY << ", " << BlockX << "-]" << std::endl;

		// We have our grid position in the map, now we need to build the walls of any nearby blocks as linesegments in a list.
		LineSegmentList = BuildOccluderLines(MapBlock);

		// Let's go through the lines, one by one, finding the closing occluding point.
		LineReference = LineSegmentList->ReturnElementReference();
		while (LineReference != NULL)
		{
			mgPoint TestPoint;
			bool Intersection;

			if (!LineReference->ObstructsVision) // This line doesn't block vision, ignore it for our raytrace.
			{
				LineReference = LineSegmentList->ReturnElementReference();
				continue;
			}

			TestPoint = TestLine.InterceptionPoint(LineReference, &Intersection);

			if (Intersection)
			{
				double testDistance = DistanceBetweenPoints(TracerOrigin, TestPoint);

				if (testDistance < distance)
				{
					distance = testDistance;
					Occluder = TestPoint;
					if (LineReference->LineSegmentBlock != NULL)
						OccluderBlock = LineReference->LineSegmentBlock->Position;

					// We're just going to jimmy this in here. Not too much concern is being made for the maintainability of this function because it is intended
					// to get scrapped for a better designed and more capable counterpart.
					Results.ImpactLine = LineReference;
					Results.CompleteScan = true;
					Results.ImpactPoint = TestPoint;
					Results.RayDistance = distance;
				}
			}
			LineReference = LineSegmentList->ReturnElementReference();
		}

		TracerPosition = TestLine.SegmentEnd;

		if (distance < 1000) // Occluder found, terminate. Gotta love all the use of these MAGIC numbers. I should be using defines here, right?
		{
			if (ListPositions)
				PositionsChecked.AddElement(OccluderBlock);

			delete LineSegmentList;
			return Results;
		}

		delete LineSegmentList; // Just incase...
	}

	return Results;
}

mgRayTracer::mgRayTracer()
{
	MapReference = NULL;
	ListPositions = false;
}

mgRayTracer::~mgRayTracer()
{
	// Any clean up needed.
}
