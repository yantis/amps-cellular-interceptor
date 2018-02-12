// ISAMMGR.H - IsamMgr Class Header

#ifndef ISAMMGR_H
#define ISAMMGR_H

#include "filemgr.h"
#include "indexmgr.h"

#define MAXIDXS 30

class IsamMgr : public FileMgr
	{

protected:

	int curidx; 
	int noidxs; 
	IndexMgr *ifile[MAXIDXS];
    char *idxname[MAXIDXS];
	char *idxfname;
	char *buffer;

	virtual void read_data(void *block) {};
	virtual void write_data(void *block) {};

    virtual void fill_buffer(int idxno, long recno) {};

	void prep(unsigned long a, char *b, int length)
		{
		*b++ = (char)((a & 0xff000000L) >> 24);
		*b++ = (char)((a & 0x00ff0000L) >> 16);
		*b++ = (char)((a & 0x0000ff00L) >> 8);
		*b++ = (char)(a & 0x000000ffL);
		}

	void prep(long a, char *b, int length)
		{
		unsigned long c = (unsigned long)a + 0x80000000L;
		*b++ = (char)((c & 0xff000000L) >> 24);
		*b++ = (char)((c & 0x00ff0000L) >> 16);
		*b++ = (char)((c & 0x0000ff00L) >> 8);
		*b++ = (char)(c & 0x000000ffL);
		}

	void prep(unsigned int a, char *b, int length)
		{

//#if sizeof(int) == 4
//		prep((unsigned long)a, b, length);
//#else
		*b++ = (char)((a & 0xff00) >> 8);
		*b++ = (char)(a & 0x00ff);
//#endif
		}

	void prep(int a, char *b, int length)
		{
//#if sizeof(int) == 4
//		prep((long)a, b, length);
//#else
		unsigned int c = (unsigned int)a + 0x8000;
		*b++ = (char)((c & 0xff00) >> 8);
		*b++ = (char)(c & 0x00ff);
//#endif
		}

	void prep(unsigned short a, char *b, int length)
		{
		*b++ = (a & 0xff00) >> 8;
		*b++ = (a & 0x00ff);
		}

	void prep(short a, char *b, int length)
		{
		unsigned short c = (unsigned short)a + 0x8000;
		*b++ = (c & 0xff00) >> 8;
		*b++ = (c & 0x00ff);
		}

    void prep(unsigned char a, char *b, int length)
         {
         *b = a;
         }

    void prep(char a, char *b, int length)
         {
         *b = (unsigned char)a + 0x80;
         }

    void prep(float a, char *b, int length)
         {
         *(float *)b = a;
		 }

	void prep(double a, char *b, int length)
		 {
		 *(double *)b = a;
		 }

	void prep(char *a, char *b, int length)
		 {
		 strncpy(b, a, length);
		 }

	void prep(short *a, char *b, int length)
		{
		memcpy(b, (char *)a, length);
		}

	void prep(unsigned short *a, char *b, int length)
		{
		memcpy(b, (char *)a, length);
		}

	void prep(int *a, char *b, int length)
		{
		memcpy(b, (char *)a, length);
		}

	void prep(unsigned int *a, char *b, int length)
		{
		memcpy(b, (char *)a, length);
		}

	void prep(long *a, char *b, int length)
		{
		memcpy(b, (char *)a, length);
		}

	void prep(unsigned long *a, char *b, int length)
		{
		memcpy(b, (char *)a, length);
		}

	void rebuild_idxs(void);

public:

	IsamMgr(char *dfName, int dSize, int isMode = moOpen);
    ~IsamMgr(void);

	int insert(void);
	int retrieve(void);
    int next(int no_recs = 1);
    int prev(int no_recs = 1);
	void rew(void);
	void ffwd(void);
	int find(void);
	int erase(void);
	int amend(void);
    void set_idx(const char *name);
    void set_idx(int idxno);
    int get_cur_idx(void) { return curidx; }
    char *get_idx_name(int idxno) { return idxname[idxno]; }
    int get_no_idxs(void) { return noidxs; }
    void open(void);
	void close(void);
	void unlock(void) { FileMgr::unlock(); aaau = -1; }
	};

#endif
