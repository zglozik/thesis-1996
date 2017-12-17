/* --------------------------------------------------------------------------
	FILE : 		exercise.h
	DESCRIPTION:	header file for exercise object
	CREATED BY:	Bigi-boys
	CREATION TIME:	11-04-95 17:00
	MODIFICATION HISTORY:
		Bigi-boys, 13-04-95  22:00
		
-----------------------------------------------------------------------------*/	



#ifndef EXERCISEH
#define EXERCISEH 

#include	<Xm/Xm.h>

#include	"types.h"

typedef enum { 
	unjumble, simpleselect, dialoghalves, wordguess, 
	nougthsandcrosses, storyboard, memory, guesstitle
} ExerciseType, *ExerciseTypePtr;

typedef enum {
	beginner, medium, advanced
} Level, *LevelPtr;

typedef void (*CreateExerciseProc)( void *, StrPtr );
typedef void (*DisplayProc)( void *, Widget );
typedef void (*GetValuationProc)( void *, int *, int * );
typedef void (*FreeExerciseProc)( void * );

typedef struct {
        int             value;
        int             maxvalue;

 	CreateExerciseProc	create;
	DisplayProc     	display;
	GetValuationProc	getvaluation;
        FreeExerciseProc       	free;
} Exercise, *ExercisePtr;

#endif
