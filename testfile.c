
/* testfile.c implementation module */


#include	"testfile.h"

#define MAXTYPELEN	30

char	*testnames[TESTNUMBER] = { "unjumble", "simpleselect",
				   "dialoghalves", "wordguess" };


/* This will give whether the file given is test file or not. */

Boolean TestFile( char *filename ) {
	extern char	*testnames[];
	FILE		*file;
	char		name[MAXTYPELEN+1];
	char		*p;
	int		i;

	if( (file=fopen( filename, "r" ))==NULL ) {
		return FALSE;
	}
	p = name;
	i=fgetc( file );
	while( i!=EOF && i!=ENDMARK && p-name<MAXTYPELEN ) {
		*p++ = i;
		i=fgetc( file );
	}
	fclose( file );

	*p = '\0';
	for( i=0; i<TESTNUMBER && strcmp( testnames[i], name ); i++ ) ;
	return i<TESTNUMBER;
}

