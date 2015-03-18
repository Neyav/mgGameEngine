#include "stdafx.h"
#include <time.h>
#include <iostream>
#include <fstream>

#include "mgVectorPoint.h"
#include "mgMapDataHandler.h"
#include "mgPathSolutionGenerator.h"
#include "mgRandomMazeGenerator.h"
#include "mgRayTracer.h"
#include "mgVisibilityMap.h"
#include "mgLineSegment.h"
#include "mgMapElement.h"
#include "mgMapObject.h"
#include "mgMapObjectPlayer.h"

std::ofstream Outputfile("Maze.txt");
std::ofstream HTMLOutput("Maze.html");

void RenderPosition(mgPoint Position, double RenderAngle, mgMapDataHandler *Map, int Rows, int Columns, double FOV)
{
	// Columns represents how wide our display is going to be, Rows is how tall
	double *ColumnDepthMap;

	HTMLOutput << "<H2>First Person Raycasting from [" << Position.Y << "," << Position.X << "] with Angle " << RenderAngle << "</H2>" << std::endl;

	ColumnDepthMap = new double[Columns];

	double DegreesPerColumn = FOV / (double)Columns;

	for (int RenderColumn = 0; RenderColumn < Columns; RenderColumn++)
	{
		// For each Column, do a ray trace from the origin and calculate the distance
		mgRayTracer RenderTracer;
		mgVector RenderVector;

		double AngleDifference = (DegreesPerColumn * (double)RenderColumn) - (FOV / 2);
		double NewAngle = RenderAngle + AngleDifference;

		RenderVector.VectorFromDegrees(NewAngle);

		RenderTracer.MapReference = Map;
		RenderTracer.OccluderPoint(Position, RenderVector);

		double Distance = RenderTracer.RayDistance;
		Distance = Distance * cos(AngleDifference* (mgPI / 180));

		ColumnDepthMap[RenderColumn] = Distance;
	}

	// Now lets Render it into our HTML file
	for (int RenderRow = 0; RenderRow < Rows; RenderRow++)
	{
		for (int RenderColumn = 0; RenderColumn < Columns; RenderColumn++)
		{
			double Height = Rows / ColumnDepthMap[RenderColumn];
			double Top = (Rows - Height) / 2;
			double Bottom = Rows - ((Rows - Height) / 2);

			if (RenderRow > round(Top) && RenderRow < round(Bottom)) // It's a wall.
			{
				HTMLOutput << "<img height=\"10\" src=\"wall.jpg\" width=\"10\">";
			}
			else
			{	// Floor or ceiling
				HTMLOutput << "<img height=\"10\" src=\"pathwall.jpg\" width=\"10\">";
			}
		}
	}

	delete ColumnDepthMap;
}

void DisplayMap(int SpecialY, int SpecialX, mgVisibilityMap *DisplayedVisibilityMap, mgMapDataHandler *DisplayedMap, mgPathSolutionGenerator *PathMap)
{
	int DrawY, DrawX;

	DrawY = DisplayedMap->mapsizeY();
	DrawX = DisplayedMap->mapsizeX();

	if (DisplayedVisibilityMap != NULL)
	{
		Outputfile << "{-mgVisibilityMap with " << DisplayedVisibilityMap->MappedItems() << " objects being represented-}" << std::endl;
		HTMLOutput << "<H1>{-mgVisibilityMap with " << DisplayedVisibilityMap->MappedItems() << " objects being represented-}</H1><BR>";
	}

	for (int TempY = 0; TempY < DrawY; TempY++)
	{
		for (int TempX = 0; TempX < DrawX; TempX++)
		{
			if (DisplayedMap->IsBlockClippable(TempY, TempX) && !(TempX == DisplayedMap->mapsizeX() - 1 && TempY == DisplayedMap->mapsizeY() - 2) && !(TempX == 1 && TempY == 0))
			{
				if (DisplayedVisibilityMap != NULL && DisplayedVisibilityMap->IsMarkedVisible(TempY, TempX))
				{
					Outputfile << "[V]";
					HTMLOutput << "<img height=\"10\" src=\"pathwall.jpg\" width=\"10\">";
				}
				else
				{
					Outputfile << "[*]";
					HTMLOutput << "<img height=\"10\" src=\"wall.jpg\" width=\"10\">";
				}
			}
			else if (TempY == SpecialY && TempX == SpecialX && DisplayedVisibilityMap == NULL)
			{
				Outputfile << " X ";
				HTMLOutput << "<img height=\"10\" src=\"path.jpg\" width=\"10\">";
			}
			else if (PathMap != NULL && PathMap->CordPartofPath(TempY, TempX))
			{
				Outputfile << " . ";
				HTMLOutput << "<img height=\"10\" src=\"path.jpg\" width=\"10\">";
			}
			else
			{
				if (DisplayedVisibilityMap != NULL && DisplayedVisibilityMap->IsMarkedVisible(TempY, TempX) &&
					!(TempY == SpecialY && TempX == SpecialX)) // Don't draw the "originating block" so we can tell where the visibility block is.
				{
					Outputfile << " V ";
					HTMLOutput << "<img height=\"10\" src=\"path.jpg\" width=\"10\">";
				}
				else
				{
					Outputfile << "   ";
					HTMLOutput << "<img height=\"10\" src=\"floor.jpg\" width=\"10\">";
				}
			}

		}
		Outputfile << std::endl;
		//HTMLOutput << "<br>";
	}
}

void ClosePresentationOutputs(void)
{
	// Setup ending of HTML
	HTMLOutput << "</div></body>" << std::endl << "</html>" << std::endl;

	// We should be closing data streams here.
}

int _tmain(int argc, _TCHAR* argv[])
{
	mgMapDataHandler *MazeMap, *MazeMapHoles;
	int MazeSizeY, MazeSizeX;

	// This block was simply used to test that map objects were working the way they were supposed to with translations.
	/*mgMapObject *Object;
	mgMapObjectPlayer Player;
	mgLinkedList<mgLineSegment> *SegmentList;
	mgLineSegment *Ref;

	Object = &Player;

	SegmentList = Object->ObjectGeometry();

	Ref = SegmentList->ReturnElementReference();
	while (Ref != NULL)
	{
		std::cout << "[" << Ref->SegmentStart.Y << ", " << Ref->SegmentStart.X << "] -> [" << Ref->SegmentEnd.Y << ", " << Ref->SegmentEnd.X << "]" << std::endl;

		Ref = SegmentList->ReturnElementReference();
	}

	Player.Position.Y = 5;
	Player.Position.X = 3;

	SegmentList = Object->ObjectGeometry();

	Ref = SegmentList->ReturnElementReference();
	while (Ref != NULL)
	{
		std::cout << "[" << Ref->SegmentStart.Y << ", " << Ref->SegmentStart.X << "] -> [" << Ref->SegmentEnd.Y << ", " << Ref->SegmentEnd.X << "]" << std::endl;

		Ref = SegmentList->ReturnElementReference();
	}*/


	std::cout << "MazeGenerator with Solution: [Uses mgMapEngine designed for a 3D Maze" << std::endl;
	std::cout << "                           : solving game]" << std::endl;
	std::cout << "                           : This program is a testing ground for it." << std::endl;
	std::cout << "                           : Output is Maze.HTML and Maze.TXT" << std::endl;
	std::cout << "=====================================================================" << std::endl;
	std::cout << "Maze Size: Y = ";
	std::cin >> MazeSizeY;
	std::cout << "         : X = ";
	std::cin >> MazeSizeX;

	MazeMap = new mgMapDataHandler;
	MazeMap->InitalizeMapData(MazeSizeY, MazeSizeX); // Initalize a maze.

	// Setup HTML output
	unsigned int HTMLWidth = 10 * MazeMap->mapsizeX() + 5;
	HTMLOutput << "<!DOCTYPE html>" << std::endl << "<head>" << std::endl;
	HTMLOutput << "  <title>Random Maze with solution</title>" << std::endl;
	HTMLOutput << "<style>" << std::endl << "#main {" << std::endl << "    width: " << HTMLWidth << "px;" << std::endl;
	HTMLOutput << "}" << std::endl << "#main img {" << std::endl << "    float: left;" << std::endl;
	HTMLOutput << "}</style>" << std::endl;
	HTMLOutput << "<meta content=\"css3, transition, image, slide\" name=\"keywords\">" << std::endl;
	HTMLOutput << "</head>" << std::endl << std::endl << "<body><div id=\"main\">" << std::endl;

	std::cout << "Generating Random Maze...";

	// Generate the maze block by block, starting at the first block in 1,1
	mgRandomMazeGenerator *MazeGenerator;
	MazeGenerator = new mgRandomMazeGenerator;
	MazeGenerator->Map = MazeMap;
	MazeGenerator->GenerateMaze(1, 1);
	delete MazeGenerator;
	mgMapElement *EndBlock;
	EndBlock = MazeMap->ReturnMapBlockReference(MazeMap->mapsizeY() - 2, MazeMap->mapsizeX() - 2);
	EndBlock->BlockType = MAP_BLOCKFLOOR;
	
	MazeMapHoles = new mgMapDataHandler; // This is going to be a copy of our inital map that is going to have holes drilled into it.
	MazeMapHoles->CloneMapIntoHost(MazeMap);

	// Test code, randomly set Y+X walls to become floors. Opens the map up for through pathfinding testing.
	int BlocksToClear = (MazeMapHoles->mapsizeY() * MazeMapHoles->mapsizeX()) / 5;
	while (BlocksToClear)
	{
		int TestClearY = (rand() % (MazeMap->mapsizeY() - 2) + 1);
		int TestClearX = (rand() % (MazeMap->mapsizeX() - 2) + 1);

		if (MazeMapHoles->IsBlockClippable(TestClearY, TestClearX))
		{
			mgMapElement *BlockReference;

			BlockReference = MazeMapHoles->ReturnMapBlockReference(TestClearY, TestClearX);

			if (BlockReference == NULL)
				continue;
			
			BlockReference->BlockType = MAP_BLOCKFLOOR;

			BlocksToClear--;
		}
	}

	std::cout << "Done." << std::endl;

	// Present the maps after generating them...
	DisplayMap(1, 1, NULL, MazeMap, NULL);
	DisplayMap(1, 1, NULL, MazeMapHoles, NULL);

	std::cout << "Generating shortest route path...";

	mgPathSolutionGenerator *PathSolution;
	PathSolution = new mgPathSolutionGenerator;
	PathSolution->LinkToMap(MazeMap);

	PathSolution->InitateMapSolution(1, 1, MazeMap->mapsizeY() - 2, MazeMap->mapsizeX() - 2); // Top left to bottom right.
	while (!PathSolution->ProcessMapSolution(10))
	{
		std::cout << ".";
	}

	std::cout << "Done." << std::endl;

	// Display the normal map after generating it
	DisplayMap(1, 1, NULL, MazeMap, PathSolution); // <-- Outputs map solution to TXT and HTML presentation formats

	// Solve the MazeMap with all the holes in it now.

	mgPathSolutionGenerator *PathSolutionHoles;
	PathSolutionHoles = new mgPathSolutionGenerator;
	PathSolutionHoles->LinkToMap(MazeMapHoles);

	PathSolutionHoles->InitateMapSolution(1, 1, MazeMapHoles->mapsizeY() - 2, MazeMap->mapsizeX() - 2);
	while (!PathSolutionHoles->ProcessMapSolution(10))
	{

	}

	// Display the holed out map after generating it.
	DisplayMap(1, 1, NULL, MazeMapHoles, PathSolutionHoles);

	// --- Calculate visibility for block 15,15

	int VisY, VisX;

	VisY = 15;
	VisX = 15;

	mgVisibilityMap *testVisibilityBlock;
	testVisibilityBlock = new mgVisibilityMap;

	testVisibilityBlock->LinkToMapHandler(MazeMap);
	testVisibilityBlock->CalculateVisibility(VisY, VisX);
	testVisibilityBlock->CalculateAdjacentVisibility(VisY, VisX); // Calculates Visibility for adjacent blocks as well

	DisplayMap(VisY, VisX, testVisibilityBlock, MazeMap, NULL);

	mgVisibilityMap *testHoleVisibilityBlock;
	testHoleVisibilityBlock = new mgVisibilityMap;

	testHoleVisibilityBlock->LinkToMapHandler(MazeMapHoles);
	testHoleVisibilityBlock->CalculateVisibility(VisY, VisX);
	testHoleVisibilityBlock->CalculateAdjacentVisibility(VisY, VisX);

	DisplayMap(VisY, VisX, testHoleVisibilityBlock, MazeMapHoles, NULL);

	// Generate first person perspective viewing

	mgPoint RenderPoint;
	RenderPoint.Y = 1.5;
	RenderPoint.X = 1.5;

	RenderPosition(RenderPoint, 0, MazeMap, MazeSizeX, MazeSizeX, 90);
	RenderPosition(RenderPoint, 25, MazeMap, MazeSizeX, MazeSizeX, 90);

	RenderPoint.Y = 15.5;
	RenderPoint.X = 15.5;

	RenderPosition(RenderPoint, 3, MazeMap, MazeSizeX, MazeSizeX, 90);
	RenderPosition(RenderPoint, 36, MazeMap, MazeSizeX, MazeSizeX, 90);
	RenderPosition(RenderPoint, 36, MazeMapHoles, MazeSizeX, MazeSizeX, 90);

	ClosePresentationOutputs();

	return 0;
}



