/* Exercise Collection object */

#ifndef EXERCISECH
#define EXERCISECH

#include	<Xm/Xm.h>

#include	"types.h"
#include	"exercise.h"

#include	"unjumble.h"
#include	"dialoghalves.h"
#include	"simpleselect.h"
#include	"wordguess.h"
#include	"nougthsandcrosses.h"
#include	"storyboard.h"
#include	"memory.h"
#include	"guesstitle.h"

#include	"indexedfile.h"

typedef ExercisePtr	(*GetExerciseProc)( void *, ExerciseType, Level );
typedef Boolean		(*ExistExerciseProc)( void *, ExerciseType, Level ); 
typedef void		(*PutExerciseProc)( void *, ExerciseType, Level, StrPtr );

typedef struct {
        GetExerciseProc		getexercise;
        ExistExerciseProc	existexercise;
        PutExerciseProc		putexercise;
} ExerciseC, *ExerciseCPtr;

ExerciseCPtr NewExerciseC( void );

#endif
