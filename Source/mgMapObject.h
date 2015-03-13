#ifndef MGMAPOBJECTH
#define MGMAPOBJECTH

#include "mgVectorPoint.h"

// =------------------------------------=
// = mgMapObject C++ class              =
// =------------------------------------=
//
// Base class for handling of map objects, used as a reference point for various elements here.
class mgMapObject
{
public:

	mgPoint Position;
	mgVector Momentum;

	double ySize;
	double xSize;
};
#endif