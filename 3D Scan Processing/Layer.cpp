#include "3D Scan Processing.h"

Layer::Layer(vector<double> xx, double yy, vector<double> zz) {
	x = xx;
	y = yy;
	z = zz;
}

double Layer::convexHull() {

	return 0;
}

vector<double> Layer::getX() {
	return x;
}

double Layer::getY() {
	return y;
}

vector<double> Layer::getZ() {
	return z;
}

int Layer::numPoints() {
	return x.size();
}

double Layer::xDiff() {
	double min = 1;
	double max = -1;
	for (double d : x) {
		if (d < min) min = d;
		else if (d > max) max = d;
	}
	return max - min;
}

double Layer::zDiff() {
	double min = 1;
	double max = -1;
	for (double d : z) {
		if (d < min) min = d;
		else if (d > max) max = d;
	}
	return max - min;
}