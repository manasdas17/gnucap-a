/***  B4SOI 03/06/2009 Wenwei Yang Release   ***/

static char rcsid[] = "$Id: b4soimdel.c  03/06/2009 Wenwei Yang Release $";

/**********
 * Copyright 2009 Regents of the University of California.  All rights reserved.
 * Authors: 1998 Samuel Fung, Dennis Sinitsky and Stephen Tang
 * Authors: 1999-2004 Pin Su, Hui Wan, Wei Jin, b3soimdel.c
 * Authors: 2005- Hui Wan, Xuemei Xi, Ali Niknejad, Chenming Hu.
 * Authors: 2009- Wenwei Yang, Chung-Hsun Lin, Ali Niknejad, Chenming Hu.
 * File: b4soimdel.c
 * Modified by Hui Wan, Xuemei Xi 11/30/2005
 * Modified by Wenwei Yang, Chung-Hsun Lin, Darsen Lu 03/06/2009
 **********/

#include "spice.h"
#include <stdio.h>
#include "util.h"
#include "b4soidef.h"
#include "sperror.h"
#include "suffix.h"

int
B4SOImDelete(inModel,modname,kill)
GENmodel **inModel;
IFuid modname;
GENmodel *kill;
{
B4SOImodel **model = (B4SOImodel**)inModel;
B4SOImodel *modfast = (B4SOImodel*)kill;
B4SOIinstance *here;
B4SOIinstance *prev = NULL;
B4SOImodel **oldmod;

    oldmod = model;
    for (; *model ; model = &((*model)->B4SOInextModel)) 
    {    if ((*model)->B4SOImodName == modname || 
             (modfast && *model == modfast))
	     goto delgot;
         oldmod = model;
    }
    return(E_NOMOD);

delgot:
    *oldmod = (*model)->B4SOInextModel; /* cut deleted device out of list */
    for (here = (*model)->B4SOIinstances; here; here = here->B4SOInextInstance)
    {    if(prev) FREE(prev);
         prev = here;
    }
    if(prev) FREE(prev);
    FREE(*model);
    return(OK);
}



