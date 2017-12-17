/* Do Test object */

#ifndef DOTESTH
#define DOTESTH

#include	<Xm/Xm.h>
#include	<Xm/FileSB.h>
#include	<Xm/Form.h>
#include	<Xm/MessageB.h>

#include	"types.h"
#include	"exercise.h"
#include	"testcollection.h"
#include	"valuation.h"

typedef void (*CreateDoTestProc)( void * );
typedef void (*SelectTestFileProc)( Widget, XtPointer, XtPointer );
typedef void (*CancelTestFileProc)( Widget, XtPointer, XtPointer );
typedef void (*NextTestExerciseProc)( Widget, XtPointer, XtPointer );
typedef void (*CancelTestExerciseProc)( Widget, XtPointer, XtPointer );
typedef void (*FreeTestExerciseProc)( void * );

typedef struct {
	ExercisePtr		exercise;
	TestCollectionPtr	testcollection;
	ValuationPtr		valuation;
	Widget			dotestdlgbox;
	Widget			form;
	

	CreateDoTestProc	create;

	SelectTestFileProc	selecttestfile;
	CancelTestFileProc	canceltestfile;
	
	NextTestExerciseProc	nexttestexercise;
	CancelTestExerciseProc	canceltestexercise;

	FreeTestExerciseProc	free;
} DoTest, *DoTestPtr;

DoTestPtr  NewDoTest( void );

#endif
