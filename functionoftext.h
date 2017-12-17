/* --------------------------------------------------------------------------
	FILE : 		functionoftext.h
	DESCRIPTION:	header file of my textfunction
	CREATED BY:	Bigi-boys
	CREATION TIME:	18-04-95 10:50
	MODIFICATION HISTORY:
		Busy	10-05-95 12:55		
		
-----------------------------------------------------------------------------*/

#ifndef FUNCTIONOFTEXT
#define FUNCTIONOFTEXT

#include	<Xm/Xm.h>

#include	"types.h"

Boolean IsChar( char  );
void SetTheText( Widget, StrPtr );
Boolean FindStrInStr( char*, char* );

#endif
