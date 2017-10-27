#pragma once
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <stack>
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

	double calculateDiff();

private:
	struct Point {
		double xp, yp;
	};
	vector<double> x;
	vector<double> z;
	vector<double> leftArmX;
	vector<double> leftArmZ;
	vector<double> rightArmX;
	vector<double> rightArmZ;
	vector<double> leftLegX;
	vector<double> leftLegZ;
	vector<double> rightLegX;
	vector<double> rightLegZ;
	
	int lowerBoundMax1;
	int lowerBoundMax2;
	double y;
	double distanceCalc(Point p1, Point p2);
	double distanceCalc(vector<Point> p);
	int orientation(Point p, Point q, Point r);
	void swap(double &v1, double &v2);
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

#ifndef GNUPLOT_H_
#define GNUPLOT_H_

// obtained from: http://liuxingguang.blogspot.com/2012/05/how-to-call-gnuplot-from-c.html
class Gnuplot {
public:
	Gnuplot();
	~Gnuplot();
	void operator ()(const string & command);
	// send any command to gnuplot
protected:
	FILE *gnuplotpipe;
};
Gnuplot::Gnuplot() {
	// with -persist option you will see the windows as your program ends
	//gnuplotpipe=_popen("gnuplot -persist","w");
	//without that option you will not see the window
	// because I choose the terminal to output files so I don't want to see the window
	gnuplotpipe = _popen("C:\\\"Program Files\"\\gnuplot\\bin\\gnuplot.exe", "w");
	if (!gnuplotpipe) {
		cerr << ("Gnuplot not found !");
	}
}
Gnuplot::~Gnuplot() {
	fprintf(gnuplotpipe, "exit\n");
	_pclose(gnuplotpipe);
}
void Gnuplot::operator()(const string & command) {
	fprintf(gnuplotpipe, "%s\n", command.c_str());
	fflush(gnuplotpipe);
	// flush is necessary, nothing gets plotted else
};
#endif
