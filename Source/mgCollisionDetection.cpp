#include <math.h>
#include <stdlib.h>
#include <iostream>

#include "mgCollisionDetection.h"
#include "mgMapObject.h"
#include "mgMapElement.h"
#include "mgLineSegment.h"
#include "mgMapDataHandler.h"
#include "mgVectorPoint.h"
#include "Containers/mgLinkedList.h"
#include "Containers/mgRBTBinaryTree.h"


// Setup the collision detection
void mgCollisionDetection::CollisionSetup(mgMapObject *MovingObject, mgVector Movement) // Stage One
{
	this->MovingObject = MovingObject;
	AttemptedMovement = Movement;

	// Make sure our lists are clean.
	MapElements.ClearList();
	CollisionLines.ClearList();
}

void mgCollisionDetection::SetupDetectionArea(unsigned int Range) // Stage Two
{
	int RangeStartY, RangeStartX, RangeStopY, RangeStopX;

	if ( Range < 2 )
		Range = 2; // The smallest range at which it will function properly.

	// Grab our position from MovingObject and offset it by range.
	RangeStartY = floor(MovingObject->Position.Y) - (Range - 1);
	RangeStartX = floor(MovingObject->Position.X) - (Range - 1);
	RangeStopY = floor(MovingObject->Position.Y) + (Range - 1);
	RangeStopX = floor(MovingObject->Position.X) + (Range - 1);

	// Move over the blocks in this range and add them to our list of Map Elements we need to check.
	// TODO: Add MapObjects to a list if they fall within this range as well.

	for (int RangeY = RangeStartY; RangeY <= RangeStopY; RangeY++)
	{
		for (int RangeX = RangeStartX; RangeX <= RangeStopX; RangeX++)
		{
			mgMapElement *ElementReference;

			ElementReference = MapReference->ReturnMapBlockReference(RangeY, RangeX);

			if (ElementReference != nullptr) // This can happen.
				MapElements.AddElementReference(ElementReference, false); 
		}
	}
}

void mgCollisionDetection::AggregateCollisionLines(void) // Stage Three
{
	MapElements.ResetIterator(); // Just ensuring that we are at the beginning of the list.

	// Let's go through all of our Map Elements and get their list of lines and add it to a master list
	// of collision lines.
	for (int ElementListIterator = 0; ElementListIterator < MapElements.NumberOfElements(); ElementListIterator++)
	{
		mgMapElement *ElementReference;
		mgLinkedList<mgLineSegment> *ElementShape;

		ElementReference = MapElements.ReturnElementReference();
		ElementShape = ElementReference->BlockGeometry();

		if (ElementShape != nullptr)
		{
			for (int LineIterator = 0; LineIterator < ElementShape->NumberOfElements(); LineIterator++)
			{
				mgLineSegment *LineSegRef;

				LineSegRef = ElementShape->ReturnElementReference();

				CollisionLines.AddElementReference(LineSegRef, false);
			}
		}
	}
}

// Tests from the Map Object against the world.
void mgCollisionDetection::PerformCollisionTestsP1(void) // Stage Four: Part One
{
	mgLinkedList<mgLineSegment> *MapObjectShape;
	mgRBTBinaryTree<mgPoint> PointTree;
	mgLinkedList<mgPoint> PointList;

	MapObjectShape = MovingObject->ObjectGeometry();

	DetectedCollisions.ClearList(); // Make sure our list is empty. We need a clean test.

	if (MapObjectShape == nullptr)
		return; // Object has no shape, cannot collide.

	for (int Iterator = 0; Iterator < MapObjectShape->NumberOfElements(); Iterator++)
	{ // For each line in the shape of our map object..
		mgLineSegment *TestLine;

		TestLine = MapObjectShape->ReturnElementReference();

		// Collect unique points in a line by storing them in a binary tree, which we will then
		// dump to a linked list and process. ( This ensures we only test each point once, our binary tree's
		// don't contain duplicate numbers ).

		PointTree.AddElement(TestLine->SegmentStart);
		PointTree.AddElement(TestLine->SegmentEnd);
	}

	PointTree.NodeToList(PointTree.Root, &PointList); // Convert the Binary Tree to a list.

	PointList.ResetIterator(); // Just to be safe.

	for (int Iterator = 0; Iterator < PointList.NumberOfElements(); Iterator++)
	{ // For each point in our object.
		mgLineSegment MovementCollisionLine;
		mgPoint TestPoint;

		TestPoint = PointList.ReturnElement();

		MovementCollisionLine.ImportLine(TestPoint, AttemptedMovement);

		CollisionLines.ResetIterator(); // Start at the beginning of the list.
		
		for (int Iterator2 = 0; Iterator2 < CollisionLines.NumberOfElements(); Iterator2++)
		{ // For each line in the Detection Area
			mgLineCollisionResults TestResults;
			mgLineSegment *LineReference;

			LineReference = CollisionLines.ReturnElementReference();

			TestResults = MovementCollisionLine.CollisionTest(LineReference);

			if (TestResults.Collision)
			{ // We have a collision.
				mgDetectedCollision Collision;

				Collision.PointOfCollision = TestResults.CollisionPoint;
				Collision.CollisionLine = LineReference;

				// Now we need to figure out the vector that will pull us out of this collision.
				Collision.CollisionCorrection.AutoNormalize = false; // We need the defined magnitude.
				Collision.CollisionCorrection.VectorFromCoord(MovementCollisionLine.SegmentEnd, TestResults.CollisionPoint);

				DetectedCollisions.AddElement(Collision);
				exit(0);
			}
		}
	}

	// Part Two: Test from the world inwards.
}

// Tests from the world against the mapobject
void mgCollisionDetection::PerformCollisionTestsP2(void) // Stage Four: Part Two
{

}

mgCollisionDetection::mgCollisionDetection()
{
	MovingObject = nullptr;
	MapReference = nullptr;
}
