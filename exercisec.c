 /* --------------------------------------------------------------------------
	FILE : 		exercisec.c
	DESCRIPTION:	Exercise collection module
	CREATED BY:	Z1
	CREATION TIME:	11-04-95 17:00
	MODIFICATION HISTORY:
		Z2, 21-04-95 11:37
		
-----------------------------------------------------------------------------*/	

#include	"exercisec.h"

#define	ENDMARK		'#'	/* it shows the end of an exercise in a file */


/* it gives the name of the practice file according to exercisetype and difficulty level */

static char *getfilename( ExerciseType exercisetype, Level level, char *buf ) {

	/* they consist of the parts of the name of the exercise files */

	char	filenames[][20] = { "unjumble", "simpleselect", "dialoghalves",
			"wordguess", "nougthsandcrosses", "storyboard", "memory",
			"guesstitle" };
	char	levelnames[][20] = { "beg", "med", "adv" };

	strcpy( buf, filenames[exercisetype] );
	strcat( buf, ".exer." );
	strcat( buf, levelnames[level] );
	return buf;
}

/* It gets out an appropriate exercise from the collection and creates it */

static ExercisePtr GetExercise( void *p, ExerciseType exercisetype, Level level ) {
	ExerciseCPtr	exercisec = (ExerciseCPtr) p;
	ExercisePtr	exercise;
	char		filename[50];
	IndexedFilePtr	indexedfile;
	char		*text;

	switch( exercisetype ) {
		case unjumble:
			exercise = (ExercisePtr) NewUnjumble();
			break;
		case simpleselect:
			exercise = (ExercisePtr) NewSimpleSelect();
			break;
		case dialoghalves:
			exercise = (ExercisePtr) NewDialogHalves();
			break;
		case wordguess:
			exercise = (ExercisePtr) NewWordGuess();
			break;
		case nougthsandcrosses:
			exercise = (ExercisePtr) NewNougthsandCrosses();
			break;
		case storyboard:
			exercise = (ExercisePtr) NewStoryboard();
			break;
		case memory:
			exercise = (ExercisePtr) NewMemory();
			break;
		case guesstitle:
			exercise = (ExercisePtr) NewGuessTitle();
			break;
	}
	
	getfilename( exercisetype, level, filename );	
	indexedfile = NewIndexedFile();
	indexedfile->open( indexedfile, filename, ENDMARK );
	text = indexedfile->read( indexedfile,
		rand() % indexedfile->recordnum( indexedfile ) );
	indexedfile->close( indexedfile );
	XtFree( (char *) indexedfile );
	
	exercise->create( exercise, text );
	XtFree( text );
	
	return exercise;
}

/* it tells that the file is empty or not */

static Boolean ExistExercise( void *p, ExerciseType exercisetype, Level level ) {
	char		filename[50];
	IndexedFilePtr	indexedfile;
	Boolean		result;

	getfilename( exercisetype, level, filename );	
	indexedfile = NewIndexedFile();
	indexedfile->open( indexedfile, filename, ENDMARK );
	result = indexedfile->recordnum( indexedfile ) > 0;
	indexedfile->close( indexedfile );
	
	return result;
}

static void PutExercise( void *p,  ExerciseType exercisetype, Level level, StrPtr text ) {
}

/* it creates a new exercise collection object */

ExerciseCPtr NewExerciseC( void ) {
	ExerciseCPtr	p;
	
	p = XtNew( ExerciseC );
	p->getexercise = GetExercise;
	p->existexercise = ExistExercise;
	p->putexercise = PutExercise;
	return p;
}
