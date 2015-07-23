#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <stdio.h>

#include "SETextureHandler.h"
#include "SERenderHandler.h"

bool SETextureHandler::loadFromFile( std::string filename, SERenderHandler *TextureRenderer )
{
	free(); // Clean up before we start.

	Renderer = TextureRenderer;

	SDL_Texture *newTexture = NULL;

	SDL_Surface *loadedSurface = IMG_Load( filename.c_str() );
	if ( loadedSurface == NULL ) // NULL because this library is a C library
	{
		printf("Unable to load image %s -> [SDL_image error: %s]\n", filename.c_str(), IMG_GetError() );
	}
	else
	{
		// Colour key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		// Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface( Renderer->Renderer, loadedSurface );
		if ( newTexture == NULL )
		{
			printf("Unable to create texture from %s -> [SDL Error: %s]\n", filename.c_str(), SDL_GetError() );
		}
		else
		{
			Width = loadedSurface->w;
			Height = loadedSurface->h;
		}

		SDL_FreeSurface( loadedSurface );
	}

	hwTexture = newTexture;
	return hwTexture != NULL;
}

void SETextureHandler::free()
{
	// Free texture if it exists
	if ( hwTexture != NULL )
	{
		SDL_DestroyTexture( hwTexture );
		hwTexture = NULL;
		Width = Height = 0;
	}
}

void SETextureHandler::setColour( Uint8 red, Uint8 green, Uint8 blue )
{
	SDL_SetTextureColorMod( hwTexture, red, green, blue );
}

void SETextureHandler::setBlendMode( SDL_BlendMode blending )
{
	SDL_SetTextureBlendMode( hwTexture, blending );
}

void SETextureHandler::setAlpha( Uint8 alpha )
{
	SDL_SetTextureAlphaMod( hwTexture, alpha );
}

// Size override incase you want to scale an image to a specific size.
void SETextureHandler::setSize( int Width, int Height )
{
	this->Width = Width;
	this->Height = Height;
}

void SETextureHandler::render( int x, int y, SDL_Rect* clip )
{
	// Set Rendering space and render to screen
	SDL_Rect renderQuad = {x, y, Width, Height };

	// Set clip rendering dimensions
	if ( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	// Render to screen
	SDL_RenderCopy( Renderer->Renderer, hwTexture, clip, &renderQuad );
}

int SETextureHandler::getWidth()
{
	return Width;
}

int SETextureHandler::getHeight()
{
	return Height;
}

SETextureHandler::SETextureHandler()
{
	// Init
	hwTexture = NULL;
	Width = Height = 0;
}

SETextureHandler::~SETextureHandler()
{
	free();
}
