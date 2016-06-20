#include "utmcalculations.h"
#include "utmpoint.h"
#include "utmpoint.h"
#include <iostream>
#include "Point.h"


UtmCalculations::UtmCalculations()
{

}

bool UtmCalculations::downCompare(UTMPoint point1, UTMPoint point2)
{

	if (point1.Hemisphere < point2.Hemisphere)
	{
		return true;
	}
	else
	{
		if (point1.Hemisphere == point2.Hemisphere)
		{
			return (point1.Northing < point2.Northing);
		}
		else
		{
			return false;
		}

	}
}

bool UtmCalculations::leftCompare(UTMPoint point1, UTMPoint point2)
{

	if (point1.Zone < point2.Zone)
	{
		return true;
	}
	else
	{
		if (point1.Zone == point2.Zone)
		{
			return (point1.Easting < point2.Easting);
		}
		else
		{
			return false;
		}
	}
}

void UtmCalculations::offset(vector<Point> &utmPoints, UTMPoint leftPoint, UTMPoint downPoint)
{
	for (unsigned int i = 0; i<utmPoints.size(); i++)
	{
		utmPoints[i].x -= leftPoint.Easting;
		utmPoints[i].y -= downPoint.Northing;
	}
}


Point UtmCalculations::LLtoUTM(Point point)
{
	double LongDegree = point.y;
	double LatDegree = point.x;

	double UTMEastingMeter, UTMNorthingMeter;

	const double k0 = 0.9996;

	// Make sure the longitude is between -180.00 .. 179.9
	double LongTemp = (LongDegree + 180) - int((LongDegree + 180) / 360) * 360 - 180; // -180.00 .. 179.9;
	double LatRad = LatDegree*deg2rad;
	double LongRad = LongTemp*deg2rad;

	int ZoneNumber = int((LongTemp + 180) / 6) + 1;

	if (LatDegree >= 56.0 && LatDegree < 64.0 && LongTemp >= 3.0 && LongTemp < 12.0) ZoneNumber = 32;

	// Special zones for Svalbard
	if (LatDegree >= 72.0 && LatDegree < 84.0)
	{
		if (LongTemp >= 0.0  && LongTemp <  9.0) ZoneNumber = 31;
		else if (LongTemp >= 9.0  && LongTemp < 21.0) ZoneNumber = 33;
		else if (LongTemp >= 21.0 && LongTemp < 33.0) ZoneNumber = 35;
		else if (LongTemp >= 33.0 && LongTemp < 42.0) ZoneNumber = 37;
	}

	double LongOriginRad = ((ZoneNumber - 1) * 6 - 180 + 3) * deg2rad;  // + 3 puts origin in middle of zone

																		// compute the UTM Zone from the latitude and longitude
																		//sprintf(UTMZone, "%d%c", ZoneNumber, UTMLetterDesignator(LatDegree));

	double equatorial_radius = 6378137;
	double eccentricity_squared = 0.00669437999013;

	double 	eccentricity_prime_squared = (eccentricity_squared) / (1 - eccentricity_squared);

	double N = equatorial_radius / sqrt(1 - eccentricity_squared*sin(LatRad)*sin(LatRad));
	double T = tan(LatRad)*tan(LatRad);
	double C = eccentricity_prime_squared*cos(LatRad)*cos(LatRad);
	double A = cos(LatRad)*(LongRad - LongOriginRad);

	double M = equatorial_radius*((1 - eccentricity_squared / 4 - 3 * eccentricity_squared*eccentricity_squared / 64 - 5 * eccentricity_squared*eccentricity_squared*eccentricity_squared / 256)*LatRad
		- (3 * eccentricity_squared / 8 + 3 * eccentricity_squared*eccentricity_squared / 32 + 45 * eccentricity_squared*eccentricity_squared*eccentricity_squared / 1024)*sin(2 * LatRad)
		+ (15 * eccentricity_squared*eccentricity_squared / 256 + 45 * eccentricity_squared*eccentricity_squared*eccentricity_squared / 1024)*sin(4 * LatRad)
		- (35 * eccentricity_squared*eccentricity_squared*eccentricity_squared / 3072)*sin(6 * LatRad));

	UTMEastingMeter = (double)(k0*N*(A + (1 - T + C)*A*A*A / 6
		+ (5 - 18 * T + T*T + 72 * C - 58 * eccentricity_prime_squared)*A*A*A*A*A / 120)
		+ 500000.0);

	UTMNorthingMeter = (double)(k0*(M + N*tan(LatRad)*(A*A / 2 + (5 - T + 9 * C + 4 * C*C)*A*A*A*A / 24
		+ (61 - 58 * T + T*T + 600 * C - 330 * eccentricity_prime_squared)*A*A*A*A*A*A / 720)));

	if (LatDegree < 0)
	{
		UTMNorthingMeter += 10000000.0; //10000000 meter offset for southern hemisphere
	}

	return Point(UTMEastingMeter, UTMNorthingMeter, point.z, 0);
}

vector<Point> UtmCalculations::Convert(vector<Point> LLPoints)
{
	double x, y;
	int z;

	int ggg;

	vector <Point> result;

	cout << "UTM\n";



	for (int i = 0; i<LLPoints.size(); i++)
	{


		result.push_back(LLtoUTM(LLPoints[i]));

		/*	if (UtmCalculations::downCompare(temp, LowermostUtmPoint))
		{
		LowermostUtmPoint = temp;
		}

		if (UtmCalculations::leftCompare(temp, LowermostUtmPoint))
		{
		LeftmostUtmPoint = temp;
		}*/
	}

	//offset(result, LeftmostUtmPoint, LowermostUtmPoint);

	return result;
}




