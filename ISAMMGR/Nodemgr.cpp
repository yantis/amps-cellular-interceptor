// Copyright 1993 Nildram Software. All rights reserved.
// May not be copied, except for backup purposes, without express written
// permission from Nildram Software

#include <stdlib.h>

#include "nodemgr.h"
#include "isammsg.h"


NodeMgr::NodeMgr(int aaar, int itsize)
	{
	if (itsize == 0)
		{
		IsamError("Error: Index Item Size Is Zero!");
		exit(0);
		}
	aaaa = itsize + 4;
	aaay = itsize;
	aaaw = (aaar - 16) / aaaa;

	aaga = new long[aaaw];
	item_aaat = new char *[aaaw];
	for (int x = 0; x < aaaw; x++)
		item_aaat[x] = new char[aaaa];
	aaba = -1;
	aaca = -1;
	aada = 0;
	aaaz = -1;
	aaea = stLeaf;
	}

NodeMgr::~NodeMgr(void)
	{
	for (int x = 0; x < aaaw; x++)
		delete item_aaat[x];
	delete item_aaat;
	delete aaga;
	}

void NodeMgr::read_data(char *buffer)
	{
	char *strptr = buffer;
	aaba = *((long *)strptr);
	strptr += 4;
	aaca = *((long *)strptr);
	strptr += 4;
	aada = *((short *)strptr);
	strptr += 2;
	aaea = *((short *)strptr);
	strptr += 2;
	aafa = *((long *)strptr);
	strptr += 4;
	for (int x = 0; x < aada; x++)
		{
		aaga[x] = *((long *)strptr);
		strptr += 4;
		memcpy(item_aaat[x], strptr, aaay);
		strptr += aaay;
		}
	aaaz = -1;
	}

void NodeMgr::write_data(char *buffer)
	{
	char *strptr = buffer;
	*((long *)strptr) = aaba;
	strptr += 4;
	*((long *)strptr) = aaca;
	strptr += 4;
	*((short *)strptr) = aada;
	strptr += 2;
	*((short *)strptr) = aaea;
	strptr += 2;
	*((long *)strptr) = ++aafa;
	strptr += 4;
	for (int x = 0; x < aada; x++)
		{
		*((long *)strptr) = aaga[x];
		strptr += 4;
		memcpy(strptr, item_aaat[x], aaay);
		strptr += aaay;
		}
	}


int NodeMgr::erase(int itemno)
	{
	if (aada <= 1)
		{
		aada = 0;
		return noEmpty;
		}
	if (itemno == -1)
		itemno = aaaz;
	if (itemno < 0 || itemno >= aada)
		return noNoSelect;
	char *tmpptr = item_aaat[itemno];
	for (int x = itemno + 1; x < aada; x++)
		{
		item_aaat[x - 1] = item_aaat[x];
		aaga[x - 1] = aaga[x];
		}
	item_aaat[aada - 1] = tmpptr;
	aada--;
	if (aaaz >= itemno)
		aaaz--;
	return 0;
	}

int NodeMgr::insert(long ptr, char *aaat)
	{
	int x = 0;
	if (aada > 0)
		{
		x = getpos(aaat, 0, aada - 1);
		if (x != -1)
			{
			if (memcmp(aaat, item_aaat[x], aaay) == 0)
				return noDuplicate;
			}
		else
			x = aada;
		}

	char *tmpptr = item_aaat[aada];
	for (int y = aada; y > x; y--)
		{
		item_aaat[y] = item_aaat[y - 1];
		aaga[y] = aaga[y - 1];
		}
	item_aaat[x] = tmpptr;
	aaga[x] = ptr;
	memcpy(item_aaat[x], aaat, aaay);
	if (++aada == aaaw)
		return noFull;
	return 0;
	}

int NodeMgr::getpos(char *aaat, int start, int stop)
	{
	for (;;)
		{
		int checkpos = (start + stop) / 2;
		int cmp = memcmp(aaat, item_aaat[checkpos], aaay);
		if (cmp == 0)
			return checkpos;
		if (cmp < 0)
			if (checkpos == start)
				return checkpos;
			else
				{
				stop = checkpos;
				continue;
				}
		else
			if (start == stop)
				return -1;
			else
				{
				start = checkpos + 1;
				continue;
				}
		}
	}

long NodeMgr::find(char *aaat)
	{
	if (aada > 0)
		{
		int x = getpos(aaat, 0, aada - 1);
		if (x != -1)
			{
			aaaz = x;
			return aaga[x];
			}
		if (aaea == stLeaf)
			{
			aaaz = aada; 
			return -1;
			}
		aaaz = aada - 1;
		return aaga[aada - 1];
		}
	return -1;
	}

long NodeMgr::aaan(void)
	{
	if (aada > 0)
		{
		if (aaea == stLeaf)
			{
			aaaz = aada; 
			return -1;
			}
		aaaz = aada - 1;
		return aaga[aada - 1];
		}
	return -1;
	}

long NodeMgr::retrieve(void)
	{
	if (aaaz >= aada || aaaz < 0)
		return -1;
	return aaga[aaaz];
	}

long NodeMgr::next(void)
	{
    if (aaaz == aada - 1)
        aaaz++;
    if (aaaz > aada - 1)
        return -1;
    return aaga[++aaaz];
	}

long NodeMgr::prev(void)
	{
    if (aaaz == 0)
        aaaz--;
    if (aaaz < 0)
        return -1;
    return aaga[--aaaz];
	}

void NodeMgr::reset(int st)
	{
	aaba = -1;
	aaca = -1;
	aaaz = -1;
	aada = 0;
	aaea = st;
	aafa = 0;
	}

void NodeMgr::split(char *block1, char *block2)
	{
	int tmp_items = aada;
	aada /= 2;
	write_data(block1);
	aada = tmp_items;
	for (int x = 0; x < tmp_items / 2; x++)
		erase(0);
	write_data(block2);
	}
