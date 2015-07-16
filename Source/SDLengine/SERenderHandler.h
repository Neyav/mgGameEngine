#ifndef SERENDERHANDLER_H
#define SERENDERHANDLER_H

#include <SDL2/SDL.h>
#include <string>

class SERenderHandler
{
public:
	SDL_Window *Window;
	SDL_Renderer *Renderer;

	bool InitWindow(int Width, int Height, std::string Title);

	void ClearScreen(void);
	void UpdateScreen(void);

	SERenderHandler();
	~SERenderHandler();
};

#endif
