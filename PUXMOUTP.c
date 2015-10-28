        /*PUXMOUTP*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
#include "PKIN.h"
#include "PKINDLOG.h"

/****CANCEL_CB()**************************************************************/
XtCallbackProc CANCEL_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
  XtUnmanageChild(w); /*remove the file dialog widget from the screen*/
  XtDestroyWidget(w); /*destroy the file dialog widget*/
#endif /* UNIX_MOTIF  060324 re compile with NOGUI, requires Lquiet=1*/
  return(NULL);
}
/*___CANCEL_CB()_____________________________________________________________*/

/****fileout_OK_CB()**********************************************************/
XtCallbackProc fileout_OK_CB(Widget w,XtPointer client_data,XtPointer call_data)
/*     Widget w;                       */
/*     caddr_t client_data;            */
/*     XmAnyCallbackStruct *call_data; */
{
#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
  char *filename;
  int  j;

  XmFileSelectionBoxCallbackStruct *cbs =
                     (XmFileSelectionBoxCallbackStruct *)call_data;

  XtUnmanageChild(w); /*remove the file dialog widget from the screen*/
  XtDestroyWidget(w); /*destroy the file dialog widget*/

  /*retrieve the filename character string from the compound string format*/
  XmStringGetLtoR(cbs->value, XmSTRING_DEFAULT_CHARSET, &filename);
  for(j=0; j<256; j++)
  {
      OutfileStr[j] = filename[j];
      if(OutfileStr[j]=='\0') break;
  }
  Lgot_outfilename = 1;
  prepareoutputfile();
#endif /* UNIX_MOTIF  060324 re compile with NOGUI, requires Lquiet=1*/
  return(NULL);
}
/*___fileout_OK_CB()_________________________________________________________*/

/****openoutput()*************************************************************/
int    openoutput(void)
{
#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
    static Widget  fileoutdialog=NULL; /*static to remember if dialog created*/
    XmString    xmstring;

    /* invoke the standard dialog for finding a file */
    /* "word" holds message, "OutfileStr" has suggested file name */

    {/*always create dialog*/ /*See D.A.Young,1994,pg238*/
        n = 0;
        XtSetArg(args[n], XmNvisual, progvisual); n++;
        XtSetArg(args[n], XmNdepth, progdepth); n++;
        XtSetArg(args[n], XmNcolormap, colormap); n++;
        fileoutdialog =
              XmCreateFileSelectionDialog(grafwindow,"openFileDialog",args,n);
        XtAddCallback(fileoutdialog,XmNokCallback
                      ,(XtCallbackProc)fileout_OK_CB,NULL);
        XtAddCallback(fileoutdialog,XmNcancelCallback
                      ,(XtCallbackProc)CANCEL_CB,NULL);
    }/*always create dialog*/

    /*force the filter Directory and file mask as specified for this task*/
    xmstring = XmStringCreateSimple(DirStr);
    XmFileSelectionDoSearch(fileoutdialog,xmstring);
    XmStringFree(xmstring);
    /*Now set the invented file name as a starting suggestion*/

     sprintf(word,"output kinemage file:");
     xmstring = XmStringCreateSimple(word);
     XtVaSetValues(fileoutdialog,XmNselectionLabelString,xmstring,NULL);
     XmStringFree(xmstring);
     xmstring = XmStringCreateSimple(OutfileStr);
     XtVaSetValues(fileoutdialog,XmNtextString,xmstring,NULL);
     XmStringFree(xmstring);

    XtManageChild(fileoutdialog); /*puts the file dialog widget on screen */
  /* return(1); 130121 move out of conditional  to avoid compiler warning*/ 
    return(1);
#endif /* UNIX_MOTIF  060324 re compile with NOGUI, requires Lquiet=1*/
    return(0); // this return(0) has two purposes: 1) prevents a compiler warning, function must have return even if the whole ifdef-endif block isn't present, 2) returning non-1 gives a different behavior from when it works right (block otherwise returns 1).  SML DCR 20151028
}
/*___openoutput()____________________________________________________________*/

/****scriptfile_OK_CB()*******************************************************/
XtCallbackProc scriptfile_OK_CB(Widget w,XtPointer client_da,XtPointer call_data)
{
#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
  char *filename;
  int  j;

  XmFileSelectionBoxCallbackStruct *cbs =
                     (XmFileSelectionBoxCallbackStruct *)call_data;
  XtUnmanageChild(w); /*remove the file dialog widget from the screen*/
  XtDestroyWidget(w); /*destroy the file dialog widget*/

  /*retrieve the filename character string from the compound string format*/
  XmStringGetLtoR(cbs->value, XmSTRING_DEFAULT_CHARSET, &filename);
  for(j=0; j<256; j++)
  {
      ScriptoutStr[j] = filename[j];
      if(ScriptoutStr[j]=='\0') break;
  }
           /*if a previous file is open, close it*/
/*c*/      if(fpscriptout != NULL) fclose(fpscriptout);

/*c*/      if ((fpscriptout = fopen(ScriptoutStr,"w")) == NULL)
           {
/*c*/         sprintf(alertstr,"Prekin failed to open file: \n%s                    "
                      ,ScriptoutStr);
/*Ms*/        DoReportDialog();
              Lrangecontrols = 0; /*go on past rangecontrols*/
              Lscriptout = 0; /*can't write a script file*/
           }
           else /* file opened, write header into it */
           {
/*c*/         fprintf(fpscriptout,"{script for coord file: %s }"EOLO,NameStr);
              Lscriptout = 1; /*can write a script file*/
              Lrangecontrols = 1;    /*still rangecontrols*/
           }
#endif /* UNIX_MOTIF  060324 re compile with NOGUI, requires Lquiet=1*/
  Lwaiting = 0;
  return(NULL);
}
/*___scriptfile_OK_CB()______________________________________________________*/

/****openscriptout()*********************************************************/
int    openscriptout(void)
{
#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
 static Widget  scriptfiledialog=NULL; /*static to remember if dialog created*/
 XmString    xmstring;

      Lopenscriptout = 0; /*don't want to return to here*/
      Lscriptout = 0; /*presume no file until proven open*/
      Lrangecontrols = 0;
         /*presume go on past rangecontrol unless can write script*/
    {/*always create dialog*/ /*See D.A.Young,1994,pg238*/
        n = 0;
        XtSetArg(args[n], XmNvisual, progvisual); n++;
        XtSetArg(args[n], XmNdepth, progdepth); n++;
        XtSetArg(args[n], XmNcolormap, colormap); n++;
        scriptfiledialog =
              XmCreateFileSelectionDialog(grafwindow,"ScriptFileDialog",args,n);
        XtAddCallback(scriptfiledialog,XmNokCallback
                      ,(XtCallbackProc)scriptfile_OK_CB,NULL);
        XtAddCallback(scriptfiledialog,XmNcancelCallback
                      ,(XtCallbackProc)CANCEL_CB,NULL);
    }/*always create dialog*/

     sprintf(word,"save script file as:");
     xmstring = XmStringCreateSimple(word);
     XtVaSetValues(scriptfiledialog,XmNselectionLabelString,xmstring,NULL);
     XmStringFree(xmstring);
     xmstring = XmStringCreateSimple(ScriptoutStr);
     XtVaSetValues(scriptfiledialog,XmNtextString,xmstring,NULL);
     XmStringFree(xmstring);

    XtManageChild(scriptfiledialog); /*puts the file dialog widget on screen */
    Lwaiting = 1;
    /* return(1); 130121 move out of conditional to avoid compile warning*/
    return(1);
#endif /* UNIX_MOTIF  060324 re compile with NOGUI, requires Lquiet=1*/
    return(0);  // this return(0) has two purposes: 1) prevents a compiler warning, function must have return even if the whole ifdef-endif block isn't present, 2) returning non-1 gives a different behavior from when it works right (block otherwise returns 1).  SML DCR 20151028

}
/*___openscriptout()________________________________________________________*/

/****helpfile_OK_CB()*******************************************************/
XtCallbackProc helpfile_OK_CB(Widget w,XtPointer client_da,XtPointer call_data)
{
#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
  char *filename;
  int  j;

  XmFileSelectionBoxCallbackStruct *cbs =
                     (XmFileSelectionBoxCallbackStruct *)call_data;
  XtUnmanageChild(w); /*remove the file dialog widget from the screen*/
  XtDestroyWidget(w); /*destroy the file dialog widget*/
  XmStringGetLtoR(cbs->value, XmSTRING_DEFAULT_CHARSET, &filename);
  for(j=0; j<256; j++)
  {
      helpoutStr[j] = filename[j];
      if(helpoutStr[j]=='\0') break;
  }

   /*if a previous file is open, close it*/
   if(fphelpout != NULL) fclose(fphelpout);

   if ((fphelpout = fopen(helpoutStr,"w")) == NULL)
   {
      sprintf(alertstr,"Prekin failed to open file: \n%s",helpoutStr);
      DoReportDialog();
      Lhelpoutopen = 0; /*can't write a help file*/
   }
   else /* file opened */
   {
      Lhelpoutopen = 1;
   }
#endif /* UNIX_MOTIF  060324 re compile with NOGUI, requires Lquiet=1*/
   Lwaiting = 0;
   return(NULL);
}
/*___helpfile_OK_CB()______________________________________________________*/

/****openhelpout()*********************************************************/
int    openhelpout(void)
{
#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
 static Widget  helpfiledialog=NULL; /*static to remember if dialog created*/
 XmString    xmstring;

    sprintf(helpoutStr,"prekinhelp.html");
    Lhelpoutopen = 0; /*don't want to return to here*/
    {/*always create dialog*/ /*See D.A.Young,1994,pg238*/
        n = 0;
        XtSetArg(args[n], XmNvisual, progvisual); n++;
        XtSetArg(args[n], XmNdepth, progdepth); n++;
        XtSetArg(args[n], XmNcolormap, colormap); n++;
        helpfiledialog =
              XmCreateFileSelectionDialog(grafwindow,"HelpFileDialog",args,n);
        XtAddCallback(helpfiledialog,XmNokCallback
                      ,(XtCallbackProc)helpfile_OK_CB,NULL);
        XtAddCallback(helpfiledialog,XmNcancelCallback
                      ,(XtCallbackProc)CANCEL_CB,NULL);
    }/*always create dialog*/

     sprintf(word,"save help file as:");
     xmstring = XmStringCreateSimple(word);
     XtVaSetValues(helpfiledialog,XmNselectionLabelString,xmstring,NULL);
     XmStringFree(xmstring);
     xmstring = XmStringCreateSimple(helpoutStr);
     XtVaSetValues(helpfiledialog,XmNtextString,xmstring,NULL);
     XmStringFree(xmstring);

    XtManageChild(helpfiledialog); /*puts the file dialog widget on screen */
    Lwaiting = 1;
#endif /* UNIX_MOTIF  060324 re compile with NOGUI, requires Lquiet=1*/
   return(1);
}
/*___openhelpout()________________________________________________________*/


