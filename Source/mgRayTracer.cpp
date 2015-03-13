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
			mgShape *BlockShape;
			mgMapElement *BlockReference;

			BlockReference = MapReference->ReturnMapBlockReference(CheckY, CheckX);
			if (BlockReference == NULL) // No mapblock here.
				continue;
			BlockShape = BlockReference->BlockGeometry();
			BlockLineList = BlockShape->ShapeForm();

			BlockLineList->ResetIterator();

			for (int Iterator = 0; Iterator < BlockLineList->NumberOfElements(); Iterator++)
			{
				mgLineSegment *CopiedLine;
				
				CopiedLine = BlockLineList->ReturnElementReference();

				// Apply offsets to the shape to ensure it is checked for occlusions properly.
				CopiedLine->SegmentStart.Y += CheckY;
				CopiedLine->SegmentStart.X += CheckX;
				CopiedLine->SegmentEnd.Y += CheckY;
				CopiedLine->SegmentEnd.X += CheckX;

				OccluderLineList->AddElement(*CopiedLine);
			}

			delete BlockLineList;

			// TODO: Add object line data here checking here.
			//		Add mgMapObject reference to the LineList so objects can be refrenced when a matching line is found.
		}
	}

	return OccluderLineList;
}

// Determine if any objects or walls are occluding our ray tracer
mgPoint mgRayTracer::OccluderPoint(mgPoint Origin, mgVector Direction)
{
	mgPoint Occluder, OccluderBlock;
	double distance = 1000;

	TracerOrigin = TracerPosition = Origin;
	TracerDirection = Direction;

	// If there is a list, clear it out.
	PositionsChecked.ClearList();

	//std::cout << "mgPoint mgRayTracer::OccluderPoint(mgPoint Origin, mgVector Direction)" << std::endl;

	while (1)
	{
		mgPoint MapBlock;
		mgLinkedList<mgLineSegment> *LineSegmentList;
		mgLineSegment *LineReference;
		mgLineSegment TestLine;

		TestLine.ImportLine(TracerPosition, Direction, 1); // We are testing one block at a time, so move one blocks distance per check.

		MapBlock.Y = floor(TracerPosition.Y);
		MapBlock.X = floor(TracerPosition.X);

		// If we're maintaining a list, add this point to the list.
		if (ListPositions)
			PositionsChecked.AddElement(MapBlock);

		//std::cout << "[-Checking block " << BlockY << ", " << BlockX << "-]" << std::endl;

		// We have our grid position in the map, now we need to build the walls of any nearby blocks as linesegments in a list.
		LineSegmentList = BuildOccluderLines(MapBlock);
		LineReference = LineSegmentList->ReturnElement();

		// Let's go through the lines, one by one, finding the closing occluding point.
		while (LineReference != NULL)
		{
			mgPoint TestPoint;
			bool Intersection;

			//std::cout << "Checking against line [" << LineSegmentList->CurrentSegment->SegmentStart.Y << ", " << LineSegmentList->CurrentSegment->SegmentStart.X << "] to" << std::endl;
			//std::cout << "                      [" << LineSegmentList->CurrentSegment->SegmentEnd.Y << ", " << LineSegmentList->CurrentSegment->SegmentEnd.X << "]" << std::endl;

			TestPoint = TestLine.InterceptionPoint(LineReference, &Intersection);

			if (Intersection)
			{
				double testDistance = DistanceBetweenPoints(TracerOrigin, TestPoint);

				//std::cout << " -[ Intersection at (" << TestPoint.Y << ", " << TestPoint.X << ": Distance-> " << testDistance << std::endl;

				if (testDistance < distance)
				{
					distance = testDistance;
					Occluder = TestPoint;
					if (LineReference->LineSegmentBlock != NULL)
						OccluderBlock = LineReference->LineSegmentBlock->Position;
				}
			}

			delete LineReference;
			LineReference = LineSegmentList->ReturnElement();
		}

		TracerPosition = TestLine.SegmentEnd;

		if (distance < 1000) // Occluder found, terminate. Gotta love all the use of these MAGIC numbers. I should be using defines here, right?
		{
			if (ListPositions)
				PositionsChecked.AddElement(OccluderBlock);

			RayDistance = distance;

			delete LineSegmentList;
			return Occluder;
		}
	}
}

mgRayTracer::~mgRayTracer()
{
	// Any clean up needed.
}
