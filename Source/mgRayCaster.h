#ifndef MGRAYCASTERH
#define MGRAYCASTERH

#include <stdlib.h>

#include "mgMapDataHandler.h"
#include "mgVectorPoint.h"
#include "mgLineSegment.h"
#include "mgLinkedList.h"

//
// =------------------------------------=
// = mgRayCaster C++ class              =
// =------------------------------------=
//
// This class functions as a method for testing two points ( with or without bounding boxes ) for any intersections between them
// and if appropriate manage a change in momentum, pass colliding object back for pick up or ignore, and determine the final
// resting position of the point so that it does not violate game object collision rules with other objects or walls.

// A decision was made a while ago that the mgRayTracer class didn't meet the needs of the project and that a full rewrite is
// appropriate. This is just a skeleton class for future work on a proper tracer.

class mgRayCaster
{
private:

public:

	mgMapDataHandler *MapReference;
};

#endif
