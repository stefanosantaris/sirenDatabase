/*
 * REM_RecordFileScan.cpp
 *
 *      Author: santaris
 */

#include "REM_RecordFileScan.h"

REM_RecordFileScan::REM_RecordFileScan() {
	openFlag = false;
	count = 0;
	pageId = 2;
	slot = 0;

}

REM_RecordFileScan::~REM_RecordFileScan() {
	// TODO Auto-generated destructor stub
}

t_rc REM_RecordFileScan::OpenRecordScan(REM_RecordFileHandle &rh,t_attrType attrType,int attrLength,int attrOffset,t_compOp compOp,void *value){
	if(openFlag)
		return STORM_FILEALREADYOPENED;

	this->attrType = attrType;
	this->attrLength = attrLength;
	this->attrOffset = attrOffset;
	this->compOp = compOp;
	this->rfh = &rh;

	if(compOp != NO_OP)
		memcpy(&this->value,&value,sizeof(value));

	openFlag = true;
	return OK;
}


t_rc REM_RecordFileScan::GetNextRecord(REM_RecordHandle &rh){
	if(!openFlag)
		return STORM_FILEALREADYCLOSED;

	t_rc rc;

	REM_RecordID rid;
	//int reservedPages = rfh->sfh.GetNumReservedPages();

	if(pageId>rfh->sfh.GetNumReservedPages())
		return REM_PAGESCANOUTOFBOUNDS;


	for( ;pageId<=rfh->sfh.GetNumReservedPages();pageId++){
		rc = rid.SetPageID(pageId);
		if(rc != OK)
			return rc;

		if(slot>rfh->rfsh.recordsPerPage)
			slot=0;

		for( ;slot<=rfh->rfsh.recordsPerPage;slot++){
			rc = rid.SetSlot(slot);
			if(rc != OK)
				return rc;

			rc = rfh->ReadRecord(rid,rh);
			if(rc != OK)
				return rc;

			rc = rh.GetData(pData);
			if(rc != OK)
				return rc;

			rc = CheckRecord();
			if(rc == OK){
				slot++;
				return OK;
			}
		}
	}

	return REM_RECORDNOTFOUND;
}

t_rc REM_RecordFileScan::CloseRecordScan(){
	if(!openFlag)
		return STORM_FILEALREADYCLOSED;

	openFlag = false;
	return OK;

}


t_rc REM_RecordFileScan::CheckRecord(){

	char *val;
	val = (char *) malloc (attrLength);
	memset (val, 0, attrLength);
	memcpy(val,&pData[attrOffset],attrLength);

	int intConvertion;
	int intComparison;
	float floatConvertion,floatComparison;
	double doubleConvertion,doubleComparison;
	char *charConvertion,*charComparison;
	//Check attribute Type
	if(attrType == TYPE_INT){
		memcpy(&intConvertion, &pData[attrOffset], attrLength);

		intComparison = *(int *) value;
	}else if(attrType == TYPE_FLOAT){
		floatConvertion = atof(val);

		floatComparison = *(float *) value;
	}
		else if(attrType == TYPE_DOUBLE){

		doubleConvertion = strtod(val, NULL);

		doubleComparison = *(double *) value;
	}
	else if(attrType == TYPE_STRING){
		charConvertion = (char *) malloc (MAXNAME);

		memset(charConvertion, 0, MAXNAME);

		memcpy(charConvertion, &val[attrOffset],MAXNAME);

		charComparison = (char*) value;
	}
	
	//Check Comparison operator
	if(compOp == EQ_OP){

		if(attrType == TYPE_INT){

			if(intConvertion == intComparison)
				return OK;
		}else if(attrType == TYPE_DOUBLE){

			if(doubleConvertion == doubleComparison)
				return OK;

			}else if(attrType == TYPE_FLOAT){

				if(floatConvertion == floatComparison)
					return OK;

			}
			else if(attrType == TYPE_STRING){

				if(strncmp(charConvertion,charComparison,attrLength)==0)
					return OK;
			}
	}
	else if(compOp == LT_OP){
		if(attrType == TYPE_INT){
			if(intConvertion < intComparison)
				return OK;
		}else if(attrType == TYPE_DOUBLE){
			if(doubleConvertion < doubleComparison)
				return OK;
		}else if(attrType == TYPE_FLOAT){
			if(floatConvertion < floatComparison)
				return OK;
		}
		else if(attrType == TYPE_STRING){
			if(strncmp(charConvertion,charComparison,attrLength)<0)
				return OK;
		}
	}
	else if(compOp == GT_OP){

		if(attrType == TYPE_INT){

			if(intConvertion > intComparison)
				return OK;

		}else if(attrType == TYPE_DOUBLE){

			if(doubleConvertion > doubleComparison)
				return OK;

		}else if(attrType == TYPE_FLOAT){

			if(floatConvertion > floatComparison)
				return OK;
		}
		else if(attrType == TYPE_STRING){
			if(strncmp(charConvertion,charComparison,attrLength)>0)
				return OK;
		}
	}
	else if(compOp == NE_OP){
		if(attrType == TYPE_INT){
			if(intConvertion != intComparison)
				return OK;
		}else if(attrType == TYPE_DOUBLE){
			if(doubleConvertion != doubleComparison)
				return OK;
		}else if(attrType == TYPE_FLOAT){
			if(floatConvertion != floatComparison)
				return OK;
		}
		else if(attrType == TYPE_STRING){
			if(strncmp(charConvertion,charComparison,attrLength)!=0)
				return OK;
		}
	}
	else if(compOp == LE_OP){
		if(attrType == TYPE_INT){
			if(intConvertion <= intComparison)
				return OK;
		}else if(attrType == TYPE_DOUBLE){
			if(doubleConvertion <= doubleComparison)
				return OK;
		}else if(attrType == TYPE_FLOAT){
			if(floatConvertion <= floatComparison)
				return OK;
		}
		else if(attrType == TYPE_STRING){
			if(strncmp(charConvertion,charComparison,attrLength)<=0)
				return OK;
		}
	}
	else if(compOp == GE_OP){
		if(attrType == TYPE_INT){
			if(intConvertion >= intComparison)
				return OK;
		}else if(attrType == TYPE_DOUBLE){
			if(doubleConvertion >= doubleComparison)
				return OK;
		}else if(attrType == TYPE_FLOAT){
			if(floatConvertion >= floatComparison)
				return OK;
		}
		else if(attrType == TYPE_STRING){
			if(strncmp(charConvertion,charComparison,attrLength)>=0)
				return OK;
		}
	}
	else if(compOp == NO_OP){
		return OK;
	}
}





















