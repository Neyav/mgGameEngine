#ifndef MGSTRESSTESTH
#define MGSTRESSTESTH

#include "mgMapDataHandler.h"
#include "mgVectorPoint.h"

class mgStressTimer
{
private:
	clock_t Start, End;
	bool TimerRunning = false;
	bool TimerComplete = false;
public:
	std::string Description = "No Description";

	void StartTimer(void);
	void StopTimer(void);
	void ConsoleOutputResults(void);
	void ConsoleOutputIterationResults(const unsigned int Iterations);
};

class mgStressTest
{
private:
	mgStressTimer Timer;
public:
	void TEST_mgMapDataHandler(void);
	void TEST_mgVector(void);

	mgStressTest();
};

#endif
