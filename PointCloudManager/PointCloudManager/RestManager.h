#pragma once

#include <cpprest/http_listener.h>
#include <vector>
              
using namespace std;
using namespace web;                        
using namespace web::http;                 
using namespace web::http::experimental::listener;

class RestManager
{
public: 
	http_listener filesListener;
	http_listener chosenFileListener;
	http_listener selectionListener;
	http_listener valuesListener;

private:
	json::value filesJson;
	json::value chosenFileJson;
	json::value selectionJson;
	json::value valuesJson;

public:
	RestManager(char *dirPath);
	~RestManager();
	void setFilesJson(char *dirPath);
	json::value getFilesJson();
	void setChosenFileAndCompression(string filePath, string compression);
	void setCompressedFile(string filePath);
	json::value getChosenFileJson();
	void setSelection(); //TODO - add parameter
	json::value getSelection();
	void setValues(vector<double> values);
	json::value getValues();
	
};

