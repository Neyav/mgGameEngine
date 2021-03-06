#ifndef SETEXTUREHANDLER_H
#define SETEXTUREHANDLER_H

#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif
#include <string>

#include "SERenderHandler.h"
class SERenderHandler;

class SETextureHandler
{
private:
	SERenderHandler *Renderer; // This textures renderer.

	SDL_Texture *hwTexture;	// The texture
	int Width, Height;	// Texture dimensions
public:
	bool loadFromFile( std::string filename );					// Loads image
	void free();												// Deallocates texture
	void setColour( Uint8 red, Uint8 green, Uint8 blue );		// Colour modulation
	void setBlendMode( SDL_BlendMode blending );				// Blending
	void setAlpha( Uint8 alpha );								// Alpha modulation
	void setSize(int Width, int Height);						// Size of image
	void render( int y, int x, SDL_Rect* clip = NULL );			// Renders texture
	void renderExt(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	// Gets image dimensions
	int getWidth();
	int getHeight();

	SETextureHandler(SERenderHandler *TextureRenderer)
	{
		// init
		hwTexture = NULL;
		Width = Height = 0;
		Renderer = TextureRenderer;
	}
	~SETextureHandler();
};
#endif
