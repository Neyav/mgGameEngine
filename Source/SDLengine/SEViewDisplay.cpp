#include "SERenderHandler.h"
#include "SETextureHandler.h"
#include "SEViewDisplay.h"
#include "GameGlobals.h"
#include "../mgMapDataHandler.h"
#include "../mgMapObject.h"
#include "../mgVectorPoint.h"
#include "../Containers/mgLinkedList.h"

/*class SEViewDisplay
{
	private:
	SERenderHandler *Renderer;
	mgMapDataHandler *GameWorld;

	public:

	void Initialize(SERenderHandler *RenderHandler, mgMapDataHandler *MapDataHandler);
	void setupView(int starty, int startx, int endy, int endx);

	SEViewDisplay();
}*/

// Almost all of these should be handled better. TODO!!! Make a Texture handler that returns textures based on a reference ID.
// Global Variables
extern mgLinkedList<mgMapObject> MOBJList;
// TEXTURES
extern SETextureHandler GameworldFloor;
extern SETextureHandler GameworldWall;
extern SETextureHandler MOBJ_PlayerSprite;

/************************************************
 *		SEViewDisplay			*
 *	Responsible for setting up the screen   *
 *	and rendering the gameworld upon it.    *
 ************************************************/

void SEViewDisplay::setupView(int starty, int startx, int endy, int endx)
{
	// PUT CODES HERES
}

void SEViewDisplay::Initialize(SERenderHandler *RenderHandler, mgMapDataHandler *MapDataHandler)
{
	Renderer = RenderHandler;
	GameWorld = MapDataHandler;
}

SEViewDisplay::SEViewDisplay()
{
	Renderer = nullptr;
	GameWorld = nullptr;
}
