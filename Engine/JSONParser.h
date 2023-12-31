#pragma once
#include <iostream>
#include <fstream>
#include <json/json.h>
#include "Globals_ENGINE.h"
#include "..\Editor\Globals.h"

using namespace std;

Json::Value GetFile(string path) {
	ifstream file(path);

	Json::Value obj;
	Json::CharReaderBuilder reader;

	string errors;
	bool parsingSuccessful = Json::parseFromStream(reader, file, &obj, &errors);

	if (!parsingSuccessful) {
		LOG("ENGINE: JsonCPP ERROR!", NULL);
		LOG("%s", errors);
	}

	file.close();

	return obj;
}


// Access data by calling the function into a Json::Value variable
// Json::Value obj = GetFile("name_of_file");
// obj["name_attribute"].asInt();
// obj["name_attribute"].asString();

// To write data, use the same function GetFile(...)
// obj["name_attribute"] = "Hello World";
// obj["name_attribute"] = 12345;

// To add this type of information:  "attribute" : [ 1, 2, 3 ]
// Use the following:
// Json::Value vec(Json::arrayValue);
// vec.append(Json::Value(1));
// vec.append(Json::Value(2));
// vec.append(Json::Value(3));
// ofstream("example.json") << obj;

// To add childs in the structure:
// {
//	 "father" :
//	 {
//		 "child" :
//		 {
//			 "name" : "Case1"
//		 },
//		 "child2" :
//		 {
//		 	 "name" : "Case2"
//		 }
//	 }
// }
//
// obj["father"]["child"]["name"] = "Case1";
// obj["father"]["child2"]["name"] = "Case2";