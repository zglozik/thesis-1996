/* --------------------------------------------------------------------------
	FILE : 		simpleselect.h
	DESCRIPTION:	header file for simpleselect object
	CREATED BY:	Bigi-boys
	CREATION TIME:	18-04-95 10:59
	MODIFICATION HISTORY:
		Busy,Z1	01-05-95 0:25
				
-----------------------------------------------------------------------------*/

#ifndef SIMPLESELECTH
#define SIMPLESELECTH

#include	<string.h>
#include	<stdlib.h>
#include	<Xm/Xm.h>
#include	<Xm/RowColumn.h>
#include	<Xm/TextF.h>
#include	<Xm/ToggleB.h>	
#include	<Xm/Label.h>
#include	<Xm/PushB.h>
#include	<Xm/MessageB.h>	

#include	"types.h"
#include	"exercise.h"

typedef struct {
	int             value;
        int             maxvalue;

	CreateExerciseProc	create;
	DisplayProc     	display;
	GetValuationProc	getvaluation;
	FreeExerciseProc       	free; /* exercise consists of these */


	Widget		simpleselect;
	Widget		radiobox;
	Widget		buttons[4];		

	StrPtr		question;
	StrPtr		answers[4];
	int		solution;
} SimpleSelect, *SimpleSelectPtr;

SimpleSelectPtr NewSimpleSelect( void );

#endif
