#ifndef MGMAPELEMENTH
#define MGMAPELEMENTH

#include <stdlib.h>

#include "mgLineSegment.h"
#include "mgLinkedList.h"

#define MAP_BLOCKFLOOR 0
#define MAP_BLOCKWALL 1

class mgMapElement
{
private:
	mgLinkedList<mgLineSegment> *BlockShape;

public:
	char BlockType;
	mgPoint Position;

	void operator=(const mgMapElement& other);

	void GenerateBlockGeometry(void);
	mgLinkedList<mgLineSegment> *BlockGeometry(void);

	mgMapElement();
	~mgMapElement();
};
#endif