#ifndef MGVECTORPOINTH
#define MGVECTORPOINTH

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
	double Magnitude;

	void NormalizeVector(double MagnitudeOverride);
	void NormalizeVector(void);
	void VectorFromCord(double Y1, double X1, double Y2, double X2);
	void VectorFromPoints(mgPoint Start, mgPoint End);
	void VectorFromRadians(double Radians);
	void VectorFromDegrees(double Degrees);
	void VectorStepCords(double VectorStep);

	mgVector();
};
#endif