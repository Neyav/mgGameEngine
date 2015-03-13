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
		ShapeLine->ImportLine(Start, End);
		BlockShape->AddElementReference(ShapeLine);

		// 0,1 - 1,1
		ShapeLine = new mgLineSegment;
		Start.X++;
		End.Y++;
		ShapeLine->LineSegmentBlock = this;
		ShapeLine->LineSegmentOwner = NULL;
		ShapeLine->ImportLine(Start, End);
		BlockShape->AddElementReference(ShapeLine);

		// 1,1 - 1,0
		ShapeLine = new mgLineSegment;
		Start.Y++;
		End.X--;
		ShapeLine->LineSegmentBlock = this;
		ShapeLine->LineSegmentOwner = NULL;
		ShapeLine->ImportLine(Start, End);
		BlockShape->AddElementReference(ShapeLine);

		// 1,0 - 0,0
		ShapeLine = new mgLineSegment;
		Start.X--;
		End.Y--;
		ShapeLine->LineSegmentBlock = this;
		ShapeLine->LineSegmentOwner = NULL;
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

mgMapElement::mgMapElement()
{
	// Initalization code for a map element.
	BlockType = MAP_BLOCKWALL;
	BlockShape = NULL;
}

mgMapElement::~mgMapElement()
{
	// Cleanup code for a map block.
	if (BlockShape != NULL)
		delete BlockShape;
}