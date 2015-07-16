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

	while(1)
	{
		printf("loop\n");
	}
}
