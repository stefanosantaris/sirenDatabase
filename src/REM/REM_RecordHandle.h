/*
 * REM_RecordHandle.h
 *
 *      Author: santaris
 */

#ifndef REM_RECORDHANDLE_H_
#define REM_RECORDHANDLE_H_

#include "../STORM/STORM.h"
#include "REM_RecordID.h"

class REM_RecordHandle {
public:
	REM_RecordHandle();
	~REM_RecordHandle();

	t_rc GetData(char *&pData) const;
	t_rc GetRecordID(REM_RecordID &rid) const;

private:
	t_rc SetData(char &pData,const REM_RecordID &rid);

	char *ppData;
	REM_RecordID rid;
	friend class REM_RecordFileHandle;
};

#endif /* REM_RECORDHANDLE_H_ */
