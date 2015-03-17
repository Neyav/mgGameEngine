#ifndef MGMAPOBJECTPLAYERH
#define MGMAPOBJECTPLAYERH

#include "mgMapObject.h"
#include "mgVectorPoint.h"
#include "mgLinkedList.h"
#include "mgLineSegment.h"

// =------------------------------------=
// = mgMapObjectPlayer C++ class        =
// =------------------------------------=
//
// This class represents a playable object in our map world.
class mgMapObjectPlayer : public mgMapObject
{
public:
	double ObjectSize = 0.2;

	mgLinkedList<mgLineSegment> *ObjectGeometry(void);
};

#endif