/* --------------------------------------------------------------------------
	FILE : 		dialoghalves.h
	DESCRIPTION	header file for guesstitle object
	CREATED BY:	Bigi-boys
	CREATION TIME:	18-04-95 10:50
	MODIFICATION HISTORY:
		Busy	11-05-95 17:06		
		
-----------------------------------------------------------------------------*/	

#ifndef DIALOGHALVESH
#define DIALOGHALVESH

#include	<stdio.h>
#include	<string.h>
#include	<Xm/Xm.h>
#include	<Xm/PushB.h>
#include	<Xm/RowColumn.h>
#include	<Xm/Label.h>
#include	<Xm/Text.h>
#include	<Xm/TextF.h>
#include	<Xm/MessageB.h>

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
	
	StrPtr			story;
	int			rowcount;
	int			wordcount;
		
	Widget			dialoghalves;
	Widget			textedit;
	Widget			textfield;
} DialogHalves, *DialogHalvesPtr;

DialogHalvesPtr NewDialogHalves( void );

#endif
