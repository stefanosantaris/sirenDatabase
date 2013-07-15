/*
 * SYSM_Manager.cpp
 *
 *      Author: santaris
 */

#include "SYSM_Manager.h"
#include <stdio.h>
#include <iostream>
#include <list>

using namespace std;
//OS identifier and include definition
#if defined (__APPLE__) || defined (__linux__)
	#include <sys/stat.h>
#else
	#include <direct.h>
#endif

SYSM_Manager::SYSM_Manager(REM_RecordFileManager &rmManager) : rfm(rmManager) {
	relMetName = "rel.met";
	attrMetName = "attr.met";
	openFlag = false;
}

SYSM_Manager::~SYSM_Manager() {
}

//Creates a new Database
t_rc SYSM_Manager::CreateDb(const char *dbName) {
	int result;
#if defined (__APPLE__) || defined (__linux__)
	//Store the working directory
	getcwd(cwd,99);

	//Create the database directory
	result = mkdir (dbName, 00700);
	if(result == -1)
		return SYSM_CANNOTCREATEDATABASE;

	//Access database directory
	result = chdir (dbName);
	if(result == -1)
		return SYSM_CANNOTACCESSDATABASE;
#else
	_getcwd(cwd,99);

	
	//Create the database directory
	result = _mkdir (dbName);
	if(result == -1)
		return SYSM_CANNOTCREATEDATABASE;

	//Access database directory
	result = _chdir (dbName);
	if(result == -1)
		return SYSM_CANNOTACCESSDATABASE;



#endif
	REM_RecordFileHandle tempFileHandle;
	REM_RecordID rid;
	SYSM_TupleHandle tuple;
	char *pData;
	t_rc rc;

	//Create rel.met
	rc = rfm.CreateRecordFile(relMetName,MAXNAME+3*INT_SIZE);
	if(rc != OK)
		return rc;

	//Open re.met
	rc = rfm.OpenRecordFile(relMetName,tempFileHandle);
	if(rc != OK)
		return rc;

	//Create rel.met Info
	t_relInfo relMetInfo[2] = {
			{"rel", MAXNAME+3*INT_SIZE, 4, 0},
			{"attr", 2*MAXNAME + 4*INT_SIZE}
	};

	for(int i=0; i < 2; i++) {
		tuple = SYSM_TupleHandle (relMetInfo[i]);

		rc = tuple.GetTupleData(pData);
		if(rc != OK)
			return rc;

		rc = tempFileHandle.InsertRecord(pData, rid);
		if(rc != OK)
			return rc;
	}

	//Flush all the pages
	rc = tempFileHandle.FlushPages();
	if(rc != OK)
		return rc;
	//Close rel.met
	rc = rfm.CloseRecordFile(tempFileHandle);
	if(rc != OK)
		return rc;

	rc = rfm.CreateRecordFile(attrMetName, 2 * MAXNAME + 4 * INT_SIZE);
	if(rc != OK)
		return rc;

	//Open the attr.met
	rc = rfm.OpenRecordFile(attrMetName,tempFileHandle);
	if(rc != OK)
		return rc;


	//Create the attr.met Info
	t_attrInfo attrMetInfo [10] = {
			{"rel", "relName", 0, TYPE_STRING, MAXNAME, -1 },
			{"rel", "recLength", MAXNAME, TYPE_STRING, MAXNAME, -1},
			{"rel", "attrCount", 2 * MAXNAME, TYPE_INT, INT_SIZE, -1},
			{"rel", "indexCount", 2 * MAXNAME + INT_SIZE, TYPE_INT, INT_SIZE, -1},
			{"attr", "relName", 0, TYPE_STRING, MAXNAME, -1},
			{"attr", "attrName", MAXNAME, TYPE_STRING, MAXNAME, -1},
			{"attr", "attrOffset", 2 * MAXNAME, TYPE_INT, INT_SIZE, -1},
			{"attr", "attrType", 2 * MAXNAME + INT_SIZE, TYPE_INT, INT_SIZE, -1},
			{"attr", "attrLength", 2 * MAXNAME+2 * INT_SIZE, TYPE_INT, INT_SIZE, -1},
			{"attr", "indexNo", 2 * MAXNAME + 3 * INT_SIZE, TYPE_INT, INT_SIZE, -1}
	};

	//Insert the attr data into the attr.met file
	for(int j=0; j < 10; j++){
		tuple = SYSM_TupleHandle(attrMetInfo[j]);

		rc = tuple.GetTupleData(pData);
		if (rc != OK)
			return rc;
		 rc = tempFileHandle.InsertRecord(pData, rid);
		 if(rc != OK)
			 return rc;
	}


	rc = tempFileHandle.FlushPages();
	if(rc != OK)
		return rc;

	rc = rfm.CloseRecordFile(tempFileHandle);
	if(rc != OK)
		return rc;

#if defined(__APPLE__) || defined(__linux__)
	result = chdir(cwd);
	if(result == -1)
		return SYSM_CANNOTACCESSDATABASE;
#else
	result = _chdir(cwd);
	if(result == -1)
		return SYSM_CANNOTACCESSDATABASE;
#endif
	return OK;

}

t_rc SYSM_Manager::DestroyDb(const char *dbName) {
	char cmd[80];
#if defined (__APPLE__) || (__linux__)
	//Create system command to delete directory
	sprintf(cmd, "rm -r %s", dbName);
	if(system (cmd) == -1)
		return SYSM_CANNOTDELETEDATABASE;
#else
	//Create system command to delete directory
	int result = _chdir(dbName);
	if(result != 0)
		return SYSM_CANNOTACCESSDATABASE;

	result = remove("*");
	if(result != 0){ 
	
		//return SYSM_CANNOTDELETEFILES;
	}

	result = _chdir(cwd);
	if(result != 0)
		return SYSM_CANNOTACCESSDATABASE;

	result = _rmdir(dbName);
	if(result != 0)
		return SYSM_CANNOTDELETEDATABASE;
#endif
	return OK;
}

t_rc SYSM_Manager::OpenDb(const char *dbName) {

	if(openFlag)
		return SYSM_DATABASEALREADYOPENED;
#if defined(__APPLE__) || defined(__linux__)
	getcwd (cwd, 80);
	if(chdir (dbName) < 0)
		return SYSM_CANNOTACCESSDATABASE;
#else
	_getcwd (cwd, 80);
	if(_chdir (dbName) < 0)
		return SYSM_CANNOTACCESSDATABASE;
#endif

	t_rc rc;
	//Open rel.met
	rc = rfm.OpenRecordFile(relMetName, relFileHandle);
	if(rc != OK)
		return rc;

	//Open attr.met
	rc = rfm.OpenRecordFile(attrMetName, attrFileHandle);
	if(rc != OK)
		return rc;

	openFlag = true;
	return OK;

}

t_rc SYSM_Manager::CloseDb(const char *dbName) {
	if(!openFlag)
		return SYSM_DATABASECLOSED;

	t_rc rc;
	//Flush all the relation pages to disk
	rc = relFileHandle.FlushPages();
	if(rc != OK)
		return rc;
	//Flush all the attribute pages to disk
	rc = attrFileHandle.FlushPages();
	if(rc != OK)
		return rc;

	//Close rel.met
	rc = rfm.CloseRecordFile(relFileHandle);
	if(rc != OK)
		return rc;

	//close attr.met
	rc = rfm.CloseRecordFile(attrFileHandle);
	if(rc != OK)
		return rc;

#if defined(__APPLE__) || defined(__linux__)
	chdir (cwd);
#else
	_chdir(cwd);
#endif

	openFlag = false;


	return OK;
}


t_rc SYSM_Manager::CreateTable(t_relInfo relInfo, vector<t_attrInfo> attrInfo) {
	//Check if file is closed
	if(!openFlag)
		return SYSM_DATABASECLOSED;

	char metName[30];
	strcpy(metName, relInfo.relName);
	strcat(metName, ".met");

	//Check if name is rel.met or attr.met
	if(strcmp(metName, relMetName) == 0
			|| strcmp(metName, attrMetName) == 0)
		return SYSM_METADATANAME;

	REM_RecordFileScan fileScan;
	REM_RecordHandle rh;
	t_rc rc;
	//Opens a record scan
	rc = fileScan.OpenRecordScan(relFileHandle, TYPE_STRING, MAXNAME, 0, EQ_OP, relInfo.relName);
	if(rc != OK)
		return rc;

	//Check if the table already exists
	if(fileScan.GetNextRecord(rh) == OK) {
		rc = fileScan.CloseRecordScan();
		if(rc != OK)
			return rc;

		return SYSM_TABLEALREADYEXISTS;
	}
	//Create a new table if it doesnot exist
	else {

		SYSM_TupleHandle tuple;
		REM_RecordID rid;
		char *pData;

		rc = fileScan.CloseRecordScan();
		if(rc != OK)
			return rc;

		//Create a tuple according to the relInfo
		tuple = SYSM_TupleHandle (relInfo);
		rc = tuple.GetTupleData(pData);
		if(rc != OK)
			return rc;

		//Insert the tuple
		rc = relFileHandle.InsertRecord (pData,rid);
		if (rc != OK)
			return rc;

		//Insert the attributes into the attr.met
		for (int i = 0; i < relInfo.attrCount; i++){
			tuple = SYSM_TupleHandle(attrInfo[i]);
			rc = tuple.GetTupleData(pData);
			if(rc != OK)
				return rc;
			rc = attrFileHandle.InsertRecord(pData,rid);
			if(rc != OK)
				return rc;
		}
	}

	return OK;
}


t_rc SYSM_Manager::DropTable(const char *relName) {
	//Check if the database is closed
	if(!openFlag)
		return SYSM_DATABASECLOSED;

	char metName[30];
	strcpy(metName, relName);
	strcat(metName, ".met");

	if(strcmp(metName, relMetName) == 0
			|| strcmp(metName, attrMetName) == 0)
		return SYSM_METADATANAME;


	t_rc rc;
	REM_RecordFileScan fileScan;
	REM_RecordHandle tempHandle;
	REM_RecordID rid;

	char rName[MAXNAME+1];
	memset (rName, 0, MAXNAME+1);
	strcpy (rName, relName);

	rc = fileScan.OpenRecordScan(relFileHandle, TYPE_STRING, MAXNAME, 0, EQ_OP, rName);
	if(rc != OK)
		return rc;

	//Check if the database exist
	if(fileScan.GetNextRecord(tempHandle) == OK) {
		rc = tempHandle.GetRecordID(rid);
		if(rc != OK)
			return rc;

		//Delete the record from the rel.met
		rc = relFileHandle.DeleteRecord(rid);
		if(rc != OK)
			return rc;

		rc = fileScan.CloseRecordScan();
		if(rc != OK)
			return rc;

		//Open attr.met for scan.
		rc = fileScan.OpenRecordScan(attrFileHandle, TYPE_STRING, MAXNAME, 0, EQ_OP, rName);
		if(rc != OK)
			return rc;

		while(fileScan.GetNextRecord(tempHandle) == OK) {
			rc = tempHandle.GetRecordID(rid);
			if(rc != OK)
				return rc;

			//Delete records
			rc = attrFileHandle.DeleteRecord(rid);
			if(rc != OK)
				return rc;
		}
		rc = fileScan.CloseRecordScan();
		if(rc != OK)
			return rc;

	}
	else {
		return SYSM_TABLEDOESNOTEXIST;
	}

	return OK;
}

t_rc SYSM_Manager::CreateIndex(const char *relName, const char *attrName,int indexNo) {
	//Check if database is closed
	if(!openFlag)
		return SYSM_DATABASECLOSED;

	char metName[30];
	strcpy(metName, relName);
	strcat(metName, ".met");

	if(strcmp(metName, relMetName) == 0
			|| strcmp(metName, attrMetName) == 0)
		return SYSM_METADATANAME;


	t_rc rc;
	REM_RecordFileScan fileScan;
	REM_RecordHandle recordHandle;
	REM_RecordID rid;

	char rName[MAXNAME+1];
	memset (rName, 0, MAXNAME+1);
	strcpy (rName, relName);

	rc = fileScan.OpenRecordScan(relFileHandle, TYPE_STRING, MAXNAME, 0, EQ_OP, rName);
	if(rc != OK)
		return rc;

	//Check if table exist into the rel.met
	if(fileScan.GetNextRecord(recordHandle) == OK) {
		t_relInfo relInfo;
		t_attrInfo attrInfo;
		SYSM_TupleHandle tuple;
		char *pData;

		rc = recordHandle.GetRecordID(rid);
		if(rc != OK)
			return rc;


		rc = recordHandle.GetData(pData);
		if(rc != OK)
			return rc;

		rc = tuple.GetRelInfo(pData, relInfo);
		if(rc != OK)
			return rc;

		//Increase the indexCount
		relInfo.indexCount++;

		rc = relFileHandle.DeleteRecord(rid);
		if(rc != OK)
			return rc;

		tuple = SYSM_TupleHandle(relInfo);
		rc = tuple.GetTupleData(pData);
		if(rc != OK)
			return rc;

		rc = relFileHandle.InsertRecord(pData, rid);
		if(rc != OK)
			return rc;

		rc = fileScan.CloseRecordScan();
		if(rc != OK)
			return rc;

		//Open attr.met
		rc = fileScan.OpenRecordScan(attrFileHandle, TYPE_STRING, MAXNAME, 0, EQ_OP, rName);
		if(rc != OK)
			return rc;

		char aName[MAXNAME+1];
		memset (aName, 0, MAXNAME+1);
		strcpy (aName, attrName);


		rc =  GetAttributeRecordHandle (fileScan, aName, recordHandle);
		if(rc != OK)
			return rc;

		rc = recordHandle.GetRecordID(rid);
		if(rc != OK)
			return rc;

		rc = recordHandle.GetData(pData);
		if(rc != OK)
			return rc;

		rc = tuple.GetAttrInfo(pData, attrInfo);
		if(rc != OK)
			return rc;

		if(attrInfo.indexNo != -1)
			return SYSM_ATTRIBUTEALREADYINDEXED;

		//Set indexNo of the attribute
		attrInfo.indexNo = indexNo;

		rc = attrFileHandle.DeleteRecord(rid);
		if(rc != OK)
			return rc;

		tuple = SYSM_TupleHandle(attrInfo);
		rc = tuple.GetTupleData(pData);
		if(rc != OK)
			return rc;

		rc = attrFileHandle.InsertRecord(pData, rid);
		if(rc != OK)
			return rc;

		rc = fileScan.CloseRecordScan();
		if(rc != OK)
			return rc;

	}
	else
		return SYSM_TABLEDOESNOTEXIST;


	return OK;
}

t_rc SYSM_Manager::DropIndex(const char *relName, const char *attrName) {
	if(!openFlag)
		return SYSM_DATABASECLOSED;

	char metName[30];
	strcpy(metName, relName);
	strcat(metName, ".met");

	if(strcmp(metName, relMetName) == 0
			|| strcmp(metName, attrMetName) == 0)
		return SYSM_METADATANAME;


	t_rc rc;
	REM_RecordFileScan fileScan;
	REM_RecordHandle recordHandle;
	REM_RecordID rid;

	char rName[MAXNAME+1];
	memset (rName, 0, MAXNAME+1);
	strcpy (rName, relName);

	rc = fileScan.OpenRecordScan(relFileHandle, TYPE_STRING, MAXNAME, 0, EQ_OP, rName);
	if(rc != OK)
		return rc;


	if(fileScan.GetNextRecord(recordHandle) == OK) {
			t_relInfo relInfo;
			t_attrInfo attrInfo;
			SYSM_TupleHandle tuple;
			char *pData;

			rc = recordHandle.GetRecordID(rid);
			if(rc != OK)
				return rc;


			rc = recordHandle.GetData(pData);
			if(rc != OK)
				return rc;

			rc = tuple.GetRelInfo(pData, relInfo);
			if(rc != OK)
				return rc;

			if(relInfo.indexCount == 0)
				return SYSM_ATTRIBUTENOTINDEXED;
			else
				relInfo.indexCount--;

			rc = relFileHandle.DeleteRecord(rid);
			if(rc != OK)
				return rc;

			tuple = SYSM_TupleHandle(relInfo);
			rc = tuple.GetTupleData(pData);
			if(rc != OK)
				return rc;

			rc = relFileHandle.InsertRecord(pData, rid);
			if(rc != OK)
				return rc;

			rc = fileScan.CloseRecordScan();
			if(rc != OK)
				return rc;

			rc = fileScan.OpenRecordScan(attrFileHandle, TYPE_STRING, MAXNAME, 0, EQ_OP, rName);
			if(rc != OK)
				return rc;

			char aName[MAXNAME+1];
			memset (aName, 0, MAXNAME+1);
			strcpy (aName, attrName);


			rc =  GetAttributeRecordHandle (fileScan, aName, recordHandle);
			if(rc != OK)
				return rc;

			rc = recordHandle.GetRecordID(rid);
			if(rc != OK)
				return rc;

			rc = recordHandle.GetData(pData);
			if(rc != OK)
				return rc;

			rc = tuple.GetAttrInfo(pData, attrInfo);
			if(rc != OK)
				return rc;

			if(attrInfo.indexNo == -1)
				return SYSM_ATTRIBUTENOTINDEXED;

			attrInfo.indexNo = -1;

			rc = attrFileHandle.DeleteRecord(rid);
			if(rc != OK)
				return rc;

			tuple = SYSM_TupleHandle(attrInfo);
			rc = tuple.GetTupleData(pData);
			if(rc != OK)
				return rc;

			rc = attrFileHandle.InsertRecord(pData, rid);
			if(rc != OK)
				return rc;

			rc = fileScan.CloseRecordScan();
			if(rc != OK)
				return rc;

		}
		else
			return SYSM_TABLEDOESNOTEXIST;

	return OK;
}


t_rc SYSM_Manager::GetAttributeRecordHandle (REM_RecordFileScan fileScan, char *attrName, REM_RecordHandle &recordHandle) {
	REM_RecordID rid;
	SYSM_TupleHandle tuple;
	t_rc rc;
	char *pData;
	t_attrInfo attrInfo;

	while(fileScan.GetNextRecord(recordHandle) == OK){
		rc = recordHandle.GetData(pData);
		if(rc != OK)
			return rc;

		rc = tuple.GetAttrInfo(pData, attrInfo);
		if(rc != OK)
			return rc;

		if(strcmp(attrInfo.attrName,attrName) == 0)
			return OK;
	}
	return SYSM_NOATTRIBUTEFOUND;
}

t_rc SYSM_Manager::GetNextIndexNo (int &indexNo) {

	t_rc rc;
	REM_RecordFileScan fileScan;
	REM_RecordHandle recordHandle;
	REM_RecordID rid;
	list<int> indexList;
	t_attrInfo attrInfo;
	SYSM_TupleHandle tuple;
	char *pData;

	int value = 0;
	//Open attr.met
	rc = fileScan.OpenRecordScan(attrFileHandle, TYPE_INT, INT_SIZE, 2*MAXNAME+3*INT_SIZE, GT_OP, &value);
	if(rc != OK)
		return rc;


	while(fileScan.GetNextRecord(recordHandle) == OK) {

		rc = recordHandle.GetData(pData);
		if(rc != OK)
			return rc;

		rc = tuple.GetAttrInfo(pData, attrInfo);
		if(rc != OK)
			return rc;

		//push back all the attributes into a list
		indexList.push_back(attrInfo.indexNo);

	}

	//Sort the list if it is not empty
	if(!indexList.empty()){
		indexList.sort();
		indexNo = indexList.back();
		indexNo++;
	}
	//Set indexNo to 1
	else
		indexNo = 1;

	rc = fileScan.CloseRecordScan();
	if(rc != OK)
		return rc;


	return OK;

}



t_rc SYSM_Manager::GetDataAttributes(const char *relName, vector<t_attrInfo> &attrInfo) {
	if(!openFlag)
		return SYSM_DATABASECLOSED;

	REM_RecordFileScan fileScan;
	REM_RecordHandle recordHandle;
	REM_RecordID rid;
	SYSM_TupleHandle tuple;
	t_relInfo relInfo;
	t_rc rc;
	char *pData;
	t_attrInfo tempInfo;

	char rName[MAXNAME+1];
	memset (rName, 0, MAXNAME+1);
	strcpy (rName, relName);

	rc = fileScan.OpenRecordScan(relFileHandle, TYPE_STRING, MAXNAME, 0, EQ_OP, rName);
	if(rc != OK)
		return rc;

	if(fileScan.GetNextRecord(recordHandle) == OK) {
		rc = recordHandle.GetData(pData);
		if(rc != OK)
			return rc;

		rc = tuple.GetRelInfo(pData, relInfo);
		if(rc != OK)
			return rc;

		rc = fileScan.CloseRecordScan();
		if(rc != OK)
			return rc;


		rc = fileScan.OpenRecordScan(attrFileHandle, TYPE_STRING, MAXNAME, 0, EQ_OP, rName);
		if(rc != OK)
			return rc;

		int i = 0;
		while(fileScan.GetNextRecord(recordHandle) == OK) {
			rc = recordHandle.GetData(pData);
			if(rc != OK)
				return rc;

			rc = tuple.GetAttrInfo(pData, tempInfo);
			if(rc != OK)
				return rc;
			attrInfo.push_back(tempInfo);
			i++;
		}

		rc = fileScan.CloseRecordScan();
		if(rc != OK)
			return rc;
	}
	else
		return SYSM_TABLEDOESNOTEXIST;
	return OK;

}


t_rc SYSM_Manager::GetRelationInfo(const char *relName, t_relInfo &relInfo) {
	if(!openFlag)
		return SYSM_DATABASECLOSED;

	t_rc rc;
	REM_RecordHandle recordHandle;
	REM_RecordFileScan fileScan;
	SYSM_TupleHandle tuple;
	char *pData;


	char rName[MAXNAME+1];
	memset (rName, 0, MAXNAME+1);
	strcpy (rName, relName);

	rc = fileScan.OpenRecordScan(relFileHandle, TYPE_STRING, MAXNAME, 0, EQ_OP, rName);
	if(rc != OK)
		return rc;
	if(fileScan.GetNextRecord(recordHandle) == OK) {
		rc = recordHandle.GetData(pData);
		if(rc != OK)
			return rc;

		rc = tuple.GetRelInfo(pData, relInfo);
		if(rc != OK)
			return rc;

		rc = fileScan.CloseRecordScan();
		if(rc != OK)
			return rc;
	}else
		return SYSM_TABLEDOESNOTEXIST;

	return OK;
}