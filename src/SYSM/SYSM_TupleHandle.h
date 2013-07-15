/*
 * SYSM_TupleHandle.h
 *
 *      Author: santaris
 */

#ifndef SYSM_TUPLEHANDLE_H_
#define SYSM_TUPLEHANDLE_H_
#include "../REM/REM.h"

class SYSM_TupleHandle {
private:
	char *pData;

	t_relInfo relInfo;

	t_attrInfo attrInfo;
public:
	SYSM_TupleHandle();
	~SYSM_TupleHandle();

	//Relation Catalogue Constructor
	SYSM_TupleHandle(t_relInfo relInfo);

	//Attribute Catalogue constructor
	SYSM_TupleHandle(t_attrInfo attrInfo);

	t_rc GetTupleData(char *&pData);

	t_rc GetRelInfo(char *pData,t_relInfo &relInfo);

	t_rc GetAttrInfo(char *pData, t_attrInfo &attrInfo);
};

#endif /* SYSM_TUPLEHANDLE_H_ */
