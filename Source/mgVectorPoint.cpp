#include <math.h>

#include "mgVectorPoint.h"

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

	TransformedY = Y = Y / Normalizer;
	TransformedX = X = X / Normalizer;
}

void mgVector::NormalizeVector(void)
{
	double Normalizer;

	Normalizer = sqrt(Y * Y + X * X) / Magnitude;

	TransformedY = Y = Y / Normalizer;
	TransformedX = X = X / Normalizer;
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
void mgVector::VectorStepCoords(double VectorStep)
{
	TransformedY = Y * (VectorStep);
	TransformedX = X * (VectorStep);
}

mgVector::mgVector()
{
	Magnitude = 1; // Default magnitude is 1 for all vectors

	AutoNormalize = true; // Default behavior for vectors is to auto normalize
}
