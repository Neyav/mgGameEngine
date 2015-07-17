#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#include "SERenderHandler.h"
#include "SETextureHandler.h"

int main ( void )
{
	SERenderHandler RenderEngine;
	int i = 0;

	RenderEngine.InitWindow(1024,748, "SDL Engine test");

	SETextureHandler FloorTexture(&RenderEngine); // Cannot be called until after the Renderer has been setup.
	SETextureHandler WallTexture(&RenderEngine);
	FloorTexture.loadFromFile("SDLenginefloor.png");
	WallTexture.loadFromFile("SDLenginewall.png");

	while(1)
	{
		RenderEngine.ClearScreen();
		i++;
		if (i > 700)
			i = 0;	
		FloorTexture.render(i,i);
		WallTexture.render(700-i,700-i);
		RenderEngine.UpdateScreen();
	}
}
