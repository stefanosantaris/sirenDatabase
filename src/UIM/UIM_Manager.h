/*
 * UIM_Manager.h
 *
 *      Author: santaris
 */

#ifndef UIM_MANAGER_H_
#define UIM_MANAGER_H_

#include "../SSQLM/SSQLM.h"
#include "../SYSM/SYSM.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;


class UIM_Manager {
private:
	string cmd;
	SYSM_Manager smm;
	SSQLM_Manager sqlm;
	SSQLM_CommandParser parse;
	t_relInfo relInfo;
	vector<t_attrInfo> attrInfo;
	t_cmd cmdType;

	bool OpenFlag;
	char *openDb;

	t_rc IdentifyCommand();



	t_rc ExecuteCreateDatabase();
	t_rc ExecuteDropDatabase();
	t_rc ExecuteOpenDatabase();
	t_rc ExecuteCloseDatabase();

	t_rc ExecuteCreateTable();
	t_rc ExecuteDropTable();
	t_rc ExecuteCreateIndex();
	t_rc ExecuteDropIndex();

	t_rc ExecuteSelect();
	t_rc ExecuteInsert();



public:
	UIM_Manager(SYSM_Manager &smManager, SSQLM_Manager &sqlManager);
	~UIM_Manager();

	void SetCommand(string cmd);

	t_rc ExecuteCommand();


};

#endif /* UIM_MANAGER_H_ */
