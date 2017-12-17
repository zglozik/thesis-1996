
/* --------------------------------------------------------------------------
	FILE : 		indexedfile.h
	DESCRIPTION:	header file for indexedfile object
	CREATED BY:	Z2
	CREATION TIME:	25-04-95 21:20
	MODIFICATION HISTORY:
		
-----------------------------------------------------------------------------*/	


#ifndef INDEXEDFILEH
#define INDEXEDFILEH

#include	<stdio.h>
#include	<sys/stat.h>
#include	<Xm/Xm.h>

Boolean ExistsFile( char * );

typedef struct {
	long		pos;
	unsigned int	recordlen;
} IndexFileType, *IndexFileTypePtr;

typedef void	(*OpenProc)( void *, char *, int );
typedef char	*(*ReadProc)( void *, long );
typedef void	(*AppendProc)( void *, char *, unsigned int );
typedef long	(*RecordNumProc)( void * );
typedef void	(*CloseProc)( void * );

typedef struct {
	FILE		*file;
	FILE		*indexfile;
	char		mark;
	
	OpenProc	open;
	ReadProc	read;
	AppendProc	append;
	RecordNumProc	recordnum;
	CloseProc	close;
} IndexedFile, *IndexedFilePtr;

IndexedFilePtr NewIndexedFile( void );

#endif
