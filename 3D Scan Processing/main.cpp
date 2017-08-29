#include "3D Scan Processing.h"
#include "GNU_plot.h"

int main(int argc, char* argv[]) {
	// Read in point cloud
	vector<double> in1;
	vector<double> in2;
	vector<double> in3;
	LayerGenerator::readObj("model_mesh.obj", &in1, &in2, &in3);

	// Calculate which axes are which
	LayerGenerator::calculateAxes(&in1, &in2, &in3);

	// Plot 2D Coordinates (XY/XZ)
	Gnuplot plot1;
	plot1("set size ratio - 1");
	plot1("plot [-1:1] [-1:1] \"point_cloud_front.txt\" with points");
	system("pause");
	plot1("plot [-1:1] [-1:1] \"point_cloud_side.txt\" with points");
	system("pause");

	// Sort by Z Axis
	cout << "Sorting..." << endl;
	LayerGenerator::mergeSort(&in1, &in2, &in3, 0, (in1.size() - 1));

	/*
	for (int i = 0; i < in1.size(); i++) {
		cout << in1.at(i) << "  " << in2.at(i) << "  " << in3.at(i) << endl;
	}
	*/

	vector<Layer> layers = LayerGenerator::generateLayers(&in1, &in2, &in3, 500);
	for (int i = 0; i < layers.size(); i++) {
		if (layers.at(i).numPoints() > 50) {
			cout << "Number of points at layer y: " << layers.at(i).getY() << " is " << layers.at(i).numPoints() << endl;
			cout << "Convex Hull Analysis: " << layers.at(i).convexHull() << endl;
			ofstream output;
			output.open("point_cloud_layer.txt");
			vector<double> x = layers.at(i).getX();
			vector<double> z = layers.at(i).getZ();
			for (int j = 0; j < x.size(); j++) {
				output << x.at(j) << " " << z.at(j) << endl;
			}
			output.close();
			plot1("plot [-1:1] [-1:1] \"point_cloud_layer.txt\" with points");
			chrono::milliseconds ms(250);
			this_thread::sleep_for(ms);
		}
	}

	return 0;
}