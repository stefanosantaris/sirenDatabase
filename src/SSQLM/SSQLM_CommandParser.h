/*
 * SSQLM_CommandParser.h
 *
 *  Created on: Aug 24, 2010
 *      Author: santaris
 */

#ifndef SSQLM_COMMANDPARSER_H_
#define SSQLM_COMMANDPARSER_H_
#include "../REM/REM.h"
#include "SSQLM_Parameters.h"
#include <iostream>
#include <map>

class SSQLM_CommandParser {
private:
	t_relInfo relInfo;
	vector<t_attrInfo> attrInfo;
	string cmd;
	t_cmd cmdType;

	int offset;
	t_attrType attrType;
	int attrLength;
	int indexNo;

	char *databaseName;
	vector<string> tokenizedCommand;
	vector<char *> values;
	vector<char *> tableNames;
	vector<char *> attributeNames;
	vector<char *> logicalOperator;
	vector<char *> comparisons;

	t_rc CheckCommandValidation();
	void FormatCommand();

	void TokenizeCommand();
	t_rc IdentifyCommandOption();

	t_rc CreateDatabaseParse();
	t_rc DropDatabaseParse();
	t_rc OpenDatabaseParse();
	t_rc CloseDatabaseParse();

	//ddl commands parse
	t_rc CreateTableParse();
	t_rc DropTableParse();
	t_rc CreateIndexParse();
	t_rc DropIndexParse();


	//dml commands parse
	t_rc SelectParse();
	t_rc InsertParse();
	/*t_rc DeleteParse();
	t_rc UpdateParse();
	*/

	t_rc GetDataAttributeInfo();
public:
	SSQLM_CommandParser();
	~SSQLM_CommandParser();


	t_rc Parse(string cmd);

	t_rc GetCreateTableData(t_relInfo &relInfo, vector<t_attrInfo> &attrInfo);

	t_rc GetTableNames(vector<char *> &tableNames);

	t_rc GetAttributeNames(vector<char *> &attrNames);

	t_rc GetComparisons(vector<char *> &comparisons);

	t_rc GetValues(vector<char *> &values);

	t_rc GetDatabaseName(char *&databaseName);

	t_rc GetCommandType(t_cmd &cmdType);

};

#endif /* SSQLM_COMMANDPARSER_H_ */
