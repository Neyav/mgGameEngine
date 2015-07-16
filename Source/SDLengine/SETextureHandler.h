#ifndef SETEXTUREHANDLER_H
#define SETEXTUREHANDLER_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class SETextureHandler
{
private:
	SDL_Texture* hwTexture;	// The texture
	int Width, Height;	// Texture dimensions
public:
	bool loadFromFile( std::string filename );		// Loads image
	void free();						// Deallocates texture
	void setColour( Uint8 red, Uint8 green, Uint8 blue );	// Colour modulation
	void setBlendMode( SDL_BlendMode blending );		// Blending
	void setAlpha( Uint8 alpha );				// Alpha modulation
	void render( int x, int y, SDL_Rect* clip = NULL );	// Renders texture

	// Gets image dimensions
	int getWidth();
	int getHeight();

	SETextureHandler();
	~SETextureHandler();
};
#endif
