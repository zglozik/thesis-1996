
/* --------------------------------------------------------------------------
	FILE : 		storyboard.c
	DESCRIPTION:	implementation modul for storyboard object
	CREATED BY:	Bigi-boys
	CREATION TIME:	18-04-95 10:58
	MODIFICATION HISTORY:
		Busy	12-05-95 11:55
		
-----------------------------------------------------------------------------*/	



#include	"storyboard.h"

#define		MAXLINELEN	60
#define		NOVISIBLE	'%'

static void SolveCB( Widget widget, XtPointer client_data, XtPointer call_data ) {
	StoryboardPtr	exercise = (StoryboardPtr) client_data;
	int		i = 0;
	
	while ( exercise->story[i]!='\0' ) {
		if ( exercise->story[i]<0 ) {
			exercise->story[i] *= (-1);
		}
		i++;
	}
	SetTheText( exercise->textedit, exercise->story );
	exercise->value = 0;
	exercise->wordcount =0;
}

static void HelpCB( Widget widget, XtPointer client_data, XtPointer call_data ) {
	StoryboardPtr	exercise = (StoryboardPtr) client_data;
	int		p,i,j;
	char		prev = ' ';
	Widget		helpmsgbox;
	
	if ( exercise->wordcount!=0 ) {
		p = ( rand() % exercise->wordcount )+1;
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

		exercise->wordcount -=1;
	} else {
		helpmsgbox = XmCreateInformationDialog ( widget, "helpmsgbox", NULL, 0 );
		XtUnmanageChild( XmMessageBoxGetChild( helpmsgbox, XmDIALOG_CANCEL_BUTTON ) );
		XtUnmanageChild( XmMessageBoxGetChild( helpmsgbox, XmDIALOG_HELP_BUTTON ) );

		XtManageChild( helpmsgbox );
		XtPopup( XtParent( helpmsgbox ), XtGrabNone );
	}

}

static void ApplyCB( Widget widget, XtPointer client_data, XtPointer call_data ) {
	StoryboardPtr	exercise = (StoryboardPtr) client_data;
	int		i = 0;
	char		*q;
	Widget		applymsgbox, nomoreapplymsgbox;
	
	if ( exercise->wordcount!=0 ) {
		q = XmTextFieldGetString( exercise->textfield );
		q[0] *= (-1);
	
		if ( strlen( q ) > (size_t) 0 ) {
			while ( i>=0 ) {
				i = 0;	
				i = FindStrInStr( q, exercise->story ); 	
		
				if ( i>=0  ) {
					exercise->story[i] *= (-1);
					exercise->wordcount -= 1;
					exercise->value += 1;
					SetTheText( exercise->textedit, exercise->story );
				}
			}
		} else {
			applymsgbox = XmCreateInformationDialog ( widget, "applymsgbox", NULL, 0 );
			XtUnmanageChild( XmMessageBoxGetChild( applymsgbox, XmDIALOG_CANCEL_BUTTON ) );
			XtUnmanageChild( XmMessageBoxGetChild( applymsgbox, XmDIALOG_HELP_BUTTON ) );
	
			XtManageChild( applymsgbox );
			XtPopup( XtParent( applymsgbox ), XtGrabNone );	
		}
		
		XmStringFree( q );		
	} else {
		nomoreapplymsgbox = XmCreateInformationDialog ( widget, "nomoreapplymsgbox", NULL, 0 );
		XtUnmanageChild( XmMessageBoxGetChild( nomoreapplymsgbox, XmDIALOG_CANCEL_BUTTON ) );
		XtUnmanageChild( XmMessageBoxGetChild( nomoreapplymsgbox, XmDIALOG_HELP_BUTTON ) );
			
		XtManageChild( nomoreapplymsgbox );
		XtPopup( XtParent( nomoreapplymsgbox ), XtGrabNone );	
	}	
	
	XmTextFieldSetString( exercise->textfield, "" );
}


static void Create( void *p, StrPtr text ) {
	StoryboardPtr	exercise = (StoryboardPtr) p;
	char		*q;
	char		prev = ' ';
	int		i = 0;
	
	q = (StrPtr) XtMalloc( strlen( text )+1 );
	strcpy( q, text );
	exercise->story = (StrPtr) XtMalloc( strlen( text ) );
	exercise->rowcount = 0;
	exercise->wordcount = 0;
	
	while ( *q!='\0' ) {
		if ( *q==NOVISIBLE ) {
			q++;
			while ( *q!=NOVISIBLE ) {
				if ( IsChar( *q ) && !IsChar( prev ) ) {
					exercise->story[i++] = (*q)*(-1);
					exercise->wordcount += 1;
				} else {
					exercise->story[i++] = *q;
				}
				if ( *q=='\n' ) {
					exercise->rowcount += 1;
				}
				prev = *q;
				q++;
			}
			q++;
		} else {
			exercise->story[i++] = *q;
			if ( *q=='\n' ) {
				exercise->rowcount += 1;
			}
			prev = *q;
			q++;
		}
	}
	exercise->story[i] = '\0';
	exercise->maxvalue = exercise->wordcount;
	exercise->value = 0; 
}

static void Disp( void *p, Widget widget ) {
	StoryboardPtr	exercise = (StoryboardPtr) p;
	Widget		solvebutton, helpbutton, applybutton, buttonpanel;
	
	exercise->storyboard = XtVaCreateWidget( "storyboard", xmRowColumnWidgetClass,
		widget, NULL );
		
	XtVaCreateManagedWidget( "label1", xmLabelWidgetClass,
		exercise->storyboard, NULL );
	
	exercise->textedit = XtVaCreateManagedWidget( "textedit", xmTextWidgetClass,
		exercise->storyboard,
		XmNcursorPositionVisible, FALSE,
		XmNeditMode, XmMULTI_LINE_EDIT,
		XmNrows, exercise->rowcount+1,
		XmNcolumns, MAXLINELEN+1,
		XmNmaxLength, MAXLINELEN,
		XmNeditable, FALSE,
		NULL );	
	SetTheText( exercise->textedit, exercise->story );
	
	XtVaCreateManagedWidget( "label2", xmLabelWidgetClass,
		exercise->storyboard, NULL );

	exercise->textfield = XtVaCreateManagedWidget( "textfield", xmTextFieldWidgetClass,
		exercise->storyboard,
		XmNcursorPositionVisible, TRUE,
		XmNcolumns, MAXLINELEN+1,
		XmNmaxLength, MAXLINELEN,
		XmNeditable, TRUE,
		NULL );
					
	buttonpanel = XtVaCreateManagedWidget( "buttonpanel", xmRowColumnWidgetClass,
		exercise->storyboard,
		XmNorientation, XmHORIZONTAL,
		NULL );
	
	solvebutton = XtVaCreateManagedWidget( "solvebutton", xmPushButtonWidgetClass,
		buttonpanel, NULL );
	XtAddCallback( solvebutton, XmNactivateCallback, SolveCB, exercise );

	helpbutton = XtVaCreateManagedWidget( "helpbutton", xmPushButtonWidgetClass,
		buttonpanel, NULL );
	XtAddCallback( helpbutton, XmNactivateCallback, HelpCB, exercise );

	applybutton = XtVaCreateManagedWidget( "applybutton", xmPushButtonWidgetClass,
		buttonpanel, NULL );
	XtAddCallback( applybutton, XmNactivateCallback, ApplyCB, exercise );

	XtManageChild( exercise->storyboard );	

}

static void GetValuation( void *p, int *value, int *maxvalue ) {
	StoryboardPtr	exercise = (StoryboardPtr) p;
	
	*maxvalue = exercise->maxvalue;
	*value = exercise->value;
}

static void Free( void *p ) {
	StoryboardPtr	exercise = (StoryboardPtr) p;
	
	XtDestroyWidget( exercise->storyboard );
	
	XmStringFree( exercise->story );

	XtFree( (char *) exercise );
}



/* Allocates memory for a new Storyboard object and sets its function fields */

StoryboardPtr NewStoryboard( void ) {
	StoryboardPtr	p;
		
	p = XtNew( Storyboard );
	p->create = Create;
	p->display = Disp;
	p->getvaluation = GetValuation;
	p->free = Free;
	return p;
}
