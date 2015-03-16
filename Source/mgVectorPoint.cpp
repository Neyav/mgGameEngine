#include <math.h>

#include "mgVectorPoint.h"

double DistanceBetweenPoints(mgPoint Start, mgPoint End)
{
	double distance;

	distance = sqrt(pow(End.Y - Start.Y, 2) + pow(End.X - Start.X, 2));

	return distance;
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

	return product;
}

// produces a product of two vectors added together
mgVector mgVector::operator+(const mgVector& other)
{
	mgVector product;

	product.Y = Y + other.Y;
	product.X = X + other.X;

	return product;
}

// produces a product of two vectors subtracted from each other.
mgVector mgVector::operator-(const mgVector& other)
{
	mgVector product;

	product.Y = Y - other.Y;
	product.X = X - other.X;

	return product;
}


void mgVector::NormalizeVector(double MagnitudeOverride)
{
	double Ypositive, Xpositive, Normalizer;

	Ypositive = Y;
	Xpositive = X;

	// For the purpose of calculating the normalizer these values cannot be negative. The normalizer is always positive.
	if (Ypositive < 0)
		Ypositive *= -1;
	if (Xpositive < 0)
		Xpositive *= -1;

	Normalizer = (Ypositive + Xpositive) / MagnitudeOverride;

	TransformedY = Y = Y / Normalizer;
	TransformedX = X = X / Normalizer;
}

void mgVector::NormalizeVector(void)
{
	double Ypositive, Xpositive, Normalizer;

	Ypositive = Y;
	Xpositive = X;

	// For the purpose of calculating the normalizer these values cannot be negative. The normalizer is always positive.
	if (Ypositive < 0)
		Ypositive *= -1;
	if (Xpositive < 0)
		Xpositive *= -1;

	Normalizer = (Ypositive + Xpositive) / Magnitude;

	TransformedY = Y = Y / Normalizer;
	TransformedX = X = X / Normalizer;
}

void mgVector::VectorFromCoord(double Y1, double X1, double Y2, double X2)
{
	// Set out vector to the difference between the two points
	Y = Y2 - Y1;
	X = X2 - X1;

	// Normalize it.
	NormalizeVector();
}

void mgVector::VectorFromPoints(mgPoint Start, mgPoint End)
{
	VectorFromCoord(Start.Y, Start.X, End.Y, End.X);
}

void mgVector::VectorFromRadians(double Radians)
{
	X = cos(Radians);
	Y = sin(Radians);

	// Normalize it
	NormalizeVector();
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
	// This is our default magnitude for all vectors
	Magnitude = 1;
}