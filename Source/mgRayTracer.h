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

	
};

#endif