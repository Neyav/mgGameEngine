#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string> 

// SDLengine includes
#include "SERenderHandler.h"
#include "SEViewDisplay.h"
#include "SETextureHandler.h"
#include "SEMovementHandler.h"
#include "SEShadowEngine.h"
#include "GameGlobals.h"

// mgGameEngine includes
#include "../mgMapDataHandler.h"
#include "../mgRandomMazeGenerator.h"
#include "../mgMapElement.h"
#include "../mgMapObject.h"
#include "../mgVectorPoint.h"
#include "../mgLineSegment.h"
#include "../mgCollisionDetection.h"
#include "../Containers/mgLinkedList.h"

// Screen resolution
int SCREENWIDTH = 1280;
int SCREENHEIGHT = 720;

// Global Variables
SERenderHandler *RenderEngine = nullptr;
mgMapDataHandler *GameWorld = nullptr;
mgLinkedList<mgMapObject> *MOBJList = nullptr;
// [END] Global Variables

mgMapObject *spawnMapObject( double Y, double X, unsigned int Type, double size )
{
	mgMapObject *newObject;

	newObject = new mgMapObject;

	newObject->Position.Y = Y;
	newObject->Position.X = X;
	newObject->ObjectType = Type;
	newObject->ObjectSize = size;

	MOBJList->AddElementReference(newObject, true); // Add our new object to the map object list.

	return newObject;
}

void initGameWorld ( void )
{
	mgRandomMazeGenerator RandomGenerator;
	mgMapObject StartingPlayer;

	if ( GameWorld != nullptr )
		delete GameWorld;

	GameWorld = new mgMapDataHandler; // New game world space.
	GameWorld->InitalizeMapData(50, 50); // Initalize the map

	RandomGenerator.Map = GameWorld;
	RandomGenerator.GenerateMaze(1, 1); // Generate our maze.
}

int main(int argc, char *argv[])
{
	// SDLengine class declarations
	SEViewDisplay ViewDisplay;		// Responsible for our game camera rendering.
	SEMovementHandler MovementHandler;	// Responsible for processing momentum of all Map Objects.
	// ----------------------------
	bool exitApplication = false; 	// True if we are quitting.
	SDL_Event eventHandler; 	// SDL Event handler
	mgMapObject *LocalPlayer = nullptr;

	// Initialize mgGameEngine.
	MOBJList = new mgLinkedList < mgMapObject > ;

	initGameWorld();
	LocalPlayer = spawnMapObject(1.5, 1.5, MOBJ_PLAYER, 0.25);

	// Initialize SDLengine classes
	RenderEngine = new SERenderHandler;
	RenderEngine->InitWindow(SCREENWIDTH,SCREENHEIGHT, __mgVersion);

	ViewDisplay.Initialize( RenderEngine, GameWorld, MOBJList );
	ViewDisplay.InitializeShadowEngine(); // Initalize the shadow engine.
	MovementHandler.Initialize ( GameWorld, MOBJList );

	RenderEngine->loadTextures();

	while( !exitApplication )
	{
		mgVector MovementDirection;

		RenderEngine->ClearScreen();
		ViewDisplay.RenderWorld( LocalPlayer->Position, 1 );		
		RenderEngine->UpdateScreen();

		while ( SDL_PollEvent( &eventHandler ) != 0 )
		{
			if ( eventHandler.type == SDL_QUIT )
			{ // Quit request
				exitApplication = true;
			}
		}

		const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );

		// -- Mouse movement
		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			int y, x;
			SDL_GetMouseState( &x, &y );

			y -= RenderEngine->getHeight() / 2;
			x -= RenderEngine->getWidth() / 2;

			MovementDirection.Y = y;
			MovementDirection.X = x;
		}

		// -- Keyboard movement
		if( currentKeyStates[ SDL_SCANCODE_UP ] )
		{
			MovementDirection.Y = -1;
		}
		else if ( currentKeyStates[ SDL_SCANCODE_DOWN ] )
		{
			MovementDirection.Y = 1;
		}

		if ( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
		{
			MovementDirection.X = 1;
		}
		else if ( currentKeyStates[ SDL_SCANCODE_LEFT ] )
		{
			MovementDirection.X = -1;
		}

		// Add momentum to player
		LocalPlayer->AddMomentum ( 0.15, MovementDirection );

		MovementHandler.ProcessMomentum(); // Process MOBJList momentum and handle collisions.		
	}

	return 0;
}
