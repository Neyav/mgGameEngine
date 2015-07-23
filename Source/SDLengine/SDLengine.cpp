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
#include "../mgMapElement.h"
#include "../mgMapObject.h"
#include "../mgVectorPoint.h"
#include "../mgLineSegment.h"

int SCREENWIDTH = 1280;
int SCREENHEIGHT = 720;

SERenderHandler *RenderEngine = nullptr;
mgMapDataHandler *GameWorld = nullptr;

// TEXTURES
SETextureHandler GameworldFloor;
SETextureHandler GameworldWall;
// [END] TEXTURES

// --- RenderDisplay
// - Renders the game world from Position.
// - Meeeeeeat and perterders
void RenderDisplay ( mgPoint Position, double zoom )
{
	// Setup the tunables based on what we know so we display what we want.
	int TileY = (int)floor(150 * zoom);
	int TileX = (int)floor(150 * zoom); // Default texture size of a tile

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

	int PixelY = (CenterY * TileY + YPixelOffset) - ((CenterY - StartY) * TileY);
	int PixelX = (CenterX * TileX + XPixelOffset) - ((CenterX - StartX) * TileX);

	// Render the floor tiles	
	for (int RenderY = StartY; RenderY <= StopY; RenderY++)
	{
		for (int RenderX = StartX; RenderX <= StopX; RenderX++)
		{
			GameworldFloor.render(PixelY, PixelX);
			PixelX += TileX;
		}
		PixelX = (CenterX * TileX + XPixelOffset) - ((CenterX - StartX) * TileX);
		PixelY += TileY;
	}
}

int main ( void )
{
	int i = 0;
	
	RenderEngine = new SERenderHandler;
	RenderEngine->InitWindow(SCREENWIDTH,SCREENHEIGHT, "SDL Engine test");

	GameworldFloor.loadFromFile("SDLenginefloor.png", RenderEngine);
	GameworldWall.loadFromFile("SDLenginewall.png", RenderEngine);

	while(1)
	{
		mgPoint Position;
		Position.Y = 5.5;
		Position.X = 4.3;

		RenderEngine->ClearScreen();
		RenderDisplay(Position, 1);
		RenderEngine->UpdateScreen();
	}
}
