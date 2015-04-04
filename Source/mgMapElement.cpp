#include <stdlib.h>

#include "mgMapElement.h"

void mgMapElement::GenerateBlockGeometry(void)
{
	if (BlockShape != NULL)
		delete BlockShape;

	switch (BlockType)
	{
	case MAP_BLOCKFLOOR: // Floors don't have any shape.
			BlockShape = NULL;
			break;
	case MAP_BLOCKWALL: // Walls have a very simple orthagonical block shape.
			mgLineSegment *ShapeLine;
			mgPoint Start, End;

			BlockShape = new mgLinkedList < mgLineSegment > ;

			// Defined shape of an orthogonical blocking wall
			// true in the AddElementReference call defines this object, this list, as being the one that owns the
			// mgLineSegment pointer. This ownership means that it is the one responsible for deleting it upon cleanup.

			// 0,0 - 0,1
			ShapeLine = new mgLineSegment;
			Start.Y = 0 + Position.Y;
			Start.X = 0 + Position.X;
			End.Y = 0 + Position.Y;
			End.X = 1 + Position.X;
			ShapeLine->LineSegmentBlock = this;
			ShapeLine->LineSegmentOwner = NULL;
			ShapeLine->Facing = LINEFACE_RIGHT;
			ShapeLine->ImportLine(Start, End);
			BlockShape->AddElementReference(ShapeLine, true);

			// 0,1 - 1,1
			ShapeLine = new mgLineSegment;
			Start.X++;
			End.Y++;
			ShapeLine->LineSegmentBlock = this;
			ShapeLine->LineSegmentOwner = NULL;
			ShapeLine->Facing = LINEFACE_RIGHT;
			ShapeLine->ImportLine(Start, End);
			BlockShape->AddElementReference(ShapeLine, true);

			// 1,1 - 1,0
			ShapeLine = new mgLineSegment;
			Start.Y++;
			End.X--;
			ShapeLine->LineSegmentBlock = this;
			ShapeLine->LineSegmentOwner = NULL;
			ShapeLine->Facing = LINEFACE_RIGHT;
			ShapeLine->ImportLine(Start, End);
			BlockShape->AddElementReference(ShapeLine, true);

			// 1,0 - 0,0
			ShapeLine = new mgLineSegment;
			Start.X--;
			End.Y--;
			ShapeLine->LineSegmentBlock = this;
			ShapeLine->LineSegmentOwner = NULL;
			ShapeLine->Facing = LINEFACE_RIGHT;
			ShapeLine->ImportLine(Start, End);
			BlockShape->AddElementReference(ShapeLine, true);
			break;
	}
}

mgLinkedList<mgLineSegment> *mgMapElement::BlockGeometry(void)
{
	if (BlockShape == NULL)
		GenerateBlockGeometry();

	return BlockShape;
}

// As new elements are added to the map element be sure to represent them here if you want them to persist through block copying.
void mgMapElement::operator=(const mgMapElement& other)
{
	BlockType = other.BlockType;
	Position = other.Position;
	for (int x = 0; x < 4; x++)
		Adjacent[x] = other.Adjacent[x];
}

mgMapElement::mgMapElement()
{
	BlockType = MAP_BLOCKWALL;
	BlockShape = NULL;
}

mgMapElement::~mgMapElement()
{
	// Cleanup code for a map block.
	if (BlockShape != NULL)
		delete BlockShape;
}