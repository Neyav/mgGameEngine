#ifndef MGRAYTRACERH
#define MGRAYTRACERH

#include <stdlib.h>

#include "mgMapDataHandler.h"
#include "mgVectorPoint.h"
#include "mgLineSegment.h"
#include "mgLinkedList.h"

// =------------------------------------=
// = mgRayTracer C++ class              =
// =------------------------------------=
//
class mgRayTracer
{
private:
	mgPoint TracerOrigin;
	mgPoint TracerPosition;
	mgVector TracerDirection;

	mgLinkedList<mgLineSegment> *BuildOccluderLines(mgPoint Position);
public:
	~mgRayTracer();

	mgMapDataHandler *MapReference = NULL;
	mgLinkedList<mgPoint> PositionsChecked;
	bool ListPositions = false;

	// Essentially a read-only list of information from the last scan
	double RayDistance = 0;
	// ---

	mgPoint OccluderPoint(mgPoint Origin, mgVector Direction);

	// Essentially a rewrite of the raytracing code. OccluderPoint above is going to be depreciated in the near future.
	
};

#endif