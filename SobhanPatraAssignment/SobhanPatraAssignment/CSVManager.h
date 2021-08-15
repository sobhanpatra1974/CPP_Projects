#pragma once

#include "DataStructure.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

extern vector<string> logMsg;
class CSVManager {
public:
  void read_file(const string fileName, string &fileContent);
  void parse_main_csv(const string fileContent,
                      vector<MainLine> &vector_MainLine);

  void parse_data_csv(const string fileContent,
                      vector<ComponentLink> &vector_ComponentLink);
  void fill_Data(vector<MainLine> &vector_MainLine,
                 vector<ComponentLink> &vector_ComponentLink);
};
