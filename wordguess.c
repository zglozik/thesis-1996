
/* --------------------------------------------------------------------------
	FILE : 		wordguess.c
	DESCRIPTION:	implementation modul for worguess object
	CREATED BY:	Bigi-boys
	CREATION TIME:	18-04-95 11:01
	MODIFICATION HISTORY:
		Busy	09-05-95 11:48		
		
-----------------------------------------------------------------------------*/	



#include	"wordguess.h"

#define		MAXLINELEN	50
#define		ENDOFINFO	'$'

static void SolveCB( Widget widget, XtPointer client_data, XtPointer call_data ){
	WordGuessPtr	exercise = (WordGuessPtr) client_data;
	Arg		arg[1];
	int		n = 0;
	
	XtSetArg( arg[n], XmNvalue, exercise->word ); n++;
	XtSetValues( exercise->textfield, arg, n );
	exercise->value = 0;
}

static void HelpCB( Widget widget, XtPointer client_data, XtPointer call_data ){
	WordGuessPtr	exercise = (WordGuessPtr) client_data;
	Widget		helpmsgbox;
	
	if ( exercise->value!=0 ) {
		XmTextInsert( exercise->textedit, (exercise->maxvalue-exercise->value+1)*MAXLINELEN, 
			exercise->information[exercise->maxvalue-exercise->value+1] );
		exercise->value -= 1;
	} else {
		helpmsgbox = XmCreateInformationDialog ( widget, "helpmsgbox", NULL, 0 );
		XtUnmanageChild( XmMessageBoxGetChild( helpmsgbox, XmDIALOG_CANCEL_BUTTON ) );
		XtUnmanageChild( XmMessageBoxGetChild( helpmsgbox, XmDIALOG_HELP_BUTTON ) );

		XtManageChild( helpmsgbox );
		XtPopup( XtParent( helpmsgbox ), XtGrabNone );
	}
}


static void Create( void *p, StrPtr text ) {
	WordGuessPtr	exercise = (WordGuessPtr) p;
	char		*q;
	char		word[MAXLINELEN];
	int		i,j;
	
	q = (StrPtr) XtMalloc( strlen( text )+1 ); /* to leave space for spaces */
	strcpy( q, text );
	
	for ( i=0; *q!=ENDOFINFO; i++ ) {
		j = 0;
		while ( *q!='\n' && *q!=ENDOFINFO && *q!='\0' ) {
			word[j++] = *q;
			q++;
		}
		if ( i==0 ) {
			word[j++] = '\0';
			exercise->word = (StrPtr) XtMalloc( strlen( word )+1 );
			strcpy( exercise->word, word );
		} else {
			word[j++] = '\n';
			word[j++] = '\0';	
			exercise->information[i-1] = (StrPtr) XtMalloc( strlen( word )+1);
			strcpy( exercise->information[i-1], word );
		}
		q++;
	}
	
	exercise->maxvalue = exercise->value = i-2;
}

static void Disp( void *p, Widget widget ) {
	WordGuessPtr	exercise = (WordGuessPtr) p;
	Widget		solvebutton, helpbutton, buttonpanel, scrolledtext;
	Arg		args[10];
	int		n = 0;
	
	exercise->wordguess = XtVaCreateWidget( "wordguess", xmRowColumnWidgetClass,
		widget, NULL );
     
	XtVaCreateManagedWidget( "label1", xmLabelWidgetClass,
		exercise->wordguess, NULL );
	
	XtSetArg( args[n], XmNcursorPositionVisible, FALSE );	 n++;
	XtSetArg( args[n], XmNeditMode, XmMULTI_LINE_EDIT );	 n++;
	XtSetArg( args[n], XmNrows, 4 );			 n++;
	XtSetArg( args[n], XmNscrollVertical, TRUE );		 n++;
	XtSetArg( args[n], XmNscrollHorizontal, FALSE );	 n++;
	XtSetArg( args[n], XmNscrollBarDisplayPolicy, XmSTATIC );n++;
	XtSetArg( args[n], XmNcolumns, MAXLINELEN+1 );		 n++;
	XtSetArg( args[n], XmNmaxLength, MAXLINELEN );		 n++;
	XtSetArg( args[n], XmNeditable, FALSE );		 n++;
	XtSetArg( args[n], XmNvalue, exercise->information[0] ); n++;

	exercise->textedit = XmCreateScrolledText( exercise->wordguess, "textedit", args, n );
	
	XtManageChild( exercise->textedit );
		
	XtVaCreateManagedWidget( "label2", xmLabelWidgetClass,
		exercise->wordguess, NULL );

	exercise->textfield = XtVaCreateManagedWidget( "textfield", xmTextFieldWidgetClass, 
		exercise->wordguess,
		XmNcolumns, MAXLINELEN+1,
		NULL );

	buttonpanel = XtVaCreateManagedWidget( "buttonpanel", xmRowColumnWidgetClass,
		exercise->wordguess,
		XmNorientation, XmHORIZONTAL,
		NULL );

	solvebutton = XtVaCreateManagedWidget( "solvebutton", xmPushButtonWidgetClass,
		buttonpanel, NULL );
	XtAddCallback( solvebutton, XmNactivateCallback, SolveCB, exercise );

	helpbutton = XtVaCreateManagedWidget( "helpbutton", xmPushButtonWidgetClass,
		buttonpanel, NULL );
	XtAddCallback( helpbutton, XmNactivateCallback, HelpCB, exercise );
	
	XtManageChild( exercise->wordguess );
}
	

static void GetValuation( void *p, int *value, int *maxvalue ) {
	WordGuessPtr	exercise = (WordGuessPtr) p;
	char		*q;
	
	q = XmTextFieldGetString( exercise->textfield );
	
	*maxvalue = exercise->maxvalue;
	if ( strcmp( q, exercise->word ) == 0 ) {
		*value = exercise->value;
	} else {
		*value = 0;
	}
}

static void Free( void *p ) {
	WordGuessPtr	exercise = (WordGuessPtr) p;
	int	i;
	
	XtDestroyWidget( exercise->wordguess );
	
	for ( i=0; i<exercise->maxvalue+1; i++ ) {
		XmStringFree( exercise->information[i] );
	}
	XmStringFree( exercise->word );
	
}


/* Allocates memory for a new WordGuess object and sets its function fields */

WordGuessPtr NewWordGuess( void ) {
	WordGuessPtr	p;
		
	p = XtNew( WordGuess );
	p->create = Create;
	p->display = Disp;
	p->getvaluation = GetValuation;
	p->free = Free;
	return p;
}
