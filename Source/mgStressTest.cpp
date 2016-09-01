#include <time.h>
#include <iostream>
#include <math.h>

#include "mgMapDataHandler.h"
#include "mgVectorPoint.h"
#include "mgCollisionDetection.h"
#include "mgLineSegment.h"
#include "mgVisibilityMap.h"
#include "mgMapObject.h"
#include "Containers/mgLinkedList.h"
#include "Containers/mgBinaryTree.h"
#include "Containers/mgAVLBinaryTree.h"
#include "Containers/mgRBTBinaryTree.h"

#include "mgStressTest.h"

#define TIME_NANOSECOND 0.000000001
#define TIME_MICROSECOND 0.000001
#define TIME_MILLISECOND 0.001

void mgStressTimer::StartTimer(void)
{
	TimerComplete = false;
	TimerRunning = true;

	Start = clock();
}

void mgStressTimer::StopTimer(void)
{
	if (!TimerRunning)
		return; // There is no timer, call start before calling stop.

	End = clock();
	TimerRunning = false;
	TimerComplete = true;
}

void mgStressTimer::ConsoleOutputFriendlyTime(double TimeInput)
{
	if (TimeInput < TIME_MICROSECOND)
	{
		TimeInput /= TIME_NANOSECOND;
		std::cout << TimeInput << " Nanoseconds." << std::endl;
	}
	else if (TimeInput < TIME_MILLISECOND)
	{
		TimeInput /= TIME_MICROSECOND;
		std::cout << TimeInput << " Microseconds." << std::endl;
	}
	else if (TimeInput < 1)
	{
		TimeInput /= TIME_MILLISECOND;
		std::cout << TimeInput << " Milliseconds." << std::endl;
	}
	else
		std::cout << TimeInput << " Seconds." << std::endl;
}

void mgStressTimer::ConsoleOutputResults(void)
{
	if (!TimerComplete)
	{
		std::cout << "Timer[" << Description.c_str() << "] results paged with no valid results." << std::endl;
		return;
	}
	std::cout << "Timer[" << Description.c_str() << "] -> ";

	ConsoleOutputFriendlyTime((double)(End - Start) / CLOCKS_PER_SEC);
}

void mgStressTimer::ConsoleOutputIterationResults(const unsigned int Iterations)
{
	if (!TimerComplete)
	{
		std::cout << "Timer[" << Description.c_str() << "] results paged with no valid results." << std::endl;
		return;
	}
	
	double TimePerIteration = (double)((double)(End - Start) / CLOCKS_PER_SEC) / Iterations;
	
	std::cout << "IterationTimer[" << Description.c_str() << "] -> ";

	ConsoleOutputFriendlyTime(TimePerIteration);
}

mgStressTimer::mgStressTimer()
{
	TimerRunning = false;
	TimerComplete = false;
	std::string Description = "No Description";
}

void mgStressTest::TEST_mgMapDataHandler(void)
{
	mgMapDataHandler testmapdata;

	std::cout << "[---void mgStressTest::TEST_mgMapDataHandler(void)---]" << std::endl;

	Timer.Description = ".InitalizeMapData(100, 100) x 1k";

	Timer.StartTimer();

	for (int x = 0; x < 1000; x++)
		testmapdata.InitalizeMapData(100, 100);

	Timer.StopTimer();
	Timer.ConsoleOutputResults();
	Timer.ConsoleOutputIterationResults(1000); 
}

void mgStressTest::TEST_mgVector(void)
{
	volatile double testmemory;

	std::cout << "[---void mgStressTest::TEST_mgVector(void)---]" << std::endl;

	Timer.Description = ".VectorFromDegrees(x % 360) x 10mil";
	Timer.StartTimer();

	for (int x = 0; x < 10000000; x++)
	{
		mgVector testvector;
		testvector.VectorFromDegrees(x % 360);

		testmemory = testvector.Y;
	}

	Timer.StopTimer();
	Timer.ConsoleOutputResults();
	Timer.ConsoleOutputIterationResults(10000000);

	Timer.Description = ".VectorFromRadians(x % 2) x 10mil";
	Timer.StartTimer();

	for (int x = 0; x < 10000000; x++)
	{
		mgVector testvector;
		testvector.VectorFromRadians(x % 2);

		testmemory = testvector.Y;
	}

	Timer.StopTimer();
	Timer.ConsoleOutputResults();
	Timer.ConsoleOutputIterationResults(10000000);

	Timer.Description = ".NormalizeVector(x % 10) x 10mil";
	Timer.StartTimer();

	{
		mgVector NormalizeVector;
		NormalizeVector.VectorFromDegrees(50);

		for (int x = 0; x < 10000000; x++)
		{
			NormalizeVector.NormalizeVector(x % 10);
		}
	}

	Timer.StopTimer();
	Timer.ConsoleOutputResults();
	Timer.ConsoleOutputIterationResults(10000000);


}

void mgStressTest::TEST_mgVisibilityMap(void)
{
	mgMapDataHandler TestMap;
	mgMapElement *MapElement;

	std::cout << "[---void mgStressTest::TEST_mgVisibilityMap(void)---]" << std::endl;

	TestMap.InitalizeMapData(50, 50);

	// Empty out the center core of the map so it's just the outer walls.
	for (unsigned int Y = 1; Y < 50; Y++)
		for (unsigned int X = 1; X < 50; X++)
		{
			MapElement = TestMap.ReturnMapBlockReference(Y, X);
			MapElement->BlockType = MAP_BLOCKFLOOR;
		}

	Timer.Description = ".CalculateVisibilityBlock(25,25) x 3";

	Timer.StartTimer();

	for (unsigned int i = 0; i < 3; i++)
	{
		mgVisibilityMap VisMap;

		VisMap.LinkToMapHandler(&TestMap);

		// Calculate both the visibility map and the adjacent visibility maps for the center
		VisMap.CalculateVisibilityBlock(25, 25);
	}

	Timer.StopTimer();
	Timer.ConsoleOutputResults();
	Timer.ConsoleOutputIterationResults(3);
}

void mgStressTest::TEST_mgMapElement(void)
{
	mgMapElement *testelement;
	mgListIterator<mgLineSegment> testList;

	std::cout << "[---void mgStressTest::TEST_mgMapElement(void)---]" << std::endl;

	Timer.Description = "Create, return shape, and delete x 1m";
	Timer.StartTimer();

	for (unsigned int iterator = 0; iterator < 1000000; iterator++)
	{
		testelement = new mgMapElement;
		testList = testelement->BlockGeometry();
		delete testelement;
	}

	Timer.StopTimer();
	Timer.ConsoleOutputResults();
	Timer.ConsoleOutputIterationResults(1000000);

	Timer.Description = ".BlockGeometry() x 1m";

	testelement = new mgMapElement;

	Timer.StartTimer();
	for (unsigned int iterator = 0; iterator < 1000000; iterator++)
		testList = testelement->BlockGeometry();
	Timer.StopTimer();	

	delete testelement;	

	Timer.ConsoleOutputResults();
	Timer.ConsoleOutputIterationResults(10000000);
}

void mgStressTest::TEST_mgBinaryTree(void)
{
	mgBinaryTree<int> BinaryTree;
	mgAVLBinaryTree<int> AVLBinaryTree;
	mgRBTBinaryTree<int> RBTBinaryTree;
	volatile bool a;

	std::cout << "[---void mgStressTest::TEST_mgBinaryTree(void)---]" << std::endl;

	std::cout << "[-> Insertion tests" << std::endl;

	Timer.Description = "Unbalanced Tree, 5k inc and dec";
	Timer.StartTimer();
	for (int i = 0; i < 5000; i++)
	{
		BinaryTree.AddElement(i);
		BinaryTree.AddElement(10000 - i);
	}
	Timer.StopTimer();

	Timer.ConsoleOutputResults();
	Timer.ConsoleOutputIterationResults(10000);

	Timer.Description = "Red Black Tree, 5k inc and dec";
	Timer.StartTimer();
	for (int i = 0; i < 5000; i++)
	{
		RBTBinaryTree.AddElement(i);
		RBTBinaryTree.AddElement(10000 - i);
	}
	Timer.StopTimer();

	Timer.ConsoleOutputResults();
	Timer.ConsoleOutputIterationResults(10000);

	Timer.Description = "AVL Tree, 5k inc and dec";
	Timer.StartTimer();
	for (int i = 0; i < 5000; i++)
	{
		AVLBinaryTree.AddElement(i);
		AVLBinaryTree.AddElement(10000 - i);
	}
	Timer.StopTimer();

	Timer.ConsoleOutputResults();
	Timer.ConsoleOutputIterationResults(10000);

	std::cout << "[-> Search tests" << std::endl;

	Timer.Description = "Unbalanced Tree, 10k incrementing";
	Timer.StartTimer();
	for (int i = 0; i < 10000; i++)
	{
		a = BinaryTree.IsElementPresent(i);
	}
	Timer.StopTimer();

	Timer.ConsoleOutputResults();
	Timer.ConsoleOutputIterationResults(10000);

	Timer.Description = "Red Black Tree, 10k incrementing";
	Timer.StartTimer();
	for (int i = 0; i < 10000; i++)
	{
		a = RBTBinaryTree.IsElementPresent(i);
	}
	Timer.StopTimer();

	Timer.ConsoleOutputResults();
	Timer.ConsoleOutputIterationResults(10000);

	Timer.Description = "AVL Tree, 10k incrementing";
	Timer.StartTimer();
	for (int i = 0; i < 10000; i++)
	{
		a = AVLBinaryTree.IsElementPresent(i);
	}
	Timer.StopTimer();

	Timer.ConsoleOutputResults();
	Timer.ConsoleOutputIterationResults(10000);

#ifdef REBALANCETREE
	Timer.Description = "Unbalanced Tree: Rebalance operation";
	Timer.StartTimer();

	BinaryTree.ReBalanceTree();

	Timer.StopTimer();
	Timer.ConsoleOutputResults();

	Timer.Description = "Rebalanced Tree, 10k incrementing";
	Timer.StartTimer();

	for (int i = 0; i < 10000; i++)
	{
		a = BinaryTree.IsElementPresent(i);
	}
	Timer.StopTimer();

	Timer.ConsoleOutputResults();
	Timer.ConsoleOutputIterationResults(10000);
#endif

}

void mgStressTest::TEST_mgCollisionDetection(void)
{
	mgMapDataHandler TestMap;
	mgMapElement *MapElement;
	mgMapObject TestObject;
	mgCollisionDetection CollisionTest;
	mgDetectedCollision Results;
	volatile bool a;

	std::cout << "[---void mgStressTest::TEST_mgCollisionDetection(void)---]" << std::endl;

	TestMap.InitalizeMapData(10, 10); // Simple 10 by 10 map as that's all we'll need.
	CollisionTest.MapReference = &TestMap;

	// Clear out some spots in the middle of the map, 4,4 to 5,5 will be hollow.
	for (unsigned int Y = 4; Y < 6; Y++)
		for (unsigned int X = 4; X < 6; X++)
		{
			MapElement = TestMap.ReturnMapBlockReference(Y, X);
			MapElement->BlockType = MAP_BLOCKFLOOR;
		}

	TestObject.ObjectSize = 0.3; // The size of our map object.
	TestObject.Position.Y = 4.5;
	TestObject.Position.X = 4.5; // The position on the map.

	Timer.Description = "Collision Test: 360 degrees - Magnitude 2 (36,000 tests)";

	Timer.StartTimer();
	for (int i = 0; i < 36000; i++)
	{
		mgVector Movement;

		Movement.Magnitude = 2;
		Movement.VectorFromDegrees(i / 100); // Create our movement vector;

		Results = CollisionTest.CollisionTest(&TestObject, Movement, 2);
		a = Results.Collision;
	}
	Timer.StopTimer();

	Timer.ConsoleOutputResults();
	Timer.ConsoleOutputIterationResults(36000);

	Timer.Description = "Collision Test: 360 degrees - Magnitude 7 (36,000 tests)";

	Timer.StartTimer();
	for (int i = 0; i < 36000; i++)
	{
		mgVector Movement;

		Movement.Magnitude = 7;
		Movement.VectorFromDegrees(i / 100); // Create our movement vector;

		Results = CollisionTest.CollisionTest(&TestObject, Movement, 7);
		a = Results.Collision;
	}
	Timer.StopTimer();

	Timer.ConsoleOutputResults();
	Timer.ConsoleOutputIterationResults(36000);
}

mgStressTest::mgStressTest()
{
	srand(time(nullptr)); // Seed the random number generator.
}
