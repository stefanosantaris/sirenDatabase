/*
 * REM_RecordFileScan.h
 *
 *      Author: santaris
 */

#ifndef REM_RECORDFILESCAN_H_
#define REM_RECORDFILESCAN_H_

#include "../STORM/STORM.h"
#include "REM_RecordFileHandle.h"
#include "REM_RecordHandle.h"
#include "REM_Parameters.h"

class REM_RecordFileScan {
public:
	REM_RecordFileScan();
	~REM_RecordFileScan();


	t_rc OpenRecordScan(REM_RecordFileHandle &rfh,
			t_attrType attrType,
			int attrLength,
			int attrOffset,
			t_compOp compOp,
			void *value);

	t_rc GetNextRecord(REM_RecordHandle &rh);
	t_rc CloseRecordScan();

private:

	bool openFlag;

	//search variables
	t_attrType attrType;
	int attrLength;
	int attrOffset;
	t_compOp compOp;
	void *value;

	char *pData;
	int count;

	int pageId;
	int slot;

	t_rc CheckRecord();


	REM_RecordFileHandle *rfh;
};

#endif /* REM_RECORDFILESCAN_H_ */
