#include "SERenderHandler.h"
#include "SETextureHandler.h"
#include "SEViewDisplay.h"
#include "GameGlobals.h"
#include "../mgMapDataHandler.h"
#include "../mgMapObject.h"
#include "../mgVectorPoint.h"
#include "../Containers/mgLinkedList.h"

// Almost all of these should be handled better. TODO!!! Make a Texture handler that returns textures based on a reference ID.
// Global Variables
// TEXTURES
extern SETextureHandler *GameworldFloor;
extern SETextureHandler *GameworldWall;
extern SETextureHandler *MOBJ_PlayerSprite;

/************************************************
 *		SEViewDisplay			*
 *	Responsible for setting up the screen   *
 *	and rendering the gameworld upon it.    *
 ************************************************/

void SEViewDisplay::Initialize(SERenderHandler *RenderHandler, mgMapDataHandler *MapDataHandler, mgLinkedList<mgMapObject> *MOBJList)
{
	Renderer = RenderHandler;
	GameWorld = MapDataHandler;
	this->MOBJList = MOBJList;
}

void SEViewDisplay::RenderWorld(mgPoint Position, double zoom)
{
	int SCREENWIDTH = Renderer->getWidth();
	int SCREENHEIGHT = Renderer->getHeight();

	// Setup the tunables based on what we know so we display what we want.
	int TileY = mgFloor(TILEY * zoom);
	int TileX = mgFloor(TILEX * zoom); // Default texture size of a tile

	int WalloffsetY = mgFloor(TILEYOFFSET * zoom);
	int WalloffsetX = mgFloor(TILEXOFFSET * zoom);

	int CenterY = mgFloor(Position.Y);
	int CenterX = mgFloor(Position.X);

	int YPixelOffset = (int)((double)TileY * (double)(Position.Y - CenterY)); // The offset is the size of a tile * the fractional portion of the position.
	int XPixelOffset = (int)((double)TileX * (double)(Position.X - CenterX));

	int TilesY = (int)(SCREENHEIGHT / TileY) + 2;
	int TilesX = (int)(SCREENWIDTH / TileX) + 2; // The number of tiles we're going to render.

	int StartY = CenterY - (int)(TilesY / 2); 
	int StopY = CenterY + (int)(TilesY / 2);
	int StartX = CenterX - (int)(TilesX / 2); 
	int StopX = CenterX + (int)(TilesX / 2);

	int PixelY = (SCREENHEIGHT / 2) - ((CenterY - StartY) * TileY) - YPixelOffset;
	int PixelX = (SCREENWIDTH / 2) - ((CenterX - StartX) * TileX) - XPixelOffset;

	GameworldFloor->setSize(TileY, TileX); // scale texture
	GameworldWall->setSize(TileY + WalloffsetY, TileX + WalloffsetX);

	// Render the floor tiles	
	for (int RenderY = StartY; RenderY <= StopY; RenderY++)
	{
		for (int RenderX = StartX; RenderX <= StopX; RenderX++)
		{
			GameworldFloor->render(PixelY, PixelX);
			PixelX += TileX;
		}
		PixelX = (SCREENWIDTH / 2) - ((CenterX - StartX) * TileX) - XPixelOffset;
		PixelY += TileY;
	}

	PixelY = (SCREENHEIGHT / 2) - ((CenterY - StartY) * TileY) - YPixelOffset;
	PixelX = (SCREENWIDTH / 2) - ((CenterX - StartX) * TileX) - XPixelOffset;

	// Render the wall tiles and any objects
	for (int RenderY = StartY; RenderY <= StopY; RenderY++)
	{
		for (int RenderX = StartX; RenderX <= StopX; RenderX++)
		{
			mgListIterator<mgMapObject> MOBJ_Iterator(MOBJList);

			if (GameWorld->IsBlockClippable(RenderY, RenderX)) // Simple check to see if it's a wall, we should really be doing a check for map element type.
				GameworldWall->render(PixelY - WalloffsetY, PixelX - WalloffsetX);

			// Check our map object list to see if any map objects reside here.
			while (!MOBJ_Iterator.IteratorAtEnd())
			{
				mgMapObject *WorkingObject = MOBJ_Iterator.ReturnElementReference();
				int offsetY = PixelY;
				int offsetX = PixelX;

				if (mgFloor(WorkingObject->Position.Y) == RenderY && mgFloor(WorkingObject->Position.X) == RenderX)
				{  // Our MOBJ is on this block.
					// Determine our render position.
					offsetY += (int)((double)(WorkingObject->Position.Y - (double)RenderY - WorkingObject->ObjectSize) * TileY);
					offsetX += (int)((double)(WorkingObject->Position.X - (double)RenderX - WorkingObject->ObjectSize) * TileX);

					switch (WorkingObject->ObjectType)
					{
					case MOBJ_PLAYER:
						MOBJ_PlayerSprite->setSize(TileY * (WorkingObject->ObjectSize * 2),TileX * (WorkingObject->ObjectSize * 2)); // Scale sprite
						MOBJ_PlayerSprite->render(offsetY, offsetX);
						break;
					default:
						break; // No sprite.
					}
				}
			}

			PixelX += TileX;
		}
		PixelX = (SCREENWIDTH / 2) - ((CenterX - StartX) * TileX) - XPixelOffset;
		PixelY += TileY;
	}
}

SEViewDisplay::SEViewDisplay()
{
	Renderer = nullptr;
	GameWorld = nullptr;
	MOBJList = nullptr;
}
