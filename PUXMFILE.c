                        /*PUXMFILE.c*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
#include "PKIN.h"
#include "PKINDLOG.h"

/****openscriptin()***********************************************************/
int    openscriptin(void)
{
   getfilename("dummy", 3); /* 3 for scriptin */
   return(1);
}
/*___openscriptin()__________________________________________________________*/

/****fileok_CB****************************************************************/
XtCallbackProc fileok_CB(Widget w,XtPointer client_data,XtPointer call_data)
/*     Widget w;                       */
/*     caddr_t client_data;            */
/*     XmAnyCallbackStruct *call_data; */
{
#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
  char *filename;
  char namestrs[256];
  int  j,nfile;
  /*See D.A.Young,1994,pg239*/
  
  XmFileSelectionBoxCallbackStruct *cbs =                           
                     (XmFileSelectionBoxCallbackStruct *)call_data; 
  nfile = *(int *)client_data;

  XtUnmanageChild(w); /*remove the file dialog widget from the screen*/
  XtDestroyWidget(w); /*destroy the file dialog widget*/
  /*retrieve the filename character string from the compound string format*/
  XmStringGetLtoR(cbs->value, XmSTRING_DEFAULT_CHARSET, &filename);
  for(j=0; j<256; j++)
  {
      namestrs[j] = filename[j];
      if(namestrs[j]=='\0') break;
  }
  openafile(namestrs,nfile);  /*PKINFILE.C */
  /*(nfile==1) just opened the input file*/
  /*successful open of input file sets Lgetfile=1, selectfiles() does routing */
  /*successful open of script file sets Lscriptfileopen = 1, Lscriptin = 1 */
  /*successful open of aalibrary file sets Laalibopen = 1 */
  /*iflag = 2;*/ /*global, where == 2 for file open */
#endif /* UNIX_MOTIF 060324 re compile with NOGUI, requires Lquiet=1*/
  Lwaiting = 0;
  return(NULL);
}
/*___fileok_CB_______________________________________________________________*/

/****filecancel_CB************************************************************/
XtCallbackProc filecancel_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    /*See D.A.Young,1994,pg240*/
    XtUnmanageChild(w); /*remove the file dialog widget from the screen*/
    XtDestroyWidget(w); /*destroy the file dialog widget*/
    Lnewfile = 0; /*flag that new file name is NOT available*/
    /*iflag = 0;*/ /*global, where == 0 for file cancel */
    Lwaiting = 0;
    return(NULL);
}
/*___filecancel_CB___________________________________________________________*/

/****getfilename()************************************************************/
int getfilename(char namestrs[256], int nfile)
{
  static Widget dialog = NULL; /*static to remember if dialog created*/
    /*namestrs is redundent here, nfile has to be passed as XtPointer*/
  
  xtptr = &intflag;  /*global XtPointer xtptr and int intflag*/
  intflag = nfile;   /*seems more robust to deal here with globals*/
                     /*so why not just pass the info as globals??*/
 Laalibopen = 0; 
#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
    {/*always create dialog*/
        n = 0;
        XtSetArg(args[n], XmNvisual, progvisual); n++;
        XtSetArg(args[n], XmNdepth, progdepth); n++;
        XtSetArg(args[n], XmNcolormap, colormap); n++;

        dialog = 
            XmCreateFileSelectionDialog(grafwindow,"openFileDialog",args,n);
        XtAddCallback(dialog,XmNokCallback,(XtCallbackProc)fileok_CB,xtptr);
        XtAddCallback(dialog,XmNcancelCallback
                                 ,(XtCallbackProc)filecancel_CB,NULL);
    }
    XtManageChild(dialog); /*puts the file dialog widget on screen */
    /*UNIX_X11 getfilename() completed by fileok_CB() and filecancel_CB()*/

    /*Lwaiting = 1 by routine that opens a non-modal dialog box*/
    /*Lwaiting = 0 by routine that is called back by the dialog box*/
    /* after dialog box business is finished */
    Lwaiting = 1;
/*printf("getfilename() returns with Lwaiting== %d\n",Lwaiting);*/
#endif /* UNIX_MOTIF 060324 re compile with NOGUI, requires Lquiet=1*/
    return(1);
}
/*___getfilename()___________________________________________________________*/

/****getnewfile()************************************************************/
int getnewfile(char TheStr[256], int itype)  
{/*get new input file*/
  /* int iflag is a global so can be set in call back routines, etc.*/

  int iflag = 1;/*common PKIN usage meaning file neither selected nor canceled*/
  IOerr = 1; 
  /* Begins with no file and this flag set for no legitimate file*/
  /*makecursoractive();*/ /*____MAIN.c*/

  if(itype==1 && Lstartfile) 
  {/* name of input file in global NameStr, type is 1*/
     Llaunched = 0;   /*clear flags so could select file if launch fails*/
     LAElaunched = 0;
     Lstartfile = 0;
     iflag = 2;
     openafile(NameStr,1);  /*PKINFILE.C */
     /*just opened the input file, successful sets Lgetfile=1,*/
     /* selectfile routes to open outputfile file*/
  }
  else 
  {
     /*Laalibopen = -1;*/ /*wierd, but not using external library now anyway*/
     /*if(Laalibopen == -1) getfilename(TheStr,itype); comment out 070726 */
     getfilename(TheStr,itype); /*070726*/
     /*calls up file dialog, Callback will call openafile*/
     if(Laalibopen ==1) iflag = 2;
  }
  /*Lwaiting = 1 by routine that opens a non-modal dialog box*/
  /*Lwaiting = 0 by routine that is called back by the dialog box*/
  /* after dialog box business is finished */
/*printf("getnewfile:pre__while Lwaiting==%d\n",Lwaiting);*/
  while(Lwaiting)
  {
      HandleEvent(); /*____MAIN.c*/
      /*sets global iflag, fileok_CB: iflag=2, filecancel_CB: iflag=2*/
  }
/*printf("getnewfile:post_while Lwaiting==%d\n",Lwaiting);*/

  return(iflag);
}
/*___getnewfile()___________________________________________________________*/

