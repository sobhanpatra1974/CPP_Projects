#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

enum class CONDITION { RED, YELLOW, GREEN };

struct ComponentLink {
  string Component_LinkID;
  string Main_Id;
  double Sub_Length;
  double Actual_Speed;

  ComponentLink() :Sub_Length(0), Actual_Speed(0) {}
};

struct MainLine {
  string Main_ID;
  vector<ComponentLink> vector_Component;
  double Main_Length;
  double Ideal_Travel_Time;
  double Ideal_Speed;
  double Length_Adjusted_Speed;
  double Deviation_Percentage_Speed_Actual_From_Ideal;
  CONDITION conditionMainLine;

  MainLine() :Main_Length(0), Ideal_Travel_Time(0), Ideal_Speed(0), Length_Adjusted_Speed(0), Deviation_Percentage_Speed_Actual_From_Ideal(0){}
};


const string mainCSV = "data\\main.csv";
const string dataCSV = "data\\data.csv";
const string sqliteDB = "data\\QI.db";

