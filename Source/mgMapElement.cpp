#include <stdlib.h>

#include "mgMapElement.h"
#include "mgShape.h"

void mgMapElement::GenerateBlockGeometry(void)
{
	if (BlockShape != NULL)
		delete BlockShape;

	if (BlockType == MAP_BLOCKFLOOR)
	{
		BlockShape = new mgShape;
	}
	else if (BlockType == MAP_BLOCKWALL)
	{
		BlockShape = new mgShape;
		BlockShape->AddLineToShape({ 0, 0 }, { 0, 1 }, this, NULL);
		BlockShape->AddLineToShape({ 0, 1 }, { 1, 1 }, this, NULL);
		BlockShape->AddLineToShape({ 1, 1 }, { 1, 0 }, this, NULL);
		BlockShape->AddLineToShape({ 1, 0 }, { 0, 0 }, this, NULL);
	}
}

mgShape *mgMapElement::BlockGeometry(void)
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