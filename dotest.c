
/* dotest.c implementation module */

#include	"dotest.h"

/* This is true when dotest is selected. */

static Boolean	selected = False;

/* This initializes a new dotest object, makes a dialog for
   selecting a test file. */

static void Create( void *p ) {
	DoTestPtr	dotest = (DoTestPtr) p;
	Widget		testselectiondlgbox;
	extern Widget	mainwindow;
	
	if( selected ) {
		XtFree( (char *) dotest );
		return;
	}
	selected = True;
	
	testselectiondlgbox = XmCreateFileSelectionDialog( mainwindow,
		"testselectiondlgbox", NULL, 0 );
	XtAddCallback( testselectiondlgbox, XmNokCallback,
		dotest->selecttestfile, dotest );
	XtAddCallback( testselectiondlgbox, XmNcancelCallback,
		dotest->canceltestfile, dotest );	
	XtUnmanageChild( XmFileSelectionBoxGetChild( testselectiondlgbox,
		XmDIALOG_HELP_BUTTON ) );
	
	XtManageChild( testselectiondlgbox );
}

/* This is invoked when thu user presses the Select test button */

static void SelectTestFile( Widget widget, XtPointer client_data,
	XtPointer call_data ) {
	DoTestPtr	dotest = (DoTestPtr) client_data;
	XmFileSelectionBoxCallbackStruct	*cbs =
			(XmFileSelectionBoxCallbackStruct *) call_data;
	char		*filename;
	extern Widget	mainwindow;
	
	XtDestroyWidget( XtParent(widget) );
	XmStringGetLtoR( cbs->value, XmFONTLIST_DEFAULT_TAG, &filename );
	dotest->testcollection = NewTestCollection();
	if( !dotest->testcollection->create( dotest->testcollection, filename ) ) {
		dotest->free( dotest );
		return;
	}
	
	dotest->valuation = NewValuation();
	dotest->valuation->create( dotest->valuation );

	dotest->dotestdlgbox = XmCreateTemplateDialog( mainwindow, "dotestdlgbox",
		NULL, 0 );

	XtAddCallback( dotest->dotestdlgbox, XmNokCallback,
		dotest->nexttestexercise, dotest );
	XtAddCallback( dotest->dotestdlgbox, XmNcancelCallback,
		dotest->canceltestexercise, dotest );

	dotest->form = XtVaCreateManagedWidget( "form", xmFormWidgetClass,
		dotest->dotestdlgbox, NULL );

	dotest->exercise = dotest->testcollection->gettest( dotest->testcollection );
	dotest->exercise->display( dotest->exercise, dotest->form );
	
	XtManageChild( dotest->dotestdlgbox );
}

/* This is invoked when the user presses the cancel button. */

static void CancelTestFile( Widget widget, XtPointer client_data,
	XtPointer call_data ) {
	DoTestPtr	dotest = (DoTestPtr) client_data;
	
	XtDestroyWidget( XtParent(widget) );
	dotest->free( dotest );	
}

/* This is invoked when the user solved a test exercise and wants to see the next. */

static void NextTestExercise( Widget widget, XtPointer client_data,
	XtPointer call_data ) {
	DoTestPtr	dotest = (DoTestPtr) client_data;
	int		value, maxvalue;
	
	dotest->exercise->getvaluation( dotest->exercise, &value, &maxvalue );
	dotest->valuation->freshvaluation( dotest->valuation, value, maxvalue );
	dotest->exercise->free( dotest->exercise );

	if( dotest->testcollection->existtest( dotest->testcollection ) ) {
		dotest->exercise = dotest->testcollection->gettest( dotest->testcollection );
		dotest->exercise->display( dotest->exercise, dotest->form );
		XtManageChild( dotest->dotestdlgbox );
	} else {
		dotest->testcollection->free( dotest->testcollection );
		dotest->valuation->dispvaluation( dotest->valuation );
		dotest->free( dotest );
	}	
}

/* This is invoked when the user interrupted the solving of the test. */

static void CancelTestExercise( Widget widget, XtPointer client_data,
	XtPointer call_data ) {
	DoTestPtr	dotest = (DoTestPtr) client_data;

	XtDestroyWidget( dotest->dotestdlgbox );
	
	dotest->exercise->free( dotest->exercise );
	XtFree( (char *) dotest->valuation );
	dotest->testcollection->free( dotest->testcollection );
	dotest->free( dotest );
}


/* This deallocates memory reserved for the dotest object. */

static void Free( void *p ) {
	DoTestPtr	dotest = (DoTestPtr) p;
	
	XtFree( (char *) dotest );
	selected = False;
}

/* This allocates memory for a new dotest object. */

DoTestPtr NewDoTest( void ) {
	DoTestPtr	p;
	
	p = XtNew( DoTest );
	p->create = Create;
	p->selecttestfile = SelectTestFile;
	p->canceltestfile = CancelTestFile;
	p->nexttestexercise = NextTestExercise;
	p->canceltestexercise = CancelTestExercise;
	p->free = Free;
	return p;
}
