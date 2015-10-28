/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/***PREKINMACmain.c**********************************************************/

#define EXTERN
    /* that is, EXTERN is blank, so prekin.h will do initial declarations */
    
#include "PKIN.h" /* #include "prekin.h" */
#include "PKINMENU.h"
#include "PKINDLOG.h"
#include <SIOUX.h>   /*980701*/
long    theclockcount(void);

/***** main ****************************************************************/
void main()
{
  int j;
  
  /*Ltestprint = TRUE;*/  /*test option, compile as TRUE to invoke,*/
  Ltestprint = 0;     /* as FALSE for distribution, can also set in Kludges */  
  Lerr =0;
 
  mainsetup(); /*____INIT.c*/
     
  /*Lnewfile will be set by AppleEvent if launched with a file name*/
  /*beginning event loop to get starting conditions sorted out*/
  
  for(j=1 ; j<10 ; j++)
  {
      HandleEvent();  /*which might pick up a HandleAEOpenDoc() event*/
  }

/*#define FAKECOMMANDLINE*/
#ifdef FAKECOMMANDLINE
sprintf(word,"6PFK.pdb 6PFK.mut.kin -scope -show \"sc,hy\" -mutate 35 tyr -chainid b -append");
/*sprintf(word,"-quiet -ribbon -colornc -chainid AB -onegroup -notext 6PFK.pdb 1atest.kin");*/
/*sprintf(word,"-quiet -show \"mc,at,ct,cb[off],lb(green)[off]\" -nosubgroupmasters -chainid _A -model \"_,6,10\" -bval -bcol -colorscale \"green,10.5,red,20,cyan\" -halfbonds -onegroup -notext 6PFK.pdb 1atest.kin");*/
/*sprintf(word,"-quiet -show \"ca,cb,lb(green)[off]\" -chainid _A -model \"6,10\" -bval -bcol -halfbonds -onegroup -notext 1OMNm6.sup 1atest.kin");*/

/*sprintf(word,"-quiet -show \"ca,cb,lb(green)[off]\" -chainid AB -bval -bcol -halfbonds -onegroup -notext 6PFK.pdb 1atest.kin");*/

/*sprintf(word,"-quiet -show \"ca,cb,lb(green)[off]\" -chainid B -bval -bcol -halfbonds -allbuttons -notext 1cruAH 1atest.kin");*/
/*sprintf(word,"-out 1ahelp.kin -quiet -help");*/
if(Ltestprint)
{
   pkintextinsert(word);
   adjusttext(0);  /*1st call 0 to set up, match CRLFs, quick reset of text*/
}

dissectcommandline(word); /*PKININIT*/
#endif /*FAKECOMMANDLINE*/

  /*if(Lbegin)*/ 
  beginnings(); /*HandleAEOpenDoc() sets Lbegin=0 */

  /*final endless event loop: program ends sitting here */
  /*  so messages can be read*/
  for (;;)
  {/*beginning & final endless event loop*/ /*990402*/
      mainlooprouter(); /*PKININIT, common router*/
      if(Lerr) break;
      if(Lquiet==0) /*i.e. NOT quiet so allow online info and control*/
          HandleEvent(); /*____MAIN.c*/
  }/*beginning & final endless event loop*/
}
/*___main()_________________________________________________________________*/

/****HandleEvent()***********************************************************/
/*    The main event dispatcher. This routine called
 *    repeatedly by the event-controlled loops, (it  handles only one event).
 */
void HandleEvent()
{
  int      ok;
  EventRecord  theEvent;
  WindowPtr  theWindow, oldPort;

    OSErr         myErr;
    Boolean       SIOUXDidEvent;  /*980701*/

    HiliteMenu(0); /*m*/ 
    if(Lappleevents == 0) SystemTask();  /*m*/
             /* Handle desk accessories, use with GetNextEvent() */  

    TEIdle(textText);
    
    if(Lappleevents==0) ok = GetNextEvent( everyEvent, &theEvent);          /*m*/
    else                ok = WaitNextEvent(everyEvent, &theEvent, 0xFFFF, nil); /*m*/
    
    if(ok)/*980701*/
    {
        SIOUXDidEvent = SIOUXHandleOneEvent(&theEvent);
    }
    if(ok && !SIOUXDidEvent)/*980701*/
    {/*ok*/
      switch (theEvent.what)
      {/*switch*/

        case osEvt:  /*980915 operating system event*/
           switch ((theEvent.message >> 24) & 0x00FF)
           {/*osEvt switch*/
               case suspendResumeMessage:
                   if((theEvent.message & resumeFlag) == 0)
                   {/*osEvt to suspend PREKIN*/
                       HiliteControl(vtextScroll,255); 
                   }
                   else
                   {/*osEvt to resume PREKIN*/
                          HiliteControl(vtextScroll,0); 
                   }
               break;
               case mouseMovedMessage:
                   ;
               break;
           }/*osEvt switch*/

        break;
        case kHighLevelEvent:
            if(Lappleevents)
            {
                myErr = AEProcessAppleEvent( (EventRecord *)&theEvent );
                        /*handlers in PKMCFILE.C*/
                  /*HandleAEOpenDoc(),HandleAEQuitApp(),HandleAEIgnore()*/

#ifdef TRIAL971223
              {    
                 sprintf(alertstr,"PREKIN high level event:theEvent.message== %ld, myErr %d"
                                  ,theEvent.message,myErr); 
                 DoReportDialog();
              }

#endif /*TRIAL971223*/

            }
        break;

           case mouseDown:
/*.*/        HandleMouseDown(&theEvent);
             break;
        
           case keyDown:
             charin = (theEvent.message & charCodeMask);
             if ((theEvent.modifiers & cmdKey) != 0)
             {
/*P*/          AdjustMenus();
/*P*/          HandleMenu(MenuKey((char) (theEvent.message & charCodeMask)));
             }
             else if (charin != 0)  /* meaningful text key */
             {
/*m*/          if (FrontWindow() == textWindow)  
               /* i.e. textWindow is selected */
               {
/*m*/            TEKey( charin, textText); /* echo */
/*I*/            textstringin();
               }
             }
             break;
      
           case updateEvt:
/*m*/        GetPort(&oldPort);
/*m*/        SetPort(theWindow = (WindowPtr)theEvent.message);
/*m*/        BeginUpdate(theWindow);
/*m*/          InvalRect(&theWindow->portRect);
               /*tell window manager window is broken*/
               if (theWindow == textWindow)
               {
/*.*/            settextScrollBar();
/*.*/            gettextTextarea();
/*.*/            settextTextarea();
/*m*/            TEActivate(textText);
/*m*/            TEUpdate(&textdRect,textText);
/*m*/            ValidRect(&textWindow->portRect); 
                 /*tell window manager window is ok*/
               }
/*m*/        EndUpdate(theWindow);
/*m*/        SetPort(oldPort);
             break;

           case activateEvt:
             theWindow = (WindowPtr)theEvent.message;
/*m*/        GetPort(&oldPort);
/*m*/        SetPort(theWindow = (WindowPtr)theEvent.message);
/*m*/        BeginUpdate(theWindow);
/*m*/          InvalRect(&theWindow->portRect);
               /*tell window manager window is broken*/
/*.*/          if (theWindow == textWindow) settextScrollBar();
/*m*/        EndUpdate(theWindow);
/*m*/        SetPort(oldPort);
             break;

         }
       }
}
/*___HandleEvent()__________________________________________________________*/

/****HandleMouseDown()*******************************************************/
void HandleMouseDown(EventRecord *theEvent) /*Take care of mouseDown events.*/
{
  WindowPtr  theWindow, oldPort;
  int        windowCode;
  long       growResult;
  Rect       limitRect;
  
      SetRect(&limitRect, 80, 80,                                          
              screenBits.bounds.right - screenBits.bounds.left -4,
              screenBits.bounds.bottom - screenBits.bounds.top -24);
  
/*m*/ windowCode = FindWindow (theEvent->where, &theWindow);
      switch (windowCode)
      {
        case inSysWindow: 
/*m*/      SystemClick (theEvent, theWindow);
        break;
      
        case inMenuBar:
/*P*/      AdjustMenus();
/*P*/      HandleMenu(MenuSelect(theEvent->where));
        break;
      
        case inDrag:
/*m*/      DragWindow(theWindow, theEvent->where, &dragRect);
        break;
        
        case inContent:
        {
          if (theWindow != FrontWindow())
/*m*/        SelectWindow(theWindow);
          else
          {
            if (theWindow == textWindow)
            {
/*m*/            SetPort(textWindow);
/*.*/            DotextContentClick( theEvent->where );
            }
          }
        }
        break;
      
        case inGoAway:
/*m*/     if (TrackGoAway(theWindow, theEvent->where))
          {
/*m*/       HideWindow(theWindow);
/*P*/       AdjustMenus();
          }
        break;

        case inGrow:
        {
          if (theWindow == textWindow )
          {
/*m*/             GetPort(&oldPort);
/*m*/             SetPort(textWindow);
/*m*/            growResult = GrowWindow(textWindow
                                         , theEvent->where, &limitRect);
/*m*/            SizeWindow(textWindow,LoWord(growResult)
                             ,HiWord(growResult),0xff);
/*m*/            InvalRect(&textWindow->portRect);
/*.*/            settextScrollBar();
/*.*/            gettextTextarea();
/*.*/            settextTextarea();
/*m*/            SetPort(oldPort);
            }
         }
         break;
       }
}
/*___HandleMouseDown()______________________________________________________*/


/****cursorstandard()*******************************************************/
void    cursorstandard()
{
    InitCursor(); /*m*/
}
/*___cursorstandard()______________________________________________________*/

/****cursorshowwait()******************************************************/
void    cursorshowwait()
{
    SetCursor(*waitcursor); /*m*/
}
/*___cursorshowwait()_____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****HandleloopEvent()*******************************************************/
/*    Minor event dispatcher, esp. for aborting endless runs. Called   */
/*    repeatedly by the processing loop, (it  handles only one event). */
 
int   HandleloopEvent()
{
  int      ok,ireturn;
  EventRecord  theEvent;

  long charCode;
  
  ireturn = 0;
    
    if(Lappleevents==0) ok = GetNextEvent( everyEvent, &theEvent);         /*m*/
    else                ok = WaitNextEvent(everyEvent, &theEvent, 0, nil); /*m*/
    if(ok)
    {/*ok*/
      switch (theEvent.what)
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
}
/*___HandleloopEvent()______________________________________________________*/
       
/****getkeydownstate()*******************************************************/
int    getkeydownstate()
{
    int       ireturn;
    
         ireturn = 0;
         if( HandleloopEvent() ) ireturn = 1;
         return(ireturn);
}
/*___getkeydownstate()______________________________________________________*/

/****waste()*******************************************************************/
void waste(long nsec)
{
   long ticksatstart; /*timing in ticks = 1/60 sec:*/
   int j;
   ticksatstart = theclockcount(); /*from last startup, good for ~1 year*/
   while( (theclockcount() - ticksatstart) < 60*nsec)
   {
      for(j=-32000; j<32000; j++) 
         ;/*waste a little time*/
   }
   
}
/*___waste()__________________________________________________________________*/

/****theclockcount()*****************************/
long    theclockcount()
{
    return( TickCount() );
}
/*___theclockcount()____________________________*/

