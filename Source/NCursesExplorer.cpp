// =-------------------------------------------------------------------------------------------------=
// = NCursesExplorer -> This file was mostly written in nano on FreeBSD 10.1. Not an ideal setup, I  =
// =                    am aware, but I wanted to play around with it. It is nice to get interactive =
// =                    results from this project finally, and also to have such a tool for          =
// =                    diagnostic purposes. Excuse code messiness. It also shows how easy this      =
// =                    engine is to program for, definitely an intended result. Happy. :)           =
// =-------------------------------------------------------------------------------------------------=

#include <ncurses.h>
#include <stdlib.h>
#include <math.h>
#include "mgVectorPoint.h"
#include "mgMapDataHandler.h"
#include "mgMapObject.h"
#include "mgCollisionDetection.h"
#include "mgRayTracer.h"
#include "mgRandomMazeGenerator.h"

#define COL_WALLS 1
#define COL_FLOORCEIL 2
#define COL_SEPERATE 3
#define COL_MAP 4

#define MAPSIZEY 25
#define MAPSIZEX 25

double FOV = 100; // Default field of view

int main(void)
{
	bool ExitProgram = false;

	mgMapDataHandler World; // Gameworld.
	mgRandomMazeGenerator Generator; // Random number generator.
	mgRayTracer RenderTracer; // Our tracer.
	mgMapObject Player; // Us

	// Make a 50x50 world by default.
	World.InitalizeMapData(MAPSIZEY, MAPSIZEX);

	// Generate a random maze in the world.
	Generator.Map = &World;
	Generator.GenerateMaze(1,1);

	RenderTracer.MapReference = &World;

	int ViewAngle = 90;

	// Setup our player
	Player.Position.Y = 1.5;
	Player.Position.X = 1.5;
	Player.ObjectSize = 0.3;

	initscr();
	noecho();
	curs_set(false);
	start_color();

	init_pair(COL_WALLS, COLOR_WHITE, COLOR_BLACK);
	init_pair(COL_FLOORCEIL, COLOR_WHITE, COLOR_BLACK);
	init_pair(COL_SEPERATE, COLOR_RED, COLOR_WHITE);
	init_pair(COL_MAP, COLOR_WHITE, COLOR_BLUE);
	attron(COLOR_PAIR(COL_WALLS));

	while(!ExitProgram)
	{ // Render our viewport
		int MaxY, MaxX;
		mgTraceResults ColumnDepthMap[500]; // If your terminal is bigger...

		if (ViewAngle >= 360)
			ViewAngle -= 360;

		if (ViewAngle < 0)
			ViewAngle += 360;

		getmaxyx(stdscr, MaxY, MaxX);
		clear();

		double DegreesPerColumn = FOV / (double)MaxX;

		for (int RenderColumn = 0; RenderColumn < MaxX; RenderColumn++)
		{	// Shoot a ray for each column to determine if we hit a wall and how far away it is.
			mgVector RenderVector;
			mgTraceResults TraceResults;

			double AngleDifference = (DegreesPerColumn * (double)RenderColumn) - (FOV / 2);
			double NewAngle = ViewAngle + AngleDifference;

			RenderVector.VectorFromDegrees(NewAngle);

			TraceResults = RenderTracer.OccluderPoint(Player.Position, RenderVector);

			// Necessary to convert to a flat view plane so we don't get fish eye view.
			TraceResults.RayDistance = TraceResults.RayDistance * cos(AngleDifference * ((double)mgPI / (double)180));

			ColumnDepthMap[RenderColumn] = TraceResults;
		}

		attron(COLOR_PAIR(COL_WALLS));

		// Render here
		// I didn't nest these loops with indents so I could view them on the local terminal screen
		// on my PC while X.Org and KDE compiled/installed... Retro.... :D
		for (int RenderRow = 0; RenderRow < MaxY; RenderRow++)
		{
		for (int RenderColumn = 0; RenderColumn < MaxX; RenderColumn++)
		{
			double Height = (double)MaxY / ColumnDepthMap[RenderColumn].RayDistance;
			double Top = ((double)MaxY - Height) / 2;
			double Bottom = (double)MaxY - (((double)MaxY - Height) / 2);

			move(RenderRow, RenderColumn);

			if (!ColumnDepthMap[RenderColumn].CompleteScan)
			{ // An incomplete scan means the results are indeterminate.
			  // This can be triggered easily by walking out of the map
			  // boundaries.
				addch(' ');
				continue;
			}

			if (RenderRow > round(Top) && RenderRow < round(Bottom))
			{
				// Determine how much of this line is facing
				// you and adjust "shading" to match.
				mgVector ViewVector;
				double DotProduct;

				// Bold walls closer than 2.5 blocks away.
				if (ColumnDepthMap[RenderColumn].RayDistance > 2.5)
					attroff(A_BOLD);
				else
					attron(A_BOLD);

				if (RenderColumn < MaxX && ColumnDepthMap[RenderColumn].ImpactLine != ColumnDepthMap[RenderColumn + 1].ImpactLine)
				{ // Adjacent Line is different, draw a straight line up and down
					attron(COLOR_PAIR(COL_SEPERATE));
					addch(179);
				}
				else
				{ // Draw surface
				ViewVector.VectorFromDegrees(ViewAngle);
				DotProduct = ViewVector * ColumnDepthMap[RenderColumn].ImpactLine->NormalFacingPosition(Player.Position);

				// The DotProduct represents the proportionate facing of two Vectors
				// -1 means they are facing each other. 1 They are looking in the same direction.
				// These values took experimenting to get right
				attron(COLOR_PAIR(COL_WALLS));
				if (DotProduct > -0.05)
					addch(176);
				else if (DotProduct > -0.1)
					addch(177);
				else if (DotProduct > -0.25)
					addch(178);
				else
					addch(219);
				}
			}
			else
			{
				attron(COLOR_PAIR(COL_FLOORCEIL));
				attroff(A_BOLD);
				addch(' ');
			}
		}
		}

		attron(COLOR_PAIR(COL_MAP));
		attron(A_BOLD);

		// Display a "minimap"
		int ScreenY = 0;
		for (int MMY = floor(Player.Position.Y) - 3; MMY < floor(Player.Position.Y + 4); MMY++)
		{
			int ScreenX = 0;
			for (int MMX = floor(Player.Position.X) - 3; MMX < floor(Player.Position.X + 4); MMX++)
			{
				if (MMY == floor(Player.Position.Y) && MMX == floor(Player.Position.X))
					mvprintw(ScreenY, ScreenX, "[YOU]");
				else if (World.IsBlockClippable(MMY, MMX))
					mvprintw(ScreenY, ScreenX, "[XXX]");
				else
					mvprintw(ScreenY, ScreenX, "[   ]");
				ScreenX += 5;
			}
			ScreenY++;
		}

		refresh();

		mgVector Movement; // Just incase we need to move
		mgMapElement *MapBlock; // Incase we want to change a tile

		Movement.Magnitude = 0;

		switch (getch())
		{
		case 68: // D/d for turn RIGHT
		case 100:
			ViewAngle += 10;
			break;
		case 65: // A/a for turn LEFT
		case 97:
			ViewAngle -= 10;
			break;
		case 87: // W/w for Forward
		case 119:
			Movement.Magnitude = 0.2; // Our movement "speed"
			Movement.VectorFromDegrees(ViewAngle);
			
			break;
		case 83: // D/d for Backwards
		case 115:
			Movement.Magnitude = -0.2; // Backwards "speed"
			Movement.VectorFromDegrees(ViewAngle);

			break;
		case 67: // C/c for change tile
		case 99:
			MapBlock = World.ReturnMapBlockReference(floor(Player.Position.Y), floor(Player.Position.X));
			if (MapBlock == NULL)
				break;
			if (MapBlock->BlockType == MAP_BLOCKWALL)
				MapBlock->SetBlockType(MAP_BLOCKFLOOR);
			else if (MapBlock->BlockType == MAP_BLOCKFLOOR)
				MapBlock->SetBlockType(MAP_BLOCKCORNER);
			else
				MapBlock->SetBlockType(MAP_BLOCKWALL);
			break;
		case 48: // 48 represents the 0 key. We want to clear the map except for the outer borders.
			for (int ty = 1; ty < MAPSIZEY - 1; ty++)
			{
				for (int tx = 1; tx < MAPSIZEX - 1; tx++)
				{
					MapBlock = World.ReturnMapBlockReference(ty,tx);
					MapBlock->SetBlockType(MAP_BLOCKFLOOR);
				}
			}
			break;
		case 81: // Q/q for Quit
		case 113:
			ExitProgram = true;
			break;
		}

		while (Movement.Y != 0 || Movement.X != 0) // There is attempted movement.
		{
			// Let's check for collisions first.
			mgCollisionDetection CollisionTest;
			mgDetectedCollision Results;

			CollisionTest.MapReference = &World;
			Results = CollisionTest.CollisionTest(&Player, Movement, 2);

			if (Results.Collision == false) // No collision
			{
				Player.Position.Y += Movement.Y;
				Player.Position.X += Movement.X;

				Movement.Y = Movement.X = 0; // We completed the move.
			}
			else
			{	// Handle it.				
				mgVector Projected;
				double dotproduct;

				// Push us 0.0000001 units away from the wall. Moments like this make me regret using
				// double point precision and not a fixed fractional unit size like everyone else does. sigh.
				Results.CollisionCorrection.NormalizeVector(Results.CollisionCorrection.Magnitude + 0.0000001);

				// Complete the movement in a manner that doesn't have us clip through the wall.
				Movement = Movement + Results.CollisionCorrection;

				Player.Position.Y += Movement.Y;
				Player.Position.X += Movement.X;

				Movement = Results.CollisionCorrection; // The remainder of the movement we want to attempt.
				Movement.ReverseDirection(); // We want this to go back into the wall we collided with.

				// Lose all velocity in the direction of the wall normal by projecting the direction onto it.
				dotproduct = Movement * Results.CollisionNormal;
				Projected = Results.CollisionNormal * dotproduct;
				Movement = Movement - Projected;
			}
		}
	
	}

	endwin();
}
