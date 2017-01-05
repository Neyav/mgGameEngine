#ifndef SERENDERHANDLER_H
#define SERENDERHANDLER_H

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <string>

#include "SETextureHandler.h"
class SETextureHandler;

// Texture array definitions
#define NUMTEXTURES 3			// Number of textures.
#define TEXTURE_GAME_DEFAULTWALL 0
#define TEXTURE_GAME_DEFAULTFLOOR 1
#define TEXTURE_GAME_PLAYER 2

class SERenderHandler
{
private:
	int ScreenHeight;
	int ScreenWidth;
public:
	SDL_Window *Window;
	SDL_Renderer *Renderer;

	// Texture array
	SETextureHandler *Texture[NUMTEXTURES];	// Adjust size of array as needed.
	void loadTextures(void);	// Load all textures. If this becomes too burdensome, we can load them on use
					// and make sure to clean them up when appropriate.
	void clearTextures(void);	// Clean up all textures.

	bool InitWindow(int Width, int Height, std::string Title);

	bool setupViewPort(float startposy, float startposx, float endposy, float endposx); // All values in percentages
	int getHeight();
	int getWidth();

	void ClearScreen(void);
	void UpdateScreen(void);

	SERenderHandler();
	~SERenderHandler();
};

#endif
