#include "RestManager.h"
#include <cpprest\json.h>
#include <cpprest\http_client.h>
#include <Windows.h>
#include <sstream>
#include <string>
#include <algorithm>
#include <sys/types.h>
#include "dirent.h"

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::experimental::listener;


RestManager::RestManager(char *dirPath){
	setFilesJson(dirPath);
	chosenFileJson[L"chosen"] = json::value::string(L"");
	chosenFileJson[L"compression"] = json::value::string(L"");
	chosenFileJson[L"compressedFile"] = json::value::string(L"");
	valuesJson[L"volume"] = json::value::string(L"");
	valuesJson[L"error"] = json::value::string(L"");
}


RestManager::~RestManager(){

}

void RestManager::setFilesJson(char *dirPath){
	DIR *dp;
	dirent *d;
	vector<string> vec;
	dp = opendir(dirPath);
	while ((d = readdir(dp)) != NULL){
		vec.push_back(d->d_name);
	}
	sort(vec.begin(), vec.end());
	for (int i = 0; i < vec.size()-2; i++){
		wstring convertedName(vec[i + 2].begin(), vec[i+2].end());
		filesJson[i] = json::value::string(convertedName.c_str());
	}
}

json::value RestManager::getFilesJson(){
	return filesJson;
}

void RestManager::setChosenFileAndCompression(string filePath, string compression){
	chosenFileJson[L"chosen"] = json::value::string(wstring(filePath.begin(), filePath.end()));
	chosenFileJson[L"compression"] = json::value::string(wstring(compression.begin(), compression.end()));
}

void RestManager::setCompressedFile(string filePath){
	chosenFileJson[L"compressedFile"] = json::value::string(wstring(filePath.begin(), filePath.end()));
}

json::value RestManager::getChosenFileJson(){
	return chosenFileJson;
}

void RestManager::setSelection(){
	//TODO - add parameter and fill
}

json::value RestManager::getSelection(){
	return selectionJson;
}

void RestManager::setValues(vector<double> values){
	string buf;
	ostringstream stream;
	stream << values[0];
	buf = stream.str();
	stream.clear();
	valuesJson[L"volume"] = json::value::string(wstring(buf.begin(), buf.end()));
	stream << values[1];
	buf = stream.str();
	stream.clear();
	valuesJson[L"error"] = json::value::string(wstring(buf.begin(), buf.end()));
}

json::value RestManager::getValues(){
	return valuesJson;
}