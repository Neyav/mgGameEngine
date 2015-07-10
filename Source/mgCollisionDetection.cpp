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


// Setup the collision detection
void mgCollisionDetection::CollisionSetup(mgMapObject *MovingObject, mgVector Movement) // Stage One
{
	this->MovingObject = MovingObject;
	RemainingMovement = Movement;

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

		for (int LineIterator = 0; LineIterator < ElementShape->NumberOfElements(); LineIterator++)
		{
			mgLineSegment *LineSegRef;

			LineSegRef = ElementShape->ReturnElementReference();

			CollisionLines.AddElementReference(LineSegRef, false);
		}
	}
}

mgCollisionDetection::mgCollisionDetection()
{
	MovingObject = nullptr;
	MapReference = nullptr;
}
