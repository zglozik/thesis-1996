/* --------------------------------------------------------------------------
	FILE : 		memory.h
	DESCRIPTION:	header for memory object
	CREATED BY:	Bigi-boys
	CREATION TIME:	18-04-95 10:52
	MODIFICATION HISTORY:
		Z1, 11-05-95 13:10		
		Z1, 20-05-95 21:30		
-----------------------------------------------------------------------------*/	
/* Memory object */

#ifndef MEMORYH
#define MEMORYH


#include	<string.h>
#include	<stdlib.h>

#include	<Xm/Xm.h>
#include	<Xm/RowColumn.h>
#include	<Xm/Form.h>
#include	<Xm/TextF.h>
#include	<Xm/ToggleB.h>	
#include	<Xm/Label.h>
#include	<Xm/PushB.h>
#include	<Xm/MessageB.h>	
#include	<Xm/Text.h>

#include	"types.h"
#include	"exercise.h"

#define MAXCARD		16

typedef struct {
	StrPtr		string;				/* an odd index means an expression, */  
	Widget		cardwidget;				/* an even one means a definiton */
} Card, *CardPtr;

typedef (*ChooseCardProc)(Widget widget, XtPointer client_data, XtPointer call_data );

typedef struct {
	int             point1, point2;			/* players' points */

	CreateExerciseProc	create;
	DisplayProc     	display;
	GetValuationProc	getvaluation;
	FreeExerciseProc       	free;			/* exercise consists of these */

	Card	cards[MAXCARD];
	int	mixedcards[MAXCARD];	

	ChooseCardProc	cardchoosen;
	
	Widget		memory, cardpanel;
	Widget		textfield[2], remarklabel;
} Memory, *MemoryPtr;

MemoryPtr NewMemory( void );

#endif
