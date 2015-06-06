#ifndef MGRAYTRACERH
#define MGRAYTRACERH

#include <stdlib.h>

#include "mgMapDataHandler.h"
#include "mgVectorPoint.h"
#include "mgLineSegment.h"
#include "Containers/mgLinkedList.h"

//#warning mgRayTracer to be replaced with mgRayCaster.

struct mgTraceResults
{
	mgLineSegment *ImpactLine;
	mgPoint	ImpactPoint;
	double RayDistance;

	bool CompleteScan; // True if the scan was complete, false if it was incomplete.
};

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
	mgRayTracer();
	~mgRayTracer();

	mgMapDataHandler *MapReference;
	mgLinkedList<mgPoint> PositionsChecked;
	bool ListPositions;

	mgTraceResults OccluderPoint(mgPoint Origin, mgVector Direction);

	// Essentially a rewrite of the raytracing code. OccluderPoint above is going to be depreciated in the near future.
};

#endif
