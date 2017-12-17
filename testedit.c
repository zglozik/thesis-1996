
/* testedit.c implementation module */

#include	"testedit.h"

static ExerciseType	exercisetype = unjumble;

/* This is true when testedit is selected. */

static Boolean	selected = False;

/* This creates a file dialog for asking the name of the test file. */

static void Create( void *p ) {
	TestEditPtr	testedit = (TestEditPtr) p;
	Widget		testselectiondlgbox;
	extern Widget	mainwindow;
	
	if( selected ) {
		XtFree( (char *) testedit );
		return;
	}
	selected = True;
	
	testselectiondlgbox = XmCreateFileSelectionDialog( mainwindow,
		"testselectiondlgbox", NULL, 0 );
	XtAddCallback( testselectiondlgbox, XmNokCallback,
		testedit->selecttesteditfile, testedit );
	XtAddCallback( testselectiondlgbox, XmNcancelCallback,
		testedit->canceltesteditfile, testedit );	
	XtUnmanageChild( XmFileSelectionBoxGetChild( testselectiondlgbox,
		XmDIALOG_HELP_BUTTON ) );
	
	XtManageChild( testselectiondlgbox );
}

/* This is invoked when the user selected a test file. */

static void SelectTestEditFile( Widget widget, XtPointer client_data,
	XtPointer call_data ) {
	TestEditPtr	testedit = (TestEditPtr) client_data;
	Widget		rc, labelform, editform, radiobox, testeditbadfiledlgbox;
	XmString	str;
	Arg		args[15];
	int		n = 0;
	extern Widget	mainwindow;
	extern char	*testnames[];
	int		i;
	XmFileSelectionBoxCallbackStruct	*cbs =
			(XmFileSelectionBoxCallbackStruct *) call_data;
	char		*filename;
	char		buffer[128];
	
	XtDestroyWidget( XtParent(widget) );
	XmStringGetLtoR( cbs->value, XmFONTLIST_DEFAULT_TAG, &filename );
	if( ExistsFile( filename ) && !TestFile( filename ) ) {
		XtFree( filename );
		testeditbadfiledlgbox = XmCreateErrorDialog( mainwindow,
			"testbadfiledlgbox", NULL, 0 );
		XtUnmanageChild( XmMessageBoxGetChild( testeditbadfiledlgbox,
			XmDIALOG_CANCEL_BUTTON ) );
		XtUnmanageChild( XmMessageBoxGetChild( testeditbadfiledlgbox,
			XmDIALOG_HELP_BUTTON ) );
		XtManageChild( testeditbadfiledlgbox );
		
		testedit->free( testedit );
		return;
	}
	testedit->testfile = NewIndexedFile();
	testedit->testfile->open( testedit->testfile, filename, ENDMARK );

	testedit->testeditdlgbox = XmCreateTemplateDialog( mainwindow,
		"testeditdlgbox", NULL, 0 );
		
	XtRemoveAllCallbacks( XmMessageBoxGetChild(
		testedit->testeditdlgbox, XmDIALOG_OK_BUTTON ),
		XmNactivateCallback );
	XtAddCallback( XmMessageBoxGetChild(
		testedit->testeditdlgbox, XmDIALOG_OK_BUTTON ),
		XmNactivateCallback, testedit->storetest, testedit );
	XtAddCallback( testedit->testeditdlgbox, XmNcancelCallback,
		testedit->canceltestedit, testedit );

	rc = XtVaCreateManagedWidget( "rowcolumn", xmRowColumnWidgetClass,
		testedit->testeditdlgbox, NULL );

	sprintf( buffer, "File: %s", filename );
	XtFree( filename );
	str = XmStringCreateLocalized( buffer );
	XtVaCreateManagedWidget( NULL, xmLabelWidgetClass, rc,
		XmNlabelString, str,
		NULL );
	XmStringFree( str );
	
	labelform = XtVaCreateManagedWidget( "labelform", xmFormWidgetClass,
		rc, NULL );
		
	XtVaCreateManagedWidget( "editlabel", xmLabelWidgetClass,
		labelform,
		XmNalignment, XmALIGNMENT_BEGINNING,
		XmNleftAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_POSITION,
		XmNrightPosition, 70,
		NULL );
		
	XtVaCreateManagedWidget( "typelabel", xmLabelWidgetClass,
		labelform,
		XmNalignment, XmALIGNMENT_BEGINNING,
		XmNrightAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_POSITION,
		XmNleftPosition, 70,
		NULL );
	
	editform = XtVaCreateManagedWidget( "editform", xmFormWidgetClass,
		rc, NULL );
		
	XtSetArg( args[n], XmNeditMode, XmMULTI_LINE_EDIT ); n++;
	XtSetArg( args[n], XmNrows, 8 ); n++;
	XtSetArg( args[n], XmNcolumns, 40 ); n++;
	XtSetArg( args[n], XmNleftAttachment, XmATTACH_FORM ); n++;
	XtSetArg( args[n], XmNtopAttachment, XmATTACH_FORM ); n++;
	XtSetArg( args[n], XmNbottomAttachment, XmATTACH_FORM ); n++;
	XtSetArg( args[n], XmNrightAttachment, XmATTACH_POSITION ); n++;
	XtSetArg( args[n], XmNrightPosition, 70 ); n++;
	
	testedit->text = XmCreateScrolledText( editform, "text", args, n );
	XtManageChild( testedit->text );

	radiobox = XtVaCreateManagedWidget( "radiobox", xmRowColumnWidgetClass,
		editform,
		XmNradioBehavior, True,
		XmNpacking, XmPACK_COLUMN,
		XmNrightAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_POSITION,
		XmNleftPosition, 70,
		NULL );
	
	for( i=0; i<TESTNUMBER; i++ ) {
		testedit->typebuttons[i] = XtVaCreateManagedWidget(
			testnames[i], xmToggleButtonWidgetClass, radiobox,
			XmNset, i==exercisetype,
			NULL );
	}
	XtVaSetValues( testedit->testeditdlgbox, XmNinitialFocus, rc, NULL );
	XtVaSetValues( rc, XmNinitialFocus, editform, NULL );
	XtVaSetValues( editform, XmNinitialFocus, XtParent(testedit->text), NULL );
	
	XtManageChild( testedit->testeditdlgbox );
}

/* This is invoked when the user cancelled the file dialog. */

static void CancelTestEditFile( Widget widget, XtPointer client_data,
	XtPointer call_data ) {
	TestEditPtr	testedit = (TestEditPtr) client_data;

	XtDestroyWidget( XtParent(widget) );
	testedit->free( testedit );
}

/* This is invoked when the user wants to save his test exercise. */

static void StoreTest( Widget widget, XtPointer client_data,
	XtPointer call_data ) {
	TestEditPtr	testedit = (TestEditPtr) client_data;
	char		*buffer;
	int		i;
	Boolean		l;
	extern char	*testnames[];
	
	buffer = XmTextGetString( testedit->text );
	if( buffer[0]!='\0' ) {
		for( i=0, l=False; i<TESTNUMBER && !l; i++ ) {
			XtVaGetValues( testedit->typebuttons[i],
				XmNset, &l, NULL );
		}
		exercisetype = i-1;

		testedit->testfile->append( testedit->testfile,
			testnames[exercisetype], strlen(testnames[exercisetype]) );
		testedit->testfile->append( testedit->testfile,
			buffer, strlen(buffer) );
	}
	XtFree( buffer );
	
	XmTextSetString( testedit->text, "" );
}

/* This is invoked when there is no more exercise. */

static void CancelTestEdit( Widget widget, XtPointer client_data,
	XtPointer call_data ) {
	TestEditPtr	testedit = (TestEditPtr) client_data;

	XtDestroyWidget( testedit->testeditdlgbox );
	testedit->testfile->close( testedit->testfile );
	testedit->free( testedit );
}


/* This deallocates memory reserved for testedit object. */

static void Free( void *p ) {
	TestEditPtr	testedit = (TestEditPtr) p;
	
	XtFree( (char *) testedit );
	selected = False;
}

/* This allocates memory for a new testedit object. */

TestEditPtr NewTestEdit( void ) {
	TestEditPtr	p;
	
	p = XtNew( TestEdit );
	p->create = Create;
	p->selecttesteditfile = SelectTestEditFile;
	p->canceltesteditfile = CancelTestEditFile;
	p->storetest = StoreTest;
	p->canceltestedit = CancelTestEdit;
	p->free = Free;
	return p;
}
