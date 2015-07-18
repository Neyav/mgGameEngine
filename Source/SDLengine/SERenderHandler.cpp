#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <stdio.h>

#include "SERenderHandler.h"

bool SERenderHandler::InitWindow(int Width, int Height, std::string Title)
{
	bool success = true;

	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! -> [SDL Error: %s]\n", SDL_GetError() );
		success = false;
	}
	else
	{
		// Linear texture filtering
		if ( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		// Create the window
		Window = SDL_CreateWindow( Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Width, Height, SDL_WINDOW_SHOWN );
		if ( Window == NULL )
		{
			printf("Window could not be created! -> [SDL Error: %s]\n", SDL_GetError() );

			success = false;
		}
		else
		{
			// Create vsynced renderer for window
			Renderer = SDL_CreateRenderer( Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

			if ( Renderer == NULL )
			{
				printf("Renderer could not be created! -> [SDL Error: %s]\n", SDL_GetError() );

				success = false;
			}
			else
			{
				// Initialize renderer colour
				SDL_SetRenderDrawColor( Renderer, 0xFF, 0xFF, 0xFF, 0xFF );

				// Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;

				if ( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf("SDL_image could not initialize! -> [SDL_image Error: %s]\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

void SERenderHandler::ClearScreen(void)
{
	SDL_SetRenderDrawColor( Renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( Renderer );
}

void SERenderHandler::UpdateScreen(void)
{
	SDL_RenderPresent( Renderer );
}

SERenderHandler::SERenderHandler()
{
	Renderer = NULL;
	Window = NULL;
}

SERenderHandler::~SERenderHandler()
{
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
}