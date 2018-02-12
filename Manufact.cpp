
#include "MANUFACT.H"

dfMANUFACT::dfMANUFACT(int isMode) :
	IsamMgr("dbase\\MANUFACT.DTA", 34)
	{
	// idxfname = new char[strlen("dbas\\MANUFACT.DTX") + 1];
	idxfname = new char[strlen("dbas\\MANUFACT.DTX") + 2]; // this is to fix memory overrun
	strcpy(idxfname, "dbase\\MANUFACT.DTX");
	ifile[0] = new IndexMgr(idxfname, 7, 0);
	idxname[0] = "Hex";
	buffer = new char[7];
	noidxs = 1;
	if (ifile[0]->created_idx())
		rebuild_idxs();
	if (isMode == moClosed)
		close();
	}

dfMANUFACT::dfMANUFACT(char *dfname, char *ifname, int isMode) :
	IsamMgr(dfname, 34)
	{
	idxfname = new char[strlen(ifname) + 1];
	strcpy(idxfname, ifname);
	ifile[0] = new IndexMgr(idxfname, 7, 0);
	idxname[0] = "Hex";
	buffer = new char[7];
	noidxs = 1;
	if (ifile[0]->created_idx())
		rebuild_idxs();
	if (isMode == moClosed)
		close();
	}

void dfMANUFACT::read_data(void *block)
	{
	char *a = (char *)block;
	memcpy((void *)hex, a, 3);
	a += 3;
	memcpy((void *)man, a, 31);
	a += 31;
	}

void dfMANUFACT::write_data(void *block)
	{
	char *a = (char *)block;
	memcpy(a, (void *)hex, 3);
	a += 3;
	memcpy(a, (void *)man, 31);
	a += 31;
	}

void dfMANUFACT::fill_buffer(int idxno, long recno)
	{
	char *a = buffer;
	switch (idxno)
		{
		case 0:
			memset(buffer, 0, 7);
			prep(hex, a, 3);
			a += 3;
			prep(recno, a, 4);
			break;
		default:
			prep(recno, a, 4);
			break;
		}
	}

void dfMANUFACT::clear_buf(void)
	{
	char *block = new char[34];
	memset(block, 0, 34);
	read_data(block);
	delete block;
	}

