#ifndef MGVECTORPOINTH
#define MGVECTORPOINTH

// Version string to faciliate quick identification of differences between uses of the included sources.
static char __mgVersion[] = "mgGameEngine 1-0";

// Helper Math functions. Too few for their own file.

#define mgPI 3.14159265359
#define mgSmallestValue(X, Y) (((X) < (Y)) ? (X) : (Y))
#define mgLargestValue(X, Y) (((X) < (Y)) ? (Y) : (X))

// floor function call is slow. Implement faster version.
inline int mgFloor(double value)
{
    return (int) value - (value<0); 
}

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

class mgPoint
{
public:
	double Y;
	double X;

	mgPoint();

	// Manipulative Overloads
	mgPoint operator+(const mgPoint& other); // addition
	mgPoint operator-(const mgPoint& other); // subtraction
	// Comparison Overloads
	bool operator>(const mgPoint& other);
	bool operator<(const mgPoint& other);
	bool operator==(const mgPoint& other);
};

double DistanceBetweenPoints(mgPoint Start, mgPoint End);

// =------------------------------------=
// = mgVector C++ class                 =
// =------------------------------------=
//
// A Vector represents a direction. This class has a bunch of functions that facilitate ease of use to that purpose.

class mgVector
{
public:
	double Y;
	double X;
	double Magnitude; // Magnitude represents the length of the vector

	bool AutoNormalize;		    // Set to true when setting a vector is intended to normalize it to 1.
								// If false, the Magnitude will instead be overwritten.

	// Operator overloading
	double operator*(const mgVector& other); // Returns the dot product
	mgVector operator*(const double& scalar); // multiplies it by a scalar
	mgVector operator/(const double& scalar); // divides it by a scalar
	mgVector operator+(const mgVector& other); // addition
	mgVector operator-(const mgVector& other); // subtraction

	void ReverseDirection(void);

	// Class functions
	void NormalizeVector(double MagnitudeOverride);
	void NormalizeVector(void);
	void CalculateMagnitude(void);
	void VectorFromCoord(mgPoint StartCoord, mgPoint EndCoord);
	void VectorFromCoord(double Y1, double X1, double Y2, double X2);
	void VectorFromPoints(mgPoint Start, mgPoint End);
	void VectorFromRadians(double Radians);
	void VectorFromDegrees(double Degrees);
	mgPoint VectorStepCoords(double VectorStep);
	mgVector ReturnUnitVector(void); // Returns a vector pointing in the same direction but with a magnitude of 1.
	double ProjectAgainst(mgVector ProjectionAxis);	// Returns a double representing this vectors projection along ProjectionAxis

	mgVector();
};
#endif
