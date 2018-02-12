// ISAMMSG.H - Isam Message Function Header

#ifndef ISAMMSG_H
#define ISAMMSG_H

void IsamProgress(long recno);
void IsamError(const char *errmsg);
void IsamInitProgress(const char *fname, const char *idx);
void IsamEndProgress(void);

#endif
