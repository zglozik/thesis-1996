/* Test Collection object */

#ifndef TESTCOLLECTIONH
#define TESTCOLLECTIONH

#include	<Xm/Xm.h>
#include	<Xm/MessageB.h>

#include	<string.h>
#include	<stdio.h>
#include	"types.h"
#include	"exercise.h"
#include	"indexedfile.h"
#include	"testfile.h"

#include	"unjumble.h"
#include	"simpleselect.h"
#include	"dialoghalves.h"
#include	"wordguess.h"

typedef Boolean		(*CreateTestCollectionProc)( void *, StrPtr );
typedef ExercisePtr	(*GetTestProc)( void * );
typedef Boolean		(*ExistTestProc)( void * );
typedef void		(*FreeTestCollectionProc)( void * );
 
typedef struct {
	IndexedFilePtr	indexedfile;	/* this consists of the test    */
	long		nextrec;

	CreateTestCollectionProc	create;
	GetTestProc			gettest;
	ExistTestProc			existtest;
	FreeTestCollectionProc		free;
} TestCollection, *TestCollectionPtr;

TestCollectionPtr  NewTestCollection( void );

#endif

 
