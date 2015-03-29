#ifndef MGMAPOBJECTPLAYERH
#define MGMAPOBJECTPLAYERH

#include "mgMapObject.h"
#include "mgVectorPoint.h"
#include "mgLinkedList.h"
#include "mgLineSegment.h"

#define MGPLAYERSIZE 0.2 // Size of our player object.

// =------------------------------------=
// = mgMapObjectPlayer C++ class        =
// =------------------------------------=
//
// This class represents a playable object in our map world.
class mgMapObjectPlayer : public mgMapObject
{
public:
	mgLinkedList<mgLineSegment> *ObjectGeometry(void);

	mgMapObjectPlayer();
	~mgMapObjectPlayer();
};

#endif