/* 
 * File:   REM_Parameters.h
 * Author: santaris
 *
 */

#ifndef _REM_OPERANTSANDSTRUCTS_H
#define	_REM_OPERANTSANDSTRUCTS_H

#define MAXSTRINGLENGTH 255

#define MAXATTRIBUTES 30

#define MAXNAME 24

typedef enum{
            EQ_OP,      //equal
            LT_OP,      //less than
            GT_OP,      //greater than
            NE_OP,      //not equal
            LE_OP,      //less than or equal
            GE_OP,      //greater than or equal
            NO_OP       //No comparison.Should be used when file scan value is NULL
}t_compOp;


typedef enum{
            TYPE_INT,   //for integers
            TYPE_FLOAT, //for float numbers
            TYPE_DOUBLE,//for double numbers
            TYPE_STRING //character string up to 255 chars

}t_attrType;

typedef struct {
	char relName[MAXNAME + 1];	//Relation Name
	int recLength;				//Record Length
	int attrCount;				//Number of attributes
	int indexCount;				//Number of indexes
}t_relInfo;

typedef struct {
	char relName[MAXNAME + 1];	//Relation Name
	char attrName[MAXNAME + 1];	//Attribute Name
	int offset;					//Byte offset of the Attribute
	t_attrType attrType;		//Attribute type
	int attrLength;				//Attribute length
	int indexNo;				//Index Code
}t_attrInfo;




#endif	/* _REM_OPERANTSANDSTRUCTS_H */

