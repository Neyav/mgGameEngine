#include <stdlib.h>

#include "mgMapObject.h"
#include "mgLinkedList.h"
#include "mgLineSegment.h"

mgLinkedList<mgLineSegment> *mgMapObject::ObjectGeometry(void)
{
	mgLineSegment *LineSegmentReference = NULL;
	mgPoint LineStart, LineEnd;

	// First check to see if any geometry has been generated, if it hasn't generate it.
	// If the geometry has been generated, then move it around our origin point.
	if (ObjectShape == NULL)
	{
		GeoPosition = Position;
		GeoSize = ObjectSize;

		ObjectShape = new mgLinkedList < mgLineSegment >;

		// 0, 0 -> 0, 1
		ObjectShape->AddElementReference(mgDefineLine( -ObjectSize, -ObjectSize, -ObjectSize, ObjectSize, Position, LINEFACE_RIGHT, NULL, this), true);

		// 0, 1 -> 1, 1
		ObjectShape->AddElementReference(mgDefineLine( -ObjectSize, ObjectSize, ObjectSize, ObjectSize, Position, LINEFACE_RIGHT, NULL, this), true);

		// 1, 1 -> 1, 0
		ObjectShape->AddElementReference(mgDefineLine( ObjectSize, ObjectSize, ObjectSize, -ObjectSize, Position, LINEFACE_RIGHT, NULL, this), true);

		// 1, 0 -> 0, 0
		ObjectShape->AddElementReference(mgDefineLine( ObjectSize, -ObjectSize, -ObjectSize, -ObjectSize, Position, LINEFACE_RIGHT, NULL, this), true);
	}
	else if (GeoPosition.Y != Position.Y || GeoPosition.X != Position.X || GeoSize != ObjectSize)
	{	// We already have geometry but we need to manipulate it to our new position and size.
		ObjectShape->ClearList();

		GeoPosition = Position;
		GeoSize = ObjectSize;

		// 0, 0 -> 0, 1
		ObjectShape->AddElementReference(mgDefineLine( -ObjectSize, -ObjectSize, -ObjectSize, ObjectSize, Position, LINEFACE_RIGHT, NULL, this), true);

		// 0, 1 -> 1, 1
		ObjectShape->AddElementReference(mgDefineLine( -ObjectSize, ObjectSize, ObjectSize, ObjectSize, Position, LINEFACE_RIGHT, NULL, this), true);

		// 1, 1 -> 1, 0
		ObjectShape->AddElementReference(mgDefineLine( ObjectSize, ObjectSize, ObjectSize, -ObjectSize, Position, LINEFACE_RIGHT, NULL, this), true);

		// 1, 0 -> 0, 0
		ObjectShape->AddElementReference(mgDefineLine( ObjectSize, -ObjectSize, -ObjectSize, -ObjectSize, Position, LINEFACE_RIGHT, NULL, this), true);
	}

	return ObjectShape;
}

mgMapObject::mgMapObject()
{
	ObjectShape = NULL;
	ObjectSize = DEFAULTOBJECTSIZE;
}

mgMapObject::~mgMapObject()
{
	if (ObjectShape != NULL)
		delete ObjectShape;
}
