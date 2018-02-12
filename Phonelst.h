
#include "isammgr\isammgr.h"


class dfPHONELST : public IsamMgr
	{
protected:

	virtual void read_data(void *block);
	virtual void write_data(void *block);
	virtual void fill_buffer(int idxno, long recno);

public:

	dfPHONELST(int isMode = moOpen);
	dfPHONELST(char *dfname, char *ifname, int isMode = moOpen);
	~dfPHONELST(void) { delete idxfname; }
	void clear_buf(void);

	char Dphone[15];
	char Dname[31];
	char Dname2[31];
	char Darea[31];
	char Doccupation[31];
	char Dsubject[31];
	char Dlanguage[11];
	char Dgender[2];
	char Dchannel[2];
	char DLastCallDate[9];
	char DLastCallTime[7];
	int DTotalCalls;
	char Dtype[2];
	char Dfax[2];
	char Dmodem[2];
	char Dalert[2];
	char DRecordCall[2];
	char DDtmfDigits[31];
	char DLinkedFile[13];
	char Dinfo1[31];
	char Dinfo2[31];
	char Dinfo3[31];
	char Desn[9];
	char Dscm[3];
	};

