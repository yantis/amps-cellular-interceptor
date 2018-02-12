#ifndef FSCREEN_H
#define FSCREEN_H

#include <string.h>
#include <ctype.h>

void FSputa(int, int, int, int);
void FSputs(const char *, int, int, int);
unsigned FSgetxy(int, int);
void FSinit(void);
void FSbox(int, int, int, int, int, int);
void FSgotopos(int, int);
void FSgetpos(int *, int *);
void FSsetstyle(int, int);
void FSgetstyle(int *, int *);
int  FSreadscr(int, int, int, int);
int  FSwritescr(int, int, int, int);
int  FSopwin(int, int, int, int, int, int);
int  FSclwin(void);
void FSscroll(int, int, int, int, int, int);
int  FSmenu(int, int, int, char *[], int, int);

int  FSinputs(char *string, int attr, int startrow, int startcol, int length,
            int forceflag = 0, char *contents = "^");
int  FSinputl(long &value, int attr, int startrow, int startcol,
            int length = 11, long minval = -2147483648L, long maxval = 2147483647L);
int  FSinputul(unsigned long &value, int attr, int startrow, int startcol,
            int length = 10, unsigned long minval = 0, unsigned long maxval = 4294967295UL);
int  FSinputi(int &value, int attr, int startrow, int startcol,
            int length = 6, int minval = -32767, int maxval = 32767);
int FSinputui(unsigned int &value, int attr, int startrow, int startcol,
            int length = 5, unsigned int minval = 0, unsigned int maxval = 65535U);
int FSinputc(char &value, int attr, int startrow, int startcol,
            int length = 4, char minval = -128, char maxval = 127);
int FSinputuc(unsigned char &value, int attr, int startrow, int startcol,
            int length = 3, unsigned char minval = 0, unsigned char maxval = 255);

void FSputl(long value, int attr, int row, int col);
void FSputul(unsigned long value, int attr, int row, int col);
void FSputi(int value, int attr, int row, int col);
void FSputui(unsigned int value, int attr, int row, int col);
#define FSputc(a, b, c, d) FSputi(a, b, c, d)
#define FSputuc(a, b, c, d) FSputui(a, b, c, d)

int  FSbarmenu(int, int, int, char *[], int, int, int);
int  FSyesno(int, int, int);
int  FSgetch(void);
int FSprompt(char *, int, int);
void FSsavescr(void);
void FSrestorescr(void);
int FSgetmode(void);

inline void FStitle(const char *a, char b, int c)
    {
    FSputs(a, b, c, 39 - strlen(a) / 2);
    }

inline void FSclrbox(int toprow, int topcol, int botrow, int botcol, int attrib)
    {
    FSscroll(toprow, topcol, botrow, botcol, 0, attrib);
    }

inline void FSclrline(int row, int attrib)
    {
    FSscroll(row, 0, row, 79, 0, attrib);
    }

inline void FSclrscr(int attrib)
    {
    FSscroll(0, 0, 24, 79, 0, attrib);
    }

#ifndef BLACK
#define BLACK 0
#endif

#ifndef BLUE
#define BLUE 1
#endif

#ifndef GREEN
#define GREEN 2
#endif

#ifndef CYAN
#define CYAN 3
#endif

#ifndef RED
#define RED 4
#endif

#ifndef MAGENTA
#define MAGENTA 5
#endif

#ifndef BROWN
#define BROWN 6
#endif

#ifndef LIGHTGREY
#define LIGHTGREY 7
#endif

#ifndef LIGHTGRAY
#define LIGHTGRAY 7
#endif

#ifndef DARKGREY
#define DARKGREY 8
#endif

#ifndef DARKGRAY
#define DARKGRAY 8
#endif

#ifndef LIGHTBLUE
#define LIGHTBLUE 9
#endif

#ifndef LIGHTGREEN
#define LIGHTGREEN 10
#endif

#ifndef LIGHTCYAN
#define LIGHTCYAN 11
#endif

#ifndef LIGHTRED
#define LIGHTRED 12
#endif

#ifndef LIGHTMAGENTA
#define LIGHTMAGENTA 13
#endif

#ifndef YELLOW
#define YELLOW 14
#endif

#ifndef WHITE
#define WHITE 15
#endif

#ifndef BLINK
#define BLINK 0x80
#endif

#define FS_CURSORUP 1
#define FS_CURSORDOWN 2
#define FS_BACKSPACE 3
#define FS_ENTER 4
#define FS_ESCAPE 5
#define FS_PGUP 6
#define FS_PGDN 7

#define TOUPPER 1
#define TOLOWER 2

#define ALNUM "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890 -"
#define ALPHA "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm "
#define NUM "1234567890 -"
#define UNUM "1234567890 "
#define ALL "^"

/* The following are for FSbox styles */

#define HSVS 0
#define HDVD 1
#define HDVS 2
#define HSVD 3

/* For FSbarmenu */

#define FS_CENTRE (-1)
#define FS_CENTER (-1)

#endif
