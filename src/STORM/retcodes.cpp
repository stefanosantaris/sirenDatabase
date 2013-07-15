#include "retcodes.h"
#include <cstdio>

void DisplayReturnCode(t_rc rc)
{
	char *msg;

	switch(rc)
	{
	case OK: msg = "OK"; break;
	case STORM_FILENOTFOUND: msg = "Specified file is not found."; break;
	case STORM_FILEEXISTS: msg = "File already exists."; break;
	case STORM_FILEDOESNOTEXIST: msg="File does not exist."; break;
	case STORM_FILEALREADYOPENED: msg = "File already opened."; break;
	case STORM_EOF: msg = "End Of File (EOF) has been reached."; break;
	case STORM_FILEFULL: msg = "File has reached its maximum capacity."; break;
	case STORM_FILEOPENERROR: msg = "File open error."; break;
	case STORM_FILECLOSEERROR: msg = "File close error."; break;
	case STORM_FILENOTOPENED: msg = "File is not opened."; break;
	case STORM_OPENEDFILELIMITREACHED: msg = "Limit of opened files reached."; break;
	case STORM_INVALIDFILEHANDLE: msg = "Invalid File Handle."; break;
	case STORM_INVALIDPAGE: msg = "Page is not valid."; break;
	case STORM_CANNOTCREATEFILE: msg = "Cannot create file."; break;
	case STORM_CANNOTDESTROYFILE: msg = "Cannot destroy file."; break;
	case STORM_PAGENOTINBUFFER: msg = "Page is not in buffer."; break;
	case STORM_PAGEISPINNED: msg = "Page is pinned."; break;
	case STORM_ALLPAGESPINNED: msg = "All pages are pinned."; break;
	case STORM_IOERROR: msg = "Input/Output error."; break;
	case STORM_MEMALLOCERROR: msg = "Memory allocation error."; break;
	
	case REM_INVALIDRECORDSIZE: msg = "Invalid record size."; break;
	case REM_INVALIDRECORDID: msg = "Invalid record id."; break;
	case REM_PAGESCANOUTOFBOUNDS: msg = "No other pages to scan."; break;
	case REM_RECORDNOTFOUND: msg = "No record found."; break;



	case SYSM_CANNOTCREATEDATABASE: msg = "Cannot create database."; break;
	case SYSM_CANNOTACCESSDATABASE: msg = "Cannot access database."; break;
	case SYSM_CANNOTDELETEDATABASE: msg = "Cannot delete database."; break;
	case SYSM_DATABASEALREADYOPENED: msg = "Database already opened."; break;
	case SYSM_DATABASECLOSED: msg = "Database is closed."; break;
	case SYSM_METADATANAME: msg = "Metadata Name."; break;
	case SYSM_TABLEALREADYEXISTS: msg = "Table already exists."; break;
	case SYSM_TABLEDOESNOTEXIST: msg = "Table does not exist."; break;
	case SYSM_NOATTRIBUTEFOUND: msg = "No attribute found."; break;
	case SYSM_ATTRIBUTEALREADYINDEXED: msg = "Attribute already indexed."; break;
	case SYSM_ATTRIBUTENOTINDEXED: msg = "Attribute is not indexed."; break;



	case SSQLM_INVALIDCOMMAND: msg = "Invalid command."; break;
	case SSQLM_INVALIDTABLENAME: msg = "Invalid table name."; break;
	case SSQLM_INVALIDARGUMENTNAME: msg = "Invalid argument name."; break;
	case SSQLM_STRINGTOOLONG: msg = "String too long"; break;
	case SSQLM_MAXATTRIBUTES: msg = "A lot of attributes"; break;
	case SSQLM_NOENTRYTOINDEX: msg = "No entry to index."; break;
	case SSQLM_NOVALUESTOINSERT: msg = "No values inserted."; break;



	case UIM_WRONGCOMMAND: msg = "Wrong command."; break;
	case UIM_NODATABASEOPEN: msg = "There is no database open."; break;
	case UIM_WRONGDATABASENAME: msg = "Wrong database name."; break;
	case UIM_ATTRIBUTENOTINDEXED: msg = "Attribute not indexed."; break;
	case UIM_TOOMUCHTABLES: msg = "Too much tables selected."; break;

	default: msg = "Unknown return code."; break;

	}

	fprintf (stderr, "ERROR: %s\n", msg);
}

void DisplayMessage(char *msg)
{
	fprintf (stderr, "ERROR: %s\n", msg); 
}
