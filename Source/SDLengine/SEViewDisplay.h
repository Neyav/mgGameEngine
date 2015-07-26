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

	public:

	void Initialize(SERenderHandler *RenderHandler, mgMapDataHandler *MapDataHandler);
	void setupView(int starty, int startx, int endy, int endx);

	SEViewDisplay();
}

#endif
