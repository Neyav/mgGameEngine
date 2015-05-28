#include <stdlib.h>

#include "mgMapElement.h"

void mgMapElement::SetBlockType(BlockDefine NewType)
{
	BlockType = NewType;
	if (BlockShape != nullptr) // If we have a shape it probably isn't valid anymore so delete it.
	{
		delete BlockShape;
		BlockShape = nullptr;
	}
}

void mgMapElement::GenerateBlockGeometry(void)
{
	// A few variables for generating shapes.
	mgPoint Start, End;

	if (BlockShape != nullptr)
		delete BlockShape;

	switch (BlockType)
	{
	case MAP_BLOCKFLOOR: // Floors don't have any shape.
			BlockShape = nullptr;
			break;
	case MAP_BLOCKWALL: // Walls have a very simple orthagonical block shape.
			BlockShape = new mgLinkedList < mgLineSegment > ;

			// Defined shape of an orthogonical blocking wall
			// true in the AddElementReference call defines this object, this list, as being the one that owns the
			// mgLineSegment pointer. This ownership means that it is the one responsible for deleting it upon cleanup.

			// 0,0 - 0,1
			BlockShape->AddElementReference(mgDefineLine(0, 0, 0, 1, Position, LINEFACE_RIGHT, this, nullptr), true);
			
			// 0,1 - 1,1
			BlockShape->AddElementReference(mgDefineLine(0, 1, 1, 1, Position, LINEFACE_RIGHT, this, nullptr), true);

			// 1,1 - 1,0
			BlockShape->AddElementReference(mgDefineLine(1, 1, 1, 0, Position, LINEFACE_RIGHT, this, nullptr), true);

			// 1,0 - 0,0
			BlockShape->AddElementReference(mgDefineLine(1, 0, 0, 0, Position, LINEFACE_RIGHT, this, nullptr), true);

			break;
	case MAP_BLOCKCORNER: // A more extravagant shape for shits and giggles.
			BlockShape = new mgLinkedList < mgLineSegment >;

			BlockShape->AddElementReference(mgDefineLine(0, 0, 0.2, 0.5, Position, LINEFACE_RIGHT, this, nullptr), true);

			BlockShape->AddElementReference(mgDefineLine(0.2, 0.5, 0, 1, Position, LINEFACE_RIGHT, this, nullptr), true);

			BlockShape->AddElementReference(mgDefineLine(0, 1, 1, 1, Position, LINEFACE_RIGHT, this, nullptr), true);

			BlockShape->AddElementReference(mgDefineLine(1, 1, 0.5, 0.3, Position, LINEFACE_RIGHT, this, nullptr), true);

			BlockShape->AddElementReference(mgDefineLine(0.5, 0.3, 1, 0, Position, LINEFACE_RIGHT, this, nullptr), true);

			BlockShape->AddElementReference(mgDefineLine(1, 0, 0, 0, Position, LINEFACE_RIGHT, this, nullptr), true);
		
			break;
	}
}

mgLinkedList<mgLineSegment> *mgMapElement::BlockGeometry(void)
{
	if (BlockShape == nullptr)
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
	BlockShape = nullptr;
}

mgMapElement::~mgMapElement()
{
	// Cleanup code for a map block.
	if (BlockShape != nullptr)
		delete BlockShape;
}
