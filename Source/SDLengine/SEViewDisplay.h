#ifndef SEVIEWDISPLAY_H
#define SEVIEWDISPLAY_H

#include "SERenderHandler.h"
#include "../mgMapDataHandler.h"
#include "../mgMapObject.h"
#include "../mgVectorPoint.h"
#include "../Containers/mgLinkedList.h"

class SEViewDisplay
{
	private:
	SERenderHandler *Renderer;
	mgMapDataHandler *GameWorld;

	mgLinkedList<mgMapObject> *MOBJList;

	public:

	void Initialize(SERenderHandler *RenderHandler, mgMapDataHandler *MapDataHandler, mgLinkedList<mgMapObject> *MOBJList);
	void RenderWorld(mgPoint Position, double zoom);

	SEViewDisplay();
};

#endif
