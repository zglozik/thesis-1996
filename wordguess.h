/* --------------------------------------------------------------------------
	FILE : 		wordguess.h
	DESCRIPTION:	definition modul for worguess object
	CREATED BY:	Bigi-boys
	CREATION TIME:	18-04-95 11:01
	MODIFICATION HISTORY:
		Busy	09-05-95 11:48		
		
-----------------------------------------------------------------------------*/

#ifndef WORDGUESSH
#define WORDGUESSH

#define	MAXINFORMATION	20

#include	<string.h>
#include	<stdlib.h>
#include	<Xm/Xm.h>
#include	<Xm/RowColumn.h>
#include	<Xm/MessageB.h>
#include	<Xm/Text.h>
#include	<Xm/TextF.h>
#include	<Xm/Label.h>
#include	<Xm/PushB.h>

#include	"types.h"
#include	"exercise.h"

typedef void	(*PutInformationProc)( Widget, XtPointer, XtPointer );

typedef struct {
	int             value;
        int             maxvalue;

 	CreateExerciseProc	create;
	DisplayProc     	display;
	GetValuationProc	getvaluation;
       	FreeExerciseProc       	free;	/* exercise consists of these */

	StrPtr			word;
	StrPtr			information[MAXINFORMATION];
	
	Widget			wordguess;
	Widget			textedit;
	Widget			textfield;
} WordGuess, *WordGuessPtr;

WordGuessPtr NewWordGuess( void );

#endif

