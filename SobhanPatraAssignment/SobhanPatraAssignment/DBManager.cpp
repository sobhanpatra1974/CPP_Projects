#include "DBManager.h"

using namespace std;
bool DBManager::create_Database() {
  sqlite3 *DB;

  int retCode = 0;
  retCode = sqlite3_open(sqliteDB.c_str(), &DB);

  if (retCode != SQLITE_OK) {
    cerr << "Error in creating/openening DB " << sqlite3_errmsg(DB) << endl;
    logMsg.push_back(string("Error in creating/openening DB\n"));
    return false;
  } else {
    cout << "Opened Database Successfully" << endl;
    logMsg.push_back(string("Opened Database Successfully\n"));
  }
  sqlite3_close(DB);
  return true;
}

bool DBManager::create_Table() {

  sqlite3 *DB;
  int retCode = 0;
  int retCodeCOMPONENT_LINK = 0;
  int retCodeMAINLINE_PARAMETERS = 0;

  string sqlCOMPONENT_LINK =
      "CREATE TABLE IF NOT EXISTS COMPONENT_LINK (Component_LinkId "
      "TEXT PRIMARY KEY  NOT NULL, Main_Id TEXT NOT NULL, Actual_Speed REAL);";

  string sqlMAINLINE_PARAMETERS =
      "CREATE TABLE IF NOT EXISTS MAINLINE_PARAMETERS (Main_Id TEXT PRIMARY "
      "KEY  NOT NULL, Main_Length REAL,Ideal_Travel_Time REAL, Ideal_Speed "
      "REAL,Length_Adjusted_Speed REAL, Deviation_Speed REAL,conditionMainLine "
      "TEXT);";

  retCode = sqlite3_open(sqliteDB.c_str(), &DB);
  if (retCode != SQLITE_OK) {
    cerr << "Error in opening DB" << sqlite3_errmsg(DB) << endl;
    return false;
  }
  // Create COMPONENT_LINK
  char *msgErrorCOMPONENT_LINK;
  retCodeCOMPONENT_LINK = sqlite3_exec(DB, sqlCOMPONENT_LINK.c_str(), NULL, 0,
                                       &msgErrorCOMPONENT_LINK);

  if (retCodeCOMPONENT_LINK != SQLITE_OK) {
    cerr << "Error Create Table [COMPONENT_LINK]" << msgErrorCOMPONENT_LINK
         << endl;
    logMsg.push_back(string("Error Create Table [COMPONENT_LINK]\n"));
    logMsg.push_back(string(msgErrorCOMPONENT_LINK));
    sqlite3_free(msgErrorCOMPONENT_LINK);
  } else {
    cout << "Table created Successfully [COMPONENT_LINK]" << endl;
    logMsg.push_back(string("Table created Successfully [COMPONENT_LINK]\n"));
  }

  // Create MAINLINE_PARAMETERS
  char *msgErrorMAINLINE_PARAMETERS;
  retCodeMAINLINE_PARAMETERS =
      sqlite3_exec(DB, sqlMAINLINE_PARAMETERS.c_str(), NULL, 0,
                   &msgErrorMAINLINE_PARAMETERS);

  if (retCodeMAINLINE_PARAMETERS != SQLITE_OK) {
    cerr << "Error Create Table [MAINLINE_PARAMETERS]"
         << msgErrorMAINLINE_PARAMETERS << endl;
    logMsg.push_back(string("Error Create Table [MAINLINE_PARAMETERS]\n"));
    logMsg.push_back(string(msgErrorMAINLINE_PARAMETERS));
    sqlite3_free(msgErrorMAINLINE_PARAMETERS);
  } else {
    cout << "Table created Successfully [MAINLINE_PARAMETERS]" << endl;
    logMsg.push_back(
        string("Table created Successfully [MAINLINE_PARAMETERS]\n"));
  }

  sqlite3_close(DB);
  return true;
}

void DBManager::insert_Into_Database(
    vector<ComponentLink> &vector_ComponentLink) {
  sqlite3 *DB;
  sqlite3_stmt *stmt;
  int retCode = 0;

  retCode = sqlite3_open(sqliteDB.c_str(), &DB);

  if (retCode != SQLITE_OK) {
    cerr << "Error in opening DB " << sqlite3_errmsg(DB) << endl;
    logMsg.push_back(string("Error in opening DB\n"));
    return;
  } else {
    // cout << "Opened Database Successfully." << endl;
    logMsg.push_back(string("Opened Database Successfully\n"));
  }

  for (ComponentLink &cL : vector_ComponentLink) {

    string Component_LinkId = cL.Component_LinkID;
    string Main_Id = cL.Main_Id;
    string Actual_Speed = to_string(cL.Actual_Speed);

    string sqlSelect(
        "SELECT COUNT(*) FROM COMPONENT_LINK WHERE Component_LinkId = '" +
        Component_LinkId + "';");

    string sqlInsert("INSERT INTO COMPONENT_LINK VALUES ('" + Component_LinkId +
                     "' , '" + Main_Id + "' , " + Actual_Speed + ");");

    retCode = sqlite3_prepare_v2(DB, sqlSelect.c_str(), -1, &stmt, 0);

    if (retCode != SQLITE_OK) {
      cerr << "Error SELECT" << endl;
    } else {
      int recordCount = 0;
      while (sqlite3_step(stmt) != SQLITE_DONE) {
        recordCount = sqlite3_column_int(stmt, 0);
      }
      if (recordCount > 0) {
        // cout << "Record already exists" << endl;
        logMsg.push_back(string("Record already exists [Component_LinkId: "));
        logMsg.push_back(Component_LinkId);
        logMsg.push_back("]\n");
      } else {
        char *msgError;
        retCode = sqlite3_exec(DB, sqlInsert.c_str(), NULL, NULL, &msgError);
        if (retCode != SQLITE_OK) {
          // cerr << "Error INSERT" << msgError << endl;
          logMsg.push_back(string("Error INSERT: "));
          logMsg.push_back(sqlInsert);
          logMsg.push_back("\n");
          logMsg.push_back(string(msgError));
          sqlite3_free(msgError);
        } else {
          // cerr << "INSERT Successful. " << sqlInsert << endl;
          logMsg.push_back(string("\nINSERT Successful: "));
          logMsg.push_back(sqlInsert);
        }
      }
    }
  }
  sqlite3_close(DB);
}

void DBManager::insert_MainLine_Into_Database(
    vector<MainLine> &vector_MainLine) {
  sqlite3 *DB;
  sqlite3_stmt *stmt;
  int retCode = 0;

  retCode = sqlite3_open(sqliteDB.c_str(), &DB);

  if (retCode != SQLITE_OK) {
    cerr << "Error in opening DB " << sqlite3_errmsg(DB) << endl;
    logMsg.push_back(string("Error in opening DB\n"));
    return;
  } else {
    // cout << "Opened Database Successfully." << endl;
    logMsg.push_back(string("Opened Database Successfully\n"));
  }

  for (auto &mL : vector_MainLine) {

    string Main_Id = mL.Main_ID;
    string Main_Length = to_string(mL.Main_Length);
    string Ideal_Travel_Time = to_string(mL.Ideal_Travel_Time);
    string Ideal_Speed = to_string(mL.Ideal_Speed);
    string Length_Adjusted_Speed = to_string(mL.Length_Adjusted_Speed);
    string Deviation_Percentage_Speed_Actual_From_Ideal =
        to_string(mL.Deviation_Percentage_Speed_Actual_From_Ideal);
    string conditionMainLine =
        mL.conditionMainLine == CONDITION::GREEN    ? "GREEN"
        : mL.conditionMainLine == CONDITION::YELLOW ? "YELLOW"
                                                    : "RED";
    string sqlSelect(
        "SELECT COUNT(*) FROM MAINLINE_PARAMETERS WHERE Main_Id = '" + Main_Id +
        "';");
    // Main_Id TEXT PRIMARY KEY  NOT NULL, Main_Length REAL,Ideal_Travel_Time
    // REAL, Ideal_Speed REAL,Length_Adjusted_Speed REAL, Deviation_Speed
    // REAL,conditionMainLine TEXT);";
    string sqlInsert("INSERT INTO MAINLINE_PARAMETERS VALUES ('" + Main_Id +
                     "' , " + Main_Length + " , " + Ideal_Travel_Time + " , " +
                     Ideal_Speed + " , " + Length_Adjusted_Speed + " , " +
                     Deviation_Percentage_Speed_Actual_From_Ideal + " , '" +
                     conditionMainLine + "');");

    retCode = sqlite3_prepare_v2(DB, sqlSelect.c_str(), -1, &stmt, 0);

    if (retCode != SQLITE_OK) {
      cerr << "Error SELECT" << endl;
    } else {
      int recordCount = 0;
      while (sqlite3_step(stmt) != SQLITE_DONE) {
        recordCount = sqlite3_column_int(stmt, 0);
      }
      if (recordCount > 0) {
        // cout << "Record already exists" << endl;
        logMsg.push_back(string("Record already exists [Main_Id: "));
        logMsg.push_back(Main_Id);
        logMsg.push_back("]\n");
      } else {
        char *msgError;
        retCode = sqlite3_exec(DB, sqlInsert.c_str(), NULL, NULL, &msgError);
        if (retCode != SQLITE_OK) {
          // cerr << "Error INSERT" << msgError << endl;
          logMsg.push_back(string("Error INSERT: "));
          logMsg.push_back(sqlInsert);
          logMsg.push_back("\n");
          logMsg.push_back(string(msgError));
          sqlite3_free(msgError);
        } else {
          // cerr << "INSERT Successful. " << sqlInsert << endl;
          logMsg.push_back(string("\nINSERT Successful: "));
          logMsg.push_back(sqlInsert);
        }
        
      }
    }
  }

  sqlite3_close(DB);
}

void DBManager::fetch_From_Database(
    unordered_map<string, double> &umap_ComponentLink) {
  sqlite3 *DB;
  sqlite3_stmt *stmt;
  int retCode = 0;
  string sqlSelect("SELECT * FROM COMPONENT_LINK;");

  retCode = sqlite3_open(sqliteDB.c_str(), &DB);

  if (retCode != SQLITE_OK) {
    cerr << "Error open DB " << sqlite3_errmsg(DB) << endl;
    logMsg.push_back(string("Error open DB\n"));
    return;
  } else {
    // cout << "Opened Database Successfully." << endl;
  }

  retCode = sqlite3_prepare_v2(DB, sqlSelect.c_str(), -1, &stmt, 0);

  if (retCode != SQLITE_OK) {
    cerr << "Error SELECT" << endl;
    logMsg.push_back(string("Error SELECT\n"));
    logMsg.push_back(sqlSelect);
  } else {
    // cout << "SELECT Operation Successful" << endl;
    logMsg.push_back(string("\nSELECT Operation Successful\n"));
    logMsg.push_back(sqlSelect);
    logMsg.push_back("\n");
    int recordCount = sqlite3_column_int(stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      const unsigned char *Component_LinkId = sqlite3_column_text(stmt, 0);
      const unsigned char *Main_Id = sqlite3_column_text(stmt, 1);
      double Actual_Speed = sqlite3_column_double(stmt, 2);
      // cout << "Component_LinkId: " << Component_LinkId << " | Actual_Speed: "
      // << Actual_Speed << endl;

      logMsg.push_back(string("Data pushed to umap_ComponentLink"));
      string data = " [Component_LinkId: ";
      data += string((const char *)Component_LinkId);
      data += " | Main_Id: ";
      data += string((const char *)Main_Id);
      data += " | Actual_Speed: ";
      data += to_string(Actual_Speed);
      data += "]\n";
      logMsg.push_back(data);
      umap_ComponentLink[string((const char *)Component_LinkId)] = Actual_Speed;
    }
  }

  sqlite3_close(DB);
}
