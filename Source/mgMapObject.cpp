#include <stdlib.h>

#include "mgMapObject.h"
#include "mgLinkedList.h"
#include "mgLineSegment.h"

mgLinkedList<mgLineSegment> *mgMapObject::ObjectGeometry(void)
{
	return NULL; // Our base object has no shape. 
}

mgMapObject::~mgMapObject()
{
	if (ObjectShape != NULL)
	{	// This object generated geometry, delete it.
		ObjectShape->ExplicitPointerCleanup = true; // We are responsible for the lines we generate.

		delete ObjectShape;
	}
}