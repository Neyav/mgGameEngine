#ifndef SERENDERHANDLER_H
#define SERENDERHANDLER_H

#include <SDL2/SDL.h>
#include <string>

class SERenderHandler
{
private:
	int ScreenHeight;
	int ScreenWidth;
public:
	SDL_Window *Window;
	SDL_Renderer *Renderer;

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
