#include "utmcalculations.h"
#include "utmpoint.h"

#include "utmpoint.h"

#include <iostream>



#define	eClarke1866	5
#define	eGRS80		19
#define	eWGS72		2
#define	eWGS84		22

UtmCalculations::UtmCalculations()
{

}

bool UtmCalculations::downCompare(UTMPoint point1, UTMPoint point2)
{
	if (point1.Hemisphere < point2.Hemisphere)
	{
		return true;
	}
	else if (point1.Hemisphere == point2.Hemisphere)
	{
		if (point1.Northing < point2.Northing)
		{
			return true;
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
	else if (point1.Zone == point2.Zone)
	{
		if (point1.Easting < point2.Easting)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}



void UtmCalculations::offset(vector< vector<double> > &utmPoints, UTMPoint leftPoint, UTMPoint downPoint)
{
	for (unsigned int i = 0; i<utmPoints.size(); i++)
	{
		utmPoints[i][0] -= leftPoint.Easting;
		utmPoints[i][1] -= downPoint.Northing;  //Northing -= downPoint.Northing;
	}
}

UTMPoint UtmCalculations::LLtoUTM(int eId, double Lat, double Long, double& Northing, double& Easting, int& Zone) {
	// converts LatLong to UTM coords;  3/22/95: by ChuckGantz chuck.gantz@globalstar.com, from USGS Bulletin 1532.
	// Lat and Long are in degrees;  North latitudes and East Longitudes are positive.
	Ellipsoid ellip[23] = {
		//		 eId,  Name,		   EquatorialRadius,    1/flattening;
		(Ellipsoid(0, "Airy1830",		6377563.396,	299.3249646)),
		(Ellipsoid(1, "AiryModified",		6377340.189,	299.3249646)),
		(Ellipsoid(2, "AustralianNational",	6378160,	298.25)),
		(Ellipsoid(3, "Bessel1841Namibia",	6377483.865,	299.1528128)),
		(Ellipsoid(4, "Bessel1841",		6377397.155,	299.1528128)),
		(Ellipsoid(5, "Clarke1866",		6378206.4,	294.9786982)),
		(Ellipsoid(6, "Clarke1880",		6378249.145,	293.465)),
		(Ellipsoid(7, "EverestIndia1830",	6377276.345,	300.8017)),
		(Ellipsoid(8, "EverestSabahSarawak",	6377298.556,	300.8017)),
		(Ellipsoid(9, "EverestIndia1956",	6377301.243,	300.8017)),
		(Ellipsoid(10, "EverestMalaysia1969",	6377295.664,	300.8017)),	//Dana has no datum that uses this ellipsoid!
		(Ellipsoid(11, "EverestMalay_Sing",	6377304.063,	300.8017)),
		(Ellipsoid(12, "EverestPakistan",	6377309.613,	300.8017)),
		(Ellipsoid(13, "Fischer1960Modified",	6378155,	298.3)),
		(Ellipsoid(14, "Helmert1906",		6378200,	298.3)),
		(Ellipsoid(15, "Hough1960",		6378270,	297)),
		(Ellipsoid(16, "Indonesian1974",		6378160,	298.247)),
		(Ellipsoid(17, "International1924",	6378388,	297)),
		(Ellipsoid(18, "Krassovsky1940",		6378245,	298.3)),
		(Ellipsoid(19, "GRS80",			6378137,	298.257222101)),
		(Ellipsoid(20, "SouthAmerican1969",	6378160,	298.25)),
		(Ellipsoid(21, "WGS72",			6378135,	298.26)),
		(Ellipsoid(22, "WGS84",			6378137,	298.257223563)) };





	double a = ellip[eId].EquatorialRadius;
	double ee = ellip[eId].eccSquared;
	Long -= int((Long + 180) / 360) * 360;			//ensure longitude within -180.00..179.9
	double N, T, C, A, M;
	double LatRad = Lat*deg2rad;
	double LongRad = Long*deg2rad;

	Zone = int((Long + 186) / 6);
	if (Lat >= 56.0 && Lat < 64.0 && Long >= 3.0 && Long < 12.0)  Zone = 32;
	if (Lat >= 72.0 && Lat < 84.0) {			//Special zones for Svalbard
		if (Long >= 0.0  && Long <  9.0)  Zone = 31;
		else if (Long >= 9.0  && Long < 21.0)  Zone = 33;
		else if (Long >= 21.0 && Long < 33.0)  Zone = 35;
		else if (Long >= 33.0 && Long < 42.0)  Zone = 37;
	}
	double LongOrigin = Zone * 6 - 183;			//origin in middle of zone
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


	char x = Northing < 0 ? 'S' : 'N';

	if (N<0) N += 10000000;

	UTMPoint point(Northing, Easting, Zone, x);

	return point;
}

vector< vector<double> > UtmCalculations::Convert(vector<vector <double> > LLPoints)
{

	double x, y;
	int z;

	UTMPoint LeftmostUtmPoint(0, 0, 1, 'c');
	UTMPoint LowermostUtmPoint(0, 0, 1, 'c');

	UTMPoint temp = LLtoUTM(eWGS84, LLPoints[0][1], LLPoints[0][0], x, y, z);

	vector < vector<double> > result;

	LeftmostUtmPoint = temp;
	LowermostUtmPoint = temp;

	for (int i = 0; i<LLPoints.size(); i++)
	{
		UTMPoint temp = LLtoUTM(eWGS84, LLPoints[i][1], LLPoints[i][0], x, y, z);

		vector<double> tempvect;
		tempvect.push_back(temp.Easting);
		tempvect.push_back(temp.Northing);
		tempvect.push_back(LLPoints[i][2]);
		result.push_back(tempvect);

		if (UtmCalculations::downCompare(temp, LowermostUtmPoint))
		{
			LowermostUtmPoint = temp;
		}

		if (UtmCalculations::leftCompare(temp, LowermostUtmPoint))
		{
			LeftmostUtmPoint = temp;
		}
	}

	offset(result, LeftmostUtmPoint, LowermostUtmPoint);

	return result;
}















































































