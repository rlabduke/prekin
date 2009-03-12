#ifdef EXTERNDLOG
#undef  EXTERNDLOG
#define EXTERNDLOG
#else
#define EXTERNDLOG extern
#endif

/*                             PKINDLOG.h                   */
/*prototypes of all subroutines in PKINDLOG.c ==> PKINDLOG.h */
void DogenericDialog(void);
void cleargenericflags(void);
void genericdlogrouter(void);

void DoInitDialog(void)      ; 
void DoReportDialog(void)      ; 
void DoRangeStartDialog(void)      ; 
void DoBuiltinScriptsDialog(void)  ; 
void DoExternalScriptDialog(void)  ; 
void DoKludgesDialog(void)         ;
void DoRangeControlDialog(void)    ; 
void DoFocusOptionsDialog(void)    ; 
void DoFocusParamDialog(void)      ; 
void DoAtomMarkerDialog(void)      ;  
void DoRibbonDialog(void)          ; 
void DoRunConditionsDialog(void)   ;  
void DoColorScaleDialog(void)      ;

void InitDialog(void)        ;
void ReportDialog(void)        ;
void RangeStartDialog(void)        ;
void BuiltinScriptsDialog(void)    ; 
void ExternalScriptDialog(void)    ; 
void KludgesDialog(void)           ;  
void RangeControlDialog(void)      ; 
void FocusOptionsDialog(void)      ; 
void FocusParamDialog(void)        ; 
void AtomMarkerDialog(void)        ;  
void RibbonDialog(void)            ; 
void RunConditionsDialog(void)     ;  
void ColorScaleDialog(void)        ;

void EndInitDialog(void)     ;
void EndReportDialog(void)     ;
void EndRangeStartDialog(void)     ;
void EndBuiltinScriptsDialog(void) ; 
void EndExternalScriptDialog(void) ; 
void EndKludgesDialog(void)        ;  
void EndRangeControlDialog(void)   ; 
void EndFocusOptionsDialog(void)   ; 
void EndFocusParamDialog(void)     ; 
void EndAtomMarkerDialog(void)     ;  
void EndRibbonDialog(void)         ; 
void EndRunConditionsDialog(void)  ;  
void EndColorScaleDialog(void)     ;

#ifndef UNIX_X11

#define DLOG_OK                  1
#define DLOG_CANCEL              2
#define DLOG_OPTION              3
#define DLOG_SUBJECT             4
#define DLOG_INFO                5
#define DLOG_CHECK_CAPTION       6
#define DLOG_CHECKLAST_CAPTION   7
#define DLOG_RADIO_CAPTION       8
#define DLOG_PARAMA             10
#define DLOG_PARAMB             11
#define DLOG_PARAMC             12
#define DLOG_PARAMD             13
#define DLOG_PARAME             14
#define DLOG_PARAMF             15
#define DLOG_PARAMG             16
#define DLOG_PARAMH             17
#define DLOG_PARAMI             18
#define DLOG_TEXTA              20
#define DLOG_TEXTB              21
#define DLOG_TEXTC              22
#define DLOG_TEXTD              23
#define DLOG_TEXTE              24
#define DLOG_TEXTF              25
#define DLOG_TEXTG              26
#define DLOG_TEXTH              27
#define DLOG_TEXTI              28
#define DLOG_CHECKA             30
#define DLOG_CHECKB             31
#define DLOG_CHECKC             32
#define DLOG_CHECKD             33
#define DLOG_CHECKE             34
#define DLOG_CHECKF             35
#define DLOG_CHECKG             36
#define DLOG_CHECKH             37
#define DLOG_CHECKI             38
#define DLOG_CHECKJ             39
#define DLOG_CHECKK             40
#define DLOG_CHECKL             41
#define DLOG_CHECKM             42
#define DLOG_CHECKN             43
#define DLOG_CHECKO             44
#define DLOG_CHECKP             45
#define DLOG_CHECKQ             46
#define DLOG_CHECKR             47
#define DLOG_CHECKS             48
#define DLOG_CHECKT             49
#define DLOG_CHECKU             66 /*020304*/
#define DLOG_CHECKV             67 /*041102*/
#define DLOG_CHECKW             68 /*041102*/
#define DLOG_CHECKX             69 /*041102*/
#define DLOG_RADIOA             50
#define DLOG_RADIOB             51
#define DLOG_RADIOC             52
#define DLOG_RADIOD             53
#define DLOG_RADIOE             54
#define DLOG_RADIOF             55
#define DLOG_RADIOG             56
#define DLOG_RADIOH             57
#define DLOG_RADIOI             58
#define DLOG_RADIOJ             59
#define DLOG_RADIOK             60
#define DLOG_RADIOL             61
#define DLOG_RADIOM             62
#define DLOG_RADION             63
#define DLOG_RADIOO             64
#define DLOG_RADIOP             65

#endif /*not UNIX_X11*/

#ifdef UNIX_X11
  EXTERNDLOG  Widget genericdialogBox;
  EXTERNDLOG  Widget DLOG_OK, DLOG_CANCEL, DLOG_OPTION;
  EXTERNDLOG  Widget DLOG_PARAMA, DLOG_PARAMB, DLOG_PARAMC, DLOG_PARAMD;
  EXTERNDLOG  Widget DLOG_PARAME, DLOG_PARAMF, DLOG_PARAMG, DLOG_PARAMH;
  EXTERNDLOG  Widget DLOG_PARAMI;
  EXTERNDLOG  Widget DLOG_TEXTA, DLOG_TEXTB, DLOG_TEXTC, DLOG_TEXTD;
  EXTERNDLOG  Widget DLOG_TEXTE, DLOG_TEXTF, DLOG_TEXTG, DLOG_TEXTH;
  EXTERNDLOG  Widget DLOG_TEXTI;
  EXTERNDLOG  Widget DLOG_SUBJECT, DLOG_INFO;
  EXTERNDLOG  Widget DLOG_CHECK_CAPTION, DLOG_RADIO_CAPTION;
  EXTERNDLOG  Widget DLOG_CHECKLAST_CAPTION;
  EXTERNDLOG  Widget DLOG_CHECKA, DLOG_CHECKB, DLOG_CHECKC, DLOG_CHECKD;
  EXTERNDLOG  Widget DLOG_CHECKE, DLOG_CHECKF, DLOG_CHECKG, DLOG_CHECKH;
  EXTERNDLOG  Widget DLOG_CHECKI, DLOG_CHECKJ, DLOG_CHECKK, DLOG_CHECKL;
  EXTERNDLOG  Widget DLOG_CHECKM, DLOG_CHECKN, DLOG_CHECKO, DLOG_CHECKP;
  EXTERNDLOG  Widget DLOG_CHECKQ, DLOG_CHECKR, DLOG_CHECKS, DLOG_CHECKT;
  EXTERNDLOG  Widget DLOG_CHECKU, DLOG_CHECKV, DLOG_CHECKW, DLOG_CHECKX;
  EXTERNDLOG  Widget DLOG_RADIOA, DLOG_RADIOB, DLOG_RADIOC, DLOG_RADIOD;
  EXTERNDLOG  Widget DLOG_RADIOE, DLOG_RADIOF, DLOG_RADIOG, DLOG_RADIOH;
  EXTERNDLOG  Widget DLOG_RADIOI, DLOG_RADIOJ, DLOG_RADIOK, DLOG_RADIOL;
  EXTERNDLOG  Widget DLOG_RADIOM, DLOG_RADION, DLOG_RADIOO, DLOG_RADIOP;
  EXTERNDLOG  Widget genericform, genericbox, generictmp; /*this 030228*/
  EXTERNDLOG  Widget genericrowA, genericrowB, genericrowC;
  EXTERNDLOG  Widget genericrowchecks, genericrowradios, genericbuttons;
  EXTERNDLOG  Widget genericcheckradiorow, genericcheckcol, genericradiocol;

XtCallbackProc fileok_CB(Widget, XtPointer, XtPointer);
XtCallbackProc filecancel_CB(Widget, XtPointer, XtPointer);
XtCallbackProc open_CB(Widget, XtPointer, XtPointer);
XtCallbackProc newpass_CB(Widget, XtPointer, XtPointer);
XtCallbackProc quit_CB(Widget, XtPointer, XtPointer);
XtCallbackProc dummy_CB(Widget, XtPointer, XtPointer);
XtCallbackProc about_CB(Widget, XtPointer, XtPointer);
XtCallbackProc launchabout_CB(Widget, XtPointer, XtPointer);
XtCallbackProc launchcurent_CB(Widget, XtPointer, XtPointer);
XtCallbackProc launchdifferent_CB(Widget, XtPointer, XtPointer);
XtCallbackProc optionsabout_CB(Widget, XtPointer, XtPointer);
XtCallbackProc mutrot_CB(Widget, XtPointer, XtPointer);
XtCallbackProc makehelphtml_CB(Widget, XtPointer, XtPointer); /*040425*/
XtCallbackProc fileout_OK_CB(Widget, XtPointer, XtPointer);
XtCallbackProc CANCEL_CB(Widget, XtPointer, XtPointer);
XtCallbackProc scriptfile_OK_CB(Widget, XtPointer, XtPointer);
XtCallbackProc helpfile_OK_CB(Widget, XtPointer, XtPointer); /*040425*/
EXTERNDLOG XtPointer xtptr;
EXTERNDLOG int intflag;


#endif /*UNIX_X11*/

EXTERNDLOG  int   ACTIVE_DLOG;
EXTERNDLOG  int   Lwaiting;

#define INIT_DLOG            1
#define REPORT_DLOG          2
#define RANGESTART_DLOG      3
#define BUILTINSCRIPTS_DLOG  4
#define EXTERNALSCRIPT_DLOG  5
#define KLUDGES_DLOG         6
#define RANGECONTROL_DLOG    7
#define RUNCONDITIONS_DLOG   8
#define ATOMMARKER_DLOG      9
#define FOCUSOPTIONS_DLOG   10
#define FOCUSPARAM_DLOG     11
#define FOCUSBYRESNUM_DLOG  12
#define RIBBON_DLOG         13
#define COLORSCALE_DLOG     14

EXTERNDLOG  int   Ldlog_OKHIT;
EXTERNDLOG  int   Ldlog_optionHIT;
EXTERNDLOG  int   Ldlog_cancelHIT;
EXTERNDLOG  int   Ldlog_OK;
EXTERNDLOG  int   Ldlog_option;
EXTERNDLOG  int   Ldlog_cancel;
EXTERNDLOG  int   Ldlog_subject;
EXTERNDLOG  int   Ldlog_paramA;
EXTERNDLOG  int   Ldlog_textA;
EXTERNDLOG  int   Ldlog_paramB;
EXTERNDLOG  int   Ldlog_textB;
EXTERNDLOG  int   Ldlog_paramC;
EXTERNDLOG  int   Ldlog_textC;
EXTERNDLOG  int   Ldlog_paramD;
EXTERNDLOG  int   Ldlog_textD;
EXTERNDLOG  int   Ldlog_paramE;
EXTERNDLOG  int   Ldlog_textE;
EXTERNDLOG  int   Ldlog_paramF;
EXTERNDLOG  int   Ldlog_textF;
EXTERNDLOG  int   Ldlog_paramG;
EXTERNDLOG  int   Ldlog_textG;
EXTERNDLOG  int   Ldlog_paramH;
EXTERNDLOG  int   Ldlog_textH;
EXTERNDLOG  int   Ldlog_paramI;
EXTERNDLOG  int   Ldlog_textI;
EXTERNDLOG  int   Ldlog_info;
EXTERNDLOG  int   Ldlog_check_caption;
EXTERNDLOG  int   Ldlog_checkLast_caption;
EXTERNDLOG  int   Ldlog_checkA, Ldlog_checkB, Ldlog_checkC, Ldlog_checkD;
EXTERNDLOG  int   Ldlog_checkE, Ldlog_checkF, Ldlog_checkG, Ldlog_checkH;
EXTERNDLOG  int   Ldlog_checkI, Ldlog_checkJ, Ldlog_checkK, Ldlog_checkL;
EXTERNDLOG  int   Ldlog_checkM, Ldlog_checkN, Ldlog_checkO, Ldlog_checkP;
EXTERNDLOG  int   Ldlog_checkQ, Ldlog_checkR, Ldlog_checkS, Ldlog_checkT;
EXTERNDLOG  int   Ldlog_checkU, Ldlog_checkV, Ldlog_checkW, Ldlog_checkX;
EXTERNDLOG  int   Ldlog_radio_caption;
EXTERNDLOG  int   Ldlog_radioA, Ldlog_radioB, Ldlog_radioC, Ldlog_radioD;
EXTERNDLOG  int   Ldlog_radioE, Ldlog_radioF, Ldlog_radioG, Ldlog_radioH;
EXTERNDLOG  int   Ldlog_radioI, Ldlog_radioJ, Ldlog_radioK, Ldlog_radioL;
EXTERNDLOG  int   Ldlog_radioM, Ldlog_radioN, Ldlog_radioO, Ldlog_radioP;

EXTERNDLOG  char  dlog_OKstr[32],dlog_cancelstr[32];
EXTERNDLOG  char  dlog_paramAstr[32],dlog_paramBstr[32],dlog_paramCstr[32];
EXTERNDLOG  char  dlog_paramDstr[32],dlog_paramEstr[32],dlog_paramFstr[32];
EXTERNDLOG  char  dlog_paramGstr[32],dlog_paramHstr[32],dlog_paramIstr[32];
EXTERNDLOG  char  dlog_textAstr[128], dlog_textBstr[128], dlog_textCstr[128];
EXTERNDLOG  char  dlog_textDstr[128], dlog_textEstr[128], dlog_textFstr[128];
EXTERNDLOG  char  dlog_textGstr[128], dlog_textHstr[128], dlog_textIstr[128];
EXTERNDLOG  char  dlog_subjectstr[256],  dlog_infostr[256],dlog_optionstr[32];
EXTERNDLOG  char  dlog_checkcaptionstr[256],dlog_radiocaptionstr[256];
EXTERNDLOG  char  dlog_checkLastcaptionstr[256];
EXTERNDLOG  char  dlog_checkAstr[96],dlog_checkBstr[96],dlog_checkCstr[96];
EXTERNDLOG  char  dlog_checkDstr[96],dlog_checkEstr[96],dlog_checkFstr[96];
EXTERNDLOG  char  dlog_checkGstr[96],dlog_checkHstr[96],dlog_checkIstr[96];
EXTERNDLOG  char  dlog_checkJstr[96],dlog_checkKstr[96],dlog_checkLstr[96];
EXTERNDLOG  char  dlog_checkMstr[96],dlog_checkNstr[96],dlog_checkOstr[96];
EXTERNDLOG  char  dlog_checkPstr[96],dlog_checkQstr[96],dlog_checkRstr[96];
EXTERNDLOG  char  dlog_checkSstr[96],dlog_checkTstr[96],dlog_checkUstr[96];
EXTERNDLOG  char  dlog_checkVstr[96],dlog_checkWstr[96],dlog_checkXstr[96];
EXTERNDLOG  int   dlog_checkAint,dlog_checkBint,dlog_checkCint,dlog_checkDint;
EXTERNDLOG  int   dlog_checkEint,dlog_checkFint,dlog_checkGint,dlog_checkHint;
EXTERNDLOG  int   dlog_checkIint,dlog_checkJint,dlog_checkKint,dlog_checkLint;
EXTERNDLOG  int   dlog_checkMint,dlog_checkNint,dlog_checkOint,dlog_checkPint;
EXTERNDLOG  int   dlog_checkQint,dlog_checkRint,dlog_checkSint,dlog_checkTint;
EXTERNDLOG  int   dlog_checkUint,dlog_checkVint,dlog_checkWint,dlog_checkXint;
EXTERNDLOG  char  dlog_radioAstr[96],dlog_radioBstr[96],dlog_radioCstr[96];
EXTERNDLOG  char  dlog_radioDstr[96],dlog_radioEstr[96],dlog_radioFstr[96];
EXTERNDLOG  char  dlog_radioGstr[96],dlog_radioHstr[96],dlog_radioIstr[96];
EXTERNDLOG  char  dlog_radioJstr[96],dlog_radioKstr[96],dlog_radioLstr[96];
EXTERNDLOG  char  dlog_radioMstr[96],dlog_radioNstr[96],dlog_radioOstr[96];
EXTERNDLOG  char  dlog_radioPstr[96];
EXTERNDLOG  int   dlog_radioAint,dlog_radioBint,dlog_radioCint,dlog_radioDint;
EXTERNDLOG  int   dlog_radioEint,dlog_radioFint,dlog_radioGint,dlog_radioHint;
EXTERNDLOG  int   dlog_radioIint,dlog_radioJint,dlog_radioKint,dlog_radioLint;
EXTERNDLOG  int   dlog_radioMint,dlog_radioNint,dlog_radioOint,dlog_radioPint;


