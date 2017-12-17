
/* --------------------------------------------------------------------------
	FILE : 		exercises.c
	DESCRIPTION:	implementation module for object Exercises
	CREATED BY:	Bigi-boys
	CREATION TIME:	18-04-95 10:28	
	MODIFICATION HISTORY:
			Z2, 28-04-95 23:00		
-----------------------------------------------------------------------------*/	

#include	"exercises.h"

/* This global variable is for indicating the state of an exercise dialog box */
/* If it is TRUE the exercise is put on the screen */

ExistExercise	existexercise = { FALSE, FALSE, FALSE, FALSE,
				FALSE, FALSE, FALSE, FALSE };

/* It creates an appropriate exercise according to the exercises object received */

static void NewAppropriateExercise( ExercisesPtr exercises ) {
	extern ExerciseCPtr	exercisec;
	
	exercises->exercise = exercisec->getexercise( exercisec, 
		exercises->exercisetype, exercises->level );
	exercises->exercise->display( exercises->exercise, exercises->form );
}	 

/* It initializes a new exercises object, sets its fields */

static void Create( void *p, ExerciseType exercisetype,	Level level ) {
	ExercisesPtr		exercises = p;
	extern Widget		mainwindow;
	extern ExistExercise	existexercise;
	
	exercises->valuation = NewValuation();
	exercises->valuation->create( exercises->valuation );

	exercises->exercisedlgbox = XmCreateTemplateDialog( mainwindow, "exercisedlgbox",
		NULL, 0 );

	XtAddCallback( XmMessageBoxGetChild(exercises->exercisedlgbox,
		XmDIALOG_OK_BUTTON), XmNactivateCallback,
		exercises->nextexercise, exercises );
	XtAddCallback( XmMessageBoxGetChild(exercises->exercisedlgbox,
		XmDIALOG_CANCEL_BUTTON), XmNactivateCallback,
		exercises->finishexercises, exercises );

	exercises->form = XtVaCreateManagedWidget( "form", xmFormWidgetClass,
		exercises->exercisedlgbox, NULL );

	exercises->exercisetype = exercisetype;
	exercises->level = level;
	NewAppropriateExercise( exercises );

	existexercise[ exercises->exercisetype ] = TRUE;

	XtManageChild( exercises->exercisedlgbox );
}

/* It freshes the valuation, creates a new exercise and display it onto the dialog. */

static void NextExercise( Widget widget, XtPointer client_data, XtPointer call_data ) {
	ExercisesPtr	exercises = (ExercisesPtr) client_data;
	int		value, maxvalue;
	
	exercises->exercise->getvaluation( exercises->exercise,
		&value, &maxvalue );
	exercises->valuation->freshvaluation( exercises->valuation,
		value, maxvalue );
	exercises->exercise->free( exercises->exercise );

	NewAppropriateExercise( exercises );
	XtManageChild( exercises->exercisedlgbox );
}

/* It destroys the last exercise and the exercise dialog. */

static void FinishExercises( Widget widget, XtPointer client_data, XtPointer call_data ) {
	ExercisesPtr		exercises = (ExercisesPtr) client_data;
	extern ExistExercise	existexercise;
	int			value, maxvalue;
	
	exercises->exercise->getvaluation( exercises->exercise,
		&value, &maxvalue );
	exercises->valuation->freshvaluation( exercises->valuation,
		value, maxvalue );

	existexercise[ exercises->exercisetype ] = FALSE;
	exercises->exercise->free( exercises->exercise );
	XtDestroyWidget( exercises->exercisedlgbox );
	
	if( exercises->exercisetype!=nougthsandcrosses &&
		exercises->exercisetype!=memory ) {
		exercises->valuation->dispvaluation( exercises->valuation );
	} else {
		XtFree( (char *) exercises->valuation );
	}
	
	XtFree( (char *) exercises );	
}

/* It creates a new exercises object and sets its function fields. */

ExercisesPtr NewExercises( void ) {
	ExercisesPtr	p;
	
	p = XtNew( Exercises );
	p->create = Create;
	p->nextexercise = NextExercise;
	p->finishexercises = FinishExercises;
	return p;
}
