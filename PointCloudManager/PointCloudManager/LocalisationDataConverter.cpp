#include "LocalisationDataConverter.h"
#include "UtmCalculations.h"
#include "OriginalSelection.h"


LocalisationDataConverter::LocalisationDataConverter() {

}


LocalisationDataConverter::~LocalisationDataConverter() {

}




vector<Point> LocalisationDataConverter::getPointsInUTM(vector <Point> llPoints) {

	UtmCalculations uc;
	return uc.Convert(llPoints);
}

vector<Point> LocalisationDataConverter::getOriginalSelection(char *pathToPlyFile, vector<size_t> selectedPoints)
{
	OriginalSelection os;
	return os.GetOriginalSelection(pathToPlyFile, selectedPoints);

}
