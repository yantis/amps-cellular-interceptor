
#include "ISAMMGR\Isammgr.h"


class dfAREA : public IsamMgr
	{
protected:

	virtual void read_data(void *block);
	virtual void write_data(void *block);
	virtual void fill_buffer(int idxno, long recno);

public:

	dfAREA(int isMode = moOpen);
	dfAREA(char *dfname, char *ifname, int isMode = moOpen);
	~dfAREA(void) { delete idxfname; }
	void clear_buf(void);

	char Anumber[14];
	char Aarea[29];
	};

