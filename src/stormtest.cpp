#include "STORM/STORM.h"
#include "REM/REM.h"
#include "SYSM/SYSM.h"
#include "SSQLM/SSQLM.h"
#include "UIM/UIM.h"
#include <stdio.h>
#include <iostream>


int main()
{




	STORM_StorageManager mgr;
	REM_RecordFileManager rfm(mgr);
	SYSM_Manager sm(rfm);
	REM_RecordID rid;
	REM_RecordFileHandle rfh;
	SSQLM_Manager sqlm(rfm);
	UIM_Manager uim(sm,sqlm);


	string cmd;
	t_rc rc;

	cout<<"sirenbase>";
	getline(cin, cmd);
	while(cmd != "exit"){
		uim.SetCommand(cmd);


	//INXM_IndexManager inxm(mgr);


		rc = uim.ExecuteCommand();
		if(rc != OK)
			DisplayReturnCode(rc);
		cout<<"sirenbase>";
		getline(cin,cmd);
	}

	/*rc = sm.CreateDb("testDB");
	if(rc != OK){DisplayReturnCode(rc); Pause(); exit(-1);}


	rc = sm.OpenDb("testDB");
	if(rc != OK){DisplayReturnCode(rc); Pause(); exit(-1);}

	t_relInfo relInfo = {"stefanos", 2*MAXNAME, 2, 0};
	t_attrInfo attrInfo[2] = {
			{"stefanos", "testAttribute", 0, TYPE_STRING, MAXNAME, -1},
			{"stefanos", "test2Attribute", MAXNAME, TYPE_STRING, MAXNAME, -1}
	};

	rc = sm.CreateTable(relInfo, attrInfo);
	if(rc != OK){DisplayReturnCode(rc); Pause(); exit(-1);}
	int indexNo;
	rc = sm.GetNextIndexNo(indexNo);
	if(rc != OK){DisplayReturnCode(rc); Pause(); exit(-1);}

	rc = sm.CreateIndex("stefanos","testAttribute",indexNo);
	if(rc != OK){DisplayReturnCode(rc); Pause(); exit(-1);}

	rc = sm.GetNextIndexNo(indexNo);
	if(rc != OK){DisplayReturnCode(rc); Pause(); exit(-1);}

	rc = sm.CreateIndex("stefanos","test2Attribute",indexNo);
	if(rc != OK){DisplayReturnCode(rc); Pause(); exit(-1);}

	rc = sm.DropIndex("stefanos", "testAttribute");
	if(rc != OK){DisplayReturnCode(rc); Pause(); exit(-1);}

	rc = sm.CloseDb("testDB");
	if(rc != OK){DisplayReturnCode(rc); Pause(); exit(-1);}
*/

	rc = rfm.OpenRecordFile("STEFANOS/rel.met",rfh);
	if(rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);};



	rc = rid.SetPageID(2);
	if(rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);};


	rc = rid.SetSlot(2);
	if(rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);};

	REM_RecordHandle rh;

	char *data;

	rc = rfh.ReadRecord(rid,rh);
	if(rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);};

	rc = rh.GetData(data);
	if(rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);};

	t_relInfo relInfo1;


		memcpy(&relInfo1.relName, &data[0], MAXNAME);

		memcpy(&relInfo1.recLength,&data[MAXNAME],INT_SIZE);

		memcpy( &relInfo1.attrCount,&data[MAXNAME+INT_SIZE],INT_SIZE);

		memcpy(&relInfo1.indexCount,&data[MAXNAME+2*INT_SIZE],INT_SIZE);

		printf("RELNAME : %s\n",&relInfo1.relName);
		printf("RECLength : %d\n",relInfo1.recLength);
		printf("AttrCount : %d\n",relInfo1.attrCount);
		printf("indexCount : %d\n",relInfo1.indexCount);



	rc = rfm.CloseRecordFile(rfh);
	if(rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);};



	rc = rfm.OpenRecordFile("STEFANOS/attr.met",rfh);
	if(rc != OK)
		return rc;

	vector<t_attrInfo>  attrInfo2;
	rc = sm.OpenDb("STEFANOS");
	if(rc != OK){DisplayReturnCode(rc); Pause(); exit(-1);};

	rc = sm.GetDataAttributes("TEST",attrInfo2);
	if(rc != OK){DisplayReturnCode(rc); Pause(); exit(-1);};

	rc = sm.CloseDb("STEFANOS");
	if(rc != OK){DisplayReturnCode(rc); Pause(); exit(-1);};

	for(int i=0; i < 2; i++){
		printf("attrInfo2 RELNAME : %s\n",&attrInfo2[i].relName);
		printf("attrInfo2 ATTRNAME : %s\n",&attrInfo2[i].attrName);
		printf("attrInfo2 attrOffset : %d\n",attrInfo2[i].offset);
		printf("attrInfo2 attrType : %d\n",attrInfo2[i].attrType);
		printf("attrInfo2 attrLength : %d\n",attrInfo2[i].attrLength);
		printf("attrInfo2 indexNo : %d\n", attrInfo2[i].indexNo);

	}
	rc = rid.SetPageID(2);
		if(rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);};


		rc = rid.SetSlot(10);
		if(rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);};



		rc = rfh.ReadRecord(rid,rh);
		if(rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);};

		rc = rh.GetData(data);
		if(rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);};

		t_attrInfo attrInfo1;

		memcpy(attrInfo1.relName, &data[0], MAXNAME);
		memcpy(attrInfo1.attrName, &data[MAXNAME], MAXNAME);
		memcpy(&attrInfo1.offset, &data[2 * MAXNAME], INT_SIZE);
		memcpy(&attrInfo1.attrType, &data[2 * MAXNAME + INT_SIZE], INT_SIZE);
		memcpy(&attrInfo1.attrLength, &data[2 * MAXNAME + 2 * INT_SIZE], INT_SIZE);
		memcpy(&attrInfo1.indexNo, &data[2 * MAXNAME + 3 * INT_SIZE], INT_SIZE);


		printf("attrInfo RELNAME : %s\n",attrInfo1.relName);
		printf("attrInfo ATTRNAME : %s\n",attrInfo1.attrName);
		printf("attrInfo attrOffset : %d\n",attrInfo1.offset);
		printf("attrInfo attrType : %d\n",attrInfo1.attrType);
		printf("attrInfo attrLength : %d\n",attrInfo1.attrLength);
		printf("attrInfo indexNo : %d\n", attrInfo1.indexNo);

		rc = rfm.CloseRecordFile(rfh);
		if(rc != OK){DisplayReturnCode(rc); Pause(); exit(-1);};


		rc = rfm.OpenRecordFile("STEFANOS/TEST", rfh);
		if(rc != OK){DisplayReturnCode(rc); Pause(); exit(-1);};

		rc = rid.SetPageID(2);
		if(rc != OK){DisplayReturnCode(rc); Pause(); exit(-1);};

		rc = rid.SetSlot(0);
		if(rc != OK){DisplayReturnCode(rc); Pause(); exit(-1);};

		rc = rfh.ReadRecord(rid, rh);
		if(rc != OK){DisplayReturnCode(rc); Pause(); exit(-1);};

		rc = rh.GetData(data);
		if(rc != OK){DisplayReturnCode(rc); Pause(); exit(-1);};

		printf("Data :%s\n",&data[4]);


		rc = rfm.CloseRecordFile(rfh);
		if(rc != OK){DisplayReturnCode(rc); Pause(); exit(-1);};
		Pause();




	//rc = rfm.CreateRecordFile("test.dat",23);
	//if(rc != OK) {DisplayReturnCode(rc); Pause(); rfm.DestroyRecordFile("test.dat"); exit(-1);}

	//REM_RecordFileHandle rfh;
	//rc = rfm.OpenRecordFile("test.dat",rfh);
	//if(rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);};

	/*SAVVAS --create the index file and open the index file
	rc = inxm.CreateIndex("index.dat",1,TYPE_STRING,sizeof(string));
	if(rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);};

	INXM_IndexHandle ih;
	rc = inxm.OpenIndex("index.dat",1,ih);
	if(rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);};
*/
	//REM_RecordID rid;
	//rc = rfh.InsertRecord("hello world",rid);
	//if(rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);};





	/*SAVVAS-insert an entry
	rc = ih.InsertEntry((void*)"hello world",rid);
	if(rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);};
        */

    //REM_RecordHandle rh;
	//rc = rfh.ReadRecord(rid,rh);
	//if(rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);};

	//char *data;
	//rc = rh.GetData(data);
	//if(rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);};

	//printf("%s\n",&data[0]);



    //rc = rfh.InsertRecord("Twra douleuei",rid);
    //if(rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);};

    /*SAVAS --insert a new entry
    rc = ih.InsertRecord((void*)"Twra douleuei",rid);
	if(rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);};
*/


    //rc = rid.SetPageID(2);
	//if(rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);};


	//rc = rid.SetSlot(0);
	//if(rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);};



	//rc = rfh.ReadRecord(rid,rh);
	//if(rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);};

	//rc = rh.GetData(data);
	//if(rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);};

	//printf("%s\n",&data[0]);




	/*Insert the other inxm functions on the same way...i have inserted two records into the database.There is only one attribute which is TYPE_STRING.attrOffset = 0*/





	//rc = rfm.CloseRecordFile(rfh);
	//if(rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);};
	/*STORM_StorageManager mgr;
	STORM_FileHandle fh;
	t_rc rc;
	int nAllocPages, nResPages;
	STORM_PageHandle ph;
	char *data;
	int val;
	int pid;
	int nreqs, nreads, nwrites, npinned, nframes;
		
	// ====================== STEP 1 =======================================//
	// Create the file. 
	// Open the file.
	// Reserve 100 pages.
	// Store something in each page.
	// Close the file.
	//======================================================================//
	
	rc = mgr.CreateFile("test.dat");
	if (rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);}

	rc = mgr.OpenFile("test.dat", fh);
	if (rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);}

	nAllocPages = fh.GetNumAllocatedPages();
	nResPages = fh.GetNumReservedPages();
	
	for (int i=1; i<=100; i++)
	{
		rc = fh.ReservePage(ph);
		if (rc != OK) {DisplayReturnCode(rc); Pause(); exit(-1);}

		// Copy something to the page.
		rc = ph.GetDataPtr(&data);
		memcpy(data, &i, sizeof(int));  

		// Mark the page as dirty.
		rc = ph.GetPageID(pid);
		if (rc != OK) {DisplayReturnCode(rc); exit(-1);}

		rc = fh.MarkPageDirty(pid);
		if (rc != OK) {DisplayReturnCode(rc); exit(-1);}

		// Unpin the page
		rc = fh.UnpinPage(pid);
		if (rc != OK) {DisplayReturnCode(rc); exit(-1);}
	}

	mgr.GetStats(nreqs, nreads, nwrites, npinned, nframes);
	printf("reqs: %d, reads: %d, writes: %d, pinned: %d, frames: %d\n", nreqs, nreads, nwrites, npinned, nframes);
	printf("allocated pages: %d, reserved pages: %d\n", fh.GetNumAllocatedPages(), fh.GetNumReservedPages());

	rc = mgr.CloseFile(fh);
	if (rc != OK) {DisplayReturnCode(rc); exit(-1);}

	Pause();

	// ====================== STEP 2 =======================================//
	// Open the file again. 
	// Read every page of the file and print the page contents.
	// Release the first 50 pages.
	// Close the file.
	//======================================================================//

	rc = mgr.OpenFile("test.dat", fh);
	if (rc != OK){DisplayReturnCode(rc); exit(-1);}

	printf("allocated pages: %d, reserved pages: %d\n", fh.GetNumAllocatedPages(), fh.GetNumReservedPages());

	// Display page contents.
	while (fh.GetNextPage(ph) != STORM_EOF)
	{
		rc = ph.GetDataPtr(&data);
		if (rc != OK) {DisplayReturnCode(rc);exit(-1);}
		memcpy(&val, data, sizeof(int));

		ph.GetPageID(pid);
		printf("contents of page %d = %d\n", pid, val);
		
		// Unpin the page
		rc = fh.UnpinPage(pid);
		if (rc != OK) {DisplayReturnCode(rc); exit(-1);}
	}

	// Release pages from 1 to 50.
	for (int p=1; p<=50; p++)
	{
		rc = fh.ReleasePage(p);
		if (rc != OK) {DisplayReturnCode(rc); exit(-1);}
	}
	
	printf("allocated pages: %d, reserved pages: %d\n", fh.GetNumAllocatedPages(), fh.GetNumReservedPages());

	rc = mgr.CloseFile(fh);
	if (rc != OK) {DisplayReturnCode(rc); exit(-1);}

	Pause();

	// ====================== STEP 3 =======================================//
	// Open the file again. 
	// Read every page of the file and print the page contents.
	// Close the file.
	// Finally, destroy the file.
	//
	// We expect to see only 50 pages staring from 51 up to 100, since the
	// first 50 have been released in the previous step.
	//======================================================================//

	rc = mgr.OpenFile("test.dat", fh);
	if (rc != OK){DisplayReturnCode(rc); exit(-1);}

	// Display page contents.
	while (fh.GetNextPage(ph) != STORM_EOF)
	{
		rc = ph.GetDataPtr(&data);
		if (rc != OK) {DisplayReturnCode(rc);exit(-1);}
		memcpy(&val, data, sizeof(int));

		ph.GetPageID(pid);
		printf("contents of page %d = %d\n", pid, val);
		
		// Unpin the page
		fh.UnpinPage(pid);
	}

	printf("allocated pages: %d, reserved pages: %d\n", fh.GetNumAllocatedPages(), fh.GetNumReservedPages());

	rc = mgr.CloseFile(fh);
	if (rc != OK) {DisplayReturnCode(rc); exit(-1);}

	Pause();

	rc = mgr.DestroyFile("test.dat");
	if (rc != OK) {DisplayReturnCode(rc);exit(-1);}

	return (0);*/
}




