#include <stdlib.h>

#include "mgMapObject.h"
#include "mgLinkedList.h"
#include "mgLineSegment.h"

mgLinkedList<mgLineSegment> *mgMapObject::ObjectGeometry(void)
{
	return NULL; // Our base object has no shape. 
}

mgMapObject::mgMapObject()
{
	ObjectShape = NULL;
	ObjectSize = 0;
}

mgMapObject::~mgMapObject()
{
	if (ObjectShape != NULL)
		delete ObjectShape;
}