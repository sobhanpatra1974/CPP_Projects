#pragma once
#include "DataStructure.h"

static void
calculate_Actual_Speed_And_Determine_Condition(vector<MainLine> &vector_MainLine,
	unordered_map<string, double> &umap_ComponentLink) {
	for (auto &mL : vector_MainLine) {

		double totalComponentLinkSubLength = 0;
		double totalComponentLinkActualSpeed = 0;
		int count_cL = 0;

		for (auto &cL : mL.vector_Component) {

			if (umap_ComponentLink.find(cL.Component_LinkID) !=
				umap_ComponentLink.end()) {
				cL.Actual_Speed = umap_ComponentLink[cL.Component_LinkID];
				totalComponentLinkActualSpeed += cL.Actual_Speed;
				totalComponentLinkSubLength += cL.Sub_Length;
				++count_cL;
			}
			else {
				cout << "Component_LinkId not found" << endl;
				logMsg.push_back(string("Component_LinkId not found\n"));
			}
		}
		// Calculate the actual speed on the main links (A, B …)
		// by doing a simple averaging on their component links
		double MainLineActualSpeed = totalComponentLinkActualSpeed / count_cL;

		// Calculate the length adjusted speed on the main links(A, B….).
		//(Length of the main component / The total length of the component links) *
		// average speed
		double LengthAdjustedSpeed =
			(mL.Main_Length / totalComponentLinkSubLength) * MainLineActualSpeed;
		// Calculate Ideal Speed of Main Line
		double MainLineIdealSpeed = mL.Main_Length / mL.Ideal_Travel_Time;
		double deviationPercentageSpeedActualFromIdeal =
			100 * abs(MainLineIdealSpeed - LengthAdjustedSpeed) /
			MainLineIdealSpeed;

		mL.Length_Adjusted_Speed = LengthAdjustedSpeed;
		mL.Ideal_Speed = MainLineIdealSpeed;
		mL.Deviation_Percentage_Speed_Actual_From_Ideal = deviationPercentageSpeedActualFromIdeal;

		if (deviationPercentageSpeedActualFromIdeal > 80.0) {
			mL.conditionMainLine = CONDITION::GREEN;
		}
		else if (deviationPercentageSpeedActualFromIdeal > 50.0) {
			mL.conditionMainLine = CONDITION::YELLOW;
		}
		else {
			mL.conditionMainLine = CONDITION::RED;
		}
	}
}

static void show_Final_Result(vector<MainLine> &vector_MainLine) {
	logMsg.push_back("\n");
	for (auto &mL : vector_MainLine) {
		string str = "=====================================\n";
		str += "Main_ID: " + mL.Main_ID + "\n";
		str += "Main_Length: " + to_string(mL.Main_Length) + "\n";
		str += "Ideal_Travel_Time: " + to_string(mL.Ideal_Travel_Time) + "\n";
		str += "Ideal_Speed: " + to_string(mL.Ideal_Speed) + "\n";
		str += "Length_Adjusted_Speed: " + to_string(mL.Length_Adjusted_Speed) + "\n";
		str += "Speed_Deviation_Percentage: " + to_string(mL.Deviation_Percentage_Speed_Actual_From_Ideal) + "\n";
		string condition = mL.conditionMainLine == CONDITION::GREEN ? "GREEN"
			: mL.conditionMainLine == CONDITION::YELLOW ? "YELLOW"
			: "RED";
		str += "Condition: " + condition + "\n";
		for (auto &cL : mL.vector_Component) {
			str += "-------------------------------------\n";
			str += "	Component_LinkId: " + cL.Component_LinkID + "\n";
			str += "	Component_Length: " + to_string(cL.Sub_Length) + "\n";
			str += "	Actual Speed: " + to_string(cL.Actual_Speed) + "\n";
		}
		cout << str.c_str() << endl;
		logMsg.push_back(str);
	}
}

