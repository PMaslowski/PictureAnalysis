

#include "utmcalculations.h"
#include "utmpoint.h"
#include "utmpoint.h"
#include <iostream>
#include "Point.h"


void UtmCalculations::LLtoUTM(int eId, double Lat, double Long, double& Northing, double& Easting, int& Zone) {
	
	double a = 6378137;

	double fr = 298.257223563;

	double ee = 2 / fr - 1 / (fr*fr);
	Long -= int((Long + 180) / 360) * 360;		
	double N, T, C, A, M;
	double LatRad = Lat*deg2rad;
	double LongRad = Long*deg2rad;

	Zone = int((Long + 186) / 6);
	if (Lat >= 56.0 && Lat < 64.0 && Long >= 3.0 && Long < 12.0)  Zone = 32;
	if (Lat >= 72.0 && Lat < 84.0) {	
		if (Long >= 0.0  && Long <  9.0)  Zone = 31;
		else if (Long >= 9.0  && Long < 21.0)  Zone = 33;
		else if (Long >= 21.0 && Long < 33.0)  Zone = 35;
		else if (Long >= 33.0 && Long < 42.0)  Zone = 37;
	}
	double LongOrigin = Zone * 6 - 183;			
	double LongOriginRad = LongOrigin * deg2rad;

	double EE = ee / (1 - ee);

	N = a / sqrt(1 - ee*sin(LatRad)*sin(LatRad));
	T = tan(LatRad)*tan(LatRad);
	C = EE*cos(LatRad)*cos(LatRad);
	A = cos(LatRad)*(LongRad - LongOriginRad);

	M = a*((1 - ee / 4 - 3 * ee*ee / 64 - 5 * ee*ee*ee / 256) *LatRad
		- (3 * ee / 8 + 3 * ee*ee / 32 + 45 * ee*ee*ee / 1024) *sin(2 * LatRad)
		+ (15 * ee*ee / 256 + 45 * ee*ee*ee / 1024) *sin(4 * LatRad)
		- (35 * ee*ee*ee / 3072) *sin(6 * LatRad));

	Easting = k0*N*(A + (1 - T + C)*A*A*A / 6 + (5 - 18 * T + T*T + 72 * C - 58 * EE)*A*A*A*A*A / 120) + 500000.0;

	Northing = k0*(M + N*tan(LatRad)*(A*A / 2 + (5 - T + 9 * C + 4 * C*C)*A*A*A*A / 24
		+ (61 - 58 * T + T*T + 600 * C - 330 * EE)*A*A*A*A*A*A / 720));
}


/******************************************************************************************************************************/

UtmCalculations::UtmCalculations()
{

}

bool UtmCalculations::downCompare(Point point1, Point point2)
{
					
			return (point1.y < point2.y);
		
	
}

bool UtmCalculations::leftCompare(Point point1, Point point2)
{

	if (point1.error < point2.error)
	{
		return true;
	}
	else
	{
		return (point1.x < point2.x);
	}
	
}

void UtmCalculations::offset(vector<Point> &utmPoints, Point leftPoint, Point downPoint)
{
	for (unsigned int i = 0; i<utmPoints.size(); i++)
	{					
			utmPoints[i].x -= leftPoint.x;
			utmPoints[i].y -= downPoint.y;						
	}
}

vector<Point> UtmCalculations::Convert(vector<Point> LLPoints)
{
	
	vector <Point> result;

	double a, b, N, S;
	int q;
		
	a = LLPoints[0].x;
	b = LLPoints[0].y;

	LLtoUTM(1, a, b, N, S, q);

	Point LeftmostUtmPoint = Point(S,N,LLPoints[0].z,q);
	Point LowermostUtmPoint = Point(S, N, LLPoints[0].z, q);

	for (int i = 0; i < LLPoints.size(); i++)
	{

		a = LLPoints[i].x;
		b = LLPoints[i].y;

		LLtoUTM(1, a, b, N, S, q);

		Point temp = Point(S, N, LLPoints[i].z, q);

		if (i == 1)
		{
			LeftmostUtmPoint = temp;
			LowermostUtmPoint = temp;
		}
						
		result.push_back(temp);
		
		if (UtmCalculations::downCompare(temp, LowermostUtmPoint))
		{
			LowermostUtmPoint = temp;
		}

		if (UtmCalculations::leftCompare(temp, LeftmostUtmPoint))
		{
			LeftmostUtmPoint = temp;
		}
	}

	cout << LeftmostUtmPoint.x << " " << LeftmostUtmPoint.y << endl;
	cout << LowermostUtmPoint.x << " " << LowermostUtmPoint.y << endl;

	offset(result, LeftmostUtmPoint, LowermostUtmPoint);		
	

	return result;
}




