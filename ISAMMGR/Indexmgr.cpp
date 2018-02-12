// Copyright 1993 Nildram Software. All rights reserved.
// May not be copied, except for backup purposes, without express written
// permission from Nildram Software

#include <io.h>
#include <stdio.h>

#include "indexmgr.h"


IndexMgr::IndexMgr(const char *fname, int itsize, int unique) :
	FileMgr(fname, idxRecSize, caReadWrite, moClosed)
	{
	aaaa = itsize;
	aaal = unique;
	node = new NodeMgr(idxRecSize, aaaa);
	aaab = aaav;
	aaac = aaav + 4;
	aaad = aaav + 8;
	aaae = aaav + 12;
	aaav += 16; 
	aaak = 0;
	FileMgr::open_file(); 
	aaaj = -1;
	DiskMgr::read(fd, &aaaf, aaab, 4);
	DiskMgr::read(fd, &aaag, aaac, 4);
	rew();
	}

IndexMgr::IndexMgr(const char *fname, int itsize, int unique, int file_des, int idxno, int create) :
	FileMgr(fname, idxRecSize, caReadWrite, moAlreadyOpen, file_des)
	{
	aaaa = itsize;
	aaal = unique;
	node = new NodeMgr(idxRecSize, aaaa);
	aaab = aaav + (idxno * 16);
	aaac = aaab + 4;
	aaad = aaab + 8;
	aaae = aaab + 12;
	aaav += (idxno + 1) * 16;
	if (create)
		create_idx();
	aaaj = -1;
	DiskMgr::read(fd, &aaaf, aaab, 4);
	DiskMgr::read(fd, &aaag, aaac, 4);
	rew();
	}

IndexMgr::~IndexMgr(void)
	{
	delete node;
	}



void IndexMgr::create_idx(void)
	{
	aaak = 1;
	aaah = aana = 0;
	node->reset(); 
	FileMgr::insert();
	DiskMgr::write(fd, &aaau, aaab, sizeof(aaau));
	DiskMgr::write(fd, &aaau, aaac, sizeof(aaau)); 
	DiskMgr::write(fd, &aaah, aaad, 4);
	DiskMgr::write(fd, &aana, aaae, 4);
	}

void IndexMgr::read_data(void *block)
	{
	node->read_data((char *)block);
	}

void IndexMgr::write_data(void *block)
	{
	node->write_data((char *)block);
	}

long IndexMgr::aaam(void *aaat, long recno)
	{
	aaai[++aaaj] = recno;
	read(recno);
	long aaoa = node->find((char *)aaat);
	if (aaoa != -1 && !node->isleaf())
		return aaam(aaat, aaoa);
	if (compare(aaat) != 0)
	   return -1;
	return aaoa;
	}

long IndexMgr::find(void *aaat)
	{
	aaaj = -1;
	
	for (;;)
		{
		long retval;
		read_head_info();
		retval = aaam(aaat, aaaf);
		if (retval != -1 || check_split_del())
			return retval;
		}
	}

void IndexMgr::node_split(void)
	{
	long recno;
	char *block1 = new char[idxRecSize];
	char *block2 = new char[idxRecSize];
	char *aaat1 = new char[aaaa];
	char *aaat2 = new char[aaaa];
	inc_node_split();
	for (int x = aaaj; x >= 0; x--)
		{
		recno = aaai[x];
		FileMgr::read(recno);
		node->split(block1, block2);
		
		node->read_data(block2);
		node->set_aaba(recno);
		long right_node = node->get_aaca();
		node->get_high_item(aaat2);
		FileMgr::insert();
		
		long new_rec = aaau;
		aaau = recno;
		node->read_data(block1);
		node->set_aaca(new_rec);
		node->get_high_item(aaat1);
		FileMgr::amend();
		
		if (right_node != -1)
			{
			FileMgr::read(right_node);
			node->set_aaba(new_rec);
			FileMgr::amend();
			}
		if (x == 0) 
			{
			node->reset(stNotLeaf);
			node->insert(recno, aaat1);
			node->insert(new_rec, aaat2);
			FileMgr::insert();
			aaaf = aaau;
			DiskMgr::write(fd, &aaau, aaab, sizeof(aaau));
			}
		else
			{
			read(aaai[x - 1]);
			node->find(aaat2);
			node->erase();
			node->insert(recno, aaat1);
			int retval = node->insert(new_rec, aaat2);
			FileMgr::amend();
			if (retval != noFull)
				break;
			}
		}
	delete aaat2;
	delete aaat1;
	delete block2;
	delete block1;
	}

int IndexMgr::insert(long recno, void *aaat)
	{
	lockfile();
	find(aaat);
	switch (node->insert(recno, (char *)aaat))
		{
		case noFull:
			amend();
			node_split();
			unlock();
			return 0;

		case noDuplicate:
			unlock();
			return noDuplicate;
		}
	amend();
	unlock();
	return 0;
	}

int IndexMgr::erase(void)
	{
	if (aaau == -1)
		return -1;
	lockfile();
	if (relocate() == -1)
		{
		unlock();
		return -1;
		}
	long aaca_if_del = node->get_aaca();
	char *aaat = new char[aaaa];
	if (node->get_cur_item(aaat) == NULL)
		{
		delete aaat;
		unlock();
		return -1;
		}
	if (aaaj == -1)
		find(aaat);
	for (int x = aaaj; x >= 0; x--)
		{
		node->get_high_item(aaat);
		switch (node->erase())
			{
			case noNoSelect:
				delete aaat;
				unlock();
				return -1;

			case noEmpty:
				inc_node_del();
				if (x == 0)
					{
					if (!node->isleaf()) 
						{
						node->reset(stLeaf);

						}
					FileMgr::amend();
					delete aaat;
					read(aaca_if_del);
					node->set_aaaz_low();
					unlock();
					return 0;
					}

				FileMgr::erase();
				long aaba, aaca;
				aaba = node->get_aaba();
				aaca = node->get_aaca();
				if (aaba != -1)
					{
					FileMgr::read(aaba);
					node->set_aaca(aaca);
					FileMgr::amend();
					}
				else
					if (node->isleaf()) 
						{
						if (aaca != -1)
							aaag = aaca;
						else
							aaag = aaai[0]; 
						DiskMgr::write(fd, &aaag, aaac, 4);
						}
				if (aaca != -1)
					{
					FileMgr::read(aaca);
					node->set_aaba(aaba);
					FileMgr::amend();
					}
				FileMgr::read(aaai[x - 1]);
				node->find(aaat);
				break;

			default:
				FileMgr::amend();
				if (x == 0 && !node->isleaf() && node->get_aada() == 1)
					{

					aaaf = node->read_ptr(0);
					FileMgr::erase();
					DiskMgr::write(fd, &aaaf, aaab, sizeof(aaaf));
					}
				delete aaat;
				if (x != aaaj)
					{
					read(aaca_if_del);
					node->set_aaaz_low();
					}
				unlock();
				return 0;
			}
		}
	delete aaat;
	unlock();
	return 0;
	}

int IndexMgr::compare(void *aaat)
	{
	char *buffer = new char[aaaa];
	if (node->get_cur_item(buffer) == NULL)
	   {
	   delete buffer;
	   return 0;
	   }

	int retval;
	if (aaal)
		retval = memcmp(aaat, buffer, aaaa);
	else
		retval = memcmp(aaat, buffer, aaaa - 4);
	delete buffer;
	return retval;
	}

long IndexMgr::retrieve(void)
	{
	if (aaau == -1)
		return -1;
	if (relocate() == -1)
		return -1;
	return node->retrieve();
	}

long IndexMgr::next(int no_recs)
	{
	long retval = -1;
	for (; no_recs > 0; no_recs--)
		{
		relocate();
		retval = node->next();
		while (retval == -1)
			{
			aaaj = -1;
			long aaca = node->get_aaca();
			if (aaca == -1)
				return -1;
			read(aaca);
			node->set_aaaz_low();
			retval = node->next();
			}
		}
	return retval;
	}

long IndexMgr::prev(int no_recs)
	{
	long retval = -1;
	for (; no_recs > 0; no_recs--)
		{
		relocate();
		retval = node->prev();
		while (retval == -1)
			{
			aaaj = -1;
			long aaba = node->get_aaba();
			if (aaba == -1)
				return -1;
			read(aaba);
			node->set_aaaz_high();
			retval = node->prev();
			}
		}
	return retval;
	}

void IndexMgr::rew(void)
	{
	read_head_info();
	read(aaag);
	node->set_aaaz_low();
	aaaj = -1;
	}

void IndexMgr::aaan(long recno)
	{
	aaai[++aaaj] = recno;
	read(recno);
	long aaoa = node->aaan();
	if (aaoa != -1 && !node->isleaf())
		aaan(aaoa);
	return;
	}

void IndexMgr::ffwd(void)
	{
	aaaj = -1;
	read_head_info();
	aaan(aaaf);
	}


