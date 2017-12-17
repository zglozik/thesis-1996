/* --------------------------------------------------------------------------
	FILE : 		nougthsandcrosses.h
	DESCRIPTION:	module for exercise Nougths&Crosses
	CREATED BY:	Z1
	CREATION TIME:	18-04-95
	MODIFICATION HISTORY:
		Z1, ??-05-95		
-----------------------------------------------------------------------------*/

/* Noughts & Crosses object */

#ifndef NOUGTHSANDCROSSESH
#define NOUGTHSANDCROSSESH

#include	<ctype.h>
#include	<Xm/Xm.h>
#include	<Xm/RowColumn.h>
#include	<Xm/List.h>
#include	<Xm/Form.h>
#include	<Xm/Label.h>
#include	<Xm/ToggleB.h>	
#include	<Xm/PushB.h>
#include	<Xm/MessageB.h>
#include	<Xm/Text.h>

#include	"types.h"
#include	"exercise.h"


typedef enum {
	empty, nougth, cross
} ButtonState;

#define	ANSWERNUMBER	4
#define	FIELDSIZE	4
#define	SELECTNUMBER	FIELDSIZE*FIELDSIZE

typedef struct {
	StrPtr		question;
	StrPtr		text[ANSWERNUMBER];
	int		solution;	/* the solution of the simple select */
	ButtonState	state;		/* is button solved? */
	Widget		button;		/* a button to click on for each field */
} TableItem, *TableItemPtr;

typedef TableItem	Table[FIELDSIZE][FIELDSIZE];

typedef void	(*ChooseButtonProc)( Widget, XtPointer, XtPointer );

typedef	char		RemarkString[30];		

typedef struct {
	int             value;
       	int             maxvalue;

 	CreateExerciseProc	create;
	DisplayProc     	display;
	GetValuationProc	getvaluation;
        FreeExerciseProc       	free;				/* exercise consists of these */

	Table			table;
	ChooseButtonProc	choosebutton;			/* called when someone pushed a button */
	Boolean			nougthtomove, istherequestion;
	int			curri, currj;
	RemarkString		remarkstring[2];
	Pixmap			x_pixmap, o_pixmap;
						
	Widget			nac;
	Widget			remark, radiobox, radiobuttons[4], questiontext;					
} NougthsandCrosses, *NougthsandCrossesPtr;

NougthsandCrossesPtr NewNougthsandCrosses( void );

#endif
