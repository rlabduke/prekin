                      /*PUXMMENU.C*/
#include "PKIN.h"
#include "PKINDLOG.h"

/****myenableitem()**********************************************************/
void myenableitem(Widget themenu, Widget theitem, int thestate)
{
#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
  if(Lquiet==0)  /*was Lcommanded before 990402*/
  {
     XtVaSetValues(theitem,XmNsensitive,thestate,NULL);
  }
#endif /* UNIX_MOTIF 060324 re compile with NOGUI, requires Lquiet=1*/
}
/*___myenableitem()_________________________________________________________*/

/****AdjustMenus()***********************************************************/
void    AdjustMenus()
{
  myenableitem(fileMenu, openItem, !Lgetfile);
  myenableitem(fileMenu, aboutItem, 1);/* info always enabled */
  myenableitem(fileMenu, passItem, Lpassposs);
  myenableitem(fileMenu, launchItem, Lmadekin);
  myenableitem(fileMenu, quitItem, 1);

  myenableitem(optiMenu, optionsaboutItem, 1);
  myenableitem(optiMenu, mutrotItem, 1);

}
/*___AdjustMenus()__________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****mycheckitem()********************************************************/
void    mycheckitem(Widget themenu, Widget theitem, int thestate)
{                                  /*common name: MAC & PC & UNIX*/
#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
  if(Lquiet==0)  /*was Lcommanded before 990402*/
  {
    XmToggleButtonSetState(theitem, thestate, 0);
     /* 0: don't generate a callback*/
  }
#endif /* UNIX_MOTIF 060324 re compile with NOGUI, requires Lquiet=1*/
}
/*___mycheckitem()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****mymodifymenuitem()******************************************************/
void    mymodifymenuitem(Widget themenu, Widget theitem, char thestring[256])
{
#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
     XmString    xmstring;

  if(Lquiet==0)  /*was Lcommanded before 990402*/
  {
     xmstring = XmStringCreateSimple(thestring);
     XtVaSetValues(theitem,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
  }
#endif /* UNIX_MOTIF 060324 re compile with NOGUI, requires Lquiet=1*/
}
/*___mymodifymenuitem()_____________________________________________________*/

/****checkmarkmenu()*********************************************************/
void  checkmarkmenu(int ion,MenuHandle menu, Widget item, int iredraw)
{
#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
  if(Lquiet==0)  /*was Lcommanded before 990402*/
  {
    XmToggleButtonSetState(item, ion, 0);
  }
#endif /* UNIX_MOTIF 060324 re compile with NOGUI, requires Lquiet=1*/
}
/*___checkmarkmenu()________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****resetmenuchecks()************************************/
void    resetmenuchecks()
{
#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
    /*XmToggleButtonSetState( testItem, Ltest, 0);*/
  if(Lquiet==0)  /*was Lcommanded before 990402*/
  {
    AdjustMenus();
  }
#endif /* UNIX_MOTIF 060324 re compile with NOGUI, requires Lquiet=1*/
}
/*___resetmenuchecks()___________________________________*/

/****launchcurrentkinemage()**************************************************/
void launchcurrentkinemage()
{
#ifdef TESTACTIVE
    sprintf(alertstr,"Now launch current kin");
    DoReportDialog(); 
#endif

    sprintf(word,"mage %s &",OutfileStr);
    system(word);
    exit(0);
}            
/*___launchcurrentkinemage()_________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****open_CB******************************************************************/
XtCallbackProc open_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
  Lnewfile = 1; /*flag so Main loop will initiate open file stuff*/
  return(NULL);
}
/*___open_CB_________________________________________________________________*/

/****quit_CB******************************************************************/
XtCallbackProc quit_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
  if(Lquiet==0) 
  {
    if(((XmAnyCallbackStruct *)(call_data))->reason == XmCR_ACTIVATE)
    {
        if(fpinput!=NULL) 
        {
            fclose(fpinput);
        }
        if(fpoutput!=NULL)
        {
            fclose(fpoutput);
        }
        exit(0);
    }
  }
#endif /* UNIX_MOTIF 060324 re compile with NOGUI, requires Lquiet=1*/
  return(NULL);
}
/*___quit_CB_________________________________________________________________*/

/****dummy_CB()***************************************************************/
XtCallbackProc dummy_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
  return(NULL);
}
/*___dummy_CB()______________________________________________________________*/

/****about_CB()***************************************************************/
XtCallbackProc about_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    menuabout(); /*PKINMENU*/
  return(NULL);
}
/*___about_CB()______________________________________________________________*/

/****newpass_CB()************************************************************/
XtCallbackProc newpass_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    Lgetfile = 1; /*must reset to 1 if continue with new pass on same file*/
    Lnewpass = 1;
    Lrangestart = 1;
    npass++;
    rewind(fpinput);
    Linclusive = 1;
  return(NULL);
}
/*___newpass_CB()___________________________________________________________*/

/****launchcurrent_CB()*******************************************************/
XtCallbackProc launchcurrent_CB(Widget w,XtPointer client_d,XtPointer call_d)
{
     if(fpinput!=NULL)
     {
         fclose(fpinput);
     }
     if(fpoutput!=NULL)
     {
            fclose(fpoutput);
     }
     launchcurrentkinemage();
     exit(0);
  return(NULL);
}
/*___launchcurrent_CB()________________________________________________________*/

/****optionsabout_CB()*********************************************************/
XtCallbackProc optionsabout_CB(Widget w,XtPointer client_da,XtPointer call_da)
{
    aboutoptions(); /*PKINMENU*/
  return(NULL);
}
/*___optionsabout_CB()________________________________________________________*/

/****mutrot_CB()***************************************************************/
XtCallbackProc mutrot_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
   menuaboutmutations(0); /*PKINMENU.c*/
   return(NULL);
}
/*___mutrot_CB()______________________________________________________________*/

/****makehelphtml_CB()********************************************************/
XtCallbackProc makehelphtml_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
   writehelptohtmlfile(0); /*PKININIT.c*/
   return(NULL);
}
/*___makehelphtml_CB()_______________________________________________________*/


/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*PUXMMENU.c*/

