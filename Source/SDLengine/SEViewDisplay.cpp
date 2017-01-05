#include "SERenderHandler.h"
#include "SETextureHandler.h"
#include "SEViewDisplay.h"
#include "GameGlobals.h"
#include "../mgMapDataHandler.h"
#include "../mgMapObject.h"
#include "../mgVectorPoint.h"
#include "../Containers/mgLinkedList.h"

/************************************************
 *		SEViewDisplay                   *
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

	// Clip based on our maximum boundries.
	if (TilesY > MAXTILESY)
		TilesY = MAXTILESY;
	if (TilesX > MAXTILESX)
		TilesX = MAXTILESX;

	int StartY = CenterY - (int)(TilesY / 2); 
	int StopY = CenterY + (int)(TilesY / 2);
	int StartX = CenterX - (int)(TilesX / 2); 
	int StopX = CenterX + (int)(TilesX / 2);

	int PixelY = (SCREENHEIGHT / 2) - ((CenterY - StartY) * TileY) - YPixelOffset;
	int PixelX = (SCREENWIDTH / 2) - ((CenterX - StartX) * TileX) - XPixelOffset;

	mgLinkedList<mgMapObject> VisibleElements[MAXTILESY][MAXTILESX];

	{
		// Go through the global map object list and fill our visible array with map objects in screen space.
		mgListIterator<mgMapObject> MOBJ_Iterator(MOBJList);
		while (!MOBJ_Iterator.IteratorAtEnd())
		{
			mgMapObject *WorkingObject = MOBJ_Iterator.ReturnElementReference();
			int PositionY = mgFloor(WorkingObject->Position.Y);
			int PositionX = mgFloor(WorkingObject->Position.X);

			if (PositionY >= StartY && PositionY <= StopY)
			{
				if (PositionX >= StartX && PositionX <= StopX)
				{
					// Object is in visible scope, add it to our array.
					VisibleElements[PositionY - StartY][PositionX - StartX].AddElementReference(WorkingObject, false);
				}
			}
		}
	}

	// Scale textures	
	Renderer->Texture[TEXTURE_GAME_DEFAULTFLOOR]->setSize(TileY, TileX);
	Renderer->Texture[TEXTURE_GAME_DEFAULTWALL]->setSize(TileY + WalloffsetY, TileX + WalloffsetX);

	// Render the floor tiles	
	for (int RenderY = StartY; RenderY <= StopY; RenderY++)
	{
		for (int RenderX = StartX; RenderX <= StopX; RenderX++)
		{
			Renderer->Texture[TEXTURE_GAME_DEFAULTFLOOR]->render(PixelY, PixelX);
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
			mgListIterator<mgMapObject> MOBJ_Iterator;
			MOBJ_Iterator.LinktoList(&VisibleElements[RenderY - StartY][RenderX - StartX]);

			if (GameWorld->IsBlockClippable(RenderY, RenderX)) // Simple check to see if it's a wall, we should really be doing a check for map element type.
				Renderer->Texture[TEXTURE_GAME_DEFAULTWALL]->render(PixelY - WalloffsetY, PixelX - WalloffsetX);				

			// Check our map object list to see if any map objects reside here.
			while (!MOBJ_Iterator.IteratorAtEnd())
			{
				mgMapObject *WorkingObject = MOBJ_Iterator.ReturnElementReference();
				int offsetY = PixelY;
				int offsetX = PixelX;

				// Determine our render position.
				offsetY += (int)((double)(WorkingObject->Position.Y - (double)RenderY - WorkingObject->ObjectSize) * TileY);
				offsetX += (int)((double)(WorkingObject->Position.X - (double)RenderX - WorkingObject->ObjectSize) * TileX);

				switch (WorkingObject->ObjectType)
				{
				case MOBJ_PLAYER:
					Renderer->Texture[TEXTURE_GAME_PLAYER]->setSize(TileY * (WorkingObject->ObjectSize * 2),TileX * (WorkingObject->ObjectSize * 2)); // Scale sprite
					Renderer->Texture[TEXTURE_GAME_PLAYER]->render(offsetY, offsetX);
					break;
				default:
					break; // No sprite.
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
