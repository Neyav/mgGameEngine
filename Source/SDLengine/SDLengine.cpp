#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#include "SERenderHandler.h"
#include "SETextureHandler.h"

int SCREENWIDTH = 1280;
int SCREENHEIGHT = 720;

int tileswide = 8;

int tilewidth = SCREENWIDTH / tileswide;
int tileheight = SCREENHEIGHT / tileswide;

int main ( void )
{
	SERenderHandler RenderEngine;
	int i = 0;

	RenderEngine.InitWindow(SCREENWIDTH,SCREENHEIGHT, "SDL Engine test");

	SETextureHandler FloorTexture(&RenderEngine); // Cannot be called until after the Renderer has been setup.
	SETextureHandler WallTexture(&RenderEngine);
	FloorTexture.loadFromFile("SDLenginefloor.png");
	WallTexture.loadFromFile("SDLenginewall.png");

	FloorTexture.setSize(tilewidth, tileheight);

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
