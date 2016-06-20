#include <iostream>

#include <process.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "ServerSetup.h"

using namespace std;

RestManager* restManager;

int main()
{
	char *dirPath = new char();
	cout << "Point Cloud Manager" << endl;
	cout << "Give path to directory with .ply files:" << endl;
	cin >> dirPath;
	getchar();
	restManager = new RestManager(dirPath);
	init(restManager);
	getchar();



}