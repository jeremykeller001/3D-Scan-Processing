#pragma once
#include <chrono>
#include <fstream>
#include <iostream>
#include <stack>
#include <stdlib.h>
#include <string>
#include <thread>
#include <vector>
using namespace std;


#ifndef Layer_h
#define Layer_h

class Layer {
public:
	Layer(vector<double> x, double y, vector<double> z);
	double convexHull();
	vector<double> getX();
	double getY();
	vector<double> getZ();
	int numPoints();
	double xDiff();
	double zDiff();

private:
	struct Point {
		double xp, yp;
	};
	vector<double> x;
	vector<double> z;
	double y;
	int compare(const void *vp1, const void *vp2);
	double distanceCalc(Point p1, Point p2);
	Point nextToTop(stack<Point> &S);
	int orientation(Point p, Point q, Point r);
	void swap(Point &p1, Point &p2);
};

#endif

#ifndef Layer_Generator_h
#define Later_Generator_h

class LayerGenerator{
public:
	static void readObj(string name, vector<double> *in1, vector<double> *in2, vector<double> *in3);
	static double diff(vector<double> v);
	static void calculateAxes(vector<double> *in1, vector<double> *in2, vector<double> *in3);
	static void mergeSort(vector<double> *x, vector<double> *y, vector<double> *z, int l, int r);
	static void merge(vector<double> *x, vector<double> *y, vector<double> *z, int l, int m, int r);
	static vector<Layer> generateLayers(vector<double> *x, vector<double> *y, vector<double> *z, int numLayers);
};

#endif
