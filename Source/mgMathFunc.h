#ifndef MGMATHFUNCH
#define MGMATHFUNCH

#include <math.h>

// Comment this out to revert to previous behavior
#define USEMGMATHFUNCH 

void mgSineStartup(unsigned int precision);
double mgSineDeg(double Degrees);

void mgCoSineStartup(unsigned int precision);
double mgCoSineDeg(double Degrees);

void mgATanStartup(unsigned int precision);
//double mgATanDeg(double parameter);

#endif