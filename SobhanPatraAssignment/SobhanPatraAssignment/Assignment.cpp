#pragma once

#include "CSVManager.h"
#include "DBManager.h"
#include "Utility.h"
#include "Log.h"
#include <iostream>

#include <windows.h>

using namespace std;

/*
1. Define Structure of MainLine
2. Define Structure of ComponentLink
3. Define local data structure for Main Line vector_MainLine
4. Define local data structure for component vector_ComponentLink
5. Write a class for reading from csv file
6. Read data.csv and load to vector_ComponentLink
7. Read main.csv and load to vector_MainLine
8. Write a class for creating database and tables
9 Write method for opening and closing the database
10. Write a routine for reading from vector_ComponentLink and insert into database
11.	Write a routine for reading from database and loading in umap_ComponentLink
12.	Loop vector_MainLine and mapping with umap_ComponentLink compute the following:
13. Calculate Actual Speed
14. Calculate the length adjusted speed
15. Compute the condition of the main link
16. Write a routine for reading from vector_MainLine and insert into database
17. Display Final result.
*/

int main() {

	CSVManager csvManager;
	vector<MainLine> vector_MainLine;
	vector<ComponentLink> vector_ComponentLink;
	unordered_map<string, double> umap_ComponentLink;

	cout << "Hello Quantum Invention" << endl;
	logMsg.push_back("\n=====Hello Quantum Invention=======\n");
	string fileContent;
	string fileNameMain = mainCSV;
	string fileNameData = dataCSV;

	// Parsing main.csv and loading in vector_MainLine
	csvManager.read_file(fileNameMain, fileContent);
	if (fileContent.empty()) {
		cout << "main.csv is empty" << endl;

		exit(EXIT_FAILURE);
	}
	else {
		csvManager.parse_main_csv(fileContent, vector_MainLine);
		cout << "Data parsing from main.csv is done [pushed to vector]" << endl;
	}

	// Parsing data.csv and loading in vector_ComponentLine
	fileContent.erase();
	csvManager.read_file(fileNameData, fileContent);
	if (fileContent.empty()) {
		cout << "data.csv is empty" << endl;

		exit(EXIT_FAILURE);
	}
	else {
		csvManager.parse_data_csv(fileContent, vector_ComponentLink);
		cout << "Data parsing from data.csv is done [pushed to vector]" << endl;
		csvManager.fill_Data(vector_MainLine,vector_ComponentLink);
	}

	DBManager db;
	if (!db.create_Database()) {
		cout << "ERROR: Failed to create DB" << endl;
	}
	else {
		if (!db.create_Table()) {
			cout << "ERROR: Failed to create Table" << endl;
		}
		else {
			db.insert_Into_Database(vector_ComponentLink);
			cout << "Data (Component_LinkId, Actual_Speed) inserted into DB [read from vector]" << endl;

			db.fetch_From_Database(umap_ComponentLink);
			cout << "Data (Component_LinkId, Actual_Speed) fetched from DB [pushed to unordered_map]" << endl;
		}
	}
	
	if (vector_MainLine.size() > 0 && umap_ComponentLink.size() > 0) {
		calculate_Actual_Speed_And_Determine_Condition(vector_MainLine, umap_ComponentLink);
		cout << "Calculate Actual speed and determine condition" << endl;
	}
	else {
		cout << "Parsing or DB ERROR" << endl;
	}
	
	db.insert_MainLine_Into_Database(vector_MainLine);

	show_Final_Result(vector_MainLine);

	//Writing log:\\Log.txt
	write_Log();

	cout<< "*************************************\n";
	cout << "LOG is available at log\\log.txt\n";
	cout << "*************************************\n";
	

	return 0;
}
