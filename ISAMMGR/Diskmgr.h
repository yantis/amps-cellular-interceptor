// DISKMGR.H - DiskMgr Class Header

#ifndef DISKMGR_H
#define DISKMGR_H

#include <io.h>
#include <share.h>

#define IM_OK 0
#define IM_ERROR (-1)

const caReadWrite = 0;
const caNoCache   = 1;

class DiskMgr
	{
private:


	int aaia; 
	void flush(int x);


	long aaja;
	long aaka;
	int aala;
	int aama;

public:

    DiskMgr(int ctype = caReadWrite);
	~DiskMgr(void);
	int open(const char *path, int access);
	int close(int handle);
	int read(int handle, void *buf, long start, unsigned len);
	int write(int handle, void *buf, long start, unsigned len);
	long seek(int fd, long offset);
	void unlock(int handle);
	void lock(int handle, long start, long length);
	void lockfile(int handle);
	void unlock_head(int handle);
	void lock_head(int handle);
	};

#endif
