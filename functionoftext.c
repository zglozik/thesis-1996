/* --------------------------------------------------------------------------
	FILE : 		functionoftext.c
	DESCRIPTION:	implementation modul for make a kozpontozast
	CREATED BY:	Bigi-boys
	CREATION TIME:	18-04-95 12:50
	MODIFICATION HISTORY:
		Busy	10-05-95 12:50		
		
-----------------------------------------------------------------------------*/

#include	"functionoftext.h"

Boolean IsChar( char q ) {
	
	if ( q>='a' && q<='z' || q>='A' && q<='Z' || q>='0' && q<='9' ) {
		return TRUE;
	}
	return FALSE;
}

void SetTheText( Widget widget, StrPtr text ) {
	char	*q;
	int	j = 0;
	Arg	arg[1];
	int	n = 0;

	q = (StrPtr) XtMalloc( strlen( text ) );
	while ( text[j]!='\0' ) {
		if ( text[j]<0 ) {
			while ( text[j]<0 || IsChar( text[j] ) ) {
				q[j++] = '*';
			}
		} else {
			q[j] = text[j];
			j++;
		}
	}
	q[j] = '\0';
		

	
	XtSetArg( arg[n], XmNvalue, q ); n++;
	XtSetValues( widget, arg, n );
	
	XmStringFree( q );
}

Boolean FindStrInStr( char* s1, char* s2 ) {
	int	i,k,m,n;
		
	i = 0;
	k = 0;
	m = strlen( s2 );
	n = strlen( s1 );
	
	while ( k!=n && i+n<=m ) {
		if ( s1[k]==s2[i+k] ) {
			k++;
		} else {
			i++;
			k = 0;
		}
		if ( k==n && IsChar( s2[i+k] ) ) {
			i++;
			k = 0;
		}	
	}
	if ( k==n && k!=0 ) {
		return i;
	} else {
		return (-1);
	}
}
