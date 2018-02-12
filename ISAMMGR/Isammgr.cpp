// Copyright 1993 Nildram Software. All rights reserved.
// May not be copied, except for backup purposes, without express written
// permission from Nildram Software

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "isammsg.h"
#include "isammgr.h"

IsamMgr::IsamMgr(char *dfName, int dSize, int isMode) :
	FileMgr(dfName, dSize, caNoCache, isMode)
	{
	curidx = 0;
	idxfname = "Invalid Index";
    aaau = -1;
	}

IsamMgr::~IsamMgr(void)
	{
	for (int x = 0; x < noidxs; x++)
		delete ifile[x];
	delete buffer;
	}

void IsamMgr::rebuild_idxs(void)
	{
	for (int x = 0; x < noidxs; x++)
		{
		long count = 0;
		aaau = -1;
		while (FileMgr::next() != -1)
			{
			fill_buffer(x, aaau);
			if (ifile[x]->insert(aaau, buffer) == noDuplicate)
				{
				IsamError("Error rebuilding Index: Duplicate Key!");
                exit(0);
				}
			if ((++count % 10) == 0)
                {
                if (count == 10)
                    IsamInitProgress(idxfname, idxname[x]);
                IsamProgress(count);
                }
			}
        if ((count % 10) != 0 && count > 10)
            {
            IsamProgress(count);
			IsamEndProgress();
			}
		}
	}

int IsamMgr::insert(void)
	{
	FileMgr::insert();
	for (int x = 0; x < noidxs; x++)
		{
		fill_buffer(x, aaau);
		if (ifile[x]->insert(aaau, buffer) == noDuplicate)
			{
			for (int y = x - 1; y >= 0; y--)
				ifile[y]->erase();
			FileMgr::erase();
			return -1;
			}
		}
	return 0;
	}

int IsamMgr::next(int no_recs)
	{
	long block;
	if (curidx == -1) 
		{
		int retval;
		for (; no_recs > 0; no_recs--)
			{
			retval = FileMgr::next();
			if (retval == -1)
				return -1;
			}
		return retval;
		}
	do  {
		aaau = -1;
		FileMgr::unlock();
		if ((block = ifile[curidx]->next(no_recs)) == -1)
			return -1;
		FileMgr::lock(block);
		FileMgr::read(block);
		no_recs = 1;
		} while (ifile[curidx]->retrieve() != block);
	return 0;
	}

int IsamMgr::prev(int no_recs)
	{
	long block;
	do  {
		FileMgr::unlock();
		aaau = -1;
		if ((block = ifile[curidx]->prev(no_recs)) == -1)
			return -1;
		FileMgr::lock(block);
		FileMgr::read(block);
		no_recs = 1;
		} while (ifile[curidx]->retrieve() != block);
	return 0;
	}

void IsamMgr::rew(void)
	{
	aaau = -1;
	if (curidx != -1)
		ifile[curidx]->rew();
	FileMgr::unlock();
	}

void IsamMgr::ffwd(void)
	{
	aaau = -1;
	if (curidx != -1)
		ifile[curidx]->ffwd();
	FileMgr::unlock();
	}

int IsamMgr::find(void)
	{
	long block;
	do {
		FileMgr::unlock();
		aaau = -1;
		fill_buffer(curidx, 0);
		if ((block = ifile[curidx]->find(buffer)) == -1)
				return -1;
		FileMgr::lock(block);
		FileMgr::read(block);
		} while (ifile[curidx]->retrieve() != block);
	return 0;
	}

int IsamMgr::retrieve(void)
	{
	long block;
	aaau = -1;
	if ((block = ifile[curidx]->retrieve()) == -1)
		return -1;
	FileMgr::lock(block);
	FileMgr::read(block);
	return 0;
	}

int IsamMgr::erase(void)
	{
	if (aaau == -1)
		return -1;
	for (int x = 0; x < noidxs; x++)
		{
		fill_buffer(x, aaau);
		if (ifile[x]->find(buffer) != -1)
			ifile[x]->erase();
		}
	FileMgr::erase();
	FileMgr::unlock();
	return 0;
	}

int IsamMgr::amend(void)
	{
	if (aaau == -1)
		return -1;
	char *block = new char[FileMgr::aaar];
	char *block2 = new char[FileMgr::aaar];
	write_data(block);

	FileMgr::read(aaau);
	write_data(block2);
	for (int x = 0; x < noidxs; x++)
		{
		fill_buffer(x, aaau);
		ifile[x]->find(buffer);
		}

	read_data(block);
	int xchg[MAXIDXS];

	
	for (x = 0; x < noidxs; x++)
		{
		xchg[x] = 0;
		fill_buffer(x, aaau);
		if (ifile[x]->compare(buffer) != 0)
			{
			xchg[x] = 1;
			if (ifile[x]->insert(aaau, buffer) == noDuplicate)
				{
				for (int y = x - 1; y >= 0; y--)
					if (xchg[y])
						ifile[y]->erase();
				
				read_data(block2);
				fill_buffer(curidx, aaau);
				ifile[curidx]->find(buffer);

				
				read_data(block);
				delete block;
				delete block2;
				return -1;
				}
			}
		}

	FileMgr::amend();

	
	read_data(block2);
	for (x = 0; x < noidxs; x++)
		if (xchg[x])
			{
			long block;
			fill_buffer(x, aaau);
			block = ifile[x]->find(buffer);
			if (block == aaau)
				ifile[x]->erase();
			}

	
	read_data(block);
	fill_buffer(curidx, aaau);
	ifile[curidx]->find(buffer);

	delete block;
	delete block2;
	return 0;
	}

void IsamMgr::set_idx(const char *name)
	{
	for (int x = 0; x < noidxs; x++)
		if (strcmp(idxname[x], name) == 0)
			{
			curidx = x;
			return;
			}
	IsamError("Invalid Index Name");
	exit(0);
	}

void IsamMgr::set_idx(int idxno)
	 {
	 if (idxno > noidxs || idxno < -1)
		IsamError("Invalid Index No");
	 curidx = idxno;
	 }

void IsamMgr::close(void)
	{
	if (fd == -1)
		return;
	aaau = -1;
	FileMgr::close_file();
	for (int x = 0; x < noidxs; x++)
		ifile[x]->close_file();
	}

void IsamMgr::open(void)
	{
	if (fd != -1)
		return;
	FileMgr::open_file();
	ifile[0]->open_file();
	for (int x = 0; x < noidxs; x++)
		ifile[x]->set_fd(ifile[0]->get_fd());
	}

