                           /*PUXMDLOG.c*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

#include "PKIN.h"
#include "PKINDLOG.h"

/*prototypes*/
void adjustgenericradios(char[32]);
void  genericdlogcleaner(void);

/*MAC and PC dialogs retain control during dialog interaction */
/*UNIX X dialogs pass control to the separate callback routines */
/* so the various callback routines servicing the dialogs using generic */
/* dialog boxes must each be able to know which specific dialog is current*/
/* fortunately, the routines that just respond to keystrokes or mouse clicks */
/* need only to maintain generic parameters.  Only routines that end the */
/* dialog need to pass control back to the correct current specific dialog */
/* these are the OK, OPTION, and CANCEL callbacks which set global flags, and*/
/* have to get things plugged back into the currect specific dialog logic */

/****genericdlogcleaner()*****************************************************/ 
void  genericdlogcleaner() /*UNIX only*/
{
  XtDestroyWidget(genericbox);
 
  genericdlogrouter(); /*PKINDLOG.c*/
}
/*___genericdlogcleaner()____________________________________________________*/

#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
/* start a whole bunch of motif-specific routines */

/****DLOG_PARAMA_CB()*********************************************************/
XtCallbackProc DLOG_PARAMA_CB(Widget w,XtPointer client ,XtPointer call)
{
 strcpy( dlog_paramAstr, XmTextGetString(DLOG_PARAMA));
 return(NULL);
}
/*___DLOG_PARAMA_CB()________________________________________________________*/

/****DLOG_PARAMB_CB()*********************************************************/
XtCallbackProc DLOG_PARAMB_CB(Widget w,XtPointer client ,XtPointer call_data)
{
 strcpy(          dlog_paramBstr ,XmTextGetString(DLOG_PARAMB));
 return(NULL);
}
/*___DLOG_PARAMB_CB()________________________________________________________*/

/****DLOG_PARAMC_CB()*********************************************************/
XtCallbackProc DLOG_PARAMC_CB(Widget w,XtPointer client ,XtPointer call_data)
{
 strcpy(          dlog_paramCstr ,XmTextGetString(DLOG_PARAMC));
 return(NULL);
}
/*___DLOG_PARAMC_CB()________________________________________________________*/

/*****************DLOG_PARAMD_CB()********************************************/
XtCallbackProc    DLOG_PARAMD_CB(Widget w,XtPointer client ,XtPointer call)
{
 strcpy(          dlog_paramDstr ,XmTextGetString(DLOG_PARAMD));
 return(NULL);
}
/*________________DLOG_PARAMD_CB()___________________________________________*/

/*****************DLOG_PARAME_CB()********************************************/
XtCallbackProc    DLOG_PARAME_CB(Widget w,XtPointer client ,XtPointer call)
{
 strcpy(          dlog_paramEstr ,XmTextGetString(DLOG_PARAME));
 return(NULL);
}
/*________________DLOG_PARAME_CB()___________________________________________*/

/*****************DLOG_PARAMF_CB()********************************************/
XtCallbackProc    DLOG_PARAMF_CB(Widget w,XtPointer client ,XtPointer call)
{
 strcpy(          dlog_paramFstr ,XmTextGetString(DLOG_PARAMF));
 return(NULL);
}
/*________________DLOG_PARAMF_CB()___________________________________________*/

/*****************DLOG_PARAMG_CB()********************************************/
XtCallbackProc    DLOG_PARAMG_CB(Widget w,XtPointer client ,XtPointer call)
{
 strcpy(          dlog_paramGstr ,XmTextGetString(DLOG_PARAMG));
 return(NULL);
}
/*________________DLOG_PARAMG_CB()___________________________________________*/

/*****************DLOG_PARAMH_CB()********************************************/
XtCallbackProc    DLOG_PARAMH_CB(Widget w,XtPointer client ,XtPointer call)
{
 strcpy(          dlog_paramHstr ,XmTextGetString(DLOG_PARAMH));
 return(NULL);
}
/*________________DLOG_PARAMH_CB()___________________________________________*/

/*****************DLOG_PARAMI_CB()********************************************/
XtCallbackProc    DLOG_PARAMI_CB(Widget w,XtPointer client ,XtPointer call)
{
 strcpy(          dlog_paramIstr ,XmTextGetString(DLOG_PARAMI));
 return(NULL);
}
/*________________DLOG_PARAMI_CB()___________________________________________*/

/****DLOG_CHECKA_CB()*********************************************************/
XtCallbackProc DLOG_CHECKA_CB(Widget w,XtPointer cl ,XtPointer ca)
{/*toggle button maintains its own state*/
 dlog_checkAint = XmToggleButtonGetState(DLOG_CHECKA);
 return(NULL);
}
/*___DLOG_CHECKA_CB()________________________________________________________*/

/****DLOG_CHECKB_CB()*********************************************************/
XtCallbackProc DLOG_CHECKB_CB(Widget w,XtPointer cl ,XtPointer ca)
{/*toggle button maintains its own state*/
 dlog_checkBint = XmToggleButtonGetState(DLOG_CHECKB);
 return(NULL);
}
/*___DLOG_CHECKB_CB()________________________________________________________*/

/****DLOG_CHECKC_CB()*********************************************************/
XtCallbackProc DLOG_CHECKC_CB(Widget w,XtPointer cl ,XtPointer ca)
{/*toggle button maintains its own state*/
 dlog_checkCint = XmToggleButtonGetState(DLOG_CHECKC); 
 return(NULL);
}
/*___DLOG_CHECKC_CB()________________________________________________________*/

/****DLOG_CHECKD_CB()*********************************************************/
XtCallbackProc DLOG_CHECKD_CB(Widget w,XtPointer cl ,XtPointer ca)
{/*toggle button maintains its own state*/
 dlog_checkDint = XmToggleButtonGetState(DLOG_CHECKD); 
 return(NULL);
}
/*___DLOG_CHECKD_CB()________________________________________________________*/

/****DLOG_CHECKE_CB()*********************************************************/
XtCallbackProc DLOG_CHECKE_CB(Widget w,XtPointer cl ,XtPointer ca)
{/*toggle button maintains its own state*/
 dlog_checkEint = XmToggleButtonGetState(DLOG_CHECKE); 
 return(NULL);
}
/*___DLOG_CHECKE_CB()________________________________________________________*/

/****DLOG_CHECKF_CB()*********************************************************/
XtCallbackProc DLOG_CHECKF_CB(Widget w,XtPointer cl ,XtPointer ca)
{/*toggle button maintains its own state*/
 dlog_checkFint = XmToggleButtonGetState(DLOG_CHECKF); 
 return(NULL);
}
/*___DLOG_CHECKF_CB()________________________________________________________*/

/****DLOG_CHECKG_CB()*********************************************************/
XtCallbackProc DLOG_CHECKG_CB(Widget w,XtPointer cl ,XtPointer ca)
{/*toggle button maintains its own state*/
 dlog_checkGint = XmToggleButtonGetState(DLOG_CHECKG); 
 return(NULL);
}
/*___DLOG_CHECKG_CB()________________________________________________________*/

/****DLOG_CHECKH_CB()*********************************************************/
XtCallbackProc DLOG_CHECKH_CB(Widget w,XtPointer cl ,XtPointer ca)
{/*toggle button maintains its own state*/
 dlog_checkHint = XmToggleButtonGetState(DLOG_CHECKH); 
 return(NULL);
}
/*___DLOG_CHECKH_CB()________________________________________________________*/

/****DLOG_CHECKI_CB()*********************************************************/
XtCallbackProc DLOG_CHECKI_CB(Widget w,XtPointer cl ,XtPointer ca)
{/*toggle button maintains its own state*/
 dlog_checkIint = XmToggleButtonGetState(DLOG_CHECKI); 
 return(NULL);
}
/*___DLOG_CHECKI_CB()________________________________________________________*/

/****DLOG_CHECKJ_CB()*********************************************************/
XtCallbackProc DLOG_CHECKJ_CB(Widget w,XtPointer cl ,XtPointer ca)
{/*toggle button maintains its own state*/
 dlog_checkJint = XmToggleButtonGetState(DLOG_CHECKJ); 
 return(NULL);
}
/*___DLOG_CHECKJ_CB()________________________________________________________*/

/****DLOG_CHECKK_CB()*********************************************************/
XtCallbackProc DLOG_CHECKK_CB(Widget w,XtPointer cl ,XtPointer ca)
{/*toggle button maintains its own state*/
 dlog_checkKint = XmToggleButtonGetState(DLOG_CHECKK); 
 return(NULL);
}
/*___DLOG_CHECKK_CB()________________________________________________________*/

/****DLOG_CHECKL_CB()*********************************************************/
XtCallbackProc DLOG_CHECKL_CB(Widget w,XtPointer cl ,XtPointer ca)
{/*toggle button maintains its own state*/
 dlog_checkLint = XmToggleButtonGetState(DLOG_CHECKL); 
 return(NULL);
}
/*___DLOG_CHECKL_CB()________________________________________________________*/

/****DLOG_CHECKM_CB()*********************************************************/
XtCallbackProc DLOG_CHECKM_CB(Widget w,XtPointer cl ,XtPointer ca)
{/*toggle button maintains its own state*/
 dlog_checkMint = XmToggleButtonGetState(DLOG_CHECKM); 
 return(NULL);
}
/*___DLOG_CHECKM_CB()________________________________________________________*/

/****DLOG_CHECKN_CB()*********************************************************/
XtCallbackProc DLOG_CHECKN_CB(Widget w,XtPointer cl ,XtPointer ca)
{/*toggle button maintains its own state*/
 dlog_checkNint = XmToggleButtonGetState(DLOG_CHECKN); 
 return(NULL);
}
/*___DLOG_CHECKN_CB()________________________________________________________*/

/****DLOG_CHECKO_CB()*********************************************************/
XtCallbackProc DLOG_CHECKO_CB(Widget w,XtPointer cl ,XtPointer ca)
{/*toggle button maintains its own state*/
 dlog_checkOint = XmToggleButtonGetState(DLOG_CHECKO); 
 return(NULL);
}
/*___DLOG_CHECKO_CB()________________________________________________________*/

/****DLOG_CHECKP_CB()*********************************************************/
XtCallbackProc DLOG_CHECKP_CB(Widget w,XtPointer cl ,XtPointer ca)
{/*toggle button maintains its own state*/
 dlog_checkPint = XmToggleButtonGetState(DLOG_CHECKP); 
 return(NULL);
}
/*___DLOG_CHECKP_CB()________________________________________________________*/

/****DLOG_CHECKQ_CB()*********************************************************/
XtCallbackProc DLOG_CHECKQ_CB(Widget w,XtPointer cl ,XtPointer ca)
{/*toggle button maintains its own state*/
 dlog_checkQint = XmToggleButtonGetState(DLOG_CHECKQ); 
 return(NULL);
}
/*___DLOG_CHECKQ_CB()________________________________________________________*/

/****DLOG_CHECKR_CB()*********************************************************/
XtCallbackProc DLOG_CHECKR_CB(Widget w,XtPointer cl ,XtPointer ca)
{/*toggle button maintains its own state*/
 dlog_checkRint = XmToggleButtonGetState(DLOG_CHECKR); 
 return(NULL);
}
/*___DLOG_CHECKR_CB()________________________________________________________*/

/****DLOG_CHECKS_CB()*********************************************************/
XtCallbackProc DLOG_CHECKS_CB(Widget w,XtPointer cl ,XtPointer ca)
{/*toggle button maintains its own state*/
 dlog_checkSint = XmToggleButtonGetState(DLOG_CHECKS); 
 return(NULL);
}
/*___DLOG_CHECKS_CB()________________________________________________________*/

/****DLOG_CHECKT_CB()*********************************************************/
XtCallbackProc DLOG_CHECKT_CB(Widget w,XtPointer cl ,XtPointer ca)
{/*toggle button maintains its own state*/
 dlog_checkTint = XmToggleButtonGetState(DLOG_CHECKT); 
 return(NULL);
}
/*___DLOG_CHECKT_CB()________________________________________________________*/

/****DLOG_CHECKU_CB()*********************************************************/
XtCallbackProc DLOG_CHECKU_CB(Widget w,XtPointer cl ,XtPointer ca)
{/*toggle button maintains its own state*/
 dlog_checkUint = XmToggleButtonGetState(DLOG_CHECKU); 
 return(NULL);
}
/*___DLOG_CHECKU_CB()________________________________________________________*/

/****DLOG_CHECKV_CB()*********************************************************/
XtCallbackProc DLOG_CHECKV_CB(Widget w,XtPointer cl ,XtPointer ca)
{/*toggle button maintains its own state*/
 dlog_checkVint = XmToggleButtonGetState(DLOG_CHECKV); 
 return(NULL);
}
/*___DLOG_CHECKV_CB()________________________________________________________*/

/****DLOG_CHECKW_CB()*********************************************************/
XtCallbackProc DLOG_CHECKW_CB(Widget w,XtPointer cl ,XtPointer ca)
{/*toggle button maintains its own state*/
 dlog_checkWint = XmToggleButtonGetState(DLOG_CHECKW); 
 return(NULL);
}
/*___DLOG_CHECKW_CB()________________________________________________________*/

/****DLOG_CHECKX_CB()*********************************************************/
XtCallbackProc DLOG_CHECKX_CB(Widget w,XtPointer cl ,XtPointer ca)
{/*toggle button maintains its own state*/
 dlog_checkXint = XmToggleButtonGetState(DLOG_CHECKX); 
 return(NULL);
}
/*___DLOG_CHECKX_CB()________________________________________________________*/

/****adjustgenericradios()****************************************************/
void adjustgenericradios(char s[32])
{/*Sets all logicals to OFF that are part of the set, except*/
 /*the first, which is set to be ON if previously OFF, else OFF also*/
   if(Ldlog_radioA && strchr(s,'A')!=NULL)
      XmToggleButtonSetState(DLOG_RADIOA 
      ,dlog_radioAint=(s[0]=='A'&&!dlog_radioAint),0);
   if(Ldlog_radioB && strchr(s,'B')!=NULL)
      XmToggleButtonSetState(DLOG_RADIOB
      ,dlog_radioBint=(s[0]=='B'&&!dlog_radioBint),0);
   if(Ldlog_radioC && strchr(s,'C')!=NULL)
      XmToggleButtonSetState(DLOG_RADIOC
      ,dlog_radioCint=(s[0]=='C'&&!dlog_radioCint),0);
   if(Ldlog_radioD && strchr(s,'D')!=NULL)
      XmToggleButtonSetState(DLOG_RADIOD
      ,dlog_radioDint=(s[0]=='D'&&!dlog_radioDint),0);
   if(Ldlog_radioE && strchr(s,'E')!=NULL)
      XmToggleButtonSetState(DLOG_RADIOE
      ,dlog_radioEint=(s[0]=='E'&&!dlog_radioEint),0);
   if(Ldlog_radioF && strchr(s,'F')!=NULL)
      XmToggleButtonSetState(DLOG_RADIOF
      ,dlog_radioFint=(s[0]=='F'&&!dlog_radioFint),0);
   if(Ldlog_radioG && strchr(s,'G')!=NULL)
      XmToggleButtonSetState(DLOG_RADIOG
      ,dlog_radioGint=(s[0]=='G'&&!dlog_radioGint),0);
   if(Ldlog_radioH && strchr(s,'H')!=NULL)
      XmToggleButtonSetState(DLOG_RADIOH
      ,dlog_radioHint=(s[0]=='H'&&!dlog_radioHint),0);
   if(Ldlog_radioI && strchr(s,'I')!=NULL)
      XmToggleButtonSetState(DLOG_RADIOI
      ,dlog_radioIint=(s[0]=='I'&&!dlog_radioIint),0);
   if(Ldlog_radioJ && strchr(s,'J')!=NULL)
      XmToggleButtonSetState(DLOG_RADIOJ
      ,dlog_radioJint=(s[0]=='J'&&!dlog_radioJint),0);
   if(Ldlog_radioK && strchr(s,'K')!=NULL)
      XmToggleButtonSetState(DLOG_RADIOK
      ,dlog_radioKint=(s[0]=='K'&&!dlog_radioKint),0);
   if(Ldlog_radioL && strchr(s,'L')!=NULL)
      XmToggleButtonSetState(DLOG_RADIOL
      ,dlog_radioLint=(s[0]=='L'&&!dlog_radioLint),0);
   if(Ldlog_radioM && strchr(s,'M')!=NULL)
      XmToggleButtonSetState(DLOG_RADIOM
      ,dlog_radioMint=(s[0]=='M'&&!dlog_radioMint),0);
   if(Ldlog_radioN && strchr(s,'N')!=NULL)
      XmToggleButtonSetState(DLOG_RADION
      ,dlog_radioNint=(s[0]=='N'&&!dlog_radioNint),0);
   if(Ldlog_radioO && strchr(s,'O')!=NULL)
      XmToggleButtonSetState(DLOG_RADIOO
      ,dlog_radioOint=(s[0]=='O'&&!dlog_radioOint),0);
   if(Ldlog_radioP && strchr(s,'P')!=NULL)
      XmToggleButtonSetState(DLOG_RADIOP
      ,dlog_radioPint=(s[0]=='P'&&!dlog_radioPint),0);

}
/*___adjustgenericradios()___________________________________________________*/ 

/****DLOG_RADIOA_CB()*********************************************************/
XtCallbackProc DLOG_RADIOA_CB(Widget w,XtPointer cl,XtPointer ca)
{
   adjustgenericradios("ABCDEFGHIJKL");
 return(NULL);
}
/*___DLOG_RADIOA_CB()________________________________________________________*/

/****DLOG_RADIOB_CB()*********************************************************/
XtCallbackProc DLOG_RADIOB_CB(Widget w,XtPointer cl,XtPointer ca)
{
   adjustgenericradios("BACDEFGHIJKL");
 return(NULL);
}
/*___DLOG_RADIOB_CB()________________________________________________________*/

/****DLOG_RADIOC_CB()*********************************************************/
XtCallbackProc DLOG_RADIOC_CB(Widget w,XtPointer cl,XtPointer ca)
{
   adjustgenericradios("CABDEFGHIJKL");
 return(NULL);
}
/*___DLOG_RADIOC_CB()________________________________________________________*/

/****DLOG_RADIOD_CB()*********************************************************/
XtCallbackProc DLOG_RADIOD_CB(Widget w,XtPointer cl,XtPointer ca)
{
   adjustgenericradios("DABCEFGHIJKL");
 return(NULL);
}
/*___DLOG_RADIOD_CB()________________________________________________________*/

/****DLOG_RADIOE_CB()*********************************************************/
XtCallbackProc DLOG_RADIOE_CB(Widget w,XtPointer cl,XtPointer ca)
{
   adjustgenericradios("EABCDFGHIJKL");
 return(NULL);
}
/*___DLOG_RADIOE_CB()________________________________________________________*/

/****DLOG_RADIOF_CB()*********************************************************/
XtCallbackProc DLOG_RADIOF_CB(Widget w,XtPointer cl,XtPointer ca)
{
   adjustgenericradios("FABCDEGHIJKL");
 return(NULL);
}
/*___DLOG_RADIOF_CB()________________________________________________________*/

/****DLOG_RADIOG_CB()*********************************************************/
XtCallbackProc DLOG_RADIOG_CB(Widget w,XtPointer cl,XtPointer ca)
{
   adjustgenericradios("GABCDEFHIJKL");
 return(NULL);
}
/*___DLOG_RADIOG_CB()________________________________________________________*/

/****DLOG_RADIOH_CB()*********************************************************/
XtCallbackProc DLOG_RADIOH_CB(Widget w,XtPointer cl,XtPointer ca)
{
   adjustgenericradios("HABCDEFGIJKL");
 return(NULL);
}
/*___DLOG_RADIOH_CB()________________________________________________________*/

/****DLOG_RADIOI_CB()*********************************************************/
XtCallbackProc DLOG_RADIOI_CB(Widget w,XtPointer cl,XtPointer ca)
{
   adjustgenericradios("IABCDEFGHJKL");
 return(NULL);
}
/*___DLOG_RADIOI_CB()________________________________________________________*/

/****DLOG_RADIOJ_CB()*********************************************************/
XtCallbackProc DLOG_RADIOJ_CB(Widget w,XtPointer cl,XtPointer ca)
{
   adjustgenericradios("JABCDEFGHIKL");
 return(NULL);
}
/*___DLOG_RADIOJ_CB()________________________________________________________*/

/****DLOG_RADIOK_CB()*********************************************************/
XtCallbackProc DLOG_RADIOK_CB(Widget w,XtPointer cl,XtPointer ca)
{
   adjustgenericradios("KABCDEFGHIJL");
 return(NULL);
}
/*___DLOG_RADIOK_CB()________________________________________________________*/

/****DLOG_RADIOL_CB()*********************************************************/
XtCallbackProc DLOG_RADIOL_CB(Widget w,XtPointer cl,XtPointer ca)
{
   adjustgenericradios("LABCDEFGHIJK");
 return(NULL);
}
/*___DLOG_RADIOL_CB()________________________________________________________*/

/****DLOG_RADIOM_CB()*********************************************************/
XtCallbackProc DLOG_RADIOM_CB(Widget w,XtPointer cl,XtPointer ca)
{
   adjustgenericradios("MNOP");
 return(NULL);
}
/*___DLOG_RADIOM_CB()________________________________________________________*/

/****DLOG_RADION_CB()*********************************************************/
XtCallbackProc DLOG_RADION_CB(Widget w,XtPointer cl,XtPointer ca)
{
   adjustgenericradios("NMOP");
 return(NULL);
}
/*___DLOG_RADION_CB()________________________________________________________*/

/****DLOG_RADIOO_CB()*********************************************************/
XtCallbackProc DLOG_RADIOO_CB(Widget w,XtPointer cl,XtPointer ca)
{
   adjustgenericradios("OMNP");
 return(NULL);
}
/*___DLOG_RADIOO_CB()________________________________________________________*/

/****DLOG_RADIOP_CB()*********************************************************/
XtCallbackProc DLOG_RADIOP_CB(Widget w,XtPointer cl,XtPointer ca)
{
   adjustgenericradios("PMNO");
 return(NULL);
}
/*___DLOG_RADIOP_CB()________________________________________________________*/

/****DLOG_OK_CB()************************************************************/
XtCallbackProc DLOG_OK_CB(Widget w,XtPointer client_data,XtPointer call_data)
{/*need separate callback so can set flag that OK button was hit*/
  Ldlog_OKHIT = 1; 
  genericdlogcleaner();
 return(NULL);
}
/*___DLOG_OK_CB()___________________________________________________________*/

/****DLOG_CANCEL_CB()********************************************************/
XtCallbackProc DLOG_CANCEL_CB(Widget w,XtPointer client_data 
                                      ,XtPointer call_data)
{/*need separate callback so can set flag that OK button was hit*/
  Ldlog_cancelHIT = 1; 
 genericdlogcleaner();
 return(NULL);
}
/*___DLOG_CANCEL_CB()_______________________________________________________*/

/****DLOG_OPTION_CB()********************************************************/
XtCallbackProc DLOG_OPTION_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{/*need separate callback so can set flag that OK button was hit*/
  Ldlog_optionHIT = 1; 
  genericdlogcleaner();
 return(NULL);
}
/*___DLOG_OPTION_CB()_______________________________________________________*/

/* end-of a whole bunch of motif-specific routines */
#endif /* UNIX_MOTIF 060324 re compile with NOGUI, requires Lquiet=1*/


/****genericDialog()*********************************************************/
void genericDialog()
{
#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
 XmString    xmstring;

 /*manage the components needed for this dialog, and load initial values*/
 /*all DLOG probably have OK and CANCEL buttons */
 if(Ldlog_subject)
 {
     XtManageChild(DLOG_SUBJECT);
     XmTextSetString(DLOG_SUBJECT,dlog_subjectstr);
 }
 if(Ldlog_paramA)
 {
  XtVaSetValues(genericrowA, XmNtopAttachment, XmATTACH_FORM);
  XtVaSetValues(genericrowA, XmNtopOffset, 10); 
  XtVaSetValues(genericrowA, XmNbottomAttachment, XmATTACH_OPPOSITE_FORM); 
  XtVaSetValues(genericrowA, XmNbottomOffset, -20); 

     XtManageChild(genericrowA);
     XtManageChild(DLOG_PARAMA);
     XmTextReplace(DLOG_PARAMA, 0, XmTextGetLastPosition(DLOG_PARAMA)
                   ,dlog_paramAstr);
     XtManageChild( DLOG_TEXTA);
     xmstring = XmStringCreateSimple(dlog_textAstr); 
     XtVaSetValues( DLOG_TEXTA,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
 }
 if(Ldlog_paramB)
 {
     XtVaSetValues(genericrowB,XmNheight,10,NULL);
     XtVaSetValues(DLOG_PARAMB,XmNheight,10,NULL);
     XtVaSetValues( DLOG_TEXTB,XmNheight,10,NULL);
     XtManageChild(genericrowB);
     XtManageChild(DLOG_PARAMB);
     XmTextReplace(DLOG_PARAMB, 0, XmTextGetLastPosition(DLOG_PARAMB)
                   ,dlog_paramBstr);
     XtManageChild( DLOG_TEXTB);
     xmstring = XmStringCreateSimple(dlog_textBstr); 
     XtVaSetValues( DLOG_TEXTB,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
     XtVaSetValues(genericrowB,XmNheight,10,NULL);
     XtVaSetValues(DLOG_PARAMB,XmNheight,10,NULL);
     XtVaSetValues( DLOG_TEXTB,XmNheight,10,NULL);
 }
 if(Ldlog_paramC)
 {
     XtManageChild(genericrowC);
     XtManageChild(DLOG_PARAMC);
     XmTextReplace(DLOG_PARAMC, 0, XmTextGetLastPosition(DLOG_PARAMC)
                   ,dlog_paramCstr);
     XtManageChild(DLOG_TEXTC);
     xmstring = XmStringCreateSimple(dlog_textCstr); 
     XtVaSetValues(DLOG_TEXTC,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
 }
/* if(Ldlog_checkA||Ldlog_checkB||Ldlog_checkC)*/
/* {*/
/*     XtManageChild(genericrowchecks);*/
 if(Ldlog_check_caption)
 {
     XtManageChild(DLOG_CHECK_CAPTION);
     XmTextSetString(DLOG_CHECK_CAPTION,dlog_checkcaptionstr);
 }
   if(Ldlog_checkA)
   {
     XtManageChild(DLOG_CHECKA);
     XmToggleButtonSetState(DLOG_CHECKA,dlog_checkAint, 0);
     xmstring = XmStringCreateSimple(dlog_checkAstr); 
     XtVaSetValues(DLOG_CHECKA,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_checkB)
   {
     XtManageChild(DLOG_CHECKB);
     XmToggleButtonSetState(DLOG_CHECKB,dlog_checkBint, 0);
     xmstring = XmStringCreateSimple(dlog_checkBstr); 
     XtVaSetValues(DLOG_CHECKB,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_checkC)
   {
     XtManageChild(DLOG_CHECKC);
     XmToggleButtonSetState(DLOG_CHECKC,dlog_checkCint, 0);
     xmstring = XmStringCreateSimple(dlog_checkCstr); 
     XtVaSetValues(DLOG_CHECKC,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_checkD)
   {
     XtManageChild(DLOG_CHECKD);
     XmToggleButtonSetState(DLOG_CHECKD,dlog_checkDint, 0);
     xmstring = XmStringCreateSimple(dlog_checkDstr); 
     XtVaSetValues(DLOG_CHECKD,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_checkE)
   {
     XtManageChild(DLOG_CHECKE);
     XmToggleButtonSetState(DLOG_CHECKE,dlog_checkEint, 0);
     xmstring = XmStringCreateSimple(dlog_checkEstr); 
     XtVaSetValues(DLOG_CHECKE,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_checkF)
   {
     XtManageChild(DLOG_CHECKF);
     XmToggleButtonSetState(DLOG_CHECKF,dlog_checkFint, 0);
     xmstring = XmStringCreateSimple(dlog_checkFstr); 
     XtVaSetValues(DLOG_CHECKF,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_checkG)
   {
     XtManageChild(DLOG_CHECKG);
     XmToggleButtonSetState(DLOG_CHECKG,dlog_checkGint, 0);
     xmstring = XmStringCreateSimple(dlog_checkGstr); 
     XtVaSetValues(DLOG_CHECKG,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_checkH)
   {
     XtManageChild(DLOG_CHECKH);
     XmToggleButtonSetState(DLOG_CHECKH,dlog_checkHint, 0);
     xmstring = XmStringCreateSimple(dlog_checkHstr); 
     XtVaSetValues(DLOG_CHECKH,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_checkI)
   {
     XtManageChild(DLOG_CHECKI);
     XmToggleButtonSetState(DLOG_CHECKI,dlog_checkIint, 0);
     xmstring = XmStringCreateSimple(dlog_checkIstr); 
     XtVaSetValues(DLOG_CHECKI,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_checkJ)
   {
     XtManageChild(DLOG_CHECKJ);
     XmToggleButtonSetState(DLOG_CHECKJ,dlog_checkJint, 0);
     xmstring = XmStringCreateSimple(dlog_checkJstr); 
     XtVaSetValues(DLOG_CHECKJ,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_checkK)
   {
     XtManageChild(DLOG_CHECKK);
     XmToggleButtonSetState(DLOG_CHECKK,dlog_checkKint, 0);
     xmstring = XmStringCreateSimple(dlog_checkKstr); 
     XtVaSetValues(DLOG_CHECKK,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_checkL)
   {
     XtManageChild(DLOG_CHECKL);
     XmToggleButtonSetState(DLOG_CHECKL,dlog_checkLint, 0);
     xmstring = XmStringCreateSimple(dlog_checkLstr); 
     XtVaSetValues(DLOG_CHECKL,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_checkM)
   {
     XtManageChild(DLOG_CHECKM);
     XmToggleButtonSetState(DLOG_CHECKM,dlog_checkMint, 0);
     xmstring = XmStringCreateSimple(dlog_checkMstr); 
     XtVaSetValues(DLOG_CHECKM,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_checkN)
   {
     XtManageChild(DLOG_CHECKN);
     XmToggleButtonSetState(DLOG_CHECKN,dlog_checkNint, 0);
     xmstring = XmStringCreateSimple(dlog_checkNstr); 
     XtVaSetValues(DLOG_CHECKN,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_checkO)
   {
     XtManageChild(DLOG_CHECKO);
     XmToggleButtonSetState(DLOG_CHECKO,dlog_checkOint, 0);
     xmstring = XmStringCreateSimple(dlog_checkOstr); 
     XtVaSetValues(DLOG_CHECKO,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_checkP)
   {
     XtManageChild(DLOG_CHECKP);
     XmToggleButtonSetState(DLOG_CHECKP,dlog_checkPint, 0);
     xmstring = XmStringCreateSimple(dlog_checkPstr); 
     XtVaSetValues(DLOG_CHECKP,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_checkQ)
   {
     XtManageChild(DLOG_CHECKQ);
     XmToggleButtonSetState(DLOG_CHECKQ,dlog_checkQint, 0);
     xmstring =    XmStringCreateSimple(dlog_checkQstr); 
     XtVaSetValues(DLOG_CHECKQ,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_checkR)
   {
     XtManageChild(DLOG_CHECKR);
     XmToggleButtonSetState(DLOG_CHECKR,dlog_checkRint, 0);
     xmstring =    XmStringCreateSimple(dlog_checkRstr); 
     XtVaSetValues(DLOG_CHECKR,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_checkS)
   {
     XtManageChild(DLOG_CHECKS);
     XmToggleButtonSetState(DLOG_CHECKS,dlog_checkSint, 0);
     xmstring =    XmStringCreateSimple(dlog_checkSstr); 
     XtVaSetValues(DLOG_CHECKS,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_checkU)
   {
     XtManageChild(DLOG_CHECKU);
     XmToggleButtonSetState(DLOG_CHECKU,dlog_checkUint, 0);
     xmstring =    XmStringCreateSimple(dlog_checkUstr); 
     XtVaSetValues(DLOG_CHECKU,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_checkV)
   {
     XtManageChild(DLOG_CHECKV);
     XmToggleButtonSetState(DLOG_CHECKV,dlog_checkVint, 0);
     xmstring =    XmStringCreateSimple(dlog_checkVstr); 
     XtVaSetValues(DLOG_CHECKV,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_checkW)
   {
     XtManageChild(DLOG_CHECKW);
     XmToggleButtonSetState(DLOG_CHECKW,dlog_checkWint, 0);
     xmstring =    XmStringCreateSimple(dlog_checkWstr); 
     XtVaSetValues(DLOG_CHECKW,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_checkX)
   {
     XtManageChild(DLOG_CHECKX);
     XmToggleButtonSetState(DLOG_CHECKX,dlog_checkXint, 0);
     xmstring =    XmStringCreateSimple(dlog_checkXstr); 
     XtVaSetValues(DLOG_CHECKX,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }



 if(Ldlog_radio_caption)
 {
     XtManageChild(DLOG_RADIO_CAPTION);
     XmTextSetString(DLOG_RADIO_CAPTION,dlog_radiocaptionstr);
 }
   if(Ldlog_radioA)
   {
     XtManageChild(DLOG_RADIOA);
     XmToggleButtonSetState(DLOG_RADIOA,dlog_radioAint, 0);
     xmstring = XmStringCreateSimple(dlog_radioAstr); 
     XtVaSetValues(DLOG_RADIOA,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_radioB)
   {
     XtManageChild(DLOG_RADIOB);
     XmToggleButtonSetState(DLOG_RADIOB,dlog_radioBint, 0);
     xmstring = XmStringCreateSimple(dlog_radioBstr); 
     XtVaSetValues(DLOG_RADIOB,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_radioC)
   {
     XtManageChild(DLOG_RADIOC);
     XmToggleButtonSetState(DLOG_RADIOC,dlog_radioCint, 0);
     xmstring = XmStringCreateSimple(dlog_radioCstr); 
     XtVaSetValues(DLOG_RADIOC,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_radioD)
   {
     XtManageChild(DLOG_RADIOD);
     XmToggleButtonSetState(DLOG_RADIOD,dlog_radioDint, 0);
     xmstring = XmStringCreateSimple(dlog_radioDstr); 
     XtVaSetValues(DLOG_RADIOD,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_radioE)
   {
     XtManageChild(DLOG_RADIOE);
     XmToggleButtonSetState(DLOG_RADIOE,dlog_radioEint, 0);
     xmstring = XmStringCreateSimple(dlog_radioEstr); 
     XtVaSetValues(DLOG_RADIOE,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_radioF)
   {
     XtManageChild(DLOG_RADIOF);
     XmToggleButtonSetState(DLOG_RADIOF,dlog_radioFint, 0);
     xmstring = XmStringCreateSimple(dlog_radioFstr); 
     XtVaSetValues(DLOG_RADIOF,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_radioG)
   {
     XtManageChild(DLOG_RADIOG);
     XmToggleButtonSetState(DLOG_RADIOG,dlog_radioGint, 0);
     xmstring = XmStringCreateSimple(dlog_radioGstr); 
     XtVaSetValues(DLOG_RADIOG,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_radioH)
   {
     XtManageChild(DLOG_RADIOH);
     XmToggleButtonSetState(DLOG_RADIOH,dlog_radioHint, 0);
     xmstring = XmStringCreateSimple(dlog_radioHstr); 
     XtVaSetValues(DLOG_RADIOH,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_radioI)
   {
     XtManageChild(DLOG_RADIOI);
     XmToggleButtonSetState(DLOG_RADIOI,dlog_radioIint, 0);
     xmstring = XmStringCreateSimple(dlog_radioIstr); 
     XtVaSetValues(DLOG_RADIOI,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }

 if(Ldlog_checkLast_caption)
 {
     XtManageChild(DLOG_CHECKLAST_CAPTION);
     XmTextSetString(DLOG_CHECKLAST_CAPTION,dlog_checkLastcaptionstr);
 }
   if(Ldlog_checkT)
   {
     XtManageChild(DLOG_CHECKT);
     XmToggleButtonSetState(DLOG_CHECKT,dlog_checkTint, 0);
     xmstring =    XmStringCreateSimple(dlog_checkTstr); 
     XtVaSetValues(DLOG_CHECKT,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }

   if(Ldlog_radioJ)
   {
     XtManageChild(DLOG_RADIOJ);
     XmToggleButtonSetState(DLOG_RADIOJ,dlog_radioJint, 0);
     xmstring = XmStringCreateSimple(dlog_radioJstr); 
     XtVaSetValues(DLOG_RADIOJ,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_radioK)
   {
     XtManageChild(DLOG_RADIOK);
     XmToggleButtonSetState(DLOG_RADIOK,dlog_radioKint, 0);
     xmstring = XmStringCreateSimple(dlog_radioKstr); 
     XtVaSetValues(DLOG_RADIOK,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_radioL)
   {
     XtManageChild(DLOG_RADIOL);
     XmToggleButtonSetState(DLOG_RADIOL,dlog_radioLint, 0);
     xmstring = XmStringCreateSimple(dlog_radioLstr); 
     XtVaSetValues(DLOG_RADIOL,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_radioM)
   {
     XtManageChild(DLOG_RADIOM);
     XmToggleButtonSetState(DLOG_RADIOM,dlog_radioMint, 0);
     xmstring = XmStringCreateSimple(dlog_radioMstr); 
     XtVaSetValues(DLOG_RADIOM,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_radioN)
   {
     XtManageChild(DLOG_RADION);
     XmToggleButtonSetState(DLOG_RADION,dlog_radioNint, 0);
     xmstring = XmStringCreateSimple(dlog_radioNstr); 
     XtVaSetValues(DLOG_RADION,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_radioO)
   {
     XtManageChild(DLOG_RADIOO);
     XmToggleButtonSetState(DLOG_RADIOO,dlog_radioOint, 0);
     xmstring = XmStringCreateSimple(dlog_radioOstr); 
     XtVaSetValues(DLOG_RADIOO,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   if(Ldlog_radioP)
   {
     XtManageChild(DLOG_RADIOP);
     XmToggleButtonSetState(DLOG_RADIOP,dlog_radioPint, 0);
     xmstring = XmStringCreateSimple(dlog_radioPstr); 
     XtVaSetValues(DLOG_RADIOP,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
 /*}*/

 /*all dialogs will probably have at least an OK button*/
 XtManageChild(genericbuttons);
 if(Ldlog_OK)
 {
     XtManageChild(DLOG_OK);
     xmstring = XmStringCreateSimple(dlog_OKstr); 
     XtVaSetValues(DLOG_OK,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
 }
 if(Ldlog_option)
 {
     XtManageChild(DLOG_OPTION);
     xmstring = XmStringCreateSimple(dlog_optionstr); 
     XtVaSetValues(DLOG_OPTION,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
 }
 if(Ldlog_cancel)
 {
     XtManageChild(DLOG_CANCEL);
     xmstring = XmStringCreateSimple(dlog_cancelstr); 
     XtVaSetValues(DLOG_CANCEL,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
 }
 if(Ldlog_info)
 {
     XtManageChild(DLOG_INFO);
     XmTextSetString(DLOG_INFO,dlog_infostr);
 }
#endif /* UNIX_MOTIF 060324 re compile with NOGUI, requires Lquiet=1*/
}
/*___genericDialog()_________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****DogenericDialog()*******************************************************/
void  DogenericDialog()
{                   /*create a generic dialogbox for multiple use */
#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
  int    n=0, narg=0, boxtop=0, holdboxtop=0, thisboxtop=0;
  int    boxhigh=0, NWIDE=600, LEFT=10;
  XmString    xmstring;
  XFontStruct *genericFont=NULL;
  XmFontList genericfontList;


/*DogenericDialog() creates all components and manages the outer box*/
/*  genericDialog() manages the components used for the current dialog*/
boxhigh = 20;
  /*if possible, buttonFont has been defined in PUXMINIT.c */

  /*Define genericFont, if possible*/
  /*if (genericFont =XLoadQueryFont(dpy, "-*-Helvetica-Bold-R-Normal--11*" ) )*/
  if (genericFont = XLoadQueryFont(dpy, "-*-Helvetica-medium-R-Normal--11*" ) )
  {
      boxhigh = 18; /*try <2* of font height*/
   /*printf("generic font Helvetica-medium-R-Normal--11, boxhigh== %d\n"*/
   /*       ,boxhigh);*/
      genericfontList = XmFontListCreate(genericFont, XmSTRING_DEFAULT_CHARSET);
  }
  else
  {
      boxhigh = 20;
      genericFont = buttonFont;
      genericfontList = fontList;
      /*printf("generic font using buttonFont, boxhigh== %d\n",boxhigh);*/
  }
  /*overall genericform*/
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNborderColor, windowborder); n++;
  XtSetArg(args[n], XmNwidth, 600); n++;
  XtSetArg(args[n], XmNheight, 600); n++;
  XtSetArg(args[n], XmNx, 50); n++;
  XtSetArg(args[n], XmNy,  0); n++;
  XtSetArg(args[n], XmNtitle, "PREKIN_DLOG"); n++;
  XtSetArg(args[n], XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL); n++;
  XtSetArg(args[n], XmNdeleteResponse, XmDO_NOTHING); n++; 
    /*ignore close commands from frame menu or X button*/
  genericbox = XmCreateFormDialog(grafwindow,"FORM",args,n);
  XtManageChild(genericbox); /*use this to turn DLOGs on and off */

 boxtop = 0; /*keeps track of the number of boxes stacked up*/
 if(Ldlog_subject)
 {
 /*Subject*/
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, boxtop); n++;
  XtSetArg(args[n], XmNwidth, 600); n++;
  XtSetArg(args[n], XmNheight, 5*boxhigh); n++;
  if(genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNeditable, False); n++;
  DLOG_SUBJECT = XtCreateWidget(" ", xmTextWidgetClass,genericbox,args,n);
  XtManageChild(DLOG_SUBJECT); /*some DLOG give no subject*/
  XmTextSetString(DLOG_SUBJECT,dlog_subjectstr);
  boxtop = boxtop + 5*boxhigh; 
 }

 if(Ldlog_paramA)
 {
#ifdef ROW
  /* A row of parameter & text for parameter */
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, boxtop); n++;
  XtSetArg(args[n], XmNheight, boxhigh); n++;
  XtSetArg(args[n], XmNwidth, 600); n++;
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  genericrowA
      = XtCreateWidget(" ", xmRowColumnWidgetClass,genericbox,args,n);
  XtManageChild(genericrowA); /*some DLOG get no parameters*/
#endif
  /*parameter value */
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNwidth, 60); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, boxtop); n++;
  XtSetArg(args[n], XmNheight, 2*boxhigh); n++;
  /*XtSetArg(args[n], XmNbottomAttachment, XmATTACH_OPPOSITE_FORM); n++;*/
  /*XtSetArg(args[n], XmNbottomOffset, -boxhigh); n++;*/
  if(genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  DLOG_PARAMA = XtCreateWidget(" ", xmTextWidgetClass,genericbox,args,n);
  XtAddCallback(DLOG_PARAMA,XmNvalueChangedCallback
                                        ,(XtCallbackProc)DLOG_PARAMA_CB,NULL); 
  XtManageChild(DLOG_PARAMA); 
  XmTextReplace(DLOG_PARAMA, 0, XmTextGetLastPosition(DLOG_PARAMA)
                   ,dlog_paramAstr);
  
  /*parameter text*/
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, DLOG_PARAMA); n++;
  XtSetArg(args[n], XmNleftOffset, 5); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, boxtop+boxhigh); n++;
  XtSetArg(args[n], XmNheight, 2*boxhigh); n++;
  /*XtSetArg(args[n], XmNbottomAttachment, XmATTACH_OPPOSITE_FORM); n++;*/
  /*XtSetArg(args[n], XmNbottomOffset, -boxhigh); n++;*/
  if (genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  DLOG_TEXTA = XtCreateWidget(" ", xmLabelGadgetClass,genericbox,args,n);
  XtManageChild(DLOG_TEXTA); 
  xmstring = XmStringCreateSimple(dlog_textAstr);
  XtVaSetValues( DLOG_TEXTA,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  boxtop = boxtop + 2*boxhigh;
 }
 if(Ldlog_paramB)
 {
#ifdef ROW
  /* B row of parameter & text for parameter */
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, boxtop); n++;
  XtSetArg(args[n], XmNheight, boxhigh); n++;
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  genericrowB
      = XtCreateWidget(" ", xmRowColumnWidgetClass,genericbox,args,n);
  XtManageChild(genericrowB); /*some DLOG get no parameters*/
#endif
  /*parameter value */
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNwidth, 60); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, boxtop); n++;
  XtSetArg(args[n], XmNheight, 2*boxhigh); n++;
  /*XtSetArg(args[n], XmNbottomAttachment, XmATTACH_OPPOSITE_FORM); n++;*/
  /*XtSetArg(args[n], XmNbottomOffset, -boxhigh); n++;*/
  if(genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  DLOG_PARAMB = XtCreateWidget(" ", xmTextWidgetClass,genericbox,args,n);
  XtAddCallback(DLOG_PARAMB,XmNvalueChangedCallback
                                        ,(XtCallbackProc)DLOG_PARAMB_CB,NULL); 
  XtManageChild(DLOG_PARAMB); 
  XmTextReplace(DLOG_PARAMB, 0, XmTextGetLastPosition(DLOG_PARAMB)
                   ,dlog_paramBstr);
  
  /*parameter text*/
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, DLOG_PARAMB); n++;
  XtSetArg(args[n], XmNleftOffset, 5); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, boxtop+boxhigh); n++;
  XtSetArg(args[n], XmNheight, 2*boxhigh); n++;
  /*XtSetArg(args[n], XmNbottomAttachment, XmATTACH_OPPOSITE_FORM); n++;*/
  /*XtSetArg(args[n], XmNbottomOffset, -boxhigh); n++;*/
  if (genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  DLOG_TEXTB = XtCreateWidget(" ", xmLabelGadgetClass,genericbox,args,n);
  XtManageChild(DLOG_TEXTB); 
  xmstring = XmStringCreateSimple(dlog_textBstr);
  XtVaSetValues( DLOG_TEXTB,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  boxtop = boxtop + 2*boxhigh;
 }
 if(Ldlog_paramC)
 {
#ifdef ROW
  /* C row of parameter & text for parameter */
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, boxtop); n++;
  XtSetArg(args[n], XmNheight, boxhigh); n++;
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  genericrowC
      = XtCreateWidget(" ", xmRowColumnWidgetClass,genericbox,args,n);
  XtManageChild(genericrowC); /*some DLOG get no parameters*/
#endif
  /*parameter value */
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNwidth, 60); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, boxtop); n++;
  XtSetArg(args[n], XmNheight, 2*boxhigh); n++;
  /*XtSetArg(args[n], XmNbottomAttachment, XmATTACH_OPPOSITE_FORM); n++;*/
  /*XtSetArg(args[n], XmNbottomOffset, -boxhigh); n++;*/
  if(genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  DLOG_PARAMC = XtCreateWidget(" ", xmTextWidgetClass,genericbox,args,n);
  XtAddCallback(DLOG_PARAMC,XmNvalueChangedCallback
                                        ,(XtCallbackProc)DLOG_PARAMC_CB,NULL); 
  XtManageChild(DLOG_PARAMC); 
  XmTextReplace(DLOG_PARAMC, 0, XmTextGetLastPosition(DLOG_PARAMC)
                   ,dlog_paramCstr);
  
  /*parameter text*/
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, DLOG_PARAMC); n++;
  XtSetArg(args[n], XmNleftOffset, 5); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, boxtop+boxhigh); n++;
  XtSetArg(args[n], XmNheight, 2*boxhigh); n++;
  /*XtSetArg(args[n], XmNbottomAttachment, XmATTACH_OPPOSITE_FORM); n++;*/
  /*XtSetArg(args[n], XmNbottomOffset, -boxhigh); n++;*/
  if (genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  DLOG_TEXTC = XtCreateWidget(" ", xmLabelGadgetClass,genericbox,args,n);
  XtManageChild(DLOG_TEXTC); /*some DLOG get no parameters*/
  xmstring = XmStringCreateSimple(dlog_textCstr);
  XtVaSetValues( DLOG_TEXTC,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  boxtop = boxtop + 2*boxhigh;
 }
 if(Ldlog_paramD)
 {
  /*parameter value */
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNwidth, 60); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, boxtop); n++;
  XtSetArg(args[n], XmNheight, 2*boxhigh); n++;
  if(genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  DLOG_PARAMD = XtCreateWidget(" ", xmTextWidgetClass,genericbox,args,n);
  XtAddCallback(          DLOG_PARAMD,XmNvalueChangedCallback
         ,(XtCallbackProc)DLOG_PARAMD_CB,NULL); 
  XtManageChild(          DLOG_PARAMD); 
  XmTextReplace(          DLOG_PARAMD, 0
   ,XmTextGetLastPosition(DLOG_PARAMD)
                         ,dlog_paramDstr);
  
  /*parameter text*/
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, DLOG_PARAMD); n++;
  XtSetArg(args[n], XmNleftOffset, 5); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, boxtop+boxhigh); n++;
  XtSetArg(args[n], XmNheight, 2*boxhigh); n++;
  if (genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  DLOG_TEXTD = XtCreateWidget(" ", xmLabelGadgetClass,genericbox,args,n);
  XtManageChild(                  DLOG_TEXTD); 
  xmstring = XmStringCreateSimple(dlog_textDstr);
  XtVaSetValues(                  DLOG_TEXTD,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  boxtop = boxtop + 2*boxhigh;
 }
 if(Ldlog_paramE)
 {
  /*parameter value */
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNwidth, 60); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, boxtop); n++;
  XtSetArg(args[n], XmNheight, 2*boxhigh); n++;
  if(genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  DLOG_PARAME = XtCreateWidget(" ", xmTextWidgetClass,genericbox,args,n);
  XtAddCallback(          DLOG_PARAME,XmNvalueChangedCallback
         ,(XtCallbackProc)DLOG_PARAME_CB,NULL); 
  XtManageChild(          DLOG_PARAME); 
  XmTextReplace(          DLOG_PARAME, 0
   ,XmTextGetLastPosition(DLOG_PARAME)
                         ,dlog_paramEstr);
  
  /*parameter text*/
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, DLOG_PARAME); n++;
  XtSetArg(args[n], XmNleftOffset, 5); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, boxtop+boxhigh); n++;
  XtSetArg(args[n], XmNheight, 2*boxhigh); n++;
  if (genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  DLOG_TEXTE = XtCreateWidget(" ", xmLabelGadgetClass,genericbox,args,n);
  XtManageChild(                  DLOG_TEXTE); 
  xmstring = XmStringCreateSimple(dlog_textEstr);
  XtVaSetValues(                  DLOG_TEXTE,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  boxtop = boxtop + 2*boxhigh;
 }
 if(Ldlog_paramF)
 {
  /*parameter value */
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNwidth, 60); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, boxtop); n++;
  XtSetArg(args[n], XmNheight, 2*boxhigh); n++;
  if(genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  DLOG_PARAMF = XtCreateWidget(" ", xmTextWidgetClass,genericbox,args,n);
  XtAddCallback(          DLOG_PARAMF,XmNvalueChangedCallback
         ,(XtCallbackProc)DLOG_PARAMF_CB,NULL); 
  XtManageChild(          DLOG_PARAMF); 
  XmTextReplace(          DLOG_PARAMF, 0
   ,XmTextGetLastPosition(DLOG_PARAMF)
                         ,dlog_paramFstr);
  
  /*parameter text*/
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, DLOG_PARAMF); n++;
  XtSetArg(args[n], XmNleftOffset, 5); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, boxtop+boxhigh); n++;
  XtSetArg(args[n], XmNheight, 2*boxhigh); n++;
  if (genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  DLOG_TEXTF = XtCreateWidget(" ", xmLabelGadgetClass,genericbox,args,n);
  XtManageChild(                  DLOG_TEXTF); 
  xmstring = XmStringCreateSimple(dlog_textFstr);
  XtVaSetValues(                  DLOG_TEXTF,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  boxtop = boxtop + 2*boxhigh;
 }
 if(Ldlog_paramG)
 {
  /*parameter value */
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNwidth, 60); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, boxtop); n++;
  XtSetArg(args[n], XmNheight, 2*boxhigh); n++;
  if(genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  DLOG_PARAMG = XtCreateWidget(" ", xmTextWidgetClass,genericbox,args,n);
  XtAddCallback(          DLOG_PARAMG,XmNvalueChangedCallback
         ,(XtCallbackProc)DLOG_PARAMG_CB,NULL); 
  XtManageChild(          DLOG_PARAMG); 
  XmTextReplace(          DLOG_PARAMG, 0
   ,XmTextGetLastPosition(DLOG_PARAMG)
                         ,dlog_paramGstr);
  
  /*parameter text*/
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, DLOG_PARAMG); n++;
  XtSetArg(args[n], XmNleftOffset, 5); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, boxtop+boxhigh); n++;
  XtSetArg(args[n], XmNheight, 2*boxhigh); n++;
  if (genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  DLOG_TEXTG = XtCreateWidget(" ", xmLabelGadgetClass,genericbox,args,n);
  XtManageChild(                  DLOG_TEXTG); 
  xmstring = XmStringCreateSimple(dlog_textGstr);
  XtVaSetValues(                  DLOG_TEXTG,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  boxtop = boxtop + 2*boxhigh;
 }
 if(Ldlog_paramH)
 {
  /*parameter value */
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNwidth, 60); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, boxtop); n++;
  XtSetArg(args[n], XmNheight, 2*boxhigh); n++;
  if(genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  DLOG_PARAMH = XtCreateWidget(" ", xmTextWidgetClass,genericbox,args,n);
  XtAddCallback(          DLOG_PARAMH,XmNvalueChangedCallback
         ,(XtCallbackProc)DLOG_PARAMH_CB,NULL); 
  XtManageChild(          DLOG_PARAMH); 
  XmTextReplace(          DLOG_PARAMH, 0
   ,XmTextGetLastPosition(DLOG_PARAMH)
                         ,dlog_paramHstr);
  
  /*parameter text*/
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, DLOG_PARAMH); n++;
  XtSetArg(args[n], XmNleftOffset, 5); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, boxtop+boxhigh); n++;
  XtSetArg(args[n], XmNheight, 2*boxhigh); n++;
  if (genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  DLOG_TEXTH = XtCreateWidget(" ", xmLabelGadgetClass,genericbox,args,n);
  XtManageChild(                  DLOG_TEXTH); 
  xmstring = XmStringCreateSimple(dlog_textHstr);
  XtVaSetValues(                  DLOG_TEXTH,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  boxtop = boxtop + 2*boxhigh;
 }

 if(Ldlog_paramI)
 {
  /*parameter value */
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNwidth, 60); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, boxtop); n++;
  XtSetArg(args[n], XmNheight, 2*boxhigh); n++;
  if(genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  DLOG_PARAMI = XtCreateWidget(" ", xmTextWidgetClass,genericbox,args,n);
  XtAddCallback(          DLOG_PARAMI,XmNvalueChangedCallback
         ,(XtCallbackProc)DLOG_PARAMI_CB,NULL); 
  XtManageChild(          DLOG_PARAMI); 
  XmTextReplace(          DLOG_PARAMI, 0
   ,XmTextGetLastPosition(DLOG_PARAMI)
                         ,dlog_paramIstr);
  
  /*parameter text*/
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, DLOG_PARAMI); n++;
  XtSetArg(args[n], XmNleftOffset, 5); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, boxtop+boxhigh); n++;
  XtSetArg(args[n], XmNheight, 2*boxhigh); n++;
  if (genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  DLOG_TEXTI = XtCreateWidget(" ", xmLabelGadgetClass,genericbox,args,n);
  XtManageChild(                  DLOG_TEXTI); 
  xmstring = XmStringCreateSimple(dlog_textIstr);
  XtVaSetValues(                  DLOG_TEXTI,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  boxtop = boxtop + 2*boxhigh;
 }

 if(Lrangecontrols)
 {/* A row holding column of check boxes and a column of radio buttons */
  NWIDE = 300;
 #ifdef EXTRAFORMCONTAINERS
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, boxtop); n++;
  /*XtSetArg(args[n], XmNheight, boxhigh); n++;*/
  /*XtSetArg(args[n], XmNheight, GHEIGHT-100); n++;*/
  XtSetArg(args[n], XmNheight, 300); n++;
  XtSetArg(args[n], XmNwidth, 600); n++;
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  genericcheckradiorow
      = XtCreateWidget(" ", xmRowColumnWidgetClass,genericbox,args,n);
  XtManageChild(genericcheckradiorow); /*some DLOG get no parameters*/
  /* form to hold column of the check boxes*/
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  /*XtSetArg(args[n], XmNtopOffset, boxtop); n++;*/
  /*XtSetArg(args[n], XmNheight, boxhigh); n++;*/
  /*XtSetArg(args[n], XmNheight, 300); n++;*/
  XtSetArg(args[n], XmNwidth, 300); n++;
  XtSetArg(args[n], XmNorientation, XmVERTICAL); n++;
  genericcheckcol
      = XtCreateWidget(" ", xmRowColumnWidgetClass,genericcheckradiorow,args,n);
  XtManageChild(genericcheckcol); /*some DLOG get no parameters*/
  /* form to hold column of the radio boxes*/
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, DLOG_CANCEL); n++;
  XtSetArg(args[n], XmNleftOffset, 10); n++;
  /*XtSetArg(args[n], XmNtopOffset, boxtop); n++;*/
  /*XtSetArg(args[n], XmNheight, boxhigh); n++;*/
  /*XtSetArg(args[n], XmNheight, 300); n++;*/
  XtSetArg(args[n], XmNwidth, 300); n++;
  XtSetArg(args[n], XmNorientation, XmVERTICAL); n++;
  genericradiocol
      = XtCreateWidget(" ", xmRowColumnWidgetClass,genericcheckradiorow,args,n);
  XtManageChild(genericradiocol); /*some DLOG get no parameters*/

  generictmp = genericcheckcol;
  holdboxtop = boxtop; /*save present vertical position*/
  thisboxtop = 0; /*inside of check or radio column box*/
  /* first expect some check boxes so redefine parent for checkboxes*/
#else
    generictmp = genericbox;
    thisboxtop = boxtop; /*still just inside of genericbox*/
    holdboxtop = boxtop; /*save present vertical position*/

#endif
  
 }
 else
 {
    generictmp = genericbox;
    thisboxtop = boxtop; /*still just inside of genericbox*/
 }
  
 if(Ldlog_check_caption)
 {
 /*CAPTION for CHECK BOXES*/
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftOffset, 10); n++;
  XtSetArg(args[n], XmNheight, boxhigh+15); n++;
  XtSetArg(args[n], XmNwidth, NWIDE); n++;
  if(genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  XtSetArg(args[n], XmNeditable, False); n++;
  DLOG_CHECK_CAPTION = XtCreateWidget(" ", xmTextWidgetClass,generictmp,args,n);
  XtManageChild(DLOG_CHECK_CAPTION);
  XmTextSetString(DLOG_CHECK_CAPTION,dlog_checkcaptionstr);
  thisboxtop = thisboxtop + boxhigh+15;
 }

 if(   Ldlog_checkA || Ldlog_checkB || Ldlog_checkC || Ldlog_checkD 
    || Ldlog_checkE || Ldlog_checkF || Ldlog_checkG || Ldlog_checkH 
    || Ldlog_checkI || Ldlog_checkJ || Ldlog_checkK || Ldlog_checkL 
    || Ldlog_checkM || Ldlog_checkN || Ldlog_checkO || Ldlog_checkP 
    || Ldlog_checkQ || Ldlog_checkS || Ldlog_checkT || Ldlog_checkU 
    || Ldlog_checkV || Ldlog_checkW || Ldlog_checkX  )
 {/*common XtSetArg for any and all check boxes*/
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftOffset, 10); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_OPPOSITE_FORM); n++;
  XtSetArg(args[n], XmNheight, boxhigh); n++;
  if (genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  narg = n;
 }
 if(Ldlog_checkA)
 {
  /*check box  A */
  n = narg;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  DLOG_CHECKA = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKA,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKA_CB, NULL); 
  XtManageChild(DLOG_CHECKA);
  XmToggleButtonSetState(DLOG_CHECKA,dlog_checkAint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkAstr); 
  XtVaSetValues(DLOG_CHECKA,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_checkB)
 {
  /*check box  B */
  n = narg;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  DLOG_CHECKB = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKB,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKB_CB, NULL); 
  XtManageChild(DLOG_CHECKB);
  XmToggleButtonSetState(DLOG_CHECKB,dlog_checkBint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkBstr); 
  XtVaSetValues(DLOG_CHECKB,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_checkC)
 {
  /*check box  C */
  n = narg;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  DLOG_CHECKC = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKC,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKC_CB, NULL); 
  XtManageChild(DLOG_CHECKC);
  XmToggleButtonSetState(DLOG_CHECKC,dlog_checkCint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkCstr); 
  XtVaSetValues(DLOG_CHECKC,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_checkD)
 {
  /*check box  D */
  n = narg;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  DLOG_CHECKD = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKD,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKD_CB, NULL); 
  XtManageChild(DLOG_CHECKD);
  XmToggleButtonSetState(DLOG_CHECKD,dlog_checkDint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkDstr); 
  XtVaSetValues(DLOG_CHECKD,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_checkE)
 {
  /*check box  E */
  n = narg;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  DLOG_CHECKE = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKE,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKE_CB, NULL); 
  XtManageChild(DLOG_CHECKE);
  XmToggleButtonSetState(DLOG_CHECKE,dlog_checkEint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkEstr); 
  XtVaSetValues(DLOG_CHECKE,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_checkF)
 {
  /*check box  F */
  n = narg;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  DLOG_CHECKF = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKF,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKF_CB, NULL); 
  XtManageChild(DLOG_CHECKF);
  XmToggleButtonSetState(DLOG_CHECKF,dlog_checkFint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkFstr); 
  XtVaSetValues(DLOG_CHECKF,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_checkG)
 {
  /*check box  G */
  n = narg;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  DLOG_CHECKG = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKG,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKG_CB, NULL); 
  XtManageChild(DLOG_CHECKG);
  XmToggleButtonSetState(DLOG_CHECKG,dlog_checkGint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkGstr); 
  XtVaSetValues(DLOG_CHECKG,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_checkH)
 {
  /*check box  H */
  n = narg;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  DLOG_CHECKH = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKH,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKH_CB, NULL); 
  XtManageChild(DLOG_CHECKH);
  XmToggleButtonSetState(DLOG_CHECKH,dlog_checkHint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkHstr); 
  XtVaSetValues(DLOG_CHECKH,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_checkI)
 {
  /*check box  I */
  n = narg;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  DLOG_CHECKI = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKI,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKI_CB, NULL); 
  XtManageChild(DLOG_CHECKI);
  XmToggleButtonSetState(DLOG_CHECKI,dlog_checkIint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkIstr); 
  XtVaSetValues(DLOG_CHECKI,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_checkJ)
 {
  /*check box  J */
  n = narg;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  DLOG_CHECKJ = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKJ,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKJ_CB, NULL); 
  XtManageChild(DLOG_CHECKJ);
  XmToggleButtonSetState(DLOG_CHECKJ,dlog_checkJint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkJstr); 
  XtVaSetValues(DLOG_CHECKJ,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_checkK)
 {
  /*check box  K */
  n = narg;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  DLOG_CHECKK = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKK,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKK_CB, NULL); 
  XtManageChild(DLOG_CHECKK);
  XmToggleButtonSetState(DLOG_CHECKK,dlog_checkKint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkKstr); 
  XtVaSetValues(DLOG_CHECKK,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_checkL)
 {
  /*check box  L */
  n = narg;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  DLOG_CHECKL = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKL,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKL_CB, NULL); 
  XtManageChild(DLOG_CHECKL);
  XmToggleButtonSetState(DLOG_CHECKL,dlog_checkLint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkLstr); 
  XtVaSetValues(DLOG_CHECKL,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_checkM)
 {
  /*check box  M */
  n = narg;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  DLOG_CHECKM = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKM,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKM_CB, NULL); 
  XtManageChild(DLOG_CHECKM);
  XmToggleButtonSetState(DLOG_CHECKM,dlog_checkMint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkMstr); 
  XtVaSetValues(DLOG_CHECKM,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_checkN)
 {
  /*check box  N */
  n = narg;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  DLOG_CHECKN = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKN,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKN_CB, NULL); 
  XtManageChild(DLOG_CHECKN);
  XmToggleButtonSetState(DLOG_CHECKN,dlog_checkNint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkNstr); 
  XtVaSetValues(DLOG_CHECKN,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_checkO)
 {
  /*check box  O */
  n = narg;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  DLOG_CHECKO = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKO,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKO_CB, NULL); 
  XtManageChild(DLOG_CHECKO);
  XmToggleButtonSetState(DLOG_CHECKO,dlog_checkOint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkOstr); 
  XtVaSetValues(DLOG_CHECKO,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_checkP)
 {
  /*check box  P */
  n = narg;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  DLOG_CHECKP = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKP,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKP_CB, NULL); 
  XtManageChild(DLOG_CHECKP);
  XmToggleButtonSetState(DLOG_CHECKP,dlog_checkPint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkPstr); 
  XtVaSetValues(DLOG_CHECKP,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_checkQ)
 {
  /*check box  Q */
  n = narg;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  DLOG_CHECKQ = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKQ,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKQ_CB, NULL); 
  XtManageChild(DLOG_CHECKQ);
  XmToggleButtonSetState(DLOG_CHECKQ,dlog_checkQint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkQstr); 
  XtVaSetValues(DLOG_CHECKQ,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_checkR)
 {
  /*check box  R */
  n = narg;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  DLOG_CHECKR = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKR,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKR_CB, NULL); 
  XtManageChild(DLOG_CHECKR);
  XmToggleButtonSetState(DLOG_CHECKR,dlog_checkRint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkRstr); 
  XtVaSetValues(DLOG_CHECKR,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_checkS)
 {
  /*check box  S */
  n = narg;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  DLOG_CHECKS = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKS,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKS_CB, NULL); 
  XtManageChild(DLOG_CHECKS);
  XmToggleButtonSetState(DLOG_CHECKS,dlog_checkSint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkSstr); 
  XtVaSetValues(DLOG_CHECKS,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_checkT) 
 {
  /*check box  T */
  n = narg;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  DLOG_CHECKT = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKT,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKT_CB, NULL); 
  XtManageChild(DLOG_CHECKT);
  XmToggleButtonSetState(DLOG_CHECKT,dlog_checkTint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkTstr); 
  XtVaSetValues(DLOG_CHECKT,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_checkU) 
 {
  /*check box  U */
  n = narg;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  DLOG_CHECKU = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKU,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKU_CB, NULL); 
  XtManageChild(DLOG_CHECKU);
  XmToggleButtonSetState(DLOG_CHECKU,dlog_checkUint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkUstr); 
  XtVaSetValues(DLOG_CHECKU,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }

 if(Lrangecontrols)
 {
  thisboxtop = holdboxtop; /*recover old vertical position*/
  LEFT = NWIDE;
  /*thisboxtop = 0;*/ /*recover old vertical position*/
  /* now expect some radio boxes so redefine parent for checkboxes*/
  /*generictmp = genericradiocol;*/
 }
 else
 {
    LEFT = 10;
 }

 if(Ldlog_checkV) 
 {
  /*check box  V */
  n = narg;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  XtSetArg(args[n], XmNleftOffset, LEFT); n++;
  DLOG_CHECKV = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKV,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKV_CB, NULL); 
  XtManageChild(DLOG_CHECKV);
  XmToggleButtonSetState(DLOG_CHECKV,dlog_checkVint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkVstr); 
  XtVaSetValues(DLOG_CHECKV,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + 2*boxhigh;
 }
 if(Ldlog_checkW) 
 {
  /*check box  W */
  n = narg;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  XtSetArg(args[n], XmNleftOffset, LEFT); n++;
  DLOG_CHECKW = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKW,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKW_CB, NULL); 
  XtManageChild(DLOG_CHECKW);
  XmToggleButtonSetState(DLOG_CHECKW,dlog_checkWint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkWstr); 
  XtVaSetValues(DLOG_CHECKW,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + 2*boxhigh;
 }
 if(Ldlog_checkX) 
 {
  /*check box  X */
  n = narg;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  XtSetArg(args[n], XmNleftOffset, LEFT); n++;
  DLOG_CHECKX = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKX,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKX_CB, NULL); 
  XtManageChild(DLOG_CHECKX);
  XmToggleButtonSetState(DLOG_CHECKX,dlog_checkXint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkXstr); 
  XtVaSetValues(DLOG_CHECKX,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + 2*boxhigh;
 }



 if(Ldlog_radio_caption)
 {
  /*CAPTION for RADIO BOXES*/
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftOffset, LEFT); n++;
  XtSetArg(args[n], XmNheight, boxhigh+15); n++;
  XtSetArg(args[n], XmNwidth, NWIDE); n++;
  if(genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  XtSetArg(args[n], XmNeditable, False); n++;
  DLOG_RADIO_CAPTION = XtCreateWidget(" ", xmTextWidgetClass,generictmp,args,n);
  XtManageChild(DLOG_RADIO_CAPTION);
  XmTextSetString(DLOG_RADIO_CAPTION,dlog_radiocaptionstr);
  thisboxtop = thisboxtop + boxhigh+15;
 }
 /*  stack of radio buttons */
 /* do not need a row widget here, since radio buttons are stacked in window*/
 if(   Ldlog_radioA || Ldlog_radioB || Ldlog_radioC || Ldlog_radioD 
    || Ldlog_radioE || Ldlog_radioF || Ldlog_radioG || Ldlog_radioH 
    || Ldlog_radioI || Ldlog_radioJ || Ldlog_radioK || Ldlog_radioL )
 {/*common XtSetArg for any and all radio boxes*/
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftOffset, LEFT); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_OPPOSITE_FORM); n++;
  if (genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  narg = n;
 }
 if(Ldlog_radioA)
 {
  /*radio box  A */
     n = narg;
     XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
     XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
     DLOG_RADIOA = XmCreateToggleButtonGadget(generictmp," ",args,n);
     XtAddCallback(                 DLOG_RADIOA,XmNvalueChangedCallback
                   ,(XtCallbackProc)DLOG_RADIOA_CB, NULL); 
     XtManageChild(                 DLOG_RADIOA);
     XmToggleButtonSetState(        DLOG_RADIOA
                                   ,dlog_radioAint, 0);
    xmstring = XmStringCreateSimple(dlog_radioAstr); 
    XtVaSetValues(                  DLOG_RADIOA,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_radioB)
 {
  /*radio box  B */
     n = narg;
     XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
     XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
     DLOG_RADIOB = XmCreateToggleButtonGadget(generictmp," ",args,n);
     XtAddCallback(                 DLOG_RADIOB,XmNvalueChangedCallback
                   ,(XtCallbackProc)DLOG_RADIOB_CB, NULL); 
     XtManageChild(                 DLOG_RADIOB);
     XmToggleButtonSetState(        DLOG_RADIOB
                                   ,dlog_radioBint, 0);
    xmstring = XmStringCreateSimple(dlog_radioBstr); 
    XtVaSetValues(                  DLOG_RADIOB,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_radioC)
 {
  /*radio box  C */
     n = narg;
     XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
     XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
     DLOG_RADIOC = XmCreateToggleButtonGadget(generictmp," ",args,n);
     XtAddCallback(                 DLOG_RADIOC,XmNvalueChangedCallback
                   ,(XtCallbackProc)DLOG_RADIOC_CB, NULL); 
     XtManageChild(                 DLOG_RADIOC);
     XmToggleButtonSetState(        DLOG_RADIOC
                                   ,dlog_radioCint, 0);
    xmstring = XmStringCreateSimple(dlog_radioCstr); 
    XtVaSetValues(                  DLOG_RADIOC,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_radioD)
 {
  /*radio box  D */
     n = narg;
     XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
     XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
     DLOG_RADIOD = XmCreateToggleButtonGadget(generictmp," ",args,n);
     XtAddCallback(                 DLOG_RADIOD,XmNvalueChangedCallback
                   ,(XtCallbackProc)DLOG_RADIOD_CB, NULL); 
     XtManageChild(                 DLOG_RADIOD);
     XmToggleButtonSetState(        DLOG_RADIOD
                                   ,dlog_radioDint, 0);
    xmstring = XmStringCreateSimple(dlog_radioDstr); 
    XtVaSetValues(                  DLOG_RADIOD,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_radioE)
 {
  /*radio box  E */
     n = narg;
     XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
     XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
     DLOG_RADIOE = XmCreateToggleButtonGadget(generictmp," ",args,n);
     XtAddCallback(                 DLOG_RADIOE,XmNvalueChangedCallback
                   ,(XtCallbackProc)DLOG_RADIOE_CB, NULL); 
     XtManageChild(                 DLOG_RADIOE);
     XmToggleButtonSetState(        DLOG_RADIOE
                                   ,dlog_radioEint, 0);
    xmstring = XmStringCreateSimple(dlog_radioEstr); 
    XtVaSetValues(                  DLOG_RADIOE,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_radioF)
 {
  /*radio box  F */
     n = narg;
     XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
     XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
     DLOG_RADIOF = XmCreateToggleButtonGadget(generictmp," ",args,n);
     XtAddCallback(                 DLOG_RADIOF,XmNvalueChangedCallback
                   ,(XtCallbackProc)DLOG_RADIOF_CB, NULL); 
     XtManageChild(                 DLOG_RADIOF);
     XmToggleButtonSetState(        DLOG_RADIOF
                                   ,dlog_radioFint, 0);
    xmstring = XmStringCreateSimple(dlog_radioFstr); 
    XtVaSetValues(                  DLOG_RADIOF,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_radioG)
 {
  /*radio box  G */
     n = narg;
     XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
     XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
     DLOG_RADIOG = XmCreateToggleButtonGadget(generictmp," ",args,n);
     XtAddCallback(                 DLOG_RADIOG,XmNvalueChangedCallback
                   ,(XtCallbackProc)DLOG_RADIOG_CB, NULL); 
     XtManageChild(                 DLOG_RADIOG);
     XmToggleButtonSetState(        DLOG_RADIOG
                                   ,dlog_radioGint, 0);
    xmstring = XmStringCreateSimple(dlog_radioGstr); 
    XtVaSetValues(                  DLOG_RADIOG,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_radioH)
 {
  /*radio box  H */
     n = narg;
     XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
     XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
     DLOG_RADIOH = XmCreateToggleButtonGadget(generictmp," ",args,n);
     XtAddCallback(                 DLOG_RADIOH,XmNvalueChangedCallback
                   ,(XtCallbackProc)DLOG_RADIOH_CB, NULL); 
     XtManageChild(                 DLOG_RADIOH);
     XmToggleButtonSetState(        DLOG_RADIOH
                                   ,dlog_radioHint, 0);
    xmstring = XmStringCreateSimple(dlog_radioHstr); 
    XtVaSetValues(                  DLOG_RADIOH,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_radioI)
 {
  /*radio box  I */
     n = narg;
     XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
     XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
     DLOG_RADIOI = XmCreateToggleButtonGadget(generictmp," ",args,n);
     XtAddCallback(                 DLOG_RADIOI,XmNvalueChangedCallback
                   ,(XtCallbackProc)DLOG_RADIOI_CB, NULL); 
     XtManageChild(                 DLOG_RADIOI);
     XmToggleButtonSetState(        DLOG_RADIOI
                                   ,dlog_radioIint, 0);
    xmstring = XmStringCreateSimple(dlog_radioIstr); 
    XtVaSetValues(                  DLOG_RADIOI,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_checkLast_caption)
 {
  /*CAPTION for Last CHECK BOX*/
  n = 0; 
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftOffset, LEFT); n++;
  XtSetArg(args[n], XmNheight, boxhigh+10); n++;
  XtSetArg(args[n], XmNwidth, NWIDE); n++;
  if(genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  XtSetArg(args[n], XmNeditable, False); n++;
  DLOG_CHECKLAST_CAPTION = 
                       XtCreateWidget(" ",xmTextWidgetClass,generictmp,args,n);
  XtManageChild(DLOG_CHECKLAST_CAPTION);
  XmTextSetString(DLOG_CHECKLAST_CAPTION,dlog_checkLastcaptionstr);
  thisboxtop = thisboxtop + boxhigh+10;
 }
#ifdef OLDSTUPIDSEPARATOR
 if(Ldlog_checkL)
 {
  /*check box  L */
  n = 0;
  XtSetArg(args[n], XmNvisual, progvisual); n++;
  XtSetArg(args[n], XmNdepth, progdepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftOffset, LEFT); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_OPPOSITE_FORM); n++;
  if (genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
  DLOG_CHECKL = XmCreateToggleButtonGadget(generictmp," ",args,n);
  XtAddCallback(DLOG_CHECKL,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKJ_CB, NULL); 
  XtManageChild(DLOG_CHECKL);
  XmToggleButtonSetState(DLOG_CHECKL,dlog_checkLint, 0);
  xmstring =    XmStringCreateSimple(dlog_checkLstr); 
  XtVaSetValues(DLOG_CHECKL,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
#endif /*OLDSTUPIDSEPARATOR*/
 if(Ldlog_radioJ)
 {
  /*radio box  J */
     n = narg;
     XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
     XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
     DLOG_RADIOJ = XmCreateToggleButtonGadget(generictmp," ",args,n);
     XtAddCallback(                 DLOG_RADIOJ,XmNvalueChangedCallback
                   ,(XtCallbackProc)DLOG_RADIOJ_CB, NULL); 
     XtManageChild(                 DLOG_RADIOJ);
     XmToggleButtonSetState(        DLOG_RADIOJ
                                   ,dlog_radioJint, 0);
    xmstring = XmStringCreateSimple(dlog_radioJstr); 
    XtVaSetValues(                  DLOG_RADIOJ,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_radioK)
 {
  /*radio box  K */
     n = narg;
     XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
     XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
     DLOG_RADIOK = XmCreateToggleButtonGadget(generictmp," ",args,n);
     XtAddCallback(                 DLOG_RADIOK,XmNvalueChangedCallback
                   ,(XtCallbackProc)DLOG_RADIOK_CB, NULL); 
     XtManageChild(                 DLOG_RADIOK);
     XmToggleButtonSetState(        DLOG_RADIOK
                                   ,dlog_radioKint, 0);
    xmstring = XmStringCreateSimple(dlog_radioKstr); 
    XtVaSetValues(                  DLOG_RADIOK,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_radioL)
 {
  /*radio box  L */
     n = narg;
     XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
     XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
     DLOG_RADIOL = XmCreateToggleButtonGadget(generictmp," ",args,n);
     XtAddCallback(                 DLOG_RADIOL,XmNvalueChangedCallback
                   ,(XtCallbackProc)DLOG_RADIOL_CB, NULL); 
     XtManageChild(                 DLOG_RADIOL);
     XmToggleButtonSetState(        DLOG_RADIOL
                                   ,dlog_radioLint, 0);
    xmstring = XmStringCreateSimple(dlog_radioLstr); 
    XtVaSetValues(                  DLOG_RADIOL,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_radioM)
 {
  /*radio box  M */
     n = narg;
     XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
     XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
     DLOG_RADIOM = XmCreateToggleButtonGadget(generictmp," ",args,n);
     XtAddCallback(                 DLOG_RADIOM,XmNvalueChangedCallback
                   ,(XtCallbackProc)DLOG_RADIOM_CB, NULL); 
     XtManageChild(                 DLOG_RADIOM);
     XmToggleButtonSetState(        DLOG_RADIOM
                                   ,dlog_radioMint, 0);
    xmstring = XmStringCreateSimple(dlog_radioMstr); 
    XtVaSetValues(                  DLOG_RADIOM,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_radioN)
 {
  /*radio box  N */
     n = narg;
     XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
     XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
     DLOG_RADION = XmCreateToggleButtonGadget(generictmp," ",args,n);
     XtAddCallback(                 DLOG_RADION,XmNvalueChangedCallback
                   ,(XtCallbackProc)DLOG_RADION_CB, NULL); 
     XtManageChild(                 DLOG_RADION);
     XmToggleButtonSetState(        DLOG_RADION
                                   ,dlog_radioNint, 0);
    xmstring = XmStringCreateSimple(dlog_radioNstr); 
    XtVaSetValues(                  DLOG_RADION,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_radioO)
 {
  /*radio box  O */
     n = narg;
     XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
     XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
     DLOG_RADIOO = XmCreateToggleButtonGadget(generictmp," ",args,n);
     XtAddCallback(                 DLOG_RADIOO,XmNvalueChangedCallback
                   ,(XtCallbackProc)DLOG_RADIOO_CB, NULL); 
     XtManageChild(                 DLOG_RADIOO);
     XmToggleButtonSetState(        DLOG_RADIOO
                                   ,dlog_radioOint, 0);
    xmstring = XmStringCreateSimple(dlog_radioOstr); 
    XtVaSetValues(                  DLOG_RADIOO,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }
 if(Ldlog_radioP)
 {
  /*radio box  P */
     n = narg;
     XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
     XtSetArg(args[n], XmNbottomOffset, -thisboxtop-boxhigh); n++;
     DLOG_RADIOP = XmCreateToggleButtonGadget(generictmp," ",args,n);
     XtAddCallback(                 DLOG_RADIOP,XmNvalueChangedCallback
                   ,(XtCallbackProc)DLOG_RADIOP_CB, NULL); 
     XtManageChild(                 DLOG_RADIOP);
     XmToggleButtonSetState(        DLOG_RADIOP
                                   ,dlog_radioPint, 0);
    xmstring = XmStringCreateSimple(dlog_radioPstr); 
    XtVaSetValues(                  DLOG_RADIOP,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  thisboxtop = thisboxtop + boxhigh;
 }


 /*all dialogs will probably have at least an OK button*/
 if(Ldlog_OK || Ldlog_option || Ldlog_cancel)
 {/*control buttons row: OK, Option, Cancel */
    n = 0; 
    XtSetArg(args[n], XmNvisual, progvisual); n++;
    XtSetArg(args[n], XmNdepth, progdepth); n++;
    XtSetArg(args[n], XmNcolormap, colormap); n++;
    XtSetArg(args[n], XmNbackground, windowbackground); n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftOffset, LEFT); n++;
    XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
    XtSetArg(args[n], XmNwidth, NWIDE); n++;
    /*XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;*/
    genericbuttons = 
             XtCreateWidget("but", xmRowColumnWidgetClass,generictmp,args,n);
    XtManageChild(genericbuttons);

    if(Ldlog_OK)
    {
       n = 0; 
       XtSetArg(args[n], XmNvisual, progvisual); n++;
       XtSetArg(args[n], XmNdepth, progdepth); n++;
       XtSetArg(args[n], XmNcolormap, colormap); n++;
       XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
       XtSetArg(args[n], XmNwidth, 100); n++;
       XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
       XtSetArg(args[n], XmNbottomAttachment, XmATTACH_OPPOSITE_FORM); n++;
       XtSetArg(args[n], XmNbottomOffset, -boxhigh); n++;
       if (genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
       DLOG_OK = XmCreatePushButtonGadget(genericbuttons,"OK",args,n);
       XtAddCallback(DLOG_OK, XmNactivateCallback
                           ,(XtCallbackProc)DLOG_OK_CB, NULL); 
       XtManageChild(DLOG_OK);
       xmstring = XmStringCreateSimple(dlog_OKstr); 
       XtVaSetValues(DLOG_OK,XmNlabelString,xmstring,NULL);
       XmStringFree(xmstring);
    }
    if(Ldlog_option)
    {
       n = 0; 
       XtSetArg(args[n], XmNvisual, progvisual); n++;
       XtSetArg(args[n], XmNdepth, progdepth); n++;
       XtSetArg(args[n], XmNcolormap, colormap); n++;
       XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
       XtSetArg(args[n], XmNleftWidget, DLOG_OK); n++;
       XtSetArg(args[n], XmNleftOffset, 10); n++;
       XtSetArg(args[n], XmNwidth, 100); n++;
       XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
       XtSetArg(args[n], XmNbottomAttachment, XmATTACH_OPPOSITE_FORM); n++;
       XtSetArg(args[n], XmNbottomOffset, -boxhigh); n++;
       if (genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
       DLOG_OPTION = XmCreatePushButtonGadget(genericbuttons,"Option",args,n);
       XtAddCallback(DLOG_OPTION, XmNactivateCallback
                               ,(XtCallbackProc)DLOG_OPTION_CB, NULL); 
       XtManageChild(DLOG_OPTION);
       xmstring = XmStringCreateSimple(dlog_optionstr); 
       XtVaSetValues(DLOG_OPTION,XmNlabelString,xmstring,NULL);
       XmStringFree(xmstring);
    }
    if(Ldlog_cancel)
    {
       n = 0; 
       XtSetArg(args[n], XmNvisual, progvisual); n++;
       XtSetArg(args[n], XmNdepth, progdepth); n++;
       XtSetArg(args[n], XmNcolormap, colormap); n++;
       XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
       XtSetArg(args[n], XmNleftWidget, DLOG_OPTION); n++;
       XtSetArg(args[n], XmNleftOffset, 10); n++;
       XtSetArg(args[n], XmNwidth, 100); n++;
       XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
       XtSetArg(args[n], XmNbottomAttachment, XmATTACH_OPPOSITE_FORM); n++;
       XtSetArg(args[n], XmNbottomOffset, -boxhigh); n++;
       if (genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
       DLOG_CANCEL = XmCreatePushButtonGadget(genericbuttons,"Cancel",args,n);
       XtAddCallback(DLOG_CANCEL, XmNactivateCallback
                               ,(XtCallbackProc)DLOG_CANCEL_CB, NULL); 
       XtManageChild(DLOG_CANCEL);
       xmstring = XmStringCreateSimple(dlog_cancelstr); 
       XtVaSetValues(DLOG_CANCEL,XmNlabelString,xmstring,NULL);
       XmStringFree(xmstring);
    }
    thisboxtop = thisboxtop + boxhigh+10;
 }/*control buttons row: OK, Option, Cancel */
 if(Ldlog_info)
 {
    /*Information*/
     n = 0; 
     XtSetArg(args[n], XmNvisual, progvisual); n++;
     XtSetArg(args[n], XmNdepth, progdepth); n++;
     XtSetArg(args[n], XmNcolormap, colormap); n++;
     XtSetArg(args[n], XmNbackground, windowbackground); n++;
     XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
     XtSetArg(args[n], XmNtopOffset, thisboxtop); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftOffset, LEFT); n++;
     XtSetArg(args[n], XmNheight, 5*boxhigh); n++;
     XtSetArg(args[n], XmNwidth, NWIDE); n++;
     if (genericFont) { XtSetArg(args[n], XmNfontList,genericfontList); n++; }
     XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
     XtSetArg(args[n], XmNeditable, False); n++;
     DLOG_INFO = XtCreateWidget(" ", xmTextWidgetClass,generictmp,args,n);
     XtManageChild(DLOG_INFO);
     XmTextSetString(DLOG_INFO,dlog_infostr);
 }

#endif /* UNIX_MOTIF 060324 re compile with NOGUI, requires Lquiet=1*/
}
/*___DogenericDialog()______________________________________________________*/


