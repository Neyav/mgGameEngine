#ifndef MGSTRESSTESTH
#define MGSTRESSTESTH

#include "mgMapDataHandler.h"
#include "mgVectorPoint.h"

class mgStressTest
{
private:
	clock_t Start, End;
public:
	void TEST_mgMapDataHandler(void);
	void TEST_mgVector(void);

	mgStressTest();
};

#endif
