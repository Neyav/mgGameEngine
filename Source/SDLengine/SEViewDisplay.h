#ifndef SEVIEWDISPLAY_H
#define SEVIEWDISPLAY_H

#include "SERenderHandler.h"
#include "../mgMapDataHandler.h"
#include "../mgMapObject.h"
#include "../mgVectorPoint.h"
#include "../Containers/mgLinkedList.h"

// Define our maximum sized viewspace.
// Rendering errors will occur if we zoom out so far that we exceed this, so we need to tailor it to our uses.
// Smaller is more efficent.
#define MAXTILESY 30
#define MAXTILESX 30

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
