#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#include "SERenderHandler.h"
#include "SETextureHandler.h"

int main ( void )
{
	SERenderHandler RenderEngine;

	RenderEngine.InitWindow(800,600, "SDL Engine test");

	SETextureHandler FloorTexture(&RenderEngine); // Cannot be called until after the Renderer has been setup.
	FloorTexture.loadFromFile("SDLenginefloor.png");

	while(1)
	{		
		FloorTexture.render(200,200);
		RenderEngine.UpdateScreen();
	}
}
