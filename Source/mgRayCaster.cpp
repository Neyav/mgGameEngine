#include <math.h>
#include <stdlib.h>
#include <iostream>

#include "mgRayCaster.h"
#include "mgLineSegment.h"
#include "mgMapDataHandler.h"
#include "mgVectorPoint.h"
#include "Containers/mgLinkedList.h"

//
// =------------------------------------=
// = mgRayCaster C++ class              =
// =------------------------------------=
//
// This class functions as a method for testing two points ( with or without bounding boxes ) for any intersections between them
// and if appropriate manage a change in momentum, pass colliding object back for pick up or ignore, and determine the final
// resting position of the point so that it does not violate game object collision rules with other objects or walls.

mgRayCaster::mgRayCaster()
{
	MapReference = nullptr;

	CurrentState.DistanceTraced = 0;
	CurrentState.ImpactLine = nullptr;
}