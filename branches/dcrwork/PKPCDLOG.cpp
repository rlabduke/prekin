#include "PKIN.h"
#include "PKINMENU.h"
#include "PKINDLOG.h"

/****DogenericDialog()*******************************************************/
void    DogenericDialog()
{
   switch(ACTIVE_DLOG)
   {
      case  INIT_DLOG:
         DialogBox(hInst,"INITIAL_DIALOG",PrekinWindow,genericDialog);
         break;
      case  REPORT_DLOG:
         DialogBox(hInst,"ALERT_DIALOG",PrekinWindow,genericDialog);
         break;
      case  RANGESTART_DLOG:
         DialogBox(hInst,"RANGESTART_DIALOG",PrekinWindow,genericDialog);
         break;
      case  BUILTINSCRIPTS_DLOG:
         DialogBox(hInst,"BUILT-IN_SCRIPTS_DIALOG",PrekinWindow,genericDialog);
         break;
      case  EXTERNALSCRIPT_DLOG:
         DialogBox(hInst,"EXT_SCRIPTS_DIALOG",PrekinWindow,genericDialog);
         break;
      case  KLUDGES_DLOG:
         DialogBox(hInst,"KLUDGES_DIALOG",PrekinWindow,genericDialog);
         break; 
      case  RANGECONTROL_DLOG:
         DialogBox(hInst,"RANGE_CONTROL_DIALOG",PrekinWindow,genericDialog);
         break;
      case  FOCUSOPTIONS_DLOG:
         DialogBox(hInst,"FOCUSOPTIONS_DIALOG",PrekinWindow,genericDialog);
         break;
      case  FOCUSPARAM_DLOG:
         DialogBox(hInst,"FOCUSPARAM_DIALOG",PrekinWindow,genericDialog);
         break;
      case  FOCUSBYRESNUM_DLOG:
         DialogBox(hInst,"FOCUSRESIDUE_DIALOG",PrekinWindow,genericDialog);
         break;
      case  ATOMMARKER_DLOG:
         DialogBox(hInst,"ATOMMARKER_DIALOG",PrekinWindow,genericDialog);
         break; 
      case  RUNCONDITIONS_DLOG:
         DialogBox(hInst,"RUNCONDITIONS_DIALOG",PrekinWindow,genericDialog);
         break; 
      case  RIBBON_DLOG:
         DialogBox(hInst,"RIBBON_DIALOG",PrekinWindow,genericDialog);
         break;
      case  COLORSCALE_DLOG:
         DialogBox(hInst,"COLORSCALE_DIALOG",PrekinWindow,genericDialog);
         break;

   }
   genericdlogrouter();
}
/*___DogenericDialog()______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****genericDialog()*********************************************************/
BOOL CALLBACK genericDialog(HWND hDlg, UINT message, WPARAM wParam,LPARAM lParam)
{
 /*  PC repeatedly calls this routine while dialog being processed*/
 /*use logicals to control which features to set, thus derivative */
 /*dialog boxes with fewer features can be directly used by this routine*/ 
 int ireturn;
 char c;

 ireturn = FALSE;
 switch (message)
 {/*switch (message)*/
    case WM_INITDIALOG:
        /* load initial values in dialog box */
        /*all DLOG have OK and CANCEL buttons, */
        /*  titles have charstr set to defaults*/
        c = ' ';

      SetDlgItemText(hDlg,DLOG_OK,(LPSTR)dlog_OKstr);
        SetDlgItemText(hDlg,DLOG_CANCEL,(LPSTR)dlog_cancelstr);
        
      if(Ldlog_option)
            SetDlgItemText(hDlg,DLOG_OPTION,(LPSTR)dlog_optionstr);
        
        if(Ldlog_subject)
            SetDlgItemText(hDlg,DLOG_SUBJECT,(LPSTR)dlog_subjectstr);

        if(Ldlog_paramA) SetDlgItemText(hDlg,DLOG_PARAMA,(LPSTR)dlog_paramAstr);
        if(Ldlog_paramB) SetDlgItemText(hDlg,DLOG_PARAMB,(LPSTR)dlog_paramBstr);
        if(Ldlog_paramC) SetDlgItemText(hDlg,DLOG_PARAMC,(LPSTR)dlog_paramCstr);
        if(Ldlog_paramD) SetDlgItemText(hDlg,DLOG_PARAMD,(LPSTR)dlog_paramDstr);
        if(Ldlog_paramE) SetDlgItemText(hDlg,DLOG_PARAME,(LPSTR)dlog_paramEstr);
        if(Ldlog_paramF) SetDlgItemText(hDlg,DLOG_PARAMF,(LPSTR)dlog_paramFstr);
        if(Ldlog_paramG) SetDlgItemText(hDlg,DLOG_PARAMG,(LPSTR)dlog_paramGstr);
        if(Ldlog_paramH) SetDlgItemText(hDlg,DLOG_PARAMH,(LPSTR)dlog_paramHstr);
        if(Ldlog_paramI) SetDlgItemText(hDlg,DLOG_PARAMI,(LPSTR)dlog_paramIstr);

        if(Ldlog_textA) SetDlgItemText(hDlg,DLOG_TEXTA,(LPSTR)dlog_textAstr);
        if(Ldlog_textB) SetDlgItemText(hDlg,DLOG_TEXTB,(LPSTR)dlog_textBstr);
        if(Ldlog_textC) SetDlgItemText(hDlg,DLOG_TEXTC,(LPSTR)dlog_textCstr);
      if(Ldlog_textD) SetDlgItemText(hDlg,DLOG_TEXTD,(LPSTR)dlog_textDstr);
        if(Ldlog_textE) SetDlgItemText(hDlg,DLOG_TEXTE,(LPSTR)dlog_textEstr);
        if(Ldlog_textF) SetDlgItemText(hDlg,DLOG_TEXTF,(LPSTR)dlog_textFstr);
      if(Ldlog_textG) SetDlgItemText(hDlg,DLOG_TEXTG,(LPSTR)dlog_textGstr);
        if(Ldlog_textH) SetDlgItemText(hDlg,DLOG_TEXTH,(LPSTR)dlog_textHstr);
        if(Ldlog_textI) SetDlgItemText(hDlg,DLOG_TEXTI,(LPSTR)dlog_textIstr);

      if(Ldlog_check_caption)
         SetDlgItemText(hDlg,DLOG_CHECK_CAPTION,(LPSTR)dlog_checkcaptionstr);
      if(Ldlog_radio_caption)
         SetDlgItemText(hDlg,DLOG_RADIO_CAPTION,(LPSTR)dlog_radiocaptionstr);
      if(Ldlog_checkLast_caption)
         SetDlgItemText(hDlg,DLOG_CHECKLAST_CAPTION,(LPSTR)dlog_checkLastcaptionstr);
/* check boxes */
        if(Ldlog_checkA)
      {
         CheckDlgButton(hDlg,DLOG_CHECKA,dlog_checkAint);
         SetDlgItemText(hDlg,DLOG_CHECKA,(LPSTR)dlog_checkAstr);
      }
        if(Ldlog_checkB)
      {
         CheckDlgButton(hDlg,DLOG_CHECKB,dlog_checkBint);
         SetDlgItemText(hDlg,DLOG_CHECKB,(LPSTR)dlog_checkBstr);
      }
        if(Ldlog_checkC)
      {
         CheckDlgButton(hDlg,DLOG_CHECKC,dlog_checkCint);
         SetDlgItemText(hDlg,DLOG_CHECKC,(LPSTR)dlog_checkCstr);
      }
      if(Ldlog_checkD)
      {
         CheckDlgButton(hDlg,DLOG_CHECKD,dlog_checkDint);
         SetDlgItemText(hDlg,DLOG_CHECKD,(LPSTR)dlog_checkDstr);
      }
        if(Ldlog_checkE)
      {
         CheckDlgButton(hDlg,DLOG_CHECKE,dlog_checkEint);
         SetDlgItemText(hDlg,DLOG_CHECKE,(LPSTR)dlog_checkEstr);
      }
        if(Ldlog_checkF)
      {
         CheckDlgButton(hDlg,DLOG_CHECKF,dlog_checkFint);
         SetDlgItemText(hDlg,DLOG_CHECKF,(LPSTR)dlog_checkFstr);
      }
      if(Ldlog_checkG)
      {
         CheckDlgButton(hDlg,DLOG_CHECKG,dlog_checkGint);
         SetDlgItemText(hDlg,DLOG_CHECKG,(LPSTR)dlog_checkGstr);
      }
        if(Ldlog_checkH)
      {
         CheckDlgButton(hDlg,DLOG_CHECKH,dlog_checkHint);
         SetDlgItemText(hDlg,DLOG_CHECKH,(LPSTR)dlog_checkHstr);
      }
        if(Ldlog_checkI)
      {
         CheckDlgButton(hDlg,DLOG_CHECKI,dlog_checkIint);
         SetDlgItemText(hDlg,DLOG_CHECKI,(LPSTR)dlog_checkIstr);
      }
      if(Ldlog_checkJ)
      {
         CheckDlgButton(hDlg,DLOG_CHECKJ,dlog_checkJint);
         SetDlgItemText(hDlg,DLOG_CHECKJ,(LPSTR)dlog_checkJstr);
      }
        if(Ldlog_checkK)
      {
         CheckDlgButton(hDlg,DLOG_CHECKK,dlog_checkKint);
         SetDlgItemText(hDlg,DLOG_CHECKK,(LPSTR)dlog_checkKstr);
      }
        if(Ldlog_checkL)
      {
         CheckDlgButton(hDlg,DLOG_CHECKL,dlog_checkLint);
         SetDlgItemText(hDlg,DLOG_CHECKL,(LPSTR)dlog_checkLstr);
      }
      if(Ldlog_checkM)
      {
         CheckDlgButton(hDlg,DLOG_CHECKM,dlog_checkMint);
         SetDlgItemText(hDlg,DLOG_CHECKM,(LPSTR)dlog_checkMstr);
      }
        if(Ldlog_checkN)
      {
         CheckDlgButton(hDlg,DLOG_CHECKN,dlog_checkNint);
         SetDlgItemText(hDlg,DLOG_CHECKN,(LPSTR)dlog_checkNstr);
      }
        if(Ldlog_checkO)
      {
         CheckDlgButton(hDlg,DLOG_CHECKO,dlog_checkOint);
         SetDlgItemText(hDlg,DLOG_CHECKO,(LPSTR)dlog_checkOstr);
      }
      if(Ldlog_checkP)
      {
         CheckDlgButton(hDlg,DLOG_CHECKP,dlog_checkPint);
         SetDlgItemText(hDlg,DLOG_CHECKP,(LPSTR)dlog_checkPstr);
      }
        if(Ldlog_checkQ)
      {
         CheckDlgButton(hDlg,DLOG_CHECKQ,dlog_checkQint);
         SetDlgItemText(hDlg,DLOG_CHECKQ,(LPSTR)dlog_checkQstr);
      }
      if(Ldlog_checkR)
      {
         CheckDlgButton(hDlg,DLOG_CHECKR,dlog_checkRint);
         SetDlgItemText(hDlg,DLOG_CHECKR,(LPSTR)dlog_checkRstr);
      }
      if(Ldlog_checkS)
      {
         CheckDlgButton(hDlg,DLOG_CHECKS,dlog_checkSint);
         SetDlgItemText(hDlg,DLOG_CHECKS,(LPSTR)dlog_checkSstr);
      }
      if(Ldlog_checkT)
      {
         CheckDlgButton(hDlg,DLOG_CHECKT,dlog_checkTint);
         SetDlgItemText(hDlg,DLOG_CHECKT,(LPSTR)dlog_checkTstr);
      }
      if(Ldlog_checkU)
      {
         CheckDlgButton(hDlg,DLOG_CHECKU,dlog_checkUint);
         SetDlgItemText(hDlg,DLOG_CHECKU,(LPSTR)dlog_checkUstr);
      }
/* radio buttons */
        if(Ldlog_radioA)
      {
         CheckDlgButton(hDlg,DLOG_RADIOA,dlog_radioAint);
         SetDlgItemText(hDlg,DLOG_RADIOA,(LPSTR)dlog_radioAstr);
      }
        if(Ldlog_radioB)
      {
         CheckDlgButton(hDlg,DLOG_RADIOB,dlog_radioBint);
         SetDlgItemText(hDlg,DLOG_RADIOB,(LPSTR)dlog_radioBstr);
      }
        if(Ldlog_radioC)
      {
         CheckDlgButton(hDlg,DLOG_RADIOC,dlog_radioCint);
         SetDlgItemText(hDlg,DLOG_RADIOC,(LPSTR)dlog_radioCstr);
      }
      if(Ldlog_radioD)
      {
         CheckDlgButton(hDlg,DLOG_RADIOD,dlog_radioDint);
         SetDlgItemText(hDlg,DLOG_RADIOD,(LPSTR)dlog_radioDstr);
      }
        if(Ldlog_radioE)
      {
         CheckDlgButton(hDlg,DLOG_RADIOE,dlog_radioEint);
         SetDlgItemText(hDlg,DLOG_RADIOE,(LPSTR)dlog_radioEstr);
      }
        if(Ldlog_radioF)
      {
         CheckDlgButton(hDlg,DLOG_RADIOF,dlog_radioFint);
         SetDlgItemText(hDlg,DLOG_RADIOF,(LPSTR)dlog_radioFstr);
      }
      if(Ldlog_radioG)
      {
         CheckDlgButton(hDlg,DLOG_RADIOG,dlog_radioGint);
         SetDlgItemText(hDlg,DLOG_RADIOG,(LPSTR)dlog_radioGstr);
      }
        if(Ldlog_radioH)
      {
         CheckDlgButton(hDlg,DLOG_RADIOH,dlog_radioHint);
         SetDlgItemText(hDlg,DLOG_RADIOH,(LPSTR)dlog_radioHstr);
      }
      if(Ldlog_radioI)
      {
         CheckDlgButton(hDlg,DLOG_RADIOI,dlog_radioIint);
         SetDlgItemText(hDlg,DLOG_RADIOI,(LPSTR)dlog_radioIstr);
      }
      if(Ldlog_radioJ)
      {
         CheckDlgButton(hDlg,DLOG_RADIOJ,dlog_radioJint);
         SetDlgItemText(hDlg,DLOG_RADIOJ,(LPSTR)dlog_radioJstr);
      }
      if(Ldlog_radioK)
      {
         CheckDlgButton(hDlg,DLOG_RADIOK,dlog_radioKint);
         SetDlgItemText(hDlg,DLOG_RADIOK,(LPSTR)dlog_radioKstr);
      }
      if(Ldlog_radioL)
      {
         CheckDlgButton(hDlg,DLOG_RADIOL,dlog_radioLint);
         SetDlgItemText(hDlg,DLOG_RADIOL,(LPSTR)dlog_radioLstr);
      }
      if(Ldlog_radioM)
      {
         CheckDlgButton(hDlg,DLOG_RADIOM,dlog_radioMint);
         SetDlgItemText(hDlg,DLOG_RADIOM,(LPSTR)dlog_radioMstr);
      }
      if(Ldlog_radioN)
      {
         CheckDlgButton(hDlg,DLOG_RADION,dlog_radioNint);
         SetDlgItemText(hDlg,DLOG_RADION,(LPSTR)dlog_radioNstr);
      }
      if(Ldlog_radioO)
      {
         CheckDlgButton(hDlg,DLOG_RADIOO,dlog_radioOint);
         SetDlgItemText(hDlg,DLOG_RADIOO,(LPSTR)dlog_radioOstr);
      }
      if(Ldlog_radioP)
      {
         CheckDlgButton(hDlg,DLOG_RADIOP,dlog_radioPint);
         SetDlgItemText(hDlg,DLOG_RADIOP,(LPSTR)dlog_radioPstr);
      }

      if(Ldlog_info)   SetDlgItemText(hDlg,DLOG_INFO,(LPSTR)dlog_infostr);

      ireturn=(TRUE); /*stay under DLOG control*/
   break;
   case WM_COMMAND:
      switch (LOWORD(wParam))
      {/*switch (LOWORD(wParam))*/
         case DLOG_OK: /* end dialog, using current values*/
            Ldlog_OKHIT = 1; /*do standard thing */
            Ldlog_cancelHIT = 0;
            Ldlog_optionHIT = 0;
            Lcancel = 0;
            EndDialog(hDlg,0);
            ireturn=(FALSE);
         break;
         case DLOG_CANCEL: /* end dialog, do not change values */
            Ldlog_cancelHIT = 1; /* take no action */
            Ldlog_OKHIT = 0;
            Ldlog_optionHIT = 0;
            Lcancel = 1;
            EndDialog(hDlg,0);
            ireturn=(FALSE);
         break;
         case DLOG_OPTION: /* end dialog, do optional thing */
            if(Ldlog_option)
            {
               Ldlog_optionHIT = 1;
               Ldlog_cancelHIT = 0;
               Ldlog_OKHIT = 0;
               Lcancel = 0;
               EndDialog(hDlg,0);
               ireturn=(FALSE);
            }
            else ireturn=(FALSE);
         break;
         case DLOG_INFO:
            if (Ldlog_info)
               GetDlgItemText(hDlg,DLOG_INFO,dlog_infostr
                                 ,sizeof(dlog_infostr));
         break;

/* DLOG PARAMS */
         case DLOG_PARAMA: /* get value as text, do not translate */
            if(Ldlog_paramA)
               GetDlgItemText(hDlg,DLOG_PARAMA,dlog_paramAstr
                                 ,sizeof(dlog_paramAstr));
         break;
         case DLOG_PARAMB: /* get value as text, do not translate */
            if(Ldlog_paramB)
               GetDlgItemText(hDlg,DLOG_PARAMB,dlog_paramBstr
                                 ,sizeof(dlog_paramBstr));
         break;
         case DLOG_PARAMC: /* get value as text, do not translate */
            if(Ldlog_paramC)
               GetDlgItemText(hDlg,DLOG_PARAMC,dlog_paramCstr
                                 ,sizeof(dlog_paramCstr));
         break;
         case DLOG_PARAMD: /* get value as text, do not translate */
            if(Ldlog_paramD)
               GetDlgItemText(hDlg,DLOG_PARAMD,dlog_paramDstr
                                 ,sizeof(dlog_paramDstr));
         break;
         case DLOG_PARAME: /* get value as text, do not translate */
            if(Ldlog_paramE)
               GetDlgItemText(hDlg,DLOG_PARAME,dlog_paramEstr
                                 ,sizeof(dlog_paramEstr));
         break;
         case DLOG_PARAMF: /* get value as text, do not translate */
            if(Ldlog_paramF)
               GetDlgItemText(hDlg,DLOG_PARAMF,dlog_paramFstr
                                 ,sizeof(dlog_paramFstr));
         break;
         case DLOG_PARAMG: /* get value as text, do not translate */
            if(Ldlog_paramG)
               GetDlgItemText(hDlg,DLOG_PARAMG,dlog_paramGstr
                                 ,sizeof(dlog_paramGstr));
         break;
         case DLOG_PARAMH: /* get value as text, do not translate */
            if(Ldlog_paramH)
               GetDlgItemText(hDlg,DLOG_PARAMH,dlog_paramHstr
                                 ,sizeof(dlog_paramHstr));
         break;
         case DLOG_PARAMI: /* get value as text, do not translate */
            if(Ldlog_paramI)
               GetDlgItemText(hDlg,DLOG_PARAMI,dlog_paramIstr
                                 ,sizeof(dlog_paramIstr));
         break;

/* DLOG CHECKS */
         case DLOG_CHECKA: /* toggle value */
            if(Ldlog_checkA)
               CheckDlgButton(hDlg,DLOG_CHECKA,dlog_checkAint
                                      =!dlog_checkAint);
         break;
         case DLOG_CHECKB: /* toggle value */
            if(Ldlog_checkB)
               CheckDlgButton(hDlg,DLOG_CHECKB,dlog_checkBint
                                      =!dlog_checkBint);
         break;
         case DLOG_CHECKC: /* toggle value */
            if(Ldlog_checkC)
               CheckDlgButton(hDlg,DLOG_CHECKC,dlog_checkCint
                                      =!dlog_checkCint);
         break;
         case DLOG_CHECKD: /* toggle value */
            if(Ldlog_checkD)
               CheckDlgButton(hDlg,DLOG_CHECKD,dlog_checkDint
                                      =!dlog_checkDint);
         break;
         case DLOG_CHECKE: /* toggle value */
            if(Ldlog_checkE)
               CheckDlgButton(hDlg,DLOG_CHECKE,dlog_checkEint
                                      =!dlog_checkEint);
         break;
         case DLOG_CHECKF: /* toggle value */
            if(Ldlog_checkF)
               CheckDlgButton(hDlg,DLOG_CHECKF,dlog_checkFint
                                      =!dlog_checkFint);
         break;
         case DLOG_CHECKG: /* toggle value */
            if(Ldlog_checkG)
               CheckDlgButton(hDlg,DLOG_CHECKG,dlog_checkGint
                                      =!dlog_checkGint);
         break;
         case DLOG_CHECKH: /* toggle value */
            if(Ldlog_checkH)
               CheckDlgButton(hDlg,DLOG_CHECKH,dlog_checkHint
                                      =!dlog_checkHint);
         break;
         case DLOG_CHECKI: /* toggle value */
            if(Ldlog_checkI)
               CheckDlgButton(hDlg,DLOG_CHECKI,dlog_checkIint
                                      =!dlog_checkIint);
         break;
         case DLOG_CHECKJ: /* toggle value */
            if(Ldlog_checkJ)
               CheckDlgButton(hDlg,DLOG_CHECKJ,dlog_checkJint
                                      =!dlog_checkJint);
         break;
         case DLOG_CHECKK: /* toggle value */
            if(Ldlog_checkK)
               CheckDlgButton(hDlg,DLOG_CHECKK,dlog_checkKint
                                      =!dlog_checkKint);
         break;
         case DLOG_CHECKL: /* toggle value */
            if(Ldlog_checkL)
               CheckDlgButton(hDlg,DLOG_CHECKL,dlog_checkLint
                                      =!dlog_checkLint);
         break;
         case DLOG_CHECKM: /* toggle value */
            if(Ldlog_checkM)
               CheckDlgButton(hDlg,DLOG_CHECKM,dlog_checkMint
                                      =!dlog_checkMint);
         break;
         case DLOG_CHECKN: /* toggle value */
            if(Ldlog_checkN)
               CheckDlgButton(hDlg,DLOG_CHECKN,dlog_checkNint
                                      =!dlog_checkNint);
         break;
         case DLOG_CHECKO: /* toggle value */
            if(Ldlog_checkO)
               CheckDlgButton(hDlg,DLOG_CHECKO,dlog_checkOint
                                      =!dlog_checkOint);
         break;
         case DLOG_CHECKP: /* toggle value */
            if(Ldlog_checkP)
               CheckDlgButton(hDlg,DLOG_CHECKP,dlog_checkPint
                                      =!dlog_checkPint);
         break;
         case DLOG_CHECKQ: /* toggle value */
            if(Ldlog_checkQ)
               CheckDlgButton(hDlg,DLOG_CHECKQ,dlog_checkQint
                                      =!dlog_checkQint);
         break;
         case DLOG_CHECKR: /* toggle value */
            if(Ldlog_checkR)
               CheckDlgButton(hDlg,DLOG_CHECKR,dlog_checkRint
                                      =!dlog_checkRint);
         break;
         case DLOG_CHECKS: /* toggle value */
            if(Ldlog_checkS)
               CheckDlgButton(hDlg,DLOG_CHECKS,dlog_checkSint
                                      =!dlog_checkSint);
         break;
         case DLOG_CHECKT: /* toggle value */
            if(Ldlog_checkT)
               CheckDlgButton(hDlg,DLOG_CHECKT,dlog_checkTint
                                      =!dlog_checkTint);
         break;
         case DLOG_CHECKU: /* toggle value */
            if(Ldlog_checkU)
               CheckDlgButton(hDlg,DLOG_CHECKU,dlog_checkUint=!dlog_checkUint);
         break;

/* DLOG RADIOS */
         case DLOG_RADIOA: /* set value on, turn other radios off */
            c = 'A';
            dlog_radioAint = 1;
            CheckRadioButton(hDlg,DLOG_RADIOA,DLOG_RADIOL,DLOG_RADIOA);
            adjustgenericradios(c);
         break;
         case DLOG_RADIOB: /* set value on, turn other radios off */
            c = 'B';
            dlog_radioBint = 1;
            CheckRadioButton(hDlg,DLOG_RADIOA,DLOG_RADIOL,DLOG_RADIOB);
            adjustgenericradios(c);
         break;
         case DLOG_RADIOC: /* set value on, turn other radios off */
            c = 'C';
            dlog_radioCint = 1;
            CheckRadioButton(hDlg,DLOG_RADIOA,DLOG_RADIOL,DLOG_RADIOC);
            adjustgenericradios(c);
         break;
         case DLOG_RADIOD: /* set value on, turn other radios off */
            c = 'D';
            dlog_radioDint = 1;
            CheckRadioButton(hDlg,DLOG_RADIOA,DLOG_RADIOL,DLOG_RADIOD);
            adjustgenericradios(c);
         break;
         case DLOG_RADIOE: /* set value on, turn other radios off */
            c = 'E';
            dlog_radioEint = 1;
            CheckRadioButton(hDlg,DLOG_RADIOA,DLOG_RADIOL,DLOG_RADIOE);
            adjustgenericradios(c);
         break;
         case DLOG_RADIOF: /* set value on, turn other radios off */
            c = 'F';
            dlog_radioFint = 1;
            CheckRadioButton(hDlg,DLOG_RADIOA,DLOG_RADIOL,DLOG_RADIOF);
            adjustgenericradios(c);
         break;
         case DLOG_RADIOG: /* set value on, turn other radios off */
            c = 'G';
            dlog_radioGint = 1;
            CheckRadioButton(hDlg,DLOG_RADIOA,DLOG_RADIOL,DLOG_RADIOG);
            adjustgenericradios(c);
         break;
         case DLOG_RADIOH: /* set value on, turn other radios off */
            c = 'H';
            dlog_radioHint = 1;
            CheckRadioButton(hDlg,DLOG_RADIOA,DLOG_RADIOL,DLOG_RADIOH);
            adjustgenericradios(c);
         break;
         case DLOG_RADIOI: /* set value on, turn other radios off */
            c = 'I';
            dlog_radioIint = 1;
            CheckRadioButton(hDlg,DLOG_RADIOA,DLOG_RADIOL,DLOG_RADIOI);
            adjustgenericradios(c);
         break;
         case DLOG_RADIOJ: /* set value on, turn other radios off */
            c = 'J';
            dlog_radioJint = 1;
            CheckRadioButton(hDlg,DLOG_RADIOA,DLOG_RADIOL,DLOG_RADIOJ);
            adjustgenericradios(c);
         break;
         case DLOG_RADIOK: /* set value on, turn other radios off */
            c = 'K';
            dlog_radioKint = 1;
            CheckRadioButton(hDlg,DLOG_RADIOA,DLOG_RADIOL,DLOG_RADIOK);
            adjustgenericradios(c);
         break;
         case DLOG_RADIOL: /* set value on, turn other radios off */
            c = 'L';
            dlog_radioLint = 1;
            CheckRadioButton(hDlg,DLOG_RADIOA,DLOG_RADIOL,DLOG_RADIOL);
            adjustgenericradios(c);
         break;

         case DLOG_RADIOM: /* set value on, turn other radios off */
            c = 'M';
            dlog_radioMint = 1;
            CheckRadioButton(hDlg,DLOG_RADIOM,DLOG_RADIOP,DLOG_RADIOM);
            adjustgenericradios(c);
         break;
         case DLOG_RADION: /* set value on, turn other radios off */
            c = 'N';
            dlog_radioNint = 1;
            CheckRadioButton(hDlg,DLOG_RADIOM,DLOG_RADIOP,DLOG_RADION);
            adjustgenericradios(c);
         break;
         case DLOG_RADIOO: /* set value on, turn other radios off */
            c = 'O';
            dlog_radioOint = 1;
            CheckRadioButton(hDlg,DLOG_RADIOM,DLOG_RADIOP,DLOG_RADIOO);
            adjustgenericradios(c);
         break;
         case DLOG_RADIOP: /* set value on, turn other radios off */
            c = 'P';
            dlog_radioPint = 1;
            CheckRadioButton(hDlg,DLOG_RADIOM,DLOG_RADIOP,DLOG_RADIOP);
            adjustgenericradios(c);
         break;

      }/*switch (LOWORD(wParam))*/
  }/*switch (message)*/

  return(ireturn);
}
/*___genericDialog()________________________________________________________*/

/****adjustgenericradios()****************************************************/
void adjustgenericradios(char c)
{
   if ((c <= 'L') && (c >= 'A'))
   {
      if(Ldlog_radioA&&c!='A') dlog_radioAint=0;
      if(Ldlog_radioB&&c!='B') dlog_radioBint=0;
      if(Ldlog_radioC&&c!='C') dlog_radioCint=0;
      if(Ldlog_radioD&&c!='D') dlog_radioDint=0;
      if(Ldlog_radioE&&c!='E') dlog_radioEint=0;
      if(Ldlog_radioF&&c!='F') dlog_radioFint=0;
      if(Ldlog_radioG&&c!='G') dlog_radioGint=0;
      if(Ldlog_radioH&&c!='H') dlog_radioHint=0;
      if(Ldlog_radioI&&c!='I') dlog_radioIint=0;
      if(Ldlog_radioJ&&c!='J') dlog_radioJint=0;
      if(Ldlog_radioK&&c!='K') dlog_radioKint=0;
      if(Ldlog_radioL&&c!='L') dlog_radioLint=0;
   }
   else if (c >= 'M')
   {
      if(Ldlog_radioM&&c!='M') dlog_radioMint=0;
      if(Ldlog_radioN&&c!='N') dlog_radioNint=0;
      if(Ldlog_radioO&&c!='O') dlog_radioOint=0;
      if(Ldlog_radioP&&c!='P') dlog_radioPint=0;
   }
} /*___adjustgenericradios()___________________________________________________*/
