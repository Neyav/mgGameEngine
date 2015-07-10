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
}

mgCollisionDetection::mgCollisionDetection()
{
	MovingObject = nullptr;
	MapReference = nullptr;
}
