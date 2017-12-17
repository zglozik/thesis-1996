/* --------------------------------------------------------------------------
	FILE : 		lt.c
	DESCRIPTION:	main modul for Language Tutor
	CREATED BY:	Z1
	CREATION TIME:	11-04-95 17:00
	MODIFICATION HISTORY:
		Z2, 21-04-95 11:15
		
-----------------------------------------------------------------------------*/	

#include	"lt.h"

int		level = 0; /* This global variable consists of the difficulty level (0-2) */
ExerciseCPtr	exercisec; /* This global variable consists of an Exercise Collection
				object */

Widget	mainwindow; /* This variable consists of the Main Window Widget */

/* Callbacks for Practice menu item */

void ChooseExercise( ExerciseType exercisetype ) {
	Widget			exercisedlgbox;
	ExercisesPtr		exercises;
	Widget			dlgbox;
	extern ExistExercise	existexercise;

	if( existexercise[ exercisetype ] ) {
		return;
	}
	if( !exercisec->existexercise( exercisec, exercisetype, level ) ) {
		dlgbox = XmCreateErrorDialog( mainwindow, "noexercise_dlgbox", NULL, 0 );
		XtUnmanageChild( XmMessageBoxGetChild( dlgbox, XmDIALOG_CANCEL_BUTTON ) );
		XtUnmanageChild( XmMessageBoxGetChild( dlgbox, XmDIALOG_HELP_BUTTON ) );
		XtManageChild( dlgbox );
		return;
	}
	exercises = NewExercises();
	exercises->create( exercises, exercisetype, level );
}
		
/* Callbacks for Set Level menu item */

void SetLevelOkCB( Widget widget, XtPointer client_data, XtPointer call_data ) {
	Widget	*button = (Widget *) client_data;
	Boolean	l;
	int	i;
	
	i = -1;
	l = False;
	while (!l) {
		++i;
		XtVaGetValues( button[i], XmNset, &l,
			NULL );
	};
	level = i;
}
	
void SetLevel( void ) {
	static Widget	leveldlgbox = NULL;
	static Widget	button[3];
	char		name[20];
	Widget		rb;
	int		i;
	
	if (!leveldlgbox) {
		leveldlgbox = XmCreateTemplateDialog( mainwindow, "leveldlgbox",
			NULL, 0 );
		XtAddCallback( leveldlgbox,
			XmNokCallback, SetLevelOkCB, (XtPointer) button );
		
		rb = XmCreateRadioBox( leveldlgbox, "radiobox", NULL, 0 );

		for( i=0; i<3; ++i ) {
			sprintf( name, "toggle_%d", i );
			button[i] = XtVaCreateManagedWidget( name,
				xmToggleButtonWidgetClass, rb,
				NULL );
		}
		XtManageChild( rb );
	}
	for( i=0; i<3; ++i ) {
		XtVaSetValues( button[i], XmNset, level==i,
			NULL );
	}
	XtManageChild( leveldlgbox );
	XtPopup( XtParent(leveldlgbox), XtGrabNone );
}

/* Callbacks for Practice menu item */

void PracticeMenuCB( Widget widget, XtPointer client_data, XtPointer call_data ) {
	int	item = (int) client_data;

	switch (item) {
		case 0 : 
		case 1 :
		case 2 :
		case 3 :
		case 4 :
		case 5 :
		case 6 :
		case 7 :
			ChooseExercise( (ExerciseType) item );
			break;
		case 8 :
			SetLevel();
			break;
	}
}

/* Callbacks for About menu item */

void About( void ) {
	Widget aboutdlgbox;

	aboutdlgbox = XmCreateInformationDialog( mainwindow, "aboutdlgbox", 
		NULL, 0 );
	XtUnmanageChild( XmMessageBoxGetChild(aboutdlgbox, XmDIALOG_CANCEL_BUTTON) );
	XtUnmanageChild( XmMessageBoxGetChild(aboutdlgbox, XmDIALOG_HELP_BUTTON) );
	
	XtManageChild( aboutdlgbox );
	XtPopup( XtParent( aboutdlgbox ), XtGrabNone );			
}

/* Callbacks for Quit menu item */

void QuitMenuCB( Widget widget, XtPointer client_data, XtPointer call_data ) {
	int	item = (int) client_data;
	
	switch( item ) {
		case 0:
			exit(0);
			break;
		case 1:
			About();
			break;
	}
}

/* Callbacks for Help menu item */

void Help( void ) {
	Widget helpdlgbox;

	helpdlgbox = XmCreateInformationDialog( mainwindow, "helpdlgbox", 
		NULL, 0 );
	XtUnmanageChild( XmMessageBoxGetChild(helpdlgbox, XmDIALOG_CANCEL_BUTTON) );
	XtUnmanageChild( XmMessageBoxGetChild(helpdlgbox, XmDIALOG_HELP_BUTTON) );
	
	XtManageChild( helpdlgbox );
	XtPopup( XtParent( helpdlgbox ), XtGrabNone );
}
		

/* Callbacks for Test Menu item */

void TestMenuCB( Widget widget, XtPointer client_data, XtPointer call_data ) {
	int		item = (int) client_data;
	DoTestPtr	dotest;
	TestEditPtr	testedit;

	switch( item ) {
		case 0:
			dotest = NewDoTest();
			dotest->create( dotest );
			break;
		case 1:
			testedit = NewTestEdit();
			testedit->create( testedit );
			break;
	}
}

/* Main procedure of the language tutor */

main( int argc, char **args ) {
	Widget		toplevel, mainmenu, button;
	Widget		menu[3];
	XtAppContext	app;
	char	mainmenunames[][20] = { "test", "practice", "quit" };
	char	testmenunames[][20] = { "dotest", "edittest" };
	char	practicemenunames[][20] = { "unjumble", "simpleselect", "dialoghalves",
			"wordguess", "nougthsandcrosses", "storyboard", "memory",
			"guesstitle", "setlevel" };
	char	quitmenunames[][20] = { "quit", "about" };
	int	i, width, height;

	/* It initializes global variables */
	exercisec = NewExerciseC();

	/* It creates the environment */
	toplevel = XtVaAppInitialize( &app, "LanguageTutor", NULL,
			0, &argc, args, NULL, NULL );

	mainwindow = XtVaCreateManagedWidget( "mainwindow", 
		xmMainWindowWidgetClass, toplevel, NULL );
	
	XtVaCreateManagedWidget( "mainlabel", xmLabelWidgetClass, mainwindow,
		XmNlabelType, XmPIXMAP,
		NULL );

	mainmenu = XmCreateMenuBar( mainwindow, "mainmenu", NULL, 0 );
	
	menu[0] = XmCreatePulldownMenu( mainmenu, "testmenu", NULL, 0 );
	for( i=0; i<2; i++ ) {
		button = XtVaCreateManagedWidget( testmenunames[i], xmPushButtonWidgetClass,
			menu[0], NULL );
		XtAddCallback( button, XmNactivateCallback, TestMenuCB, (XtPointer) i );
	}
	
	menu[1] = XmCreatePulldownMenu( mainmenu, "practicemenu", NULL, 0 );
	for( i=0; i<9; i++ ) {
		button = XtVaCreateManagedWidget( practicemenunames[i], xmPushButtonWidgetClass,
			menu[1], NULL );
		XtAddCallback( button, XmNactivateCallback, PracticeMenuCB,
			(XtPointer) i );
		if( i==7 ) {
			XtVaCreateManagedWidget( NULL, xmSeparatorWidgetClass,
				menu[1], NULL );
		}
	}
	
	menu[2] = XmCreatePulldownMenu( mainmenu, "quitmenu", NULL, 0 );
	for( i=0; i<2; i++ ) {
		button = XtVaCreateManagedWidget( quitmenunames[i], xmPushButtonWidgetClass,
			menu[2], NULL );
		XtAddCallback( button, XmNactivateCallback, QuitMenuCB, (XtPointer) i );
	}

	for( i=0; i<3; i++ ) {
		XtVaCreateManagedWidget( mainmenunames[i], xmCascadeButtonWidgetClass,
			mainmenu,
			XmNsubMenuId, menu[i],
			NULL );
	}
	
	XtManageChild( mainmenu );
	XtRealizeWidget( toplevel );

	XtVaGetValues( toplevel,
		XmNwidth, &width,
		XmNheight, &height,
		NULL );
	XtVaSetValues( toplevel,
		XmNmaxWidth, width,
		XmNminWidth, width,
		XmNmaxHeight, height,
		XmNminHeight, height,
		NULL );

	XtAppMainLoop( app );
}
