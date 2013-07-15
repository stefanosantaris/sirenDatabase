/*
 * REM_RecordHandle.cpp
 *
 *      Author: santaris
 */

#include "REM_RecordHandle.h"

REM_RecordHandle::REM_RecordHandle() {
	// TODO Auto-generated constructor stub

}

REM_RecordHandle::~REM_RecordHandle() {
	// TODO Auto-generated destructor stub
}

t_rc REM_RecordHandle::GetData(char *&pData) const{
	pData = this->ppData;
	return OK;

}

t_rc REM_RecordHandle::GetRecordID(REM_RecordID &rid) const{
	rid = this->rid;
	return OK;
}

t_rc REM_RecordHandle::SetData(char &data,const REM_RecordID &rid){
	ppData = &data;
	if(ppData[0]=='@')
		return REM_INVALIDRECORDID;
	this->rid=rid;
	return OK;
}


