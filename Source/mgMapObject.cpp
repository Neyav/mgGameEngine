#include <stdlib.h>

#include "mgMapObject.h"
#include "Containers/mgLinkedList.h"
#include "mgLineSegment.h"

mgListIterator<mgLineSegment> mgMapObject::ObjectGeometry(void)
{
	mgLineSegment *LineSegmentReference = nullptr;
	mgPoint LineStart, LineEnd;
	mgListIterator<mgLineSegment> ListIterator;

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

	ListIterator.LinktoList(ObjectShape);

	return ListIterator;
}

void mgMapObject::AddMomentum ( double speed, mgVector Direction )
{
	double canMove;

	if (Direction.Y == 0 && Direction.X == 0)
		return; // No movement.

	Direction.NormalizeVector( speed );
	Direction = Direction - this->Momentum;

	canMove = Direction.Magnitude;

	if ( canMove > 0.15 ) // Maximum movement speed.
		canMove = 0.15;

	this->Momentum = this->Momentum + Direction * canMove;
}

mgMapObject::mgMapObject()
{
	ObjectShape = nullptr;
	GeoSize = ObjectSize = DEFAULTOBJECTSIZE;
	ObjectType = MOBJ_UNDEFINED;

	this->Momentum.AutoNormalize = false; // Momentum is not normalized.
	this->Momentum.Magnitude = 0;
}

mgMapObject::~mgMapObject()
{
	if (ObjectShape != nullptr)
		delete ObjectShape;
}
