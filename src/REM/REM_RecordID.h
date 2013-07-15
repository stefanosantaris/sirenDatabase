/*
 * REM_RecordID.h
 *
 *      Author: santaris
 */

#ifndef REM_RECORDID_H_
#define REM_RECORDID_H_

#include "../STORM/STORM.h"

class REM_RecordID {
public:
	REM_RecordID();
	REM_RecordID(int pageID,int slot);
	~REM_RecordID();

	t_rc GetPageID(int &pageID) const;
	t_rc GetSlot(int &slot) const;
	t_rc SetPageID(int pageID);
	t_rc SetSlot(int slot);

private:
	int pageId;
	int slot;
};

#endif /* REM_RECORDID_H_ */
