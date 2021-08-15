#pragma once

#include "DataStructure.h"
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "sqlite/sqlite3.h"

extern vector<string> logMsg;

class DBManager {
public:
	bool create_Database();

	bool create_Table();

	void insert_Into_Database(vector<ComponentLink>& vector_ComponentLink);

	void insert_MainLine_Into_Database(vector<MainLine>& vector_MainLine);

	void fetch_From_Database(unordered_map<string, double>& umap_ComponentLink);
};
