#include "3D Scan Processing.h"

void LayerGenerator::readObj(string name, vector<double> *in1, vector<double> *in2, vector<double> *in3) {
	ifstream objFile;

	// Modify to accept arguments
	objFile.open(name);

	// Check to see if file opened
	if (!objFile) {
		cerr << "Unable to open file";
		exit(1);
	}

	string input;

	///*
	cout << "Reading in point cloud..." << endl;

	while (!objFile.eof()) {
		objFile >> input;
		if (input.compare("v") == 0) {
			objFile >> input;
			in1->push_back(atof(input.c_str()));
			//cout << input << " x " << count << endl;
			objFile >> input;
			in2->push_back(atof(input.c_str()));
			//cout << input << " y " << count << endl;
			objFile >> input;
			in3->push_back(atof(input.c_str()));
			//cout << input << " z " << count << endl;
		}
	}
	cout << "Point Cloud is in memory" << endl;
	objFile.close();
}

double LayerGenerator::diff(vector<double> v) {
	double min = 1;
	double max = -1;
	for (double d : v) {
		if (d < min) min = d;
		if (d > max) max = d;
	}
	return max - min;
}

void LayerGenerator::calculateAxes(vector<double> *in1, vector<double> *in2, vector<double> *in3) {
	double diff1 = diff(*in1);
	double diff2 = diff(*in2);
	double diff3 = diff(*in3);
	cout << "Difference 1: " << diff1 << endl;
	cout << "Difference 2: " << diff2 << endl;
	cout << "Difference 3: " << diff3 << endl;

	// normalize order
	// assume x is left/right (shoulder to shoulder)
	// z is front/back (Chest to back)
	// y is up/down (height)
	if (diff1 > diff2 && diff1 > diff3) {
		// diff1 is y axis
		if (diff2 > diff3) {
			// diff2 is x axis
			// diff3 is z axis
			//order = 213;
			in1->swap(*in2);
		}
		else {
			// diff3 is x axis
			// diff2 is z axis
			//order = 312;
			in1->swap(*in2);
			in2->swap(*in3);
		}
	}
	else if (diff2 > diff1 && diff2 > diff3) {
		// diff2 is y axis
		if (diff1 > diff3) {
			//diff1 is x axis
			//diff3 is z axis
			//order = 123;
		}
		else {
			//diff3 is x axis
			//diff1 is z axis
			//order = 321;
			in1->swap(*in3);
		}
	}
	else {
		// diff3 is y axis
		if (diff1 > diff2) {
			//order = 132;
			// diff1 is x axis
			// diff2 is z axis
			in2->swap(*in3);
		}
		else {
			// diff2 is x axis
			// diff1 is z axis
			//order = 231;
			in1->swap(*in3);
			in2->swap(*in3);
		}
	}

	// Write coordinates to file
	cout << "Writing coordinates to file" << endl;
	ofstream output1;
	output1.open("point_cloud_front.txt");
	for (int i = 0; i < (in1->size()); i++) {
		output1 << in1->at(i) << " " << in2->at(i) << endl;
	}
	output1.close();

	ofstream output2;
	output2.open("point_cloud_side.txt");
	for (int i = 0; i < (in2->size()); i++) {
		output2 << in3->at(i) << " " << in2->at(i) << endl;
	}
output2.close();

cout << "Coordinates written, plotting..." << endl;
}

// adopted from http://www.geeksforgeeks.org/merge-sort/
void LayerGenerator::mergeSort(vector<double> *x, vector<double> *y, vector<double> *z, int l, int r) {
	if (l < r) {
		int m = l + (r - l) / 2;
		mergeSort(x, y, z, l, m);
		mergeSort(x, y, z, m + 1, r);
		merge(x, y, z, l, m, r);
	}
}

// adopted from http://www.geeksforgeeks.org/merge-sort/
void LayerGenerator::merge(vector<double> *x, vector<double> *y, vector<double> *z, int l, int m, int r) {
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;

	// Create temporary vectors
	vector<double> xL, xR, yL, yR, zL, zR;
	for (i = 0; i < n1; i++) {
		xL.push_back(x->at(l + i));
		yL.push_back(y->at(l + i));
		zL.push_back(z->at(l + i));
	}
	for (j = 0; j < n2; j++) {
		xR.push_back(x->at(m + 1 + j));
		yR.push_back(y->at(m + 1 + j));
		zR.push_back(z->at(m + 1 + j));
	}

	// merge temp vectors back into original vector
	i = 0; // index of first subarray
	j = 0; // index of second subarray
	k = l; // index of merged subarray

	while (i < n1 && j < n2) {
		if (yL.at(i) <= yR.at(j)) {
			x->at(k) = xL.at(i);
			y->at(k) = yL.at(i);
			z->at(k) = zL.at(i);
			i++;
		}
		else {
			x->at(k) = xR.at(j);
			y->at(k) = yR.at(j);
			z->at(k) = zR.at(j);
			j++;
		}
		k++;
	}

	// copy remaining elements of ?L, if there are any
	while (i < n1) {
		x->at(k) = xL.at(i);
		y->at(k) = yL.at(i);
		z->at(k) = zL.at(i);
		i++;
		k++;
	}

	// copy remaining elements of ?R, if there are any
	while (j < n2) {
		x->at(k) = xR.at(j);
		y->at(k) = yR.at(j);
		z->at(k) = zR.at(j);
		j++;
		k++;
	}
}

vector<Layer> LayerGenerator::generateLayers(vector<double> *x, vector<double> *y, vector<double> *z, int numLayers) {
	vector<Layer> layers;
	double* values = new double[numLayers];
	double step = 2.0 / numLayers;
	double value = -1 - step;
	int leftIndex = -1;
	int rightIndex = -1;
	for (int i = 0; i < numLayers; i++) {
		value += step;
		//cout << "Value at step " << i << " is " << value << endl;
		values[i] = value;
		vector<double> xx;
		vector<double> zz;
		for (int j = rightIndex + 1; j < x->size(); j++) {
			if (y->at(j) >= value - (step / 2) && y->at(j) < value + (step / 2)) {
				xx.push_back(x->at(j));
				zz.push_back(z->at(j));
			}
		}
		layers.push_back(Layer(xx, value, zz));
	}
	return layers;
}

void LayerGenerator::generateImage(vector<double> *z, vector<double> *y) {
	vector<int> data;
	for (int i = 0; i < (RESOLUTION * RESOLUTION); i++) {
		data.push_back(0);
	}
	for (int i = 0; i < z->size(); i++) {
		vector<double> c;
		c.push_back(z->at(i));
		c.push_back(y->at(i));
		int index = convertToPixel(c);
		data.at(index) = 255;
		//cout << index << endl;
	}
	SaveBitmap(&data);
}

int LayerGenerator::SaveBitmap(vector<int> *data) {
	int  size;
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;

	ofstream out;
	out.open("model_image.bmp", ios::binary);
	if (!out.is_open()) {
		cerr << "Error: Could not open bitmap file for writing" << endl;
		return -1;
	}

	size = RESOLUTION * RESOLUTION * 3;

	// File Header
	bfh.bfType = 0x4D42;
	out.write((char*)&bfh.bfType, 2); // bfType 2
	bfh.bfSize = size + 54;
	out.write((char*)&bfh.bfSize, 4); // bfSize 4
	bfh.bfReserved1 = 0;
	out.write((char*)&bfh.bfReserved1, 2); // bfReserved1 2
	bfh.bfReserved2 = 0;
	out.write((char*)&bfh.bfReserved1, 2); // bfReserved2 2
	bfh.bfOffBits = 14;
	out.write((char*)&bfh.bfOffBits, 4); // bfOffBits 4

	// Image Header
	bih.biSize = 40;
	out.write((char*)&bih.biSize, 4); // biSize 4
	bih.biWidth = RESOLUTION;
	out.write((char*)&bih.biWidth, 4); // biWidth 4
	bih.biHeight = RESOLUTION;
	out.write((char*)&bih.biHeight, 4); // biHeight 4
	bih.biPlanes = 1;
	out.write((char*)&bih.biPlanes, 2); // biPlanes 2
	bih.biBitCount = 24;
	out.write((char*)&bih.biBitCount, 2); // biBitCount 2
	bih.biCompression = 0;
	out.write((char*)&bih.biCompression, 4); // biCompression 4
	bih.biSizeImage = 0;
	out.write((char*)&bih.biSizeImage, 4); // biSizeImage 4
	bih.biXPelsPerMeter = size;
	out.write((char*)&bih.biXPelsPerMeter, 4); // biXPelsPerMeter 4
	bih.biYPelsPerMeter = 0;
	out.write((char*)&bih.biYPelsPerMeter, 4); // biYPelsPerMeter 4
	bih.biClrUsed = 0;
	out.write((char*)&bih.biClrUsed, 4); // biClrUsed 4
	bih.biClrImportant = 0;
	out.write((char*)&bih.biClrImportant, 4); // biClrImportant 4

	if (data->size() != (RESOLUTION * RESOLUTION)) {
		cerr << "Error: incorrect data size for bitmap output" << endl;
		return -1;
	}
	
	for (int i = 0; i < (RESOLUTION * RESOLUTION); i++) {
		char a = 0;
		if (data->at(i) == 255) {
			out.write((char *)&a, 1);
			out.write((char *)&a, 1);
			out.write((char *)&data->at(i), 1);
		}
		else {
			char b = 255;
			out.write((char *)&b, 1);
			out.write((char *)&b, 1);
			out.write((char *)&b, 1);
		}
		
	}

	out.close();
	return 0;
}

int LayerGenerator::convertToPixel(vector<double> coordinates) {
	int mult = RESOLUTION / 2;
	int z = coordinates.at(0) * mult + mult;
	int y = coordinates.at(1) * mult + mult;
	//cout << y * RESOLUTION + z << endl;
	return y * RESOLUTION + z;
}