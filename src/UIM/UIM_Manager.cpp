/*
 * UIM_Manager.cpp
 *
 *      Author: santaris
 */

#include "UIM_Manager.h"
#include <iostream>
#include <map>
#include <sstream>
using namespace std;

UIM_Manager::UIM_Manager(SYSM_Manager &smManager,SSQLM_Manager &sqlManager) : smm (smManager), sqlm (sqlManager){

}
UIM_Manager::~UIM_Manager() {
	// TODO Auto-generated destructor stub
}


void UIM_Manager::SetCommand(string cmd){
	this->cmd = cmd;
}

//Start the execution of the command
t_rc UIM_Manager::ExecuteCommand(){

	t_rc rc;
	//Parse the command
	rc = parse.Parse(cmd);
	if(rc != OK)
		return rc;

	//Get the command type
	rc = parse.GetCommandType(cmdType);
	if(rc != OK)
		return rc;

	//Identify the comand
	rc = IdentifyCommand();
	if(rc != OK)
		return rc;

	return OK;
}

//Start the execution of each command
t_rc UIM_Manager::IdentifyCommand() {
	t_rc rc;
	switch(cmdType){
		case CREATEDATABASE_CMD :
			rc = ExecuteCreateDatabase();
			if(rc != OK)
				return rc;
			break;
		case DROPDATABASE_CMD :
			rc = ExecuteDropDatabase();
			if(rc != OK)
				return rc;
			break;
		case OPENDATABASE_CMD :
			rc = ExecuteOpenDatabase();
			if(rc != OK)
				return rc;
			break;
		case CLOSEDATABASE_CMD :
			rc = ExecuteCloseDatabase();
			if(rc != OK)
				return rc;
			break;
		case CREATETABLE_CMD :
			rc = ExecuteCreateTable();
			if(rc != OK)
				return rc;
			break;
		case DROPTABLE_CMD :
			rc = ExecuteDropTable();
			if(rc != OK)
				return rc;
			break;
		case CREATEINDEX_CMD :
			rc = ExecuteCreateIndex();
			if(rc != OK)
				return rc;
			break;
		case DROPINDEX_CMD :
			rc = ExecuteDropIndex();
			if(rc != OK)
				return rc;
			break;
		case SELECT_CMD :
			rc = ExecuteSelect();
			if(rc != OK)
				return rc;
			break;
		case INSERT_CMD :
			rc = ExecuteInsert();
			if(rc != OK)
				return rc;
			break;
		case DELETE_CMD :
			//rc = DeleteParse();
			if(rc != OK)
				return rc;
			break;
		case UPDATE_CMD :
			//rc = UpdateParse();
			if(rc != OK)
				return rc;
			break;
		default:
			return UIM_WRONGCOMMAND;
		}
	return OK;
}





t_rc UIM_Manager::ExecuteCreateDatabase() {

	t_rc rc;

	char *databaseName;
	rc = parse.GetDatabaseName(databaseName);
	if(rc != OK)
		return rc;

	rc = smm.CreateDb(databaseName);
	if(rc != OK)
		return rc;

	return OK;
}


t_rc UIM_Manager::ExecuteDropDatabase() {
	t_rc rc;

	char *databaseName;
	rc = parse.GetDatabaseName(databaseName);
	if(rc != OK)
		return rc;

	rc = smm.DestroyDb(databaseName);
	if(rc != OK)
		return rc;


	return OK;
}




t_rc UIM_Manager::ExecuteOpenDatabase() {
	t_rc rc;


	char *databaseName;
	rc = parse.GetDatabaseName(databaseName);
	if(rc != OK)
		return rc;

	rc = smm.OpenDb(databaseName);
	if(rc != OK)
		return rc;

	openDb = (char *) malloc (MAXNAME + 1);
	strcpy(openDb, databaseName);

	OpenFlag = true;

	return OK;
}

t_rc UIM_Manager::ExecuteCloseDatabase() {
	t_rc rc;
	if(!OpenFlag)
		return UIM_NODATABASEOPEN;

	char *databaseName;
	rc = parse.GetDatabaseName(databaseName);
	if(rc != OK)
		return rc;

	if(strcmp(databaseName, openDb) != 0)
		return UIM_WRONGDATABASENAME;

	rc = smm.CloseDb(databaseName);
	if(rc != OK)
		return rc;

	OpenFlag = false;
	openDb = NULL;

	return OK;
}



t_rc UIM_Manager::ExecuteCreateTable() {
	t_rc rc;
	if(!OpenFlag)
		return UIM_NODATABASEOPEN;

	rc = parse.GetCreateTableData(relInfo, attrInfo);
	if(rc != OK)
		return rc;

	rc = sqlm.CreateTable(relInfo, attrInfo);
	if(rc != OK)
		return rc;

	rc = smm.CreateTable(relInfo,attrInfo);
	if(rc != OK)
		return rc;


	return OK;

}


t_rc UIM_Manager::ExecuteDropTable() {
	t_rc rc;
	if(!OpenFlag)
		return UIM_NODATABASEOPEN;

	vector<t_attrInfo> attrInfo;
	vector<char *> relName;
	rc = parse.GetTableNames(relName);
	if(rc != OK)
		return rc;

	rc = smm.GetDataAttributes(relName.at(0), attrInfo);
	if(rc != OK)
		return rc;

	rc = sqlm.DropTable(relName.at(0), attrInfo);
	if(rc != OK)
		return rc;


	rc = smm.DropTable(relName.at(0));
	if(rc != OK)
		return rc;

	return OK;

}





t_rc UIM_Manager::ExecuteCreateIndex() {
	t_rc rc;
	if(!OpenFlag)
		return UIM_NODATABASEOPEN;

	vector<t_attrInfo> attrInfo;
	vector<char *> relName;
	rc = parse.GetTableNames(relName);
	if(rc != OK)
		return rc;
	
	//get table attributes

	vector<char *> attrName;
	rc = parse.GetAttributeNames(attrName);
	if(rc != OK)
		return rc;

	rc = smm.GetDataAttributes(relName.at(0), attrInfo);
	if(rc != OK)
		return rc;
	
	//get the next index number

	int indexNo;
	rc = smm.GetNextIndexNo(indexNo);
	if(rc != OK)
		return rc;

	t_attrInfo tempInfo;
	for(int i=0;i<attrInfo.size();i++){
		if(strcmp(attrInfo.at(i).attrName, attrName.at(0))) {
			tempInfo = attrInfo.at(i);
		}
	}
	
	//start sqlm create index
	rc = sqlm.CreateIndex(relName.at(0), tempInfo, indexNo);
	if(rc != OK)
		return rc;

	//start sysm create index
	rc = smm.CreateIndex(relName.at(0), attrName.at(0), indexNo);
	if(rc != OK)
		return rc;

	return OK;
}



t_rc UIM_Manager::ExecuteDropIndex() {
	t_rc rc;
	if(!OpenFlag)
		return UIM_NODATABASEOPEN;

	vector<t_attrInfo> attrInfo;
	vector<char *> relName;
	int indexNo;

	rc = parse.GetTableNames(relName);
	if(rc != OK)
		return rc;

	vector<char *> attrName;
	rc = parse.GetAttributeNames(attrName);
	if(rc != OK)
		return rc;


	rc = smm.GetDataAttributes(relName.at(0), attrInfo);
	if(rc != OK)
		return rc;

	for(int i=0;i<attrInfo.size();i++){
		if(strcmp(attrInfo.at(i).attrName, attrName.at(0))) {
			indexNo = attrInfo.at(i).indexNo;
		}
	}
	if(indexNo == -1)
		return UIM_ATTRIBUTENOTINDEXED;

	std::stringstream indexName;
	indexName << relName.at(0) << "." << indexNo;

	rc = sqlm.DestroyIndex(indexName.str().data(), indexNo);
	if(rc != OK)
		return rc;

	rc = smm.DropIndex(relName.at(0), attrName.at(0));
	if(rc != OK)
		return rc;

	return OK;
}

t_rc UIM_Manager::ExecuteSelect() {
	t_rc rc;
	if(!OpenFlag)
		return UIM_NODATABASEOPEN;

	vector<char *> attrNames;
	rc = parse.GetAttributeNames(attrNames);
	if(rc != OK) 
		return rc;

	vector<char *> tableNames;
	rc = parse.GetTableNames(tableNames);
	if(rc != OK)
		return rc;

	if(tableNames.size() > 3)
		return UIM_TOOMUCHTABLES;

	vector<char *> comparisons;
	rc = parse.GetComparisons(comparisons);
	if(rc != OK)
		return rc;

	vector<t_attrInfo> attrInfo;
	vector<t_attrInfo> tableAttributes;
	for(int i = 0; i < tableNames.size(); i++) {
		attrInfo.clear();
		rc = smm.GetDataAttributes(tableNames.at(i), attrInfo);
		if(rc != OK)
			return rc;

		for(int j = 0; j < attrInfo.size(); j++) {
			tableAttributes.push_back(attrInfo.at(j));
		}
	}


	rc = sqlm.Select(tableNames, attrNames,comparisons, tableAttributes);
	if(rc != OK)
		return rc;

	return OK;
}

t_rc UIM_Manager::ExecuteInsert() {
	t_rc rc;
	if(!OpenFlag)
		return UIM_NODATABASEOPEN;

	vector<char *> relName;
	rc = parse.GetTableNames(relName);
	if(rc != OK)
		return rc;

	vector<char *> values;
	rc = parse.GetValues(values);
	if(rc != OK)
		return rc;

	vector<t_attrInfo> attrInfo;
	rc = smm.GetDataAttributes(relName.at(0), attrInfo);
	if(rc != OK)
		return rc;

	t_relInfo relInfo;
	rc = smm.GetRelationInfo(relName.at(0), relInfo);
	if(rc != OK)
		return rc;



	rc = sqlm.Insert(relName.at(0),relInfo, attrInfo, values);
	if(rc != OK)
		return rc;

	return OK;
}

