#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include "mgVectorPoint.h"
#include "mgMapDataHandler.h"
#include "mgPathSolutionGenerator.h"
#include "mgRandomMazeGenerator.h"
#include "mgRayTracer.h"
#include "mgVisibilityMap.h"
#include "mgLineSegment.h"
#include "mgMapElement.h"
#include "mgMapObject.h"
#include "mgStressTest.h"

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
		mgTraceResults TraceResults;

		double AngleDifference = (DegreesPerColumn * (double)RenderColumn) - (FOV / 2);
		double NewAngle = RenderAngle + AngleDifference;

		RenderVector.VectorFromDegrees(NewAngle);

		RenderTracer.MapReference = Map;
		TraceResults = RenderTracer.OccluderPoint(Position, RenderVector);

		double Distance = TraceResults.RayDistance;
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

	if (DisplayedVisibilityMap != nullptr)
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
				if (DisplayedVisibilityMap != nullptr && DisplayedVisibilityMap->IsMarkedVisible(TempY, TempX))
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
			else if (TempY == SpecialY && TempX == SpecialX && DisplayedVisibilityMap == nullptr)
			{
				Outputfile << " X ";
				HTMLOutput << "<img height=\"10\" src=\"path.jpg\" width=\"10\">";
			}
			else if (PathMap != nullptr && PathMap->CordPartofPath(TempY, TempX))
			{
				Outputfile << " . ";
				HTMLOutput << "<img height=\"10\" src=\"path.jpg\" width=\"10\">";
			}
			else
			{
				if (DisplayedVisibilityMap != nullptr && DisplayedVisibilityMap->IsMarkedVisible(TempY, TempX) &&
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

void RenderHQPosition(mgPoint Position, double RenderAngle, mgMapDataHandler *Map, std::ofstream *HOutput)
{
	// Columns represents how wide our display is going to be, Rows is how tall
	double *ColumnDepthMap;

	*HOutput << "<H2>First Person Raycasting from [" << Position.Y << "," << Position.X << "] with Angle " << RenderAngle << "</H2>" << std::endl;

	ColumnDepthMap = new double[180];

	double DegreesPerColumn = 90 / (double)180;

	for (int RenderColumn = 0; RenderColumn < 180; RenderColumn++)
	{
		// For each Column, do a ray trace from the origin and calculate the distance
		mgRayTracer RenderTracer;
		mgVector RenderVector;
		mgTraceResults TraceResults;

		double AngleDifference = (DegreesPerColumn * (double)RenderColumn) - (90 / 2);
		double NewAngle = RenderAngle + AngleDifference;

		RenderVector.VectorFromDegrees(NewAngle);

		RenderTracer.MapReference = Map;
		TraceResults = RenderTracer.OccluderPoint(Position, RenderVector);

		double Distance = TraceResults.RayDistance;
		Distance = Distance * cos(AngleDifference* (mgPI / 180));

		ColumnDepthMap[RenderColumn] = Distance;
	}

	// Now lets Render it into our HTML file
	for (int RenderRow = 0; RenderRow < 100; RenderRow++)
	{
		for (int RenderColumn = 0; RenderColumn < 180; RenderColumn++)
		{
			double Height = 100 / ColumnDepthMap[RenderColumn];
			double Top = (100 - Height) / 2;
			double Bottom = 100 - ((100 - Height) / 2);

			if (RenderRow > round(Top) && RenderRow < round(Bottom)) // It's a wall.
			{
				*HOutput << "<img height=\"10\" src=\"wall.jpg\" width=\"10\">";
			}
			else
			{	// Floor or ceiling
				*HOutput << "<img height=\"10\" src=\"pathwall.jpg\" width=\"10\">";
			}
		}
	}

	delete ColumnDepthMap;
}

void MapGrid(mgMapDataHandler *MapData, mgPoint Position)
{
	mgVisibilityMap PositionVisMap;
	mgStressTimer VisTimer;

	VisTimer.Description = "MapGrid mgVisibilityMap";

	PositionVisMap.LinkToMapHandler(MapData);

	if (!MapData->IsBlockClippable(floor(Position.Y), floor(Position.X)))
	{	// Only calculate the visibility data here if our position is valid.
		VisTimer.StartTimer();
		PositionVisMap.CalculateVisibilityBlock(floor(Position.Y), floor(Position.X));
		VisTimer.StopTimer();
		VisTimer.ConsoleOutputResults();
	}

	for (int Y = floor(Position.Y) - 5; Y < floor(Position.Y) + 5; Y++)
	{
		for (int X = floor(Position.Y) - 5; X < floor(Position.X) + 5; X++)
		{
			mgMapElement *PositionBlock;

			PositionBlock = MapData->ReturnMapBlockReference(Y, X);

			if (Y == floor(Position.Y) && X == floor(Position.X))
			{
				std::cout << " YOU ";
			}
			else if (PositionBlock == nullptr)
			{
				std::cout << "     ";
			}
			else if (PositionBlock->BlockType == MAP_BLOCKWALL)
			{
				if (PositionVisMap.IsMarkedVisible(Y, X))
					std::cout << "[---]";
				else
					std::cout << "[   ]";
			}
			else if (PositionBlock->BlockType == MAP_BLOCKFLOOR)
			{
				if (PositionVisMap.IsMarkedVisible(Y, X))
					std::cout << " ... ";
				else
					std::cout << "     ";
			}
			else
			{
				std::cout << "[???]";
			}
		}

		std::cout << std::endl;
	}
}

void CustomPositionRenders(mgMapDataHandler *MapData)
{
	mgPoint InputPosition;
	double angle;
	bool validPos = false;

	do
	{
		std::cout << "Position: Y -> ";
		std::cin >> InputPosition.Y;
		std::cout << "          X -> ";
		std::cin >> InputPosition.X;

		// If the position is just an integer then assume they want it put in the center of the block.
		if (InputPosition.Y == floor(InputPosition.Y) && InputPosition.X == floor(InputPosition.X))
		{
			InputPosition.Y += .5;
			InputPosition.X += .5;
		}

		MapGrid(MapData, InputPosition);

		if (MapData->IsBlockClippable(floor(InputPosition.Y), floor(InputPosition.X)))
		{
			std::cout << "Input position is inside a wall or off the map, try another..." << std::endl;
			continue;
		}

		std::cout << "Angle to Render -> ";
		std::cin >> angle;

		validPos = true;

	} while (!validPos);

	std::string FileName;
	FileName = "FPS" + std::to_string((int)floor(InputPosition.Y)) + "-" + std::to_string((int)floor(InputPosition.X)) + "-" + std::to_string((int)angle) + ".HTML";

	std::ofstream FPSOutput(FileName);

	FPSOutput << "<!DOCTYPE html>" << std::endl << "<head>" << std::endl;
	FPSOutput << "  <title>FPS Render Shot</title>" << std::endl;
	FPSOutput << "<style>" << std::endl << "#main {" << std::endl << "    width: " << 10 * 180 + 5 << "px;" << std::endl;
	FPSOutput << "}" << std::endl << "#main img {" << std::endl << "    float: left;" << std::endl;
	FPSOutput << "}</style>" << std::endl;
	FPSOutput << "<meta content=\"css3, transition, image, slide\" name=\"keywords\">" << std::endl;
	FPSOutput << "</head>" << std::endl << std::endl << "<body><div id=\"main\">" << std::endl;

	mgStressTimer FPSTimer;

	FPSTimer.Description = "FPS HQ Render";

	FPSTimer.StartTimer();

	RenderHQPosition(InputPosition, angle, MapData, &FPSOutput);

	FPSTimer.StopTimer();

	FPSOutput << "</div></body>" << std::endl << "</html>" << std::endl;

	FPSOutput.close();

	std::cout << "Output saved to " << FileName << std::endl;

	FPSTimer.ConsoleOutputResults();
}

int main(int argc, char* argv[])
{
	mgMapDataHandler *MazeMap, *MazeMapHoles;
	int MazeSizeY, MazeSizeX;
	bool stresstest = true;

	mgStressTimer Timer_Program, Timer_Generate, Timer_PathFind, Timer_vismap, Timer_render;
	mgStressTest test;

	std::cout << "mgGameEngine production testing suite. Engine Version: " << mgGameEngineVersion.ReleaseVersion << "." << mgGameEngineVersion.FeatureVersion << "." << mgGameEngineVersion.PatchVersion << std::endl;
	std::cout << "  Generated output is saved to both Maze.HTML and .TXT unless otherwise stated." << std::endl;
	std::cout << "       Stress tests generate console output only" << std::endl;
	while (stresstest)
	{
		char selection;

		std::cout << "---[ Stress Test Selection ]---" << std::endl;
		std::cout << "---[ 1) mgMapDataHandler   ]---" << std::endl;
		std::cout << "---[ 2) mgVector           ]---" << std::endl;
		std::cout << "---[ 3) mgVisibilityMap    ]---" << std::endl;
		std::cout << "---[ 4) mgMapElement       ]---" << std::endl;
		std::cout << "---[ 5) mgBinaryTree       ]---" << std::endl;
		std::cout << "---[                       ]---" << std::endl;
		std::cout << "---Any other key skips tests---" << std::endl;
		std::cout << "---: ";
		std::cin >> selection;

		switch (selection)
		{
		case '1':
			test.TEST_mgMapDataHandler();
			break;
		case '2':
			test.TEST_mgVector();
			break;
		case '3':
			test.TEST_mgVisibilityMap();
			break;
		case '4':
			test.TEST_mgMapElement();
			break;
		case '5':
			test.TEST_mgBinaryTree();
			break;
		default:
			stresstest = false;
		}
	}

	Timer_Program.Description = "Program run";
	Timer_Generate.Description = "mgRandomMazeGenerator";
	Timer_PathFind.Description = "mgPathSolutionGenerator";
	Timer_vismap.Description = "mgVisibilityMap";
	Timer_render.Description = "HTML 3D Viewport X 5";

	std::cout << "Enter size of maze to process test results against..." << std::endl;
	std::cout << "=====================================================================" << std::endl;
	std::cout << "Maze Size: Y = ";
	std::cin >> MazeSizeY;
	std::cout << "         : X = ";
	std::cin >> MazeSizeX;

	Timer_Program.StartTimer();

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

	Timer_Generate.StartTimer();

	// Generate the maze block by block, starting at the first block in 1,1
	mgRandomMazeGenerator *MazeGenerator;
	MazeGenerator = new mgRandomMazeGenerator;
	MazeGenerator->Map = MazeMap;
	MazeGenerator->GenerateMaze(1, 1);
	delete MazeGenerator;

	Timer_Generate.StopTimer();

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

			if (BlockReference == nullptr)
				continue;
			
			BlockReference->BlockType = MAP_BLOCKFLOOR;

			BlocksToClear--;
		}
	}

	std::cout << "Done." << std::endl;

	Timer_Generate.ConsoleOutputResults();

	// Present the maps after generating them...
	DisplayMap(1, 1, nullptr, MazeMap, nullptr);
	DisplayMap(1, 1, nullptr, MazeMapHoles, nullptr);

	std::cout << "Generating shortest route path...";

	Timer_PathFind.StartTimer();

	mgPathSolutionGenerator *PathSolution;
	PathSolution = new mgPathSolutionGenerator;
	PathSolution->LinkToMap(MazeMap);

	PathSolution->InitateMapSolution(1, 1, MazeMap->mapsizeY() - 2, MazeMap->mapsizeX() - 2); // Top left to bottom right.
	while (!PathSolution->ProcessMapSolution(10))
	{
		std::cout << ".";
	}

	Timer_PathFind.StopTimer();

	std::cout << "Done." << std::endl;

	Timer_PathFind.ConsoleOutputResults();

	// Display the normal map after generating it
	DisplayMap(1, 1, nullptr, MazeMap, PathSolution); // <-- Outputs map solution to TXT and HTML presentation formats

	// Solve the MazeMap with all the holes in it now.

	mgPathSolutionGenerator *PathSolutionHoles;
	PathSolutionHoles = new mgPathSolutionGenerator;
	PathSolutionHoles->LinkToMap(MazeMapHoles);

	PathSolutionHoles->InitateMapSolution(1, 1, MazeMapHoles->mapsizeY() - 2, MazeMap->mapsizeX() - 2);
	while (!PathSolutionHoles->ProcessMapSolution(100))
	{

	}

	// Display the holed out map after generating it.
	DisplayMap(1, 1, nullptr, MazeMapHoles, PathSolutionHoles);

	// --- Calculate visibility for block 15,15

	int VisY, VisX;

	VisY = 15;
	VisX = 15;

	Timer_vismap.StartTimer();

	mgVisibilityMap *testVisibilityBlock;
	testVisibilityBlock = new mgVisibilityMap;

	testVisibilityBlock->LinkToMapHandler(MazeMap);
	testVisibilityBlock->CalculateVisibilityBlock(VisY, VisX);

	Timer_vismap.StopTimer();

	Timer_vismap.ConsoleOutputResults();

	DisplayMap(VisY, VisX, testVisibilityBlock, MazeMap, nullptr);

	mgVisibilityMap *testHoleVisibilityBlock;
	testHoleVisibilityBlock = new mgVisibilityMap;

	testHoleVisibilityBlock->LinkToMapHandler(MazeMapHoles);
	testHoleVisibilityBlock->CalculateVisibilityBlock(VisY, VisX);

	DisplayMap(VisY, VisX, testHoleVisibilityBlock, MazeMapHoles, nullptr);

	// Generate first person perspective viewing

	Timer_render.StartTimer();

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

	Timer_render.StopTimer();

	Timer_render.ConsoleOutputResults();

	ClosePresentationOutputs();

	Timer_Program.StopTimer();

	Timer_Program.ConsoleOutputResults();

	//
	// --[ Custom position renders]--
	//
	while (true)
	{
		char response;

		std::cout << "Do you wish to render a position to a seperate HTML file (y\\n)? ";

		std::cin >> response;

		if (response == 'n' || response == 'N')
			break;

		CustomPositionRenders(MazeMap);
	}

	return 0;
}



