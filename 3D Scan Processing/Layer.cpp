#include "3D Scan Processing.h"

Layer::Layer(vector<double> xx, double yy, vector<double> zz) {
	x = xx;
	y = yy;
	z = zz;
}

double Layer::convexHull() {
	// convert layer to set of points
	vector<Point> points;
	for (int i = 0; i < x.size(); i++) {
		Point p;
		p.xp = x.at(i);
		p.yp = z.at(i);
		points.push_back(p);
	}
	int n = points.size();

	// Find the bottom-most point
	double ymin = points[0].yp, min = 0;
	for (int i = 1; i < n; i++) {
		int y = points[i].yp;

		// Pick the bottom-most point
		// Choose the left-most point in case of tie
		if ((y < ymin) || (ymin == y && points[i].xp < points[min].xp)) {
			ymin = points[i].yp;
			min = i;
		}
	}

	// Place the bottom-most point at first position
	swap(points.at(0), points.at(min));

	// Sort n-1 points with respect to the first point
	// a A point p1 comes before p2 in sorted output if
	//	p2 has larger polar angle (in ccw direction) than p1
	p0 = points.at(0);
	points = insertionSort(points);

	// If two or more points make the same angle with p0,
	// Remove all but the one that is farthest from p0.
	int m = 1;
	for (int i = 1; i < n; i++) {
		// Keep removing i while angle of i and i+1 is same with respect to p0
		while (i < n - 1 && orientation(p0, points.at(i), points.at(i + 1)) == 0) {
			i++;
		}

		points.at(m) = points.at(i);
		m++; // update size of modified vector
	}

	// If modified array of points has less than 3 points, convex hull is not possible
	if(m < 3) return -1;

	// Create an empty stack and push first three points to it
	stack<Point> S;
	S.push(points.at(0));
	S.push(points.at(1));
	S.push(points.at(2));

	for (int i = 3; i < m; i++) {
		// Keep removing top while the angle formed by 
		// points next-to-top, top, and points[i] makes a non-left turn
		while (orientation(nextToTop(S), S.top(), points.at(i)) != 2) S.pop();
		S.push(points.at(i));
	}

	return distanceCalc(S);
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

bool Layer::compare(Point p1, Point p2) {

	// Find orientation
	int o = orientation(p0, p1, p2);
	if (o == 0) {
		//return (distanceCalc(p0, p2) >= distanceCalc(p0, p1)) ? -1 : 1;
		return (distanceCalc(p0, p2) >= distanceCalc(p0, p1));
	}

	//return(o == 2) ? -1 : 1;
	return(o == 2);
}

double Layer::distanceCalc(Point p1, Point p2) {
	return (p1.xp - p2.xp) * (p1.xp - p2.xp) + (p1.yp - p2.yp) * (p1.yp - p2.yp);
}

double Layer::distanceCalc(stack<Point> &S) {
	double dist = 0;
	int n = S.size();

	for (int i = 1; i < n; i++) {
		Point p1 = S.top();
		S.pop();
		Point p2 = S.top();
		dist += distanceCalc(p1, p2);
	}

	return dist;
}

Layer::Point Layer::nextToTop(stack<Point> &S) {
	Point p = S.top();
	S.pop();
	Point res = S.top();
	S.push(p);
	return res;
}

int Layer::orientation(Point p, Point q, Point r) {
	int val = (q.yp - p.yp) * (r.xp - q.xp) -
		(q.xp - p.xp) * (r.yp - q.yp);

	if (val == 0) return 0; // colinear
	return (val > 0) ? 1 : 2; // clock or counterclock wise
}

vector<Layer::Point> Layer::insertionSort(vector<Point> p) {
	vector<Point> newP;
	newP.push_back(p.at(0));
	newP.push_back(p.at(1));
	for (int i = 2; i < p.size(); i++) {
		bool insert = false;
		for (int j = 1; j < newP.size(); j++) {
			if (compare(p.at(i), newP.at(j))) {
				newP.insert(newP.begin() + j, p.at(i));
				insert = true;
				break;
			}
		}
		if (!insert) newP.push_back(p.at(i));
	}

	return newP;
}

void Layer::swap(Point &p1, Point &p2) {
	Point temp = p1;
	p1 = p2;
	p2 = temp;
}