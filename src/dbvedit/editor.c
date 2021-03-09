/*
 * Copyright (C) 2001 FirstBase Software, Inc. All rights reserved.
 *
 * $Id: editor.c,v 9.0 2001/01/09 02:55:58 john Exp $
 *
 * See the file LICENSE for conditions of use and distribution.
 *
 */

#ifndef LINT
static char Editor_sid[] = "@(#) $Id: editor.c,v 9.0 2001/01/09 02:55:58 john Exp $";
#endif

#include <dbve_ext.h>

static char *FMT1 = "(dot@%ld/%ld)";
extern int add_exactly;			/* counter used to edit -a N records */

/* 
 *  editor - main loop of the dbvedit visual fb_database editor.
 */
 
   editor()
      
      {      
         int i, disp_st;
	 char hlp[FB_MAXNAME];

         pindx = 0;
         rec = oldrec = 0L;
	 for (i = 0; i < FB_MAXIDXS; i++)
            irec[i] = ibase[i] = 0L;
         mode = NULL;
	 disp_st = 0;
         for(;;){
            com[0] = NULL;
	    st = FB_AOK;
	    if (hp->ihfd > 0){
               fb_lock_head(hp);
	       fb_getxhead(hp->ihfd, &(hp->bsmax), &(hp->bsend));
	       fb_unlock_head(hp);
               if (hp->b_tree)
	          hlp[0] = NULL;
               else
	          sprintf(hlp, FMT1, irec[pindx], hp->bsmax);
	       }
	     else
	       sprintf(hlp, FMT1, rec, hp->reccnt);
            if (mode == NULL){
	       fb_scrstat(SYSMSG[S_COMMAND_LEVEL]);
	       fb_move(2, 64), fb_clrtoeol();	/* cleanup individual rec stat */
               st = scanput(hp->ip[pindx], hlp, disp_st);
	       }
            else{
               /* must be a signal mode - decipher for docmd */
               com[0] = NULL;
	       if (mode == FB_FADD)	/* autoadd - leave mode untouched */
	          com[0] = FB_FADD;
	       else if (mode == FB_BSIGNAL){
	          st = FB_YSIGNAL;		/* rec-to-rec Record Level - fwd */
		  mode = NULL;
		  }
	       else if (mode == FB_FSIGNAL){
	          st = FB_ESIGNAL;		/* rec-to-rec Record Level - bkwd */
		  mode = NULL;
		  }
	       else if (mode == FB_SSIGNAL){
	          st = FB_SSIGNAL;		/* rec-to-rec Record Level - ssearch */
		  mode = NULL;
		  }
	       }
	    if (st == FB_DSIGNAL){
	        /*
	         * fb_scrhdr(hp, NIL);
	         * infotoggle();
	         * disp_st = ZAP_REDRAW;
		 */
	       }
	    else if ((disp_st = docmd()) == FB_END)
               break;			/* REDRAW,NOREDRAW flags display */
            if (add_exactly > 0)
               if (--add_exactly <= 0)
                  break;
            }
      }
