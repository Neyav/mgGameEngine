#include <stdlib.h>

#include "mgMapObject.h"
#include "Containers/mgLinkedList.h"
#include "mgLineSegment.h"

mgLinkedList<mgLineSegment> *mgMapObject::ObjectGeometry(void)
{
	mgLineSegment *LineSegmentReference = nullptr;
	mgPoint LineStart, LineEnd;

	// First check to see if any geometry has been generated, if it hasn't generate it.
	// If the geometry has been generated, then move it around our origin point.
	if (ObjectShape == nullptr)
	{
		GeoPosition = Position;
		GeoSize = ObjectSize;

		ObjectShape = new mgLinkedList < mgLineSegment >;

		// 0, 0 -> 0, 1
		ObjectShape->AddElementReference(mgDefineLine( -ObjectSize, -ObjectSize, -ObjectSize, ObjectSize, Position, LINEFACE_RIGHT, nullptr, this), true);

		// 0, 1 -> 1, 1
		ObjectShape->AddElementReference(mgDefineLine( -ObjectSize, ObjectSize, ObjectSize, ObjectSize, Position, LINEFACE_RIGHT, nullptr, this), true);

		// 1, 1 -> 1, 0
		ObjectShape->AddElementReference(mgDefineLine( ObjectSize, ObjectSize, ObjectSize, -ObjectSize, Position, LINEFACE_RIGHT, nullptr, this), true);

		// 1, 0 -> 0, 0
		ObjectShape->AddElementReference(mgDefineLine( ObjectSize, -ObjectSize, -ObjectSize, -ObjectSize, Position, LINEFACE_RIGHT, nullptr, this), true);
	}
	else if (GeoPosition.Y != Position.Y || GeoPosition.X != Position.X || GeoSize != ObjectSize)
	{	// We already have geometry but we need to manipulate it to our new position and size.
		ObjectShape->ClearList();

		GeoPosition = Position;
		GeoSize = ObjectSize;

		// 0, 0 -> 0, 1
		ObjectShape->AddElementReference(mgDefineLine( -ObjectSize, -ObjectSize, -ObjectSize, ObjectSize, Position, LINEFACE_RIGHT, nullptr, this), true);

		// 0, 1 -> 1, 1
		ObjectShape->AddElementReference(mgDefineLine( -ObjectSize, ObjectSize, ObjectSize, ObjectSize, Position, LINEFACE_RIGHT, nullptr, this), true);

		// 1, 1 -> 1, 0
		ObjectShape->AddElementReference(mgDefineLine( ObjectSize, ObjectSize, ObjectSize, -ObjectSize, Position, LINEFACE_RIGHT, nullptr, this), true);

		// 1, 0 -> 0, 0
		ObjectShape->AddElementReference(mgDefineLine( ObjectSize, -ObjectSize, -ObjectSize, -ObjectSize, Position, LINEFACE_RIGHT, nullptr, this), true);
	}

	ObjectShape->ResetIterator(); // We presume this list is going to be used, and so we want it at the beginning.

	return ObjectShape;
}

mgMapObject::mgMapObject()
{
	ObjectShape = nullptr;
	ObjectSize = DEFAULTOBJECTSIZE;
}

mgMapObject::~mgMapObject()
{
	if (ObjectShape != nullptr)
		delete ObjectShape;
}
