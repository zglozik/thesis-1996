
/* --------------------------------------------------------------------------
	FILE : 		nougthandcrosses.c
	DESCRIPTION:	implementation modul for nougthandcrosses object
	CREATED BY:	Bigi-boys
	CREATION TIME:	18-04-95 10:54
	MODIFICATION HISTORY:
		Z1, 26-05-95		
		
-----------------------------------------------------------------------------*/	


#include	"nougthsandcrosses.h"

#define	MAXLINESIZE	255	/* the maximum length of a line in the data file. */
#define MAXLENGTH	30

#define		STARTX		10
#define		STARTY		10
#define		WIDTH		58
#define		HEIGHT		69
#define		SPACE		5

/* The format of the text is:
	int number		<- this is the right answer of the first simple select
	question		<- this is the sentence of the test
	first answer
	second answer
	third answer
	fourth answer
	int number
	.
	.
	.			<- this must be continued sixteen times
*/


/* This reads an integer value from a string. */

static StrPtr	getinteger( StrPtr text, int *x ) {

	*x = 0;
	while( !isdigit( *text ) ) text++;
	while( isdigit( *text ) ) {
		*x = *x * 10 + *text - '0';
		text++;
	}
	while( *text++!='\n' );
	
	return text;
}

/* This reads a line from string. */

static StrPtr	getline( StrPtr text, StrPtr *line ) {
	char	buf[MAXLINESIZE+1];
	int	size = 0;

	while( *text!='\n' ) {
		buf[size++] = *text++;
	}
	buf[size++] = '\0';
	*line = XtMalloc( size );
	strcpy( *line, buf );

	return ++text;
}


/* This initializes a new nougthsandcrosses object, sets its fields, etc... */

static void Create( void *p, StrPtr text ) {
	NougthsandCrossesPtr	exercise = (NougthsandCrossesPtr) p;
	int			e,s;
	
	exercise->nougthtomove = TRUE;
	exercise->istherequestion = FALSE;
	strcpy( exercise->remarkstring[0], "Player with crosses to move." );
	strcpy( exercise->remarkstring[1], "Player with nougths to move." );

	exercise->value = 0;
	exercise->maxvalue = SELECTNUMBER;

	for( e=0; e<SELECTNUMBER; e++ ) {
		exercise->table[ e/FIELDSIZE ][ e%FIELDSIZE ].state = empty;
		text = getinteger( text,
			&exercise->table[ e/FIELDSIZE ][ e%FIELDSIZE ].solution );
		text = getline( text,
			&exercise->table[ e/FIELDSIZE ][ e%FIELDSIZE ].question );
		
		for( s=0; s<ANSWERNUMBER; s++ ) {
			text = getline( text,
				&exercise->table[ e/FIELDSIZE ][ e%FIELDSIZE ].text[ s ] );
		}
	}
}

/* It will check whether there are four figures in the correct place or not. */

static Boolean CheckDirection( NougthsandCrossesPtr exercise, ButtonState who,
	int sx, int sy, int addx, int addy ) {
	int	count;
	Boolean	correct;
	
	for( count = 0, correct = TRUE; count<4 && sx<FIELDSIZE && sy<FIELDSIZE &&
		correct; sx += addx, sy += addy, count++ ) {
		correct = exercise->table[sx][sy].state == who;
	}
	return correct && count==4;
}

/* It will check whether someone wins or not. */

static Boolean Win( NougthsandCrossesPtr exercise, ButtonState who ) {
	int	i,j;
	Boolean	found;

	for( i = 0, found = False; i<FIELDSIZE && !found; i++ ) {
		for( j = 0; j<FIELDSIZE && !found; j++ ) {
			found = CheckDirection( exercise, who, i, j, +1, 0 );
			found = found || CheckDirection( exercise, who, i, j, +1, +1 );
			found = found || CheckDirection( exercise, who, i, j, 0, +1 );
		}
	}

	return found;
}

/* It will show the winner. */

static void DisplayWinner( NougthsandCrossesPtr exercise, StrPtr name ) {
	Widget		msgbox;
	extern Widget	mainwindow;


	msgbox = XmCreateInformationDialog( mainwindow, name, NULL, 0 );
	XtUnmanageChild( XmMessageBoxGetChild( msgbox, XmDIALOG_CANCEL_BUTTON ) );
	XtUnmanageChild( XmMessageBoxGetChild( msgbox, XmDIALOG_HELP_BUTTON ) );
	XtManageChild( msgbox );
	XtSetSensitive( exercise->nac, False );
}

/* It is invoked when the user presses the OK button. */

static void OkPressed( Widget widget, XtPointer client_data, XtPointer call_data ) {
	NougthsandCrossesPtr	exercise = (NougthsandCrossesPtr) client_data;
	int			k;
	XmString		xmstring;
	
	if( !exercise->istherequestion ) return;
	
	XmTextSetString( exercise->questiontext, "" );
	
	xmstring = XmStringCreateLocalized( "  " );
	
	for( k=0; k<ANSWERNUMBER; k++ ) {
		XtVaSetValues( exercise->radiobuttons[k],
			 XmNlabelString, xmstring,
			 NULL );
	}
	
	if( XmToggleButtonGetState( exercise->radiobuttons[exercise->table[exercise->curri][exercise->currj].solution] ) ) {
		if( exercise->nougthtomove ) {
			exercise->table[exercise->curri][exercise->currj].state = nougth;
			XtVaSetValues( exercise->table[exercise->curri][exercise->currj].button,
				XmNlabelType, XmPIXMAP,
				XmNlabelPixmap, exercise->o_pixmap,
				XmNlabelInsensitivePixmap, exercise->o_pixmap,
				NULL );
			if( Win( exercise, nougth ) ) {
				DisplayWinner( exercise, "nacnougthwinner" );
			}
		}
		else {
			exercise->table[exercise->curri][exercise->currj].state = cross;
			XtVaSetValues( exercise->table[exercise->curri][exercise->currj].button,
				XmNlabelType, XmPIXMAP,
			 	XmNlabelPixmap, exercise->x_pixmap,
				XmNlabelInsensitivePixmap, exercise->x_pixmap,
				NULL );			
			if( Win( exercise, cross ) ) {
				DisplayWinner( exercise, "natcrosswinner" );
			}
		}
	}
	
	for( k=0; k<ANSWERNUMBER; k++ ) {
		XmToggleButtonSetState( exercise->radiobuttons[k], False, False );
	}
			
	exercise->nougthtomove = !exercise->nougthtomove;
	xmstring  = XmStringCreateLocalized( exercise->remarkstring[(int)exercise->nougthtomove] );
	
	XtVaSetValues( exercise->remark, XmNlabelString, xmstring, NULL );		
	exercise->istherequestion = FALSE;	 
	
	XmStringFree( xmstring );
}

/* It is invoked when someone presses one of the card buttons. */

static void ChooseButton( Widget widget, XtPointer client_data, XtPointer call_data ) {
	NougthsandCrossesPtr	exercise = (NougthsandCrossesPtr) client_data;
	int			i, j, k;
	Boolean			found;
	XmString		xmstring;
	
	if( exercise->istherequestion ) return;
	
	for( i=-1, found=FALSE; !found; i++ ) {
		for( j=-1; j<FIELDSIZE-1 && !found; j++ ) {
			found = exercise->table[i+1][j+1].button == widget;
		}
	}
	
	exercise->curri = i;
	exercise->currj = j;
	
	if( exercise->table[i][j].state != empty ) return;
	
	exercise->istherequestion = TRUE;
	
	XmTextSetString( exercise->questiontext, exercise->table[i][j].question );
	
	for( k=0; k<4; k++ ) {
		xmstring = XmStringCreateLocalized( exercise->table[i][j].text[k] );
		XtVaSetValues( exercise->radiobuttons[k], 
			 XmNlabelString, xmstring,
			 NULL );
	}
}	

	
/* It will display the nougthsandcrosses panel. */
		
static void Disp( void *p, Widget widget ) {
	NougthsandCrossesPtr	exercise = (NougthsandCrossesPtr) p;
	Widget			remarkpanel, buttonpanel, sspanel, okbutton, okbuttonpanel;
	int			i,j,n;
	Arg			args[15];
	char			name[10];	
	XmString		string;	
	Pixel			cur_color;
	
	
	exercise->nac = XtVaCreateWidget( "nac", xmRowColumnWidgetClass, widget,
		XmNorientation, XmVERTICAL,
		NULL );
	
	remarkpanel = XtVaCreateManagedWidget( "remarkpanel", xmFormWidgetClass, exercise->nac,
		NULL );
	
	buttonpanel = XtVaCreateManagedWidget( "button", xmBulletinBoardWidgetClass, exercise->nac,
		NULL );
	
	sspanel = XtVaCreateManagedWidget( "ss", xmRowColumnWidgetClass, exercise->nac,
		NULL );	

	okbuttonpanel = XtVaCreateManagedWidget( "okbuttonpanel", xmFormWidgetClass, exercise->nac,
		NULL );	

											
	cur_color = BlackPixelOfScreen (XtScreen ( buttonpanel ));			/* initialize bitmaps */

	exercise->x_pixmap = XmGetPixmap (XtScreen ( buttonpanel ), "cross.xbm",
		cur_color, WhitePixelOfScreen (XtScreen ( buttonpanel )));
	exercise->o_pixmap = XmGetPixmap (XtScreen ( buttonpanel ), "nougth.xbm",
		cur_color, WhitePixelOfScreen (XtScreen ( buttonpanel )));

	if ( ( exercise->x_pixmap == XmUNSPECIFIED_PIXMAP ) || ( exercise->o_pixmap == XmUNSPECIFIED_PIXMAP ) ) {
        	printf( "can't create initial pixmap" );
        	exit (1);
    	}
	
	string = XmStringCreateLocalized( " " );
		
	for ( i=0; i<4; i++ ) {
		for ( j=0; j<4; j++ ) {
			exercise->table[i][j].state = empty;
			sprintf( name, "button%d%d", i,j );
			exercise->table[i][j].button = XtVaCreateManagedWidget( name, xmPushButtonWidgetClass,
				buttonpanel,
				XmNx, STARTX + (WIDTH+SPACE)*i,
				XmNy, STARTY + (HEIGHT+SPACE)*j,
				XmNwidth, WIDTH,
				XmNheight, HEIGHT,			
				XmNlabelString, string,
				XmNmarginWidth, 0,
				XmNmarginHeight, 0,
				NULL );
			XtAddCallback( exercise->table[i][j].button, XmNactivateCallback, ChooseButton,
				exercise );
		}
	}  
	
	string = XmStringCreateLocalized( exercise->remarkstring[exercise->nougthtomove] );
	
	exercise->remark = XtVaCreateManagedWidget( "remark", xmLabelWidgetClass, remarkpanel,
		XmNleftAttachment, XmATTACH_POSITION,
		XmNleftPosition, 20,
		XmNrightAttachment, XmATTACH_POSITION,
		XmNrightPosition, 80,
		XmNlabelString, string,
		NULL );
	
	XtVaCreateManagedWidget( "questionlabel", xmLabelWidgetClass, sspanel, NULL );
		
	n = 0;	
	XtSetArg( args[n], XmNcursorPositionVisible, FALSE );	 	n++;
	XtSetArg( args[n], XmNeditMode, XmSINGLE_LINE_EDIT );	 	n++;
	XtSetArg( args[n], XmNscrollBarDisplayPolicy, XmAS_NEEDED );	n++;
	XtSetArg( args[n], XmNcolumns, MAXLENGTH );		 	n++;
	XtSetArg( args[n], XmNscrollHorizontal, TRUE );		 	n++;	
	XtSetArg( args[n], XmNeditable, FALSE );		 	n++;

	exercise->questiontext = XmCreateScrolledText( sspanel, "questiontext", args, n );
	XtManageChild( exercise->questiontext );

	XtVaCreateManagedWidget( "answerlabel", xmLabelWidgetClass, sspanel, NULL );	
	
	exercise->radiobox = XmCreateRadioBox( sspanel, "radiobox", NULL, 0 );
	
	string = XmStringCreateLocalized( " " );
	
	for ( i=0; i<4; i++ ) {
		sprintf( name, "rbuttons%d", i );
		exercise->radiobuttons[i] = XtVaCreateManagedWidget( name,
			xmToggleButtonWidgetClass, exercise->radiobox,
			XmNlabelString, string,
			NULL );
	}
	
	okbutton = XtVaCreateManagedWidget( "okbutton", xmPushButtonWidgetClass, okbuttonpanel,
		XmNleftAttachment, XmATTACH_POSITION,
		XmNleftPosition, 30,
		XmNrightAttachment, XmATTACH_POSITION,
		XmNrightPosition, 70,
		NULL );
	
	XtAddCallback( okbutton, XmNactivateCallback, OkPressed, exercise );
	
	XtManageChild( exercise->radiobox );			
	
	XtManageChild( exercise->nac );	
	
	XmStringFree( string );
}


/* It is not used. */

static void GetValuation( void *p, int *value, int *maxvalue ) {
}

/* It will deallocate memory which is used by the object. */

static void Free( void *p ) {
	NougthsandCrossesPtr	exercise = (NougthsandCrossesPtr) p;
	int			i,j,k;
	
	for( i=0; i<FIELDSIZE; i++ ) {
		for( j=0; j<FIELDSIZE; j++ ) {
			XtFree( (char *) exercise->table[i][j].question );
			for( k=0; k<ANSWERNUMBER; k++ ) {
				XtFree( (char *) exercise->table[i][j].text[k] );
			}
		}
	}

	XtFree( (char *) exercise );
}


/* Allocates memory for a new NougthsandCrosses object and sets its function fields */

NougthsandCrossesPtr NewNougthsandCrosses( void ) {
	NougthsandCrossesPtr	p;
	
	p = XtNew( NougthsandCrosses );
	p->create = Create;
	p->display = Disp;
	p->getvaluation = GetValuation;
	p->free = Free;
	p->choosebutton = ChooseButton;
	return p;
}
