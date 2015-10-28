/*PKINANGL*/
void idealizedCalphaAngles(char[4],float*,float*,float*,float*,float*,float*);
int     constructbeta(float*,char[4],float*,float*,char*);
int     constructgamma(float*,char,char);

void    constructfourth(float (*)[3],float,float,float);


void construct4th(float[4],float[4],float[4],float[4],float,float,float);

void perpendicular(float[4],float[4],float[4],float[4]);

void    docrossproduct( double,double,double
                       ,double,double,double
                       ,double*,double*,double*);

void crossproduct(double ,double ,double ,double ,double ,double );
void   crossproduct3array(double*,double*,double*); /*040705*/
double dotproduct(double ,double ,double ,double ,double ,double );
double dotproduct3array(double* ,double*); /*050403*/
double dihedral4pt( double ,double ,double 
                   ,double ,double ,double 
                   ,double ,double ,double 
                   ,double ,double ,double );
double    angle3pt( double ,double ,double 
                   ,double ,double ,double 
                   ,double ,double ,double );

void     doaxisrot( double *,double *,double *,float
                   ,double,double,double,double,double,double);

int  constructcircle(float C[12][3], float A[3], float B[3], float r);

/*PKINFILE*/
void    openafile(char[256] ,int);
void    selectfiles(void);
void    prepareinputfile(void);
void    prepareoutputfile(void);
void    makeoutputfilename(void);
/*____FILE*/
int    getfilename(char[256], int);
int    openscriptin(void);
/*int    openhelpout(void); prototyped under ____OUTP*/
int  getnewfile(char[256], int); /*moved here from PC & Mac 061020*/

/*PKININIT*/
void    mainlooprouter(void); /*990402 common routing in mainloop*/
void    getversion(void);
void    initialvalues(void);
void    aboutprekin(void);
void    doexplanation(void);
void	dissectcommandline(char*);
void	parsecommandline(int*, char**);
void    writehelptohtmlfile(int); /*040425*/

/*PKINMENU*/
void menuabout(void);
void aboutlaunching(void);
void aboutoptions(void);
int  menuaboutmutations(int); /*040425*/
/*____MENU*/
void   launchcurrentkinemage(void);

/*PKININPT*/
void   textstringin(void);
int    Getonescriptline(void);
int    Getoneinputline(void);
int    Getonescratchline(void);
int    Getoneaalibline(void);
int    Getoneline(FILE *);
int    pdbInOutprocessing(char[256]); /*040304*/
void   rotatepdbatomrecord(char[256], float[10], float[4]); /*040304*/

/*PKINRSUB*/
void   cleartexts(void);
void   setuptexts(void);
void   cleanuptexts(void);
int    dot3numbertexts(void);
void   cleanupfoveatexts(void);
void   cleanupmarkertexts(void);

/*PKINUTIL*/
int      CompArgStr(char*, char*, int); 
float    floatfromstr(char[256]);
int      intfromstr(char[256]);
int  matrixinvert(int);
void DoPerpendicularToPlane(float[3][7]);
void DoPerpendicularToLine(float[3], float[3], float[3], float[3]); /*050125*/

/*PKINHBND*/
void   getmchbatoms(void);
void   storemchbatoms(void);
void   connecthbonds(void);

/*PKINRIBN*/
int    getribbonatoms(void); /*make int instead of void 010511*/
void    ribmak(void);
void outputinterval(int*,int,int[5],int,int,float,int
  ,float[5][4],float[5][4],char[5][4],char[5][2],char[5][2],float[5]
  ,float[5][4],float[5][4],int[5],float[5],float[5]
  ,float[5],float[5],float[5],float[5]); /*omega 041023 */


/*PKINSPOS*/
int   superpos(void);

/*PKINRNGE*/
void getranges(void); 
void finishranges(void);
void rangestartinitialize(void);
void builtinscripts(void);

void scriptout(void);
void startarange(void);
void endarange(void);
void getatommarker(void);
void focusoption(void);
void focusoptionend(void);

/*PKINCRTL*/
void flowControl(void);

/*PKINCSUB*/
void   setsubunitconditions(void);
void   rangecontrol(void);
int    checkline(char[256]);
void   evalnonatmrecord(void);
void   convertcase(char[256]);
void   readbrkatmrecord(void);
void   evalatomrecord(void);
void   screenaltconf(void);
void   checkfocus(void);
void   connecthydrogen(void);
void   connecthydrogenbyname(void);/*130318 supersede connecthydrogen(by dist)*/
void   connectheavyatom(int,int);
void   storecyssg(int);
void   connecthetatms(void);
void   connectcaca(void);
int    storeconnectingatom(int,int); /*rtn int and 2nd int arg 061001*/
int    notawater(void);  /* res[MAXARR][4] && called with maxatm == 1 */
int    isawater(void);  /* res[MAXARR][4] && called with maxatm == 1 */
int    isahydrogen(char[7],char[6],char[3]); /*revised pdb atom names 070520*/
int    isahydrogenname(char[6]); /*revised pdb atom names 071222*/
int    iselement(char[3],char[3]);  /*070520*/
void   whatelement(char[7],char[6],char[3]);  /*int 070520, void 070730*/
void   connectssbonds(void);
void   getresiduename(void);
void   readbrkaalibrecord(char[256]);
void   mutateresidue(void);
int    getinteger(int,int,int*);
int    getbracketed(char,int,int,char[256]);
void   accumsiteranges(char[256]);
void   computesiteranges(void);

/*PKINCSBS*/
float  distan(double,double,double,double,double,double);
float  distanfa4(float[4],float[4]);
void   uctolctexts(char[256]);
char   lctouc(char);
char   uctolc(char);
void   oldcon(int, float, int);
void   tacker(int,int);
void   muttacker(int,int);

void   sstapler(int,int);
int    creatm(int);
void   mutadder(int,int);
int    screenatompair(int);
int    screenhetpairs(int);
int    decidemainside(int);
int    decidemutmainside(int);
void   writescratch(int);
void   writeatomscratch(int);
void   writelabelscratch(void);
void   pperptobase(int);  /*030515*/
int    pperpoutlier(float, float); /*040705*/
int    epsilonoutlier(float); /*070521*/
void   drawclaw(float*, float*, float*, char*, char*); /*040705, 070731 colr*/
void   Bcoloredoutput(char* color, float Avalue); /*separate routine 050704*/
void   makemetalsymbol(void);

/*PKINROTL*/
void   getresiduerotations(void);
char   getresiduecode(char, char, char); /*returns 1-letter code*/
float  findrotationangle(char[5][6]);
void   findrotationaxis(int, int, char[5][6]);
void   findrotationriders(int, int, char[5][6]);
void   writescrotscratch(void); 
void   rotamerhypertext(char[4]);
void   mutatepdbrecords(int[2],float,float,double[3],double[3],double[3]);
int    getoldlibraryline(int,int); /*071215 was getonelibraryline()*/
int    getnewlibraryline(int,int); /*071215*/
int    getonekinemageline(int);
int    getonerotamerline(int);

/*PKINCOUT*/
void   transferout(char[13],char[5],char[20],char[20]);
void   writeoutput(void);
void writeVRMLvectortoscratch(char[5],char[16],float,char[32],float,float,float,float,float,float); /*050211*/

/*PKINSCRT*/
int    countpoints(char[5], int);
void   inittextblock(textblock*);
void   rewindtextblock(textblock*);
void   buildtextblock(textblock*, size_t);
void   disposetextblock(textblock*);
void   putonetextblockline(textblock*, char*);
void   getonetextblockline(textblock*, char*);

/*____TEXT*/
void    pkintextinsert(char[256]);
void    inserttexts(char[256], int);
void    pkintextreplace(char[256]);
void    adjusttext(int );
void    cleanuptextwindow(void);

/*____MAIN*/
void    HandleEvent(void);
void    makecursorwait(void);
void    makecursoractive(void);

/*____OUTP*/
int    openoutput(void);
int    openscriptout(void);
int    openhelpout(void);

/*____DLOG*/
void DogenericDialog();

/*PKPCINIT or PKMCINIT or PUXMOSX,PUXMLNX,PUXMSGI*/ 
char* operatingsystem(void); /*051201*/

/*general declarations 130121 etc... copied here... */
void    mainsetup(int *, char **); /*130121*/
void    getmactextselect(void); /*130121*/
void    AdjustMenus(void);     /*130121*/
int     getkeydownstate(void); /*130121*/
long    TickCount(void);       /*130121*/



#ifdef MACINTOSH
void    waste(long); /*for number of seconds as originally written*/
/*PKMCFILE*/


/*static void  ConvertFilename(FSSpec *) ;*/
void  ConvertFilename(FSSpec *) ;
#ifdef SYSTEM6
  int    ifilelaunched(void);
  void   getnamestroffile(void);
#endif /*SYSTEM6*/

/*PKMCINIT*/
/*void    mainsetup(void); 130121 declare this outside of ifdef MACINTOSH*/
void    beginnings(void);
void    InitMacintosh(void);
void    setdemo(void);
void    SetUptextWindow(void);
void    updatethetextwindow(void);
pascal void  textScroller(ControlHandle , short);
void    DotextContentClick(Point );
void    settextScrollBar(void);
void    gettextTextarea(void);
void    settextTextarea(void);

/* Apple Event Handler Prototypes! */
pascal OSErr HandleAEOpenDoc( AppleEvent*, AppleEvent*, long); /*980917*/
pascal OSErr HandleAEQuitApp( AppleEvent*, AppleEvent*, long); /*980917*/
pascal OSErr HandleAEIgnore(  AppleEvent*, AppleEvent*, long); /*980917*/

/*pascal OSErr HandleAEOpenDoc(AppleEvent, AppleEvent, long );*/
/*pascal OSErr HandleAEQuitApp(AppleEvent, AppleEvent, long );*/
/*pascal OSErr HandleAEIgnore(AppleEvent, AppleEvent, long ); */

pascal OSErr HandleAECommandLine(AppleEvent*, AppleEvent*, long);/*971223*/

void DoINITreport(void);
void INITreport(void);
pascal Boolean  reportFilter(DialogPtr, EventRecord *, int *);

/*PKMCMAIN*/
void    main(void);
void    HandleMouseDown(EventRecord *);
void    cursorstandard(void);
void    cursorshowwait(void);
int     HandleloopEvent(void);
/*int getkeydownstate(void);130121 declare this outside of ifdef MACINTOSH*/

/*PKMCMENU*/
OSErr  FindAProcess(OSType,OSType,ProcessSerialNumber*);
OSErr  OpenSelection(FSSpecPtr);
void   SetUpMenus(void);
/*void   AdjustMenus(void);130121 declare this outside of ifdef MACINTOSH*/
void   enable(MenuHandle , int , int );
int    exec(char *, ...);
void   HandleMenu (long );

/*PKMCTEXT*/
/*void getmactextselect(void); 130121 declare this outside of ifdef MACINTOSH*/

#endif /*MACINTOSH*/


#ifdef PCWINDOWS

/* PKPCMAIN.C */
int  WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
/*long TickCount(void);130121 declare this outside of ifdef...*/

char* dirstrip(char[256]);
/*int  getkeydownstate(void);130121 declare this outside of ifdef...*/

/* PKPCINIT.C */
void beginnings(void);
BOOL InitInstance(HINSTANCE, int);

/* PKPCFILE.C */
int  getoutfilename(char[256], int);
void SetPrekinCaption(void);

/* routines to handle menu choices */
int getnextchar(void);
void PKIN_FileOpenFile(void);
void PKIN_FileQuit(void);
void PKIN_NewPass(void);

/* PKPCTEXT.C (text.c) */
/*void getmactextselect(void);130121 declare this outside of ifdef...*/
void SetTextCaption(HWND, LPSTR);
void SystemFontSize(void);
long totalbufferlines(HWND, HDC, LPSTR);
long linefrooffest(HWND, unsigned long*, long);
long makelinestarts(HWND);
int  linesonascreen(HWND);
void resetscrollbarvalues(HWND);
int  RePaint(HWND, HDC);
int  nlinebreaks(LPSTR, LPSTR*);
int  sizeoftextline(HDC, int, char*);
void completetext(void);

/* PKPCMENU.C (pkmenu.c) */
/*void AdjustMenus(void);130121 declare this outside of ifdef...*/
void checkamenuitem(int, int);
void enableamenuitem(int, int);
void openfilebyname(LPSTR);
void findpdb(void);
void openpdbfile(LPSTR);
int  openkinfile(LPSTR);
void PKIN_FileNewPass(void);
void PKIN_FileQuit(void);
void PKIN_OptionsAboutOptions(void);
void PKIN_OptionsMutations(void);

/*PKPCDLOG.C*/
void adjustgenericradios(char);
BOOL CALLBACK genericDialog(HWND, UINT, WPARAM, LPARAM);

#endif /*PCWINDOWS*/
