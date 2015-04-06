#ifndef MGMAPELEMENTH
#define MGMAPELEMENTH

#include <stdlib.h>

#include "mgLineSegment.h"
#include "mgLinkedList.h"

#define MAP_BLOCKFLOOR 0
#define MAP_BLOCKWALL 1

#define ELEMENT_NORTH 0
#define ELEMENT_EAST 1
#define ELEMENT_SOUTH 2
#define ELEMENT_WEST 3

class mgMapElement
{
public: // Add a member to clear the blockshape.
//private:
	mgLinkedList<mgLineSegment> *BlockShape;

//public:
	unsigned char BlockType;
	mgMapElement *Adjacent[4];
	mgPoint Position;

	void operator=(const mgMapElement& other);

	void GenerateBlockGeometry(void);
	mgLinkedList<mgLineSegment> *BlockGeometry(void);

	mgMapElement();
	~mgMapElement();
};
#endif
