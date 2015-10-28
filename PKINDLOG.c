/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

#include "PKIN.h"
#define EXTERNDLOG
#include "PKINDLOG.h"

                             /*PKINDLOG.c*/

/*   generic Dialog sequence:
DoSpecialDialog
   cleargenericflags    PKIN
   SpecialDialog        PKIN
   DogenericDialog      P___
      genericDialog     P___
--processing of dialog- P___
 ---clean up dialog --- P___
genericdlogrouter       PKIN
EndSpecialDialog        PKIN
*/


/****cleargenericflags()*****************************************************/
void cleargenericflags()
{
 Ldlog_subject = 0;
 Ldlog_paramA  = 0;
 Ldlog_textA   = 0;
 Ldlog_paramB  = 0;
 Ldlog_textB   = 0;
 Ldlog_paramC  = 0;
 Ldlog_textC   = 0;
 Ldlog_paramD  = 0;
 Ldlog_textD   = 0;
 Ldlog_paramE  = 0;
 Ldlog_textE   = 0;
 Ldlog_paramF  = 0;
 Ldlog_textF   = 0;
 Ldlog_paramG  = 0;
 Ldlog_textG   = 0;
 Ldlog_paramH  = 0;
 Ldlog_textH   = 0;
 Ldlog_paramI  = 0;
 Ldlog_textI   = 0;
 Ldlog_info    = 0;
 Ldlog_check_caption = 0;
 Ldlog_checkA  = 0;
 Ldlog_checkB  = 0;
 Ldlog_checkC  = 0;
 Ldlog_checkD  = 0;
 Ldlog_checkE  = 0;
 Ldlog_checkF  = 0;
 Ldlog_checkG  = 0;
 Ldlog_checkH  = 0;
 Ldlog_checkI  = 0;
 Ldlog_checkJ  = 0;
 Ldlog_checkK  = 0;
 Ldlog_checkL  = 0;
 Ldlog_checkM  = 0;
 Ldlog_checkN  = 0;
 Ldlog_checkO  = 0;
 Ldlog_checkP  = 0;
 Ldlog_checkQ  = 0;
 Ldlog_checkLast_caption = 0;
 Ldlog_checkR  = 0; /*990130*/
 Ldlog_checkS  = 0; /*990130*/
 Ldlog_checkT  = 0; /*990130*/
 Ldlog_checkU  = 0; 
 Ldlog_checkV  = 0; /*041102 not used*/
 Ldlog_checkW  = 0; /*041102 not used*/
 Ldlog_checkX  = 0; /*041102 not used*/

 Ldlog_radio_caption = 0;
 Ldlog_radioA  = 0;
 Ldlog_radioB  = 0;
 Ldlog_radioC  = 0;
 Ldlog_radioD  = 0;
 Ldlog_radioE  = 0;
 Ldlog_radioF  = 0;
 Ldlog_radioG  = 0;
 Ldlog_radioH  = 0;
 Ldlog_radioI  = 0;
 Ldlog_radioJ  = 0;
 Ldlog_radioK  = 0;
 Ldlog_radioL  = 0;
 Ldlog_radioM  = 0;
 Ldlog_radioN  = 0;
 Ldlog_radioO  = 0;
 Ldlog_radioP  = 0;

 Ldlog_OK  = 0;
 Ldlog_option  = 0;
 Ldlog_cancel  = 0;
 sprintf(dlog_OKstr,"    OK    ");
 sprintf(dlog_cancelstr,"CANCEL");
 sprintf(dlog_optionstr," ");
 sprintf(dlog_textAstr," "); 
 sprintf(dlog_textBstr," "); 
 sprintf(dlog_textCstr," ");
 sprintf(dlog_textDstr," ");
 sprintf(dlog_textEstr," ");
 sprintf(dlog_textFstr," ");
 sprintf(dlog_textGstr," ");
 sprintf(dlog_textHstr," ");
 sprintf(dlog_subjectstr," ");
 sprintf(dlog_infostr," ");
 sprintf(dlog_checkcaptionstr," ");
 sprintf(dlog_radiocaptionstr," ");
 sprintf(dlog_checkLastcaptionstr," ");
 ACTIVE_DLOG = 0;
 Ldlog_OKHIT = 0;
 Ldlog_cancelHIT = 0;
 Ldlog_optionHIT = 0;
}
/*___cleargenericflags()____________________________________________________*/

/****genericdlogrouter()******************************************************/ 
void  genericdlogrouter()
{
  /*now the generic parameters have been adjusted by the generic dialog*/
  /*need to get back to the specific dialog and set specific parameters */

 switch(ACTIVE_DLOG)
 {
    case  INIT_DLOG:            EndInitDialog();             break;
    case  REPORT_DLOG:          EndReportDialog();           break;
    case  RANGESTART_DLOG:      EndRangeStartDialog();       break;
    case  BUILTINSCRIPTS_DLOG:  EndBuiltinScriptsDialog();   break;
    case  EXTERNALSCRIPT_DLOG:  EndExternalScriptDialog();   break;
    case  KLUDGES_DLOG:         EndKludgesDialog();          break; 
    case  RANGECONTROL_DLOG:    EndRangeControlDialog();     break;
    case  RUNCONDITIONS_DLOG:   EndRunConditionsDialog();    break; 
    case  ATOMMARKER_DLOG:      EndAtomMarkerDialog();       break; 
    case  FOCUSOPTIONS_DLOG:    EndFocusOptionsDialog();     break;
    case  FOCUSPARAM_DLOG:      EndFocusParamDialog();       break;
    case  FOCUSBYRESNUM_DLOG:   EndFocusParamDialog();       break;
    case  RIBBON_DLOG:          EndRibbonDialog();           break;
    case  COLORSCALE_DLOG:      EndColorScaleDialog();       break;

 }  
}
/*___genericdlogrouter()_____________________________________________________*/

/****DoInitDialog()***********************************************************/
void DoInitDialog(void)
{
    if(!ACTIVE_DLOG)
    {
        cleargenericflags(); /*PKINDLOG.c*/
        InitDialog();  /*PKINDLOG.c*/
        DogenericDialog();   /*____DLOG.c*/
    }
}
/*___DoInitDialog()__________________________________________________________*/

/****InitDialog()*************************************************************/
void InitDialog()      /*INIT_DLOG*/
{
   sprintf(dlog_subjectstr,"%s",alertstr);
   Ldlog_subject = 1;
   sprintf(dlog_OKstr,"PROCEED");
   Ldlog_OK = 1;
   sprintf(dlog_optionstr,"EXPLANATION");
   Ldlog_option = 1;
   sprintf(dlog_cancelstr,"QUIT");
   Ldlog_cancel = 1;
   ACTIVE_DLOG = INIT_DLOG;
   Lwaiting = 1; /*wheel spinner while Dialog is active*/
}
/*___InitDialog()____________________________________________________________*/

/****EndInitDialog()**********************************************************/
void EndInitDialog()
{
   int j;
   
   if(Ldlog_OKHIT)
   {
      Lnewfile=1;
   }
   else if(Ldlog_cancelHIT)
   {
      /* Quit */
      if(fpinput!=0) fclose(fpinput);
      if(fpoutput!=0) fclose(fpoutput);
      exit(0);
   }
   else if(Ldlog_optionHIT)
   {
      Lexplanation = 1;
   }
   for(j=0; j<256; j++) alertstr[j] = '\0';  /* clear alert report */
   ACTIVE_DLOG = 0;
   Lwaiting = 0; /*wheel spinner while Dialog active*/
}
/*___EndInitDialog()_________________________________________________________*/

/****DoReportDialog()*********************************************************/
void DoReportDialog(void)
{
  if(!Lquiet)
  {
    if(!ACTIVE_DLOG)
    {
        cleargenericflags(); /*PKINDLOG.c*/
        ReportDialog();  /*PKINDLOG.c*/
        DogenericDialog();   /*____DLOG.c*/
#ifdef UNIX_X11
        while(Lwaiting) HandleEvent();
#endif
    }
  }
  else
  {/*presume if have commandline so can run with Lquiet, has stderr*/
     fprintf(stderr,"%s\n",alertstr);
  }
}
/*___DoReportDialog()________________________________________________________*/

/****ReportDialog()***********************************************************/
void ReportDialog()       /*REPORT_DLOG*/
{
   sprintf(dlog_subjectstr,"%s",alertstr);
   Ldlog_subject = 1;
   sprintf(dlog_OKstr,"PROCEED");
   Ldlog_OK = 1;
   sprintf(dlog_optionstr,"QUIT");
   Ldlog_option = 1;
   ACTIVE_DLOG = REPORT_DLOG;
   Lwaiting = 1; /*wheel spinner while Dialog is active*/
}
/*___ReportDialog()__________________________________________________________*/

/****EndReportDialog()********************************************************/
void EndReportDialog()
{
   int j;
   if(Ldlog_OKHIT)
   {
      ; /*NOP*/
   }
   else if(Ldlog_optionHIT)
   {
      /* Quit */
      if(fpinput!=0) fclose(fpinput);
      if(fpoutput!=0) fclose(fpoutput);
      exit(0);
   }
   for(j=0; j<256; j++) alertstr[j] = '\0';  /* clear alert report */
   ACTIVE_DLOG = 0;
   Lwaiting = 0; /*wheel spinner while Dialog active*/
}
/*___EndReportDialog()_______________________________________________________*/

/*****DoRangeStartDialog()****************************************************/
void DoRangeStartDialog(void)
{
     if(!ACTIVE_DLOG)
     {
         cleargenericflags(); /*PKINDLOG.c*/
         RangeStartDialog();  /*PKINDLOG.c*/
         DogenericDialog();   /*____DLOG.c*/
     }
}
/*____DoRangeStartDialog_____________________________________________________*/

/****RangeStartDialog()*******************************************************/
void RangeStartDialog(void)      /*RANGESTART_DLOG*/
{
#ifdef PCWINDOWS
 sprintf(dlog_subjectstr,"PREKIN NEW PASS:"CRLF"coordinates from file: %s"
                          CRLF"%s"
                         ,dirstrip(NameStr),chainreport); /*041031*/
#endif
#ifndef PCWINDOWS
 sprintf(dlog_subjectstr,"PREKIN NEW PASS:"CRLF"coordinates from file: %s"
                          CRLF"%s"
                         ,NameStr,chainreport); /*041031*/
#endif
 Ldlog_subject = 1;
 sprintf(dlog_infostr,
      "for C-alphas of whole molecule, choose Backbone Browsing Script"
      CRLF"built-in scripts for other, standard combinations"
      CRLF"New Ranges lets you build your own selection"
      );
 Ldlog_info = 1;
   
 dlog_radioAint = 1; /*990407 forced defaults for New Pass dialog */
 sprintf(dlog_radioAstr,"Backbone Browsing Script");
 Ldlog_radioA = 1;

 dlog_radioBint = 0;
 sprintf(dlog_radioBstr,"Selection of built-in scripts");
 Ldlog_radioB = 1;

 dlog_radioCint = 0;
 sprintf(dlog_radioCstr,"New Ranges");
 Ldlog_radioC = 1;

 dlog_radioDint = 0;
 sprintf(dlog_radioDstr,"Focus only");
 Ldlog_radioD = 1;

/* obselete:  scripted ranges not kept up, not used!?, 050929
 dlog_radioEint = 0;
 sprintf(dlog_radioEstr,"Range List from external file");
 Ldlog_radioE = 1;
*/
 dlog_radioFint = 0;
 sprintf(dlog_radioFstr,"first: Reset Options");
 Ldlog_radioF = 1;

 sprintf(dlog_OKstr,"      OK      ");
 Ldlog_OK = 1;

 sprintf(dlog_optionstr,"   Quit   ");
 Ldlog_option = 1;

 sprintf(dlog_cancelstr,"restart this Pass");
 Ldlog_cancel = 1;

    ACTIVE_DLOG = RANGESTART_DLOG;
    Lwaiting = 1; /*wheel spinner while Dialog is active*/
}
/*___RangeStartDialog()______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****EndRangeStartDialog()****************************************************/ 
void EndRangeStartDialog(void)
{ 
 /*clear control logicals*/
 Lbuiltin = 0;
 Lexternal = 0;
 Lrangecontrols = 0;
 Lbrowsedefault = 0;

 if(Ldlog_OKHIT)
 {
    /* interpret various radio integers */
    if     (dlog_radioAint == 1) 
    {
       Lbrowsedefault = 1; /*go right to default script*/
       Lbuiltin = 1;
    }
    else if(dlog_radioBint == 1) 
    {
       Lbuiltin  = 1;
       Lbuiltindialog = 1; /*Do dialog to get script choice*/
    }
    else if(dlog_radioCint == 1) /* do range controls */
    {
        nrange = 1; /*==0 ends range control, similar to Lrangecontrols scope*/
        n = 0;  /*global used for range counter, */
              /* but global mrange carries this info for later use */
        Lrangecontrols = 1;
        Lstartarange = 1;
    }
    else if(dlog_radioDint == 1) /*focus only*/
    {
       Lfirstsubunit=0;
       Lallsubunits=1;
       Ldetails = 1;
       Lfocusoption = 1;
       Lresnumfovea = 1; /*control global*/
    }

  /*obseleted 050929
    else if(dlog_radioEint == 1) Lexternal = 1;
  */
    else if(dlog_radioFint == 1) Lkluges = 1;
    
    else Lrangestart = 1; /*since ALL radio buttons can be off!!!!*/
 }
 else if(Ldlog_optionHIT)
 {
    /* Quit */
    if(fpinput!=0) fclose(fpinput);
    if(fpoutput!=0) fclose(fpoutput);
    exit(0);
 }
 else if(Ldlog_cancelHIT)
 {
     Lrangestart = 1;
 }
 ACTIVE_DLOG = 0;
 Lwaiting = 0; /*wheel spinner while Dialog active*/
}
/*___EndRangeStartDialog()___________________________________________________*/

/*****DoBuiltinScriptsDialog**************************************************/
void DoBuiltinScriptsDialog(void)
{
     if(!ACTIVE_DLOG)
     {
         cleargenericflags();     /*PKINDLOG.c*/
         BuiltinScriptsDialog();  /*PKINDLOG.c*/
         DogenericDialog();       /*PUXMDLOG.c*/
     }
}
/*____DoBuiltinScriptsDialog_________________________________________________*/

/****BuiltinScriptsDialog()***************************************************/
void BuiltinScriptsDialog(void)   /*BUILTINSCRIPTS_DLOG*/
{
 sprintf(dlog_subjectstr,"kinemage built-in scripts");
 Ldlog_subject = 1;

 sprintf(dlog_infostr,
      "for C-alphas of whole molecule, choose caSS"
      CRLF"built-in scripts for other, standard combinations"
      CRLF"range controls lets you build your own selection"
      );
 Ldlog_info = 1;
   
 dlog_radioAint = 1;
 sprintf(dlog_radioAstr /*1*/
  ,"caSS - Calphas & -ss- (or nucleic acid pseudo-backbone) non-H2O hets");
 Ldlog_radioA = 1;

 dlog_radioBint = 0; 
 sprintf(dlog_radioBstr /*2*/
  ,"mcHb - mainchain and its h-bonds");
 Ldlog_radioB = 1;

 dlog_radioCint = 0;
 sprintf(dlog_radioCstr /*3*/
  ,"aasc - individual amino acids in separate sets and Calphas");
 Ldlog_radioC = 1;

 dlog_radioDint = 0;
 sprintf(dlog_radioDstr /*9*/
  ,"aasc+R - individual amino acids in separate sets and Calphas and ranges");
 Ldlog_radioD = 1;

 dlog_radioEint = 0;
 sprintf(dlog_radioEstr /*6*/
  ,"naba - individual nucleic acid bases in separate sets and lots else");
 Ldlog_radioE = 1;

 dlog_radioFint = 0;
 sprintf(dlog_radioFstr /*7*/
  ,"separate bases, pseudo-backbone, non-carbon atom markers");
 Ldlog_radioF = 1;

 dlog_radioGint = 0;
 sprintf(dlog_radioGstr /*4*/
  ,"lots - mc, sc, ca, (hy), (hets), H2O balls");
 Ldlog_radioG = 1;

 dlog_radioHint = 0;
 sprintf(dlog_radioHstr /*8*/
  ,"ribbon: varible width dependent on local curvature ");
 Ldlog_radioH = 1;

 dlog_radioIint = 0;
 sprintf(dlog_radioIstr /*5*/
  ,"bestribbon: HELIX & SHEET from pdb header, one-strand coil, flat nucleic acid ");
 Ldlog_radioI = 1;

 dlog_radioJint = 0;
 sprintf(dlog_radioJstr /*5*/
  ,"ribnhet: bestribbon and hets with nonCatom balls ");
 Ldlog_radioJ = 1;

 dlog_radioKint = 0;
 sprintf(dlog_radioKstr /*10*/
  ,"Table group  (based on protein Calphas)");
 Ldlog_radioK = 1;

 sprintf(dlog_OKstr,"OK");
 Ldlog_OK = 1;

 sprintf(dlog_optionstr,"do range controls instead");
 Ldlog_option = 1;

 sprintf(dlog_cancelstr,"restart this Pass");
 Ldlog_cancel = 1;

    ACTIVE_DLOG = BUILTINSCRIPTS_DLOG;
    Lwaiting = 1; /*wheel spinner while Dialog is active*/
}
/*___BuiltinScriptsDialog()__________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****EndBuiltinScriptsDialog()************************************************/ 
void EndBuiltinScriptsDialog(void)
{ 
 if(Ldlog_OKHIT)
 {
    /*set control logical*/
    Lbuiltin = 1; /*still true*/
    Lbuiltinchoice = 1; /*now set*/
    /* interpret various radio integers */
    if     (dlog_radioAint == 1) {ithscript =  1;}
    else if(dlog_radioBint == 1) {ithscript =  2;}
    else if(dlog_radioCint == 1) {ithscript =  3;}
    else if(dlog_radioDint == 1) {ithscript =  9;}
    else if(dlog_radioEint == 1) {ithscript =  6;}
    else if(dlog_radioFint == 1) {ithscript =  7;}
    else if(dlog_radioGint == 1) {ithscript =  4;}
    else if(dlog_radioHint == 1) {ithscript =  8;}
    else if(dlog_radioIint == 1) {ithscript =  5; Lribonucleic=1;} /*RNA type*/
    else if(dlog_radioJint == 1) {ithscript =  5;
            Lribonucleic = 1;
            LcolorbyNtoC = 1;
            Lblackedge = 1;
            Lhetconnect = 0;
            ballradius = (float)0.32;
            htxl[1] = 1;
            atxl[1] = 1;
            htal[1] = 1; /*set this here to simulate rangeend logic*/
    }
    else if(dlog_radioKint == 1) {ithscript = 10;}
    else {Lbuiltin = 0;Lrangestart = 1;}
  }
 else if(Ldlog_optionHIT)
 {
    Lbuiltin = 0; 
    Lrangecontrols = 1; /*will then do new ranges*/
        nrange = 1; /*==0 ends range control, similar to Lrangecontrols scope*/
        n = 0;  /*global used for range counter, */
              /* but global mrange carries this info for later use */
        Lrangecontrols = 1;
        Lstartarange = 1;

 }
 else if(Ldlog_cancelHIT)
 {
    Lbuiltin = 0; 
    Lrangestart = 1;
 }
 Lbuiltindialog = 0; /*clear flag, not under this control anymore*/
 ACTIVE_DLOG = 0;
 Lwaiting = 0; /*wheel spinner while Dialog active*/
 
}
/*___EndBuiltinScriptsDialog()_______________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/*****DoExternalScriptDialog**************************************************/
void DoExternalScriptDialog(void)
{
     if(!ACTIVE_DLOG)
     {
         cleargenericflags();     /*PKINDLOG.c*/
         ExternalScriptDialog();  /*PKINDLOG.c*/
         DogenericDialog();       /*PUXMDLOG.c*/
     }
}
/*____DoExternalScriptDialog_________________________________________________*/

/****ExternalScriptDialog()***************************************************/
void ExternalScriptDialog(void) /*EXTERNALSCRIPT_DLOG*/
 /*050929 obseleted: should never be callled*/
{
 sprintf(dlog_subjectstr,"External Range List to feed Range Controls");
 Ldlog_subject = 1;

 dlog_radioAint = 1;
 sprintf(dlog_radioAstr ,"accept list exactly");
 Ldlog_radioA = 1;

 dlog_radioBint = 0;
 sprintf(dlog_radioBstr ,"modify list as it is read in");
 Ldlog_radioB = 1;

 dlog_radioCint = 0;
 sprintf(dlog_radioCstr ,"find residues by labellist coordinates");
 Ldlog_radioC = 1;

 sprintf(dlog_optionstr,"do Range Controls instead");
 Ldlog_option = 1;

 sprintf(dlog_OKstr,"OK");
 Ldlog_OK = 1;

 sprintf(dlog_cancelstr,"restart this Pass");
 Ldlog_cancel = 1;

    ACTIVE_DLOG = EXTERNALSCRIPT_DLOG;
    Lwaiting = 1; /*wheel spinner while Dialog is active*/
}
/*___ExternalScriptDialog()__________________________________________________*/

/****EndExternalScriptDialog()************************************************/ 
void EndExternalScriptDialog(void)
{ 
 if(Ldlog_OKHIT)
 {
    /* interpret various radio integers */
    if     (dlog_radioAint == 1)
    {/*accept script exactly*/
        Laccept = 1;
        Lmodify = 0;
        Lopenscriptin = 1; /*for flow control in PKINRNGE.c */ 
        Llabellist = 0;
    }
    else if(dlog_radioBint == 1)
    {/*modify script as it is read in*/
        Laccept = 0;
        Lmodify = 1;    
        Lopenscriptin = 1; /*for flow control in PKINRNGE.c */ 
        Llabellist = 0;
    }
    else if(dlog_radioCint == 1)
    {/*find residues by labellist coordinates*/
        Laccept = 0;
        Lmodify = 1;
        Llabellist = 1;
        Lopenscriptin = 1; /*for flow control in PKINRNGE.c */ 
    }
    else /*NO radios ON */
    {/*restart this Pass*/
        Laccept = 0;
        Lmodify = 0;
        Llabellist = 0;
        Lrangestart = 1;
    }
 }
 else if(Ldlog_optionHIT)
 {/*do new range controls instead*/
     Laccept = 0;
     Lmodify = 0;
     Llabellist = 0;
     Lrangecontrols = 1; /*will then do range controls */
     Lstartarange = 1;
 }
 else if(Ldlog_cancelHIT)
 {/*restart this Pass*/
     Laccept = 0;
     Lmodify = 0;
     Llabellist = 0;
     Lrangestart = 1;
 }
 Lexternal = 0;  /*no matter what, not under this control anymore*/
 ACTIVE_DLOG = 0;
 Lwaiting = 0; /*wheel spinner while Dialog active*/
}
/*___EndExternalScriptDialog()_______________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*****DoKludgesDialog()********************************************************/
void DoKludgesDialog(void)  
{
     if(!ACTIVE_DLOG)
     {
         cleargenericflags();  /*PKINDLOG.c*/
         KludgesDialog();       /*PKINDLOG.c*/
         DogenericDialog();    /*____DLOG.c*/
     }
}
/*___DoKludgesDialog()________________________________________________________*/

/****KludgesDialog()***********************************************************/
void KludgesDialog(void)   /*KLUDGES_DLOG*/
{
 sprintf(dlog_subjectstr,"Various Kludges");
 Ldlog_subject = 1;

 sprintf(dlog_checkcaptionstr,"independant Kludges:");
 Ldlog_check_caption = 1;

 dlog_checkAint = Ltestprint;
 sprintf(dlog_checkAstr ,"debug test print statements");
 Ldlog_checkA = 1;

 dlog_checkBint = Lleftadjatomname;
 sprintf(dlog_checkBstr ,"Left adjusted, non-standard, PDB atom names");
 Ldlog_checkB = 1;

 dlog_checkCint = Lhetconnect;
 sprintf(dlog_checkCstr ,"connect hetatom 'residues' within a subunit");
 Ldlog_checkC = 1;

 dlog_checkDint = IDEMO;
 sprintf(dlog_checkDstr 
    ,"DEMO mode: just BIG print in text window to demo PREKIN");
 Ldlog_checkD = 1;

 dlog_checkEint = Listext;
 sprintf(dlog_checkEstr 
    ,"extra output at end of this pass");
 Ldlog_checkE = 1;

 dlog_checkFint = Lcolorscale;
 sprintf(dlog_checkFstr 
    ,"Change color scale values, e.g. for B-value colors");
 Ldlog_checkF = 1;

 dlog_checkGint = Leachsclist;
 sprintf(dlog_checkGstr 
    ,"each sidechain in a separate, nobutton, list");
 Ldlog_checkG = 1;

 dlog_checkHint = Lballonscpoints;
 sprintf(dlog_checkHstr 
    ,"individual balls on sidechain n,o,s atoms");
 Ldlog_checkH = 1;

 dlog_checkIint = Lfillscrings;
 sprintf(dlog_checkIstr 
    ,"triangle tesselation fills sidechain rings");
 Ldlog_checkI = 1;

 dlog_checkJint = Lonlygroups;
 sprintf(dlog_checkJstr 
    ,"groups dominant for each chain, subgroups by masters");
 Ldlog_checkJ = 1;

 dlog_checkKint = Lonegroup;
 sprintf(dlog_checkKstr 
    ,"Just one group dominant; chains and subgroups by masters");
 Ldlog_checkK = 1;

 dlog_checkLint = Lnogroups;
 sprintf(dlog_checkLstr 
    ,"NO groups; all chains and subgroups by masters");
 Ldlog_checkL = 1;

         /*050211 this is now the place that initiates VRML output*/
 dlog_checkMint = LdumpVRML;
 sprintf(dlog_checkMstr ,"VRML output, e.g. for 3D Printer");
 Ldlog_checkM = 1;

 sprintf(dlog_radiocaptionstr,"Range Controls apply:");
 Ldlog_radio_caption = 1;

 dlog_radioAint = (Linclusive && !Loverlap);
 sprintf(dlog_radioAstr ,"Inclusive (default)");
 Ldlog_radioA = 1;

 dlog_radioBint = (!Linclusive && !Loverlap);
 sprintf(dlog_radioBstr ,"Exclusive");
 Ldlog_radioB = 1;

 dlog_radioCint = (Linclusive && Loverlap);
 sprintf(dlog_radioCstr ,"Inclusive lapped out by one");
 Ldlog_radioC = 1;

 sprintf(dlog_OKstr,"OK");
 Ldlog_OK = 1;

 sprintf(dlog_cancelstr,"CANCEL");
 Ldlog_cancel = 1;

    ACTIVE_DLOG = KLUDGES_DLOG;
    Lwaiting = 1; /*wheel spinner while Dialog is active*/
}
/*___KludgesDialog()__________________________________________________________*/

/****EndKludgesDialog()********************************************************/
void EndKludgesDialog(void)  
{
 if(Ldlog_OKHIT)
 {
    /* interpret various radio integers */
    if     (dlog_radioAint == 1) {Linclusive = 1; Loverlap = 0;}
    else if(dlog_radioBint == 1) {Linclusive = 0; Loverlap = 0;}
    else if(dlog_radioCint == 1) {Linclusive = 1; Loverlap = 1;}
    
    /* interpret various check boxes */
    Ltestprint =       dlog_checkAint;
    Lleftadjatomname = dlog_checkBint;
    Lhetconnect =      dlog_checkCint;
    IDEMO =            dlog_checkDint;
    Listext =          dlog_checkEint;
    Lcolorscale =      dlog_checkFint;
    Leachsclist =      dlog_checkGint;
    Lballonscpoints =  dlog_checkHint;
    Lfillscrings    =  dlog_checkIint;
    Lonlygroups     =  dlog_checkJint;
    Lonegroup       =  dlog_checkKint;
    Lnogroups       =  dlog_checkLint;
    LdumpVRML       =  dlog_checkMint;
    if(LdumpVRML)
    {
       nstrnd = 5;
       ballradius = 2*ribwidVRML/5; /*expecting nstrnd==5  050211*/
    }
 }
 else if(Ldlog_optionHIT)
 {
    ;
 }
 else if(Ldlog_cancelHIT)
 {
    /*Why did this bother to reset to arbitrary defaults here????
      rather than just bailing out????
    Linclusive = 1; 
    Loverlap = 0; 
    Ltestprint =       0;
    Lleftadjatomname = 0;
    Lhetconnect =      1; 990403 changed default
    IDEMO =            0;
    */
    ; /*NOP*/
 }
 Lrangestart = 1; /*always return to rangestart*/
 Lkluges = 0; /*clear flag*/
 ACTIVE_DLOG = 0;
 Lwaiting = 0; /*wheel spinner while Dialog active*/
}
/*___EndKludgesDialog()_______________________________________________________*/

/*****DoRangeControlDialog()**************************************************/
void DoRangeControlDialog(void) 
{
     if(!ACTIVE_DLOG)
     {
         cleargenericflags();  /*PKINDLOG.c*/
         RangeControlDialog(); /*PKINDLOG.c*/
         DogenericDialog();    /*____DLOG.c*/
     }
}
/*___DoRangeControlDialog()__________________________________________________*/
 
/****RangeControlDialog()*****************************************************/
void RangeControlDialog(void)   /*RANGECONTROL_DLOG*/
{
       int        j;

 LRCDialogInput = 0;

       if(Lscriptin&&!Llabellist) j = n;
       else j = n-1;

 if(Lscriptin) sprintf(word,"from script");
 else          sprintf(word," ");
 sprintf(dlog_subjectstr,"Previous range information"
  CRLF"     %s"CRLF"     %s"CRLF"%s"CRLF"This range   %d   %s"
  ,olderstr,oldstr,newstr,n,word);
 Ldlog_subject = 1;

 resnu1[n] = resnu1[j];
 resnu2[n] = resnu2[j];

 sprintf(dlog_paramAstr,"%d",resnu1[n]);
 sprintf(dlog_textAstr,"Number of start residue");
 Ldlog_paramA = 1;
 Ldlog_textA = 1;
 sprintf(dlog_paramBstr,"%d",resnu2[n]);
 sprintf(dlog_textBstr,"Number of end residue");
 Ldlog_paramB = 1;
 Ldlog_textB = 1;
 sprintf(dlog_paramCstr,"...");
 sprintf(dlog_textCstr,"3-letter residue code");
 Ldlog_paramC = 1;
 Ldlog_textC = 1;
 
 dlog_checkAint = mcxl[n]=mcxl[j];
 sprintf(dlog_checkAstr ,"mc: mainchain");
 Ldlog_checkA = 1;

 dlog_checkBint = scxl[n]=scxl[j];
 sprintf(dlog_checkBstr ,"sc: sidechain");
 Ldlog_checkB = 1;

 dlog_checkCint = hbxl[n]=hbxl[j];
 sprintf(dlog_checkCstr ,"hb: h-bonds (mainchain)");
 Ldlog_checkC = 1;

 dlog_checkDint = hyxl[n]=hyxl[j];
 sprintf(dlog_checkDstr ,"hy: hydrogens");
 Ldlog_checkD = 1;

 dlog_checkEint = caxl[n]=caxl[j];
 sprintf(dlog_checkEstr,"ca: protein Calphas; nucl.acid pseudo-backbone");
 Ldlog_checkE = 1;

 dlog_checkFint = lbxl[n]=lbxl[j];
 sprintf(dlog_checkFstr ,"lb: residue labels");
 Ldlog_checkF = 1;

 dlog_checkGint = htxl[n]=htxl[j];
 sprintf(dlog_checkGstr ,"ht: hetatoms (non-water)");
 Ldlog_checkG = 1;

 dlog_checkHint = waxl[n]=waxl[j];
 sprintf(dlog_checkHstr ,"wa: water");
 Ldlog_checkH = 1;

 dlog_checkIint = atxl[n]=atxl[j];
 sprintf(dlog_checkIstr ,"at: atom markers or balls (not C or H)");
 Ldlog_checkI = 1;

 dlog_checkJint = ctxl[n]=ctxl[j];
 sprintf(dlog_checkJstr ,"   atom markers on carbons as well as n,o,s");
 Ldlog_checkJ = 1;

 dlog_checkKint = mcCPKl[n]=mcCPKl[j];
 sprintf(dlog_checkKstr ,"mc CPKs instead of vectors");
 Ldlog_checkK = 1;

 dlog_checkLint = scCPKl[n]=scCPKl[j];
 sprintf(dlog_checkLstr ,"sc CPKs instead of vectors");
 Ldlog_checkL = 1;

 dlog_checkMint = htCPKl[n]=htCPKl[j];
 sprintf(dlog_checkMstr ,"ht CPKs instead of vectors");
 Ldlog_checkM = 1;

 dlog_checkNint = Bcol[n]=Bcol[j];
 sprintf(dlog_checkNstr ,"color by B-values");
 Ldlog_checkN = 1;

 dlog_checkOint = Bval[n]=Bval[j];
 sprintf(dlog_checkOstr ,"pointID B-values & occupancies");
 Ldlog_checkO = 1;

 dlog_checkPint = mutl[n]=0;
 sprintf(dlog_checkPstr ,"ro: mutation to idealized geom. of one residue");
 Ldlog_checkP = 1;

 dlog_checkQint = Laspectnc;
 sprintf(dlog_checkQstr ,"N-->C colored aspect");
 Ldlog_checkQ = 1;

 dlog_checkRint = Laspectabc;
 sprintf(dlog_checkRstr ,"ABC colored aspect");
 Ldlog_checkR = 1;

 dlog_checkSint = Lcbetadev;
 sprintf(dlog_checkSstr ,"C-beta deviations from ideal");
 Ldlog_checkS = 1;
 
 dlog_checkTint = supl[n]=supl[j];
 sprintf(dlog_checkTstr ,"superpos each instance of this aa on an ideal aa");
 Ldlog_checkT = 1;

 dlog_radioAint = Coil[n]=Coil[j];
 sprintf(dlog_radioAstr ,"Ribbon: COIL range: adds to header info");
 Ldlog_radioA = 1;
 dlog_radioBint = Alpha[n]=Alpha[j];
 sprintf(dlog_radioBstr ,"Ribbon: ALPHA range: adds to HELIX");
 Ldlog_radioB = 1;
 dlog_radioCint = Beta[n]=Beta[j];
 sprintf(dlog_radioCstr ,"Ribbon: BETA/nucleic range: adds to SHEET");
 Ldlog_radioC = 1;
 dlog_radioDint = Best[n]=Best[j];
 sprintf(dlog_radioDstr ,"bestribbon over this range");
 Ldlog_radioD = 1;
 dlog_radioEint = cisl[n]=cisl[j];
 sprintf(dlog_radioEstr ,"bestribbon with cis peptides emphasized"); /*cispep*/
 Ldlog_radioE = 1;
 dlog_radioFint = cisl[n]=cisl[j];
 sprintf(dlog_radioFstr ,"only cis peptides Ca--Ca emphasized");
 Ldlog_radioF = 1;

 dlog_radioGint = ribl[n]=ribl[j];
 sprintf(dlog_radioGstr ,"Ribbon: by curvature over entire range");
 Ldlog_radioG = 1;

/*the NO Ribbon choice lets one get out of the radio buttons once one is hit!*/
 dlog_radioHint = (!ribl[j]&&!Coil[j]&&!Alpha[j]&&!Beta[j]&&!Best[j]&&!cisl[j]);
 sprintf(dlog_radioHstr ,"NO Ribbon selection this range");
 Ldlog_radioH = 1;


 sprintf(dlog_checkLastcaptionstr,"--------------------------------- ");
 Ldlog_checkLast_caption = 1;

/*050929 jumper this script stuff -- it is not up to date, nor ever used!? 
 dlog_checkUint = 0;
 sprintf(dlog_checkUstr ,"write script when end ranges");
 Ldlog_checkU = 1;
*/
 dlog_radioMint = 1;
 sprintf(dlog_radioMstr ,"OK accepts and comes back for more");
 Ldlog_radioM = 1;

 dlog_radioNint = 0;
 sprintf(dlog_radioNstr ,"OK accepts and end ranges");
 Ldlog_radioN = 1;

 dlog_radioOint = 0;
 sprintf(dlog_radioOstr ,"OK ends ranges at previous selection");
 Ldlog_radioO = 1;

 sprintf(dlog_OKstr,"    OK    ");
 Ldlog_OK = 1;

 sprintf(dlog_optionstr,"Abandon range controls");
 Ldlog_option = 1;

 sprintf(dlog_cancelstr,"CANCEL this range");
 Ldlog_cancel = 1;

    ACTIVE_DLOG = RANGECONTROL_DLOG;
    Lwaiting = 1; /*wheel spinner while Dialog is active*/

}
/*___RangeControlDialog()____________________________________________________*/

/****EndRangeControlDialog()**************************************************/
void EndRangeControlDialog(void)  
{
 int    j;

 if(Ldlog_OKHIT)
 {
    /*get residue range (and identity)*/
    resnu1[n] = intfromstr(dlog_paramAstr); /*PKINUTIL.c*/
    resnu2[n] = intfromstr(dlog_paramBstr); /*PKINUTIL.c*/
    for(j=0;j<3;j++) resid[n][j] = dlog_paramCstr[j];
    resid[n][3] = '\0';
    if (    resid[n][0] != '.'
         && resid[n][1] != '.'
         && resid[n][2] != '.' ) resl[n]=1;/*look for changes*/
    if (    resid[n][0] == ' '
         && resid[n][1] == ' '
         && resid[n][2] == ' ' ) resl[n]=0;/*reset if blank*/
 
    /*strobe out check boxes*/
    mcxl[n] = dlog_checkAint;
    scxl[n] = dlog_checkBint;
    hbxl[n] = dlog_checkCint;
    hyxl[n] = dlog_checkDint;
    caxl[n] = dlog_checkEint;
    lbxl[n] = dlog_checkFint;
    htxl[n] = dlog_checkGint;
    waxl[n] = dlog_checkHint;
    atxl[n] = dlog_checkIint;
    ctxl[n] = dlog_checkJint;
    mcCPKl[n] = dlog_checkKint;
    scCPKl[n] = dlog_checkLint;
    htCPKl[n] = dlog_checkMint;
    Bcol[n] = dlog_checkNint;
    Bval[n] = dlog_checkOint;
    mutl[n] = dlog_checkPint;
    Laspectnc = dlog_checkQint;
    Laspectabc = dlog_checkRint;
    Lcbetadev = dlog_checkSint;
    /*if(Lcbetadev) {Ltaudev=1;}*/ /*011030*/ /*comment out 011212*/
    supl[n] = dlog_checkTint;
    if(supl[n])
    {
       Lsuperpos = 1; /*redundancy: supoutl vs Lsuperpos ????*/
    }
    

    /*ribbon controls*/
    /*ribl[n]  = dlog_radioAint;*/ /*990130*/ 
    /*050206 just ribl[n] by itself is NOT the right way to do this!*/
    /*This control calls for ribbon throughout this whole range */
    /*trying to do it piecemeal gets in trouble with the complicated way that*/
    /*various froms of ribbon are now specified */
    /*re PKINRNGE/builtinscripts()/case 8: ribbon fudged from local geometry*/
    if(dlog_radioAint)
    {
       Coil[n] = 1;
       /*050206 PKINRNGE/endarange() takes care of further settings...*/
    }
    Alpha[n] = dlog_radioBint; /*990130*/
    Beta[n]  = dlog_radioCint; /*990130*/
    if(dlog_radioCint)
    {/*nucleic acid ribbon to have beta-like arrows on delineated ranges*/
       Lnucleicarrows = 1; /*040226*/
    }
    if(dlog_radioDint || dlog_radioEint) /*bestribbon or bestribbon and cispep*/
    {/*setup to do bestribbon over this range*/
       Best[n] = 1; /*041103*/
       /*from PKININIT commandline invocation*/
       /*050206 PKINRNGE/endarange() takes care of further settings...*/
    }
    else
    {
       Best[n] = 0;
    }
    if(dlog_radioEint ||dlog_radioFint)/*cispeptide with or without bestribbon*/
    {/*041102 uses ribbon routines*/
       if(dlog_radioFint) {Best[n] = 1;}   /*need to invoke the ribbon code*/
       cisl[n] = 1;
       /*LvectorCO = 1;*/ /*flags subversion of ribbon code for this purpose*/
       Lcispeptides = 1; /*flags the special case of cispeptide emphasis*/
       extoutl = 1; /*to transfer these vectors from scratch to output*/
    }
    else
    {
       cisl[n] = 0;
    }
    ribl[n]  = dlog_radioGint; /*050206 now ribbon by curvature */

    if(scxl[n] && (ribl[n]||Coil[n]||Alpha[n]||Beta[n]||Best[n]) ) 
    {/*NOTE: in same range!*/
       Lcbstubsrib = 1; /*050206 does not work: no ribbon with selected sc*/ 
       sitel[n] = 1; /*fake as if a site record had set this residue*/
       Lsitefake = 1; /*fake existance of site records*/
         /*but do not automatically invoke site record residues here*/
    }

    if(ctxl[n] > 0) atxl[n]=1;/*atom markers on c imply on n,o,s*/
    
    /* interpret Last check box only when closing rangecontrols*/

    /* interpret various radio integers */
    if     (dlog_radioMint == 1) 
    {
        mrange = n;  /*use this nth range*/
        /*continue with ranges*/
    }
    else if(dlog_radioNint == 1) 
    {
        mrange = n;  /*use this nth range*/
        nrange = 0;   /*leave range control at end of this cycle*/
        if(Lscriptfileopen) fclose(fpscriptin);
            /* close script file if there was one */
        /* interpret Last check box only when closing rangecontrols*/
        Lopenscriptout = dlog_checkUint;
    }
    else if(dlog_radioOint == 1) 
    {
        mrange = n-1;  /*will ignore this nth range*/
        nrange = 0;   /*leave range control at end of this cycle*/
        if(Lscriptfileopen) fclose(fpscriptin);
            /* close script file if there was one */
        /* interpret Last check box only when closing rangecontrols*/
       /* Lopenscriptout = dlog_checkUint; NOT used, comment out 050929 */ 
    }
    else mrange = n;  /*use this nth range*/
    Lstartarange = 0;
    Lendarange = 1;
    /*endarange() will reset Lrangecontrols as needed to continue or not*/
 }
 else if(Ldlog_optionHIT)
 {
    Lrangecontrols = 0;
    Lstartarange = 0;
    Lendarange = 0;
    Lrangestart = 1;
    if(Lscriptfileopen) fclose(fpscriptin);
        /* close script file if there was one */
 }
 else if(Ldlog_cancelHIT)
 {
    mrange = n-1;  /*will ignore this nth range*/
    n = n-1;  /*will ignore this nth range*/
    Lstartarange = 1;
    Lendarange = 0;
 }
 ACTIVE_DLOG = 0;
 Lwaiting = 0; /*wheel spinner while Dialog active*/
}
/*___EndRangeControlDialog()________________________________________________*/

/*****DoFocusOptionsDialog****************************************************/
void DoFocusOptionsDialog(void)
{
     if(!ACTIVE_DLOG)
     {
         cleargenericflags();     /*PKINDLOG.c*/
         FocusOptionsDialog();    /*PKINDLOG.c*/
         DogenericDialog();       /*PUXMDLOG.c*/
     }
}
/*____DoFocusOptionsDialog___________________________________________________*/

/****FocusOptionsDialog()*****************************************************/
void FocusOptionsDialog(void)    /*FOCUSOPTIONS_DLOG*/
{
 sprintf(dlog_subjectstr,"Focus Point Options:");
 Ldlog_subject = 1;

 if(Lresnumfovea) dlog_radioAint = 0;
 else             dlog_radioAint = 1;
 sprintf(dlog_radioAstr ,"No focus at all");
 Ldlog_radioA = 1;

 dlog_radioBint = 0;
 sprintf(dlog_radioBstr ,"type in x,y,z, for a focus point");
 Ldlog_radioB = 1;

 dlog_radioCint = 0;
 sprintf(dlog_radioCstr ,"get pre-existing file with x,y,z, focus point");
 Ldlog_radioC = 1;

 dlog_radioDint = Lresnumfovea;
 sprintf(dlog_radioDstr ,"focus on a residue by number");
 Ldlog_radioD = 1;

 sprintf(dlog_OKstr,"        OK        ");
 Ldlog_OK = 1;

 sprintf(dlog_cancelstr,"restart this Pass");
 Ldlog_cancel = 1;

    ACTIVE_DLOG = FOCUSOPTIONS_DLOG;
    Lwaiting = 1; /*wheel spinner while Dialog is active*/
}
/*___FocusOptionsDialog()____________________________________________________*/

/****EndFocusOptionsDialog()**************************************************/ 
void EndFocusOptionsDialog(void)
{ 
 if(Ldlog_OKHIT)
 {
    /* interpret various radio integers */
    if(dlog_radioAint == 1) /*No focus at all*/
    {
        Ldetails = 1;
        FOVEA = 0;
        Lfocusdistance = 0;
        Lconditions = 1;
        Lscriptin = 0;
        Lresnumfovea = 0; 
    }
    else if(dlog_radioBint == 1) /*type in x,y,z, for a focus point*/
    {
        Ldetails = 1;
        FOVEA = 1;
        Lfocusdistance = 1;
        Lconditions = 0;
        Lscriptin = 0;
        Lresnumfovea = 0; 
    }
    else if(dlog_radioCint == 1)/*get pre-existing file with x,y,z,focus point*/
    {
        Ldetails = 1;
        FOVEA = 1;
        Lfocusdistance = 1;
        Lconditions = 0;
        Lscriptin = 1;
        Lresnumfovea = 0; 
    }
    else if(dlog_radioDint == 1) /*focus on a residue by number*/
    {
        Ldetails = 1;
        FOVEA = 1;
        Lfocusdistance = 1;
        Lconditions = 0;
        Lscriptin = 0;
        Lresnumfovea = 1;
    }
    Lfocusoptionend = 1;
 }
 else if(Ldlog_cancelHIT)
 {
    Ldetails = 0;
    FOVEA = 0;
    Lfocusdistance = 0;
    Lconditions = 0;
    Lscriptin = 0;
    Lresnumfovea = 0; 
    Lrangestart = 1;
 }
 Lfocusoption = 0; /*clear flag*/
 ACTIVE_DLOG = 0;
 Lwaiting = 0; /*wheel spinner while Dialog active*/
}
/*___EndFocusOptionsDialog()_________________________________________________*/

/****DoFocusParamDialog*******************************************************/
void DoFocusParamDialog(void)
{
     if(!ACTIVE_DLOG)
     {
         cleargenericflags();     /*PKINDLOG.c*/
         FocusParamDialog();      /*PKINDLOG.c*/
         DogenericDialog();       /*PUXMDLOG.c*/
     }
}
/*___DoFocusParamDialog______________________________________________________*/

/****FocusParamDialog()*******************************************************/
void FocusParamDialog(void) /*FOCUSBYRESNUM_DLOG*/ /*FOCUSPARAM_DLOG*/
{
 sprintf(dlog_subjectstr,"Focus Point Values:");
 Ldlog_subject = 1;

 if(Lresnumfovea)  /*PC: FOCUSRESIDUE_DIALOG */
 {
    sprintf(dlog_paramAstr,"%d",resnumfovea);
    sprintf(dlog_textAstr,"focus around residue");
    Ldlog_paramA = 1;
    Ldlog_textA = 1;
 }
 else  /*PC: FOCUSPARAM_DIALOG */
 {
    sprintf(dlog_paramAstr,"%.2f",focus[1]);
    sprintf(dlog_textAstr,"x coord");
    Ldlog_paramA = 1;
    Ldlog_textA = 1;

    sprintf(dlog_paramBstr,"%.2f",focus[2]);
    sprintf(dlog_textBstr,"y coord");
    Ldlog_paramB = 1;
    Ldlog_textB = 1;

    sprintf(dlog_paramCstr,"%.2f",focus[3]);
    sprintf(dlog_textCstr,"z coord");
    Ldlog_paramC = 1;
    Ldlog_textC = 1;

 }
 sprintf(dlog_paramDstr,"%.2f",focusscdefault);
 sprintf( dlog_textDstr,"sidechain radius of focus selection");
        Ldlog_paramD = 1;
        Ldlog_textD = 1;

 sprintf(dlog_paramEstr,"%.2f",focusmcdefault);
 sprintf( dlog_textEstr,"mainchain radius of focus selection");
        Ldlog_paramE = 1;
        Ldlog_textE = 1;

 sprintf(dlog_paramFstr,"%.2f",focuscadefault);
 sprintf( dlog_textFstr,"Calpha-Calpha radius of focus selection");
        Ldlog_paramF = 1;
        Ldlog_textF = 1;

 sprintf(dlog_paramGstr,"%.2f",focuswadefault);
 sprintf( dlog_textGstr,"waters radius of focus selection");
        Ldlog_paramG = 1;
        Ldlog_textG = 1;

 sprintf(dlog_paramHstr,"%.2f",focushtdefault);
 sprintf( dlog_textHstr,"hetatoms radius of focus selection");
        Ldlog_paramH = 1;
        Ldlog_textH = 1;

 sprintf(dlog_OKstr,"  OK ");
 Ldlog_OK = 1;

 sprintf(dlog_optionstr,"cancel all focus & proceed");
 Ldlog_option = 1;

 sprintf(dlog_cancelstr,"restart this Pass");
 Ldlog_cancel = 1;

 if(Lresnumfovea) ACTIVE_DLOG = FOCUSBYRESNUM_DLOG;
 else             ACTIVE_DLOG = FOCUSPARAM_DLOG;
 Lwaiting = 1; /*wheel spinner while Dialog is active*/
}
/*___FocusParamDialog()______________________________________________________*/

/****EndFocusParamDialog()****************************************************/ 
void EndFocusParamDialog(void)
{ 
 if(Ldlog_OKHIT)
 {
    if(Lresnumfovea) /*and continues to be set for flow control*/
    {
       resnumfovea = intfromstr(dlog_paramAstr); /*PKINUTIL.c*/
    }
    else
    {
       focus[1] = floatfromstr(dlog_paramAstr); /*PKINUTIL.c*/
       focus[2] = floatfromstr(dlog_paramBstr); /*PKINUTIL.c*/
       focus[3] = floatfromstr(dlog_paramCstr); /*PKINUTIL.c*/
    }
    focussc  = floatfromstr(dlog_paramDstr); /*PKINUTIL.c*/
    focusmc  = floatfromstr(dlog_paramEstr); /*PKINUTIL.c*/
    focusca  = floatfromstr(dlog_paramFstr); /*PKINUTIL.c*/
    focuswa  = floatfromstr(dlog_paramGstr); /*PKINUTIL.c*/
    focusht  = floatfromstr(dlog_paramHstr); /*PKINUTIL.c*/
    
    FOVEA = 1; /*continues to be set for flow control*/
    Ldetails = 1;
    Lconditions = 1; /* go on */
 }
 else if(Ldlog_optionHIT) /*just forget all focus stuff*/
 {
    FOVEA = 0;
    Lresnumfovea = 0;
    Lscriptin = 0;
    
    Ldetails = 1;
    Lconditions = 1; /* go on */
 }
 else if(Ldlog_cancelHIT) /*restart this Pass*/
 {
    FOVEA = 0;
    Lresnumfovea = 0;
    Lscriptin = 0;

    Lrangestart = 1; /*restart dialog*/
    Ldetails = 0;
    Lconditions = 0;
 }
 Lfocusdistance = 0; /*clear flag that got us to this dialog */
 ACTIVE_DLOG = 0;
 Lwaiting = 0; /*wheel spinner while Dialog active*/
}
/*___EndFocusParamDialog()___________________________________________________*/

/*****DoAtomMarkerDialog******************************************************/
void DoAtomMarkerDialog(void)
{
     if(!ACTIVE_DLOG)
     {
         cleargenericflags();     /*PKINDLOG.c*/
         AtomMarkerDialog();      /*PKINDLOG.c*/
         DogenericDialog();       /*PUXMDLOG.c*/
     }
}
/*____DoAtomMarkerDialog_____________________________________________________*/

/****AtomMarkerDialog()*******************************************************/
void AtomMarkerDialog(void)    /*ATOMMARKER_DLOG*/
{
 sprintf(dlog_subjectstr,"Atom Marker choices:");
 Ldlog_subject = 1;

 sprintf(dlog_paramAstr,"%f",atmarker);
 sprintf(dlog_textAstr,"Current size of cross mark");
 Ldlog_paramA = 1;
 Ldlog_textA = 1;

 /*see KludgesDialog for VRML nstrnd and ballradius defaults*/

 sprintf(dlog_paramBstr,"%f",ballradius);
 sprintf(dlog_textBstr,"Current radius of ball");
 Ldlog_paramB = 1;
 Ldlog_textB = 1;

 /*sanity checking, with balls preferred*/
 if     (!Lcrosses && !Ldotsonly &&  Lballs){;}
 else if(!Lcrosses &&  Ldotsonly && !Lballs){;}
 else if( Lcrosses && !Ldotsonly && !Lballs){;}
 else if( Lcrosses &&  Ldotsonly && !Lballs){Lcrosses = 0;}
 else if( Lcrosses && !Ldotsonly &&  Lballs){Lcrosses = 0;}
 else if(!Lcrosses &&  Ldotsonly &&  Lballs){Ldotsonly = 0;}
 else if( Lcrosses &&  Ldotsonly &&  Lballs){Ldotsonly = 0;Lcrosses=0;}
 else if(!Lcrosses && !Ldotsonly && !Lballs){Lballs = 1;}

 dlog_radioAint = Lcrosses;
 sprintf(dlog_radioAstr ,"atom mark = cross, of size given above");
 Ldlog_radioA = 1;

 dlog_radioBint = Ldotsonly;
 sprintf(dlog_radioBstr ,"atom mark = dot ");
 Ldlog_radioB = 1;

 dlog_radioCint = Lballs;
 sprintf(dlog_radioCstr ,"atom mark = ball, of radius given above");
 Ldlog_radioC = 1;

 sprintf(dlog_OKstr,"  OK ");
 Ldlog_OK = 1;

 sprintf(dlog_cancelstr,"restart this Pass");
 Ldlog_cancel = 1;

    ACTIVE_DLOG = ATOMMARKER_DLOG;
    Lwaiting = 1; /*wheel spinner while Dialog is active*/
}
/*___AtomMarkerDialog()______________________________________________________*/

/****EndAtomMarkerDialog()****************************************************/ 
void EndAtomMarkerDialog(void)
{ 
 if(Ldlog_OKHIT)
 {
    /* interpret various radio integers */
    if     (dlog_radioAint == 1) /* atom mark = cross */
    {
        Lcrosses = 1;
        Ldotsonly = 0;
        Lballs = 0;
        atmarker = floatfromstr(dlog_paramAstr); /*PKINUTIL.c*/
    }
    else if(dlog_radioBint == 1)/* atom mark = dot */
    {
        Lcrosses = 0;
        Ldotsonly = 1;
        Lballs = 0;
    }
    else if(dlog_radioCint == 1) /* atom mark = radius */
    {
        Lcrosses = 0;
        Ldotsonly = 0;
        Lballs = 1;
        ballradius = floatfromstr(dlog_paramBstr); /*PKINUTIL.c*/
    }
    Ldetails = 1;
    Lconditions = 1; /* go on */
 }
 else if(Ldlog_cancelHIT)
 {
    Lrangestart = 1;
    Ldetails = 0;
    Lconditions = 0; 
 }
 Lgetatommarker = 0; /* clear flag */
 ACTIVE_DLOG = 0;
 Lwaiting = 0; /*wheel spinner while Dialog active*/
}
/*___EndAtomMarkerDialog()___________________________________________________*/

/*****DoRunConditionsDialog****************************************************/
void DoRunConditionsDialog(void)
{
     if(!ACTIVE_DLOG)
     {
         cleargenericflags();     /*PKINDLOG.c*/
         RunConditionsDialog();   /*PKINDLOG.c*/
         DogenericDialog();       /*PUXMDLOG.c*/
     }
}
/*____DoRunConditionsDialog___________________________________________________*/

/****RunConditionsDialog()*****************************************************/
void RunConditionsDialog(void)     /*RUNCONDITIONS_DLOG*/
{
 sprintf(dlog_subjectstr,"Subunit, or molecule, choice:");
 Ldlog_subject = 1;

 sprintf(dlog_paramAstr,"%d",beginsubunit);
 sprintf(dlog_textAstr,"Beginning subunit number: IF USE THESE LIMITS");
 Ldlog_paramA = 1;
 Ldlog_textA = 1;

 sprintf(dlog_paramBstr,"%d",endsubunit);
 sprintf(dlog_textBstr,"Ending subunit number:   ALSO CHECK RADIO BUTTON");
 Ldlog_paramB = 1;
 Ldlog_textB = 1;
 
 sprintf(dlog_paramCstr,"%s",codestr);
 sprintf(dlog_textCstr,"add this text to all pointIDs:   ALSO CHECK BOX below");
 Ldlog_paramC = 1;
 Ldlog_textC = 1;

 dlog_radioAint = Lneedsubunits;
 sprintf(dlog_radioAstr ,"Do subunits in range given above:  RADIO for LIMITS");
 Ldlog_radioA = 1;

 dlog_radioBint = Lfirstsubunit;
 sprintf(dlog_radioBstr ,"Do only first subunit of input file");
 Ldlog_radioB = 1;

 dlog_radioCint = Lallsubunits;
 sprintf(dlog_radioCstr ,"Do all subunits of input file");
 Ldlog_radioC = 1;

 dlog_checkAint = Lcodestr;
 sprintf(dlog_checkAstr ,"add text to all pointIDs");
 Ldlog_checkA = 1;

 sprintf(dlog_OKstr,"  OK ");
 Ldlog_OK = 1;

 sprintf(dlog_cancelstr,"restart this Pass");
 Ldlog_cancel = 1;

    ACTIVE_DLOG = RUNCONDITIONS_DLOG;
    Lwaiting = 1; /*wheel spinner while Dialog is active*/
}
/*___RunConditionsDialog()___________________________________________________*/

/****EndRunConditionsDialog()*************************************************/
void EndRunConditionsDialog(void)
{ 
 int j=0;

 if(Ldlog_OKHIT)
 {
    /* interpret various radio integers */
    if     (dlog_radioAint == 1) /* Do subunits in range given above */
    {
        Lallsubunits = 0;
        Lfirstsubunit = 0;
        Lneedsubunits = 1;
        beginsubunit = intfromstr(dlog_paramAstr); /*PKINUTIL.c*/
        endsubunit   = intfromstr(dlog_paramBstr); /*PKINUTIL.c*/
    }
    else if(dlog_radioBint == 1)/* Do only first subunit of input file */
    {
        Lallsubunits = 0;
        Lfirstsubunit = 1;
        Lneedsubunits = 0;
    }
    else if(dlog_radioCint == 1) /* Do all subunits of input file */
    {
        Lallsubunits = 1;
        Lfirstsubunit = 0;
        Lneedsubunits = 0;
    }
    if(dlog_checkAint == 1)
    {
        Lcodestr = 1;
        for(j=0; j<=MAXNAMECHAR; j++) 
        {
           codestr[j] = dlog_paramCstr[j];
           if(codestr[j] == '\0') {break;}
        }
        codestr[j] = '\0';
    }

    Lconditions = 0; /*just did this, turn off request*/
 }
 else if(Ldlog_cancelHIT)
 {/* "restart this Pass"*/
    Lconditions = 0; /*just did this, turn off request*/
    Lrangestart = 1;
    Ldetails = 0;
 }
 ACTIVE_DLOG = 0;
 Lwaiting = 0; /*wheel spinner while Dialog active*/
}
/*___EndRunConditionsDialog()_________________________________________________*/

/*****DoRibbonDialog******************************************************/
void DoRibbonDialog(void)
{
     if(!ACTIVE_DLOG)
     {
         cleargenericflags();     /*PKINDLOG.c*/
         RibbonDialog();          /*PKINDLOG.c*/
         DogenericDialog();       /*____DLOG.c*/
     }
}
/*____DoRibbonDialog_____________________________________________________*/

/****RibbonDialog()***********************************************************/
void RibbonDialog(void)   /*RIBBON_DLOG*/
{  /*RibbonControl ribboncontrol ribbondialog*/

  /*see KludgesDialog for VRML nstrnd and ballradius defaults*/
  if(LdumpVRML)
  {
     if(LarrowVRML)
     {
        Lcrosstie = 0; 
        Ledgedribbon = 1; 
        Lskeinedribbon = 0;
        nstrnd = 5;
     }
     else /*if(LsplineVRML)*/
     {
        Lcrosstie = 0; 
        Ledgedribbon = 0; 
        Lskeinedribbon = 1;
        nstrnd = 5;
     }
  }

  if(Lexplicitribbon)
  {
    sprintf(dlog_subjectstr,"Ribbon Control Values for explicit ranges"
    CRLF"alpha, beta, etc. have specified regions:"
    " hit OK for good defaults");
  }
  else
  {
    sprintf(dlog_subjectstr,"Ribbon Control Values for simple curves,"
    CRLF"alpha,beta,etc NOT explicit, no arrowheads,"
    " hit OK for good defaults");
  }
  Ldlog_subject = 1;

        /*defined ribbon default parameters*/
        /*nstrnd = 3;*/
        /*ribwidcoil = 1.0;*/
        /*ichord = 4;*/ /* number of vec/res */
        /*#define CORDER 4 */ /*order of the B-spline defined in PKINRIBN.C*/
        /*check in dialog box*/
        /* ' number of ribbon strands, width, & vec/res:'*/
        /* ' (return accepts:' ,nstrnd,ribwidcoil,ichord,'):'*/
 
  
   sprintf(dlog_paramAstr,"%.2f",ribwidcoil);     /*paramA coil width*/
   sprintf( dlog_textAstr,"Ribbon width in coil");
   Ldlog_paramA = 1;
   Ldlog_textA = 1;

   sprintf(dlog_paramBstr,"%.2f",ribwidalpha);/*paramB alpha width*/
   sprintf( dlog_textBstr,"Ribbon width in Alpha");
   Ldlog_paramB = 1;
   Ldlog_textB = 1;
   
   sprintf(dlog_paramCstr,"%.2f",ribwidbeta); /*paramC beta width*/
   sprintf( dlog_textCstr,"Ribbon width in Beta");
   Ldlog_paramC = 1;
   Ldlog_textC = 1;
   
   sprintf(dlog_paramDstr,"%.2f",ribwidnucleic); /*paramD beta width*/
   sprintf( dlog_textDstr,"Ribbon width in Nucleic acids");
   Ldlog_paramD = 1;
   Ldlog_textD = 1;

   sprintf(dlog_paramEstr,"%d",onestrandcoilwidth); /*paramE 1strand width*/
   sprintf( dlog_textEstr,"number of pixels width of single strand coil (1-7)");
   Ldlog_paramE = 1;
   Ldlog_textE = 1;

   sprintf(dlog_paramFstr,"%d",nstrnd); /*paramF number of strands*/
   sprintf( dlog_textFstr,"number of strands (1 - 9), use 2 for arrows,etc.");
   Ldlog_paramF = 1;
   Ldlog_textF = 1;

   sprintf(dlog_paramGstr,"%d",ichord); /*paramG chords/residue*/
   sprintf( dlog_textGstr
       ,"number of vectors/residue (1 - 10), always use 4, esp. for arrows");
   Ldlog_paramG = 1;
   Ldlog_textG = 1;

   sprintf(dlog_paramHstr,"%.2f", ribwidVRML); /*paramH VRML strand width*/
   sprintf( dlog_textHstr,"radius of a VRML strand =(this number)/(nstrand)");
   Ldlog_paramH = 1;
   Ldlog_textH = 1;

   dlog_checkAint = Lribonucleic; /*checkA Lribonucleic */
   sprintf(dlog_checkAstr,"if nucleic acid, RNA (A DNA) type instead of B DNA");
   Ldlog_checkA = 1;

   dlog_checkBint = Lfudgedwidth;/*checkB Lfudgedwidth */
   sprintf(dlog_checkBstr ,"ribbon width fudged by curvature, not by 2D type");
   Ldlog_checkB = 1;

   dlog_checkCint = Lonestrandcoil; /*checkC Lonestrandcoil */
   sprintf(dlog_checkCstr ,"single strand in coil regions");
   Ldlog_checkC = 1;

   dlog_checkDint = Lbetaarrows; /*checkD Lbetaarrows */
   sprintf(dlog_checkDstr ,"Arrowheads on Beta strands");
   Ldlog_checkD = 1;

   dlog_checkEint = Lsmoothtwist; /*checkE Lsmoothtwist */ /*010515*/
   sprintf(dlog_checkEstr ,"Smooth twist between adjacent residues");
   Ldlog_checkE = 1;

   dlog_checkFint = LcolorbyNtoC; /*checkF LcolorbyNtoC */ /*010530*/
   sprintf(dlog_checkFstr ,"spectral colors by relative N to C position");
   Ldlog_checkF = 1;

   /*Lcolorbycurvature control would go here if this were worth doing*/  
   
   dlog_checkGint = LNtermBetaFudge; /*checkG */ /*030706*/
   sprintf(dlog_checkGstr ,"Nterm fudged to be Beta if 1,2,3 straight");
   Ldlog_checkG = 1;

   dlog_checkHint = LNOfudge; /*checkH */ /*050327*/
   sprintf(dlog_checkHstr ,"NO curvature-fudged offsets for turns or width");
   Ldlog_checkH = 1;

   dlog_checkIint = Lribbonguidepts; /*checkI */ /*050327*/
   sprintf(dlog_checkIstr ,"ribbon guide points displayed");
   Ldlog_checkI = 1;

   dlog_checkJint = Lnucleicarrows; /*checkJ */ /*040228*/
   sprintf(dlog_checkJstr ,"beta-like arrow heads on nucleic ribbons");
   Ldlog_checkJ = 1;

   dlog_radioAint = Lcrosstie;  /*radioA Lcrosstie==flat-faced ribbon */
   sprintf(dlog_radioAstr ,"Flat faced ribbon (crossties)");
   Ldlog_radioA = 1;

   dlog_radioBint = Ledgedribbon;  /*radioB Ledgedribbon==edged,flat-faced*/
   sprintf(dlog_radioBstr ,"Edged, flat faced ribbon (instead of skeins)");
   Ldlog_radioB = 1;

   dlog_radioCint = Lskeinedribbon;  /*radioC Lskeinedribbon*/
   sprintf(dlog_radioCstr ,"Ribbon of long skeins");
   Ldlog_radioC = 1;

   sprintf(dlog_OKstr,"OK"); 
   Ldlog_OK = 1; /*Ldlog_OK*/

   sprintf(dlog_optionstr,"cancel ribbon work");
   Ldlog_option = 1; /*Ldlog_option: cancel ribbon work*/

   sprintf(dlog_cancelstr,"restart this Pass"); 
   Ldlog_cancel = 1; /*Ldlog_cancel: restart this Pass*/

   ACTIVE_DLOG = RIBBON_DLOG;
   Lwaiting = 1; /*wheel spinner while Dialog is active*/
}
/*___RibbonDialog()__________________________________________________________*/


/****EndRibbonDialog()********************************************************/ 
void EndRibbonDialog(void)
{ 
 int ntry;

 if(Ldlog_OKHIT)
 {/*Ldlog_OK*/
    ribwidcoil    = floatfromstr(dlog_paramAstr);/*paramA coil width*/
    ribwidalpha   = floatfromstr(dlog_paramBstr);/*paramB alpha width  */
    ribwidbeta    = floatfromstr(dlog_paramCstr);/*paramC beta width   */
    ribwidnucleic = floatfromstr(dlog_paramDstr);/*paramD nucleic width*/
    
    ntry = intfromstr(dlog_paramEstr);/*paramE 1strand width*/
       if(ntry < 1) onestrandcoilwidth = 1;
       else if(ntry > 7) onestrandcoilwidth = 7;
       else onestrandcoilwidth = ntry;     

    ntry = intfromstr(dlog_paramFstr);/*paramF number of strands*/
       if(ntry > 9 )     nstrnd = 9;
       else if(ntry < 1) nstrnd = 1;
       else              nstrnd = ntry;

    ntry = intfromstr(dlog_paramGstr);/*paramG chords/residue*/
       if(ntry > 10)     ichord = 10;
       else if(ntry < 1) ichord = 1;
       else              ichord = ntry;

    ribwidVRML = floatfromstr(dlog_paramHstr);/*paramH VRML radius of strand*/

    /*Lnucleicacid now set by atoms encountered in file*/
    Lribonucleic = dlog_checkAint; /*checkA Lribonucleic */
    Lfudgedwidth = dlog_checkBint; /*checkB Lfudgedwidth */
    Lonestrandcoil = dlog_checkCint; /*checkC Lonestrandcoil */
    Lbetaarrows = dlog_checkDint; /*checkD Lbetaarrows */
    Lsmoothtwist =dlog_checkEint;/*smooth out twist between adjacent residues*/
    LcolorbyNtoC =dlog_checkFint;/*spectral colors by relative N to C position*/
    LNtermBetaFudge = dlog_checkGint;/*straight Nterm fudged to Beta 030706*/
    LNOfudge = dlog_checkHint; /*050327 NO curvature-fudged offsets*/
    Lribbonguidepts = dlog_checkIint; /*050327 ribbon guide points displayed*/
    Lnucleicarrows =dlog_checkJint;/*beta-like arrow heads on nucleic ribbons*/

    Lcrosstie      = dlog_radioAint; /*radioA Lcrosstie==flat-faced ribbon */
    Ledgedribbon   = dlog_radioBint; /*radioB Ledgedribbon==edged,flat-faced*/
    Lskeinedribbon = dlog_radioCint; /*radioC Lskeinedribbon*/

    if(LdumpVRML && Ledgedribbon){LarrowVRML=1; LsplineVRML=0;} /*050212*/

    Lribbon = 1; /*this is used later in PKINCRTL*/
    Lgetribbonparam = 0; /*just did this, now turn off request*/
    Ldetails = 1;
    Lconditions = 1; 
 }
 else if(Ldlog_optionHIT) 
 {/*Ldlog_option: cancel ribbon work*/
    Lribbon = 0;
    Lgetribbonparam = 0; /*just did this, now turn off request*/
    Ldetails = 1;
    Lconditions = 1; 
 }
 else if(Ldlog_cancelHIT)
 {/*Ldlog_cancel: restart this Pass*/
    Lribbon = 0;
    Lgetribbonparam = 0; /*just did this, now turn off request*/
    Lrangestart = 1;
    Ldetails = 0;
    Lconditions = 0; 
 }

 ACTIVE_DLOG = 0;
 Lwaiting = 0; /*wheel spinner while Dialog active*/
}
/*___EndRibbonDialog()___________________________________________________*/

/*****DoColorScaleDialog()****************************************************/
void DoColorScaleDialog(void)
{
     if(!ACTIVE_DLOG)
     {
         cleargenericflags();     /*PKINDLOG.c*/
         ColorScaleDialog();      /*PKINDLOG.c*/
         DogenericDialog();       /*PUXMDLOG.c*/
     }
}
/*____DoColorScaleDialog()___________________________________________________*/

/****ColorScaleDialog()*******************************************************/
void ColorScaleDialog(void)  /*COLORSCALE_DLOG*/
{
 sprintf(dlog_subjectstr,"Color Scale: values based on PDB field:"
CRLF"occupancy 55...60, temp factor 61...66, Unusual 67...72");

   Ldlog_subject = 1;

   sprintf(dlog_paramAstr,"%.2f",colorscale[1].value);
   sprintf(dlog_textAstr,"%s below this number",colorscale[1].color);
   Ldlog_paramA = 1;
   Ldlog_textA = 1;

   sprintf(dlog_paramBstr,"%.2f",colorscale[2].value);
   sprintf(dlog_textBstr,"%s",colorscale[2].color);
   Ldlog_paramB = 1;
   Ldlog_textB = 1;

   sprintf(dlog_paramCstr,"%.2f",colorscale[3].value);
   sprintf(dlog_textCstr,"%s",colorscale[3].color);
   Ldlog_paramC = 1;
   Ldlog_textC = 1;

   sprintf(dlog_paramDstr,"%.2f",colorscale[4].value);
   sprintf( dlog_textDstr,"%s",colorscale[4].color);
        Ldlog_paramD = 1;
        Ldlog_textD = 1;

   sprintf(dlog_paramEstr,"%.2f",colorscale[5].value);
   sprintf( dlog_textEstr,"%s",colorscale[5].color);
        Ldlog_paramE = 1;
        Ldlog_textE = 1;

   sprintf(dlog_paramFstr,"%.2f",colorscale[6].value);
   sprintf( dlog_textFstr,"%s",colorscale[6].color);
        Ldlog_paramF = 1;
        Ldlog_textF = 1;

   sprintf(dlog_paramGstr,"%.2f",colorscale[7].value);
   sprintf( dlog_textGstr,"%s",colorscale[7].color);
        Ldlog_paramG = 1;
        Ldlog_textG = 1;

   sprintf(dlog_paramHstr,"%.2f",colorscale[8].value);
   sprintf( dlog_textHstr,"%s",colorscale[8].color);
        Ldlog_paramH = 1;
        Ldlog_textH = 1;
        
   sprintf(dlog_paramIstr,"%.2f",colorscale[9].value);
   sprintf( dlog_textIstr
      ,"%s below,  %s above",colorscale[9].color,colorscale[10].color);
        Ldlog_paramI = 1;
        Ldlog_textI = 1;
 
 if(Ncolorscalebasis == 1) dlog_radioAint = 1;
 else dlog_radioAint = 0; 
 sprintf(dlog_radioAstr ,"B-value: PDB col 61...66");
 Ldlog_radioA = 1;

 if(Ncolorscalebasis == 2) dlog_radioBint = 1;
 else dlog_radioBint = 0; 
 sprintf(dlog_radioBstr ,"Occupancy: PDB col 55...60");
 Ldlog_radioB = 1;

 if(Ncolorscalebasis == 3) dlog_radioCint = 1;
 else dlog_radioCint = 0; 
 sprintf(dlog_radioCstr ,"Unusual: PDB col 67...72");
 Ldlog_radioC = 1;

   sprintf(dlog_OKstr,"  OK ");
   Ldlog_OK = 1;

   sprintf(dlog_optionstr,"original color scale");
   Ldlog_option = 1;

   sprintf(dlog_cancelstr,"cancel & proceed");
   Ldlog_cancel = 1;

   ACTIVE_DLOG = COLORSCALE_DLOG;
   Lwaiting = 1; /*wheel spinner while Dialog is active*/
}
/*___ColorScaleDialog()______________________________________________________*/

/****EndColorScaleDialog()****************************************************/ 
void EndColorScaleDialog(void)
{ 
 if(Ldlog_OKHIT)
 {
    colorscale[1].value = floatfromstr(dlog_paramAstr); /*PKINUTIL.c*/
    colorscale[2].value = floatfromstr(dlog_paramBstr); /*PKINUTIL.c*/
    colorscale[3].value = floatfromstr(dlog_paramCstr); /*PKINUTIL.c*/
    colorscale[4].value = floatfromstr(dlog_paramDstr); /*PKINUTIL.c*/
    colorscale[5].value = floatfromstr(dlog_paramEstr); /*PKINUTIL.c*/
    colorscale[6].value = floatfromstr(dlog_paramFstr); /*PKINUTIL.c*/
    colorscale[7].value = floatfromstr(dlog_paramGstr); /*PKINUTIL.c*/
    colorscale[8].value = floatfromstr(dlog_paramHstr); /*PKINUTIL.c*/
    colorscale[9].value = floatfromstr(dlog_paramIstr); /*PKINUTIL.c*/
    if     (dlog_radioCint ==1) Ncolorscalebasis = 3;
    else if(dlog_radioBint ==1) Ncolorscalebasis = 2;
    else                        Ncolorscalebasis = 1;

 }
 else if(Ldlog_optionHIT) /*restore original values*/
 {
    colorscale[1].value = colorscale[1].origvalue;
    colorscale[2].value = colorscale[2].origvalue;
    colorscale[3].value = colorscale[3].origvalue;
    colorscale[4].value = colorscale[4].origvalue;
    colorscale[5].value = colorscale[5].origvalue;
    colorscale[6].value = colorscale[6].origvalue;
    colorscale[7].value = colorscale[7].origvalue;
    colorscale[8].value = colorscale[8].origvalue;
    colorscale[9].value = colorscale[9].origvalue;
    Ncolorscalebasis = 1; /*B value*/
 }
 else if(Ldlog_cancelHIT) /*Cancel*/
 {
    ; 
 }
 Lcolorscale = 0; /*clear flag that got us to this dialog */
 ACTIVE_DLOG = 0;
 Lwaiting = 0; /*wheel spinner while Dialog active*/
}
/*___EndColorScaleDialog()___________________________________________________*/

