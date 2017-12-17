
/* --------------------------------------------------------------------------
	FILE : 		indexedfile.c
	DESCRIPTION:	implementation modul for indexedfile object
	CREATED BY:	Z2
	CREATION TIME:	24-04-95 21:25
	MODIFICATION HISTORY:
		
-----------------------------------------------------------------------------*/	


#include	"indexedfile.h"

/* it is the length of a record in the index file */

#define	INDEXRECORDLEN	(sizeof(long)+sizeof(int))

#define	MAXFILENAMELEN	120


/* This will give whether a file exists or not. */

Boolean ExistsFile( char *filename ) {
	FILE	*file;
	
	if( (file = fopen( filename, "r" ))==NULL ) {
		return FALSE;
	} else {
		fclose( file );
		return TRUE;
	}
}

/* it creates a new index file and fills it perfectly */

static CreateIndexFile( IndexedFilePtr indexedfile, char  *filename ) {
	FILE		*indexfile;
	IndexFileType	indexrec;
	int		ch;

	if( (indexfile=fopen( filename, "w" ))==NULL ) {
		fprintf( stderr, "I can't create index file: %s\n", filename );
		exit( 1 );
	}
	indexrec.recordlen = 0;
	indexrec.pos = 0;
	rewind( indexedfile->file );
	while( (ch=fgetc(indexedfile->file))!=EOF ) {
		if( ch==indexedfile->mark ) {
			while( (ch=fgetc(indexedfile->file))!='\n' ) ;
			fwrite( &indexrec, sizeof(indexrec), 1, indexfile );
			indexrec.recordlen = 0;
			indexrec.pos = ftell( indexedfile->file );
		} else {
			++indexrec.recordlen;
		}
	}
	fclose( indexfile );
}

/* this procedure opens a file with its index file, and creates it if it doesn't exist */

static void Open( void *p, char *name, int mark ) {
	IndexedFilePtr	indexedfile = (IndexedFilePtr) p;
	char		filename[MAXFILENAMELEN];
	struct stat	buf, bufndx;
	Boolean		error;
	
	if( (indexedfile->file=fopen(name,"a+"))==NULL ) {
		fprintf( stderr, "I can't open file: %s\n", name );
		exit( 1 );
	}
	indexedfile->mark = mark;
	stat( name, &buf );
	strcpy( filename, name );
	strcat( filename, ".ndx" );
	error = stat( filename, &bufndx )==-1;
	if( error || (buf.st_mtime>bufndx.st_mtime) ) {
		CreateIndexFile( indexedfile, filename );
	}
	if( (indexedfile->indexfile=fopen(filename, "r+"))==NULL ) {
		fprintf( stderr, "I can't open index file: %s\n", filename );
		exit( 1 );
	}
}

/* it gives the size of the file */

static long RecordNum( void *p ) {
	IndexedFilePtr	indexedfile = (IndexedFilePtr) p;

	fseek( indexedfile->indexfile, 0, SEEK_END );
	return ftell( indexedfile->indexfile ) / INDEXRECORDLEN;
}

/* it reads a record from the file */

static char *Read( void *p, long pos ) {
	IndexedFilePtr	indexedfile = (IndexedFilePtr) p;
	IndexFileType	indexrec;
	char		*buffer;
	
	if( pos>=indexedfile->recordnum( indexedfile ) ) {
		fprintf( stderr, "Bad index file or bad position.\n" );
		exit( 1 );
	}
	fseek( indexedfile->indexfile, pos*INDEXRECORDLEN, SEEK_SET );
	fread( &indexrec, sizeof(indexrec), 1, indexedfile->indexfile );
	fseek( indexedfile->file, indexrec.pos, SEEK_SET );
	buffer = (char *) XtMalloc( indexrec.recordlen+1 );
	fread( buffer, indexrec.recordlen, 1, indexedfile->file );
	buffer[indexrec.recordlen] = '\0';
	return buffer;
}

/* this writes a record to the end of a file */

static void Append( void *p, char *buffer, unsigned int buflen ) {
	IndexedFilePtr	indexedfile = (IndexedFilePtr) p;
	IndexFileType	indexrec;
	
	fseek( indexedfile->file, 0, SEEK_END );
	indexrec.pos = ftell( indexedfile->file );
	indexrec.recordlen = buflen;
	fseek( indexedfile->indexfile, 0, SEEK_END );
	fwrite( &indexrec, sizeof(indexrec), 1, indexedfile->indexfile );
	fwrite( buffer, buflen, 1, indexedfile->file );
	fprintf( indexedfile->file, "%c\n", indexedfile->mark );
}	


/* it closes the file */

static void Close( void *p ) {
	IndexedFilePtr	indexedfile = (IndexedFilePtr) p;

	fclose( indexedfile->file );
	fclose( indexedfile->indexfile );
}

IndexedFilePtr NewIndexedFile( void ) {
	IndexedFilePtr	p;
	
	p = (IndexedFilePtr) XtMalloc( sizeof(IndexedFile) );
	p->open = Open;
	p->read = Read;
	p->append = Append;
	p->recordnum = RecordNum;
	p->close = Close;
	return p;
}
