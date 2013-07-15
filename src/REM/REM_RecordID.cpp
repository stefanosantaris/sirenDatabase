/*
 * REM_RecordID.cpp
 *
 *      Author: santaris
 */

#include "REM_RecordID.h"

REM_RecordID::REM_RecordID() {
	// TODO Auto-generated constructor stub

}

REM_RecordID::REM_RecordID(int pageID,int slot){
	pageId = pageID;
	this->slot=slot;

}

REM_RecordID::~REM_RecordID() {
	// TODO Auto-generated destructor stub
}


t_rc REM_RecordID::GetPageID(int &pageID)const{
	pageID= this->pageId;
	return OK;
}

t_rc REM_RecordID::GetSlot(int &slot)const{
	slot=this->slot;
	return OK;
}

t_rc REM_RecordID::SetPageID(int pageID){
	pageId = pageID;
	return OK;
}

t_rc REM_RecordID::SetSlot(int slot){
	this->slot=slot;
	return OK;
}



