#ifndef MGMATHFUNCH
#define MGMATHFUNCH

#include <math.h>

// Comment this out to revert to previous behavior
// 3/22/2015 -> According to the stress test this is not only slower than doing it properly, but 
//				approx 5.6x slower for mgVector::VectorFromDegrees... I am astounded and confused
//				but truth is truth. I will leave this here for reference material, but once I prove beyond a shadow
//				of a doubt that it is slower, it is scrap metal.
//#define USEMGMATHFUNCH 

void mgSineStartup(unsigned int precision);
double mgSineDeg(double Degrees);

void mgCoSineStartup(unsigned int precision);
double mgCoSineDeg(double Degrees);

void mgATanStartup(unsigned int precision);
//double mgATanDeg(double parameter);

#endif