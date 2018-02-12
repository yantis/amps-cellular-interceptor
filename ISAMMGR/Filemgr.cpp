// Copyright 1993 Nildram Software. All rights reserved.
// May not be copied, except for backup purposes, without express written
// permission from Nildram Software

#include <sys\stat.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <share.h>
#include <errno.h>
#include <stdlib.h>

#include "filemgr.h"
#include "isammsg.h"

FileMgr::FileMgr(const char *fname, int rsize, int ctype, int mode, int file_des) :
	DiskMgr(ctype)
	{
	aaao = new char[strlen(fname) + 1];
	strcpy(aaao, fname);
	aaar = rsize;
	aaaq = aaar + 2;
	fd = -1;
    aaas = 1;
	if (mode == moOpen)
		{
		open_file();
		}
	if (mode == moAlreadyOpen)
		{
		fd = file_des;
        aaas = 0;
		int tmp_aaaq = -1;
        DiskMgr::read(fd, &tmp_aaaq, 0, sizeof(tmp_aaaq));
		if (tmp_aaaq != aaaq)
			{
			IsamError("Error: Program and Data Files out of sync!");
			exit(0);
			}
		}
	aaau = -1;
	aaav = 6;
	}

FileMgr::~FileMgr(void)
	{
    close_file();
	delete aaao;
	}

int FileMgr::create(void)
	{
    long temp = -1;
	fd = sopen(aaao, O_RDWR | O_BINARY | O_CREAT | O_TRUNC, SH_DENYRW, S_IREAD | S_IWRITE);
	if (fd == -1)
		return -1;
    DiskMgr::write(fd, &aaaq, 0, sizeof(aaaq));
    DiskMgr::write(fd, &temp, sizeof(aaaq), sizeof(temp));
	create_file();
    DiskMgr::close(fd);
	fd = -1;
	return 0;
	}

int FileMgr::open_file(void)
	{
	while (fd == -1)
		{
		fd = DiskMgr::open(aaao, O_RDWR | O_BINARY);
		if (fd == -1)
			{
			switch (errno)
				{
				case ENOENT:
					create();
					break;

				case EACCES:
					break;

				default:
					return -1;
				}
			}
		}
	short tmp_aaaq = -1;
	DiskMgr::read(fd, &tmp_aaaq, 0, sizeof(tmp_aaaq));
	if (tmp_aaaq != aaaq)
		{
		close_file();
		return -1;
		}
	return 0;
	}

void FileMgr::close_file(void)
	{
	if (fd != -1)
		{
		aaau = -1;
		DiskMgr::unlock(fd);
		if (aaas)
            DiskMgr::close(fd);
		fd = -1;
		}
	}

void FileMgr::insert(void)
	{
	aaat = new char[aaaq];
	write_data(aaat + 2);
	*aaat = 0;
	*(aaat + 1) = 0;
	long aaoa;
	DiskMgr::lock_head(fd);
	DiskMgr::read(fd, &aaau, 2, sizeof(aaau));
	if (aaau < 0)
		{
		aaau = -1 - aaau;
		aaoa = -aaau - 2;
		DiskMgr::write(fd, aaat, HEADSIZ + (aaau * aaaq), aaaq);
		}
	else
		{
		DiskMgr::read(fd, &aaoa, HEADSIZ + (aaau * aaaq) + 2, sizeof(aaoa));
		DiskMgr::write(fd, aaat, HEADSIZ + (aaau * aaaq), aaaq);
		}
	DiskMgr::write(fd, &aaoa, 2, sizeof(aaoa));
	DiskMgr::unlock_head(fd);
	delete aaat;
	}


int FileMgr::amend(void)
	{
	if (aaau == -1)
		return -1;
	aaat = new char[aaaq];
	write_data(aaat + 2);
	*aaat = 0;
	*(aaat + 1) = 0;
	DiskMgr::write(fd, aaat, HEADSIZ + (aaau * aaaq), aaaq);
	delete aaat;
	return 0;
	}

int FileMgr::read(long recno)
	{
	if (fd == -1)
		return -1;
	aaau = recno;
	if (recno == -1)
		return -1;
	aaat = new char[aaaq];
	DiskMgr::read(fd, aaat, HEADSIZ + (recno * aaaq), aaaq);
	read_data(aaat + 2);
	delete aaat;
	return 0;
	}

int FileMgr::lock(long recno)
	{
	if (fd == -1)
		return -1;
	if (recno == -1)
		return -1;
	DiskMgr::lock(fd, HEADSIZ + (recno * aaaq), aaaq);
	return 0;
	}

int FileMgr::unlock(void)
	{
	if (fd == -1)
		return -1;
	DiskMgr::unlock(fd);
	return 0;
	}

int FileMgr::next(void)
	{
	if (fd == -1)
		return -1;
	aaat = new char[aaaq];
	unlock();
	do	{
		aaau++;
		lock(aaau);
		if (DiskMgr::read(fd, aaat, HEADSIZ + (aaau * aaaq), aaaq) != aaaq)
			{
			delete aaat;
			unlock();
			return -1;
			}
		} while (*(int *)aaat == rsDeleted);
	read_data(aaat + 2);
	delete aaat;
	return 0;
	}

int FileMgr::erase(void)
	{
	if (fd == -1)
		return -1;
	if (aaau == -1)
		return -1;

	long aaoa;
	DiskMgr::lock_head(fd);
	DiskMgr::read(fd, &aaoa, 2, sizeof(aaoa));
	DiskMgr::write(fd, &aaau, 2, sizeof(aaau));

	

	int rec_aaea = rsDeleted;
	write_head(rec_aaea, aaoa, aaau);

	DiskMgr::unlock_head(fd);
	aaau = -1;
	return 0;
	}

void FileMgr::write_head(int rec_aaea, long aaoa, long recno)
	 {
	 char buffer[6];
	 *(int *)buffer = rec_aaea;
	 *(long *)(buffer + 2) = aaoa;
	 DiskMgr::write(fd, buffer, HEADSIZ + (recno * aaaq), 6);
	 }

