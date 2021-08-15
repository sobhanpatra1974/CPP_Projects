#pragma once

#include <vector>
#include<string>
#include <fstream>
#include <iostream>
#include <fileapi.h>
using namespace std;

const string logFile = "log\\log.txt";
vector<string> logMsg;

static void write_Log() {

	CreateDirectory("log", NULL);
	ofstream myfile;
	myfile.open(logFile, ios::app);


	if (!myfile.is_open()) {
		cerr << "Could not open the log file - '" << logFile << "'" << endl;
		exit(EXIT_FAILURE);
	}

	for (auto& str : logMsg) {
		myfile << str.c_str();
	}

	myfile.close();
}