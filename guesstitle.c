
/* --------------------------------------------------------------------------
	FILE : 		guesstitle.c
	DESCRIPTION:	implementation modul for guesstitle object
	CREATED BY:	Bigi-boys
	CREATION TIME:	18-04-95 10:50
	MODIFICATION HISTORY:
		Busy	09-05-95 17:38		
		
-----------------------------------------------------------------------------*/	


#include	"guesstitle.h"

#define		MAXLINELEN	60
			
static void SolveCB( Widget widget, XtPointer client_data, XtPointer call_data ) {
	GuessTitlePtr	exercise = (GuessTitlePtr) client_data;
	
	XmToggleButtonSetState( exercise->buttons[exercise->solution-1],
		TRUE, TRUE );
	exercise->value = 0;
}

static void HelpCB( Widget widget, XtPointer client_data, XtPointer call_data ) {
	GuessTitlePtr	exercise = (GuessTitlePtr) client_data;
	int		p,i,j;
	char		prev = ' ';
	Widget		helpmsgbox;
	
	if ( exercise->value!=0 ) {
		p = ( rand() % exercise->value )+1;
		i = j = 0;
		while ( i<p ) {
			if ( exercise->story[j]<0 ) {
				i++;
			}
			j++;
		}
		j--;
		exercise->story[j] *= (-1);
		SetTheText( exercise->textedit, exercise->story );	

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
	GuessTitlePtr	exercise = (GuessTitlePtr) p;
	char		*q;
	char		prev = ' ';
	int		i,j = 0;
	char		word[MAXLINELEN];
	
	q = (StrPtr) XtMalloc( strlen( text )+1 );
	strcpy( q, text );
	

	for ( i=0; i<5; i++ ) {
		j = 0;
		while ( *q!='\n' && *q!='\0' ) {
			word[j++] = *q;
			q++;
		}
		word[j++] = '\0';
		exercise->titles[i] = (StrPtr) XtMalloc( strlen( word )+1 );
		strcpy( exercise->titles[i], word );
		q++;
	}
	
	exercise->solution = *q-'0';
	q++;
	q++;
	
	exercise->value = exercise->rowcount = 0;
	exercise->story = (StrPtr) XtMalloc( strlen( text ) );
	j = 0;
	while ( *q!='\0' ) {		
		if ( IsChar( *q ) && !IsChar( prev ) ) {
			exercise->story[j++] = (*q)*(-1);
			exercise->value += 1;
		} else {
			exercise->story[j++] = *q;
		}
		if ( *q=='\n' ) {
			exercise->rowcount += 1;
		}
		prev = *q;
		q++;
	}
	exercise->story[j] = '\0';
	exercise->maxvalue = exercise->value; 
}

static void Disp( void *p, Widget widget ) {
	GuessTitlePtr	exercise = (GuessTitlePtr) p;
	Widget		solvebutton, helpbutton, buttonpanel, radiobox;
	int		i;

	exercise->guesstitle = XtVaCreateWidget( "guesstitle", xmRowColumnWidgetClass,
		widget, NULL );
		
	XtVaCreateManagedWidget( "label1", xmLabelWidgetClass,
		exercise->guesstitle, NULL );
		
	exercise->textedit = XtVaCreateManagedWidget( "textedit", xmTextWidgetClass,
		exercise->guesstitle,
		XmNcursorPositionVisible, FALSE,
		XmNeditMode, XmMULTI_LINE_EDIT,
		XmNrows, exercise->rowcount,
		XmNcolumns, MAXLINELEN+1,
		XmNmaxLength, MAXLINELEN,
		XmNeditable, FALSE,
		NULL );	
	SetTheText( exercise->textedit, exercise->story );
	
	XtVaCreateManagedWidget( "label2", xmLabelWidgetClass,
		exercise->guesstitle, NULL );
		
	radiobox = XmCreateRadioBox( exercise->guesstitle, "radiobox", NULL, 0 );
	for ( i=0; i<5; i++ ) {
		exercise->buttons[i] = XtVaCreateManagedWidget( exercise->titles[i],
			xmToggleButtonWidgetClass, radiobox, NULL );
	}
	XtManageChild( radiobox );		

	buttonpanel = XtVaCreateManagedWidget( "buttonpanel", xmRowColumnWidgetClass,
		exercise->guesstitle,
		XmNorientation, XmHORIZONTAL,
		NULL );

	solvebutton = XtVaCreateManagedWidget( "solvebutton", xmPushButtonWidgetClass,
		buttonpanel, NULL );
	XtAddCallback( solvebutton, XmNactivateCallback, SolveCB, exercise );

	helpbutton = XtVaCreateManagedWidget( "helpbutton", xmPushButtonWidgetClass,
		buttonpanel, NULL );
	XtAddCallback( helpbutton, XmNactivateCallback, HelpCB, exercise );
	
	XtManageChild( exercise->guesstitle );
}


static void GetValuation( void *p, int *value, int *maxvalue ) {
	GuessTitlePtr	exercise = (GuessTitlePtr) p;
	
	*maxvalue = exercise->maxvalue;
	if ( XmToggleButtonGetState( exercise->buttons[exercise->solution-1] ) ){
		*value = exercise->value;
	} else {
		*value = 0;
	}
}

static void Free( void *p ) {
	GuessTitlePtr	exercise = (GuessTitlePtr) p;
	int		i;
	
	XtDestroyWidget( exercise->guesstitle );	 

	XmStringFree( exercise->story );
	for( i=0; i < 5; ++i ) {
		XmStringFree( exercise->titles[i] );
	} 
}

/* Allocates memory for a new GuessTitle object and sets its function fields */

GuessTitlePtr NewGuessTitle( void ) {
	GuessTitlePtr	p;
		
	p = XtNew( GuessTitle );
	p->create = Create;
	p->display = Disp;
	p->getvaluation = GetValuation;
	p->free = Free;
	return p;
}
