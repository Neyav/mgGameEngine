#include <stdlib.h>

#include "mgMapObjectPlayer.h"
#include "mgMapObject.h"
#include "mgLinkedList.h"
#include "mgVectorPoint.h"
#include "mgLineSegment.h"

mgLinkedList<mgLineSegment> *mgMapObjectPlayer::ObjectGeometry(void)
{
	mgLineSegment *LineSegmentReference = NULL;
	mgPoint LineStart, LineEnd;

	// First check to see if any geometry has been generated, if it hasn't generate it.
	// If the geometry has been generated, then move it around our origin point.
	if (ObjectShape == NULL)
	{
		GeoPosition = Position;
		GeoSize = ObjectSize;

		ObjectShape = new mgLinkedList < mgLineSegment > ;

		// 0, 0 -> 0, 1
		LineSegmentReference = new mgLineSegment;
		LineStart.Y = Position.Y - ObjectSize;
		LineStart.X = Position.X - ObjectSize;
		LineEnd.Y = LineStart.Y;
		LineEnd.X = Position.X + ObjectSize;
		LineSegmentReference->ImportLine(LineStart, LineEnd);
		LineSegmentReference->LineSegmentOwner = this;
		LineSegmentReference->Description = LINEDES_TOP;
		LineSegmentReference->Facing = LINEFACE_RIGHT;
		ObjectShape->AddElementReference(LineSegmentReference, true);

		// 0, 1 -> 1, 1
		LineSegmentReference = new mgLineSegment;
		LineStart.X += ObjectSize * 2;
		LineEnd.Y += ObjectSize * 2;
		LineSegmentReference->ImportLine(LineStart, LineEnd);
		LineSegmentReference->LineSegmentOwner = this;
		LineSegmentReference->Description = LINEDES_RIGHTSIDE;
		LineSegmentReference->Facing = LINEFACE_RIGHT;
		ObjectShape->AddElementReference(LineSegmentReference, true);

		// 1, 1 -> 1, 0
		LineSegmentReference = new mgLineSegment;
		LineStart.Y += ObjectSize * 2;
		LineEnd.X -= ObjectSize * 2;
		LineSegmentReference->ImportLine(LineStart, LineEnd);
		LineSegmentReference->LineSegmentOwner = this;
		LineSegmentReference->Description = LINEDES_BOTTOM;
		LineSegmentReference->Facing = LINEFACE_RIGHT;
		ObjectShape->AddElementReference(LineSegmentReference, true);

		// 1, 0 -> 0, 0
		LineSegmentReference = new mgLineSegment;
		LineStart.X -= ObjectSize * 2;
		LineEnd.Y -= ObjectSize * 2;
		LineSegmentReference->ImportLine(LineStart, LineEnd);
		LineSegmentReference->LineSegmentOwner = this;
		LineSegmentReference->Description = LINEDES_LEFTSIDE;
		LineSegmentReference->Facing = LINEFACE_RIGHT;
		ObjectShape->AddElementReference(LineSegmentReference, true);
	}
	else if (GeoPosition.Y != Position.Y || GeoPosition.X != Position.X || GeoSize != ObjectSize)
	{	// We already have geometry but we need to manipulate it to our new position and size.
		ObjectShape->ResetIterator(); // Make sure we're at the beginning of the list.

		GeoPosition = Position;
		GeoSize = ObjectSize;

		LineSegmentReference = ObjectShape->ReturnElementReference();
		while (LineSegmentReference != NULL)
		{
			if (LineSegmentReference->Description == LINEDES_TOP)
			{ // 0, 0 -> 0, 1
				LineStart.Y = Position.Y - ObjectSize;
				LineStart.X = Position.X - ObjectSize;
				LineEnd.Y = LineStart.Y;
				LineEnd.X = Position.X + ObjectSize;
			}
			else if (LineSegmentReference->Description == LINEDES_RIGHTSIDE)
			{ // 0, 1 -> 1, 1
				LineStart.Y = Position.Y - ObjectSize;
				LineStart.X = Position.X + ObjectSize;
				LineEnd.Y = Position.Y + ObjectSize;
				LineEnd.X = Position.X + ObjectSize;
			}
			else if (LineSegmentReference->Description == LINEDES_BOTTOM)
			{ // 1, 1 -> 1, 0
				LineStart.Y = Position.Y + ObjectSize;
				LineStart.X = Position.X + ObjectSize;
				LineEnd.Y = Position.Y + ObjectSize;
				LineEnd.X = Position.X - ObjectSize;
			}
			else if (LineSegmentReference->Description == LINEDES_LEFTSIDE)
			{ // 1, 0 -> 0, 0
				LineStart.Y = Position.Y + ObjectSize;
				LineStart.X = Position.X - ObjectSize;
				LineEnd.Y = Position.Y - ObjectSize;
				LineEnd.X = Position.X - ObjectSize;
			}
			LineSegmentReference->ImportLine(LineStart, LineEnd);

			LineSegmentReference = ObjectShape->ReturnElementReference();
		}
	}

	// Just like the gentlemen before us we make sure we replace the toilet paper roll after we've used the last of it because
	// without such distingiushed care for our fellow person democracy will crumble.
	ObjectShape->ResetIterator();

	return ObjectShape;
}

mgMapObjectPlayer::mgMapObjectPlayer()
{
	ObjectShape = NULL;
	ObjectSize = 0;
}

mgMapObjectPlayer::~mgMapObjectPlayer()
{
	if (ObjectShape != NULL)
		delete ObjectShape;
}