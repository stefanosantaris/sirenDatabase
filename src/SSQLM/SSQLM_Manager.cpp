/*
 * SSQLM_Manager.cpp
 *
 *      Author: santaris
 */

#include "SSQLM_Manager.h"
#include <iostream>
#include <sstream>
using namespace std;

SSQLM_Manager::SSQLM_Manager(REM_RecordFileManager &rfManager/*, INXM_IndexManager inxManager*/) : rfm(rfManager)/*,inxm(inxManager)*/ {
	// TODO Auto-generated constructor stub

}

SSQLM_Manager::~SSQLM_Manager() {
	// TODO Auto-generated destructor stub
}

//Set the command
void SSQLM_Manager::SetCommand(vector<string> cmd, t_cmd cmdType) {
	this->cmd = cmd;
	this->cmdType = cmdType;
}

//Create the table according to the relInfo and the attrInfo
t_rc SSQLM_Manager::CreateTable(t_relInfo &relInfo,vector<t_attrInfo> &attrInfo) {
	t_rc rc;

	rc = rfm.CreateRecordFile(relInfo.relName, relInfo.recLength);
	if(rc != OK)
		return rc;



	return OK;
}

//Drop table and drop each available index
t_rc SSQLM_Manager::DropTable(const char *tableName, vector<t_attrInfo> attrInfo) {

	t_rc rc;

	rc = rfm.DestroyRecordFile(tableName);
	if(rc != OK)
		return rc;
/*
	for(int i = 0; i<attrInfo.size(); i++) {
		rc = inxm.DestroyIndex(tableName, attrInfo.at(i).indexNo);
		if(rc != OK)
			return rc;
	}
*/

	return OK;
}


//Create index
t_rc SSQLM_Manager::CreateIndex(const char *tableName, t_attrInfo attrInfo, int indexNo) {
	t_rc rc;

	//Open table file
	REM_RecordFileHandle rfh;
	rc = rfm.OpenRecordFile(tableName, rfh);
	if(rc != OK)
		return rc;



	/*rc = inxm.CreateIndex(tableName, indexNo, attrInfo.attrType, attrInfo.attrLength);
	if(rc != OK)
		return rc;

	INXM_IndexHandle ih;
	rc = inxm.OpenIndex(indexName, indexNo, ih);
	if(rc != OK)
		retrn rc;
*/
	//Insert index entries
	rc = InsertIndexEntries(rfh, attrInfo.offset/*, ih*/);
	if(rc != OK)
		return rc;

	/*rc = inxm.CloseIndex(ih);
	if(rc != OK)
		return rc;
*/
	rc = rfm.CloseRecordFile(rfh);
	if(rc != OK)
		return rc;




	return OK;

}

//Insert the exact field of each record into the indexing
t_rc SSQLM_Manager::InsertIndexEntries(REM_RecordFileHandle &rfh, int attrOffset/*, INXM_IndexHandle ih*/) {
	t_rc rc;
	int numReservedPages = rfh.sfh.GetNumReservedPages();
	int numberofSlots = rfh.rfsh.nrecords;

	REM_RecordID rid;
	REM_RecordHandle rh;
	int slot = 0;
	int pageID = 2;
	char *data;

	if(numReservedPages < 2)
		return SSQLM_NOENTRYTOINDEX;

	if(numberofSlots == 0)
		return SSQLM_NOENTRYTOINDEX;

	for( ; pageID <= numReservedPages; pageID++) {
		for( ; slot <= rfh.rfsh.recordsPerPage; slot++) {
			if(slot >(rfh.rfsh.nrecords - 1))
				return OK;

			rc = rid.SetPageID(pageID);
			if(rc != OK)
				return rc;

			rc = rid.SetSlot(slot);
			if(rc != OK)
				return rc;

			rc = rfh.ReadRecord(rid, rh);
			if(rc != OK)
				return rc;

			rc = rh.GetData(data);
			if(rc != OK)
				return rc;

			/*rc = ih.InsertEntry(data[attrOffset], rid);
			if(rc != OK)
				return rc;
*/
		}
	}
}


//Destroy index
t_rc SSQLM_Manager::DestroyIndex(const char *tableName, int indexNo) {
	t_rc rc;

	/*rc = inxm.DestroyIndex(tableName, indexNo);
	if(rc != OK)
		return rc;
*/
	return OK;
}

//Execute Select
t_rc SSQLM_Manager::Select(vector<char *> tableNames, vector<char *> attrNames, vector<char *> comparisons,  vector<t_attrInfo> tableAttributes){
	t_rc rc;
	int i = 0;
	vector<char *> pData;
	while(i < comparisons.size()) {
		rc = CalculateComparison(tableAttributes, comparisons.at(i), comparisons.at(i+1), comparisons.at(i+2), pData);
		if(rc != OK)
			return rc;

		i = i+3;
	}
	
	return OK;
}


//Insert command
t_rc SSQLM_Manager::Insert(const char *tableName, t_relInfo relInfo, vector<t_attrInfo> attrInfo, vector<char *> values) {
	t_rc rc;
	REM_RecordFileHandle rfh;

	rc = rfm.OpenRecordFile(tableName, rfh);
	if(rc != OK)
		return rc;

	char *record;
	rc = GetRecord(attrInfo, relInfo, values, record);
	if(rc != OK)
		return rc;

	REM_RecordID rid;
	rc = rfh.InsertRecord(record, rid);
	if(rc != OK)
		return rc;

	rc = rfm.CloseRecordFile(rfh);
	if(rc != OK)
		return rc;

	return OK;
}

//Create the record according to the metadata information
t_rc SSQLM_Manager::GetRecord(vector<t_attrInfo> attrInfo,t_relInfo relInfo, vector<char *> values, char *&record) {
	t_rc rc;
	if(attrInfo.size() != values.size())
		return SSQLM_INVALIDCOMMAND;

	int intNum;
	float floatNum;
	double doubleNum;

	record = (char *) malloc (relInfo.recLength);
	for(int i = 0; i < attrInfo.size(); i++) {
		switch(attrInfo.at(i).attrType){
		case TYPE_STRING :
			strcpy(&record[attrInfo.at(i).offset],values.at(i));
			break;

		case TYPE_INT :
			intNum = atoi(values.at(i));
			memcpy(&record[attrInfo.at(i).offset], values.at(i), attrInfo.at(i).attrLength);
			break;
		case TYPE_FLOAT :
			floatNum = atof(values.at(i));
			memcpy(&record[attrInfo.at(i).offset], values.at(i), attrInfo.at(i).attrLength);
			break;
		case TYPE_DOUBLE :
			doubleNum = strtod(values.at(i), NULL);
			memcpy(&record[attrInfo.at(i).offset], values.at(i), attrInfo.at(i).attrLength);
			break;
		}

	}

	return OK;
}


t_rc SSQLM_Manager::CalculateComparison(vector<t_attrInfo> tableAttributes, char *comparison, char *comperator, char *convertion, vector<char *> &pData) {
	t_rc rc;
	t_attrInfo comparisonAttribute;
	t_attrInfo convertionAttribute;
	bool comparisonFlag = false;
	bool convertionFlag = false;
	char *charConvertion;
	int intConvertion;
	float floatConvertion;
	double doubleConvertion;
	t_compOp compOp;

	for(int i = 0; i < tableAttributes.size() ; i++ ) {
		if(strcmp(tableAttributes.at(i).attrName, comparison) == 0) {
			comparisonAttribute = tableAttributes.at(i);
			comparisonFlag = true;
		}
		
		if(strcmp(tableAttributes.at(i).attrName, convertion) == 0) {
			convertionAttribute = tableAttributes.at(i);
			convertionFlag = true;
		}
	}

	if(strcmp(comperator,">") == 0) {
		compOp = GT_OP;
	}
	else if(strcmp(comperator,"<") == 0) {
		compOp = LT_OP;
	}
	else if(strcmp(comperator,"=") == 0) {
		compOp = EQ_OP;
	}
	else if(strcmp(comperator,">=") == 0) {
		compOp = GE_OP;
	}
	else if(strcmp(comperator,"<=") == 0) {
		compOp = LE_OP;
	}
	else if(strcmp(comperator,"<>") == 0) {
		compOp = NE_OP;
	}
	else
		return SSQLM_INVALIDCOMMAND;

	if((comparisonFlag == true) && (convertionFlag == true)) {
		rc = CompareAttributes(comparisonAttribute, convertionAttribute,compOp, pData);
		if(rc != OK)
			return rc;
	}

	if(!comparisonFlag) {
		string ss;
		ss = comparison;
		char *table;
		char *attribute;
		int found = ss.find(".");
		if(found!=string::npos) {
			table = (char *) malloc (MAXNAME + 1);
			attribute = (char *) malloc (MAXNAME + 1);
			strcpy(table, ss.substr(0, int(found)).data());
			strcpy(attribute, ss.substr(int(found)+1, ss.length()).data());
			for(int i = 0; i < tableAttributes.size(); i++) {
				if((strcmp(tableAttributes.at(i).relName, table) == 0) && (strcmp(tableAttributes.at(i).attrName, attribute) == 0)) {
					comparisonAttribute = tableAttributes.at(i);
				}
			}
			if(convertionFlag) {
				rc = CompareAttributes(comparisonAttribute, convertionAttribute,compOp, pData);
				if(rc != OK)
					return rc;
			}

		}else
			return SSQLM_INVALIDCOMMAND;
	}

	if(!convertionFlag) {
		string ss;
		ss = convertion;
		char *table;
		char *attribute;
		int found = ss.find(".");
		if(found!=string::npos) {
			table = (char *) malloc (MAXNAME + 1);
			attribute = (char *) malloc (MAXNAME + 1);
			strcpy(table, ss.substr(0, int(found)).data());
			strcpy(attribute, ss.substr(int(found)+1, ss.length()).data());
			for(int i = 0; i < tableAttributes.size(); i++) {
				if((strcmp(tableAttributes.at(i).relName, table) == 0) && (strcmp(tableAttributes.at(i).attrName, attribute) == 0)) {
					convertionAttribute = tableAttributes.at(i);
				}
			}
		
			rc = CompareAttributes(comparisonAttribute, convertionAttribute,compOp, pData);
			if(rc != OK)
				return rc;

		}else {
			rc = CompareValue(comparisonAttribute, convertion, compOp, pData);
			switch(comparisonAttribute.attrType) {
			case TYPE_STRING :
				charConvertion = (char *) malloc (MAXNAME + 1);
				strcpy(charConvertion, convertion);
				break;
			case TYPE_INT :
				intConvertion = atoi(convertion);
				break;
			case TYPE_FLOAT :
				floatConvertion = atof(convertion);
				break;
			case TYPE_DOUBLE :
				doubleConvertion = strtod(convertion, NULL);
				break;
			default :
				return SSQLM_INVALIDCOMMAND;
			}
		}
			
	}


	return OK;
}

t_rc SSQLM_Manager::CompareValue(t_attrInfo comparisonAttribute, char *convertion, t_compOp compOp, vector<char *> &pData) {
	t_rc rc;
	if(comparisonAttribute.indexNo != -1) {
		/*INXM_IndexHandle indesHandle;
		INXM_IndexScan indexScan;
		

		std::stringstream ss;
		ss << comparisonAttribute.relName << "." << comparisonAttribute.indexNo;
		
		rc = inxm.OpenIndex(ss.str(), comparisonAttribute.indexNo, indexHandle);
		if(rc != OK)
			return rc;

		rc = indexScan.OpenIndexScan(indexHandle, compOp, convertion);
		if(rc != OK)
			return rc;

		char *data;
		while(GetNextEntry(rid) == OK) {
			rc = rfh.ReadRecord(rid, rh);
			if(rc != OK)
				return rc;

			rc = rh.GetData(data);
			if(rc != OK)
				return rc;

			pData.push_back(data);
		}
		rc = fileScan.CloseIndexScan();
		if(rc != OK)
			return rc;

		rc = inxm.CloseIndex(indexHandle);
		if(rc != OK)
			return rc;*/
	}else{
		REM_RecordFileHandle rfh;
		REM_RecordHandle rh;
		REM_RecordID rid;
		rc = rfm.OpenRecordFile(comparisonAttribute.relName, rfh);
		if(rc != OK)
			return rc;

		//void *value;
		double doubleNum;
		int intNum;
		float floatNum;
		REM_RecordFileScan fileScan;

		switch(comparisonAttribute.attrType) {
		case TYPE_DOUBLE :
			doubleNum = strtod(convertion,NULL);
			rc = fileScan.OpenRecordScan(rfh, comparisonAttribute.attrType, comparisonAttribute.attrLength, comparisonAttribute.offset, compOp, &doubleNum);
			if(rc != OK)
				return rc;
			break;
		case TYPE_INT :
			intNum = atoi(convertion);
			rc = fileScan.OpenRecordScan(rfh, comparisonAttribute.attrType, comparisonAttribute.attrLength, comparisonAttribute.offset, compOp, &intNum);
			if(rc != OK)
				return rc;
			break;
		case TYPE_FLOAT :
			floatNum = atof(convertion);
			rc = fileScan.OpenRecordScan(rfh, comparisonAttribute.attrType, comparisonAttribute.attrLength, comparisonAttribute.offset, compOp, &floatNum);
			if(rc != OK)
				return rc;
			break;
		case TYPE_STRING:
			rc = fileScan.OpenRecordScan(rfh, comparisonAttribute.attrType, comparisonAttribute.attrLength, comparisonAttribute.offset, compOp, convertion);
			if(rc != OK)
				return rc;
			break;
		}


		char *data;
		while(fileScan.GetNextRecord(rh) == OK) {
			rc = rh.GetData(data);
			if(rc != OK)
				return rc;

			pData.push_back(data);
		}

		rc = fileScan.CloseRecordScan();
		if(rc != OK)
			return rc;
		rc = rfm.CloseRecordFile(rfh);
		if(rc != OK)
			return rc;
	}

	return OK;
}

t_rc SSQLM_Manager::CompareAttributes(t_attrInfo comparisonAttribute, t_attrInfo convertionAttribute, t_compOp compOp, vector<char *> &pData) {
	t_rc rc;
	REM_RecordFileHandle rfhCompare;
	REM_RecordFileHandle rfhConvertion;
	REM_RecordHandle rhCompare;
	REM_RecordHandle rhConvertion;
	REM_RecordID ridCompare;
	REM_RecordID ridConvertion;
	if(comparisonAttribute.indexNo != -1) {
		/*INXM_IndexHandle indesHandle;
		INXM_IndexScan indexScan;
		

		std::stringstream ss;
		ss << comparisonAttribute.relName << "." << comparisonAttribute.indexNo;
		
		rc = inxm.OpenIndex(ss.str(), comparisonAttribute.indexNo, indexHandle);
		if(rc != OK)
			return rc;

		rc = indexScan.OpenIndexScan(indexHandle, compOp, 

*/

	}else {
		if(comparisonAttribute.relName != convertionAttribute.relName) {
			rc = rfm.OpenRecordFile(comparisonAttribute.relName, rfhCompare);
			if(rc != OK)
				return rc;
			rc = rfm.OpenRecordFile(convertionAttribute.relName, rfhConvertion);
			if(rc != OK)
				return rc;

			REM_RecordFileScan fileScan;
		
			int slotCompare = 0;
			int pageIDCompare = 2;
			int slotConvertion = 0;
			int pageIDConvertion = 2;

			for( ;pageIDCompare <= rfhCompare.sfh.GetNumReservedPages(); pageIDCompare++) {
				for( ;slotCompare <= rfhCompare.rfsh.nrecords; slotCompare++) {
					rc = ridCompare.SetPageID(pageIDCompare);
					if(rc != OK)
						return rc;
					
					rc = ridCompare.SetSlot(slotCompare);
					if(rc != OK)
						return rc;

					rc = rfhCompare.ReadRecord(ridCompare, rhCompare);
					if(rc != OK)
						return rc;

					char *dataCompare;
					rc = rhCompare.GetData(dataCompare);
					if(rc != OK)
						return rc;

					rc = fileScan.OpenRecordScan(rfhConvertion, convertionAttribute.attrType, convertionAttribute.attrLength, convertionAttribute.offset, compOp, dataCompare);
					if(rc != OK)
						return rc;

					char *dataConvertion;
					while(fileScan.GetNextRecord(rhConvertion) == OK){
						rc = rhConvertion.GetData(dataConvertion);
						if(rc != OK)
							return rc;

						pData.push_back(dataCompare);
						pData.push_back(dataConvertion);
					}
					fileScan.CloseRecordScan();
				}
			}
		}
	}
	return OK;
}
