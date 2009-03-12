
#include "PKIN.h"
#include "PKINDLOG.h"
#include "PKINMENU.h"

WindowPtr  genericdialogBox;
 Handle    subjectHandle,infoHandle; 
 Handle    checkcaptionHandle,LastcheckcaptionHandle,radiocaptionHandle;
 Handle    paramAHandle,paramBHandle,paramCHandle,paramDHandle,paramEHandle;
 Handle     textAHandle, textBHandle, textCHandle, textDHandle, textEHandle;   
 Handle    paramFHandle,paramGHandle,paramHHandle,paramIHandle;
 Handle     textFHandle, textGHandle, textHHandle, textIHandle;   

 Handle    OKHandle,optionHandle,cancelHandle;
 Handle    checkAHandle,checkBHandle,checkCHandle,checkDHandle,checkEHandle;
 Handle    checkFHandle,checkGHandle,checkHHandle,checkIHandle,checkJHandle;
 Handle    checkKHandle,checkLHandle,checkMHandle,checkNHandle,checkOHandle;
 Handle    checkPHandle,checkQHandle,checkRHandle,checkSHandle,checkTHandle;
 Handle    checkUHandle;

 Handle    radioAHandle,radioBHandle,radioCHandle,radioDHandle,radioEHandle;
 Handle    radioFHandle,radioGHandle,radioHHandle,radioIHandle,radioJHandle;
 Handle    radioKHandle,radioLHandle,radioMHandle,radioNHandle,radioOHandle;
 Handle    radioPHandle;

void DogenericDialog(void);
void adjustgenericradios(char[32]);
void genericDialog(void);
void DrawOKButton(DialogPtr);

#ifdef __POWERPC__
    ModalFilterUPP modfiltupp; /*980919*/
#endif        

#ifdef POWERMAC
#define FILTERCAST ModalFilterUPP
#else
#define FILTERCAST ProcPtr
#endif

/*****DogenericDialog********************************************************/
void    DogenericDialog()
{
  int idialogptr;
 
  if(     ACTIVE_DLOG == INIT_DLOG)           idialogptr = 9001;
  else if(ACTIVE_DLOG == REPORT_DLOG)         idialogptr = 9099;
  else if(ACTIVE_DLOG == RANGESTART_DLOG)     idialogptr = 9002;
  else if(ACTIVE_DLOG == BUILTINSCRIPTS_DLOG) idialogptr = 9003;
  else if(ACTIVE_DLOG == EXTERNALSCRIPT_DLOG) idialogptr = 9004;
  else if(ACTIVE_DLOG == KLUDGES_DLOG)        idialogptr = 9005;
  else if(ACTIVE_DLOG == RANGECONTROL_DLOG)   idialogptr = 9006;
  else if(ACTIVE_DLOG == FOCUSOPTIONS_DLOG)   idialogptr = 9007;
  else if(ACTIVE_DLOG == FOCUSBYRESNUM_DLOG)  idialogptr = 9008;
  else if(ACTIVE_DLOG == FOCUSPARAM_DLOG)     idialogptr = 9009;
  else if(ACTIVE_DLOG == ATOMMARKER_DLOG)     idialogptr = 9010;
  else if(ACTIVE_DLOG == RUNCONDITIONS_DLOG)  idialogptr = 9011;
  else if(ACTIVE_DLOG == RIBBON_DLOG)
  {
     if(!Lexplicitribbon) {idialogptr = 9012;}
     else  {idialogptr = 9013;}
  }
  else if(ACTIVE_DLOG == COLORSCALE_DLOG)     idialogptr = 9014;
  else idialogptr = 9000;
  
  genericdialogBox = GetNewDialog(idialogptr, 0L, (WindowPtr)-1L);  
                        /* ( BASE_RES_ID, NIL_POINTER, MOVE_TO_FRONT) */
  genericDialog();      /*PKMCDLOG*/
  DisposeDialog(genericdialogBox);
  genericdlogrouter();  /*PKINDLOG*/

}
/*___DogenericDialog________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****adjustgenericradios()***************************************************/
void adjustgenericradios(char s[32])
{/*Sets all logicals to OFF that are part of the set, except*/
 /*the first, which is set to be ON if previously OFF, else OFF also*/
   if(Ldlog_radioA && strchr(s,'A')!=NULL)
      SetControlValue((ControlHandle)radioAHandle
      ,dlog_radioAint=(s[0]=='A'&&!dlog_radioAint));
   if(Ldlog_radioB && strchr(s,'B')!=NULL)
      SetControlValue((ControlHandle)radioBHandle
      ,dlog_radioBint=(s[0]=='B'&&!dlog_radioBint));
   if(Ldlog_radioC && strchr(s,'C')!=NULL)
      SetControlValue((ControlHandle)radioCHandle
      ,dlog_radioCint=(s[0]=='C'&&!dlog_radioCint));
   if(Ldlog_radioD && strchr(s,'D')!=NULL)
      SetControlValue((ControlHandle)radioDHandle
      ,dlog_radioDint=(s[0]=='D'&&!dlog_radioDint));
   if(Ldlog_radioE && strchr(s,'E')!=NULL)
      SetControlValue((ControlHandle)radioEHandle
      ,dlog_radioEint=(s[0]=='E'&&!dlog_radioEint));
   if(Ldlog_radioF && strchr(s,'F')!=NULL)
      SetControlValue((ControlHandle)radioFHandle
      ,dlog_radioFint=(s[0]=='F'&&!dlog_radioFint));
   if(Ldlog_radioG && strchr(s,'G')!=NULL)
      SetControlValue((ControlHandle)radioGHandle
      ,dlog_radioGint=(s[0]=='G'&&!dlog_radioGint));
   if(Ldlog_radioH && strchr(s,'H')!=NULL)
      SetControlValue((ControlHandle)radioHHandle
      ,dlog_radioHint=(s[0]=='H'&&!dlog_radioHint));
   if(Ldlog_radioI && strchr(s,'I')!=NULL)
      SetControlValue((ControlHandle)radioIHandle
      ,dlog_radioIint=(s[0]=='I'&&!dlog_radioIint));
   if(Ldlog_radioJ && strchr(s,'J')!=NULL)
      SetControlValue((ControlHandle)radioJHandle
      ,dlog_radioJint=(s[0]=='J'&&!dlog_radioJint));
   if(Ldlog_radioK && strchr(s,'K')!=NULL)
      SetControlValue((ControlHandle)radioKHandle
      ,dlog_radioKint=(s[0]=='K'&&!dlog_radioKint));
   if(Ldlog_radioL && strchr(s,'L')!=NULL)
      SetControlValue((ControlHandle)radioLHandle
      ,dlog_radioLint=(s[0]=='L'&&!dlog_radioLint));
   if(Ldlog_radioM && strchr(s,'M')!=NULL)
      SetControlValue((ControlHandle)radioMHandle
      ,dlog_radioMint=(s[0]=='M'&&!dlog_radioMint));
   if(Ldlog_radioN && strchr(s,'N')!=NULL)
      SetControlValue((ControlHandle)radioNHandle
      ,dlog_radioNint=(s[0]=='N'&&!dlog_radioNint));
   if(Ldlog_radioO && strchr(s,'O')!=NULL)
      SetControlValue((ControlHandle)radioOHandle
      ,dlog_radioOint=(s[0]=='O'&&!dlog_radioOint));
   if(Ldlog_radioP && strchr(s,'P')!=NULL)
      SetControlValue((ControlHandle)radioPHandle
      ,dlog_radioPint=(s[0]=='P'&&!dlog_radioPint));

} /*___adjustgenericradios()________________________________________________*/ 

/****genericDialog()*********************************************************/
void    genericDialog()
{
 Boolean      done;
 short        itemHit, itemType;
 Rect        iRect;
 WindowPtr    oldPort;
 int hite,marg,it;
 
 GetPort(&oldPort);
 SetPort(genericdialogBox);
 /*ShowWindow(genericdialogBox);*/
 /*SelectWindow(genericdialogBox);*/

 


 hite = 20; /*uniform height of all dialog items*/
 marg = 2;  /*uniform margin between all dialog items*/
 /*hite and marg could be substituted for particular items ... */

 it = 0; /*starting value*/
 /*all DLOG have OK and CANCEL buttons at bottom*/
 /* titles have charstr set to defaults*/
 /* 4 */
 if(Ldlog_subject) 
 {
   GetDialogItem(genericdialogBox,DLOG_SUBJECT,&itemType,&subjectHandle,&iRect);
   CtoPstr(dlog_subjectstr);
   SetDialogItemText(subjectHandle, (unsigned char *)dlog_subjectstr);
 }

 /*  9  */
 if(Ldlog_paramA) 
 {/* 10 */     
   GetDialogItem(genericdialogBox,DLOG_PARAMA,&itemType,&paramAHandle,&iRect);
   CtoPstr(dlog_paramAstr);
   SetDialogItemText(paramAHandle, (unsigned char *)dlog_paramAstr);
 }

 if(Ldlog_textA) 
 {/* 20 */ 
   GetDialogItem(genericdialogBox,DLOG_TEXTA,&itemType,&textAHandle,&iRect);
   CtoPstr(dlog_textAstr);
   SetDialogItemText(textAHandle, (unsigned char *)dlog_textAstr);
 }
 
 if(Ldlog_paramB) 
 {/* 11 */   
   GetDialogItem(genericdialogBox,DLOG_PARAMB,&itemType,&paramBHandle,&iRect);
   CtoPstr(dlog_paramBstr);
   SetDialogItemText(paramBHandle, (unsigned char *)dlog_paramBstr);
 }
 
 if(Ldlog_textB) 
 {/* 21 */ 
   GetDialogItem(genericdialogBox,DLOG_TEXTB,&itemType,&textBHandle,&iRect);
   CtoPstr(dlog_textBstr);
   SetDialogItemText(textBHandle, (unsigned char *)dlog_textBstr);
 }
 
 if(Ldlog_paramC)
 {/* 12 */ 
   GetDialogItem(genericdialogBox,DLOG_PARAMC,&itemType,&paramCHandle,&iRect);
   CtoPstr(dlog_paramCstr);
   SetDialogItemText(paramCHandle, (unsigned char *)dlog_paramCstr);
 }
 
 if(Ldlog_textC)
 {/* 22 */ 
   GetDialogItem(genericdialogBox,DLOG_TEXTC,&itemType,&textCHandle,&iRect);
   CtoPstr(dlog_textCstr);
   SetDialogItemText(textCHandle, (unsigned char *)dlog_textCstr);
 }
 
 if(Ldlog_paramD)
 {/* 13 */ 
   GetDialogItem(genericdialogBox,DLOG_PARAMD,&itemType,&paramDHandle,&iRect);
   CtoPstr(dlog_paramDstr);
   SetDialogItemText(paramDHandle, (unsigned char *)dlog_paramDstr);
 }
 
 if(Ldlog_textD)
 {/* 23 */ 
   GetDialogItem(genericdialogBox,DLOG_TEXTD,&itemType,&textDHandle,&iRect);
   CtoPstr(dlog_textDstr);
   SetDialogItemText(textDHandle, (unsigned char *)dlog_textDstr);
 }
 
 if(Ldlog_paramE)
 {/* 14 */ 
   GetDialogItem(genericdialogBox,DLOG_PARAME,&itemType,&paramEHandle,&iRect);
   CtoPstr(dlog_paramEstr);
   SetDialogItemText(paramEHandle, (unsigned char *)dlog_paramEstr);
 }
 
 if(Ldlog_textE)
 {/* 24 */ 
   GetDialogItem(genericdialogBox,DLOG_TEXTE,&itemType,&textEHandle,&iRect);
   CtoPstr(dlog_textEstr);
   SetDialogItemText(textEHandle, (unsigned char *)dlog_textEstr);
 }
 
 if(Ldlog_paramF)
 {/* 15 */ 
   GetDialogItem(genericdialogBox,DLOG_PARAMF,&itemType,&paramFHandle,&iRect);
   CtoPstr(dlog_paramFstr);
   SetDialogItemText(paramFHandle, (unsigned char *)dlog_paramFstr);
 }
 
 if(Ldlog_textF)
 {/* 25 */ 
   GetDialogItem(genericdialogBox,DLOG_TEXTF,&itemType,&textFHandle,&iRect);
   CtoPstr(dlog_textFstr);
   SetDialogItemText(textFHandle, (unsigned char *)dlog_textFstr);
 }
 
 if(Ldlog_paramG)
 {/* 16 */ 
   GetDialogItem(genericdialogBox,DLOG_PARAMG,&itemType,&paramGHandle,&iRect);
   CtoPstr(dlog_paramGstr);
   SetDialogItemText(paramGHandle, (unsigned char *)dlog_paramGstr);
 }
 
 if(Ldlog_textG)
 {/* 26 */ 
   GetDialogItem(genericdialogBox,DLOG_TEXTG,&itemType,&textGHandle,&iRect);
   CtoPstr(dlog_textGstr);
   SetDialogItemText(textGHandle, (unsigned char *)dlog_textGstr);
 }
 
 if(Ldlog_paramH)
 {/* 17 */ 
   GetDialogItem(genericdialogBox,DLOG_PARAMH,&itemType,&paramHHandle,&iRect);
   CtoPstr(dlog_paramHstr);
   SetDialogItemText(paramHHandle, (unsigned char *)dlog_paramHstr);
 }

 if(Ldlog_textH)
 {/* 27 */ 
   GetDialogItem(genericdialogBox,DLOG_TEXTH,&itemType,&textHHandle,&iRect);
   CtoPstr(dlog_textHstr);
   SetDialogItemText(textHHandle, (unsigned char *)dlog_textHstr);
 }

 if(Ldlog_paramI)
 {/* 17 */ 
   GetDialogItem(genericdialogBox,DLOG_PARAMI,&itemType,&paramIHandle,&iRect);
   CtoPstr(dlog_paramIstr);
   SetDialogItemText(paramIHandle, (unsigned char *)dlog_paramIstr);
 }
 
 if(Ldlog_textI)
 {/* 27 */ 
   GetDialogItem(genericdialogBox,DLOG_TEXTI,&itemType,&textIHandle,&iRect);
   CtoPstr(dlog_textIstr);
   SetDialogItemText(textIHandle, (unsigned char *)dlog_textIstr);
 }

 
 /* 18 28 19 29  */
 /* 6  just above first check boxes*/ 
 if(Ldlog_check_caption)
 {
   GetDialogItem(genericdialogBox,DLOG_CHECK_CAPTION,&itemType,&checkcaptionHandle,&iRect);
   CtoPstr(dlog_checkcaptionstr);
   SetDialogItemText((Handle)checkcaptionHandle, (unsigned char *)dlog_checkcaptionstr);
 }
 
 if(Ldlog_checkA)
 {/*30*/
   GetDialogItem(genericdialogBox,DLOG_CHECKA,&itemType,&checkAHandle,&iRect);
   SetControlValue((ControlHandle)checkAHandle, dlog_checkAint);
   CtoPstr(dlog_checkAstr);
   SetControlTitle( (ControlHandle)checkAHandle, (unsigned char *)dlog_checkAstr);    
 }
 
 if(Ldlog_checkB)
 {/*31*/
   GetDialogItem(genericdialogBox,DLOG_CHECKB,&itemType,&checkBHandle,&iRect);
   SetControlValue((ControlHandle)checkBHandle, dlog_checkBint);
   CtoPstr(dlog_checkBstr);
   SetControlTitle( (ControlHandle)checkBHandle, (unsigned char *)dlog_checkBstr);    
 }
 
 if(Ldlog_checkC)
 {/*32*/
   GetDialogItem(genericdialogBox,DLOG_CHECKC,&itemType,&checkCHandle,&iRect);
   SetControlValue((ControlHandle)checkCHandle, dlog_checkCint);
   CtoPstr(dlog_checkCstr);
   SetControlTitle( (ControlHandle)checkCHandle, (unsigned char *)dlog_checkCstr);    
 }
 
 if(Ldlog_checkD)
 {/*33*/
   GetDialogItem(genericdialogBox,DLOG_CHECKD,&itemType,&checkDHandle,&iRect);
   SetControlValue((ControlHandle)checkDHandle, dlog_checkDint);
   CtoPstr(dlog_checkDstr);
   SetControlTitle( (ControlHandle)checkDHandle, (unsigned char *)dlog_checkDstr);    
 }
 
 if(Ldlog_checkE)
 {/*34*/
   GetDialogItem(genericdialogBox,DLOG_CHECKE,&itemType,&checkEHandle,&iRect);
   SetControlValue((ControlHandle)checkEHandle, dlog_checkEint);
   CtoPstr(dlog_checkEstr);
   SetControlTitle( (ControlHandle)checkEHandle, (unsigned char *)dlog_checkEstr);    
 }
 
 if(Ldlog_checkF)
 {/*35*/
 GetDialogItem(genericdialogBox,DLOG_CHECKF,&itemType,&checkFHandle,&iRect);
   SetControlValue((ControlHandle)checkFHandle, dlog_checkFint);
   CtoPstr(dlog_checkFstr);
   SetControlTitle( (ControlHandle)checkFHandle, (unsigned char *)dlog_checkFstr);    
 }
 
 if(Ldlog_checkG)
 {/*36*/
   GetDialogItem(genericdialogBox,DLOG_CHECKG,&itemType,&checkGHandle,&iRect);
   SetControlValue((ControlHandle)checkGHandle, dlog_checkGint);
   CtoPstr(dlog_checkGstr);
   SetControlTitle( (ControlHandle)checkGHandle, (unsigned char *)dlog_checkGstr);    
 }
 
 if(Ldlog_checkH)
 {/*37*/
   GetDialogItem(genericdialogBox,DLOG_CHECKH,&itemType,&checkHHandle,&iRect);
   SetControlValue((ControlHandle)checkHHandle, dlog_checkHint);
   CtoPstr(dlog_checkHstr);
   SetControlTitle( (ControlHandle)checkHHandle, (unsigned char *)dlog_checkHstr);    
 }
 
 if(Ldlog_checkI)
 {/*38*/
   GetDialogItem(genericdialogBox,DLOG_CHECKI,&itemType,&checkIHandle,&iRect);
   SetControlValue((ControlHandle)checkIHandle, dlog_checkIint);
   CtoPstr(dlog_checkIstr);
   SetControlTitle( (ControlHandle)checkIHandle, (unsigned char *)dlog_checkIstr);    
 }
 
 if(Ldlog_checkJ)
 {/*39*/
   GetDialogItem(genericdialogBox,DLOG_CHECKJ,&itemType,&checkJHandle,&iRect);
   SetControlValue((ControlHandle)checkJHandle, dlog_checkJint);
   CtoPstr(dlog_checkJstr);
   SetControlTitle( (ControlHandle)checkJHandle, (unsigned char *)dlog_checkJstr);    
 }
 
 if(Ldlog_checkK)
 {/*40*/
   GetDialogItem(genericdialogBox,DLOG_CHECKK,&itemType,&checkKHandle,&iRect);
   SetControlValue((ControlHandle)checkKHandle, dlog_checkKint);
   CtoPstr(dlog_checkKstr);
   SetControlTitle( (ControlHandle)checkKHandle, (unsigned char *)dlog_checkKstr);    
 }
 
 if(Ldlog_checkL)
 {/*41*/
   GetDialogItem(genericdialogBox,DLOG_CHECKL,&itemType,&checkLHandle,&iRect);
   SetControlValue((ControlHandle)checkLHandle, dlog_checkLint);
   CtoPstr(dlog_checkLstr);
   SetControlTitle( (ControlHandle)checkLHandle, (unsigned char *)dlog_checkLstr);    
 }
 
 if(Ldlog_checkM)
 {/*42*/
   GetDialogItem(genericdialogBox,DLOG_CHECKM,&itemType,&checkMHandle,&iRect);
   SetControlValue((ControlHandle)checkMHandle, dlog_checkMint);
   CtoPstr(dlog_checkMstr);
   SetControlTitle( (ControlHandle)checkMHandle, (unsigned char *)dlog_checkMstr);    
 }
 
 if(Ldlog_checkN)
 {/*43*/
   GetDialogItem(genericdialogBox,DLOG_CHECKN,&itemType,&checkNHandle,&iRect);
   SetControlValue((ControlHandle)checkNHandle, dlog_checkNint);
   CtoPstr(dlog_checkNstr);
   SetControlTitle( (ControlHandle)checkNHandle, (unsigned char *)dlog_checkNstr);    
 }
 
 if(Ldlog_checkO)
 {/*44*/
   GetDialogItem(genericdialogBox,DLOG_CHECKO,&itemType,&checkOHandle,&iRect);
   SetControlValue((ControlHandle)checkOHandle, dlog_checkOint);
   CtoPstr(dlog_checkOstr);
   SetControlTitle( (ControlHandle)checkOHandle, (unsigned char *)dlog_checkOstr);    
 }
 
 if(Ldlog_checkP)
 {/*45*/
   GetDialogItem(genericdialogBox,DLOG_CHECKP,&itemType,&checkPHandle,&iRect);
   SetControlValue((ControlHandle)checkPHandle, dlog_checkPint);
   CtoPstr(dlog_checkPstr);
   SetControlTitle( (ControlHandle)checkPHandle, (unsigned char *)dlog_checkPstr);    
 }
 
 if(Ldlog_checkQ)
 {/*46*/
   GetDialogItem(genericdialogBox,DLOG_CHECKQ,&itemType,&checkQHandle,&iRect);
   SetControlValue((ControlHandle)checkQHandle, dlog_checkQint);
   CtoPstr(dlog_checkQstr);
   SetControlTitle( (ControlHandle)checkQHandle, (unsigned char *)dlog_checkQstr);    
 }
 
 if(Ldlog_checkR)
 {/*47*/
   GetDialogItem(genericdialogBox,DLOG_CHECKR,&itemType,&checkRHandle,&iRect);
   SetControlValue((ControlHandle)checkRHandle, dlog_checkRint);
   CtoPstr(dlog_checkRstr);
   SetControlTitle( (ControlHandle)checkRHandle, (unsigned char *)dlog_checkRstr);    
 }
 
 if(Ldlog_checkS)
 {/*48*/
   GetDialogItem(genericdialogBox,DLOG_CHECKS,&itemType,&checkSHandle,&iRect);
   SetControlValue((ControlHandle)checkSHandle, dlog_checkSint);
   CtoPstr(dlog_checkSstr);
   SetControlTitle( (ControlHandle)checkSHandle, (unsigned char *)dlog_checkSstr);    
 }
 
 /* 7  just above last check box*/ 
 if(Ldlog_checkLast_caption)
 {
   GetDialogItem(genericdialogBox,DLOG_CHECKLAST_CAPTION,&itemType,&LastcheckcaptionHandle,&iRect);
   CtoPstr(dlog_checkLastcaptionstr);
   SetDialogItemText((Handle)LastcheckcaptionHandle, (unsigned char *)dlog_checkLastcaptionstr);
 }

 if(Ldlog_checkT)
 {/*49*/
   GetDialogItem(genericdialogBox,DLOG_CHECKT,&itemType,&checkTHandle,&iRect);
   SetControlValue((ControlHandle)checkTHandle, dlog_checkTint);
   CtoPstr(dlog_checkTstr);
   SetControlTitle( (ControlHandle)checkTHandle, (unsigned char *)dlog_checkTstr);    
 }
 if(Ldlog_checkU)
 {/*66*/
   GetDialogItem(genericdialogBox,DLOG_CHECKU,&itemType,&checkUHandle,&iRect);
   SetControlValue((ControlHandle)checkUHandle, dlog_checkUint);
   CtoPstr(dlog_checkUstr);
   SetControlTitle( (ControlHandle)checkUHandle, (unsigned char *)dlog_checkUstr);    
 }
 
 
 /* 8  just above radio boxes*/ 
 if(Ldlog_radio_caption)
 {
   GetDialogItem(genericdialogBox,DLOG_RADIO_CAPTION,&itemType,&radiocaptionHandle,&iRect);
   CtoPstr(dlog_radiocaptionstr);
   SetDialogItemText((Handle)radiocaptionHandle, (unsigned char *)dlog_radiocaptionstr);
 }
 
 if(Ldlog_radioA)
 {/*50*/
   GetDialogItem(genericdialogBox,DLOG_RADIOA,&itemType,&radioAHandle,&iRect);
   SetControlValue((ControlHandle)radioAHandle, dlog_radioAint);
   CtoPstr(dlog_radioAstr);
   SetControlTitle( (ControlHandle)radioAHandle, (unsigned char *)dlog_radioAstr); 
 }
 
 if(Ldlog_radioB)
 {/*51*/
   GetDialogItem(genericdialogBox,DLOG_RADIOB,&itemType,&radioBHandle,&iRect);
   SetControlValue((ControlHandle)radioBHandle, dlog_radioBint);
   CtoPstr(dlog_radioBstr);
   SetControlTitle( (ControlHandle)radioBHandle, (unsigned char *)dlog_radioBstr);    
 }
 
 if(Ldlog_radioC)
 {/*52*/
   GetDialogItem(genericdialogBox,DLOG_RADIOC,&itemType,&radioCHandle,&iRect);
   SetControlValue((ControlHandle)radioCHandle, dlog_radioCint);
   CtoPstr(dlog_radioCstr);
   SetControlTitle( (ControlHandle)radioCHandle, (unsigned char *)dlog_radioCstr);    
 }
 
 if(Ldlog_radioD)
 {/*53*/
   GetDialogItem(genericdialogBox,DLOG_RADIOD,&itemType,&radioDHandle,&iRect);
   SetControlValue((ControlHandle)radioDHandle, dlog_radioDint);
   CtoPstr(dlog_radioDstr);
   SetControlTitle( (ControlHandle)radioDHandle, (unsigned char *)dlog_radioDstr);    
 }
 
 if(Ldlog_radioE)
 {/*54*/
   GetDialogItem(genericdialogBox,DLOG_RADIOE,&itemType,&radioEHandle,&iRect);
   SetControlValue((ControlHandle)radioEHandle, dlog_radioEint);
   CtoPstr(dlog_radioEstr);
   SetControlTitle( (ControlHandle)radioEHandle, (unsigned char *)dlog_radioEstr);    
 }
 
 if(Ldlog_radioF)
 {/*55*/
   GetDialogItem(genericdialogBox,DLOG_RADIOF,&itemType,&radioFHandle,&iRect);
   SetControlValue((ControlHandle)radioFHandle, dlog_radioFint);
   CtoPstr(dlog_radioFstr);
   SetControlTitle( (ControlHandle)radioFHandle, (unsigned char *)dlog_radioFstr);    
 }
 
 if(Ldlog_radioG)
 {/*56*/
   GetDialogItem(genericdialogBox,DLOG_RADIOG,&itemType,&radioGHandle,&iRect);
   SetControlValue((ControlHandle)radioGHandle, dlog_radioGint);
   CtoPstr(dlog_radioGstr);
   SetControlTitle( (ControlHandle)radioGHandle, (unsigned char *)dlog_radioGstr);    
 }
 
 if(Ldlog_radioH)
 {/*57*/
   GetDialogItem(genericdialogBox,DLOG_RADIOH,&itemType,&radioHHandle,&iRect);\
   SetControlValue((ControlHandle)radioHHandle, dlog_radioHint);
   CtoPstr(dlog_radioHstr);
   SetControlTitle( (ControlHandle)radioHHandle, (unsigned char *)dlog_radioHstr);    
 }
 
 if(Ldlog_radioI)
 {/*58*/
   GetDialogItem(genericdialogBox,DLOG_RADIOI,&itemType,&radioIHandle,&iRect);
   SetControlValue((ControlHandle)radioIHandle, dlog_radioIint);
   CtoPstr(dlog_radioIstr);
   SetControlTitle( (ControlHandle)radioIHandle, (unsigned char *)dlog_radioIstr);    
 }
 
 if(Ldlog_radioJ)
 {/*59*/
   GetDialogItem(genericdialogBox,DLOG_RADIOJ,&itemType,&radioJHandle,&iRect);
   SetControlValue((ControlHandle)radioJHandle, dlog_radioJint);
   CtoPstr(dlog_radioJstr);
   SetControlTitle( (ControlHandle)radioJHandle, (unsigned char *)dlog_radioJstr);    
 }
 
 if(Ldlog_radioK)
 {/*60*/
   GetDialogItem(genericdialogBox,DLOG_RADIOK,&itemType,&radioKHandle,&iRect);
   SetControlValue((ControlHandle)radioKHandle, dlog_radioKint);
   CtoPstr(dlog_radioKstr);
   SetControlTitle( (ControlHandle)radioKHandle, (unsigned char *)dlog_radioKstr);    
 }
 
 if(Ldlog_radioL)
 {/*61*/
   GetDialogItem(genericdialogBox,DLOG_RADIOL,&itemType,&radioLHandle,&iRect);
   SetControlValue((ControlHandle)radioLHandle, dlog_radioLint);
   CtoPstr(dlog_radioLstr);
   SetControlTitle( (ControlHandle)radioLHandle, (unsigned char *)dlog_radioLstr);    
 }
 if(Ldlog_radioM)
 {/*62*/
   GetDialogItem(genericdialogBox,DLOG_RADIOM,&itemType,&radioMHandle,&iRect);
   SetControlValue((ControlHandle)radioMHandle, dlog_radioMint);
   CtoPstr(dlog_radioMstr);
   SetControlTitle( (ControlHandle)radioMHandle, (unsigned char *)dlog_radioMstr);    
 }
 if(Ldlog_radioN)
 {/*63*/
   GetDialogItem(genericdialogBox,DLOG_RADION,&itemType,&radioNHandle,&iRect);
   SetControlValue((ControlHandle)radioNHandle, dlog_radioNint);
   CtoPstr(dlog_radioNstr);
   SetControlTitle( (ControlHandle)radioNHandle, (unsigned char *)dlog_radioNstr);    
 }
 if(Ldlog_radioO)
 {/*64*/
   GetDialogItem(genericdialogBox,DLOG_RADIOO,&itemType,&radioOHandle,&iRect);
   SetControlValue((ControlHandle)radioOHandle, dlog_radioOint);
   CtoPstr(dlog_radioOstr);
   SetControlTitle( (ControlHandle)radioOHandle, (unsigned char *)dlog_radioOstr);    
 }
 if(Ldlog_radioP)
 {/*65*/
   GetDialogItem(genericdialogBox,DLOG_RADIOP,&itemType,&radioPHandle,&iRect);
   SetControlValue((ControlHandle)radioPHandle, dlog_radioPint);
   CtoPstr(dlog_radioPstr);
   SetControlTitle( (ControlHandle)radioPHandle, (unsigned char *)dlog_radioPstr);    
 }
 
 /* 5 at bottom of dialog box just before OK,Cancel,Option  row*/
 if(Ldlog_info)
 {
   GetDialogItem(genericdialogBox,DLOG_INFO,&itemType,&infoHandle,&iRect);
   CtoPstr(dlog_infostr);
   SetDialogItemText((Handle)infoHandle, (unsigned char *)dlog_infostr);
 }
 
 /* 1 */ 
 GetDialogItem( genericdialogBox,DLOG_OK,&itemType,&OKHandle,&iRect);
 CtoPstr(dlog_OKstr);
 SetControlTitle( (ControlHandle)OKHandle, (unsigned char *)dlog_OKstr);
 /* 2 */    
 GetDialogItem( genericdialogBox,DLOG_CANCEL,&itemType,&cancelHandle,&iRect);
 CtoPstr(dlog_cancelstr);
 SetControlTitle((ControlHandle)cancelHandle,(unsigned char *)dlog_cancelstr);
 /* 3 */    
 
 if(Ldlog_option)
 {
   GetDialogItem(genericdialogBox,DLOG_OPTION,&itemType,&optionHandle,&iRect);
   CtoPstr(dlog_optionstr);
   SetControlTitle((ControlHandle)optionHandle,(unsigned char *)dlog_optionstr);
 }
 
 if(Ldlog_paramA)  SelectDialogItemText(genericdialogBox, DLOG_PARAMA, 0, 32767);
         /*set up so first key stroke overwrites*/   

 DrawOKButton(genericdialogBox); /*MACDLOG.C*/

 ShowWindow(genericdialogBox);
 
 done = FALSE;
 Ldlog_OKHIT = 0;
 Ldlog_cancelHIT = 0;
 Ldlog_optionHIT = 0; 
 while (!done)
 {/*!done*/
     ModalDialog((FILTERCAST)0L, &itemHit); /*cast OK __POWERPC__ 980919*/
               /*0L==NIL default filterProc enters characters,*/
               /* CR and enter sets itemHit==1==DLOG_OK*/
      /*prototype is Dialog box with unused options that might be hitable*/
      /*test for hit first, then cover ass */
     
     if(itemHit == DLOG_PARAMA) 
       if(Ldlog_paramA)
          GetDialogItemText(paramAHandle,(unsigned char *)dlog_paramAstr);
     if(itemHit == DLOG_PARAMB) 
       if(Ldlog_paramB)
          GetDialogItemText(paramBHandle,(unsigned char *)dlog_paramBstr);
     if(itemHit == DLOG_PARAMC) 
       if(Ldlog_paramC)
          GetDialogItemText(paramCHandle,(unsigned char *)dlog_paramCstr);
     if(itemHit == DLOG_PARAMD) 
       if(Ldlog_paramD)
          GetDialogItemText(paramDHandle,(unsigned char *)dlog_paramDstr);
     if(itemHit == DLOG_PARAME) 
       if(Ldlog_paramE)
          GetDialogItemText(paramEHandle,(unsigned char *)dlog_paramEstr);
     if(itemHit == DLOG_PARAMF) 
       if(Ldlog_paramF)
          GetDialogItemText(paramFHandle,(unsigned char *)dlog_paramFstr);
     if(itemHit == DLOG_PARAMG) 
       if(Ldlog_paramG)
          GetDialogItemText(paramGHandle,(unsigned char *)dlog_paramGstr);
     if(itemHit == DLOG_PARAMH) 
       if(Ldlog_paramH)
          GetDialogItemText(paramHHandle,(unsigned char *)dlog_paramHstr);
     if(itemHit == DLOG_PARAMI) 
       if(Ldlog_paramI)
          GetDialogItemText(paramIHandle,(unsigned char *)dlog_paramIstr);

     
     if(itemHit == DLOG_CHECKA) 
        if(Ldlog_checkA) /* toggle value */ 
          SetControlValue((ControlHandle)checkAHandle
               ,dlog_checkAint=!dlog_checkAint);
     if(itemHit == DLOG_CHECKB) 
       if(Ldlog_checkB) /* toggle value */ 
          SetControlValue((ControlHandle)checkBHandle
               ,dlog_checkBint=!dlog_checkBint);
     if(itemHit == DLOG_CHECKC) 
       if(Ldlog_checkC) /* toggle value */               
          SetControlValue((ControlHandle)checkCHandle
               ,dlog_checkCint=!dlog_checkCint);
     if(itemHit == DLOG_CHECKD) 
       if(Ldlog_checkD) /* toggle value */ 
          SetControlValue((ControlHandle)checkDHandle
               ,dlog_checkDint=!dlog_checkDint);
     if(itemHit == DLOG_CHECKE) 
       if(Ldlog_checkE) /* toggle value */ 
          SetControlValue((ControlHandle)checkEHandle
               ,dlog_checkEint=!dlog_checkEint);
     if(itemHit == DLOG_CHECKF) 
       if(Ldlog_checkF) /* toggle value */ 
          SetControlValue((ControlHandle)checkFHandle
               ,dlog_checkFint=!dlog_checkFint);
     if(itemHit == DLOG_CHECKG) 
       if(Ldlog_checkG) /* toggle value */ 
          SetControlValue((ControlHandle)checkGHandle
               ,dlog_checkGint=!dlog_checkGint);
     if(itemHit == DLOG_CHECKH) 
       if(Ldlog_checkH) /* toggle value */ 
          SetControlValue((ControlHandle)checkHHandle
               ,dlog_checkHint=!dlog_checkHint);
     if(itemHit == DLOG_CHECKI) 
       if(Ldlog_checkI) /* toggle value */ 
          SetControlValue((ControlHandle)checkIHandle
               ,dlog_checkIint=!dlog_checkIint);
     if(itemHit == DLOG_CHECKJ) 
       if(Ldlog_checkJ) /* toggle value */ 
          SetControlValue((ControlHandle)checkJHandle
               ,dlog_checkJint=!dlog_checkJint);
     if(itemHit == DLOG_CHECKK) 
       if(Ldlog_checkK) /* toggle value */ 
          SetControlValue((ControlHandle)checkKHandle
               ,dlog_checkKint=!dlog_checkKint);
     if(itemHit == DLOG_CHECKL) 
       if(Ldlog_checkL) /* toggle value */ 
          SetControlValue((ControlHandle)checkLHandle
               ,dlog_checkLint=!dlog_checkLint);
     if(itemHit == DLOG_CHECKM) 
       if(Ldlog_checkM) /* toggle value */ 
          SetControlValue((ControlHandle)checkMHandle
               ,dlog_checkMint=!dlog_checkMint);
     if(itemHit == DLOG_CHECKN) 
       if(Ldlog_checkN) /* toggle value */ 
          SetControlValue((ControlHandle)checkNHandle
               ,dlog_checkNint=!dlog_checkNint);
     if(itemHit == DLOG_CHECKO) 
       if(Ldlog_checkO) /* toggle value */ 
          SetControlValue((ControlHandle)checkOHandle
               ,dlog_checkOint=!dlog_checkOint);
     if(itemHit == DLOG_CHECKP) 
       if(Ldlog_checkP) /* toggle value */ 
          SetControlValue((ControlHandle)checkPHandle
               ,dlog_checkPint=!dlog_checkPint);

     if(itemHit == DLOG_CHECKQ) 
       if(Ldlog_checkQ) /* toggle value */ 
          SetControlValue((ControlHandle)checkQHandle
               ,dlog_checkQint=!dlog_checkQint);
     if(itemHit == DLOG_CHECKR) 
       if(Ldlog_checkR) /* toggle value */ 
          SetControlValue((ControlHandle)checkRHandle
               ,dlog_checkRint=!dlog_checkRint);
     if(itemHit == DLOG_CHECKS) 
       if(Ldlog_checkS) /* toggle value */ 
          SetControlValue((ControlHandle)checkSHandle
               ,dlog_checkSint=!dlog_checkSint);


     if(itemHit == DLOG_CHECKT) 
       if(Ldlog_checkT) /* toggle value */ 
          SetControlValue((ControlHandle)checkTHandle
               ,dlog_checkTint=!dlog_checkTint);

     if(itemHit == DLOG_CHECKU) 
       if(Ldlog_checkU) /* toggle value */ 
          SetControlValue((ControlHandle)checkUHandle
               ,dlog_checkUint=!dlog_checkUint);


     
     if(itemHit == DLOG_RADIOA) /* set 1st on, turn other radios off */
       if(Ldlog_radioA) adjustgenericradios("ABCDEFGHIJKL");
      
     if(itemHit == DLOG_RADIOB) 
       if(Ldlog_radioB) adjustgenericradios("BACDEFGHIJKL");
        
     if(itemHit == DLOG_RADIOC) 
       if(Ldlog_radioC) adjustgenericradios("CABDEFGHIJKL");
       
     if(itemHit == DLOG_RADIOD) 
       if(Ldlog_radioD) adjustgenericradios("DABCEFGHIJKL");
        
     if(itemHit == DLOG_RADIOE) 
       if(Ldlog_radioE) adjustgenericradios("EABCDFGHIJKL");
        
     if(itemHit == DLOG_RADIOF) 
       if(Ldlog_radioF) adjustgenericradios("FABCDEGHIJKL");
        
     if(itemHit == DLOG_RADIOG) 
       if(Ldlog_radioG) adjustgenericradios("GABCDEFHIJKL");
        
     if(itemHit == DLOG_RADIOH) 
       if(Ldlog_radioH) adjustgenericradios("HABCDEFGIJKL");
        
     if(itemHit == DLOG_RADIOI) 
       if(Ldlog_radioI) adjustgenericradios("IABCDEFGHJKL");
       
     if(itemHit == DLOG_RADIOJ) 
       if(Ldlog_radioJ) adjustgenericradios("JABCDEFGHIKL");
       
     if(itemHit == DLOG_RADIOK) 
       if(Ldlog_radioK) adjustgenericradios("KABCDEFGHIJL");
       
     if(itemHit == DLOG_RADIOL) 
       if(Ldlog_radioL) adjustgenericradios("LABCDEFGHIJK");
       
       
     if(itemHit == DLOG_RADIOM) 
       if(Ldlog_radioM) adjustgenericradios("MNOP");

     if(itemHit == DLOG_RADION) 
       if(Ldlog_radioN) adjustgenericradios("NMOP");

     if(itemHit == DLOG_RADIOO) 
       if(Ldlog_radioO) adjustgenericradios("OMNP");

     if(itemHit == DLOG_RADIOP) 
       if(Ldlog_radioP) adjustgenericradios("PMNO");

     if(Ldlog_info) /*971223*/
       if(itemHit == DLOG_INFO) 
          GetDialogItemText(infoHandle,(unsigned char *)dlog_infostr);
          /*recondition(dlog_infostr);*/ /*reset to be a C string, MACMAIN.c*/
    
     if(itemHit == DLOG_OK) { Ldlog_OKHIT = 1; done=1; }       
     if(itemHit == DLOG_CANCEL) { Ldlog_cancelHIT = 1; done=1; } 
     if(Ldlog_option) if(itemHit == DLOG_OPTION) { Ldlog_optionHIT = 1; done=1; } 
 }/*!done*/
 if(Ldlog_paramA) PtoCstr((unsigned char *)dlog_paramAstr);
 if(Ldlog_paramB) PtoCstr((unsigned char *)dlog_paramBstr);
 if(Ldlog_paramC) PtoCstr((unsigned char *)dlog_paramCstr);
 if(Ldlog_paramD) PtoCstr((unsigned char *)dlog_paramDstr);
 if(Ldlog_paramE) PtoCstr((unsigned char *)dlog_paramEstr);
 if(Ldlog_paramF) PtoCstr((unsigned char *)dlog_paramFstr);
 if(Ldlog_paramG) PtoCstr((unsigned char *)dlog_paramGstr);
 if(Ldlog_paramH) PtoCstr((unsigned char *)dlog_paramHstr);
 if(Ldlog_paramI) PtoCstr((unsigned char *)dlog_paramIstr);

 if(Ldlog_info) PtoCstr((unsigned char *)dlog_infostr);

 SetPort(oldPort);    
}
/*___genericDialog()________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*****DrawOKButton***********************************************************/
void    DrawOKButton(DialogPtr theDialog )
{          
    short       itemType;
    Rect        itemRect;          
    Handle      item;
    GrafPtr     oldPort;
    
     GetDialogItem( theDialog, DLOG_OK, &itemType, &item, &itemRect );
     GetPort( &oldPort );
     SetPort( theDialog );
#ifdef POWERMAC
/*m*/        PenPat(&qd.black); 
#else
/*m*/        PenPat(black); 
#endif
     PenSize( 3, 3);
     InsetRect( &itemRect, -4, -4 );
     FrameRoundRect( &itemRect, 16, 16 );
     PenNormal();
     SetPort( oldPort );
}
/*___DrawOKButton___________________________________________________________*/

