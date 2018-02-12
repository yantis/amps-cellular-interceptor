// Copyright 1993 Nildram Software. All rights reserved.
// May not be copied, except for backup purposes, without express written
// permission from Nildram Software

#include "isammsg.h"

#include <stdio.h>

void IsamProgress(long recno)
	{
    char bs[9] = { 8, 8, 8, 8, 8, 8, 8, 8, 0 };
    fprintf(stderr, "%s%8ld", bs, recno);
	}

void IsamError(const char *errmsg)
	{
	fprintf(stderr, "%s\n", errmsg);
    }

void IsamInitProgress(const char *fname, const char *idx)
	 {
	 fprintf(stderr, "Rebuilding Index File: \"%s\", Name: \"%s\"\nRecords Processed:         ", fname, idx);
     }

void IsamEndProgress(void)
     {
     fprintf(stderr, "\n");
     }

