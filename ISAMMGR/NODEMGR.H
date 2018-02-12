// NODEMGR.H - NodeMgr Class Header

#ifndef NODEMGR_HPP
#define NODEMGR_HPP

#include <stddef.h>
#include <stdio.h>
#include <string.h>

const stLeaf = -1; 
const stNotLeaf = 0;

const noEmpty = -1;
const noFull = -2;
const noDuplicate = -3; 
const noNoSelect = -4; 

class NodeMgr
	{

private:

	int aaaw;  
	int aaaa;  
	int aaay;  
	int aaaz;     

	long aaba; 
	long aaca; 
	short aada;   
	short aaea;    
	long aafa;	

	long *aaga; 
	char **item_aaat; 

	int getpos(char *aaat, int start, int stop);
public:

	NodeMgr(int aaar, int itsize);
	~NodeMgr(void);

	void read_data(char *buffer);
	void write_data(char *buffer);

	int erase(int itemno = -1);
	int insert(long ptr, char *aaat);

	int isleaf(void) { return (aaea == stLeaf); }

	long get_aaba(void) { return aaba; }
	long get_aaca(void) { return aaca; }
	void set_aaba(long node) { aaba = node; }
	void set_aaca(long node) { aaca = node; }
	void set_aaaz_low(void) { aaaz = -1; }
	void set_aaaz_high(void) { aaaz = aada; }
	char *get_high_item(char *buffer) 
		{
		if (aada == 0)
			return NULL;
		else
			memcpy(buffer, item_aaat[aada - 1], aaay);
		return buffer;
		}
	char *get_cur_item(char *buffer) 
		{
		if (aaaz < 0 || aaaz >= aada)
			return NULL;
		else
			memcpy(buffer, item_aaat[aaaz], aaay);
		return buffer;
		}
	long read_ptr(int item) { return aaga[item]; }
	int get_aada(void) { return aada; }
	long find(char *aaat);
	long retrieve(void);
	long next(void);
	long prev(void);
	long aaan(void);
	void reset(int st = stLeaf);
	void split(char *block1, char *block2);
	long get_aafa(void) { return aafa; }
	int get_aaaz(void) { return aaaz; }
	void set_aaaz(int pos) { aaaz = pos; }
	};

#endif
