/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
#include "PKIN.h"
#include "PKINMENU.h"
#include "PKINDLOG.h"

#define kReverServerClass		'PKIN'
#define kReverServerSignature	'PKIN'
#define kSetTextID				'SETT'
#define kNoTextString		"\p<No Text>"
#define kMaxTextSize		255

/****operatingsystem()********************************************************/
char* operatingsystem()
{
   return(" mac old OS,");
}
/*___operatingsystem()_______________________________________________________*/

/****mainsetup()*************************************************************/
void    mainsetup()
{
    OSErr      myErr;
    long       result;
	AEEventHandlerUPP	HandleAECommandLineUPP;
#ifdef __POWERPC__
    ModalFilterUPP odupp, oaupp, pdupp, qaupp; /*980917*/
#endif        
  
    InitMacintosh(); /*.*/

    Llaunched = 0;
      
    Lappleevents = 0;
    Lcanlaunchmage = 1;
    Lnewfile = 0; /*no file in mind*/
    Lstartfile = 0; /*presume prekin not launched by a file*/
    Lgot_infilename = 0;
    Lgot_infileopen = 0;
    Lgot_outfilename = 0;
    Lgot_outfileopen = 0;
    
    myErr = Gestalt(gestaltAppleEventsAttr, &result);
/*
sprintf(alertstr,"myErr on calling for Gestalt == %d, gestaltAppleEventsAttr == %lx"
                 "\015 result for gestaltAppleEventsAttr == %ld, > g && 0x0001 ??"
                 "\015 g == gestaltAppleEventsPresent == %d",
                 myErr,gestaltAppleEventsAttr,result,gestaltAppleEventsPresent);
DoReportDialog();
*/
    if(myErr != noErr) Lappleevents = 0;
    /* problem  calling gestalt, probably means AppleEvents aren't available either*/
    else
    {
        if( !(result & ( 1L << gestaltAppleEventsPresent ) ) ) Lappleevents = 0;
        else Lappleevents = 1;
        /* see pg 157, Macintosh C Programming Primer, Vol I, 2nd Ed. 1992 Mark & Reed*/
    }
/*
sprintf(alertstr,"Lappleevents == %d",Lappleevents);
DoReportDialog();
*/
    if(Lappleevents)  /*+ server 971223*/
    {/* Install the 4 Required Apple Event Handlers which are in MACFILE.C*/
#ifdef __POWERPC__
        odupp = NewModalFilterProc(HandleAEOpenDoc);
        oaupp = NewModalFilterProc(HandleAEIgnore);
        pdupp = NewModalFilterProc(HandleAEOpenDoc);
        qaupp = NewModalFilterProc(HandleAEQuitApp);

        myErr = AEInstallEventHandler(kCoreEventClass,kAEOpenDocuments,
                           odupp, 0L, false);
        if(myErr != noErr)
        {sprintf(alertstr,"kAEOpenDocuments NOT installed");DoReportDialog();}
        
        myErr = AEInstallEventHandler(kCoreEventClass,kAEOpenApplication,
                          oaupp, 0L, false);
        if(myErr != noErr)
        {sprintf(alertstr,"kAEOpenApplication NOTinstalled");DoReportDialog();}
        
        myErr = AEInstallEventHandler(kCoreEventClass,kAEPrintDocuments,
                          pdupp, 0L, false);
        if(myErr!=noErr)
        {sprintf(alertstr,"kAEPrintDocuments NOT installed");DoReportDialog();}
        
        myErr = AEInstallEventHandler(kCoreEventClass,kAEQuitApplication,
                          qaupp, 0L, false);
        if(myErr!=noErr)
        {sprintf(alertstr,"kAEQuitApplication NOTinstalled");DoReportDialog();}
#else
#ifdef POWERMAC
#define CAST AEEventHandlerUPP
#else
#define CAST
#endif
        myErr = AEInstallEventHandler(kCoreEventClass,kAEOpenDocuments,
                           (CAST)HandleAEOpenDoc, 0L, false);
        if(myErr != noErr)
        {sprintf(alertstr,"kAEOpenDocuments NOT installed");DoReportDialog();}
        
        myErr = AEInstallEventHandler(kCoreEventClass,kAEOpenApplication,
                          (CAST)HandleAEIgnore, 0L, false);
        if(myErr != noErr)
        {sprintf(alertstr,"kAEOpenApplication NOTinstalled");DoReportDialog();}
        
        myErr = AEInstallEventHandler(kCoreEventClass,kAEPrintDocuments,
                          (CAST)HandleAEOpenDoc, 0L, false);
        if(myErr!=noErr)
        {sprintf(alertstr,"kAEPrintDocuments NOT installed");DoReportDialog();}
        
        myErr = AEInstallEventHandler(kCoreEventClass,kAEQuitApplication,
                          (CAST)HandleAEQuitApp, 0L, false);
        if(myErr!=noErr)
        {sprintf(alertstr,"kAEQuitApplication NOTinstalled");DoReportDialog();}
#endif

	HandleAECommandLineUPP = NewAEEventHandlerProc( HandleAECommandLine );
	myErr = AEInstallEventHandler( kReverServerClass, kSetTextID,
				HandleAECommandLineUPP, 0L, false );
	DisposeRoutineDescriptor( HandleAECommandLineUPP );

	    /*myErr = AEInstallEventHandler( kReverServerClass, kSetTextID,
				          (CAST)HandleAECommandLine, 0L, false );*/
        if(myErr!=noErr)
        {    sprintf(alertstr,"kSetTextID handler installation failed"); 
              DoReportDialog();}


    }/* Install the 4 Required Apple Event Handlers + server 971223*/

    /*Has to get back to beginning endless loop to see if EventHandler*/
    /*has a launch AppleEvent waiting*/

    initialvalues(); /*PKININIT.C*/
    getversion(); /*common version number: PKININIT.c */

    SetUpMenus();       /*PKMCMENU.C*/
    AdjustMenus();
    
    SetUptextWindow();  /*PKININIT.C*/

    waitcursor = GetCursor(watchCursor); /*m*/
    SetCursor(*waitcursor); /*m*/
    InitCursor(); /*m*/
    
    Lbegin = 1;
    /*beginnings();*/ /*checks to see if launched with a specific file*/


    /*if(Llaunched == 0)*/
    /*{*/
    /*   aboutprekin(); */ /*PKININIT.c*/

    /*    DoINITreport(); */ /*PKININIT.c*/ /*sets Lnewfile = 1 */

    /*    if(Lexplanation){ doexplanation(); Lnewfile = 0;}*/ /*PKINMAIN.C*/
    /*}*/
    /*finished mainsetup, set flag so will do beginnings() */
    /*Lbegin = 1;*/
    /*beginnings(); called from main waiting loop*/ /*PUXMINIT.C*/
}
/*___mainsetup()____________________________________________________________*/

/****beginnings()*************************************************************/
void    beginnings()
{ 
        Lbegin = 0; /*reset flag*/
       
        IOerr = 1;    /* Begins asumming no file */ 
            /* and various flags set for no legitimate file */
        
        if(LAElaunched) Llaunched = 1;

#ifdef SYSTYEM6
        else Llaunched=(int)ifilelaunched();/*PKMCFILE.C*/
           /*see if invoked by a file under system 6*/
#endif /*SYSTEM6*/

        /*makecursoractive(); */ /*MACMAIN*/
        /*initial dialog box here for always initial box*/
       /* makecursorwait(); */ /*MACMAIN.C*/
        SelectWindow(textWindow);/*anticrash magic, next thing is a message*/
             /* to be shown in the textWindow.   */
        
        if( Llaunched != 0 || Lcommanded != 0) /* invoked by a file */
        {
            
            Lnewfile = 1;
            sprintf(word,"%s"
              CRLF" A request for a PDB file has launched PREKIN"CRLF,version);
            pkintextinsert(word); /*PKMCTEXT.C*/
        
            Lgot_infilename = 1;  /*972123*/
            
            if(Lcommanded != 0)
            {
               sprintf(word,"A commandline requested file: %s\r",NameStr);   
               pkintextinsert(word); /*PKMCTEXT.C*/         
            } 
        }
        else /*Llaunched == 0*/
        {/*NOT launched with a known file name*/
            aboutprekin(); /*loads alertstr[256] */ /*PKININIT.c*/
            DoInitDialog(); /*PKINDLOG.c*/ /*sets Lnewfile = 1 */
            if(Lexplanation){ doexplanation(); Lnewfile = 0;}/*PKINMAIN.C*/
           if(Lnewfile == 0)
           {
              sprintf(word,"%s"
                           CRLF"Use File pull-down menu above"    
                           " to Open a PDB File."CRLF,version);
               pkintextinsert(word); /*PKMCTEXT.C*/
           }
           else /*Lnewfile==1 so will actually put up a new file dialog*/
           {
              sprintf(word,"%s"
                        CRLF"Select a PDB-type File "CRLF,version);

              pkintextinsert(word); /*PKMCTEXT.C*/
           }
        }/*NOT launched with a known file name*/

        /*if( launched == 0)*/ /*makecursoractive()*/;/*MACMAIN.C*/
            /*not invoked by a file*/
}
/*___beginnings()____________________________________________________________*/

/****InitMacintosh()*********************************************PREKIN******/
void InitMacintosh()
{
/*m*/  MaxApplZone();
/*m*/  InitGraf(&qd.thePort);/*qd. part new*/
/*m*/  InitFonts();
/*m*/  FlushEvents(everyEvent, 0);
/*m*/  InitWindows();
/*m*/  InitMenus();
/*m*/  TEInit();
/*m*/  InitDialogs(0L);
/*m*/  InitCursor();
}
/*___InitMacintosh()________________________________________________________*/


/****setdemo()*************************************************************/
void setdemo()
{
            IDEMO = 1;
/*m*/       DisposeWindow(textWindow); 
/*s*/       SetUptextWindow();
/*m*/       SelectWindow(textWindow);
}
/*___setdemo()____________________________________________________________*/



/**** SetUptextWindow() *****************************************************/
void   SetUptextWindow()
{
  int textwinleft, textwintop, textwinright, textwinbottom;
     FontInfo    fontStuff;
     Rect      vTScrollRect;
  
       /* set up on-screen text window */
       dragRect = screenBits.bounds;
    
       textwinleft = 2;
       textwintop = 38;
       textwinright = screenBits.bounds.right;
       textwinbottom = screenBits.bounds.bottom-2;

/*m*/  SetRect( &windowBounds,
       textwinleft,  textwintop,
       textwinright,  textwinbottom ); /* text in upper left of big screen */
  
/*m*/  textWindow = NewWindow( 0L, &windowBounds, "\pPREKIN", true
        , documentProc, (WindowPtr)-1L, false, 0);
/*m*/  /*textWindow = NewWindow( 0L, &windowBounds, "\pTEXT : PREKIN", true
        , documentProc, (WindowPtr)-1L, true, 0);*/

/*m*/  SetPort(textWindow);

       /* define vertical scroll bar */
/*m*/  SetRect(&vTScrollRect,  (*textWindow).portRect.right-15,
              (*textWindow).portRect.top-1,
              (*textWindow).portRect.right+1,
              (*textWindow).portRect.bottom-14);
/*m*/  vtextScroll = NewControl(textWindow,&vTScrollRect,"\p",true
             ,minValue,minValue,maxValue,scrollBarProc,2);

       /* gettextTextarea defines destRect & viewRect in terms of textWindow*/
/*.*/  gettextTextarea();
       /* allocate TERec (TExt Record) for this window */
/*m*/  /*textText = TENew(&destRect, &viewRect);*/
/*m*/  textText = TENew(&textdRect, &textvRect);

       /* store TEHandle into refCon of the window  */
       /* so text and window associated */
/*m*/  SetWRefCon(textWindow, (long)textText);
/*m*/  TEActivate(textText);
  
    /* System Font default: ascent 12, descent 3, leading 1, widMax 15   */
    /* courier 12    use  :         8,         2,         1,         7   */
    /* geneva  12    use  :         9,         3,         4,         10  */
    (*textText)->txFont = 22;    /* 22 = courier */
    /*(*textText)->txFont =  3;*/  /*  3 = geneva  */
     
    if(IDEMO)
    {
      (*textText)->txSize = 24; /* points */
      fontStuff.ascent = 16;
      fontStuff.descent = 4;
      fontStuff.leading = 2;
      fontStuff.widMax = 14;
        }
        else
        {
      (*textText)->txSize = 12; /* points */
      fontStuff.ascent =  8;
      fontStuff.descent = 2;
      fontStuff.leading = 1;
      fontStuff.widMax =  7;
    }
    
    widMax = fontStuff.widMax;
    /* set up for single space */
     textHeight = fontStuff.ascent + fontStuff.descent + fontStuff.leading;
    (*textText)->fontAscent = fontStuff.ascent;
    (*textText)->lineHeight = textHeight;
}
/*___SetUptextWindow()______________________________________________________*/

/****updatethetextwindow()***************************************************/
void updatethetextwindow()
{
/*m*/      BeginUpdate(textWindow);
/*m*/        InvalRect(&textWindow->portRect);
             /*tell window manager window is broken*/
/*.*/        settextScrollBar();
/*.*/        gettextTextarea();
/*.*/        settextTextarea();
/*m*/        TEActivate(textText);
/*m*/        TEUpdate(&textdRect,textText);
/*m*/        ValidRect(&textWindow->portRect); 
             /*tell window manager window is ok*/
/*m*/      EndUpdate(textWindow);
}
/*___updatethetextwindow()__________________________________________________*/

/******textScroller()********************************************************/
pascal void  textScroller(ControlHandle theControl, short partcode)
{
    int        lineWorth, imove = 0, target, linestomove;

      /*settextTextarea();*/

/*m*/ startValue = GetControlValue(theControl);
      /*startValue = GetCtlValue(theControl);*/ 
      viewLines = (textdRect.bottom - textdRect.top)/textHeight;
       /* lines viewed in window */
       
       
      if(nLines < viewLines)         linestomove = 0;
                              /*nLines = # lines total*/
                              /*textLines = # lines off screen*/
      else if(textLines < viewLines) linestomove = textLines;
      else                           linestomove = viewLines;
                              /*viewLines = # lines on screen*/
    
      /*set scope of the thumb movement*/
      lineWorth = (maxValue -minValue)/(textLines); 
                                     /*textLines = # lines off screen*/
                                     /*viewLines = # lines on screen*/
      switch(partcode)
      {
       /*case inUpButton:*/
       case kControlUpButtonPart:
         if(viewLines > nLines) target = minValue;
         else
         {
           if (startValue > minValue)
           {
              imove = +1;
              target = startValue - (imove*lineWorth);
              if( target < minValue) target = minValue;
           }
           else target = minValue;
         }
         break;
       /*case inPageUp:*/
       case kControlPageUpPart:
         if(viewLines > nLines) target = minValue;
         else
         {
           if (startValue > minValue)
           {
              imove = +linestomove; /*full # lines for one page*/
              target = startValue - (imove*lineWorth);
              if( target < minValue)
              {
                 target = minValue;
                 /*calc # lines for fraction of a page left to go*/
                 imove = (startValue - minValue)/(lineWorth);
              }
           }
           else target = minValue;
         }
         break;
       /*case inDownButton:*/
       case kControlDownButtonPart:
         if(viewLines > nLines) target = maxValue;
         else
         {
           if (startValue < maxValue)
           {
              imove = -1; /*imove negative*/
              target = startValue - (imove*lineWorth);
              if( target > maxValue) target = maxValue;
           }
           else target = maxValue;
         }
         break;
       /*case inPageDown:*/
       case kControlPageDownPart:
         if(viewLines > nLines) target = maxValue;
         else
         {
           if (startValue < maxValue)
           {
              imove = -linestomove; /*imove negative*/
                 /*full # lines for one page*/
              target = startValue - (imove*lineWorth);
              if( target > maxValue)
              {
                 target = maxValue;
                 /*calc # lines for fraction of a page left to go*/
                 imove = -(maxValue - startValue)/(lineWorth);
              }
           }
           else target = maxValue;
         }
         break;
      }
      if (imove != 0)
      {
/*m*/    SetControlValue(vtextScroll, target); /* move thumb */
         if (textLines > 0) /* actual lines > possible lines in window */
/*m*/       TEScroll(0, (imove*textHeight), textText);  /* move contents */
      }
  
/*m*/  endValue = GetControlValue(theControl);
}
/*___textScroller()_________________________________________________________*/

/******DotextContentClick()**************************************************/
void  DotextContentClick(Point where)
{
  long      direction;
  short      partcode, varyValue;
  ControlHandle  theControl;
  int         lineshifted, shiftWorth, moveHeight,lineWorth, shiftPlace;

#ifdef __POWERPC__     /*980917*/
    ControlActionUPP tscrollerupp; /*980917*/   
#endif

  
/*m*/  GlobalToLocal(&where);
/*m*/  partcode = FindControl(where,textWindow,&theControl);
       HLock((Handle)theControl);
       if(partcode)  
       {/* in a control region of the window */
/*m*/    /*direction = GetCRefCon(theControl);*/
         direction = GetControlReference(theControl);
         /*if(partcode == inThumb)*/
         if(partcode == kControlIndicatorPart)
         {/* track the thumb box */
/*m*/       startValue = GetControlValue (theControl);
/*m*/       varyValue = TrackControl(theControl,where,NULL);
/*m*/       endValue = GetControlValue(theControl);
            /* thumb box going up:   (startValue - endValue) positive */
            /* thumb box going down: (startValue - endValue) negative */
            lineWorth = (maxValue -minValue)/textLines;
            /* effective #lines can be scrolled*/
            lineshifted = (startValue - endValue)/lineWorth;
            shiftWorth = lineshifted * lineWorth; /* integer arithmetic */
            shiftPlace = startValue - shiftWorth;/*compute new thumb setting*/
            if ((startValue - shiftWorth) <= minValue) shiftPlace = minValue;
            if ((startValue - shiftWorth) >= maxValue) shiftPlace = maxValue;
            if ((endValue - minValue) <= lineWorth) shiftPlace = minValue;
            if ((maxValue - endValue) < lineWorth) shiftPlace = maxValue; 
                                                     /*ware end state!*/
            moveHeight = lineshifted * textHeight;
/*m*/       SetControlValue(theControl, shiftPlace); /* move thumb */
            if (textLines > 1)
/*m*/          TEScroll(0, moveHeight, textText);  /* move contents */
         }/* track the thumb box */
         else if(partcode != 0) /* not in the thumb,*/
         {/*not in the thumb, may be on scroll arrows, or scroll region */
            
#ifdef __POWERPC__     /*980917  ref same tracking routines in MACWIND.c*/
            tscrollerupp  = NewControlActionProc(textScroller);
            varyValue = TrackControl(theControl,where,tscrollerupp);/*980629*/
#else
            varyValue = TrackControl(theControl,where,(ControlActionUPP)textScroller);/*980629*/
#endif
          }/*not in the thumb, may be on scroll arrows, or scroll region */
       }/* in a control region of the window */
       else
       {  /* in the text region of the window */
         ;  /* mouse controlled cut-copy-paste stuff not implemented */
       }
       HUnlock((Handle)theControl);
}
/*___DotextContentClick()___________________________________________________*/

/******settextScrollBar()****************************************************/
void settextScrollBar()
{
  Rect      vTScrollRect;
  
       if (textWindow != FrontWindow())  
         /* un-hilite scroll bars if window not front */
/*m*/    HiliteControl(vtextScroll,255);
       else  /* hilite scroll bars of front window */
/*m*/    HiliteControl(vtextScroll,0);
    
/*m*/  SetRect(&vTScrollRect,  (*textWindow).portRect.right-15,
                (*textWindow).portRect.top-1,
                (*textWindow).portRect.right+1,
                (*textWindow).portRect.bottom-14);
       (**vtextScroll).contrlRect = vTScrollRect;
  
/*m*/  DrawControls(textWindow);
/*m*/  DrawGrowIcon(textWindow);
}
/*___settextScrollBar()_____________________________________________________*/

/****gettextTextarea()*******************************************************/
void gettextTextarea()
{
/*m*/  BlockMove(&textWindow->portRect, &textvRect, sizeof(Rect)); 
       /* define text area of window */
       textvRect.right -= 16;      /* allow for vertical scroll bar */  
/*m*/  BlockMove(&textvRect, &textdRect, sizeof(Rect));
       /* redefine destRect to be that area */
       textdRect.left   += 4;     /* indent for readability */
       textdRect.top   += 4;
       textdRect.right   -= 4;
       textdRect.bottom -= 4;
}
/*___gettextTextarea()______________________________________________________*/

/****settextTextarea()*******************************************************/
void settextTextarea()
{
  int     lineshifted, moveHeight,lineWorth;

       (*textText)->destRect = textdRect; 
       /* into which text is tried to be drawn */
       (*textText)->viewRect = textvRect;
/*m*/  TECalText(textText);
       nLines = (*textText)->nLines + 1;  
       /* total lines of text +1 for seeing bottom*/
       viewLines = (textdRect.bottom - textdRect.top)/textHeight; 
       /* lines viewed in window */
       
       if (nLines <= viewLines) textLines = 1;
       else textLines = nLines - viewLines;
       
       (*textText)->destRect = textdRect; 
       /* into which text is drawn completely*/
/*m*/  TECalText(textText);
/*m*/  startValue = GetControlValue(vtextScroll);

       if(textLines == 1) lineWorth = (maxValue -minValue);
       else lineWorth = (maxValue -minValue)/textLines;
       
       if(lineWorth == 1) lineshifted = (startValue - minValue);
       lineshifted = (startValue - minValue)/lineWorth;
       
       moveHeight = lineshifted * textHeight;          
/*m*/  TEScroll(0, -moveHeight, textText);  /* move contents */
}
/*___settextTextarea()______________________________________________________*/


/****HandleAEOpenDoc()********************************************************/
/*pascal OSErr HandleAEOpenDoc(AppleEvent event, AppleEvent reply, long ref)*/
pascal OSErr HandleAEOpenDoc(AppleEvent *event, AppleEvent *, long)
{
    OSErr stat;
    AEDescList list;
    AEKeyword keywd;
    DescType dtype;
    FSSpec fss;
    long count,i;
    Size size;

/*#define SHOWANDTELL   */

    /*950113*/
  if(!Lindicator)
  {
    sprintf(word," An AppleEvent request for a kinemage "      /*950113*/
                 "file has launched PREKIN ."CRLF);
    pkintextinsert(word);    /*PKMCTEXT.C*/
    Lindicator = 1;
  }
         
/*NOTE: a dialog alert will change what the current highlevelevent is*/

    stat = AEGetParamDesc(event,keyDirectObject,typeAEList,&list);
    if( stat ) return( stat );

    stat = AEGetAttributePtr(event,keyMissedKeywordAttr,
                             typeWildCard, &dtype, nil, 0, &size );
    if( stat != errAEDescNotFound ) 
    {   AEDisposeDesc( &list );
        return( stat? stat : errAEEventNotHandled );
    }
    stat = AECountItems( &list, &count );
    for( i=1; i<=count; i++ )
    {   
        stat = AEGetNthPtr(&list,
                           (long)i,
                           typeFSS,
                           &keywd,
                           &dtype,
                           (Ptr)&fss, 
                           (long)sizeof(fss),
                           &size);

#ifdef SHOWANDTELL
sprintf(alertstr,"AEGetNthPtr:count== %ld stat== %d, "
                 "Sizeof(fss)== %ld, actual size== %ld"
                 "\015keywd== %lx, dtype== %lx"
                ,count
                ,stat
                ,(long)sizeof(fss)
                ,size
                ,keywd
                ,dtype
                );
DoReportDialog();
#endif /*SHOWANDTELL*/

        if( !stat )
        {   
             ConvertFilename(&fss); /*from RasMol2.5 Roger Sayle*/
            
              /*this is the magic that some how recovers the full path name*/
              /*even though the vRefNum and parID return seemingly useless*/
              /*Convert... build full name directly into NameStr, as a C string */
            
#ifdef POWERMAC             
             LMSetCurDirStore(fss.parID);
#else
             CurDirStore = fss.parID;
#endif
               /* (&fss)->vRefNum really is the volume here, can't use SetVol*/
               /*   which works on "working directory" */
               /*System 7 magic global holds "current directory" */
             
             
             LAElaunched = 1;
             Llaunched = 1;
             Lbegin = 0;
             Lnewfile = 1;
             
             Lgot_infilename = 1; /*971223*/
             
             /*PtoCstr((unsigned char *)NameStr);*/ /*C type name from Convert...*/
#ifdef SHOWANDTELL
{
sprintf(alertstr,"AppleEvent file name: %s\015 vRefNum== %d\015 parID== %d"
,NameStr,(&fss)->vRefNum,(&fss)->parID);
DoReportDialog();
}
#endif /*SHOWANDTELL*/
        }
    }
    AEDisposeDesc( &list );         
    return noErr;
}
/*___HandleAEOpenDoc()_______________________________________________________*/

#ifdef UNDEFINED
/********NEW FSSpec file designation ***********/
/*            
StandardFileReply      outputfileinfo
    sfGood:       Boolean;
    sfReplacing:  Boolean;
    sfType:       OSType;
    sfFile:       FSSpec;
    sfScript:     ScriptCode;
    sfFlags:      Integer;    {Finder Flags}
    sfIsFolder:   Boolean;
    sfIsVolume:   Boolean;
FSSpec
    vRefNum:      Integer;
    parID:        LongInt;
    name:         Str63;
    
/*m*/  SetVol( "\p", outputfileinfo.sfFile.parID ); /* change to directory of file */

*/ 
#endif /*UNDEFINED*/

/****HandleAEQuitApp()********************************************************/
pascal OSErr HandleAEQuitApp(AppleEvent *, AppleEvent *, long)
{
    ExitToShell();
    return noErr;
}
/*___HandleAEQuitApp()_______________________________________________________*/

/****HandleAEIgnore()*********************************************************/
pascal OSErr HandleAEIgnore(AppleEvent *, AppleEvent *, long)
{
    return noErr;
}
/*___HandleAEIgnore()________________________________________________________*/

/****************** HandleAECommandLine *******************************************/
pascal OSErr HandleAECommandLine( AppleEvent *event, AppleEvent *reply, long refcon )
{
	DescType		actualtype;
	Size			actualSize;
	Str255			theText;
	OSErr			err;
    char            cmdline[256];
    int             i;
reply = reply;  /*avoid unused error message*/
refcon = refcon;      /*avoid unused error message*/

{    
   sprintf(alertstr,"PREKIN entered HandleAECommandLine"); 
   DoReportDialog();
}

	
	err = AEGetParamPtr( event, keyDirectObject, typeChar,
			&actualtype, (Ptr)(&(theText[ 1 ])), kMaxTextSize, &actualSize );
	if( err != noErr ) return err;
			
	theText[0] = actualSize;
	for(i=0; i<256; i++) cmdline[i] = theText[i];
	PtoCstr((unsigned char *)cmdline);
    dissectcommandline(cmdline);  /*PKININIT*/
    
	return noErr; /*eventually goes back to mainloop*/
}
/*___HandleAECommandLine()________________________________________________________*/


