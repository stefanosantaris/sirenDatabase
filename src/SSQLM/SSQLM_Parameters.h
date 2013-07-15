/*
 * SSQLM_Parameters.h
 *
 *      Author: santaris
 */

#ifndef SSQLM_PARAMETERS_H_
#define SSQLM_PARAMETERS_H_

typedef struct {
	t_attrType attrType;
	void *value;
} t_attrValues;

typedef enum {
	CREATEDATABASE_CMD,
	DROPDATABASE_CMD,
	OPENDATABASE_CMD,
	CLOSEDATABASE_CMD,
	CREATETABLE_CMD,
	DROPTABLE_CMD,
	CREATEINDEX_CMD,
	DROPINDEX_CMD,
	SELECT_CMD,
	INSERT_CMD,
	DELETE_CMD,
	UPDATE_CMD
} t_cmd;

#endif /* SSQLM_PARAMETERS_H_ */
