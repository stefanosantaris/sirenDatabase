/*
 * SSQLM_Manager.h
 *
 *      Author: santaris
 */

#ifndef SSQLM_MANAGER_H_
#define SSQLM_MANAGER_H_
#include "../REM/REM.h"
//#include "../INXM/INXM.h"
#include "SSQLM_CommandParser.h"

class SSQLM_Manager {
	vector<string> cmd;
	t_cmd cmdType;
	REM_RecordFileManager rfm;
	//INXM_IndexManager inxm;



	t_rc InsertIndexEntries(REM_RecordFileHandle &rfh, int attrOffset/*, INXM_IndexHandle ih*/);

	t_rc GetRecord(vector<t_attrInfo> attrInfo, t_relInfo relInfo, vector<char *> values, char *&data);

	t_rc CalculateComparison(vector<t_attrInfo> tableAttributes, char *comparison, char *comperator, char * convertion,vector<char *> &pData);

	t_rc CompareAttributes(t_attrInfo comparisonAttribute, t_attrInfo convertionAttribute, t_compOp compOp, vector<char *> &pData);

	t_rc CompareValue(t_attrInfo comparisonAttribute, char *convertion, t_compOp compOp, vector<char *> &pData);


public:
	SSQLM_Manager(REM_RecordFileManager &rfm/*, INXM_IndexManager &inxm*/);
	~SSQLM_Manager();

	void SetCommand(vector<string> cmd, t_cmd cmdType);

	t_rc CreateTable(t_relInfo &relInfo,vector<t_attrInfo> &attrInfo);

	t_rc DropTable(const char *tableName, vector<t_attrInfo> attrInfo);

	t_rc CreateIndex(const char *tableName, t_attrInfo attrInfo, int indexNo);

	t_rc DestroyIndex(const char *tableName, int indexNo);

	t_rc Select(vector<char *> tableNames, vector<char *> attrNames,vector<char *> comparisons, vector<t_attrInfo> tableAttributes);

	t_rc Insert(const char *tableName, t_relInfo relInfo, vector<t_attrInfo> attrInfo, vector<char *> values);

};

#endif /* SSQLM_MANAGER_H_ */
