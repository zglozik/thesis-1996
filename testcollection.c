
/* testcollection.c implementation module */

#include	"testcollection.h"


/* The first record of the testcollection file consists of the type of
   the test in text format. for example: "unjumble" means it is a test
   of unjumble exercises */


/* This converts a text into ExerciseType */

static Boolean GetExerciseType( TestCollectionPtr testc,
	StrPtr text, ExerciseTypePtr exercisetype ) {
	extern StrPtr	testnames[];
	int		i;
	
	for( i=0; i<TESTNUMBER && strcmp( testnames[i], text ); i++ ) ;
	*exercisetype = i;
	return i<TESTNUMBER;
}


/* This initializes a new testcollection, opens file, and so on. */

static Boolean Create( void *p, StrPtr filename ) {
	TestCollectionPtr	testc = (TestCollectionPtr) p;
	extern Widget		mainwindow;
	Widget			collectionerrordlgbox;
	
	if( !ExistsFile( filename ) ) {
		collectionerrordlgbox = XmCreateErrorDialog( mainwindow,
			"testnofiledlgbox", NULL, 0 );
          	XtUnmanageChild( XmMessageBoxGetChild( collectionerrordlgbox,
			XmDIALOG_CANCEL_BUTTON ) );
          	XtUnmanageChild( XmMessageBoxGetChild( collectionerrordlgbox,
			XmDIALOG_HELP_BUTTON ) );

		XtManageChild( collectionerrordlgbox );
		XtFree( (char *) testc );
		
		return FALSE;
	}
	if( !TestFile( filename ) ) {
		collectionerrordlgbox = XmCreateErrorDialog( mainwindow,
			"testbadfiledlgbox", NULL, 0 );
          	XtUnmanageChild( XmMessageBoxGetChild( collectionerrordlgbox,
			XmDIALOG_CANCEL_BUTTON ) );
          	XtUnmanageChild( XmMessageBoxGetChild( collectionerrordlgbox,
			XmDIALOG_HELP_BUTTON ) );

		XtManageChild( collectionerrordlgbox );
		XtFree( (char *) testc );
		return FALSE;
	}
	testc->indexedfile = NewIndexedFile();
	testc->indexedfile->open( testc->indexedfile, filename, ENDMARK );
	testc->nextrec = 0;
	
	return TRUE;	
}


/* This creates an appropriate exercise from the test file. */

static ExercisePtr GetTest( void *p ) {
	TestCollectionPtr	testc = (TestCollectionPtr) p;
	char			*buffer;
	ExercisePtr		exercise;
	ExerciseType		exercisetype;
	
	buffer = testc->indexedfile->read( testc->indexedfile,
		testc->nextrec++ );
	if( !GetExerciseType( testc, buffer, &exercisetype ) ) {
		fprintf( stderr, "Bad test file.\n" );
		exit( 1 );
	}
	XtFree( buffer );
	
	switch( exercisetype ) {
		case unjumble:
			exercise = (ExercisePtr) NewUnjumble();
			break;
		case simpleselect:
			exercise = (ExercisePtr) NewSimpleSelect();
			break;
		case dialoghalves:
			exercise = (ExercisePtr) NewDialogHalves();
			break;
		case wordguess:
			exercise = (ExercisePtr) NewWordGuess();
			break;
	}
	buffer = testc->indexedfile->read( testc->indexedfile,
		testc->nextrec++ );
	exercise->create( exercise, buffer );
	XtFree( buffer );
	
	return exercise;
}

/* This will give whether there is some exercise left or not. */

static Boolean ExistTest( void *p ) {
	TestCollectionPtr	testc = (TestCollectionPtr) p;

	return testc->indexedfile->recordnum( testc->indexedfile ) >
		testc->nextrec;
}

/* This will free the object and its fields. */

static void Free( void *p ) {
	TestCollectionPtr	testc = (TestCollectionPtr) p;

	testc->indexedfile->close( testc->indexedfile );
	XtFree( (char *) testc->indexedfile );
	XtFree( (char *) testc );
}


/* This allocates memory for a testcollection object and sets its
   function fields. */

TestCollectionPtr NewTestCollection( void ) {
	TestCollectionPtr	p;
	
	p = XtNew( TestCollection );
	p->create = Create;
	p->gettest = GetTest;
	p->existtest = ExistTest;
	p->free = Free;
	return p;
}
