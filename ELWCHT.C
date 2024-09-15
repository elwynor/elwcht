/*****************************************************************************
 *                                                                           *
 *    Chatter / Chat Lounge                                            v1.1  *
 *                                                                           *
 *    Initial development: Mike Polzin, WilderLand Software, 1991            *
 *    v1.0: Mike Polzin April 29, 1994                                       *
 *    WG3.2 port: Rick Hadsall February 18, 2006                             *
 *    V10/WG32 single kit v1.1: Rick Hadsall September 15, 2024              *
 *                                                                           *
 * Copyright (C) 2024 Rick Hadsall.  All Rights Reserved.                    *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU Affero General Public License as published  *
 * by the Free Software Foundation, either version 3 of the License, or      *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU Affero General Public License for more details.                       *
 *                                                                           *
 * You should have received a copy of the GNU Affero General Public License  *
 * along with this program. If not, see <https://www.gnu.org/licenses/>.     *
 *                                                                           *
 * Additional Terms for Contributors:                                        *
 * 1. By contributing to this project, you agree to assign all right, title, *
 *    and interest, including all copyrights, in and to your contributions   *
 *    to Rick Hadsall and Elwynor Technologies.                              *
 * 2. You grant Rick Hadsall and Elwynor Technologies a non-exclusive,       *
 *    royalty-free, worldwide license to use, reproduce, prepare derivative  *
 *    works of, publicly display, publicly perform, sublicense, and          *
 *    distribute your contributions                                          *
 * 3. You represent that you have the legal right to make your contributions *
 *    and that the contributions do not infringe any third-party rights.     *
 * 4. Rick Hadsall and Elwynor Technologies are not obligated to incorporate *
 *    any contributions into the project.                                    *
 * 5. This project is licensed under the AGPL v3, and any derivative works   *
 *    must also be licensed under the AGPL v3.                               *
 * 6. If you create an entirely new project (a fork) based on this work, it  *
 *    must also be licensed under the AGPL v3, you assign all right, title,  *
 *    and interest, including all copyrights, in and to your contributions   *
 *    to Rick Hadsall and Elwynor Technologies, and you must include these   *
 *    additional terms in your project's LICENSE file(s).                    *
 *                                                                           *
 * By contributing to this project, you agree to these terms.                *
 *                                                                           *
 *****************************************************************************/

#include "gcomm.h"
#include "majorbbs.h"
#include "elwcht.h"

VOID EXPORT init__elwcht();

GBOOL elwchtchk(VOID);
VOID wldchathelp(VOID), wldccls(VOID), wld_lounge(VOID),
     wld_cht_channel(VOID), wld_do_action(VOID), chatsysopscan(VOID);

struct module elwchatmd={       /* Module interface block           */
	"",                          /* Description for main menu        */
	NULL,                        /* User Logon Supplemental Routine  */
	elwchtchk,                   /* Input routine                    */
	dfsthn,                      /* status-input routine             */
	NULL,                        /* "injoth" routine if selected     */
	NULL,                        /* user logoff supplemental routine */
	NULL,                        /* Hang up routine                  */
	NULL,                        /* Cleanup routine                  */
	NULL,                        /* Delete-account routine           */
	wldccls,                     /* finish-up (sys shutdown) routine */
};

static
HMCVFILE wldchtmsg;

INT wldchtstt, user_list_flag;

struct wldcht {
   char nickname[31];           /* User's Nickname (31 bytes)          */
   char userid[UIDSIZ];         /* User's actual ID - for SysOp        */
};

struct wldcht *chatuser,        /* dynam alloc'd player data structure array */
	      *wldchat,              /* ptr to current user's chatuser element     */
	      *wldcoth;              /* "other" player ptr -- for actions etc.    */

#define VERSION "1.1"

VOID EXPORT
init__elwcht()
{

   stzcpy(elwchatmd.descrp, gmdnam("ELWCHT.MDF"), MNMSIZ);
   wldchtstt = register_module(&elwchatmd);

   wldchtmsg = opnmsg("elwcht.mcv");
   setmbk(wldchtmsg);

   chatuser=(struct wldcht *)alczer(nterms*sizeof(struct wldcht));

   shocst(spr("ELW Chat Lounge v%s",VERSION),"(C) Copyright 2004-2024 Elwynor Technologies - www.elwynor.com");
}

GBOOL elwchtchk(VOID)
{
   setmbk(wldchtmsg);
   wldchat = &(chatuser[usrnum]);
   if (!hasmkey(CHTKEY))
   {
      prfmsg(SORRY);
      outprf(usrnum);
      return(0);
   }
   if (sameas(margv[0], "exit") || sameas(margv[0], "x"))
   {
      prfmsg(EXIT);
      setmem(wldchat,sizeof(struct wldcht),0);
      return(0);
   }
   switch (usrptr->substt)
   {
   case 0:
      prfmsg(GETNAME);
      outprf(usrnum);
      usrptr->substt=1;
      break;
   case 1:
      wldchat = &(chatuser[usrnum]);
      rstrin();
      if (inplen > 30 || inplen < 2)
      {
	 prfmsg(REDO);
	 outprf(usrnum);
	 usrptr->substt = 1;
	 break;
      }
      strcpy(wldchat->nickname, input);
      strcpy(wldchat->userid, usaptr->userid);
      prfmsg(WELCOME);
      outprf(usrnum);
      usrptr->substt = 2;
      wld_cht_channel();
      break;
   case 2:
      usrptr->substt = 2;
      if (margc == 1)
      {
	 if (sameas(margv[0], "?") || sameas(margv[0], "/h"))
	 {
	    wldchathelp();
	    break;
	 }
	 if ((sameas(margv[0], "scan") || sameas(margv[0], "/s")) && usrptr->flags&MASTER) {
	    chatsysopscan();
	    break;
	 }
      }
      if (margc > 1)
      {
         /* Check for a generic action */
         if (sameas(margv[0], "ga"))
         {
            /* We have a generic action */
            wld_do_action();
            break;
         }
      }
      wld_lounge();
      break;
   }
   return(1);
}

VOID wldchathelp(VOID)
{
   setmbk(wldchtmsg);
   if (usrptr->flags&MASTER) {
      prfmsg(SYSHELP);
   } else {
     prfmsg(GLOHELP);
   }
   outprf(usrnum);
}

VOID wldccls(VOID)
{
   clsmsg(wldchtmsg);
}

VOID wld_lounge(VOID)
{
INT count;
char *n;
char scratch[256];
INT gender;


   usaptr=uacoff(usrnum);
   usrptr=usroff(usrnum);
   setmem(scratch, 256, 0);
   n = wldchat->nickname;
   gender = usaptr->sex;
   rstrin();
   if (inplen < 1)
   {
      wld_cht_channel();
      return;
   }
   if (strlen(input) > 0) strcpy(scratch, input);
   for (count = 0; count < nterms; count++)
   {
      usaptr=uacoff(count);
      usrptr=usroff(count);
      if (usrptr->state == wldchtstt)
      {
	 if (usrptr->substt == 2)
	 {
	    othusn=count;
	    if (count == usrnum)
	    {
	       prfmsg(SENT);
	    }
	    else
	    {
	       switch(gender)
	       {
		  case 'M':
		     prfmsg(CHTMS1, n, scratch);
		     break;
		  case 'F':
		     prfmsg(CHTMS2, n, scratch);
		     break;
	       }
	    }
	    outprf(othusn);
	 }
      }
   }
   usaptr=uacoff(usrnum);
   usrptr=usroff(usrnum);
}

VOID wld_cht_channel(VOID)
{
   INT count, first;

   first = 0;
   prfmsg(INCHT);
   outprf(usrnum);
   for (count = 0; count < nterms; count++)
   {
      usaptr=uacoff(count);
      usrptr=usroff(count);
      wldcoth = &(chatuser[count]);
      if (usrptr->state == wldchtstt)
      {
	 if (usrptr->substt == 2)
	 {
	    if (first > 0)
	       prf("[0;1;35m, ");
	    switch(usaptr->sex)
	    {
	       case 'M':
		  prf("[0;1;34m%s", wldcoth->nickname);
		  break;
	       case 'F':
		  prf("[0;1;31m%s", wldcoth->nickname);
		  break;
	    }
	    first++;
	 }
      }
   }
   prf("[0m\r");
   prfmsg(PROMPT);
   usaptr=uacoff(usrnum);
   usrptr=usroff(usrnum);
   outprf(usrnum);
}

VOID chatsysopscan(VOID)
{
   INT count, first;

   first = 0;
   prfmsg(INCHT);
   outprf(usrnum);
   for (count = 0; count < nterms; count++)
   {
      usaptr=uacoff(count);
      usrptr=usroff(count);
      wldcoth = &(chatuser[count]);
      if (usrptr->state == wldchtstt)
      {
	 if (usrptr->substt == 2)
	 {
	    if (first > 0)
	       prf("[0;1;35m, ");
	    switch(usaptr->sex)
	    {
	       case 'M':
  	          prf("[0;1;34m%s (%s)", wldcoth->nickname,wldcoth->userid);
		  break;
	       case 'F':
		  prf("[0;1;31m%s (%s)", wldcoth->nickname,wldcoth->userid);
		  break;
	    }
	    first++;
	 }
      }
   }
   prf("[0m\r");
   prfmsg(PROMPT);
   usaptr=uacoff(usrnum);
   usrptr=usroff(usrnum);
   outprf(usrnum);
}

VOID wld_do_action(VOID)
{
INT count;
char *n;
char scratch[256];
INT gender;


   usaptr=uacoff(usrnum);
   usrptr=usroff(usrnum);
   setmem(scratch, 256, 0);
   n = wldchat->nickname;
   gender = usaptr->sex;
   rstrin();
   if (strlen(input) > 0)
      strcpy(scratch, &input[3]);
   for (count = 0; count < nterms; count++)
   {
      usaptr=uacoff(count);
      usrptr=usroff(count);
      if (usrptr->state == wldchtstt)
      {
	 if (usrptr->substt == 2)
	 {
	    othusn=count;
	    if (count == usrnum)
	    {
               prfmsg(ACTSENT);
	    }
	    else
	    {
	       switch(gender)
	       {
		  case 'M':
                     prfmsg(ACTMS1, n, scratch);
		     break;
		  case 'F':
                     prfmsg(ACTMS2, n, scratch);
		     break;
	       }
	    }
	    outprf(othusn);
	 }
      }
   }
   usaptr=uacoff(usrnum);
   usrptr=usroff(usrnum);
}
