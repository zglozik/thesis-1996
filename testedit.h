/* Test Edit object */

#ifndef TESTEDITH
#define TESTEDITH

#include	<string.h>
#include	<Xm/Xm.h>
#include	<Xm/FileSB.h>
#include	<Xm/ToggleB.h>
#include	<Xm/Label.h>
#include	<Xm/RowColumn.h>
#include	<Xm/Form.h>
#include	<Xm/MessageB.h>
#include	<Xm/Text.h>

#include	"types.h"
#include	"exercise.h"
#include	"indexedfile.h"
#include	"testfile.h"

typedef void	(*CreateTestEditProc)( void * );
typedef void	(*SelectTestEditFileProc)( Widget, XtPointer, XtPointer );
typedef void	(*CancelTestEditFileProc)( Widget, XtPointer, XtPointer );
typedef void	(*StoreTestProc)( Widget, XtPointer, XtPointer );
typedef void	(*CancelTestEditProc)( Widget widget, XtPointer, XtPointer );
typedef void	(*FreeTestEditProc)( void * );

typedef struct {
	IndexedFilePtr		testfile;
	Widget			testeditdlgbox;
	Widget			text;		/* This involves the exercise. */
	Widget			typebuttons[TESTNUMBER];

	CreateTestEditProc  	create;
	
	SelectTestEditFileProc	selecttesteditfile;
	CancelTestEditFileProc	canceltesteditfile;

	StoreTestProc	    	storetest;
	CancelTestEditProc	canceltestedit;
	
	FreeTestEditProc	free;
} TestEdit, *TestEditPtr;

TestEditPtr  NewTestEdit( void );

#endif                       	 
