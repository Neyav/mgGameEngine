#ifndef MGMAPOBJECTH
#define MGMAPOBJECTH

#include "mgVectorPoint.h"
#include "mgLinkedList.h"
#include "mgLineSegment.h"

// =------------------------------------=
// = mgMapObject C++ class              =
// =------------------------------------=
//
// Base class for handling of map objects, used as a reference point for various elements here.
class mgMapObject
{
public:

	// Information about the map object in the world.
	mgPoint Position;
	mgVector Momentum;
	mgLinkedList<mgLineSegment> *ObjectShape = NULL;

	// cached information to decide if new geometry calculations are needed
	mgPoint GeoPosition;
	double GeoSize;

	// Information defining the map object.
	double ObjectSize = 0; // Used to scale and define the geometry generated

	// Functions for the map object.
	virtual mgLinkedList<mgLineSegment> *ObjectGeometry(void);

	~mgMapObject();
};
#endif