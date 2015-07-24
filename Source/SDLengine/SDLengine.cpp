#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <string>

// SDLengine includes
#include "SERenderHandler.h"
#include "SETextureHandler.h"

// mgGameEngine includes
#include "../mgMapDataHandler.h"
#include "../mgRandomMazeGenerator.h"
#include "../mgMapElement.h"
#include "../mgMapObject.h"
#include "../mgVectorPoint.h"
#include "../mgLineSegment.h"

// Screen resolution
int SCREENWIDTH = 1280;
int SCREENHEIGHT = 720;

// Tile size
#define TILEY 128
#define TILEX 128

// Wall offset
#define TILEYOFFSET 28
#define TILEXOFFSET 28

// MapObject Defines
#define MOBJ_PLAYER 1

// Global Variables
SERenderHandler *RenderEngine = nullptr;
mgMapDataHandler *GameWorld = nullptr;
mgLinkedList<mgMapObject> MOBJList;
// TEXTURES
SETextureHandler GameworldFloor;
SETextureHandler GameworldWall;
SETextureHandler MOBJ_PlayerSprite;
// [END] Global Variables

// --- RenderDisplay
// - Renders the game world from Position.
// - Meeeeeeat and perterders
void RenderDisplay ( mgPoint Position, double zoom )
{
	// Setup the tunables based on what we know so we display what we want.
	int TileY = (int)floor(TILEY * zoom);
	int TileX = (int)floor(TILEX * zoom); // Default texture size of a tile

	int WalloffsetY = (int)floor(TILEYOFFSET * zoom);
	int WalloffsetX = (int)floor(TILEXOFFSET * zoom);

	int CenterY = (int) floor(Position.Y);
	int CenterX = (int) floor(Position.X);

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

	GameworldFloor.setSize(TileY, TileX); // scale texture
	GameworldWall.setSize(TileY + WalloffsetY, TileX + WalloffsetX);

	// Render the floor tiles	
	for (int RenderY = StartY; RenderY <= StopY; RenderY++)
	{
		for (int RenderX = StartX; RenderX <= StopX; RenderX++)
		{
			GameworldFloor.render(PixelY, PixelX);
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
			mgListIterator<mgMapObject> MOBJ_Iterator(&MOBJList);

			if (GameWorld->IsBlockClippable(RenderY, RenderX)) // Simple check to see if it's a wall, we should really be doing a check for map element type.
				GameworldWall.render(PixelY - WalloffsetY, PixelX - WalloffsetX);

			// Check our map object list to see if any map objects reside here.
			while (!MOBJ_Iterator.IteratorAtEnd())
			{
				mgMapObject *WorkingObject = MOBJ_Iterator.ReturnElementReference();
				int offsetY = PixelY;
				int offsetX = PixelX;

				if ((int)floor(WorkingObject->Position.Y) == RenderY && (int)floor(WorkingObject->Position.X) == RenderX)
				{  // Our MOBJ is on this block.
					// Determine our render position.
					offsetY += (int)((double)(WorkingObject->Position.Y - (double)RenderY - WorkingObject->ObjectSize) * TileY);
					offsetX += (int)((double)(WorkingObject->Position.X - (double)RenderX - WorkingObject->ObjectSize) * TileX);

					switch (WorkingObject->ObjectType)
					{
					case MOBJ_PLAYER:
						MOBJ_PlayerSprite.setSize(TileY * (WorkingObject->ObjectSize * 2),TileX * (WorkingObject->ObjectSize * 2)); // Scale sprite
						MOBJ_PlayerSprite.render(offsetY, offsetX);
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

mgMapObject *spawnMapObject( double Y, double X, unsigned int Type, double size )
{
	mgMapObject *newObject;

	newObject = new mgMapObject;

	newObject->Position.Y = Y;
	newObject->Position.X = X;
	newObject->ObjectType = Type;
	newObject->ObjectSize = size;

	MOBJList.AddElementReference(newObject, true); // Add our new object to the map object list.

	return newObject;
}

void initGameWorld ( void )
{
	mgRandomMazeGenerator RandomGenerator;
	mgMapObject StartingPlayer;

	if ( GameWorld != nullptr )
		delete GameWorld;

	GameWorld = new mgMapDataHandler; // New game world space.
	GameWorld->InitalizeMapData(50, 50); // Initalize the map

	RandomGenerator.Map = GameWorld;
	RandomGenerator.GenerateMaze(1, 1); // Generate our maze.
}

int main ( void )
{
	bool exitApplication = false; 	// True if we are quitting.
	SDL_Event eventHandler; 	// SDL Event handler
	mgMapObject *LocalPlayer = nullptr;

	RenderEngine = new SERenderHandler;
	RenderEngine->InitWindow(SCREENWIDTH,SCREENHEIGHT, "SDL Engine test");

	GameworldFloor.loadFromFile("SDLenginefloor.png", RenderEngine);
	GameworldWall.loadFromFile("SDLenginewall.png", RenderEngine);
	MOBJ_PlayerSprite.loadFromFile("SDLplayer.png", RenderEngine);

	initGameWorld();
	LocalPlayer = spawnMapObject(1.5, 1.5, MOBJ_PLAYER, 0.25);

	while( !exitApplication )
	{
		RenderEngine->ClearScreen();
		RenderDisplay(LocalPlayer->Position, 1);
		RenderEngine->UpdateScreen();

		while ( SDL_PollEvent( &eventHandler ) != 0 )
		{
			if ( eventHandler.type == SDL_QUIT )
			{ // Quit request
				exitApplication = true;
			}
		}
	}
}
