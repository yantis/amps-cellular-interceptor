
#include "PHONELST.H"

dfPHONELST::dfPHONELST(int isMode) :
	IsamMgr("dbase\\PHONELST.DTA", 362)
	{
	idxfname = new char[strlen("dbase\\PHONELST.DTX") + 1];
	strcpy(idxfname, "dbase\\PHONELST.DTX");
	ifile[0] = new IndexMgr(idxfname, 19, 0);
	idxname[0] = "Phone";
	buffer = new char[19];
	noidxs = 1;
	if (ifile[0]->created_idx())
		rebuild_idxs();
	if (isMode == moClosed)
		close();
	}

dfPHONELST::dfPHONELST(char *dfname, char *ifname, int isMode) :
	IsamMgr(dfname, 362)
	{
	idxfname = new char[strlen(ifname) + 1];
	strcpy(idxfname, ifname);
	ifile[0] = new IndexMgr(idxfname, 19, 0);
	idxname[0] = "Phone";
	buffer = new char[19];
	noidxs = 1;
	if (ifile[0]->created_idx())
		rebuild_idxs();
	if (isMode == moClosed)
		close();
	}

void dfPHONELST::read_data(void *block)
	{
	char *a = (char *)block;
	memcpy((void *)Dphone, a, 15);
	a += 15;
	memcpy((void *)Dname, a, 31);
	a += 31;
	memcpy((void *)Dname2, a, 31);
	a += 31;
	memcpy((void *)Darea, a, 31);
	a += 31;
	memcpy((void *)Doccupation, a, 31);
	a += 31;
	memcpy((void *)Dsubject, a, 31);
	a += 31;
	memcpy((void *)Dlanguage, a, 11);
	a += 11;
	memcpy((void *)Dgender, a, 2);
	a += 2;
	memcpy((void *)Dchannel, a, 2);
	a += 2;
	memcpy((void *)DLastCallDate, a, 9);
	a += 9;
	memcpy((void *)DLastCallTime, a, 7);
	a += 7;
	memcpy((void *)&DTotalCalls, a, 2);
	a += 2;
	memcpy((void *)Dtype, a, 2);
	a += 2;
	memcpy((void *)Dfax, a, 2);
	a += 2;
	memcpy((void *)Dmodem, a, 2);
	a += 2;
	memcpy((void *)Dalert, a, 2);
	a += 2;
	memcpy((void *)DRecordCall, a, 2);
	a += 2;
	memcpy((void *)DDtmfDigits, a, 31);
	a += 31;
	memcpy((void *)DLinkedFile, a, 13);
	a += 13;
	memcpy((void *)Dinfo1, a, 31);
	a += 31;
	memcpy((void *)Dinfo2, a, 31);
	a += 31;
	memcpy((void *)Dinfo3, a, 31);
	a += 31;
	memcpy((void *)Desn, a, 9);
	a += 9;
	memcpy((void *)Dscm, a, 3);
	a += 3;
	}

void dfPHONELST::write_data(void *block)
	{
	char *a = (char *)block;
	memcpy(a, (void *)Dphone, 15);
	a += 15;
	memcpy(a, (void *)Dname, 31);
	a += 31;
	memcpy(a, (void *)Dname2, 31);
	a += 31;
	memcpy(a, (void *)Darea, 31);
	a += 31;
	memcpy(a, (void *)Doccupation, 31);
	a += 31;
	memcpy(a, (void *)Dsubject, 31);
	a += 31;
	memcpy(a, (void *)Dlanguage, 11);
	a += 11;
	memcpy(a, (void *)Dgender, 2);
	a += 2;
	memcpy(a, (void *)Dchannel, 2);
	a += 2;
	memcpy(a, (void *)DLastCallDate, 9);
	a += 9;
	memcpy(a, (void *)DLastCallTime, 7);
	a += 7;
	memcpy(a, (void *)&DTotalCalls, 2);
	a += 2;
	memcpy(a, (void *)Dtype, 2);
	a += 2;
	memcpy(a, (void *)Dfax, 2);
	a += 2;
	memcpy(a, (void *)Dmodem, 2);
	a += 2;
	memcpy(a, (void *)Dalert, 2);
	a += 2;
	memcpy(a, (void *)DRecordCall, 2);
	a += 2;
	memcpy(a, (void *)DDtmfDigits, 31);
	a += 31;
	memcpy(a, (void *)DLinkedFile, 13);
	a += 13;
	memcpy(a, (void *)Dinfo1, 31);
	a += 31;
	memcpy(a, (void *)Dinfo2, 31);
	a += 31;
	memcpy(a, (void *)Dinfo3, 31);
	a += 31;
	memcpy(a, (void *)Desn, 9);
	a += 9;
	memcpy(a, (void *)Dscm, 3);
	a += 3;
	}

void dfPHONELST::fill_buffer(int idxno, long recno)
	{
	char *a = buffer;
	switch (idxno)
		{
		case 0:
			memset(buffer, 0, 19);
			prep(Dphone, a, 15);
			a += 15;
			prep(recno, a, 4);
			break;
		default:
			prep(recno, a, 4);
			break;
		}
	}

void dfPHONELST::clear_buf(void)
	{
	char *block = new char[362];
	memset(block, 0, 362);
	read_data(block);
	delete block;
	}

