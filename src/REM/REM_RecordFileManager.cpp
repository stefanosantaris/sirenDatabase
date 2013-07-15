/*
 * REM_RecordFileManager.cpp
 *
 *      Author: santaris
 */

#include "REM_RecordFileManager.h"
#include <stdio.h>

REM_RecordFileManager::REM_RecordFileManager(STORM_StorageManager &smManager):sm(smManager) {
	// TODO Auto-generated constructor stub

}

REM_RecordFileManager::~REM_RecordFileManager() {
}

t_rc REM_RecordFileManager::CreateRecordFile(const char *fname,int rs){

	//Calculate the records per page
	int recordsPerPage = (int)(PAGE_DATA_SIZE/rs);

	//check if the record size is a valid one
	if(recordsPerPage<1)
		return REM_INVALIDRECORDSIZE;

	t_rc rc;

	//Create the file
	rc = sm.CreateFile(fname);
	if(rc != OK)
		return rc;


	//Open the file
	STORM_FileHandle sfh;
	rc = sm.OpenFile(fname,sfh);
	if(rc != OK)
		return rc;

	//Reserve a new Page
	STORM_PageHandle sph;
	rc = sfh.ReservePage(sph);
	if(rc != OK)
		return rc;


	char *ppData;
	int pageId;

	//Get the pointer of the data
	rc = sph.GetDataPtr(&ppData);
	if(rc != OK)
		return rc;

	//Get the pageId
	rc = sph.GetPageID(pageId);
	if(rc != OK)
		return rc;


	//Create the file subheader
	REM_RecordFileHandle::REM_FileSubHeader rfsh = {0,recordsPerPage,rs,0,0};


	//copy the file sub header into the page
	memcpy(&ppData[0],&rfsh.nrecords,sizeof(int));
	memcpy(&ppData[4],&rfsh.recordsPerPage,sizeof(int));
	memcpy(&ppData[8],&rfsh.recordSize,sizeof(int));
	memcpy(&ppData[12],&rfsh.pageID,sizeof(int));
	memcpy(&ppData[16],&rfsh.slot,sizeof(int));



	//Mark the page as dirty
	rc = sfh.MarkPageDirty(pageId);
	if(rc != OK)
		return rc;

	//Unpin the page
	rc = sfh.UnpinPage(pageId);
	if(rc != OK)
		return rc;

	//Close the file
	rc = sm.CloseFile(sfh);
	if(rc != OK)
		return rc;

	//Return OK if everything goes fine
	return OK;

}

t_rc REM_RecordFileManager::DestroyRecordFile(const char *fname){
	return sm.DestroyFile(fname);
}

t_rc REM_RecordFileManager::OpenRecordFile(const char *fname,REM_RecordFileHandle &rfh){
	return rfh.OpenFile(sm,fname);
}

t_rc REM_RecordFileManager::CloseRecordFile(REM_RecordFileHandle &rfh){
	return rfh.CloseFile();
}
