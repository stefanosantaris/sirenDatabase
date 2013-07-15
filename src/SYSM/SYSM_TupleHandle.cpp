/*
 * SYSM_TupleHandle.cpp
 *
 *      Author: santaris
 */

#include "SYSM_TupleHandle.h"
#include <stdio.h>
using namespace std;

SYSM_TupleHandle::SYSM_TupleHandle() {
	pData = NULL;
}

SYSM_TupleHandle::~SYSM_TupleHandle() {
	// TODO Auto-generated destructor stub
}


//t_relInfo constructor
SYSM_TupleHandle::SYSM_TupleHandle(t_relInfo relInfo) {

	pData = (char *) malloc (MAXNAME+2*INT_SIZE);

	memset (pData, 0 ,MAXNAME+2*INT_SIZE);

	memcpy(&pData[0],relInfo.relName, MAXNAME);

	memcpy(&pData[MAXNAME],&relInfo.recLength,INT_SIZE);

	memcpy(&pData[MAXNAME+INT_SIZE], &relInfo.attrCount,INT_SIZE);

	memcpy(&pData[MAXNAME+2*INT_SIZE],&relInfo.indexCount,INT_SIZE);

	this->relInfo = relInfo;
}

//t_attrInfo constructor
SYSM_TupleHandle::SYSM_TupleHandle(t_attrInfo attrInfo) {
	pData = (char *) malloc (2*MAXNAME+4*INT_SIZE);

	memset (pData, 0, 2*MAXNAME+4*INT_SIZE );

	memcpy(&pData[0], attrInfo.relName,MAXNAME);

	memcpy(&pData[MAXNAME], attrInfo.attrName, MAXNAME);

	memcpy(&pData[2*MAXNAME],& attrInfo.offset, INT_SIZE);

	memcpy(&pData[2*MAXNAME+INT_SIZE], &attrInfo.attrType, INT_SIZE);

	memcpy(&pData[2*MAXNAME+2*INT_SIZE], &attrInfo.attrLength, INT_SIZE);

	memcpy(&pData[2*MAXNAME+3*INT_SIZE], &attrInfo.indexNo, INT_SIZE);

	this->attrInfo = attrInfo;

}

//return the created tupne
t_rc SYSM_TupleHandle::GetTupleData(char *&pData) {
	pData = this->pData;

	return OK;
}

//get the relation info
t_rc SYSM_TupleHandle::GetRelInfo(char *pData, t_relInfo &relInfo) {
	memcpy(relInfo.relName, &pData[0], MAXNAME);

	memcpy(&relInfo.recLength,&pData[MAXNAME],INT_SIZE);

	memcpy( &relInfo.attrCount,&pData[MAXNAME+INT_SIZE],INT_SIZE);

	memcpy(&relInfo.indexCount,&pData[MAXNAME+2*INT_SIZE],INT_SIZE);

	return OK;

}

//get the attribute info
t_rc SYSM_TupleHandle::GetAttrInfo(char *pData, t_attrInfo &attrInfo) {

	memcpy(attrInfo.relName, &pData[0], MAXNAME);

	memcpy(attrInfo.attrName, &pData[MAXNAME], MAXNAME);

	memcpy(&attrInfo.offset, &pData[2 * MAXNAME], INT_SIZE);

	memcpy(&attrInfo.attrType, &pData[2 * MAXNAME + INT_SIZE], INT_SIZE);

	memcpy(&attrInfo.attrLength, &pData[2*MAXNAME+2*INT_SIZE], INT_SIZE);

	memcpy(&attrInfo.indexNo, &pData[2*MAXNAME+3*INT_SIZE], INT_SIZE);

	return OK;
}
