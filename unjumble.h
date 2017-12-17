
/* --------------------------------------------------------------------------
	FILE : 		unjumble.h
	DESCRIPTION:	header file for unjumble object
	CREATED BY:	Bigi-boys
	CREATION TIME:	18-04-95 10:59
	MODIFICATION HISTORY:
		
-----------------------------------------------------------------------------*/	

#ifndef UNJUMBLEH
#define UNJUMBLEH

#include	<string.h>
#include	<stdlib.h>
#include	<Xm/Xm.h>
#include	<Xm/RowColumn.h>
#include	<Xm/List.h>
#include	<Xm/TextF.h>
#include	<Xm/Label.h>
#include	<Xm/PushB.h>
#include	<Xm/MessageB.h>

#include	"types.h"
#include	"exercise.h"

#define	MAXWORDLEN	20
#define	MAXWORDCOUNT	15

typedef struct {
	int             value;
       	int             maxvalue;

	CreateExerciseProc	create;
	DisplayProc     	display;
	GetValuationProc	getvaluation;
       	FreeExerciseProc       	free; /* exercise consists of these */

	StrPtr			sentence;
	XmString		words[MAXWORDCOUNT];
	int			wordcount;
	Widget			unjumble;	/* this is the main panel of
						the unjumble dialog */
	Widget			textfield;
	Widget			listbox;
} Unjumble, *UnjumblePtr;

UnjumblePtr NewUnjumble( void );

#endif

