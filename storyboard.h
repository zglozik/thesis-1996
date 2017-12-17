
/* Storyboard object */

#ifndef STORYBOARDH
#define STORYBOARDH

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
	
	Widget			storyboard;
	Widget			textedit;
	Widget			textfield;
} Storyboard, *StoryboardPtr;

StoryboardPtr NewStoryboard( void );

#endif
