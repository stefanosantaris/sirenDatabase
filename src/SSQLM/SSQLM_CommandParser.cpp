/*
 * SSQLM_CommandParser.cpp
 *
 *  Created on: Aug 24, 2010
 *      Author: santaris
 */

#include "SSQLM_CommandParser.h"
#include <iostream>
#include <stdio.h>
using namespace std;

SSQLM_CommandParser::SSQLM_CommandParser() {
	offset = 0;
	attrLength = 0;
	indexNo = -1;
}

SSQLM_CommandParser::~SSQLM_CommandParser() {
	// TODO Auto-generated destructor stub
}

//Parse the string
t_rc SSQLM_CommandParser::Parse(string cmd){
	if(cmd.empty())
		return SSQLM_INVALIDCOMMAND;
	//Clear all the previous situations
	tokenizedCommand.clear();
	tableNames.clear();
	attributeNames.clear();
	values.clear();
	attrInfo.clear();
	logicalOperator.clear();
	comparisons.clear();

	t_rc rc;
	this->cmd = cmd;
	//Check if the command is valid
	rc = CheckCommandValidation();
	if(rc != OK)
		return rc;

	//Format the command
	FormatCommand();
	
	//Tokenize the command
	TokenizeCommand();
	
	//Identify the command option
	rc = IdentifyCommandOption();
	if(rc != OK)
		return rc;

	return OK;
}


t_rc SSQLM_CommandParser::CreateDatabaseParse() {
	//Check if the command is valid
	if(tokenizedCommand.size() != 3)
		return SSQLM_INVALIDCOMMAND;

	//Get the database name
	databaseName = (char *) malloc (MAXNAME + 1);
	strcpy(databaseName, tokenizedCommand.at(2).data());

	return OK;
}

t_rc SSQLM_CommandParser::DropDatabaseParse() {
	//Check if the command is valid
	if(tokenizedCommand.size() != 3)
		return SSQLM_INVALIDCOMMAND;

	//Get the database name
	databaseName = (char *) malloc (MAXNAME + 1);
	strcpy(databaseName, tokenizedCommand.at(2).data());

	return OK;
}

t_rc SSQLM_CommandParser::OpenDatabaseParse() {
	//Check if the command is valid
	if(tokenizedCommand.size() != 3)
		return SSQLM_INVALIDCOMMAND;

	cout<<tokenizedCommand.size()<<endl;

	//Get the database name
	databaseName = (char *) malloc (MAXNAME + 1);
	strcpy(databaseName, tokenizedCommand.at(2).data());


	cout<<tokenizedCommand.size()<<endl;


	return OK;
}

t_rc SSQLM_CommandParser::CloseDatabaseParse() {
	//Check if the command is valid
	if(tokenizedCommand.size() != 3)
		return SSQLM_INVALIDCOMMAND;

	//Get the database name
	databaseName = (char *) malloc (MAXNAME + 1);
	strcpy(databaseName, tokenizedCommand.at(2).data());

	return OK;
}

t_rc SSQLM_CommandParser::CreateTableParse() {
	//Check if the tableName is bigger than the allowed one
	if(tokenizedCommand.at(2).size() > MAXNAME)
		return SSQLM_INVALIDTABLENAME;


	//Get the table Name and insert into a vector
	char *tableName;
	tableName = (char *) malloc (MAXNAME + 1);
	strcpy(tableName,tokenizedCommand.at(2).data());
	if(tableNames.empty())
		tableNames.clear();
	tableNames.push_back(tableName);

	t_rc rc;
	//Get data attributes information
	rc = GetDataAttributeInfo();
	if(rc != OK)
		return rc;

	if(attrInfo.size() > MAXATTRIBUTES)
		return SSQLM_MAXATTRIBUTES;


	strcpy(relInfo.relName, tableName);
	relInfo.recLength = offset;
	relInfo.attrCount = attrInfo.size();
	relInfo.indexCount = 0;

	return OK;
}

t_rc SSQLM_CommandParser::DropTableParse() {
	//Check if the command is valid
	if(tokenizedCommand.size() != 3)
		return SSQLM_INVALIDCOMMAND;

	//get the table name
	char *tableName;
	tableName = (char *) malloc (MAXNAME + 1);
	strcpy(tableName,tokenizedCommand.at(2).data());

	tableNames.push_back(tableName);

	return OK;
}

t_rc SSQLM_CommandParser::CreateIndexParse() {
	//Check if the command is valid
	if(tokenizedCommand.size() != 4)
		return SSQLM_INVALIDCOMMAND;

	//Get the table name
	char *tableName;
	tableName = (char *) malloc (MAXNAME + 1);
	strcpy(tableName, tokenizedCommand.at(2).data());

	//Get the attribute name
	char *attrName;
	attrName = (char *) malloc (MAXNAME + 1);
	strcpy(attrName, tokenizedCommand.at(3).data());

	tableNames.push_back(tableName);

	attributeNames.push_back(attrName);

	return OK;
}

t_rc SSQLM_CommandParser::DropIndexParse() {
	//Check if the command is valid
	if(tokenizedCommand.size() != 4)
		return SSQLM_INVALIDCOMMAND;

	//Get the table name
	char *tableName;
	tableName = (char *) malloc (MAXNAME + 1);
	strcpy(tableName, tokenizedCommand.at(2).data());
	
	//Get the attribute name
	char *attrName;
	attrName = (char *) malloc (MAXNAME + 1);
	strcpy(attrName, tokenizedCommand.at(3).data());

	tableNames.push_back(tableName);
	attributeNames.push_back(attrName);
	return OK;
}

t_rc SSQLM_CommandParser::SelectParse() {
	//Check if the command starts with select
	if(tokenizedCommand.at(0) != "SELECT")
		return SSQLM_INVALIDCOMMAND;

	int i=1;
	char *attribute;
	//Get the attributes
	while(tokenizedCommand.at(i) != "FROM") {
		attribute = (char *) malloc (MAXNAME + 1);
		strcpy(attribute, tokenizedCommand.at(i).data());
		attributeNames.push_back(attribute);
		i++;
		if(i > tokenizedCommand.size())
			return SSQLM_INVALIDCOMMAND;
	}
	
	//Get the table names
	
	i++;
	char *tableName;
	while(tokenizedCommand.at(i) != "WHERE") {
		tableName = (char *) malloc (MAXNAME + 1);
		strcpy(tableName, tokenizedCommand.at(i).data());
		tableNames.push_back(tableName);
		i++;
		if(i > tokenizedCommand.size())
			return SSQLM_INVALIDCOMMAND;
	}

	//Get the comparisons
	i++;
	char *comparison;
	for( ; i<tokenizedCommand.size();i++) {
		comparison = (char *) malloc (MAXNAME + 1);
		strcpy(comparison, tokenizedCommand.at(i).data());
		if(comparison == "AND" || comparison == "OR")
			logicalOperator.push_back(comparison);
		else
			comparisons.push_back(comparison);

	}

	return OK;
}




t_rc SSQLM_CommandParser::InsertParse() {
	//Get the table Name
	char *tableName;
	tableName = (char *) malloc (MAXNAME + 1);
	strcpy(tableName, tokenizedCommand.at(2).data());

	tableNames.push_back(tableName);
	char *pData;


	//Get the data
	for(int i = 4; i < tokenizedCommand.size(); i++) {
		pData = (char *) malloc (MAXNAME + 1);
		strcpy(pData,tokenizedCommand.at(i).data());
		values.push_back(pData);
	}


	return OK;
}
/*
t_rc SSQLM_CommandParser::DeleteParse() {
	return OK;
}

t_rc SSQLM_CommandParser::UpdateParse() {
	return OK;
}
*/


//Get the tableData
t_rc SSQLM_CommandParser::GetCreateTableData(t_relInfo &relInfo, vector<t_attrInfo> &attrInfo) {
	relInfo = this->relInfo;
	attrInfo = this->attrInfo;
	return OK;
}


//Get the table Names
t_rc SSQLM_CommandParser::GetTableNames(vector<char *> &tableNames) {
	tableNames = this->tableNames;
	return OK;
}


//Get the attribute names
t_rc SSQLM_CommandParser::GetAttributeNames(vector<char *> &attrNames) {
	attrNames = this->attributeNames;
	return OK;
}

//Check if the command is valid
t_rc SSQLM_CommandParser::CheckCommandValidation() {

	int iParentheses = 0;
    if( cmd.find( ";" ) == -1 )
    {
        return SSQLM_INVALIDCOMMAND;
    }
    else if( cmd.find( ";" ) != cmd.rfind( ";" ) )
    {
        return SSQLM_INVALIDCOMMAND;
    }
    else if( ( cmd.find( ";" ) + 1 ) != cmd.size() )
    {
        return SSQLM_INVALIDCOMMAND;
    }

    for(int i = 0; i < cmd.size(); i++) {
    	if(cmd[i] == '(')
    		iParentheses++;
    	else if(cmd[i] == ')') {
    		iParentheses--;
    		if(iParentheses < 0)
    			return SSQLM_INVALIDCOMMAND;
    	}
    }

	return OK;
}


//Format the command to upper case
void SSQLM_CommandParser::FormatCommand (){


	for(int i=0; i<cmd.size(); i++){
		cmd[i] = toupper(cmd[i]);
	}

}


//Tokenize the command
void SSQLM_CommandParser::TokenizeCommand () {

	const string &delimiters = " \t,;()";
    // Skip delimiters at beginning.
    string::size_type lastPos = cmd.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = cmd.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
    	tokenizedCommand.push_back(cmd.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = cmd.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = cmd.find_first_of(delimiters, lastPos);

    }

}


//Identify the command option
t_rc SSQLM_CommandParser::IdentifyCommandOption() {
	t_rc rc;
	if((tokenizedCommand.at(0) == "CREATE") && (tokenizedCommand.at(1) == "DATABASE")) {
		cmdType = CREATEDATABASE_CMD;
		rc = CreateDatabaseParse();
		if(rc != OK)
			return rc;
	}
	else if((tokenizedCommand.at(0) == "DROP") && (tokenizedCommand.at(1) == "DATABASE")) {
		cmdType = DROPDATABASE_CMD;
		rc = DropDatabaseParse();
		if(rc != OK)
			return rc;
	}
	else if((tokenizedCommand.at(0) == "OPEN") && (tokenizedCommand.at(1) == "DATABASE")) {
		cmdType = OPENDATABASE_CMD;
		rc = OpenDatabaseParse();
		if(rc != OK)
			return rc;
	}
	else if((tokenizedCommand.at(0) == "CLOSE") && (tokenizedCommand.at(1) == "DATABASE")) {
		cmdType = CLOSEDATABASE_CMD;
		rc = CloseDatabaseParse();
		if(rc != OK)
			return rc;
	}
	else if((tokenizedCommand.at(0) == "CREATE") && (tokenizedCommand.at(1) == "TABLE")) {
		cmdType = CREATETABLE_CMD;
		rc = CreateTableParse();
		if(rc != OK)
			return rc;
	}
	else if((tokenizedCommand.at(0) == "DROP") && (tokenizedCommand.at(1) == "TABLE")) {
		cmdType = DROPTABLE_CMD;
		rc = DropTableParse();
		if(rc != OK)
			return rc;
	}
	else if((tokenizedCommand.at(0) == "CREATE") && (tokenizedCommand.at(1) == "INDEX")) {
		cmdType = CREATEINDEX_CMD;
		rc = CreateIndexParse();
		if(rc != OK)
			return rc;
	}
	else if((tokenizedCommand.at(0) == "DROP") && (tokenizedCommand.at(1) == "INDEX")) {
		cmdType = DROPINDEX_CMD;
		rc = DropIndexParse();
		if(rc != OK)
			return rc;
	}
	else if(tokenizedCommand.at(0) == "SELECT") {
		cmdType = SELECT_CMD;
		rc = SelectParse();
		if(rc != OK)
			return rc;
	}
	else if(tokenizedCommand.at(0) == "INSERT" && (tokenizedCommand.at(1) == "INTO")) {
		cmdType = INSERT_CMD;
		rc = InsertParse();
		if(rc != OK)
			return rc;
	}
	return OK;
}


//Get the database name
t_rc SSQLM_CommandParser::GetDatabaseName(char *&databaseName) {
	databaseName = this->databaseName;
	return OK;
}

//Get the command type
t_rc SSQLM_CommandParser::GetCommandType(t_cmd &cmdType) {
	cmdType = this->cmdType;
	return OK;
}

//Get the data attribute info
t_rc SSQLM_CommandParser::GetDataAttributeInfo() {
	t_attrInfo tempInfo;
	int i=3;
	while(i<tokenizedCommand.size()) {
		if(tokenizedCommand.at(i).size() > MAXNAME)
			return SSQLM_INVALIDARGUMENTNAME;


		if(tokenizedCommand.at(i+1) == "INT") {
			attrType = TYPE_INT;
			attrLength = INT_SIZE;
		}
		else if(tokenizedCommand.at(i+1) == "FLOAT") {
			attrType = TYPE_FLOAT;
			attrLength = sizeof(float);
		}
		else if(tokenizedCommand.at(i+1) == "DOUBLE") {
			attrType = TYPE_DOUBLE;
			attrLength = sizeof(double);
		}
		else if(tokenizedCommand.at(i+1) == "CHAR") {
			attrType = TYPE_STRING;
			char *sizeChar = (char *) malloc (MAXNAME + 1);
			strcpy(sizeChar, tokenizedCommand.at(i+2).data());

			int size = atoi(sizeChar);
			if(size > MAXSTRINGLENGTH)
				return SSQLM_STRINGTOOLONG;
			attrLength = size;
		}

		strcpy(tempInfo.relName,tableNames.at(0));
		strcpy(tempInfo.attrName,tokenizedCommand.at(i).data());
		tempInfo.attrType = attrType;
		tempInfo.offset = offset;
		tempInfo.attrLength = attrLength;
		tempInfo.indexNo = -1;
		offset += attrLength;

		if(attrType == TYPE_STRING)
			i = i+3;
		else
			i = i+2;

		attrInfo.push_back(tempInfo);

	}
	return OK;
}

//Get the values
t_rc SSQLM_CommandParser::GetValues(vector<char *> &values) {
	if(this->values.empty())
		return SSQLM_NOVALUESTOINSERT;

	values = this->values;
	return OK;
}


//Get the comparisons
t_rc SSQLM_CommandParser::GetComparisons(vector<char *> &comparisons) {
	comparisons = this->comparisons;
	return OK;
}