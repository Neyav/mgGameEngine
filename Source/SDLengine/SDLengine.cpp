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

	int PixelY = (SCREENHEIGHT / 2) - ((CenterY - StartY) * TileY) - YPixelOffset;
	int PixelX = (SCREENWIDTH / 2) - ((CenterX - StartX) * TileX) - XPixelOffset;

	GameworldFloor.setSize(TileY, TileX); // scale texture

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
}

int main ( void )
{
	int i = 0;
	mgPoint Position;
	bool zoomup = true;
	double zoom = 1;

	Position.Y = 5.5;
	Position.X = 4.3;
	
	RenderEngine = new SERenderHandler;
	RenderEngine->InitWindow(SCREENWIDTH,SCREENHEIGHT, "SDL Engine test");

	GameworldFloor.loadFromFile("SDLenginefloor.png", RenderEngine);
	GameworldWall.loadFromFile("SDLenginewall.png", RenderEngine);

	while(1)
	{
		Position.Y += 0.01;
		Position.X += 0.03;

		if (zoomup)
		{
			zoom += 0.01;

			if (zoom > 2)
				zoomup = false;
		}
		else
		{
			zoom -= 0.1;

			if (zoom < 0.5)
				zoomup = true;
		}

		RenderEngine->ClearScreen();
		RenderDisplay(Position, zoom);
		RenderEngine->UpdateScreen();
	}
}
