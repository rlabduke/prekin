                             /*PUXMMAIN.c*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

#define EXTERN
    /* that is, EXTERN is blank, so PKIN.H will do initial declarations */

#include "PKIN.h"
#include "PKINDLOG.h"

/***** main ****************************************************************/
int main(int argc, char** argv) /*main is of type int !? 130121*/
{
  /*Ltestprint = TRUE;*/  /*test option, compile as TRUE to invoke,*/
  Ltestprint = 0;  /* as FALSE for distribution, can also set in Kludges */
  Lerr = 0;

  if(Ltestprint) {printf("entered main(argc== %d, argv)\n",argc);}

  mainsetup(&argc, argv); /*____INIT.c*/
     /*calls beginnings(), then control returns here*/
     /*Lnewfile will be set if launched with a file name*/

if(Ltestprint)
{
 printf("->loop: Lcommanded %d, Lnewfile %d, Lnewpass %d, Lgetranges %d\n"
        ,Lcommanded,Lnewfile,Lnewpass,Lgetranges);
}

  /*beginning & final endless event loop: program ends sitting here */
  /*  so messages can be read*/
  for (;;)
  {/*beginning & final endless event loop*/
      mainlooprouter(); /*PKININIT, common router, calls HandleEvent()*/
      if(Lerr) break;
      if(Lquiet==0) /*was Lcommanded 990402*/
          HandleEvent(); /*____MAIN.c*/
  }/*beginning & final endless event loop*/
  return(0);
}
/*___main()_________________________________________________________________*/

/****HandleEvent()***********************************************************/
/*    The main event dispatcher. This routine called   */
/*    repeatedly by the event-controlled loops, (it  handles only one event).*/
void HandleEvent()
{
  XEvent theevent;

  XtAppNextEvent(app_context,&theevent); /* get the next event */
  XtDispatchEvent(&theevent);
}
/*___HandleEvent()__________________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****HandleloopEvent()*******************************************************/
/*    Minor event dispatcher, esp. for aborting endless runs. Called   */
/*    repeatedly by the processing loop, (it  handles only one event). */

int   HandleloopEvent()
{
  int ireturn = 0;
#ifdef UNIX_X11
  XEvent       theevent;
  /*ref: Johnson&Reichard,2nd Ed,Motif,MIS Press,1993, pg 387*/
  KeySym         keysym;
  XComposeStatus compose_status;
  int            length, size;
  char           string[10];
#endif

#ifdef MACINTOSH
  EventRecord  theevent;
  int      ok = 1;
  long charCode = 0;

  if(Lappleevents==0) ok = GetNextEvent( everyEvent, &theevent);         /*m*/
  else                ok = WaitNextEvent(everyEvent, &theevent, 0, nil); /*m*/
  if(ok)
  {/*ok*/
     switch (theevent.what)
     {/*switch*/
        case keyDown:
           charin = (theEvent.message & charCodeMask);
           if ((theEvent.modifiers & cmdKey) != 0)
           {
              ; /*ignore*/
           }
           else if (charin != 0)  /* meaningful text key */
           {
              charCode = BitAnd(theEvent.message, charCodeMask);
              if(charCode==46) /* . */      /* ascii code */
              {
                 ireturn = 1;
              }
           }
        break;
     }/*switch*/
  }/*ok*/
  return(ireturn);
#endif /*MACINTOSH*/
#ifdef UNIX_X11
 if(XtAppPending(app_context)) /*960325*/
 {
  XtAppNextEvent(app_context,&theevent); /* get the next event */
  if(theevent.type == KeyPress)
  {
     size = 1; /*which is guess work , but really need only one byte*/
     length = XLookupString((XKeyEvent*)&theevent
                            ,string,size,&keysym,&compose_status);
     if(length>0 && length<=9)
     {
        string[length] = '\0';
        if( string[0] == '.'  ) /* '.'  program interruption*/
        {
           ireturn = 1;
        }
     }
  }
 }/*960325*/
 return(ireturn);
#endif /*UNIX_X11*/
}
/*___HandleloopEvent()______________________________________________________*/

/****getkeydownstate()*******************************************************/
int    getkeydownstate()
{
  int       ireturn;

   ireturn = 0;
   if(Lquiet==0) /*was Lcommanded before 990402*/
   {
     if( HandleloopEvent() ) ireturn = 1;
   }
   return(ireturn);
}
/*___getkeydownstate()______________________________________________________*/

/****makecursoractive()******************************************************/
void    makecursoractive()
{

#ifdef UNIX_X11
/*
printf("called makecursoractive() with Lcursoractive==%d, activecursor:%ld\n"
,Lcursoractive,activecursor);
*/
 if(Lquiet==0)  /*was Lcommanded before 990402*/
 {
  if(!Lcursoractive)
  {
    XDefineCursor(dpy,drawWindow,activecursor);
    Lcursoractive = 1;
  }
 }
#endif
#ifdef MACINTOSH
    InitCursor(); /*m*/
#endif
#ifdef PCWINDOWS
    SetCursor(activecursor); /*????*/
#endif
}
/*___makecursoractive()_____________________________________________________*/

/****makecursorwait()********************************************************/
void    makecursorwait()
{
#ifdef UNIX_X11
/*
printf("called makecursorwait() with Lcursoractive==%d, waitcursor:%ld\n"
,Lcursoractive,waitcursor);
*/
 if(Lquiet) /*was Lcommanded before 990402*/
 {
  if(Lcursoractive)
  {
    XDefineCursor(dpy,drawWindow,waitcursor);
    Lcursoractive = 0;
  }
 }
#endif
#ifdef MACINTOSH
    SetCursor(*waitcursor); /*m*/
#endif
#ifdef PCWINDOWS
    SetCursor(waitcursor)
#endif
}
/*___makecursorwait()_______________________________________________________*/

/****cursorstandard()*******************************************************/
void    cursorstandard()
{
    makecursoractive();
}
/*___cursorstandard()______________________________________________________*/

/****cursorshowwait()******************************************************/
void    cursorshowwait()
{
    makecursorwait();
}
/*___cursorshowwait()_____________________________________________________*/

/****condition()*************************************************************/
void    condition(char strg[256]){;}
/*___condition()____________________________________________________________*/
/****recondition()***********************************************************/
void    recondition(char strg[256]){;}
/*___recondition()__________________________________________________________*/

/****TickCount()-dummy-******************************************************/
long TickCount()
{
    return(0);
}
/*___TickCount()-dummy-_____________________________________________________*/
