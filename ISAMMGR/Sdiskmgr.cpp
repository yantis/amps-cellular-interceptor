// Copyright 1993 Nildram Software. All rights reserved.
// May not be copied, except for backup purposes, without express written
// permission from Nildram Software

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "diskmgr.h"

const stEmpty = 0;
const stFull  = 1;
const stDirty = 2;

static int no_instance = 0; 

struct BUFFER
	{
	char *buf; 
	int aaea; 
	unsigned long lruval; 
	int fd; 
	long start; 
	unsigned len; 
	};

static int nobufs;
static BUFFER *buffer;
static long lrucount;

extern int DMCacheSize;

DiskMgr::DiskMgr(int ctype)
	{
	aaia = ctype;
	if (no_instance++ != 0)
		return;
    nobufs = DMCacheSize;
	lrucount = 0;
	buffer = new BUFFER[nobufs];
	for (int x = 0; x < nobufs; x++)
		buffer[x].aaea = stEmpty;
	}

DiskMgr::~DiskMgr(void)
	{
	if (--no_instance != 0)
	   return;
	for (int x = 0; x < nobufs; x++)
		if (buffer[x].aaea != stEmpty)
			{
			flush(x);
			delete buffer[x].buf;
			}
	delete buffer;
	}

int DiskMgr::open(const char *path, int access)
	{
	return sopen(path, access, SH_DENYRW);
	}

long DiskMgr::seek(int fd, long offset)
	{
	return lseek(fd, offset, SEEK_SET);
	}

int DiskMgr::close(int handle)
	{
	if (aaia != caNoCache)
		for (int x = 0; x < nobufs; x++)
			if (buffer[x].fd == handle && buffer[x].aaea != stEmpty)
				{
				flush(x);
				delete buffer[x].buf;
				buffer[x].aaea = stEmpty;
				}
	return ::close(handle);
	}

void DiskMgr::flush(int x)
	{
	if (buffer[x].aaea == stDirty)
		{
		seek(buffer[x].fd, buffer[x].start);
		::write(buffer[x].fd, buffer[x].buf, buffer[x].len);
		}
	buffer[x].aaea = stFull;
	}

int DiskMgr::read(int handle, void *buf, long start, unsigned len)
	{
	if (aaia == caNoCache)
		{
		seek(handle, start);
		return ::read(handle, buf, len);
		}
	for (int x = 0; x < nobufs; x++)
		if (buffer[x].fd == handle && buffer[x].aaea != stEmpty &&
			((buffer[x].start <= start && (buffer[x].start + buffer[x].len) > start &&
			(buffer[x].start + buffer[x].len) < (start + len)) ||
			(buffer[x].start > start && buffer[x].start < (start + len))))
			{
			flush(x);
			delete buffer[x].buf;
			buffer[x].aaea = stEmpty;
			}
	for (x = 0; x < nobufs; x++)
		if (buffer[x].fd == handle && buffer[x].aaea != stEmpty &&
			buffer[x].start <= start &&
			(buffer[x].start + buffer[x].len) >= (start + len))
			{
			memcpy(buf, buffer[x].buf + (start - buffer[x].start), len);
			buffer[x].lruval = lrucount++;
			return len;
			}
	int lru_item = 0;
	unsigned long lru_value = 0xFFFFFFFFL;
	for (x = 0; x < nobufs; x++)
		{
		if (buffer[x].aaea == stEmpty)
			break;
		if (buffer[x].lruval < lru_value)
			{
			lru_item = x;
			lru_value = buffer[x].lruval;
			}
		}
	if (x == nobufs) 
		{
		x = lru_item;
		flush(x);
		delete buffer[x].buf;
		buffer[x].aaea = stEmpty;
		}
	seek(handle, start);
	int retval = ::read(handle, buf, len);
	if (retval != len)
	   return retval;
	buffer[x].buf = new char[len];
	buffer[x].aaea = stFull;
	buffer[x].lruval = lrucount++;
	buffer[x].fd = handle;
	buffer[x].start = start;
	buffer[x].len = len;
	memcpy(buffer[x].buf, buf, len);
	return len;
	}

int DiskMgr::write(int handle, void *buf, long start, unsigned len)
	{
	if (aaia == caNoCache)
		{
		seek(handle, start);
		return ::write(handle, buf, len);
		}
	for (int x = 0; x < nobufs; x++)
		if (buffer[x].fd == handle && buffer[x].aaea != stEmpty &&
			((buffer[x].start <= start && (buffer[x].start + buffer[x].len) > start &&
			(buffer[x].start + buffer[x].len) < (start + len)) ||
			(buffer[x].start > start && buffer[x].start < (start + len))))
			{
			flush(x);
			delete buffer[x].buf;
			buffer[x].aaea = stEmpty;
			}
	for (x = 0; x < nobufs; x++)
		if (buffer[x].fd == handle && buffer[x].aaea != stEmpty &&
			buffer[x].start <= start &&
			(buffer[x].start + buffer[x].len) >= (start + len))
			{
			memcpy(buffer[x].buf + (start - buffer[x].start), buf, len);
			buffer[x].lruval = lrucount++;
			buffer[x].aaea = stDirty;
			return len;
			}

	int lru_item = 0;
	unsigned long lru_value = 0xFFFFFFFFL;
	for (x = 0; x < nobufs; x++)
		{
		if (buffer[x].aaea == stEmpty)
			break;
		if (buffer[x].lruval < lru_value)
			{
			lru_item = x;
			lru_value = buffer[x].lruval;
			}
		}
	if (x == nobufs) 
		{
		x = lru_item;
		flush(x);
		delete buffer[x].buf;
		}
	buffer[x].buf = new char[len];
	buffer[x].aaea = stDirty;
	buffer[x].lruval = lrucount++;
	buffer[x].fd = handle;
	buffer[x].start = start;
	buffer[x].len = len;
	memcpy(buffer[x].buf, buf, len);
	return len;
	}

void DiskMgr::unlock(int)
	{
	}

void DiskMgr::unlock_head(int)
	{
	}

void DiskMgr::lock_head(int)
	{
	}

void DiskMgr::lock(int, long, long)
	{
	}

void DiskMgr::lockfile(int)
	{
	}



