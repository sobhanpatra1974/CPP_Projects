#pragma once
#include "CSVManager.h"
#include "DBManager.h"
//#include <hash_map>
#include <iostream>
#include <windows.h>
using namespace std;

/*
1. Define Structure of Main Line (SG)
2. Define Structure of Component Line (LT)
3. Define local data structure for Main Line vector_SG/HashTable_SG
4. Define local data structure for component Line vector_LT/HashTable_LT
5. Write a class for reading from csv file
6. Read data.csv and load to HashTable_LT
7. Read main.csv and load to HashTable_SG
8. Calculate Actual Speed
9. Calculate the length adjusted speed
10. Compute the condition of the main link
11. Write a class for creating database and tables
12. Write method for opening and closing the database
13. Write a routine for reading from main.csv and inserrt into database
14. Write a routine for reading from data.csv and inserrt into database
15. Write a routine for rading records from main.csv and push to HashTable_SG
16. Write a routine for rading records from data.csv and push to HashTable_LT
*/

int main() {
	// std::hash_map<int, int> hash;
	cout << "Hello Quantum Invention" << endl;
	CSVManager csvManager;
	vector<MainLine> vector_MainLine;
	vector<ComponentLine> vector_ComponentLine;
	map<string, double> map_ComponentLine;

	string fileContent;
	char directory[MAX_PATH];
	DWORD len = GetCurrentDirectory(MAX_PATH, directory);
	string fileNameMain, fileNameData;
	fileNameMain = string(directory);
	fileNameMain.append("\\main.csv");

	fileNameData = string(directory);
	fileNameData.append("\\data.csv");

	fileNameMain = "main.csv";
	fileNameData = "data.csv";
	/*
	Question 1
	i.Regular access / iteration through the data structure
	ii.Query the record using the Main_ID(e.g.A)
	iii.All the fields have to be stored as they will be accessed in a
	later part of the program
  */

  // Parsing main.csv and loading in vector_MainLine
	csvManager.read_file(fileNameMain, fileContent);
	if (fileContent.empty()) {
		cout << "main.csv is empty" << endl;
		exit(EXIT_FAILURE);
	}
	else {
		csvManager.parse_main_csv(fileContent, vector_MainLine);
	}

	// Parsing data.csv and loading in vector_ComponentLine
	fileContent.erase();
	csvManager.read_file(fileNameData, fileContent);
	if (fileContent.empty()) {
		cout << "data.csv is empty" << endl;
		exit(EXIT_FAILURE);
	}
	else {
		csvManager.parse_data_csv(fileContent, vector_ComponentLine);
	}
	/*
	char exepath[MAX_PATH];
	DWORD szPath;
	szPath = GetModuleFileName(NULL, exepath, MAX_PATH);
	*/

	DBManager db;
	db.create_Database();
	db.create_Table();

	return 0;
}
