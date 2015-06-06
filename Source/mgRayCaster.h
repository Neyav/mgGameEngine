#ifndef MGRAYCASTERH
#define MGRAYCASTERH

#include <stdlib.h>

#include "mgMapDataHandler.h"
#include "mgVectorPoint.h"
#include "mgLineSegment.h"
#include "Containers/mgLinkedList.h"

// mgRayCasterState is going to store all internal information relating to the current cast, it is going to be accessable publicly and manipulateable.
// The cast is going to be initalized, then run, if the unresolved motion is solved to zero the cast is considered complete. Upon impact the program
// will need to identify the impacted line to see if it is something to ignore ( adding it to the line ignore list ) or if it is something that needs
// to be picked up, or if it's a wall. If it is a wall we need to use the normal of the wall to manipulate the unresolvedmotion vector so that the next
// run of the trace has us sliding against the wall, unless that is deemed to be impossible and then the motion is considered resolved.
struct mgRayCasterState
{
	mgPoint Position;
	mgVector UnresolvedMotion;
	mgLineSegment *ImpactLine;
	double DistanceTraced;
};

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
	mgLinkedList<mgLineSegment> IgnoredLines;
public:
	mgRayCaster();

	mgRayCasterState CurrentState;

	mgMapDataHandler *MapReference;
};

#endif
