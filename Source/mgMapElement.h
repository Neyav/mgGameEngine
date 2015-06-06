#ifndef MGMAPELEMENTH
#define MGMAPELEMENTH

#include <stdlib.h>

#include "mgLineSegment.h"
#include "Containers/mgLinkedList.h"

#define BlockDefine unsigned char
#define MAP_BLOCKFLOOR 0
#define MAP_BLOCKWALL 1
#define MAP_BLOCKCORNER 2 // Just for testing purposes.

#define ELEMENT_NORTH 0
#define ELEMENT_EAST 1
#define ELEMENT_SOUTH 2
#define ELEMENT_WEST 3

class mgMapElement
{
private:
	mgLinkedList<mgLineSegment> *BlockShape;

public:	
	BlockDefine BlockType; // Make private!!!!

	mgMapElement *Adjacent[4];
	mgPoint Position;

	void operator=(const mgMapElement& other);

	void SetBlockType(BlockDefine NewType);
	void GenerateBlockGeometry(void);
	mgLinkedList<mgLineSegment> *BlockGeometry(void);

	mgMapElement();
	~mgMapElement();
};
#endif
