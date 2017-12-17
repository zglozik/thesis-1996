
/* --------------------------------------------------------------------------
	FILE : 		simpleselect.c
	DESCRIPTION:	implementation module for simpleselect object
	CREATED BY:	Bigi-boys
	CREATION TIME:	18-04-95 10:56
	MODIFICATION HISTORY:
		Busy, Z1	01-05-95 0:30
		Busy		07-05-95 10:35     	
		
-----------------------------------------------------------------------------*/	



#include	"simpleselect.h"

#define	SEPARATOR 	'\n' 
#define MAXLINELEN	255

/* callbacks for buttons */

static void SolveCB( Widget widget, XtPointer client_data, XtPointer call_data ) {
	SimpleSelectPtr	exercise = (SimpleSelectPtr) client_data;
	
	XmToggleButtonSetState( exercise->buttons[exercise->solution-1],
		TRUE, TRUE );
	exercise->value = 0;
}

static void HelpCB( Widget widget, XtPointer client_data, XtPointer call_data ) {
	Widget		helpmsgbox;

	helpmsgbox = XmCreateInformationDialog ( widget, "helpmsgbox", NULL, 0 );
	XtUnmanageChild( XmMessageBoxGetChild( helpmsgbox, XmDIALOG_CANCEL_BUTTON ) );
	XtUnmanageChild( XmMessageBoxGetChild( helpmsgbox, XmDIALOG_HELP_BUTTON ) );

	XtManageChild( helpmsgbox );
	XtPopup( XtParent( helpmsgbox ), XtGrabNone );
}
	
static void Create( void *p, StrPtr text ) {
	SimpleSelectPtr	exercise = (SimpleSelectPtr) p;
	char		*q = text;
	int		i,j;
	char		word[MAXLINELEN];
	
	for ( i=0; i<5; i++ ) {
		j = 0;
		while( *q!=SEPARATOR && *q!='\0')  {
			word[j++] = *q;
			q++;
		}
		word[j++] = '\0';
		if (i==0) {
			exercise->question = (StrPtr) XtMalloc( strlen(word)+1 );			
			strcpy( exercise->question, word);
		} else {
			exercise->answers[i-1] = (StrPtr) XtMalloc( strlen(word)+1 );			
			strcpy( exercise->answers[i-1], word);
		}
		q++;
	}
	exercise->solution = *q-'0';
	exercise->maxvalue = exercise->value = 1;	
}


/* This puts the appropriate widgets onto the exercise dialog */

static void Disp( void *p, Widget widget ) {
	SimpleSelectPtr	exercise = (SimpleSelectPtr) p;
	Widget		solvebutton, helpbutton, buttonpanel;
	int		i;
	
	exercise->simpleselect = XtVaCreateWidget( "simpleselect", xmRowColumnWidgetClass,
		widget, NULL );

	XtVaCreateManagedWidget( "label1", xmLabelWidgetClass,
		exercise->simpleselect, NULL );

	XtVaCreateManagedWidget( "textfield", xmTextFieldWidgetClass, exercise->simpleselect,
		XmNcursorPositionVisible, FALSE,
		XmNeditable, FALSE,
		XmNcolumns, strlen( exercise->question ),	
		XmNvalue, exercise->question,
		NULL );	

	XtVaCreateManagedWidget( "label2", xmLabelWidgetClass,
		exercise->simpleselect, NULL );	

	exercise->radiobox = XmCreateRadioBox( exercise->simpleselect, "radiobox", NULL, 0 );
	for ( i=0; i<4; i++ ) {
		exercise->buttons[i] = XtVaCreateManagedWidget( exercise->answers[i],
			xmToggleButtonWidgetClass, exercise->radiobox, NULL );
	}
	XtManageChild( exercise->radiobox );		

	buttonpanel = XtVaCreateManagedWidget( "buttonpanel", xmRowColumnWidgetClass,
		exercise->simpleselect,
		XmNorientation, XmHORIZONTAL,
		NULL );

	solvebutton = XtVaCreateManagedWidget( "solvebutton", xmPushButtonWidgetClass,
		buttonpanel, NULL );
	XtAddCallback( solvebutton, XmNactivateCallback, SolveCB, exercise );

	helpbutton = XtVaCreateManagedWidget( "helpbutton", xmPushButtonWidgetClass,
		buttonpanel, NULL );
	XtAddCallback( helpbutton, XmNactivateCallback, HelpCB, exercise );
	
	XtManageChild( exercise->simpleselect );
}



static void GetValuation( void *p, int *value, int *maxvalue ){
	SimpleSelectPtr	exercise = (SimpleSelectPtr) p;
	
	
	*maxvalue = exercise->maxvalue;
	if ( XmToggleButtonGetState( exercise->buttons[exercise->solution] ) ){
		*value = exercise->value;
	} else {
		*value = 0;
	}
}

static void Free( void *p ) {
	SimpleSelectPtr	exercise = (SimpleSelectPtr) p;
	int		i;
	
	XtDestroyWidget( exercise->simpleselect );	 

	XmStringFree( exercise->question );
	for( i=0; i < 4; ++i ) {
		XmStringFree( exercise->answers[i] );
	}
}



/* Allocates memory for a new SimpleSelct object and 
sets its function fields */

SimpleSelectPtr NewSimpleSelect( void ) {
	SimpleSelectPtr	p;
	
	p = XtNew( SimpleSelect );
	p->create = Create;
	p->display = Disp;
	p->getvaluation = GetValuation;
	p->free = Free;
	return p;
}
