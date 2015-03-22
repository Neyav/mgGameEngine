#include <math.h>

#include "mgVectorPoint.h"

#include "mgMathFunc.h"

// The square roots in this source file are a point of much contention for me. I keep reading and hearing that square roots are expensive in terms of
// processing time, and I would like, as much as possible, to limit them or eliminate them. Other people just use the results of Y^2 + X^2 without square rooting
// them for the purpose of comparing lengths and distances but I use them for more than that here, and they get used often. I would like an alternative that
// is less expensive but produces the same results for calculating the distance between two points. I suspect that if such a thing existed, it would have already been
// discovered but my mind is set to task regardless. I might just cave and accept square roots as a part of my life for this project.

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
	double Ypositive, Xpositive, Normalizer;

	Ypositive = Y;
	Xpositive = X;

	// For the purpose of calculating the normalizer these values cannot be negative. The normalizer is always positive.
	if (Ypositive < 0)
		Ypositive = -Ypositive;
	if (Xpositive < 0)
		Xpositive = -Xpositive;

	Normalizer = sqrt(Ypositive * Ypositive + Xpositive * Xpositive) / MagnitudeOverride;

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
		Ypositive = -Ypositive;
	if (Xpositive < 0)
		Xpositive = -Xpositive;

	Normalizer = sqrt(Ypositive * Ypositive + Xpositive * Xpositive) / Magnitude;

	TransformedY = Y = Y / Normalizer;
	TransformedX = X = X / Normalizer;
}

void mgVector::CalculateMagnitude(void)
{
	double PosX, PosY;

	PosX = X;
	PosY = Y;

	if (PosX < 0)
		PosX = -PosX;

	if (PosY < 0)
		PosY = -PosY;

	Magnitude = sqrt((PosX * PosX) + (PosY * PosY));
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
	X = cos(Radians);
	Y = sin(Radians);

	// Normalize it
	if (Magnitude != 1)
	{
		if (AutoNormalize)
			NormalizeVector();
		else
			CalculateMagnitude(); // True embelishment of chaos! This will only get hit here on a grave computing error, but I EMBRACE IT
								  // by accepting the reality that there is not one computer God, but MANY. I WANT MY 72 VIRGIN INTEGERS!
								  // EDIT: Was I fucking drunk? Setting Magnitude to a value that isn't one, and then disabiling AutoNormalize before
								  // calling this function would trigger this situation. Good thing I "embelished chaos". Fuck me. -- Chris.
	}
}

void mgVector::VectorFromDegrees(double Degrees)
{
#ifndef USEMGMATHFUNCH
	double Radians = Degrees * (mgPI / 180);

	VectorFromRadians(Radians);
#else
	X = mgCoSineDeg(Degrees);
	Y = mgSineDeg(Degrees);

	if (Magnitude != 1)
	{
		if (AutoNormalize)
			NormalizeVector();
		else
			CalculateMagnitude(); 
	}
#endif
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