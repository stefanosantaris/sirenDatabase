/*
 * REM_RecordFileHandle.cpp
 *
 *      Author: santaris
 */

#include "REM_RecordFileHandle.h"
#include <stdio.h>

REM_RecordFileHandle::REM_RecordFileHandle() {
	// TODO Auto-generated constructor stub

}

REM_RecordFileHandle::~REM_RecordFileHandle() {
	// TODO Auto-generated destructor stub
}

//Read a record using a valid RecordID
t_rc REM_RecordFileHandle::ReadRecord(const REM_RecordID &rid,REM_RecordHandle &rh){
	//Check if the file is open
	if(!openFlag)
		return STORM_FILEALREADYCLOSED;

	STORM_PageHandle mph;

	t_rc rc;
	int currentPageId;;
	int slot;
	char *data;

	rc = rid.GetPageID(currentPageId);
	if(rc != OK)
		return rc;

	rc = rid.GetSlot(slot);
	if(rc != OK)
		return rc;


	rc = sfh.GetPage(currentPageId,mph);
	if(rc != OK)
		return rc;

	rc = mph.GetDataPtr(&data);
	if(rc != OK)
		return rc;


	rc = rh.SetData(data[slot*rfsh.recordSize],rid);
	if(rc != OK)
		return rc;

	rc = sfh.UnpinPage(currentPageId);
	if(rc != OK)
		return rc;

	return OK;
}

//Insert a record
t_rc REM_RecordFileHandle::InsertRecord(const char *pData,REM_RecordID &rid){
	//Check if the file is opened
	if(!openFlag)
		return STORM_FILEALREADYCLOSED;

	t_rc rc;
	STORM_PageHandle mph;
	char *data;
	int currentPageId;
	if(rfsh.pageID == 0){
		//Reserve a new Page
		rc = sfh.ReservePage(mph);
		if(rc != OK)
			return rc;

		//Get the data pointer
		rc = mph.GetDataPtr(&data);
		if(rc != OK)
			return rc;

		//Get the page id
		rc = mph.GetPageID(currentPageId);
		if(rc != OK)
			return rc;

		char checkAvailability = '@';
		for(int i=0;i<rfsh.recordsPerPage;i++)
			memcpy(&data[i*rfsh.recordSize],&checkAvailability,sizeof(char));

		rfsh.pageID=currentPageId;
	}else{
		//Get the page handler
		rc = sfh.GetPage(rfsh.pageID,mph);
		if(rc != OK)
			return rc;

		//get the data pointer
		rc = mph.GetDataPtr(&data);
		if(rc != OK)
			return rc;

		//get the page id
		rc = mph.GetPageID(currentPageId);
		if(rc != OK)
			return rc;

	}

	//Set the record page id and slot
	rid = REM_RecordID(rfsh.pageID,rfsh.slot);


        //data = data[rfsh.slot*rfsh.recordSize];
        
	memcpy(&data[rfsh.slot*rfsh.recordSize],&pData[0],rfsh.recordSize);

	//get the first available slot
	rc = GetAvailableSlot();
	if(rc != OK)
		return rc;

	//increase the number of records
	rfsh.nrecords++;

	//copy the file sub header
	memcpy(&ppData[0],&rfsh.nrecords,sizeof(int));
	memcpy(&ppData[12],&rfsh.pageID,sizeof(int));
	memcpy(&ppData[16],&rfsh.slot,sizeof(int));

	rc = sfh.MarkPageDirty(pageId);
	if(rc != OK)
		return rc;

	rc = sfh.MarkPageDirty(currentPageId);
	if(rc != OK)
		return rc;

	rc = sfh.UnpinPage(pageId);
	if(rc != OK)
		return rc;

	rc = sfh.UnpinPage(currentPageId);
	if(rc != OK)
		return rc;

	return OK;


}

//Delete the record with the exact RecordID
t_rc REM_RecordFileHandle::DeleteRecord(const REM_RecordID &rid){
	t_rc rc;
	int currentPageId;
	int slot;
	char *data;
	STORM_PageHandle mph;


	rc = rid.GetPageID(currentPageId);
	if(rc != OK)
		return rc;

	rc = rid.GetSlot(slot);
	if(rc != OK)
		return rc;


	rc = sfh.GetPage(currentPageId,mph);
	if(rc != OK)
		return rc;

	rc = mph.GetDataPtr(&data);
	if(rc != OK)
		return rc;

	//check if slot is empty
	if(data[slot*rfsh.recordSize]=='@')
		return REM_INVALIDRECORDID;

	char val = '@';
	memcpy(&data[slot*rfsh.recordSize],&val,rfsh.recordSize);
	rfsh.nrecords--;

	if(currentPageId<rfsh.pageID){
		rfsh.pageID=currentPageId;
		rfsh.slot=slot;
	}else if (currentPageId == rfsh.pageID){
		if(slot<rfsh.slot)
			rfsh.slot=slot;
	}

	//update file subheader
	memcpy(&ppData[0],&rfsh.nrecords,sizeof(int));
	memcpy(&ppData[12],&rfsh.pageID,sizeof(int));
	memcpy(&ppData[16],&rfsh.slot,sizeof(int));

	rc = sfh.MarkPageDirty(currentPageId);
	if(rc != OK)
		return rc;

	rc = sfh.MarkPageDirty(pageId);
	if(rc != OK)
		return rc;

	rc = sfh.UnpinPage(currentPageId);
	if(rc != OK)
		return rc;

	rc = sfh.UnpinPage(pageId);
	if(rc != OK)
		return rc;

	return OK;
}


//Update the record of the exact record handle
t_rc REM_RecordFileHandle::UpdateRecord(const REM_RecordHandle &rh){
	if(!openFlag)
		return STORM_FILEALREADYCLOSED;

	t_rc rc;
	int currentPageId;
	int slot;
	char *data;
	char *val;
	STORM_PageHandle mph;
	REM_RecordID rid;


	rc = rh.GetRecordID(rid);
	if(rc != OK)
		return rc;

	rc = rh.GetData(val);
	if(rc != OK)
		return rc;

	rc = rid.GetPageID(currentPageId);
	if(rc != OK)
		return rc;


	rc = rid.GetSlot(slot);
	if(rc != OK)
		return rc;


	rc = sfh.GetPage(currentPageId,mph);
	if(rc != OK)
		return rc;


	rc = mph.GetDataPtr(&data);
	if(rc != OK)
		return rc;


	memcpy(&data[slot*rfsh.recordSize],val,sizeof(val));


	rc = sfh.MarkPageDirty(currentPageId);
	if(rc != OK)
		return rc;

	rc = sfh.UnpinPage(currentPageId);
	if(rc != OK)
		return rc;

	return OK;
}


//Flush all the dirty pages
t_rc REM_RecordFileHandle::FlushPages()const{
	if(!openFlag)
		return STORM_FILEALREADYCLOSED;

	return sfh.FlushAllPages();
}


t_rc REM_RecordFileHandle::OpenFile(STORM_StorageManager &sm,const char *fname){
	this->sm=&sm;

	//Open the file
	t_rc rc;
	rc = this->sm->OpenFile(fname,sfh);
	if(rc != OK)
		return rc;


	//Get the first page where the file subheader is located
	rc = sfh.GetFirstPage(sph);
	if(rc != OK)
		return rc;

	//get tha pointer of the file subheader

	rc = sph.GetDataPtr(&ppData);
	if(rc != OK)
		return rc;

	//Get the page id of the file subheader
	rc = sph.GetPageID(pageId);
	if(rc != OK)
		return rc;

	//Get the file subheader to the memory
	memcpy(&rfsh.nrecords,&ppData[0],sizeof(int));
	memcpy(&rfsh.recordsPerPage,&ppData[4],sizeof(int));
	memcpy(&rfsh.recordSize,&ppData[8],sizeof(int));
	memcpy(&rfsh.pageID,&ppData[12],sizeof(int));
	memcpy(&rfsh.slot,&ppData[16],sizeof(int));


	openFlag = true;

	return OK;


}


t_rc REM_RecordFileHandle::CloseFile(){

	//Check if the file is already closed
	if(!openFlag)
		return STORM_FILEALREADYCLOSED;

	//Close the file
	t_rc rc;
	rc = sm->CloseFile(sfh);
	if(rc != OK)
		return rc;


	//Set the flag false if everything goes fine
	openFlag= false;


	return OK;
}



//Get the next available slot
t_rc REM_RecordFileHandle::GetAvailableSlot(){
	char *data;
	char val;
	t_rc rc;

	STORM_PageHandle mph;

	//Scan for the first emtpy slot
	for(int i=2;i<=sfh.GetNumReservedPages();i++){
		rc = sfh.GetPage(i,mph);
		if(rc != OK)
			return rc;

		rc = mph.GetDataPtr(&data);
		if(rc != OK)
			return rc;

		memcpy(&val,data,sizeof(char));

		//Check every page
		for(int j=0;j<rfsh.recordsPerPage;j++){
			if(data[j*rfsh.recordSize]=='@'){
				rfsh.pageID=i;
				rfsh.slot=j;
				return OK;
			}
		}
	}

	rfsh.pageID=0;
	rfsh.slot=0;
	return OK;
}
























