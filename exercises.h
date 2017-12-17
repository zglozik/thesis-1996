/* --------------------------------------------------------------------------
	FILE : 		exercises.h
	DESCRIPTION:	header for Object Exercise
	CREATED BY:	Bigi-boys
	CREATION TIME:	18-04-95 10:35
	MODIFICATION HISTORY:
			
-----------------------------------------------------------------------------*/	

#ifndef EXERCISESH
#define EXERCISESH

#include	<Xm/Xm.h>
#include	<Xm/MessageB.h>
#include	<Xm/Form.h>

#include	"types.h"
#include	"exercise.h"
#include	"exercisec.h"
#include	"valuation.h"

/* This will show whether an exercise exists */

typedef Boolean	ExistExercise[8];

typedef void	(*CreateExercisesProc)( void *, ExerciseType, Level );
typedef void	(*NextExerciseProc)( Widget, XtPointer, XtPointer );	
typedef void	(*FinishExercisesProc)( Widget, XtPointer, XtPointer );

typedef struct {
	ExerciseType	exercisetype;
	Level		level;
	ExercisePtr	exercise;
	ValuationPtr	valuation;
	Widget		exercisedlgbox;  	/* to put exercises on */
	Widget		form;

	CreateExercisesProc	create;
	NextExerciseProc	nextexercise;
	FinishExercisesProc	finishexercises;
} Exercises, *ExercisesPtr;

ExercisesPtr NewExercises( void );

#endif
