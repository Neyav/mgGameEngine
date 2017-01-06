#ifndef SEVIEWDISPLAY_H
#define SEVIEWDISPLAY_H

#include "SERenderHandler.h"
#include "SETextureHandler.h"
#include "../mgMapDataHandler.h"
#include "../mgMapObject.h"
#include "../mgVectorPoint.h"
#include "../Containers/mgLinkedList.h"

// Define our maximum sized viewspace.
// Rendering errors will occur if we zoom out so far that we exceed this, so we need to tailor it to our uses.
// Smaller is more efficent.
#define MAXTILESY 30
#define MAXTILESX 30

struct SEViewDisplayContext
{
	mgPoint Position;		// Last calculated position.
	double zoom;			// Last calculated zoom.
	int ScreenWidth;		// Screen Width in pixels.
	int ScreenHeight;		// Screen Height in pixels.
	int TileSizeY;			// Size of a tile in pixels.
	int TileSizeX;
	int WalloffsetY;		// Offset of a wall tile in pixels.
	int WalloffsetX;
	int CenterY;			// Y tile that represents the center of the screen.
	int CenterX;			// X tile that represents the center of the screen.
	int PositionOffsetY;	// Offset of all tiles in pixels based on camera position inside a tile.
	int PositionOffsetX;
	int TilesY;				// Number of tiles that are going to be rendered.
	int TilesX;
	int StartY;				// Y tile to start rendering
	int StartX;				// X tile to start rendering
	int StopY;				// Y tile to stop rendering
	int StopX;				// X tile to stop rendering
};

class SEViewDisplay
{
	private:
	SERenderHandler *Renderer;
	mgMapDataHandler *GameWorld;

	SEViewDisplayContext ViewContext;

	mgLinkedList<mgMapObject> *MOBJList;

	public:

	void Initialize(SERenderHandler *RenderHandler, mgMapDataHandler *MapDataHandler, mgLinkedList<mgMapObject> *MOBJList);
	void SetupViewContext(mgPoint Position, double zoom);
	void RenderWorld(mgPoint Position, double zoom);

	SEViewDisplay();
};

#endif
