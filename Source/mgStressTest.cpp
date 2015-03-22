#include <time.h>
#include <iostream>
#include <math.h>

#include "mgMapDataHandler.h"
#include "mgVectorPoint.h"

#include "mgStressTest.h"

void mgStressTest::TEST_mgMapDataHandler(void)
{

}

void mgStressTest::TEST_mgVector(void)
{
	mgVector testvector;

	std::cout << "void mgStressTest::TEST_mgVector(void)" << std::endl;

	std::cout << "void mgVector::VectorFromDegrees(double Degrees) * 10mil -> ";

	Start = clock();

	for (int x = 0; x < 10000000; x++)
	{
		testvector.VectorFromDegrees(rand() % 360);
	}

	End = clock();

	std::cout << ((double)End - (double)Start) / CLOCKS_PER_SEC << " Seconds." << std::endl;

}

mgStressTest::mgStressTest()
{
	srand(time(NULL)); // Seed the random number generator.
}