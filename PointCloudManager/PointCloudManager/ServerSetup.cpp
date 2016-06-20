#include "ServerSetup.h"
#include <iostream>
#include <string>
#include <cpprest\http_listener.h>
#include <cpprest\json.h>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::experimental::listener;

RestManager* restManagerRef;

void handle_files_get(http_request request){
	http_response response(status_codes::OK);
	response.headers().add(L"Access-Control-Allow-Origin", L"*");
	response.set_body(json::value(restManagerRef->getFilesJson()));
	request.reply(response);
}

pplx::task<void> initFilesListener(){
	return pplx::create_task([]{
		restManagerRef->filesListener = http_listener(L"http://localhost:8080/files");
		restManagerRef->filesListener.support(methods::GET, handle_files_get);
		try{
			restManagerRef->filesListener
				.open()
				.wait();
			while (true);
		}
		catch (exception const & e){
			cout << e.what() << endl;
		}
	});
}

void handle_chosen_file_get(http_request request){
	http_response response(status_codes::OK);
	response.headers().add(L"Access-Control-Allow-Origin", L"*");
	response.set_body(restManagerRef->getChosenFileJson());
	request.reply(response);
}

void handle_chosen_file_post(http_request request){
	auto sentData = request.extract_string().get();
	string dataInStdString = utility::conversions::to_utf8string(sentData);
	string pathToChosenFile, compression, ereaseBuffor;
	size_t position;
	ereaseBuffor = "chosen=";
	position = dataInStdString.find(ereaseBuffor);
	if (position != string::npos){
		dataInStdString.erase(position, ereaseBuffor.length());
	}
	ereaseBuffor = "compression=";
	position = dataInStdString.find(ereaseBuffor);
	if (position != string::npos){
		dataInStdString.erase(position, ereaseBuffor.length());
	}
	position = dataInStdString.find("&");
	pathToChosenFile = dataInStdString; compression = dataInStdString;
	pathToChosenFile.erase(pathToChosenFile.begin() + position, pathToChosenFile.end());
	compression.erase(compression.begin(), compression.begin() + position+1);
	restManagerRef->setChosenFileAndCompression(pathToChosenFile, compression);
}

pplx::task<void> initChosenFileListener(){
	return pplx::create_task([]{
		restManagerRef->chosenFileListener = http_listener(L"http://localhost:8080/chosenfile");
		restManagerRef->chosenFileListener.support(methods::GET, handle_chosen_file_get);
		restManagerRef->chosenFileListener.support(methods::POST, handle_chosen_file_post);
		try{
			restManagerRef->chosenFileListener
				.open()
				.wait();
			while (true);
		}
		catch (exception const & e){
			cout << e.what() << endl;
		}
	});
}

void handle_selection_post(http_request request){
	
}

pplx::task<void> initSelectionListener(){
	return pplx::create_task([]{
		restManagerRef->selectionListener = http_listener(L"http://localhost:8080/selection");
		restManagerRef->selectionListener.support(methods::POST, handle_selection_post);
		try{
			restManagerRef->selectionListener
				.open()
				.wait();
			while (true);
		}
		catch (exception const & e){
			cout << e.what() << endl;
		}
	});
}

void handle_values_get(http_request request){
	http_response response(status_codes::OK);
	response.headers().add(L"Access-Control-Allow-Origin", L"*");
	response.set_body(restManagerRef->getValues());
	request.reply(response);
}

pplx::task<void> initValuesListener(){
	return pplx::create_task([]{
		restManagerRef->valuesListener = http_listener(L"http://localhost:8080/values");
		restManagerRef->valuesListener.support(methods::GET, handle_values_get);
		try{
			restManagerRef->valuesListener
				.open()
				.wait();
			while (true);
		}
		catch (exception const & e){
			cout << e.what() << endl;
		}
	});
}

void init(RestManager* restManager){
	restManagerRef = restManager;
	initFilesListener();
	initChosenFileListener();
	initSelectionListener();
	initValuesListener();
}