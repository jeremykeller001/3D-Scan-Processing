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

	// Initialize result
	vector<Point> hull;

	// Find the leftmost point
	int l = 0;
	for (int i = 1; i < n; i++) {
		if (points.at(i).xp < points.at(l).xp) l = i;
	}

	// Start from leftmost point, keep moving counterclockwise
	// until reach the start point again.  This loop runs O(h)
	// times where h is number of points in result or output.
	int p = l, q;

	do
	{
		// Add current point to result
		hull.push_back(points.at(p));

		// Search for a point 'q' such that orientation(p, x,
		// q) is counterclockwise for all points 'x'. The idea
		// is to keep track of last visited most counterclock-
		// wise point in q. If any point 'i' is more counterclock-
		// wise than q, then update q.
		q = (p + 1) % n;

		for (int i = 0; i < n; i++) {
			// If i is more counterclockwise than current q, then
			// update q
			if (orientation(points[p], points[i], points[q]) == 2)
				q = i;
		}

		// Now q is the most counterclockwise with respect to p
		// Set p as q for next iteration, so that q is added to
		// result 'hull'
		p = q;

	} while (p != l);

	return distanceCalc(hull);
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

double Layer::calculateDiff() {
	double max1 = 0;
	double max2 = 0;
	lowerBoundMax1 = 0;
	lowerBoundMax2 = 0;
	double diff = -1;

	vector<double> xs;
	for (int i = 0; i < x.size(); i++) {
		xs.push_back(x.at(i));
	}
	sort(xs.begin(), xs.end());

	for (int i = 0; i < xs.size() - 1; i++) {
		diff = xs.at(i + 1) - xs.at(i);
		if (diff > max2) {
			if (diff > max1) {
				max2 = max1;
				max1 = diff;
				lowerBoundMax1 = i;
			}
			else {
				max2 = diff;
				lowerBoundMax2 = i;
			}
		}
	}
	cout << "Max1: " << max1 << endl;
	cout << "Max2: " << max2 << endl;
	return max1;
}

double Layer::distanceCalc(Point p1, Point p2) {
	return sqrt((p1.xp - p2.xp) * (p1.xp - p2.xp) + (p1.yp - p2.yp) * (p1.yp - p2.yp));
}

double Layer::distanceCalc(vector<Point> p) {
	double dist = 0;
	for (int i = 0; i < p.size() - 1; i++) {
		dist += distanceCalc(p.at(i), p.at(i + 1));

	}
	return dist;
}

int Layer::orientation(Point p, Point q, Point r) {
	double val = (q.yp - p.yp) * (r.xp - q.xp) -
		(q.xp - p.xp) * (r.yp - q.yp);
	
	if (val == 0) return 0; // colinear
	return (val > 0) ? 1 : 2; // clock or counterclock wise
}

void Layer::swap(double &v1, double &v2) {
	double temp = v1;
	v2 = v1;
	v1 = temp;
}