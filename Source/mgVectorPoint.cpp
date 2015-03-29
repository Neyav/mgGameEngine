#include <math.h>

#include "mgVectorPoint.h"

// The square roots in this source file are a point of much contention for me. I keep reading and hearing that square roots are expensive in terms of
// processing time, and I would like, as much as possible, to limit them or eliminate them. Other people just use the results of Y^2 + X^2 without square rooting
// them for the purpose of comparing lengths and distances but I use them for more than that here, and they get used often. I would like an alternative that
// is less expensive but produces the same results for calculating the distance between two points. I suspect that if such a thing existed, it would have already been
// discovered but my mind is set to task regardless. I might just cave and accept square roots as a part of my life for this project.

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
