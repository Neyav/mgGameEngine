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

	Timer.Description = ".InitalizeMapData(100, 100) x 100k";

	Timer.StartTimer();

	for (int x = 0; x < 100000; x++)
		testmapdata.InitalizeMapData(100, 100);

	Timer.StopTimer();
	Timer.ConsoleOutputResults();
}

void mgStressTest::TEST_mgVector(void)
{
	mgVector testvector;

	std::cout << "[---void mgStressTest::TEST_mgVector(void)---]" << std::endl;

	Timer.Description = ".VectorFromDegrees(random) x 10mil";

	Timer.StartTimer();

	for (int x = 0; x < 10000000; x++)
	{
		testvector.VectorFromDegrees(rand() % 360);
	}

	Timer.StopTimer();

	Timer.ConsoleOutputResults();

}

mgStressTest::mgStressTest()
{
	srand(time(NULL)); // Seed the random number generator.
}