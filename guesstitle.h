/* --------------------------------------------------------------------------
	FILE : 		guesstitle.h
	DESCRIPTION:	header file for guesstitle object
	CREATED BY:	Bigi-boys
	CREATION TIME:	18-04-95 10:50
	MODIFICATION HISTORY:
		Busy	09-05-95 17:38		
		
-----------------------------------------------------------------------------*/

#ifndef GUESSTITLEH
#define GUESSTITLEH

#include	<string.h>
#include	<stdlib.h>
#include	<Xm/Xm.h>
#include	<Xm/RowColumn.h>
#include	<Xm/MessageB.h>
#include	<Xm/Text.h>
#include	<Xm/TextF.h>
#include	<Xm/ToggleB.h>	
#include	<Xm/Label.h>
#include	<Xm/PushB.h>

#include	"types.h"
#include	"exercise.h"
#include	"functionoftext.h"

typedef struct {
	int             value;
	int             maxvalue;

	CreateExerciseProc	create;
	DisplayProc     	display;
	GetValuationProc	getvaluation;
       	FreeExerciseProc       	free;	/* exercise consists of these */

	StrPtr		titles[5];
	StrPtr		story;
	int		rowcount;
	int		solution;
	
	Widget		guesstitle;		
	Widget		textedit;
	Widget		buttons[5]; 		
} GuessTitle, *GuessTitlePtr;

GuessTitlePtr NewGuessTitle( void );

#endif
