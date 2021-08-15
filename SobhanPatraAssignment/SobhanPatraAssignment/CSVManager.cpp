#include "CSVManager.h"
using namespace std;

void CSVManager::read_file(const string fileName, string &fileContent) {

  fstream myfile;
  myfile.open(fileName, ios::in);

  if (!myfile.is_open()) {
    cerr << "Could not open the file - '" << fileName << "'" << endl;
    logMsg.push_back(string("Could not open the file\n"));
    exit(EXIT_FAILURE);
  }

  stringstream fc;
  fc << myfile.rdbuf();
  fileContent = fc.str();

  myfile.close();
}

void CSVManager::parse_main_csv(const string fileContent,
                                vector<MainLine> &vector_MainLine) {

  vector<string> vector_row;
  string line, token;
  stringstream ssfileContent(fileContent);

  logMsg.push_back("\nReading from main.csv\n");

  while (getline(ssfileContent, line)) {
    logMsg.push_back(line);
    logMsg.push_back("\n");
    // Skip heading line of main.csv
    int pos = line.find("Main_ID");
    if (pos >= 0) {
      continue;
    }

    vector_row.clear();
    stringstream ss(line);

    while (getline(ss, token, '^')) {
      // line is broken into tokens and temporarily
      // stored it in a string variable, 'token'
      vector_row.push_back(token);
    }
    if (5 == vector_row.size()) {
      int countComponent_LinkID =
          count(vector_row[1].begin(), vector_row[1].end(), ',');
      int countSub_Length =
          count(vector_row[3].begin(), vector_row[3].end(), ',');

      // Check if all components have corresponding Sub_Lengths
      if (countComponent_LinkID == countSub_Length) {
        MainLine mL;
        ComponentLink cL;
        vector<ComponentLink> vector_ComponentLink;
        string Component_LinkID;
        stringstream ssComponent_LinkID(vector_row[1]);

        while (getline(ssComponent_LinkID, Component_LinkID, ',')) {
          // Component_Ids are parsed and pushed into vector
          cL.Component_LinkID = Component_LinkID;
          cL.Main_Id = vector_row[0];
          vector_ComponentLink.push_back(cL);
        }

        string Sub_Length;
        stringstream ssSub_Length(vector_row[3]);

        int i = 0;
        while (getline(ssSub_Length, Sub_Length, ',')) {
          // Sub_Length are assigned to the corresponding Component_Id
          vector_ComponentLink[i].Sub_Length = stod(Sub_Length);
          ++i;
        }
        mL.Main_ID = vector_row[0];
        mL.vector_Component = vector_ComponentLink;
        mL.Main_Length = stod(vector_row[2]);
        mL.Ideal_Travel_Time = stod(vector_row[4]);
        vector_MainLine.push_back(mL);
      }
    } else {
      logMsg.push_back("\nERROR in parsing: ");
      logMsg.push_back(line);
      logMsg.push_back("\n");
    }
  }
}

void CSVManager::parse_data_csv(const string fileContent,
                                vector<ComponentLink> &vector_ComponentLink) {

  vector<string> vector_row;
  string line, token;
  stringstream ssfileContent(fileContent);

  logMsg.push_back("\nReading from data.csv\n");

  while (getline(ssfileContent, line)) {
    logMsg.push_back(line);
    logMsg.push_back("\n");
    // Skip heading line of data.csv
    int pos = line.find("Component_LinkID");
    if (pos >= 0) {
      continue;
    }

    vector_row.clear();
    stringstream ss(line);

    while (getline(ss, token, '^')) {
      // line is broken into tokens and temporarily
      // stored it in a string variable, 'token'
      vector_row.push_back(token);
    }

    if (2 == vector_row.size()) {
      ComponentLink cL;
      cL.Component_LinkID = vector_row[0];
      cL.Actual_Speed = stod(vector_row[1]);
      vector_ComponentLink.push_back(cL);
    } else {
      logMsg.push_back("\nERROR in parsing: ");
      logMsg.push_back(line);
      logMsg.push_back("\n");
    }
  }
}

void CSVManager::fill_Data(vector<MainLine> &vector_MainLine,
                           vector<ComponentLink> &vector_ComponentLink) {

  for (auto &mL : vector_MainLine) {
    for (auto &cL : mL.vector_Component) {
      for (auto &cVL : vector_ComponentLink) {
        if (cL.Component_LinkID.compare( cVL.Component_LinkID)==0) {
          cVL.Main_Id = cL.Main_Id;
          break;
        }
      }
    }
  }
}
