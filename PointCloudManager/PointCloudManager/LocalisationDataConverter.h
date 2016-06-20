//#pragma once
/*
Gets: List of points.
Those points are bare from GPS system
Returns: List of points.
Those points are in UTM system
*/

#include "Point.h"
#include <vector>

using namespace std;

class LocalisationDataConverter
{

private:
	vector<Point*> pointsInUTM;


public:

	LocalisationDataConverter();
	~LocalisationDataConverter();
	vector<Point> getPointsInUTM(vector <Point> llPoints);
	vector<Point> getOriginalSelection(char *pathToPlyFile, vector<size_t> selectedPoints);
};

