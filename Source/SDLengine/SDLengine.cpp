#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
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

int main ( void )
{
	int i = 0;
	
	RenderEngine = new SERenderHandler;
	RenderEngine->InitWindow(SCREENWIDTH,SCREENHEIGHT, "SDL Engine test");

	SETextureHandler FloorTexture(RenderEngine); // Cannot be called until after the Renderer has been setup.
	SETextureHandler WallTexture(RenderEngine);
	FloorTexture.loadFromFile("SDLenginefloor.png");
	WallTexture.loadFromFile("SDLenginewall.png");

	while(1)
	{
		RenderEngine->ClearScreen();
		i++;
		if (i > 700)
			i = 0;	
		FloorTexture.render(i,i);
		WallTexture.render(700-i,700-i);
		RenderEngine->UpdateScreen();
	}
}
