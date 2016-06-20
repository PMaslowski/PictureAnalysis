#include "LocalisationDataConverter.h"
#include "UtmCalculations.h"
#include "OriginalSelection.h"

//
//LocalisationDataConverter::LocalisationDataConverter(){
//
//}
//
//
//LocalisationDataConverter::~LocalisationDataConverter(){
//
//}
//
//
//
//
//vector<Point*> LocalisationDataConverter::getPointsInUTM(vector <Point*> llPoints){
//
//	vector <vector<double> > input; 
//	vector <vector<double> > result;
//	
//	
//
//	for (unsigned int i = 0; i < llPoints.size(); i++)
//	{
//		vector<double> temp;
//		temp.push_back(llPoints[i]->x);
//		temp.push_back(llPoints[i]->y);
//		temp.push_back(llPoints[i]->z);		
//		input.push_back(temp);
//	}
//
//	result = UtmCalculations::Convert(input);
//
//	pointsInUTM.clear();
//
//	for (unsigned int i = 0; i < input.size(); i++)
//	{
//		pointsInUTM.push_back(new Point(result[i][0], result[i][1], result[i][2], 0));
//	}
//
//	return pointsInUTM;	
//}
//
//vector<Point*> LocalisationDataConverter::getOriginalSelection(char *pathToPlyFile, vector<size_t> selectedPoints)
//{
//
//	vector< vector<double>> input;
//	OriginalSelection os;
//	os.readPlyFile(pathToPlyFile, selectedPoints, input);
//
//	vector<Point*> result;
//
//	for (unsigned int i = 0; i < input.size(); i++)
//	{
//		result.push_back(new Point(input[i][0], input[i][1], input[i][2], 0));
//	}
//
//	return result;
//}
