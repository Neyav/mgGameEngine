#ifndef MGMAPELEMENTH
#define MGMAPELEMENTH

#include "mgShape.h"

#define MAP_BLOCKFLOOR 0
#define MAP_BLOCKWALL 1

class mgMapElement
{
private:
	mgShape *BlockShape;

public:
	char BlockType;
	mgPoint Position;

	void GenerateBlockGeometry(void);
	mgShape *BlockGeometry(void);

	mgMapElement();
	~mgMapElement();
};
#endif