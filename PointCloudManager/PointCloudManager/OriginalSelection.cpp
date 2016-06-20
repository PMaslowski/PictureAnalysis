#include "originalselection.h"
#include "stdlib.h"
#include "rply.h"
#include "ctime"
#include "iostream"
#include "Point.h"
#include <fstream>
using namespace std;

int OriginalSelection::currentIndex;
int OriginalSelection::count;
int OriginalSelection::count3;
vector<vector<double> > OriginalSelection::points;

bool OriginalSelection::pair;

OriginalSelection::OriginalSelection()
{

}

vector<Point> OriginalSelection::GetOriginalSelection(char * pathToFile, vector<size_t> selectedIndexes)
{
	int x = readPlyFile(pathToFile);
	vector<Point> w;
	for (int i = 0; i < points.size(); i++)
		w.push_back(Point(points[i][0], points[i][1], points[i][2], 0));

	return w;
	//return checkPoints(selectedIndexes);
}


int OriginalSelection::GetVertex(p_ply_argument argument) {
	long eol;

	ply_get_argument_user_data(argument, NULL, &eol);

	count3++;

	double value = ply_get_argument_value(argument);

	if (pair)
		points[currentIndex].push_back(value);

	if (eol)
	{
		pair = !pair;
		count3 = 0;
		currentIndex++;
		vector<double> a;
		points.push_back(a);
	}

	return 1;
}

vector<Point> OriginalSelection::getPointsFromFile(string filePath) {

	vector<Point> vectorOfPoints;
	double a, b, c;
	int m, n, o;
	int numberOfPoints;
	int tmp = 0;
	int i = 0;
	ifstream points(filePath);

	if (!points)
	{
		cout << "Nie mozna otworzyc pliku";
		getchar();
		return vectorOfPoints;
	}

	while (!points.eof()) {
		points >> a >> b >> c >> m >> n >> o;
		Point point(a, b, c, 0);
		vectorOfPoints.push_back(point);
		tmp++;
	}

	points.close();

	return vectorOfPoints;

}


int OriginalSelection::readPlyFile(char *path)
{
	size_t nvertices;

	currentIndex = 0;
	count3 = 0;
	pair = false;
	p_ply ply = ply_open(path, NULL, 0, NULL);

	if (!ply) return 1;
	if (!ply_read_header(ply)) 1;

	nvertices = ply_set_read_cb(ply, "vertex", "x", reinterpret_cast<p_ply_read_cb>(&GetVertex), NULL, 0);
	ply_set_read_cb(ply, "vertex", "y", reinterpret_cast<p_ply_read_cb>(&GetVertex), NULL, 0);
	ply_set_read_cb(ply, "vertex", "z", reinterpret_cast<p_ply_read_cb>(&GetVertex), NULL, 1);

	points.reserve(nvertices);

	vector<double> a;
	points.push_back(a);

	cout << "size" << points.size() << endl;

	if (!ply_read(ply)) return 1;
	OriginalSelection::points.pop_back();
	cout << "data size:" << points.size() << endl;


	ply_close(ply);

	return 0;
}

vector<Point> OriginalSelection::checkPoints(vector<size_t> index)
{
	vector<double> polyX, polyY;

	vector<Point> result;

	for (size_t i = 0; i < index.size(); i++)
	{
		polyX.push_back(points[index[i]][0]);
		polyY.push_back(points[index[i]][1]);
	}

	double xMin = polyX[0],
		xMax = polyX[0],
		yMin = polyY[0],
		yMax = polyY[0];

	for (size_t i = 0; i<polyX.size(); i++)
	{
		if (polyX[i] > xMax)
		{
			xMax = polyX[i];
		}
		if (polyX[i] < xMin)
		{
			xMin = polyX[i];
		}
		if (polyY[i] > yMax)
		{
			yMax = polyY[i];
		}
		if (polyY[i] < yMax)
		{
			yMin = polyY[i];
		}
	}

	for (size_t i = 0; i<points.size(); i++)
	{
		if (points[i][0] < xMin)
			continue;

		if (points[i][0] > xMax)
			continue;

		if (points[i][1] < yMin)
			continue;

		if (points[i][1] > yMax)
			continue;

		int w = IsInPolygin(index.size(), polyX, polyY, points[i][0], points[i][1]);

		if (w == 1)
		{
			result.push_back(Point(points[i][0], points[i][1], points[i][2], 0));
		}
	}

	return result;
}

int OriginalSelection::IsInPolygin(int nvert, vector<double> vertx, vector<double> verty, double testx, double testy)
{
	int i, j, c = 0;

	for (i = 0, j = nvert - 1; i < nvert; j = i++)
	{
		if (((verty[i]>testy) != (verty[j]>testy)) &&
			(testx < (vertx[j] - vertx[i]) * (testy - verty[i]) / (verty[j] - verty[i]) + vertx[i]))
			c = !c;
	}
	return c;
}


