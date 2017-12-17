/* --------------------------------------------------------------------------
	FILE : 		memory.c
	DESCRIPTION:	implementation module for memory object
	CREATED BY:	Bigi-boys
	CREATION TIME:	18-04-95 10:52
	MODIFICATION HISTORY:
		Z1, 11-05-95 13:10		
		Z1, 19-05-95 11:25
		Z1, 21-05-95 19:33		
		Z1, 22-05-95 10:20
		
-----------------------------------------------------------------------------*/	


#include	"memory.h"

#define		CARDSEP		'%'		/* separator between cards */

#define		MAXWORD		250
#define		MAXLENGTH	20

#define		STARTX		0
#define		STARTY		0
#define		SCALE		25



	int	turnedcards;	
	int	choosencards[2];
	char	BLANKSTRING [MAXLENGTH] = " ";
	int	numberofcardsstillvisible = 16;

/* exercise string format is: "<expression>%<definition>%...%<expression>%<definition>%" */
	
/* this mixes up the cards */

static  void Mix( int mixedcards[] ) {
	int		a,b,i,temp;
	
	for (i=0; (mixedcards[i]=i) < MAXCARD ; i++);
		

	for( i=0; i<20; ++i ) {
		a = rand() % MAXCARD;
		b = rand() % MAXCARD;
		temp = mixedcards[a];
		mixedcards[a] = mixedcards[b];
		mixedcards[b] = temp;
	}
}
			
static	void CardChoosen( Widget widget, XtPointer client_data, XtPointer call_data ) {
	MemoryPtr	exercise = (MemoryPtr) client_data;
	Widget		msgbox;
	int		i;		
	char		message[10];
	XmString	pressedbuttonlabel;
	
	i = 0;
	while( widget != exercise->cards[i++].cardwidget );
	
	if (( turnedcards == 2 ) || ( ( turnedcards == 1 ) && (choosencards[0] ==  i-1 ) ) ) {
		if ( turnedcards == 2 ) {
			strcpy( message, "manyclick" );
		}
		else {	
 			strcpy( message, "sameclick" );
		}
		msgbox = XmCreateInformationDialog( exercise->memory, message, NULL, 0 );	
		XtUnmanageChild( XmMessageBoxGetChild( msgbox, XmDIALOG_CANCEL_BUTTON ) );
		XtUnmanageChild( XmMessageBoxGetChild( msgbox, XmDIALOG_HELP_BUTTON ) ); 
		XtManageChild( msgbox );
		XtPopup( XtParent( msgbox ), XtGrabNone ); 
		return;
	}
	
			
	XtVaSetValues( exercise->cards[i-1].cardwidget, XmNlabelType, XmSTRING, NULL );
	pressedbuttonlabel = XmStringCreateLocalized( "<selected>" );
	XtVaSetValues( exercise->cards[i-1].cardwidget, XmNlabelString, pressedbuttonlabel, NULL );
	XmStringFree( pressedbuttonlabel );
	choosencards[turnedcards] = i-1;
	XmTextSetString( exercise->textfield[++turnedcards-1], exercise->cards[i-1].string );
	
}
	
static	void NewPairClicked( Widget widget, XtPointer client_data, XtPointer call_data ) {			
	MemoryPtr	exercise = ( MemoryPtr ) client_data;
	int		i;
	Widget		congratulationslabel;
	
	if ( turnedcards == 0 ) return;
	
	if ( choosencards[0] > choosencards[1] ) {
		i = choosencards[1];
		choosencards[1] = choosencards[0];
		choosencards[0] = i;
	}		
	
	if ( ( ( choosencards[0] % 2 ) == 0 ) && ( choosencards[1] == choosencards[0] + 1 ) ) {
		for ( i = 0; i < 2; i++ ) {
			XtUnmanageChild( exercise->cards[choosencards[i]].cardwidget );
			numberofcardsstillvisible-=1;
			XmTextSetString( exercise->textfield[i], ""  );
		}
	}
	else {
		for ( i = 0; i < 2; i++ ) {
			XtVaSetValues( exercise->cards[choosencards[i]].cardwidget,
				 XmNlabelType, XmPIXMAP, NULL );
			XmTextSetString( exercise->textfield[i], ""  );	 
		}
	}
	
	if ( numberofcardsstillvisible == 0 ) {
		congratulationslabel =  XtVaCreateManagedWidget( "conlabel",
			xmLabelWidgetClass, exercise->cardpanel,
			NULL );
	}					
	
	turnedcards = 0;
}		
			

static void Create( void *p, StrPtr text ) {
	MemoryPtr	exercise = (MemoryPtr) p;
	char		*q;
	int		i,j;
	char		word[MAXWORD];
	

	q = (StrPtr) XtMalloc( strlen(text)+1 );	 /* to leave space for spaces */
	strcpy(q,text);
	
	i = 0;
	j = 0;
	while( *q!='\0' ) {
		if( *q ==  CARDSEP ) {
			word[j++] = '\0';
			exercise->cards[i].string = (char *) XtMalloc( j );
			strcpy( exercise->cards[i++].string, word ); 
			j = 0;
		} 
		if( *q != CARDSEP) {
			word[j++] = *q;
		}
		q++;		
	}
}

static void Disp( void *p, Widget widget ) {
	MemoryPtr	exercise = (MemoryPtr) p;
	Widget		falsepanel, answerpanel, buttonpanel;
	Widget		textlabel[2], newpair;
	int		i, j, k;
	char		name[15];
	int		n;
	Arg		args[15];			
	
	exercise->memory = XtVaCreateWidget( "memory", xmRowColumnWidgetClass, widget,
		XmNorientation, XmVERTICAL,
		NULL );
		
	exercise->cardpanel = XtVaCreateManagedWidget( "cardpanel", xmFormWidgetClass, exercise->memory,
		XmNresizePolicy, XmRESIZE_NONE,
		NULL );
		
	answerpanel = XtVaCreateManagedWidget( "answerpanel", xmRowColumnWidgetClass, exercise->memory,
		XmNnumColumns, 2,
		XmNpacking, XmPACK_COLUMN,
		NULL );
		
	buttonpanel = XtVaCreateManagedWidget( "buttonpanel", xmFormWidgetClass, exercise->memory,
		NULL );	
		
	newpair = XtVaCreateManagedWidget( "New Pair", xmPushButtonWidgetClass, buttonpanel,
		XmNleftAttachment, XmATTACH_POSITION,
		XmNleftPosition, 35,
		XmNrightAttachment, XmATTACH_POSITION,
		XmNrightPosition, 65,
		NULL );				

	XtAddCallback( newpair, XmNactivateCallback, NewPairClicked, exercise );  
	
	Mix( exercise->mixedcards );
	
	for ( i=0; i<4; i++) {
		for ( j = 0; j < 4; j++ ) {
			k = exercise->mixedcards[i*4+j];
			sprintf( name, "card%d", k );
			exercise->cards[k].cardwidget =
			XtVaCreateManagedWidget( name, xmPushButtonWidgetClass, exercise->cardpanel,
				XmNlabelType, XmPIXMAP,
				XmNleftAttachment, XmATTACH_POSITION,
				XmNleftPosition,  STARTX + SCALE*i,		
				XmNtopAttachment, XmATTACH_POSITION,
				XmNtopPosition,  STARTY + SCALE*j,		
				XmNrightAttachment, XmATTACH_POSITION,
				XmNrightPosition,  STARTX + SCALE*(i+1),		
				XmNbottomAttachment, XmATTACH_POSITION,
				XmNbottomPosition,  STARTY + SCALE*(j+1),
				XmNrecomputeSize, FALSE,
				NULL );
			
			XtAddCallback( exercise->cards[k].cardwidget, XmNactivateCallback,
				CardChoosen, exercise ); 
		}		
		
	}	
	
	textlabel[0] = 	XtVaCreateManagedWidget( "textlabel0", xmLabelWidgetClass, answerpanel, NULL );
	
	textlabel[1] = 	XtVaCreateManagedWidget( "textlabel1", xmLabelWidgetClass, answerpanel, NULL );
	
	
	for ( i=0; i < 2; i++ ) {
		
		n = 0;	
		XtSetArg( args[n], XmNcursorPositionVisible, FALSE );	 	n++;
		XtSetArg( args[n], XmNeditMode, XmSINGLE_LINE_EDIT );	 	n++;
		XtSetArg( args[n], XmNscrollBarDisplayPolicy, XmAS_NEEDED );	n++;
		XtSetArg( args[n], XmNcolumns, MAXLENGTH );		 	n++;
		XtSetArg( args[n], XmNscrollHorizontal, TRUE );		 	n++;	
		XtSetArg( args[n], XmNeditable, FALSE );		 	n++;
		XtSetArg( args[n], XmNvalue, "" );			 	n++;
		XtSetArg( args[n], XmNleftAttachment, XmATTACH_POSITION );	n++;
		XtSetArg( args[n], XmNleftPosition, 20 );			n++;
		
		if ( i == 0 ) {
			XtSetArg( args[n], XmNtopAttachment, XmATTACH_FORM );	 	n++;
		}
		else {
			XtSetArg( args[n], XmNtopAttachment, XmATTACH_WIDGET );	 	n++;
			XtSetArg( args[n], XmNtopWidget, exercise->textfield[0] );	n++;
		}	
		
		sprintf( name, "textfield%d", i );	
		exercise->textfield[i] = XmCreateScrolledText( answerpanel, name, args, n );
	
		XtManageChild( exercise->textfield[i] );
	}	
	
		
	XtManageChild( exercise->memory );
}

static void GetValuation( void *p, int *value, int *maxvalue ) {
}

static void Free( void *p ) {
	MemoryPtr	exercise = p;
	
	XtDestroyWidget( exercise->memory );	
}

/* Allocates memory for a new Memory object and sets its function fields */

MemoryPtr NewMemory( void ) {
	MemoryPtr	p;
		
	p = XtNew( Memory );
	p->create = Create;
	p->display = Disp;
	p->getvaluation = GetValuation;
	p->free = Free;
	p->cardchoosen = (ChooseCardProc) CardChoosen;
	return p;
}
