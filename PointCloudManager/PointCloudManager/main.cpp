#include <iostream>

#include <process.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "ServerSetup.h"

#include <Winsock2.h>
#include "VolumeCounter.h"
#include "OriginalSelection.h"
#include "Point.h"
#include "UtmCalculations.h"

using namespace std;

RestManager* restManager;

int main()
{
	cout << "Point Cloud Manager" << endl;
	restManager = new RestManager();
	init(restManager);
	cout << "zaczynam czesc UTM" << endl;

	OriginalSelection *os = new OriginalSelection();
	UtmCalculations *utm = new UtmCalculations();
	vector<Point> pointsBeforeUTM;
	vector<Point> pointsAfterUTM;
	vector<size_t> ww;
	pointsBeforeUTM = os->getPointsFromFile("out.ply");
	pointsAfterUTM = utm->Convert(pointsBeforeUTM);


	for (int i = 0; i < 10; i++)
	{
		cout << pointsAfterUTM[i].x << " " << pointsAfterUTM[i].y << " " << pointsAfterUTM[i].z << endl;
		cout << pointsBeforeUTM[i].x << " " << pointsBeforeUTM[i].y << " " << pointsBeforeUTM[i].z << endl;

		cout << "--\n";
	}
	cout << "zaczynam czesc VolumeCalculation" << endl;
	VolumeCounter volume(pointsBeforeUTM);
	system("PAUSE");
	return 0;


}