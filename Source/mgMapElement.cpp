#include <stdlib.h>

#include "mgMapElement.h"

void mgMapElement::GenerateBlockGeometry(void)
{
	if (BlockShape != NULL)
		delete BlockShape;

	if (BlockType == MAP_BLOCKFLOOR)
	{
		BlockShape = NULL;
	}
	else if (BlockType == MAP_BLOCKWALL)
	{
		mgLineSegment *ShapeLine;
		mgPoint Start, End;

		BlockShape = new mgLinkedList<mgLineSegment>;

		// Defined shape of an orthogonical blocking wall

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
		BlockShape->AddElementReference(ShapeLine);

		// 0,1 - 1,1
		ShapeLine = new mgLineSegment;
		Start.X++;
		End.Y++;
		ShapeLine->LineSegmentBlock = this;
		ShapeLine->LineSegmentOwner = NULL;
		ShapeLine->Facing = LINEFACE_RIGHT;
		ShapeLine->ImportLine(Start, End);
		BlockShape->AddElementReference(ShapeLine);

		// 1,1 - 1,0
		ShapeLine = new mgLineSegment;
		Start.Y++;
		End.X--;
		ShapeLine->LineSegmentBlock = this;
		ShapeLine->LineSegmentOwner = NULL;
		ShapeLine->Facing = LINEFACE_RIGHT;
		ShapeLine->ImportLine(Start, End);
		BlockShape->AddElementReference(ShapeLine);

		// 1,0 - 0,0
		ShapeLine = new mgLineSegment;
		Start.X--;
		End.Y--;
		ShapeLine->LineSegmentBlock = this;
		ShapeLine->LineSegmentOwner = NULL;
		ShapeLine->Facing = LINEFACE_RIGHT;
		ShapeLine->ImportLine(Start, End);
		BlockShape->AddElementReference(ShapeLine);
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
}

mgMapElement::mgMapElement()
{
	BlockShape = NULL;
}

mgMapElement::~mgMapElement()
{
	// Cleanup code for a map block.
	if (BlockShape != NULL)
	{
		// The map element block is considered responsible for all the lines that belong to it.
		// This flag makes sure the linked list cleans the lines up and frees their memory.
		BlockShape->ExplicitPointerCleanup = true;

		delete BlockShape;
	}
}