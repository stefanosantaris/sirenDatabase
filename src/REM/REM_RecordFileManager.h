/*
 * REM_RecordFileManager.h
 *
 *      Author: santaris
 */

#ifndef REM_RECORDFILEMANAGER_H_
#define REM_RECORDFILEMANAGER_H_

#include "../STORM/STORM.h"
#include "REM_RecordFileHandle.h"


class REM_RecordFileManager {
public:
	REM_RecordFileManager(STORM_StorageManager &smManager);
	~REM_RecordFileManager();

	t_rc CreateRecordFile(const char *fname,int rs);
	t_rc DestroyRecordFile(const char *fname);
	t_rc OpenRecordFile(const char *fname,REM_RecordFileHandle &rfh);
	t_rc CloseRecordFile(REM_RecordFileHandle &rfh);
private:
	STORM_StorageManager &sm;
};

#endif /* REM_RECORDFILEMANAGER_H_ */
