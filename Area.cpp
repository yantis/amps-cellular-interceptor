
#include "AREA.H"

dfAREA::dfAREA(int isMode) :
	IsamMgr("dbase//AREA.DTA", 43)
	{
	idxfname = new char[strlen("dbase//AREA.DTX") + 1];
	strcpy(idxfname, "dbase//AREA.DTX");
	ifile[0] = new IndexMgr(idxfname, 18, 0);
	idxname[0] = "Number";
	buffer = new char[18];
	noidxs = 1;
	if (ifile[0]->created_idx())
		rebuild_idxs();
	if (isMode == moClosed)
		close();
	}

dfAREA::dfAREA(char *dfname, char *ifname, int isMode) :
	IsamMgr(dfname, 43)
	{
	idxfname = new char[strlen(ifname) + 1];
	strcpy(idxfname, ifname);
	ifile[0] = new IndexMgr(idxfname, 18, 0);
	idxname[0] = "Number";
	buffer = new char[18];
	noidxs = 1;
	if (ifile[0]->created_idx())
		rebuild_idxs();
	if (isMode == moClosed)
		close();
	}

void dfAREA::read_data(void *block)
	{
	char *a = (char *)block;
	memcpy((void *)Anumber, a, 14);
	a += 14;
	memcpy((void *)Aarea, a, 29);
	a += 29;
	}

void dfAREA::write_data(void *block)
	{
	char *a = (char *)block;
	memcpy(a, (void *)Anumber, 14);
	a += 14;
	memcpy(a, (void *)Aarea, 29);
	a += 29;
	}

void dfAREA::fill_buffer(int idxno, long recno)
	{
	char *a = buffer;
	switch (idxno)
		{
		case 0:
			memset(buffer, 0, 18);
			prep(Anumber, a, 14);
			a += 14;
			prep(recno, a, 4);
			break;
		default:
			prep(recno, a, 4);
			break;
		}
	}

void dfAREA::clear_buf(void)
	{
	char *block = new char[43];
	memset(block, 0, 43);
	read_data(block);
	delete block;
	}

