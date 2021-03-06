/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1995 Min-Chie Jeng and Mansun Chan.
File: b3trunc.c
**********/

#include "spice.h"
#include <stdio.h>
#include <math.h>
#include "cktdefs.h"
#include "bsim3def.h"
#include "sperror.h"
#include "suffix.h"


int
BSIM3trunc(inModel,ckt,timeStep)
GENmodel *inModel;
register CKTcircuit *ckt;
double *timeStep;
{
register BSIM3model *model = (BSIM3model*)inModel;
register BSIM3instance *here;

#ifdef STEPDEBUG
    double debugtemp;
#endif /* STEPDEBUG */

    for (; model != NULL; model = model->BSIM3nextModel)
    {    for (here = model->BSIM3instances; here != NULL;
	      here = here->BSIM3nextInstance)
	 {
#ifdef STEPDEBUG
            debugtemp = *timeStep;
#endif /* STEPDEBUG */
            CKTterr(here->BSIM3qb,ckt,timeStep);
            CKTterr(here->BSIM3qg,ckt,timeStep);
            CKTterr(here->BSIM3qd,ckt,timeStep);
#ifdef STEPDEBUG
            if(debugtemp != *timeStep)
	    {  printf("device %s reduces step from %g to %g\n",
                       here->BSIM3name,debugtemp,*timeStep);
            }
#endif /* STEPDEBUG */
        }
    }
    return(OK);
}



