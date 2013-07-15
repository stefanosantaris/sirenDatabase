/*
 * SYSM_Manager.h
 *
 *      Author: santaris
 */

#ifndef SYSM_MANAGER_H_
#define SYSM_MANAGER_H_

#include "../REM/REM.h"
#include "SYSM_TupleHandle.h"
class SYSM_Manager {

private:
	bool openFlag;

	char cwd[100];

	REM_RecordFileManager &rfm;

	REM_RecordFileHandle relFileHandle;

	REM_RecordFileHandle attrFileHandle;
	const char *relMetName;
	const char *attrMetName;



	t_rc GetAttributeRecordHandle (REM_RecordFileScan fileScan, char *attrName, REM_RecordHandle &recordHandle);


public:
	SYSM_Manager (REM_RecordFileManager &rmManager); 		//Constructor
	~SYSM_Manager ();										//Destructor

	t_rc CreateDb (const char *dbName);						//Create database

	t_rc DestroyDb (const char *dbName);					//Destroy database

	t_rc OpenDb (const char *dbName);						//Open database

	t_rc CloseDb (const char *dbName);						//Close database

	t_rc CreateTable (t_relInfo relInfo, vector<t_attrInfo> attrInfo);	//Create table

	t_rc DropTable (const char *relName);					//Drop relation

	t_rc GetNextIndexNo(int &indexNo); 						//Get next Index code

	t_rc CreateIndex (const char *relName, const char *attrName, int indexNo);		//Update index attribute

	t_rc DropIndex (const char *relName, const char *attrName);			//Update index attribute

	t_rc GetDataAttributes(const char *relName, vector<t_attrInfo> &attrInfo);

	t_rc GetRelationInfo(const char *relName, t_relInfo &relInfo);


};

#endif /* SYSM_MANAGER_H_ */
