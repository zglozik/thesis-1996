
/* --------------------------------------------------------------------------
	FILE : 		valuation.h
	DESCRIPTION:	header file for valuation object
	CREATED BY:	Bigi-boys
	CREATION TIME:	18-04-95 10:59
	MODIFICATION HISTORY:
		
-----------------------------------------------------------------------------*/	

#ifndef VALUATIONH
#define VALUATIONH

#include	<stdio.h>
#include	<Xm/Xm.h>
#include	<Xm/RowColumn.h>
#include	<Xm/MessageB.h>
#include	<Xm/Label.h>

#include	"types.h"

typedef void	(*CreateValuationProc)( void * );
typedef void	(*FreshValuationProc)( void *, int, int );
typedef void	(*DispValuationProc)( void * );

typedef struct {
	int	currentpoint;
	int	maxpoint;

	CreateValuationProc	create;
	FreshValuationProc	freshvaluation;
	DispValuationProc	dispvaluation;
} Valuation, *ValuationPtr;

ValuationPtr NewValuation( void );

#endif
