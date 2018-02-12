// INDEXMGR.H - IndexMgr Class Header

#ifndef INDEXMGR_H
#define INDEXMGR_H

#include "filemgr.h"
#include "nodemgr.h"

const idxRecSize = 1022;

class IndexMgr : public FileMgr
	{

private:

	int aaaa; 
	long aaab; 
	long aaac; 
	long aaad; 
	long aaae; 
	long aaaf; 
	long aaag; 
	long aaah; 
	long aana; 
	long aaai[32]; 
	int aaaj; 
	int aaak; 
	int aaal; 
	NodeMgr *node;

	virtual void read_data(void *block); 
	virtual void write_data(void *block); 

	virtual void create_file(void) { create_idx(); }

	long aaam(void *aaat, long recno); 
public:

    IndexMgr(const char *fname, int itsize, int unique);
    IndexMgr(const char *fname, int itsize, int unique, int file_des,
             int idxno, int create);
	~IndexMgr(void);
	void create_idx(void);

	int get_fd(void) { return fd; }
    void set_fd(int file_des) { fd = file_des; aaaj = -1; rew(); }
	long find(void *aaat);
	long retrieve(void);
    long next(int no_recs);
	long prev(int no_recs);
	void rew(void);
	int compare(void *aaat);
	int insert(long recno, void *aaat);
	int erase(void);
	void node_split(void);
	int created_idx(void) { return aaak; }
	void read_head_info(void);
	int check_node_del(void); 
	int check_split_del(void); 
	void inc_node_del(void);
	void inc_node_split(void);
	long relocate(void);
	void aaan(long recno);
	void ffwd(void);	
	};

#endif
