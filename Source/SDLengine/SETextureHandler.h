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

class SETextureHandler
{
private:
	SERenderHandler *Renderer; // This textures renderer.

	SDL_Texture *hwTexture;	// The texture
	int Width, Height;	// Texture dimensions
public:
	bool loadFromFile( std::string filename, SERenderHandler *TextureRenderer );	// Loads image
	void free();									// Deallocates texture
	void setColour( Uint8 red, Uint8 green, Uint8 blue );				// Colour modulation
	void setBlendMode( SDL_BlendMode blending );					// Blending
	void setAlpha( Uint8 alpha );							// Alpha modulation
	void setSize(int Width, int Height);						// Size of image
	void render( int y, int x, SDL_Rect* clip = NULL );				// Renders texture

	// Gets image dimensions
	int getWidth();
	int getHeight();

	SETextureHandler();
	~SETextureHandler();
};
#endif
