#ifndef MGMAPOBJECTH
#define MGMAPOBJECTH

#include "mgVectorPoint.h"
#include "Containers/mgLinkedList.h"
#include "mgLineSegment.h"

#define DEFAULTOBJECTSIZE 0.2

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
	mgLinkedList<mgLineSegment> *ObjectShape;

	// cached information to decide if new geometry calculations are needed
	mgPoint GeoPosition;
	double GeoSize;

	// Information defining the map object.
	double ObjectSize; // Used to scale and define the geometry generated

	// Functions for the map object.
	mgLinkedList<mgLineSegment> *ObjectGeometry(void);

	mgMapObject();
	~mgMapObject();
};
#endif