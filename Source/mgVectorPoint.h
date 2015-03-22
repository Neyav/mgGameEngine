#ifndef MGVECTORPOINTH
#define MGVECTORPOINTH

#define mgPI 3.14159265359

struct mgPoint
{
	double Y;
	double X;
};

double DistanceBetweenPoints(mgPoint Start, mgPoint End);

// =------------------------------------=
// = mgVector C++ class                 =
// =------------------------------------=
//
// Responsible for both the simplistic handling of Y, X coordinate transferal as a function return
// or for the handling of creating a vector of variable magnitudes from two points and then the transforming
// of that vector to obtain the points inbetween two coordinates 
class mgVector
{
public:
	double Y;
	double X;
	double TransformedY;
	double TransformedX;
	double Magnitude = 1; // Default magnitude is 1 for all vectors

	bool AutoNormalize = true; // Set to true when setting a vector is intended to normalize it to 1.
								// If false, the Magnitude will instead be overwritten.

	// Operator overloading
	double operator*(const mgVector& other); // Returns the dot product
	mgVector operator*(const double& scalar); // multiplies it by a scalar
	mgVector operator+(const mgVector& other); // addition
	mgVector operator-(const mgVector& other); // subtraction

	// Class functions
	void NormalizeVector(double MagnitudeOverride);
	void NormalizeVector(void);
	void CalculateMagnitude(void);
	void VectorFromCoord(double Y1, double X1, double Y2, double X2);
	void VectorFromPoints(mgPoint Start, mgPoint End);
	void VectorFromRadians(double Radians);
	void VectorFromDegrees(double Degrees);
	void VectorStepCoords(double VectorStep);
};
#endif