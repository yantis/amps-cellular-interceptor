
#include "isammgr\isammgr.h"


class dfMANUFACT : public IsamMgr
	{
protected:

	virtual void read_data(void *block);
	virtual void write_data(void *block);
	virtual void fill_buffer(int idxno, long recno);

public:

	dfMANUFACT(int isMode = moOpen);
	dfMANUFACT(char *dfname, char *ifname, int isMode = moOpen);
	~dfMANUFACT(void) { delete idxfname; }
	void clear_buf(void);

	char hex[3];
	char man[31];
	};

