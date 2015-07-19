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
			mgListIterator<mgLineSegment> BlockLineList;
			mgMapElement *BlockReference;

			BlockReference = MapReference->ReturnMapBlockReference(CheckY, CheckX);
			if (BlockReference == nullptr) // No mapblock here.
				continue;
			
			BlockLineList = BlockReference->BlockGeometry(); 

			while (!BlockLineList.IteratorAtEnd())
			{
				mgLineSegment *CopiedLine;

				CopiedLine = BlockLineList.ReturnElementReference();

				// False because we do not own the reference, reference is owned by the MapElement.
				OccluderLineList->AddElementReference(CopiedLine, false);
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

	Results.ImpactLine = nullptr;
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
		mgListIterator<mgLineSegment> LineSegmentIterator;
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
		LineSegmentIterator.LinktoList(LineSegmentList);

		// Let's go through the lines, one by one, finding the closing occluding point.
		while (!LineSegmentIterator.IteratorAtEnd())
		{
			mgLineCollisionResults TestResults;

			LineReference = LineSegmentIterator.ReturnElementReference();

			if (!LineReference->ObstructsVision) // This line doesn't block vision, ignore it for our raytrace.
				continue;

			TestResults = TestLine.CollisionTest(LineReference);

			if (TestResults.Collision)
			{
				double testDistance = DistanceBetweenPoints(TracerOrigin, TestResults.CollisionPoint);

				if (testDistance < distance)
				{
					distance = testDistance;
					Occluder = TestResults.CollisionPoint;
					if (LineReference->LineSegmentBlock != nullptr)
						OccluderBlock = LineReference->LineSegmentBlock->Position;

					// We're just going to jimmy this in here. Not too much concern is being made for the maintainability of this function because it is intended
					// to get scrapped for a better designed and more capable counterpart.
					Results.ImpactLine = LineReference;
					Results.CompleteScan = true;
					Results.ImpactPoint = TestResults.CollisionPoint;
					Results.RayDistance = distance;
				}
			}
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
	MapReference = nullptr;
	ListPositions = false;
}

mgRayTracer::~mgRayTracer()
{
	// Any clean up needed.
}
