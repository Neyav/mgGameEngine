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
// TEXTURES
SETextureHandler *GameworldFloor = nullptr;
SETextureHandler *GameworldWall = nullptr;
SETextureHandler *MOBJ_PlayerSprite = nullptr;
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
	SEViewDisplay ViewDisplay;	// Responsible for our game camera rendering.
	bool exitApplication = false; 	// True if we are quitting.
	SDL_Event eventHandler; 	// SDL Event handler
	mgMapObject *LocalPlayer = nullptr;

	RenderEngine = new SERenderHandler;
	RenderEngine->InitWindow(SCREENWIDTH,SCREENHEIGHT, "SDL Engine test");

	MOBJList = new mgLinkedList < mgMapObject > ;
	GameworldFloor = new SETextureHandler;
	GameworldWall = new SETextureHandler;
	MOBJ_PlayerSprite = new SETextureHandler;

	GameworldFloor->loadFromFile("SDLenginefloor.png", RenderEngine);
	GameworldWall->loadFromFile("SDLenginewall.png", RenderEngine);
	MOBJ_PlayerSprite->loadFromFile("SDLplayer.png", RenderEngine);

	initGameWorld();
	LocalPlayer = spawnMapObject(1.5, 1.5, MOBJ_PLAYER, 0.25);

	ViewDisplay.Initialize( RenderEngine, GameWorld, MOBJList );

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

		// TODO: THIS SECTION NEEDS TO BE MOVED INTO ITS OWN FILE AND MADE TO APPLY TO ALL MOBILE MOBJS

		mgVector Movement = LocalPlayer->Momentum;	

		// Attempt movement
		while (Movement.Y != 0 || Movement.X != 0) // There is attempted movement.
		{
			// Let's check for collisions first.
			mgCollisionDetection CollisionTest;
			mgDetectedCollision Results;

			CollisionTest.MapReference = GameWorld;
			Results = CollisionTest.CollisionTest(LocalPlayer, Movement, 2);

			if (Results.Collision == false) // No collision
			{
				LocalPlayer->Position.Y += Movement.Y;
				LocalPlayer->Position.X += Movement.X;

				Movement.Y = Movement.X = 0; // We completed the move.
			}
			else
			{	// Handle it.				
				mgVector Projected;
				double dotproduct;

				// Push us 0.0000001 units away from the wall. Moments like this make me regret using
				// double point precision and not a fixed fractional unit size like everyone else does. sigh.
				Results.CollisionCorrection.NormalizeVector(Results.CollisionCorrection.Magnitude + 0.00001);

				// Complete the movement in a manner that doesn't have us clip through the wall.
				Movement = Movement + Results.CollisionCorrection;

				LocalPlayer->Position.Y += Movement.Y;
				LocalPlayer->Position.X += Movement.X;

				Movement = Results.CollisionCorrection; // The remainder of the movement we want to attempt.
				Movement.ReverseDirection(); // We want this to go back into the wall we collided with.

				// Lose all velocity in the direction of the wall normal by projecting the direction onto it.
				dotproduct = Movement * Results.CollisionNormal;
				Projected = Results.CollisionNormal * dotproduct;
				Movement = Movement - Projected;
			}
		}

		// ATROPHY MOMENTUM
		LocalPlayer->Momentum = LocalPlayer->Momentum * 0.9;

		if (LocalPlayer->Momentum.Magnitude < 0.001 )
			LocalPlayer->Momentum.Y = LocalPlayer->Momentum.X = LocalPlayer->Momentum.Magnitude = 0;
	}

	return 0;
}
