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

	product.Magnitude = Magnitude * scalar;

	return product;
}

// divides a vector by a scalar.
mgVector mgVector::operator/(const double& scalar)
{
	mgVector product;

	product.Y = Y / scalar;
	product.X = X / scalar;

	product.Magnitude = Magnitude / scalar;

	return product;
}

// produces a product of two vectors added together
mgVector mgVector::operator+(const mgVector& other)
{
	mgVector product;

	product.Y = Y + other.Y;
	product.X = X + other.X;

	product.Magnitude = Magnitude + other.Magnitude;

	return product;
}

// produces a product of two vectors subtracted from each other.
mgVector mgVector::operator-(const mgVector& other)
{
	mgVector product;

	product.Y = Y - other.Y;
	product.X = X - other.X;

	product.Magnitude = Magnitude - other.Magnitude;

	if (product.Magnitude < 0)
		product.Magnitude = -product.Magnitude; // No such thing as a negative magnitude here.

	return product;
}

void mgVector::ReverseDirection(void)
{ // Really simple, and the magnitude remains unchanged.
	Y = -Y;
	X = -X;
}

void mgVector::NormalizeVector(double MagnitudeOverride)
{
	double Normalizer;

	Magnitude = MagnitudeOverride;

	if ( Magnitude == 0 )
	{ // We know the answer, why risk a divide by zero.
		Y = X = 0;
		return;
	}

	Normalizer = sqrt(Y * Y + X * X) / MagnitudeOverride;

	Y = Y / Normalizer;
	X = X / Normalizer;
}

void mgVector::NormalizeVector(void)
{
	double Normalizer;

	if ( Magnitude == 0 )
	{ // We know the answer, why risk a divide by zero.
		Y = X = 0;
		return;
	}

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

void mgVector::VectorFromCoord(mgPoint StartCoord, mgPoint EndCoord)
{
	// Set out vector to the difference between the two points
	Y = EndCoord.Y - StartCoord.Y;
	X = EndCoord.X - StartCoord.X;

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
			Magnitude = 1;	// The results of sin/cos operations on a radian always result in a 1 magnitude vector.
							// There is no need to calculate anything here.
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

// Returns a vector of identical orientation to this one, but with a magnitude of 1.
mgVector mgVector::ReturnUnitVector(void)
{
	mgVector UnitVector;

	UnitVector = *this / Magnitude;

	return UnitVector;
}

// Returns a double representing this vectors projection along ProjectionAxis
double mgVector::ProjectAgainst(mgVector ProjectionAxis)
{
	double Projection;
	mgVector AxisUnitVector = ProjectionAxis.ReturnUnitVector();

	Projection = *this * AxisUnitVector;

	return Projection;
}

mgVector::mgVector()
{
	Magnitude = 1; // Default magnitude is 1 for all vectors
	Y = X = 0;

	AutoNormalize = true; // Default behavior for vectors is to auto normalize
}
