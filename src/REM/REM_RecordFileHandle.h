/*
 * REM_RecordFileHandle.h
 *
 *      Author: santaris
 */

#ifndef REM_RECORDFILEHANDLE_H_
#define REM_RECORDFILEHANDLE_H_

#include "../STORM/STORM.h"
#include "REM_RecordHandle.h"
#include "REM_RecordID.h"

class REM_RecordFileHandle {
public:
	REM_RecordFileHandle();
	~REM_RecordFileHandle();

	t_rc ReadRecord(const REM_RecordID &rid,REM_RecordHandle &rh);
	t_rc InsertRecord (const char *pData,REM_RecordID &rid);
	t_rc DeleteRecord (const REM_RecordID &rid);
	t_rc UpdateRecord (const REM_RecordHandle &rh);
	t_rc FlushPages () const;

private:
	t_rc OpenFile(STORM_StorageManager &sm,const char *fname);
	t_rc CloseFile();
	t_rc GetAvailableSlot();




	typedef struct{
		int nrecords;		//Number of records
		int recordsPerPage;	//Records per Page
		int recordSize;		//Record Size
		int pageID;			//stores the first empty page
		int slot;			//stores the first empty slot
	}REM_FileSubHeader;

	REM_FileSubHeader rfsh;


	STORM_StorageManager *sm;
	STORM_FileHandle sfh;

	//file subheader attributes
	STORM_PageHandle sph;
	char *ppData;
	int pageId;

	bool openFlag;

	friend class REM_RecordFileManager;
	friend class REM_RecordFileScan;
	friend class SSQLM_Manager;
};

#endif /* REM_RECORDFILEHANDLE_H_ */
