/*PUXMINIT.c*/  /* MOTIF widgets */
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

#include "PKIN.h"
#include "PKINDLOG.h"


#ifdef UNIX_X11
#include <X11/cursorfont.h>
#endif /*UNIX_X11*/

XtCallbackProc launchcurrent_CB(Widget,XtPointer,XtPointer);

/****initcomputer()***********************************************************/
void initcomputer()
{ ; }
/*___initcomputer()__________________________________________________________*/

/****definecursors()*********************************************************/
void    definecursors()
{
 XColor  body, back;
    activecursor = XCreateFontCursor(dpy,XC_top_left_arrow);
    waitcursor = XCreateFontCursor(dpy,XC_watch);
    body.red = 65535;     body.green = 0;     body.blue = 0;
/*
    body.red = 0;     body.green = 0;     body.blue = 0;
*/
    back.red = 65535; back.green = 65535; back.blue = 65535;
    XRecolorCursor(dpy,activecursor,&body,&back);
    XRecolorCursor(dpy,waitcursor,&body,&back);
/*
printf("definecursors: activecursor:%ld, waitcursor:%ld\n"
,activecursor,waitcursor);
*/
}
/*___definecursors()________________________________________________________*/

/****beginnings()*************************************************************/
void    beginnings()
{
  int  launched = 0;

  Lbegin = 0;
  fpinput = NULL;
  IOerr = 1;    /* Begins asumming no file */
            /* and various flags set for no legitimate file */
  Lnewfile = 0;
  launched = Lstartfile; /*gotten from ARGC in MAIN */
  makecursoractive(); /*____MAIN*/
  aboutprekin();         /*PKININIT.c, loads alertstr[256]*/
  /*initial dialog box here for always initial box*/
  /*DoInitDialog();*/  /*DoINITDialog(), old PKMCMSUB.C, PUXMDLOG.c*/ 

if(Ltestprint) printf("start beginnings with launched== %d\n",launched);
  if( launched != 0) /* invoked by a file */
  {
     makecursorwait();  /*____MAIN.C*/
     IOerr = 0; /* putative .kin file available */
     Lnewfile = 1;
     Lstartfile = 1;
     if(!Lquiet) /*was Lcommanded before 990402*/
     {
        sprintf(word,"%s\n A request to make a kinemage " 
                  "file has launched PREKIN.",version);
        cleartexts();      /*PUXMTEXT.C*/
        inserttexts(word,0); /*0 insert, 1 replace */ /*PUXMTEXT.C*/
     }
  }
  else
  {
     /*initial dialog box here for no box on file launch*/
     DoInitDialog();        /*DoINITreport(), old PKMCMSUB.C, PUXMDLOG.c*/ 
     if(Lexplanation)
     { 
        /*990409 This is Never accessed because UNIX NON-MODAL Dialog*/
        /*lets the program flash past before Lexplanation can be set*/
        /*All platforms could do this in common code with a call in the*/
        /*main loop, for instance; or have a menu item that does this*/
        doexplanation(); /*PKINMAIN.C*/
        Lnewfile = 0;  /*990409*/
     }
     else
     {
        sprintf(word,"%s\nUse File pull-down menu above"   
                     " to Open a PDB File.",version);
        cleartexts();         /*PUXMTEXT.C*/
        inserttexts(word,0);    /*PUXMTEXT.C*/
     }
  }
}
/*___beginnings()____________________________________________________________*/

/****mainsetup()*************************************************************/
void    mainsetup(int* argc, char** argv)
{
  int     copy_of_argc, argv_size;
  char**  copy_of_argv;

  int     ncommon;
  int     scrHeight, scrWidth;

if(Ltestprint) printf("entered mainsetup(*argc== %d, argv)\n",*argc);
  initcomputer();        /*____INIT.c*/ /*dummy except for MACINTOSH*/
  initialvalues(); /*pkininit.c*/ 

  Lappleevents = 0;
  Lcanlaunchmage = 1;
  Lnewfile = 0; /*no file in mind*/
  Lstartfile = 0; /*presume prekin not launched by a file*/
  Lin_stdin = 0;
  Lout_stdout = 0;
      Lgot_infilename = 0; /*ready for new set of files*/
      Lgot_outfilename = 0;
      Lgot_infileopen = 0; /*ready for new set of files*/
      Lgot_outfileopen = 0;

  getversion(); /*common version number: PKININIT.c */

  if(*argc>1)
  {
      parsecommandline(argc, argv);
  }
#ifdef NOGUI  /*060324 re compile with NOGUI, requires Lquiet=1*/
   if(!Lquiet)
   {
      fprintf(stderr,"PREKIN %s\n",version);
      fprintf(stderr,"commands required; for usage do: prekin -h\n");
      exit(0);
   }
#endif /* NOGUI 060324 re compile with NOGUI, requires Lquiet=1*/


if(Ltestprint) printf("Prekin past parser with Lquiet == %d\n",Lquiet);
/*was Lcommanded before 990402*/

#ifdef MACINTOSH
/*move the Llaunched && LAElaunched stuff from getnewfile() to here */
#endif /*MACINTOSH*/

#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
if(Lquiet==0) /*was Lcommanded before 990402*/
{/*Need X interface for interactive control (Lquiet==0) */
  /*Initialize X toolkit the long way so can mess with color palette */
  XtToolkitInitialize();
  app_context = XtCreateApplicationContext();
  /*copy commandline parameters*/
  copy_of_argc = *argc;
  argv_size = copy_of_argc * (sizeof(char*));
  copy_of_argv = (char**)XtMalloc(argv_size);
  memcpy(copy_of_argv, argv, argv_size);

  /*open display connection*/
  dpy = XtOpenDisplay(app_context,
        NULL,
        NULL,
        "PREKIN",
        (XrmOptionDescList) NULL,
        0,
        argc,
        argv);

  /*Now can set visual stuff before opening top shell */
  scrHeight = DisplayHeight( dpy, DefaultScreen(dpy));
  scrWidth  = DisplayWidth ( dpy, DefaultScreen(dpy));
  /*this duplicate some of the static initializations in PKININIT.c*/
  if(scrHeight > 700)
  {
      GHEIGHT =  700;  /*  window height*/
  }
  else
  {
      GHEIGHT = scrHeight-100;  /*  window height*/
  }
  if(scrWidth >  700)
  {
      GWIDTH =   700;   /*  effective  window width */
  }
  else
  {
      GWIDTH  = scrWidth-100;   /*  effective window width */
  }
  if(GWIDTH<GHEIGHT) oldmaxwide = GWIDTH;
  else               oldmaxwide = GHEIGHT;
  maxwide = oldmaxwide;

/*Define buttonFont, if possible, 030228 use GHEIGHT rather than GWIDTH*/
if (GHEIGHT<800 && (buttonFont =
    XLoadQueryFont(dpy, "-*-Helvetica-Bold-R-Normal--12*") ) )
{
    /*fprintf(stderr,"little font Helvetica-Bold-R-Normal--12\n");*/
    fontList = XmFontListCreate(buttonFont, XmSTRING_DEFAULT_CHARSET);
}
else if (buttonFont =
    XLoadQueryFont(dpy, "-*-Helvetica-Bold-R-Normal--14*") )
{
    /*fprintf(stderr,"little font Helvetica-Bold-R-Normal--14\n");*/
    fontList = XmFontListCreate(buttonFont, XmSTRING_DEFAULT_CHARSET);
}
else if(buttonFont =
       XLoadQueryFont(dpy, "-*-fixed-bold-r-normal--15*") )
{
    /*fprintf(stderr,"little font fixed-bold-r 15\n");*/
    fontList = XmFontListCreate(buttonFont, XmSTRING_DEFAULT_CHARSET);
}
else if(buttonFont =
       XLoadQueryFont(dpy, "-*-fixed--r-normal--15*") )
{
    /*fprintf(stderr,"little font fixed--r 15\n");*/
    fontList = XmFontListCreate(buttonFont, XmSTRING_DEFAULT_CHARSET);
}
else
{
    /*fprintf(stderr,"using default font for little font\n");*/
    ;
}

/*printf("about to set default Font stuff\n");*/
/*find some bigger fonts*/
oldfontsize = 14;
littlefontsize = 14;
mediumfontsize = 18;
largefontsize = 24;
/*defaults*/
littleFont = buttonFont;
if (buttonFont) littlefontList = fontList;

if (mediumFont =
    XLoadQueryFont(dpy, "-*-Helvetica-Bold-R-Normal--18*") )
{
/*printf("medium font Helvetica-Bold-R-Normal--18\n");*/
    mediumfontList = XmFontListCreate(buttonFont, XmSTRING_DEFAULT_CHARSET);
}
else if( mediumFont =
       XLoadQueryFont(dpy, "-*-fixed-bold-r-normal--18*") )
{
/*printf("medium font fixed-bold-r-normal--18\n");*/
    mediumfontList = XmFontListCreate(mediumFont, XmSTRING_DEFAULT_CHARSET);
}
else if( mediumFont =
       XLoadQueryFont(dpy, "-*-fixed--r-normal--18*") )
{
/*printf("medium font fixed--r-normal--18\n");*/
    mediumfontList = XmFontListCreate(mediumFont, XmSTRING_DEFAULT_CHARSET);
}
else 
{
    mediumFont = buttonFont;
/*printf("medium font == little font\n");*/
    if (buttonFont) mediumfontList = fontList;
}
/*printf("mediumFont== %ld\n",(long)mediumFont);*/

if (largeFont =
    XLoadQueryFont(dpy, "-*-Helvetica-Bold-R-Normal--24*") )
{
/*printf("large font Helvetica-Bold-R-Normal--24\n");*/
    largefontList = XmFontListCreate(buttonFont, XmSTRING_DEFAULT_CHARSET);
}
else if( largeFont =
       XLoadQueryFont(dpy, "-*-fixed-bold-r-normal--24*") )
{
/*printf("large font fixed-bold-r-normal--24\n");*/
    largefontList = XmFontListCreate(largeFont, XmSTRING_DEFAULT_CHARSET);
}
else if( largeFont =
       XLoadQueryFont(dpy, "-*-fixed--r-normal--24*") )
{
/*printf("large font fixed--r-normal--24\n");*/
    largefontList = XmFontListCreate(largeFont, XmSTRING_DEFAULT_CHARSET);
}
else 
{
    largeFont = buttonFont;
/*printf("large font == little font\n");*/
    if (buttonFont) largefontList = fontList;
}
/*printf("largeFont== %ld\n",(long)largeFont);*/

/*printf("screen Height== %d, Width== %d\n",scrHeight,scrWidth);*/
/*discover Visual attributes of this computer */
  progvisual = DefaultVisual(dpy,DefaultScreen(dpy));
  progdepth = DefaultDepth(dpy,DefaultScreen(dpy));
  colormap = DefaultColormap(dpy,DefaultScreen(dpy));
  windowbackground = WhitePixel(dpy,DefaultScreen(dpy));
  windowborder = BlackPixel(dpy,DefaultScreen(dpy));


  /* create Application Shell  named topLevel */
  /* set in commandline parameters */
  n = 0;
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNborderColor, windowborder); n++;
  XtSetArg(args[n], XmNargc, copy_of_argc); n++;
  XtSetArg(args[n], XmNargv, copy_of_argv); n++;
  XtSetArg(args[n], XmNmappedWhenManaged, False); n++;
  XtSetArg(args[n], XmNtitle, "topLevel"); n++;
  XtSetArg(args[n], XmNx, 10); n++;
  XtSetArg(args[n], XmNy, 10); n++;
  XtSetArg(args[n], XmNwidth, GWIDTH); n++; 
  XtSetArg(args[n], XmNheight,GHEIGHT); n++;
  XtSetArg(args[n], XmNallowResize, True); n++;
  topLevel = XtAppCreateShell(NULL,
                             "topLevel",
                             applicationShellWidgetClass,
                             dpy,
                             args,n);

  /*create topgraf as a popup shell under topLevel*/
  n = 0;
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNborderColor, windowborder); n++;
  XtSetArg(args[n], XmNtitle, "PREKIN"); n++;
  XtSetArg(args[n], XmNx, 10); n++;
  XtSetArg(args[n], XmNy, 10); n++;
  XtSetArg(args[n], XmNwidth, GWIDTH); n++; 
  XtSetArg(args[n], XmNheight,GHEIGHT); n++;
  XtSetArg(args[n], XmNallowResize, True); n++;
  XtSetArg(args[n], XmNdeleteResponse, XmDO_NOTHING); n++; 
    /*ignore close commands from frame menu or X button*/
  topgraf = XtCreatePopupShell("topgraf",
                               topLevelShellWidgetClass,
                               topLevel,
                               args, n);

  /*create grafwindow as a form widget to hold textwindow &  menus*/
  n = 0;  /* window title is a regular string */
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNborderColor, windowborder); n++;
  XtSetArg(args[n], XmNtitle, "grafwindow"); n++;
  XtSetArg(args[n], XmNx, 10); n++;
  XtSetArg(args[n], XmNy, 10); n++;
  XtSetArg(args[n], XmNwidth, GWIDTH); n++;
  XtSetArg(args[n], XmNheight,GHEIGHT); n++;
  XtSetArg(args[n], XmNallowResize, True); n++;
  grafwindow = 
    XtCreateManagedWidget("grafwindow",xmFormWidgetClass,topgraf, args, n);

#ifdef UNDEFINED
  SetUpMenus();          /*____MENU.C*/
/****SetUpMenus()*************************************************************/
void SetUpMenus()
{
#endif
/***********************MENU************************************************/
/*create menuBar*/
  n = 0;
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNborderColor, windowborder); n++;
  if (buttonFont){ XtSetArg(args[n], XmNfontList, fontList); n++;}
  ncommon = n; /*use these args for all widgets in menus group */
  XtSetArg(args[n], XmNrowColumnType, XmMENU_BAR); n++;
  XtSetArg(args[n], XmNheight, 30); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  menuBar = XmCreateMenuBar(grafwindow, "menuBar", args, n);
  XtManageChild(menuBar);

/*FILEMENU**********************************************************/
/*create a Pulldown Menu Pane */
  n = ncommon;
  fileMenu = XmCreatePulldownMenu(menuBar, "File", args, n);
  /*do not managechild here, popup level is not complete*/
/*create menu CascadeButton for this Menu Pane*/
  n = ncommon;
  XtSetArg(args[n], XmNsubMenuId, fileMenu); n++;
  fileButton = XmCreateCascadeButton(menuBar, "File", args, n);
  XtManageChild(fileButton);

/*create buttons on this Pulldown Menu Pane*/
  n = ncommon;
  openItem = XmCreatePushButton(fileMenu,"Open File...",args,n);
  XtAddCallback(openItem, XmNactivateCallback,(XtCallbackProc)open_CB, NULL); 
  XtManageChild(openItem);
  
  n = ncommon;
  aboutItem = XmCreatePushButtonGadget(fileMenu,"About PREKIN",args,n);
  XtAddCallback(aboutItem, XmNactivateCallback,(XtCallbackProc)about_CB, NULL);
  XtManageChild(aboutItem);

  n = ncommon;
  passItem = XmCreatePushButtonGadget(fileMenu,"New Pass",args,n);
  XtAddCallback(passItem, XmNactivateCallback,(XtCallbackProc)newpass_CB, NULL);
  XtManageChild(passItem);

  n = ncommon;
  launchItem = 
      XmCreatePushButtonGadget(fileMenu,"launch current kinemage",args,n);
  XtAddCallback(launchItem,XmNactivateCallback
                                 ,(XtCallbackProc)launchcurrent_CB, NULL);
  XtManageChild(launchItem);

  n = ncommon;
  quitItem = XmCreatePushButtonGadget(fileMenu,"Quit",args,n);
  XtAddCallback(quitItem,XmNactivateCallback,(XtCallbackProc)quit_CB, NULL);
  XtManageChild(quitItem);
/*FILEMENU______________________________________________________*/

/*OPTIMENU***********************************************************/
/*create a Pulldown Menu Pane */
  n = ncommon;
  optiMenu = XmCreatePulldownMenu(menuBar, "Options", args, n);

/*create a CascadeButton for this Menu Pane*/
  n = ncommon;
  XtSetArg(args[n], XmNsubMenuId, optiMenu); n++;
  optiButton = XmCreateCascadeButton(menuBar, "Options", args, n);
  XtManageChild(optiButton);

  n = ncommon;
  optionsaboutItem = XmCreatePushButtonGadget(optiMenu,"About Options",args,n);
  XtAddCallback(optionsaboutItem,XmNactivateCallback
                ,(XtCallbackProc)optionsabout_CB,NULL); 
  XtManageChild(optionsaboutItem);

#ifdef TOGGLE
  n = ncommon;  /*Toggle buttons all have same args */
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, (Pixel)1); n++;
  mutrotItem = XmCreateToggleButtonGadget(optiMenu,"mutations",args,n);
  XtAddCallback(mutrotItem,XmNvalueChangedCallback
                                  ,(XtCallbackProc)mutrot_CB, NULL); 
  XtManageChild(mutrotItem);
#endif

  n = ncommon;
  mutrotItem = XmCreatePushButtonGadget(optiMenu,"mutations",args,n);
  XtAddCallback(mutrotItem,XmNactivateCallback
                                  ,(XtCallbackProc)mutrot_CB, NULL); 
  XtManageChild(mutrotItem);

  n = ncommon;
  makehelphtmlItem = XmCreatePushButtonGadget(optiMenu,"Help->HTML...",args,n);
  XtAddCallback(makehelphtmlItem,XmNactivateCallback
                                    ,(XtCallbackProc)makehelphtml_CB,NULL);
  XtManageChild(makehelphtmlItem);


/*___________________________MENU__________________________________________*/
#ifdef UNDEFINED
}
/*___SetUpMenus()____________________________________________________________*/
  SetUptextWindow();     /*____INIT.c, old PKMCMSUB.C*/
/****SetUptextWindow()********************************************************/
void SetUptextWindow()
{
#endif
  /*create textwindow as child of grafwindow, the form window*/
  n = 0;  /* window title is a regular string */
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNborderColor, windowborder); n++;
  XtSetArg(args[n], XmNtitle, "textwindow"); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, 35); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNscrollHorizontal, False); n++;
  XtSetArg(args[n], XmNscrollVertical, True); n++;
  XtSetArg(args[n], XmNscrollingPolicy, XmAUTOMATIC); n++;
  XtSetArg(args[n], XmNeditable, False); n++;
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNautoShowCursorPosition, False); n++;
  XtSetArg(args[n], XmNcursorPositionVisible, False); n++;
  XtSetArg(args[n], XmNwordWrap, True); n++;
  if (buttonFont) { XtSetArg(args[n], XmNfontList, fontList); n++; }
  /* create the textwindow for PREKIN */
  textwindow =
    XmCreateScrolledText(grafwindow,"textwindow", args, n);
  XtManageChild(textwindow);
#ifdef UNDEFINED
}
/*___SetUptextWindow()_______________________________________________________*/
#endif /*UNDEFINED*/

  XtPopup(topgraf,XtGrabNone);
  XtRealizeWidget(topgraf);    /* realize widget hierarchy */
  drawWindow = XtWindow(textwindow);  /* get the d.a. widget's window ID*/

  AdjustMenus(); /*PUXMMENU.c*/ /*deactivate launches, etc. until file comes in*/

   definecursors();  /*PUXMINIT.C*/
   makecursorwait(); /*PUXMMAIN.C*/

}/*Need X interface for interactive control (Lquiet==0) */
#endif /* UNIX_MOTIF  060324 re compile with NOGUI, requires Lquiet=1*/
/*finished mainsetup, set flag so will do beginnings() */
   Lbegin = 1;
   beginnings(); /*PUXMINIT.C*/
}
/*___mainsetup()____________________________________________________________*/



