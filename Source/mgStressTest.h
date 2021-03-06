#ifndef MGSTRESSTESTH
#define MGSTRESSTESTH

#include "mgMapDataHandler.h"
#include "mgVectorPoint.h"
#include "mgVisibilityMap.h"
#include "mgCollisionDetection.h"
#include "Containers/mgLinkedList.h"
#include "Containers/mgBinaryTree.h"
#include "mgLineSegment.h"

class mgStressTimer
{
private:
	clock_t Start, End;
	bool TimerRunning;
	bool TimerComplete;
public:
	std::string Description;

	void StartTimer(void);
	void StopTimer(void);
	void ConsoleOutputFriendlyTime(double TimeInput);
	void ConsoleOutputResults(void);
	void ConsoleOutputIterationResults(const unsigned int Iterations);

	mgStressTimer();
};

class mgStressTest
{
private:
	mgStressTimer Timer;
public:
	void TEST_mgMapDataHandler(void);
	void TEST_mgVector(void);
	void TEST_mgVisibilityMap(void);
	void TEST_mgMapElement(void);
	void TEST_mgBinaryTree(void);
	void TEST_mgCollisionDetection(void);

	mgStressTest();
};

#endif
