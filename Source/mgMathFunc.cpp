#include <math.h>
#include <stdlib.h>
#include "mgMathFunc.h"

#include <iostream>

// The determinant for whether this is needed is merely one of speed versus memory and accuracy. sin and cos are compute expensive, and converting from degrees to
// radians is a tax you pay every time you need to do it. This generates tables of precalculated values up to a precision you deem important. Memory expenditures can get quite high
// though as a 4 precision table will use about 2.8MB of RAM, and a 5 precision table will use 28MB of RAM. The 4 precision table seems to be right on the money if you aren't doing
// scientific work, and well, 2.8MB, cough, at least we aren't in 1994 anymore. 

// IF YOU ATTEMPT TO USE THESE FUNCTIONS WITHOUT INITALIZING THEM FIRST THE CRASH YOU GET WILL C-C-C-C-C-C-COMBO BREAK YOUR HAPPINESS.

// SINE
double *mgSineTable = NULL;
unsigned int mgSineTablePrecision;
unsigned int mgSineTableDecimalMux;

// COSINE
double *mgCoSineTable = NULL;
unsigned int mgCoSineTablePrecision;
unsigned int mgCoSineTableDecimalMux;

// ATAN
double *mgATanTable = NULL;
unsigned int mgATanTablePrecision;
unsigned int mgATanTableDecimalMux;

inline double degrees2radians(double Degrees)
{
	double Radians = Degrees * (3.14159265358979323846264338327950288419716939937510 / 180); // OH NO I DIDN'T! SUPER PRECISION BODY CHECK!

	return Radians;
}

inline double radians2degrees(double Radians)
{
	double Degrees = Radians / (3.14159265358979323846264338327950288419716939937510 / 180); // ENFORCER!

	return Degrees;
}

// ----------------------------------------------------
// - Sine of Degrees								  -
// ----------------------------------------------------

void mgSineStartup(unsigned int precision)
{
	unsigned int TableIndex;

	if (mgSineTable != NULL)
		delete mgSineTable; // So what, you already called this function? Let's START IT ALL OVER!

	if (precision > 7)
		precision = 7;
	else if (precision < 3)
		precision = 3;

	precision--;

	mgSineTableDecimalMux = (pow((double)10, (double)precision));
	mgSineTablePrecision = 360 * mgSineTableDecimalMux;
	mgSineTable = new double[mgSineTablePrecision];
	TableIndex = 0;

	while (TableIndex < mgSineTablePrecision)
	{
		mgSineTable[TableIndex] = sin(degrees2radians((double)TableIndex / (double)mgSineTableDecimalMux));
		TableIndex++;
	}
}

double mgSineDeg(double Degrees)
{
	unsigned int DegreeInt;

	while (Degrees > 359)
		Degrees -= 360;	// Just incase.
	while (Degrees < 0)
		Degrees += 360;

	DegreeInt = Degrees * mgSineTableDecimalMux;

	return mgSineTable[DegreeInt];
}

// ----------------------------------------------------
// - Cosine of Degrees								  -
// ----------------------------------------------------

void mgCoSineStartup(unsigned int precision)
{
	unsigned int TableIndex;

	if (mgCoSineTable != NULL)
		delete mgCoSineTable; // So what, you already called this function? Let's START IT ALL OVER!
								// DON'T EVEN THINK OF ACCUSING ME OF COPY PASTING THIS!

	if (precision > 7)
		precision = 7;
	else if (precision < 3)
		precision = 3;

	precision--;

	mgCoSineTableDecimalMux = (pow((double)10, (double)precision));
	mgCoSineTablePrecision = 360 * mgCoSineTableDecimalMux;
	mgCoSineTable = new double[mgCoSineTablePrecision];
	TableIndex = 0;

	while (TableIndex < mgCoSineTablePrecision)
	{
		mgCoSineTable[TableIndex] = cos(degrees2radians((double)TableIndex / (double)mgCoSineTableDecimalMux));
		TableIndex++;
	}
}

double mgCoSineDeg(double Degrees)
{
	unsigned int DegreeInt;

	while (Degrees > 359)
		Degrees -= 360;	// Just incase.
	while (Degrees < 0)
		Degrees += 360;

	DegreeInt = Degrees * mgCoSineTableDecimalMux;

	return mgCoSineTable[DegreeInt];
}

// ----------------------------------------------------
// - Arc Tangent of Degrees							  -
// ----------------------------------------------------

void mgATanStartup(unsigned int precision)
{
	unsigned int TableIndex;

	if (mgATanTable != NULL)
		delete mgATanTable; // So what, you already called this function? Let's START IT ALL OVER!
							// DON'T EVEN THINK OF ACCUSING ME OF COPY PASTING THIS AGAIN!!!

	if (precision > 10)
		precision = 10;
	else if (precision < 3)
		precision = 3;

	precision--;

	mgATanTableDecimalMux = (pow((double)10, (double)precision));
	mgATanTablePrecision = mgATanTableDecimalMux * 2;
	mgATanTableDecimalMux--; // This represents zero in the table as we are going both into positives and negatives.
	mgATanTable = new double[mgATanTablePrecision];
	TableIndex = 0;

	//std::cout << mgATanTableDecimalMux << " " << mgATanTablePrecision << std::endl;

	// IN PROGRESS!!!!
	/*
	while (TableIndex < mgATanTablePrecision)
	{
		mgATanTable[TableIndex] = radians2degrees(atan(((double)TableIndex - (double)mgATanTableDecimalMux)));
		TableIndex++;
	}
	*/
}