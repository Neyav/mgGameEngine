#include <ncurses.h>
#include <stdlib.h>
#include <math.h>
#include "mgVectorPoint.h"
#include "mgMapDataHandler.h"
#include "mgRayTracer.h"
#include "mgRandomMazeGenerator.h"

int FOV = 90; // Default field of view

int main(void)
{
	bool ExitProgram = false;

	mgMapDataHandler World;
	mgRandomMazeGenerator Generator;

	// Make a 50x50 world by default.
	World.InitalizeMapData(50, 50);

	// Generate a random maze in the world.
	Generator.Map = &World;
	Generator.GenerateMaze(1,1);

	// Your position in this world
	mgPoint ViewPort = { 1.5, 1.5 };
	int ViewAngle = 90;

	initscr();
	noecho();
	curs_set(false);

	while(!ExitProgram)
	{ // Render our viewport
		int MaxY, MaxX;
		double ColumnDepthMap[300]; // If your terminal is bigger...

		if (ViewAngle >= 360)
			ViewAngle -= 360;

		if (ViewAngle < 0)
			ViewAngle += 360;

		getmaxyx(stdscr, MaxY, MaxX);
		clear();

		double DegreesPerColumn = FOV / (double)MaxX;

		for (int RenderColumn = 0; RenderColumn < MaxX; RenderColumn++)
		{	// Shoot a ray for each column to determine if we hit a wall and how far away it is.
			mgRayTracer RenderTracer;
			mgVector RenderVector;

			double AngleDifference = (DegreesPerColumn * (double)RenderColumn) - (FOV / 2);
			double NewAngle = ViewAngle + AngleDifference;

			RenderVector.VectorFromDegrees(NewAngle);

			RenderTracer.MapReference = &World;
			RenderTracer.OccluderPoint(ViewPort, RenderVector);

			double Distance = RenderTracer.RayDistance;
			Distance = Distance * cos(AngleDifference * (mgPI / 180));

			ColumnDepthMap[RenderColumn] = Distance;
		}

		// Render here
		for (int RenderRow = 0; RenderRow < MaxY; RenderRow++)
		{
		for (int RenderColumn = 0; RenderColumn < MaxX; RenderColumn++)
		{
			double Height = MaxY / ColumnDepthMap[RenderColumn];
			double Top = (MaxY - Height) / 2;
			double Bottom = MaxY - ((MaxY - Height) / 2);

			if (RenderRow > round(Top) && RenderRow < round(Bottom))
				mvprintw(RenderRow,RenderColumn, "O");
			else
				mvprintw(RenderRow,RenderColumn, " ");
		}
		}

		// Display a "minimap"
		int ScreenY = 0;
		for (int MMY = floor(ViewPort.Y) - 2; MMY < floor(ViewPort.Y + 3); MMY++)
		{
		int ScreenX = 0;
		for (int MMX = floor(ViewPort.X) - 2; MMX < floor(ViewPort.X + 3); MMX++)
		{
		if (MMY == floor(ViewPort.Y) && MMX == floor(ViewPort.X))
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

			ViewPort.Y += Movement.Y;
			ViewPort.X += Movement.X;
			break;
		case 83: // D/d for Backwards
		case 115:
			Movement.Magnitude = -0.2; // Backwards "speed"
			Movement.VectorFromDegrees(ViewAngle);

			ViewPort.Y += Movement.Y;
			ViewPort.X += Movement.X;
			break;
		case 81: // Q/q for Quit
		case 113:
			ExitProgram = true;
			break;
		}
	}

	endwin();
}
