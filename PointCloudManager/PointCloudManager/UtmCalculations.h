#pragma once


#include "utmpoint.h"

#include "vector"
#include "Point.h"
#include "cmath"

using namespace std;

class UtmCalculations
{
public:
	UtmCalculations();
	
	vector<Point> Convert(vector<Point> LLPoints);
	



	void LLtoUTM(int eId, double Lat, double Long, double& Northing, double& Easting, int& Zone);

	


private:
	bool downCompare(Point point1, Point point2);
	bool leftCompare(Point point1, Point point2);
	void offset(vector<Point> &utmPoints, Point leftPoint, Point downPoint);
	
	double PI = 3.14159265;
	double deg2rad = PI / 180;
	double rad2deg = 180 / PI;
	double k0 = 0.9996;

	double FalseEasting = 500000.0;
	double EarthRadius = 6378137.0;
	double ScaleFactor = 0.9996;

};


