
/* --------------------------------------------------------------------------
	FILE : 		unjumble.c
	DESCRIPTION:	implementation modul for unjumble object
	CREATED BY:	Bigi-boys
	CREATION TIME:	18-04-95 10:59
	MODIFICATION HISTORY:
		Z2, 24-04-95 12:05
		
-----------------------------------------------------------------------------*/	



#include	"unjumble.h"

/* It initializes an unjumble object, sets its variable fields, ... */

static void Create( void *p, StrPtr text ) {
	UnjumblePtr	exercise = (UnjumblePtr) p;
	char		word[MAXWORDLEN+1];
	int		wordlen;
	char		*q;
	int		a,b;

	exercise->sentence = (StrPtr) XtMalloc( strlen(text)+2 );
	strcpy( exercise->sentence, " " );
	strcat( exercise->sentence, text );
	q = exercise->sentence;

	exercise->wordcount = 0;
	while( *q==' ' && *q!='\0' ) {
		++q;
	}
	while( *q!='\0' ) {
		for( wordlen = 0; *q!=' ' && *q!='\0' && wordlen<MAXWORDLEN;
			++q ) {
			word[wordlen++] = *q;
		}
		word[wordlen] = '\0';
		exercise->words[exercise->wordcount++] =
			XmStringCreateLocalized( word );
		while( *q==' ' && *q!='\0' ) {
			++q;
		}
	}
	exercise->maxvalue = exercise->wordcount;
	exercise->value = exercise->wordcount;
}

/* This callback is invoked when the user double clicks a word in the list box */

static void SelectWordCB( Widget widget, XtPointer client_data, XtPointer call_data ) {
	UnjumblePtr		exercise = (UnjumblePtr) client_data;
	XmListCallbackStruct	*msg = (XmListCallbackStruct *) call_data;
	char			*sentence;
	char			*word;
	char			temp[(MAXWORDLEN+1)*MAXWORDCOUNT+1];
	
	XmStringGetLtoR( msg->item, XmFONTLIST_DEFAULT_TAG, &word );
	XmListDeleteItemsPos( exercise->listbox, 1, msg->item_position );
	sentence = XmTextFieldGetString( exercise->textfield );
	
	strcpy( temp, sentence );
	strcat( temp, " " );
	strcat( temp, word );
	XtFree( sentence );
	XmTextFieldSetString( exercise->textfield, temp );
}

/* This function mixes the words. */

static void MixWords( XmString *words, XmString *oldwords, int count ) {
	int		a,b,i;
	XmString	temp;

	memcpy( words, oldwords, sizeof(XmString)*count );
	for( i=0; i<20; ++i ) {
		a = rand() % count;
		b = rand() % count;
		temp = words[a];
		words[a] = words[b];
		words[b] = temp;
	}
}

/* This is the callback for the REMOVE button */

static void RemoveCB( Widget widget, XtPointer client_data, XtPointer call_data ) {
	UnjumblePtr	exercise = (UnjumblePtr) client_data;
	XmString	newwords[MAXWORDCOUNT];

	XmTextFieldSetString( exercise->textfield, "" );
	MixWords( newwords, exercise->words, exercise->wordcount );
	XtVaSetValues( exercise->listbox,
		XmNitemCount, exercise->wordcount,
		XmNitems, newwords,
		NULL );
}


/* This is the callback for the SOLVE button */

static void SolveCB( Widget widget, XtPointer client_data, XtPointer call_data ) {
	UnjumblePtr	exercise = (UnjumblePtr) client_data;
	
	exercise->value = 0;
	XmTextFieldSetString( exercise->textfield, exercise->sentence );
	XtVaSetValues( exercise->listbox,
		XmNitemCount, 0,
		XmNitems, NULL,
		NULL );
}


/* This function is invoked when OK BUTTON is pressed on the help dialog box */

static void HelpOkCB( Widget widget, XtPointer client_data, XtPointer call_data ) {
	UnjumblePtr	exercise = (UnjumblePtr) client_data;
	
	XtDestroyWidget( widget );
}

/* This is the callback for the HELP button */

static void HelpCB( Widget widget, XtPointer client_data, XtPointer call_data ) {
	UnjumblePtr	exercise = (UnjumblePtr) client_data;
	Widget		unjumblehelpdlgbox, rc;
	XmString	str;
	char		buffer[10];
	int		index;
	
	if( exercise->value>0 ) {
		--exercise->value;
	}

	unjumblehelpdlgbox = XmCreateTemplateDialog( XtParent(widget),
		"unjumblehelpdlgbox", NULL, 0);
	XtAddCallback( XmMessageBoxGetChild( unjumblehelpdlgbox,
		XmDIALOG_OK_BUTTON ), XmNactivateCallback, HelpOkCB, exercise );

	rc = XtVaCreateManagedWidget( "rowcolumn", xmRowColumnWidgetClass,
		unjumblehelpdlgbox,
		XmNpacking, XmPACK_COLUMN,
		XmNorientation, XmHORIZONTAL,
		XmNnumColumns, 2,
		NULL );

	index = rand() % exercise->wordcount;
	XtVaCreateManagedWidget( "wordlabel", xmLabelWidgetClass,
		rc, NULL );
	XtVaCreateManagedWidget( NULL, xmLabelWidgetClass,
		rc,
		XmNlabelString, exercise->words[index],
		NULL );

	sprintf( buffer, "%d", index + 1 );
	str = XmStringCreateLocalized( buffer );
	XtVaCreateManagedWidget( "positionlabel", xmLabelWidgetClass,
		rc, NULL );
	XtVaCreateManagedWidget( NULL, xmLabelWidgetClass,
		rc,
		XmNlabelString, str,
		NULL );
	XmStringFree( str );
	
	XtManageChild( unjumblehelpdlgbox );
}


/* It puts the appropriate widgets onto the exercise dialog */

static void Disp( void *p, Widget widget ) {
	UnjumblePtr	exercise = (UnjumblePtr) p;
	Widget		removebutton, solvebutton, helpbutton, buttonpanel;
	XmString	newwords[MAXWORDCOUNT];

	exercise->unjumble = XtVaCreateWidget( "unjumble", xmRowColumnWidgetClass,
		widget, NULL );

	XtVaCreateManagedWidget( "label1", xmLabelWidgetClass,
		exercise->unjumble, NULL );
	
	MixWords( newwords, exercise->words, exercise->wordcount );
	exercise->listbox = XtVaCreateManagedWidget( "listbox", xmListWidgetClass,
		exercise->unjumble,
		XmNitemCount, exercise->wordcount,
		XmNitems, newwords,
		XmNvisibleItemCount, exercise->wordcount,
		NULL );
	XtAddCallback( exercise->listbox, XmNdefaultActionCallback, SelectWordCB, exercise );

	XtVaCreateManagedWidget( "label2", xmLabelWidgetClass,
		exercise->unjumble, NULL );

	exercise->textfield = XtVaCreateManagedWidget( "textfield",
		xmTextFieldWidgetClass, exercise->unjumble,
		XmNeditable, False,
		XmNcursorPositionVisible, False,
		XmNcolumns, strlen( exercise->sentence ),
		NULL );

	buttonpanel = XtVaCreateManagedWidget( "buttonpanel", xmRowColumnWidgetClass,
		exercise->unjumble,
		XmNorientation, XmHORIZONTAL,
		NULL );

	removebutton = XtVaCreateManagedWidget( "removebutton", xmPushButtonWidgetClass,
		buttonpanel, NULL );
	XtAddCallback( removebutton, XmNactivateCallback, RemoveCB, exercise );

	solvebutton = XtVaCreateManagedWidget( "solvebutton", xmPushButtonWidgetClass,
		buttonpanel, NULL );
	XtAddCallback( solvebutton, XmNactivateCallback, SolveCB, exercise );

	helpbutton = XtVaCreateManagedWidget( "helpbutton", xmPushButtonWidgetClass,
		buttonpanel, NULL );
	XtAddCallback( helpbutton, XmNactivateCallback, HelpCB, exercise );
	
	XtManageChild( exercise->unjumble );
}


/* This function resturns the maxvalue and current value in the unjumble. */

static void GetValuation( void *p, int *value, int *maxvalue ) {
	UnjumblePtr	exercise = (UnjumblePtr) p;
	char		*buffer;
	
	buffer = XmTextFieldGetString( exercise->textfield );
	*value = strcmp( exercise->sentence, buffer ) == 0 ? exercise->value : 0;
	XtFree( buffer );
	*maxvalue = exercise->maxvalue;
}

/* This function is operated for deallocating the unjumble object. */

static void Free( void *p ) {
	UnjumblePtr	exercise = p;
	int		i;
	
	XtDestroyWidget( exercise->unjumble );	 

	XtFree( exercise->sentence );
	for( i=0; i < exercise->wordcount; ++i ) {
		XmStringFree( exercise->words[i] );
	}
	XtFree( (char *) exercise );
}

/* Allocates memory for a new Unumble object and sets its function fields */

UnjumblePtr NewUnjumble( void ) {
	UnjumblePtr	p;
	
	p = XtNew( Unjumble );
	p->create = Create;
	p->display = Disp;
	p->getvaluation = GetValuation;
	p->free = Free;
	return p;
}
