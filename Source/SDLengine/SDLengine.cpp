#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string>

// SDLengine includes
#include "SERenderHandler.h"
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
int SCREENWIDTH = 1920;
int SCREENHEIGHT = 1080;

// Global Variables
SERenderHandler *RenderEngine = nullptr;
mgMapDataHandler *GameWorld = nullptr;
mgLinkedList<mgMapObject> MOBJList;
// TEXTURES
SETextureHandler GameworldFloor;
SETextureHandler GameworldWall;
SETextureHandler MOBJ_PlayerSprite;
// [END] Global Variables

// --- RenderDisplay
// - Renders the game world from Position.
// - Meeeeeeat and perterders
void RenderDisplay ( mgPoint Position, double zoom )
{
	// Setup the tunables based on what we know so we display what we want.
	int TileY = mgFloor(TILEY * zoom);
	int TileX = mgFloor(TILEX * zoom); // Default texture size of a tile

	int WalloffsetY = mgFloor(TILEYOFFSET * zoom);
	int WalloffsetX = mgFloor(TILEXOFFSET * zoom);

	int CenterY = mgFloor(Position.Y);
	int CenterX = mgFloor(Position.X);

	int YPixelOffset = (int)((double)TileY * (double)(Position.Y - CenterY)); // The offset is the size of a tile * the fractional portion of the position.
	int XPixelOffset = (int)((double)TileX * (double)(Position.X - CenterX));

	int TilesY = (int)(SCREENHEIGHT / TileY) + 2;
	int TilesX = (int)(SCREENWIDTH / TileX) + 2; // The number of tiles we're going to render.

	int StartY = CenterY - (int)(TilesY / 2); 
	int StopY = CenterY + (int)(TilesY / 2);
	int StartX = CenterX - (int)(TilesX / 2); 
	int StopX = CenterX + (int)(TilesX / 2);

	int PixelY = (SCREENHEIGHT / 2) - ((CenterY - StartY) * TileY) - YPixelOffset;
	int PixelX = (SCREENWIDTH / 2) - ((CenterX - StartX) * TileX) - XPixelOffset;

	GameworldFloor.setSize(TileY, TileX); // scale texture
	GameworldWall.setSize(TileY + WalloffsetY, TileX + WalloffsetX);

	// Render the floor tiles	
	for (int RenderY = StartY; RenderY <= StopY; RenderY++)
	{
		for (int RenderX = StartX; RenderX <= StopX; RenderX++)
		{
			GameworldFloor.render(PixelY, PixelX);
			PixelX += TileX;
		}
		PixelX = (SCREENWIDTH / 2) - ((CenterX - StartX) * TileX) - XPixelOffset;
		PixelY += TileY;
	}

	PixelY = (SCREENHEIGHT / 2) - ((CenterY - StartY) * TileY) - YPixelOffset;
	PixelX = (SCREENWIDTH / 2) - ((CenterX - StartX) * TileX) - XPixelOffset;

	// Render the wall tiles and any objects
	for (int RenderY = StartY; RenderY <= StopY; RenderY++)
	{
		for (int RenderX = StartX; RenderX <= StopX; RenderX++)
		{
			mgListIterator<mgMapObject> MOBJ_Iterator(&MOBJList);

			if (GameWorld->IsBlockClippable(RenderY, RenderX)) // Simple check to see if it's a wall, we should really be doing a check for map element type.
				GameworldWall.render(PixelY - WalloffsetY, PixelX - WalloffsetX);

			// Check our map object list to see if any map objects reside here.
			while (!MOBJ_Iterator.IteratorAtEnd())
			{
				mgMapObject *WorkingObject = MOBJ_Iterator.ReturnElementReference();
				int offsetY = PixelY;
				int offsetX = PixelX;

				if (mgFloor(WorkingObject->Position.Y) == RenderY && mgFloor(WorkingObject->Position.X) == RenderX)
				{  // Our MOBJ is on this block.
					// Determine our render position.
					offsetY += (int)((double)(WorkingObject->Position.Y - (double)RenderY - WorkingObject->ObjectSize) * TileY);
					offsetX += (int)((double)(WorkingObject->Position.X - (double)RenderX - WorkingObject->ObjectSize) * TileX);

					switch (WorkingObject->ObjectType)
					{
					case MOBJ_PLAYER:
						MOBJ_PlayerSprite.setSize(TileY * (WorkingObject->ObjectSize * 2),TileX * (WorkingObject->ObjectSize * 2)); // Scale sprite
						MOBJ_PlayerSprite.render(offsetY, offsetX);
						break;
					default:
						break; // No sprite.
					}
				}
			}

			PixelX += TileX;
		}
		PixelX = (SCREENWIDTH / 2) - ((CenterX - StartX) * TileX) - XPixelOffset;
		PixelY += TileY;
	}
}

mgMapObject *spawnMapObject( double Y, double X, unsigned int Type, double size )
{
	mgMapObject *newObject;

	newObject = new mgMapObject;

	newObject->Position.Y = Y;
	newObject->Position.X = X;
	newObject->ObjectType = Type;
	newObject->ObjectSize = size;

	MOBJList.AddElementReference(newObject, true); // Add our new object to the map object list.

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

int main ( void )
{
	bool exitApplication = false; 	// True if we are quitting.
	SDL_Event eventHandler; 	// SDL Event handler
	mgMapObject *LocalPlayer = nullptr;

	RenderEngine = new SERenderHandler;
	RenderEngine->InitWindow(SCREENWIDTH,SCREENHEIGHT, "SDL Engine test");

	GameworldFloor.loadFromFile("SDLenginefloor.png", RenderEngine);
	GameworldWall.loadFromFile("SDLenginewall.png", RenderEngine);
	MOBJ_PlayerSprite.loadFromFile("SDLplayer.png", RenderEngine);

	initGameWorld();
	LocalPlayer = spawnMapObject(1.5, 1.5, MOBJ_PLAYER, 0.25);

	while( !exitApplication )
	{
		RenderEngine->ClearScreen();
		RenderDisplay(LocalPlayer->Position, 1);
		RenderEngine->UpdateScreen();

		while ( SDL_PollEvent( &eventHandler ) != 0 )
		{
			if ( eventHandler.type == SDL_QUIT )
			{ // Quit request
				exitApplication = true;
			}
		}

		const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
		mgVector MovementDirection;

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
}
