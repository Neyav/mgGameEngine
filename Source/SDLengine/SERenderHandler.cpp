#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif
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

	if ( success )
	{
		ScreenWidth = Width;
		ScreenHeight = Height;
	}

	return success;
}

bool SERenderHandler::setupViewPort(float startposy, float startposx, float endposy, float endposx)
{
	int starty = (int)(ScreenHeight * startposy);
	int startx = (int)(ScreenWidth * startposx);
	int endy = (int)(ScreenHeight * endposy);
	int endx = (int)(ScreenWidth * endposx);
	SDL_Rect newViewport;
	
	if (startx >= endx)
		return false;

	if (starty >= endy)
		return false;

	newViewport.x = startx;
	newViewport.y = starty;
	newViewport.w = endx - startx;
	newViewport.h = endy - starty;

	SDL_RenderSetViewport( this->Renderer, &newViewport );

	return true;
}

int SERenderHandler::getHeight()
{
	return ScreenHeight;
}

int SERenderHandler::getWidth()
{
	return ScreenWidth;
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

void SERenderHandler::loadTextures(void)
{
	// Game Wall
	this->Texture[TEXTURE_GAME_DEFAULTWALL] = new SETextureHandler(this);
	this->Texture[TEXTURE_GAME_DEFAULTWALL]->loadFromFile("SDLenginewall.png");

	// Game Floor
	this->Texture[TEXTURE_GAME_DEFAULTFLOOR] = new SETextureHandler(this);
	this->Texture[TEXTURE_GAME_DEFAULTFLOOR]->loadFromFile("SDLenginefloor.png");

	// Game Player
	this->Texture[TEXTURE_GAME_PLAYER] = new SETextureHandler(this);
	this->Texture[TEXTURE_GAME_PLAYER]->loadFromFile("SDLplayer.png");
}

void SERenderHandler::clearTextures(void)
{
	for (int x = 0; x < NUMTEXTURES; x++)
		if (this->Texture[x] != nullptr )
		{
			delete this->Texture[x];
			this->Texture[x] = nullptr;
		}
}

SERenderHandler::SERenderHandler()
{
	Renderer = NULL;
	Window = NULL;
	ScreenWidth = ScreenHeight = 0;

	for (int x = 0; x < NUMTEXTURES; x++)
		this->Texture[x] = nullptr;
}

SERenderHandler::~SERenderHandler()
{
	this->clearTextures();

	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
}
