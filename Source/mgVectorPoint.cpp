#include <math.h>

#include "mgVectorPoint.h"

// =------------------------------------=
// = mgPoint C++ class                  =
// =------------------------------------=
//
// mgPoint is a container for a 2D position reference on the gameworld.
//
// Hilariously I originally named this file VectorPoint because the two are so similar in the sense that they are both Y and X values, and most
// applications use them interchangably. It originally just had the structure for a point, and a class for the vector because the vector class uses more
// memory than a point needs and that was bothering me. Now the mgPoint class has so many overloads it is really a proper and true class all on its own and
// the file name, that was previously just a nod to the mgPoint struct being in here, has true meaning.

mgPoint::mgPoint()
{
	Y = X = 0;
}

mgPoint mgPoint::operator+(const mgPoint& other)
{
	mgPoint product;

	product.Y = Y + other.Y;
	product.X = X + other.X;

	return product;
}

mgPoint mgPoint::operator-(const mgPoint& other)
{
	mgPoint product;

	product.Y = Y - other.Y;
	product.X = X - other.X;

	return product;
}

// Comparative operator overloads for quickly comparing mgPoints for storage in a binary search tree.
bool mgPoint::operator>(const mgPoint& other)
{
	if (Y == other.Y)
	{
		if (X > other.X)
			return true;
		else
			return false;
	}

	if (Y > other.Y)
		return true;
	else
		return false;
}

bool mgPoint::operator<(const mgPoint& other)
{
	if (Y == other.Y)
	{
		if (X < other.X)
			return true;
		else
			return false;
	}

	if (Y < other.Y)
		return true;
	else
		return false;
}

bool mgPoint::operator==(const mgPoint& other)
{
	if (Y == other.Y && X == other.X)
		return true;
	else
		return false;
}

double DistanceBetweenPoints(const mgPoint Start, const mgPoint End)
{
	double Y, X;

	Y = End.Y - Start.Y;
	X = End.X - Start.X;

	// A lot of straight lines are used, and since that is the case these two test case scenarios remove a lot of the "work"
	if (Y == 0)
		return X;
	else if (X == 0)
		return Y;
	else
		return sqrt(Y * Y + X * X);
}

// =------------------------------------=
// = mgVector C++ class                 =
// =------------------------------------=
//
// A Vector represents a direction. This class has a bunch of functions that facilitate ease of use to that purpose.

// produces the dot product of two vectors.
double mgVector::operator*(const mgVector& other)
{
	double dotproduct;

	dotproduct = (X * other.X) + (Y * other.Y);

	return dotproduct;
}

// produces a product vector by modifying a vector by a scalar value
mgVector mgVector::operator*(const double& scalar)
{
	mgVector product;

	product.Y = Y * scalar;
	product.X = X * scalar;

	product.CalculateMagnitude();

	return product;
}

// produces a product of two vectors added together
mgVector mgVector::operator+(const mgVector& other)
{
	mgVector product;

	product.Y = Y + other.Y;
	product.X = X + other.X;

	product.CalculateMagnitude();

	return product;
}

// produces a product of two vectors subtracted from each other.
mgVector mgVector::operator-(const mgVector& other)
{
	mgVector product;

	product.Y = Y - other.Y;
	product.X = X - other.X;

	product.CalculateMagnitude();

	return product;
}


void mgVector::NormalizeVector(double MagnitudeOverride)
{
	double Normalizer;

	Normalizer = sqrt(Y * Y + X * X) / MagnitudeOverride;

	Y = Y / Normalizer;
	X = X / Normalizer;
}

void mgVector::NormalizeVector(void)
{
	double Normalizer;

	Normalizer = sqrt(Y * Y + X * X) / Magnitude;

	Y = Y / Normalizer;
	X = X / Normalizer;
}

void mgVector::CalculateMagnitude(void)
{
	Magnitude = sqrt(X * X + Y * Y);
}

void mgVector::VectorFromCoord(double Y1, double X1, double Y2, double X2)
{
	// Set out vector to the difference between the two points
	Y = Y2 - Y1;
	X = X2 - X1;

	// Normalize it.
	if (AutoNormalize)
		NormalizeVector();
	else
		CalculateMagnitude();
}

void mgVector::VectorFromPoints(mgPoint Start, mgPoint End)
{
	VectorFromCoord(Start.Y, Start.X, End.Y, End.X);
}

void mgVector::VectorFromRadians(double Radians)
{
	X = sin(Radians);
	Y = cos(Radians);

	// Normalize it
	if (Magnitude != 1)
	{
		if (AutoNormalize)
			NormalizeVector();
		else
			CalculateMagnitude(); 
	}
}

void mgVector::VectorFromDegrees(double Degrees)
{
	double Radians = Degrees * (mgPI / 180);

	VectorFromRadians(Radians);
}

// -- Return the vector modified to represent "VectorStep" steps away from the center.
mgPoint mgVector::VectorStepCoords(double VectorStep)
{
	mgPoint Transformed;

	Transformed.Y = Y * (VectorStep);
	Transformed.X = X * (VectorStep);

	return Transformed;
}

mgVector::mgVector()
{
	Magnitude = 1; // Default magnitude is 1 for all vectors
	Y = X = 0;

	AutoNormalize = true; // Default behavior for vectors is to auto normalize
}
