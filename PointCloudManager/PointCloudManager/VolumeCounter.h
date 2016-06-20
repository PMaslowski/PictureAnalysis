#pragma once


/*
Gets: List of points.
Those points are in UTM system
Returns: Given point cloud's volume in double, counted error in double
*/


#include <iostream>
#include <cstdio>
#include <vector>
#include <fstream>
#include <string>
#include "Point.h"
#include "tetgen.h"

using namespace std;

class VolumeCounter
{
private:
	double volume;
	double error;
	struct Tetrahedron;
	struct PointsArray;
	struct VolumeArray;
	VolumeArray fillArray(Tetrahedron tet);
	double distanceBetweenPoints(Point p1, Point p2);
	double determinant(VolumeArray volumeArray, int n);
	double calculateVolume(double determinant);
	void saveToFile(string path, vector <Point> pointsArray, int howManyInputs, int numberOfPoints, int howManyEdges);
	//vector<Point> getPointsFromFile(string filePath);
public:
	VolumeCounter(vector<Point> vector);
	double run_test(vector<Point> vector);
};

