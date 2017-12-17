
/* --------------------------------------------------------------------------
	FILE : 		valuation.c
	DESCRIPTION:	implementation modul for valuation object
	CREATED BY:	Bigi-boys
	CREATION TIME:	18-04-95 10:59
	MODIFICATION HISTORY:
		
-----------------------------------------------------------------------------*/	

#include	"valuation.h"

/* This initializes a new valuation object, clears its point fields. */

static void Create( void *p ) {
	ValuationPtr	valuation = (ValuationPtr) p;
	
	valuation->currentpoint = 0;
	valuation->maxpoint = 0;
}

/* This function freshes the valuation with points given. */

static void FreshValuation( void *p, int value, int maxvalue ) {
	ValuationPtr	valuation = (ValuationPtr) p;
	
	valuation->currentpoint += value;
	valuation->maxpoint += maxvalue;
}

/* This callback is invoked when OK BUTTON is pressed. */

static void OkCB( Widget widget, XtPointer client_data, XtPointer call_data ) {
	ValuationPtr	valuation = (ValuationPtr) client_data;
	
	XtDestroyWidget( widget );
	XtFree( (char *) valuation );
}

/* This function puts a message box onto the screen with the current valuation. */

static void DispValuation( void *p ) {
	ValuationPtr	valuation = (ValuationPtr) p;
	extern Widget	mainwindow;
	char		buffer[10];
	Widget		valuationdlgbox, rc;
	XmString	valuestr, maxvaluestr;
	
	valuationdlgbox = XmCreateTemplateDialog( mainwindow, "valuationdlgbox",
		NULL, 0 );
	XtAddCallback( XmMessageBoxGetChild( valuationdlgbox,
		XmDIALOG_OK_BUTTON ), XmNactivateCallback, OkCB, valuation );
		
	rc = XtVaCreateManagedWidget( "rowcolumn", xmRowColumnWidgetClass,
		valuationdlgbox,
		XmNpacking, XmPACK_COLUMN,
		XmNnumColumns,	2,
		XmNorientation, XmHORIZONTAL,
		NULL );

	sprintf( buffer, "%d", valuation->currentpoint );
	valuestr = XmStringCreateLocalized( buffer );
	sprintf( buffer, "%d", valuation->maxpoint );
	maxvaluestr = XmStringCreateLocalized( buffer );

	XtVaCreateManagedWidget( "maxvaluelabel", xmLabelWidgetClass,
		rc, NULL );
	XtVaCreateManagedWidget( NULL, xmLabelWidgetClass, rc,
		XmNlabelString, maxvaluestr,
		NULL );
	XtVaCreateManagedWidget( "valuelabel", xmLabelWidgetClass,
		rc, NULL );
	XtVaCreateManagedWidget( NULL, xmLabelWidgetClass, rc,
		XmNlabelString, valuestr,
		NULL );

	XmStringFree( valuestr );
	XmStringFree( maxvaluestr );
	
	XtManageChild( valuationdlgbox );
	XtPopup( XtParent(valuationdlgbox), XtGrabNone );
}


/* This function allocates memory for a new valuation object and
   sets its function fields */

ValuationPtr NewValuation( void ) {
	ValuationPtr	p;
	
	p = XtNew( Valuation );
	p->create = Create;
	p->freshvaluation = FreshValuation;
	p->dispvaluation = DispValuation;
	return p;
}
