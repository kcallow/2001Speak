#include "XteWrapper.h"
#include <math.h>

double spiralRadius(double a, double b, double theta) {
	return a * exp(b * theta);
}

void mouseSpiral(double a, double b) {
	double step = 0.05;
	double maxTheta = 8;
	double radius;
	int x, y;
	for (double theta = 0; 0 < maxTheta; theta += step) {
		radius = spiralRadius(a,b,theta);
		x = radius * cos(theta);
		y = radius * sin(theta);
		XteWrapper::mousermove(to_string(x),to_string(y));
		//XteWrapper::sleep("1");
	}
}

int main() {
	XteWrapper::mousermove("24","200");
	XteWrapper::mouseclick("3");
	return 0;
}
