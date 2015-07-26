#include <math.h>
#include <stdlib.h>

#include "mgCollisionDetection.h"
#include "mgMapObject.h"
#include "mgMapElement.h"
#include "mgLineSegment.h"
#include "mgMapDataHandler.h"
#include "mgVectorPoint.h"
#include "Containers/mgLinkedList.h"
#include "Containers/mgRBTBinaryTree.h"

//
// =------------------------------------=
// = mgCollisionDetection C++ class     =
// =------------------------------------=
//
// This class tests a MapObject that is moving against the world and reports back the results of the
// nature of the collision, and information relevant to moving the object to a safe position.

// Setup the collision detection
void mgCollisionDetection::CollisionSetup(mgMapObject *MovingObject, mgVector Movement) // Stage One
{
	this->MovingObject = MovingObject;
	AttemptedMovement = Movement;

	// Make sure our lists are clean.
	CollisionLines.ClearList();
}

void mgCollisionDetection::SetupDetectionArea(unsigned int Range) // Stage Two
{
	int RangeStartY, RangeStartX, RangeStopY, RangeStopX;

	if ( Range < 2 )
		Range = 2; // The smallest range at which it will function properly.

	// Grab our position from MovingObject and offset it by range.
	RangeStartY = mgFloor(MovingObject->Position.Y) - (Range - 1);
	RangeStartX = mgFloor(MovingObject->Position.X) - (Range - 1);
	RangeStopY = mgFloor(MovingObject->Position.Y) + (Range - 1);
	RangeStopX = mgFloor(MovingObject->Position.X) + (Range - 1);

	// Move over the blocks in this range and add them to our list of Map Elements we need to check.
	// TODO: Add MapObjects to a list if they fall within this range as well.

	for (int RangeY = RangeStartY; RangeY <= RangeStopY; RangeY++)
	{
		for (int RangeX = RangeStartX; RangeX <= RangeStopX; RangeX++)
		{
			mgMapElement *ElementReference;

			ElementReference = MapReference->ReturnMapBlockReference(RangeY, RangeX);

			if (ElementReference != nullptr) // This can happen.
			{
				mgListIterator<mgLineSegment> ElementShape;

				ElementShape = ElementReference->BlockGeometry();

				while (!ElementShape.IteratorAtEnd())
				{
					mgLineSegment *LineSegRef;
					bool exitLoop = true; // Must be proven false.

					LineSegRef = ElementShape.ReturnElementReference();

					// Now we will attempt to prove that this line cannot possibly be collided with on our current trajectory
					// from our current position. These checks are significantly cheaper than the line intersection check, so
					// while that check is inevitable, we want to as much as possible eliminate using it.

					// Only add the line to this list if it's facing the same direction as the attempted movement, because
					// otherwise a collision should be impossible.
					if (!(AttemptedMovement * LineSegRef->NormalFacingPosition(MovingObject->Position) <= 0))
						continue;

					// If both the points in the line are in the wrong direction of the movement collision is also impossible
					// If only ONE of the exitLoop tests passes, the line is in the same direction as the movement from us
					// and we can't exclude it on that basis alone.
					if ( AttemptedMovement.Y > 0 ) 
					{
						if ( LineSegRef->SegmentStart.Y > MovingObject->Position.Y + MovingObject->ObjectSize && 
							LineSegRef->SegmentEnd.Y > MovingObject->Position.Y + MovingObject->ObjectSize)
							exitLoop = false;

						if ( MovingObject->Position.Y + MovingObject->ObjectSize + AttemptedMovement.Y <
							mgSmallestValue( LineSegRef->SegmentStart.Y, LineSegRef->SegmentEnd.Y ) )
							continue; // It is beyond the range of our movements, don't add it.
					}
					else if ( AttemptedMovement.Y < 0 )
					{
						if ( LineSegRef->SegmentStart.Y < MovingObject->Position.Y - MovingObject->ObjectSize &&
							LineSegRef->SegmentEnd.Y < MovingObject->Position.Y - MovingObject->ObjectSize)
							exitLoop = false;

						if ( MovingObject->Position.Y - MovingObject->ObjectSize + AttemptedMovement.Y >
							mgLargestValue( LineSegRef->SegmentStart.Y, LineSegRef->SegmentEnd.Y ) )
							continue; // It is beyond the range of our movements, don't add it.
					}


					if ( AttemptedMovement.X > 0 )
					{ 
						if ( LineSegRef->SegmentStart.X > MovingObject->Position.X + MovingObject->ObjectSize && 
							LineSegRef->SegmentEnd.X > MovingObject->Position.X + MovingObject->ObjectSize)
							exitLoop = false;

						if ( MovingObject->Position.X + MovingObject->ObjectSize + AttemptedMovement.X <
							mgSmallestValue( LineSegRef->SegmentStart.X, LineSegRef->SegmentEnd.X ) )
							continue; // It is beyond the range of our movements, don't add it.
					}
					else if ( AttemptedMovement.X < 0 )
					{
						if ( LineSegRef->SegmentStart.X < MovingObject->Position.X - MovingObject->ObjectSize &&
							LineSegRef->SegmentEnd.X < MovingObject->Position.X - MovingObject->ObjectSize)
							exitLoop = false;

						if ( MovingObject->Position.X - MovingObject->ObjectSize + AttemptedMovement.X >
							mgLargestValue( LineSegRef->SegmentStart.X, LineSegRef->SegmentEnd.X ) )
							continue; // It is beyond the range of our movements, don't add it.

					}

					if ( exitLoop )
						continue; // None of the past checks were proven true, which means these objects are behind us.

					CollisionLines.AddElementReference(LineSegRef, false);
				}
			} // if (ElementReference != nullptr)
		} // for (int RangeX...
	} // for (int RangeY...
}

// Tests from the Map Object against the world.
void mgCollisionDetection::PerformCollisionTestsP1(void) // Stage Four: Part One
{
	mgListIterator<mgLineSegment> MapObjectShape;
	mgRBTBinaryTree<mgPoint> PointTree;
	mgLinkedList<mgPoint> PointList;

	MapObjectShape = MovingObject->ObjectGeometry();

	DetectedCollisions.ClearList(); // Make sure our list is empty. We need a clean test.

	while (!MapObjectShape.IteratorAtEnd())
	{ // For each line in the shape of our map object..
		mgLineSegment *TestLine;

		TestLine = MapObjectShape.ReturnElementReference();

		// Collect unique points in a line by storing them in a binary tree, which we will then
		// dump to a linked list and process. ( This ensures we only test each point once, our binary tree's
		// don't contain duplicate numbers ).

		PointTree.AddElement(TestLine->SegmentStart);
		PointTree.AddElement(TestLine->SegmentEnd);
	}

	PointTree.NodeToList(PointTree.Root, &PointList); // Convert the Binary Tree to a list.

	mgListIterator<mgPoint> PointListIterator(&PointList);

	while (!PointListIterator.IteratorAtEnd())
	{ // For each point in our object.
		mgLineSegment MovementCollisionLine;
		mgPoint TestPoint;

		TestPoint = PointListIterator.ReturnElement();

		MovementCollisionLine.ImportLine(TestPoint, AttemptedMovement);

		mgListIterator<mgLineSegment> CollisionLinesIterator(&CollisionLines);
		
		while (!CollisionLinesIterator.IteratorAtEnd())
		{ // For each line in the Detection Area
			mgLineCollisionResults TestResults;
			mgLineSegment *LineReference;

			LineReference = CollisionLinesIterator.ReturnElementReference();

			TestResults = MovementCollisionLine.CollisionTest(LineReference);

			if (TestResults.Collision)
			{ // We have a collision.
				mgDetectedCollision Collision;

				Collision.PointOfCollision = TestResults.CollisionPoint;
				Collision.CollisionLine = LineReference;

				// Now we need to figure out the vector that will pull us out of this collision.
				Collision.CollisionCorrection.AutoNormalize = false; // We need the defined magnitude.
				Collision.CollisionCorrection.VectorFromCoord(MovementCollisionLine.SegmentEnd, TestResults.CollisionPoint);
				Collision.CollisionNormal = LineReference->NormalFacingPosition(TestPoint); // Get the normal of the colliding wall.
				Collision.Collision = true;

				DetectedCollisions.AddElement(Collision);
			}
		}
	}
}

// Tests from the world against the mapobject
void mgCollisionDetection::PerformCollisionTestsP2(void) // Stage Four: Part Two
{
	mgListIterator<mgLineSegment> MapObjectShape;
	mgRBTBinaryTree<mgCollisionPoint> PointTree;
	mgLinkedList<mgCollisionPoint> PointList;
	mgVector ReversedMovement = AttemptedMovement;

	ReversedMovement.ReverseDirection(); // Reverse the direction of the vector.

	mgListIterator<mgLineSegment> CollisionLinesIterator(&CollisionLines);

	while (!CollisionLinesIterator.IteratorAtEnd())
	{ // For each line in the shape of our map object..
		mgLineSegment *TestLine;
		mgCollisionPoint a, b;

		TestLine = CollisionLinesIterator.ReturnElementReference();

		// Collect unique points in a line by storing them in a binary tree, which we will then
		// dump to a linked list and process. ( This ensures we only test each point once, our binary tree's
		// don't contain duplicate numbers ).

		a.LineReference = b.LineReference = TestLine;
		a.Position = TestLine->SegmentStart;
		b.Position = TestLine->SegmentEnd;

		PointTree.AddElement(a);
		PointTree.AddElement(b);
	}

	PointTree.NodeToList(PointTree.Root, &PointList); // Convert the Binary Tree to a list.

	mgListIterator<mgCollisionPoint> PointListIterator(&PointList);

	MapObjectShape = MovingObject->ObjectGeometry();

	while (!PointListIterator.IteratorAtEnd())
	{ // For each point in our object.
		mgLineSegment MovementCollisionLine;
		mgCollisionPoint TestCollisionPoint;

		TestCollisionPoint = PointListIterator.ReturnElement();

		MovementCollisionLine.ImportLine(TestCollisionPoint.Position, ReversedMovement);

		MapObjectShape.JumptoStart(); // Back to the front.
		while (!MapObjectShape.IteratorAtEnd())
		{ // For each line in the Detection Area
			mgLineCollisionResults TestResults;
			mgLineSegment *LineReference;

			LineReference = MapObjectShape.ReturnElementReference();

			TestResults = MovementCollisionLine.CollisionTest(LineReference);

			if (TestResults.Collision)
			{ // We have a collision.
				mgDetectedCollision Collision;

				Collision.PointOfCollision = TestResults.CollisionPoint;
				Collision.CollisionLine = TestCollisionPoint.LineReference; // The line that the test point belonged to.

				// Now we need to figure out the vector that will pull us out of this collision.
				Collision.CollisionCorrection.AutoNormalize = false; // We need the defined magnitude.
				Collision.CollisionCorrection.VectorFromCoord(TestResults.CollisionPoint, MovementCollisionLine.SegmentEnd);
				// This one is a bit different because a wall point is colliding with the side of the objects body, so we need the
				// object body's line normal reversed to be our Collision Normal.
				Collision.CollisionNormal = LineReference->NormalFacingPosition(TestCollisionPoint.Position);
				Collision.CollisionNormal.ReverseDirection();
				Collision.Collision = true;

				DetectedCollisions.AddElement(Collision);
			}
		}
	}
}

mgDetectedCollision mgCollisionDetection::CollisionTest(mgMapObject *MovingObject, mgVector Movement, unsigned int Range)
{
	mgDetectedCollision FirstCollision;
	mgListIterator<mgDetectedCollision> DetectedCollisionsIterator;
	
	FirstCollision.Collision = false; // If there wasn't a collision at all.

	// Perform steps One through Four
	this->CollisionSetup(MovingObject, Movement);
	this->SetupDetectionArea(2);
	this->PerformCollisionTestsP1();
	this->PerformCollisionTestsP2();

	// Now we have a list of all of our collisions, if this list is empty, we have no collisions, otherwise we need to know
	// which collision has the biggest CollisionCorrection, as that would be the first impact.
	DetectedCollisionsIterator.LinktoList(&DetectedCollisions);

	while (!DetectedCollisionsIterator.IteratorAtEnd())
	{
		mgDetectedCollision Collision;

		Collision = DetectedCollisionsIterator.ReturnElement();

		if (FirstCollision.Collision == false) // This is the first collision
			FirstCollision = Collision;
		else if (Collision.CollisionCorrection.Magnitude > FirstCollision.CollisionCorrection.Magnitude) // This Collision has a bigger correction.
			FirstCollision = Collision;
	}

	return FirstCollision;
}

mgCollisionDetection::mgCollisionDetection()
{
	MovingObject = nullptr;
	MapReference = nullptr;
}
