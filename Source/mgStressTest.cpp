#include <time.h>
#include <iostream>
#include <math.h>

#include "mgMapDataHandler.h"
#include "mgVectorPoint.h"

#include "mgStressTest.h"

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

void mgStressTimer::ConsoleOutputResults(void)
{
	if (!TimerComplete)
	{
		std::cout << "Timer[" << Description.c_str() << "] results paged with no valid results." << std::endl;
		return;
	}
	std::cout << "Timer[" << Description.c_str() << "] -> " << ((double)End - (double)Start) / CLOCKS_PER_SEC << " Seconds elapsed." << std::endl;
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


}

mgStressTest::mgStressTest()
{
	srand(time(NULL)); // Seed the random number generator.
}