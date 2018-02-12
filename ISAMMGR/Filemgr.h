// FILEMGR.H - FileMgr Class Header

#ifndef FILEMGR_H
#define FILEMGR_H

#include "diskmgr.h"

const moOpen   = 1;
const moClosed = 2;
const moAlreadyOpen = 3;

const rsDeleted = 1; 
const rsOK      = 0; 

#define HEADSIZ 1024 /* Size of Header record */


class FileMgr : public DiskMgr
	{

protected:

	char *aaao; 
	int fd; 
	short aaaq; 
	int aaar; 

	int aaas; 

	char *aaat; 
	long aaau; 

	int create(void);

	int aaav; 

	virtual void read_data(void *block) {} 
	virtual void write_data(void *block) {} 
	virtual void create_file(void) {} 

	void write_head(int rec_aaea, long aaoa, long recno);

public:

	FileMgr(const char *fname, int aaar, int ctype, int mode = moOpen, int file_des = -1);
	~FileMgr(void);
    int open_file(void);
    void close_file(void);
	void insert(void);
	int amend(void);
	int erase(void);
	int read(long recno);
	int next(void);
	int lock(long recno);
	int unlock(void);
	void lockfile(void)
		{
		DiskMgr::lockfile(fd);
		}
	};

#endif
