#ifndef SEMOVEMENTHANDLER_H
#define SEMOVEMENTHANDLER_H

#include "../mgMapDataHandler.h"
#include "../mgMapObject.h"
#include "../Containers/mgLinkedList.h"

class SEMovementHandler
{
private:
	mgMapDataHandler *GameWorld;
	mgLinkedList<mgMapObject> *MOBJList;

public:

	void Initialize(mgMapDataHandler *Map, mgLinkedList<mgMapObject> *MOBJList);
	void ProcessMomentum(void);

	SEMovementHandler();
};

#endif
