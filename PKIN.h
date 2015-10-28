/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*PREKIN.H declarations for all prekin files: */
/*   invokes include PREKIN.HDR with subroutine and function prototypes*/
/*  main defines EXTERN as nothing ( "" ), so really does the declarations */
/* if EXTERN not defined, then it is defined as "extern"  so only referenced*/
/*The specific compiler on each computer defines its own name control */
/*e.g. THINK_C defined: MACINTOSH, __BORLANDC__ defined: PCWINDOWS MSWINDOWS*/

/* The Weiss Borland Workshop resource compiler chokes on*/
/* function declarations; thus all function declarations are kept in */
/* PREKIN.HDR (this is a useful distinction anyway) */
/* Furthermore, the Richardson Borland Workshop can't find standard */
/* includes; so all variables needed for menus and dialogs are kept */
/* in PKINMENU.h, IPCWPKIN.RC just #includes PKINMENU.h */

/* common include files: */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*#define GFPchromophore 070731 sequential atoms not in original residue order*/
#define GFPchromophore

#define UNIX_X11
#define UNIX_MOTIF

#ifdef NOGUI     /*for cmdline MolProbity to avoid needing MOTIF libraries*/
#undef UNIX_MOTIF
#endif /*NOGUI*/ /*NOGUI set in Makefile with CFLAGS = -D NOGUI ... */

#ifdef __MWERKS__            /*__MWERKS__ compiler specific*/
#define MACINTOSH
#define POWERMAC     /*MetroWerks defines __POWERPC__  __CFM68K__  */
#undef UNIX_X11
#undef UNIX_MOTIF
#undef MSWINDOWS
#undef PCWINDOWS
#define screenBits qd.screenBits
#include <math.h>
#include <Palettes.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <Balloons.h>

#include <Dialogs.h>
#include <Controls.h>
#include <MacMemory.h>
#include <Menus.h>
#include <MacTypes.h>
#endif /*__MWERKS__*/

#ifdef OLD__MWERKS__         /*__MWERKS__ compiler specific*/
/*#include <MacHeaders>*/
#include <stdio.h>

#include <Gestalt.h>
#include <AppleEvents.h>
#include <Quickdraw.h>
#include <Memory.h>
#include <Fonts.h>
#include <Events.h>

#include <Windows.h>
#include <OSUtils.h>
#include <ToolUtils.h>
#include <SegLoad.h>


#include <Dialogs.h>
#include <Types.h>
#include <TextEdit.h>
#endif /*OLD__MWERKS__*/



#ifdef THINK_C
#undef UNIX_X11
#undef UNIX_MOTIF
#define MACINTOSH
#include <MacHeaders>
#include <GestaltEqu.h>
#include <AppleEvents.h>
#endif

#ifdef _WIN32
#undef UNIX_X11
#undef UNIX_MOTIF
#define MSWINDOWS
#define PCWINDOWS
#endif


#ifdef EXTERN
#undef EXTERN
#define EXTERN
#define MAGE_INIT
#else
#define EXTERN extern
#endif

#ifdef PCWINDOWS
#define STRICT
#include <afxwin.h>
#include <afxext.h>
#include <time.h>
#include <io.h>
#include <fcntl.h>

#ifndef _MAX_PATH
#define _MAX_PATH 80
#endif
#define CRLF "\r"
#define EOLO "\r\n"

#define EOL  '\r'
#endif

#ifdef MACINTOSH
#define CRLF "\r"
#define EOLO "\n"
#define EOL  '\r'
#endif

#ifdef UNIX_X11
#define CRLF "\n"
#define EOLO "\n"
#define EOL  '\n'
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#endif  /*UNIX_X11*/

#ifdef UNIX_MOTIF
#include <Xm/Xm.h> /*grafwindow*/
#include <Xm/Form.h> /*grafwindow*/
#include <Xm/DrawingA.h> /*actual drawing region*/
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h> /*menuBar widget*/
#include <Xm/CascadeB.h> /*for individual menus */
#include <Xm/MessageB.h>
#include <Xm/ToggleB.h> /*for button boxes on screen */
#include <Xm/ToggleBG.h> /*for button boxes on screen */
#include <Xm/Text.h>
#include <Xm/FileSB.h>
#include <Xm/ScrolledW.h>
#include <Xm/ScrollBar.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/List.h>
#endif /*UNIX_MOTIF*/

#ifdef UNIX_X11
#define TRUE    1
#define FALSE   0
typedef Widget  MenuHandle;
typedef Widget  ControlHandle;
typedef Widget  WindowPtr;
typedef Cursor CursHandle;
#endif /*UNIX_X11*/

#ifdef PCWINDOWS
typedef HWND WindowPtr;
typedef HMENU MenuHandle;
typedef HCURSOR CursHandle;
#endif /*PCWINDOWS*/

#define  MAXARR   411  /*array limits number of atoms in a residue 990416*/
                        /*Lipid A with Hydrogens... arrays reused each residue*/
#define  MAXR     901 /*050309 array limits number of ranges, 101, 61, 101,...*/
                      /*050920 301 to 601 for 3PCC */
#define  MAXC    1024  /*array limits number of chains for ribbons 011030*/
/*MAXC up from 36 040324, up from 256 060120 match MAXlevels*/
#define  MAXANGLES  9  /*limit of ratatable bonds*/
#define  MAXmodchr  7  /*number of characters for an atoms mod indicator field*/
#define NMATRIX     9
                            /*mod field added to sub field  000418*/
#define  MAXchaincharcode  256  /*limit to number of chain ID's in one commandline run*/
#define  MAXmodelcharcode  256  /*limit to number of model ID's in one commandline run*/
#define  MAXlevels        1024  /*ribosome around 35 or so */
/*TB Glutamine Synthetase 1HTQ has 10 models of 24 chains with hets and water*/
/*uses 490 levels but still lumps all hets as one subgroup*/
/*MAXlevels only used for 2 int arrays and 3 char arrays so prekin can keep*/
/*track of what is in the pdb file and so user could specify partial output*/
/*--this provides selection while still keeping the old sequential pass mode*/
/*of the original prekin code*/
/*crystal symmetry can easily exceed 100 chains! 040324*/
/*so MAXchaincharcode,MAXmodelcharcode,MAXlevels changed from 64 ... */
#define  MAXaspects        32 /*color coded aspects for individual atoms*/
#define  MAXaspectchars    32 /*str length aspect labels*/
/* constants needed by Diamond's simple superposition subroutine*/
#ifndef PI
#define PI        3.141593
#endif /*PI*/
#define MAXPTS   50  /*maximum number of corresponding point pairs possible*/
#define GHOST      1 /*030920*/ /*040902 removed trailing ; */

#define MAXNAMECHAR 14 /*matches MAGE restriction for master, group, etc name*/

#define NCON      10000 /*number of CONECT atom pairs */ /*050409*/
  /*this should be a dynamically managed array...*/
EXTERN long iconect[NCON+1]; /*CONECT atom pairs as combined long integer*/
EXTERN int  ncon;
EXTERN int  Lconect; /*try to use CONECT records*/

/* external variables, all defined here so can find them.*/
EXTERN float PREKINVERSION;
EXTERN char  PREKINDATE[32];
EXTERN int IDEMO; /* IDEMO == 1 for big type, terse comments,*/
                  /* IDEMO == 0 for regular use prekin, set in main*/
EXTERN int IPC, IMAC, Lexplanation,Llaunched,Lbegin,Lbrowsedefault,Lindicator;
EXTERN double A[NMATRIX+1][NMATRIX+1];
EXTERN double C[NMATRIX+1][NMATRIX+1];
EXTERN int LAElaunched,Lappleevents; /*Mac AppleEvent flags*/
EXTERN int Lstartfile,Lcanlaunchmage;
EXTERN int Llaunchkinemage,Lskip,Llaunchcurrentkin,Llaunchdiffkin,Lmadekin;
EXTERN int Ldotsonly,Lballs,Lcrosses,Lvector,Lleftadjatomname;
EXTERN int Laccept,Lmodify,Llabellist,Ldotlist; /*script controls */
EXTERN int Lkluges,Lbuiltin,Lbuiltindialog,Lbuiltinchoice,Lexternal,ithscript;
EXTERN int Lcolorscale,Ncolorscalebasis,Leachsclist,Lballonscpoints,Lfillscrings;
EXTERN int Latomradii; /*030330*/
EXTERN float polarHradius; /*030331*/
EXTERN int Ncolorscale; /*010526*/
EXTERN int Lallsubunits,Lfirstsubunit,beginsubunit,endsubunit,Lonesubunit;
EXTERN int Lperiod,iscan,Lcbonly,Lhalfbonds;
EXTERN float fxmin,fxmax,fymin,fymax,fzmin,fzmax; /*050204*/
/*-----define constants: array sizes and distance checks */
EXTERN int    lastat,ssmax,hetmax;
EXTERN int    icount,numrotsc;
EXTERN int    level,irpass,ksave,levmsg;
EXTERN int    oldatm[2],oldata[2],oldatb[2],oldatc[2];
EXTERN char   nmrmodel[MAXmodchr]; /*use for mod[MAXARR][MAXmodchr] 000418*/
            /*set by MODEL record, so moves ahead at transition between models*/
EXTERN char   modelnumber[MAXmodchr]; /*model # of current coordinates 050121*/
EXTERN int    Lmodel,Nthmodel; /*existance of MODEL, which sequential MODEL*/
EXTERN char   FirstModel[MAXmodchr]; /*pass problem, remember actual 050923*/

/*range specificity */
EXTERN int    n,mrange,nrange,Linclusive,Loverlap;
EXTERN int    mcxl[MAXR],scxl[MAXR],hbxl[MAXR],hyxl[MAXR],caxl[MAXR];
EXTERN int    htxl[MAXR],waxl[MAXR],atxl[MAXR],lbxl[MAXR],mcal[MAXR];
EXTERN int    scal[MAXR],htal[MAXR],waal[MAXR],resl[MAXR],ribl[MAXR];

EXTERN int    ctxl[MAXR],mccl[MAXR],sccl[MAXR],htcl[MAXR];
    /*carbons as well as other atom markers*/
EXTERN int    mcCPKl[MAXR],scCPKl[MAXR],htCPKl[MAXR];
    /*CPK instead of vectors*/
EXTERN int    Bcol[MAXR],Bval[MAXR];
EXTERN int    Best[MAXR]; /*bestribbon for these general ranges 041103*/

EXTERN int    cisl[MAXR]; /*cis peptides Ca--Ca uses ribbon routines 041102*/
EXTERN int    resnu1[MAXR],resnu2[MAXR],cssl[MAXR],mutl[MAXR],xyzl[MAXR];
EXTERN char   insert1[MAXR],insert2[MAXR]; /*050924 residue insertion code*/
EXTERN int    Alpha[MAXR],Beta[MAXR],Coil[MAXR];/*explicit ribbon types 050206*/
EXTERN int    rotl[MAXR],supl[MAXR];
EXTERN int    sitel[MAXR]; /*050731 this range has site residues*/
    /*sitel[n] part of n from 1 to mranges selected in GUI or cmdline*/

    /*sitechain[j] part of j from 1 to Nsite gathered from Header 050928*/
EXTERN char   sitechain[MAXR][3],siteinsert[MAXR];/*050731,2charChainID 070926*/
EXTERN int    siteresnum[MAXR]; /*050731*/
EXTERN int    Lsite,Nsite; /*050731 honor site range restriction, num site res*/
EXTERN int    Lsitefake;/*050928 selected residue(s) act like site record ones*/

  /*ribchain[k][3] part of k from 1 to Nribs gathered from HELIX,SHEET records*/
    /*            or constructed from GUI ranges   050928 */
/*EXTERN char   ribchain[MAXR];*/ /*050924 this range applies to this chain ID*/
EXTERN char   ribchain[MAXR][3]; /*range for this chainID 2char chainID 070926*/
EXTERN char   ribinsert1[MAXR],ribinsert2[MAXR]; /* residue insertion code*/
EXTERN int    ribresnu1[MAXR],ribresnu2[MAXR]; /*1st,last res # of secstr rib*/
EXTERN int    ribAlpha[MAXR],ribBeta[MAXR],ribCoil[MAXR];/*HELIX,SHEET types*/
EXTERN int    Lribs,Nribs; /*050928 honor rib ranges, num of rib ranges*/
/*SHEET & HELIX record: cols 12 - 14  LString(3)  identifier. 060929*/
/* nothing defined for col 11, as of PDB format version 2.2 1996 */
EXTERN char   ribidentity[MAXR][5]; /*060929*/
EXTERN int    Lribidentity; /*SHEET (HELIX) identities, need colorsets 060929*/

EXTERN int    Lhylens;
EXTERN int    FOVEA,Latmark,Lresnumfovea,resnumfovea,Lfoundresnum;
EXTERN float  focus[4],focusca,focusmc,focussc,focuswa,focusht;
EXTERN float  focuscadefault,focusmcdefault,focusscdefault;
EXTERN float  focuswadefault,focushtdefault;
EXTERN char   focuschaincharcode[3]; /*011109,2char chainID 070926*/
EXTERN float  atmarker,ballradius;
EXTERN float  fxyz[MAXR][3];
EXTERN char   gflag[MAXlevels],resid[MAXR][33]; /*970419: 4->256*/
EXTERN int    Nchaincharcode; /*010425 treat as a varible rather than logical*/
EXTERN char   chaincharcode[MAXchaincharcode][3];/*010425,2char chainID 070926*/
EXTERN int    Nproteinchains, Nnucleicchains; /*041031*/
EXTERN int    Nmodelcharcode; /*010425*/
EXTERN char   modelcharcode[MAXmodelcharcode][3]; /*010425*/
EXTERN int    Lnosubgroupmasters; /*010426*/
EXTERN int    Lspeciallistmaster; /*040902*/
EXTERN char   speciallistmaster[MAXNAMECHAR+1]; /*040902*/
EXTERN int    Lspeciallistname; /*051129*/
EXTERN char   speciallistname[MAXNAMECHAR+1]; /*051129*/
/*-----local controls that may be needed in subroutines */
EXTERN int    m,nnow,maxatm,natom,inputflag,atomin;
/*--counters for number of main chain and side chain vectors, labels, etc.*/
EXTERN int    mccnt,sccnt,sscnt,ssknt,hcount,hetcnt,metcnt,hetknt,lbacnt;
EXTERN int    atmcnt,atncnt,atocnt,atscnt,atwcnt,atccnt;
EXTERN int    lbncnt,lbzcnt,vccnt,mutcnt,rotcnt;
EXTERN int  atpdbEOF,atarraymax,resinloop,finishres;

/* ribbon variables and NtoC coloring in general*/
EXTERN    int   nnew,canew,cnew,onew,oold,azimuthnew; /*041106 oold*/
EXTERN    int   cbnew,Lcbrib[5],Lcbstubsrib; /*041205*/
EXTERN    int   Lstub[5]; /*050731 ribbon residue can have cbstub */
EXTERN    int   Lcacbvector; /*050403*/
EXTERN    int   numrib[5],ribcnt,Lendribbon,Lautoribbon;
EXTERN    int   nstrnd,ichord,nrib,npept,nchord,nfrag,Lribbon,Lfudgedwidth;
EXTERN    int   Lribbonalso; /*050206 e.g. for sidechains on a ribbon*/
EXTERN    int   Lsmoothtwist,LNtermBetaFudge; /*LNtermBetaFudge 030706*/
EXTERN    float ribwidcoil,ribwidalpha,ribwidbeta,ribwidnucleic,ribwidVRML;
EXTERN    float npeprib[5][4],cpeprib[5][4],azimuth[5]; /*030127*/
                /*030127 NOTE carib[6] to hold one calc Calpha beyond end*/
EXTERN    float carib[6][4],orib[5][4],c3srib[5][4],c1srib[5][4],nuprib[5][4];
EXTERN    float o3srib[5][4]; /*040229*/
EXTERN    float c4srib[5][4]; /*040419*/
EXTERN    float cbrib[5][4]; /*041205*/
EXTERN    char  resrib[5][4],subrib[5][2],rinsrib[5][2],ribnam[10][13];
EXTERN    float Bvalrib[5]; /*021213*/
EXTERN    int LvectorCO,LazimuthCO,LphipsiCO,LpsiphiCO,LvectCOpdb,LpdbCACOpdb; 
EXTERN    int LfakeCaCaspline; /*111106*/
EXTERN    int   LvectorCOlabels,Ldumpcispeptides,Lcispeptides; /*041023,041102*/
EXTERN    int   LdumpVRML,LsplineVRML,LheaderVRML,LvectorVRML; /*050204,08*/
EXTERN    int   LsphereVRML,LarrowVRML; /*050208,12*/
EXTERN    int   LdoRNAribbon,LdoDNAribbon; /*060114*/
EXTERN    int   Lribonucleic,Lcrosstie,Ledgedribbon,Lskeinedribbon;
EXTERN    int   Lbetaarrows,Lonestrandcoil,onestrandcoilwidth;
EXTERN    int   Larrowhead; /*051210*/ /*for newribbon, skeined with arrows*/
EXTERN    int   Lbyresidue; /*051210*/ /*for newribbon, skeined with arrows*/
EXTERN    int   Lvariablewidthribbon; /*051208*/
EXTERN    int   Lexplicitribbon,explicitrib[5];
EXTERN    int   explicitribbontype; /*051208*/
EXTERN    int   Lexplicitribbonpossible[MAXC],Lbestribbon; /*011030*/
EXTERN    char  explicitribbonidentity[5]; /*SHEET record: sec str ID 060929*/
EXTERN    int   Ltable; /*991122*/
EXTERN    int   Lcolorbycurvature,LcolorbyNtoC,LcolorbyABC; /*010918*/
EXTERN    int   LcolorbyBval; /*021213*/
EXTERN    char  colorbyNtoC[32]; /*011109*/
EXTERN    int   Lpeptideperprib,Lnoface,Lnoedge; /*040319*/
EXTERN    int   Lblackedge; /*040924*/
EXTERN    int   LNOfudge,Lribbonguidepts; /*050327*/
EXTERN    int   Lribbonrawsplines,Lribbonaxisspline; /*051208,060115*/
EXTERN    int   Lsplinevector; /*140520*/
EXTERN    int   Lribbondensity; /*060115*/
EXTERN    int   Lribnhet; /*051214 distinguish RCSB PDB ribbon command */
    /* keep track of ribose c1*,c3*,p for nucleic acids */
EXTERN    int   c1snew,c3snew,nupnew,o3snew,c4snew; /*040229,040419*/
EXTERN    int   Ldoublenucleics; /*040419 double interval ribbons*/
EXTERN    int   Lcurvednucleics; /*040420 curvature offset ribbons*/
EXTERN    int   Lc4primenucleics; /*040420 curvature offset ribbons to c4prime*/
EXTERN    int   Lribbonnew;  /*051102*/
EXTERN    int   Lribbonhbonds;  /*051212*/
EXTERN    int   Lmagekinjas;  /*101225*/
#define ALPHA2D    1  
#define BETA2D     2
#define COIL2D     4
#define NUCLEIC2D  8
#define ALL2D      ( ALPHA2D | BETA2D | COIL2D )
#define ARROW2D   16
#define ALPHAARROW2D  17
#define BETAARROW2D   18 /*bit-wise logic imposed 061021*/
#define SEGMENTS   12   /*050204 for VRMLxSectionCircle */
/*------output controls for ribbons spanning all chains, MAXC max number of chains*/
EXTERN int    Nnucleic,Nprotein,Nhetgroup,Nwater,NDNA,NRNA,LRNA;
/*OLDPKINRIBNCODE*/
/*EXTERN char   chainIDlevel[MAXC][3]; unused?070926*/ /*2char chainID 070926*/
/*EXTERN int    nA[MAXC],kchainA,istartA[MAXR][MAXC],iendA[MAXR][MAXC];*/
/*EXTERN char   chainIDA[MAXC][3];unused?070926*/ /*2char chainID 070926*/
/*EXTERN char   istartinsertA[MAXR][MAXC],iendinsertA[MAXR][MAXC];*/
/*EXTERN int    nB[MAXC],kchainB,istartB[MAXR][MAXC],iendB[MAXR][MAXC];*/
/*EXTERN char   chainIDB[MAXC][3];unused?070926*/ /*2char chainID 070926*/
/*EXTERN char   istartinsertB[MAXR][MAXC],iendinsertB[MAXR][MAXC];*/

/*aspect controls and variables*/
EXTERN int  Lallowaspectsin,Laspectnc,Laspectabc; /*010918*/
EXTERN int  Naspectcode,MAXaspectcode,Ninternalaspects;
EXTERN int  aspectcodecol[MAXaspects+1];
EXTERN char aspectid[MAXaspects+1][MAXaspectchars+1];
EXTERN int  Ncolorabc,Naspectnc,Naspectabc;
EXTERN int  Lemphasis;

/*----- control flags */
EXTERN int  quiet,endfil,endsub,endchn,discon,endres,first1,morsub,stonex;
EXTERN int  delnum,delres,delnam,delsub;
EXTERN int  catrue;
EXTERN int  altstd,altcfa[2],altcfb[2],altcfc[2],Lalts;
EXTERN char altstr[5];
EXTERN int  Lpointmasteralta,Lpointmasteraltb,Lpointmasteraltc; /*030503*/
EXTERN int  Lribbonmasters; /*051128 for alpha,beta,coil  now optional*/
EXTERN int  Lca3ribbon;/*130121 rib plane use ca-1,ca,ca+1 perp instead of C=O*/

/*------control logicals, one for each type */
EXTERN int  mcoutl,scoutl,lboutl,croutl,htoutl,hyoutl;
EXTERN int  hboutl,cxoutl,caonly,caoutl,waoutl,atoutl,rangel;
EXTERN int  mcaoutl,scaoutl,htaoutl,waaoutl,ssscoutl,rotoutl,rotoutl2,mutoutl;
EXTERN int  numoutl,riboutl,extoutl,supoutl;
EXTERN int  mccoutl,sccoutl,htcoutl; /*960815*/
EXTERN int  mcCPKoutl,scCPKoutl,htCPKoutl; /*960815*/
EXTERN int  Bcoloutl,Bvaloutl; /*971213*/
EXTERN int  tabloutl; /*991122*/
EXTERN char branch;
EXTERN int  cbdevoutl; /*010918*/
EXTERN int  siteoutl; /*050731 this residue is in a site range*/

/*------output control logicals, one for each type of output */
EXTERN int    Listca,Listmc,Listsc,Listhy,Listhb,Listss,Listlb,Listht;
EXTERN int    Listwa,Listat,Listmcn,Listmco,Listmcs,Listscn;
EXTERN int    Listsco,Listscs,Listhtn,Listhto,Listhts,Listhtm,Listwao;
EXTERN int    Listnum,Listrib,Listlbn,Listmut,Listrot,Listrot2,Listext;
EXTERN int    Listmcc,Listscc,Listhtc;
EXTERN int    Listmcv,Listmck,Listscv,Listsck,Listhtv,Listhtk;
EXTERN int    Listmckc,Listmcko,Listmckn,Listmcks,Listmckh;
EXTERN int    Listsckc,Listscko,Listsckn,Listscks,Listsckh;
EXTERN int    Listhtkc,Listhtko,Listhtkn,Listhtks,Listhtkh;
EXTERN int    Listtabl;  /*991122*/
EXTERN int    Listscring; /*991128*/
EXTERN int    Listcbdev, Lcbetadev; /*keep noted cbdev==cbetadev 010918*/
EXTERN int    Listtaudev, Ltaudev; /*011024*/
EXTERN int    Ldump, Lcbdevdump,Lcbdevdumpmax; /*011027,040318*/
EXTERN int    Lconnectresiduesdump; /*061202*/
EXTERN float  cbetadevmax;
EXTERN int    Lsuperpos,Listsuppos; /*020214*/
EXTERN int    Lpdbmutout,Listpdb; /*040303*/
EXTERN int    Lpdbout,Lmatrix,Lrotcenter,LpdbIO,Lmutate; /*040304*/
EXTERN int    Ltranslate; /*050324*/
EXTERN float  matrix[10],rotcenter[4],translate[4]; /*040304,050324*/

/*------output color overrides, one for each type of output */
EXTERN char  colormcvec[128],colorscvec[128],colorcavec[128],colorhyvec[128];
EXTERN char  colorhtvec[128],colorhbvec[128];
EXTERN char  colorwavec[128],colorlbvec[128];
EXTERN char  colorextra[16]; /*041106*/
#define      MAXKEYWORD 64                /*041107*/
EXTERN char  passkeywordstr[MAXKEYWORD];  /*041107*/
EXTERN int   Lpasskeyword;                /*041107*/
/*------output ON/OFF overrides, one for each type of output */
EXTERN char offmc[5],offsc[5],offhy[5],offhb[5],offca[5],offlb[5],offht[5];
EXTERN char offwa[5];
/* e.g. offmc[ off\0] with leading space to separate it on list parameter line*/

/*------output alternate conformations overrides  */
EXTERN char  altcontrol[12];

/*-----counter info for constructing CHAOS files-----*/
#define   MAXTYPES   100
enum
{
typenull,
typemc,typemcn,typemco,typemcs,
typemcc,/*960809*/
typesc,typescn,typesco,typescs,
typescc,/*960809*/
typeca,typerib,
typelb,typelbn,
typehb,
typehy,
typess,
typeht,typehtn,typehto,typehts,typehtm,
typehtc,
typewa,typewao,typeext,
typecbda,typecbdb,typecbdc,typecbdm,typecbdv,typecbdl,typecbde,
typetau,typetaub,typetaus,
typero
};
/* typero...digit,blank AND typero...digit,h  PKINROTL.c*/
/* typerib ... rb  , rbae, rbbe, rbne, rbce, rbaf, rbbf, rbnf  PKINRIBN.c*/
/* typehy... scv1, scv2, hysc, */
/* typesc... sc 1, sc 2, scv1, scv2, sckh, scks, */
/* typeht... htv , htkc, htko, htkn, htkh, htks, */
/* typehy... hywa, hymc, mckh, hysc, sckh*/
/* typewa... wa  , */
/* typeca... tabl, ca  , */
/* typemc... mc  , mcv , mckc, mcko, mckn, mcks, */
EXTERN int    countxyz,lastsc,typenow, pointcnt[MAXTYPES],ps300color;
EXTERN int    ntypes,cntn[MAXTYPES];  /*011202*/
EXTERN char   cntp[MAXTYPES][5];      /*011202*/

EXTERN float  ps300saturation;
EXTERN float  ssdist,bondst,cadist;
EXTERN char  oldsub[5],nexsub[5],oldnam[7],althed[2]; /*sub[3] 070926*/
EXTERN char  oldmod[MAXmodchr],nexmod[MAXmodchr];
EXTERN char  topID[MAXmodchr+2];
EXTERN char  ChainID[MAXlevels][3]; /*051128, 2char 070926*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*residue's atom records inc mod[][MAXmodchr] for nmrmodel number str 0-99 */
EXTERN char  mod[MAXARR][MAXmodchr]; /*000418*/
EXTERN char  atmcpy[6],name[MAXARR][7];
             /*070731 fix format 1st 15 char of pointID==PDB %5s%3s %1s%4d%1s*/
             /* atom, res, sub, num, rins : NB atom incl alt as 5th character*/
             /* %5s%3s%2s%4d%1s sub[][3] 070926*/
EXTERN char  atom[MAXARR][6],res[MAXARR][4],sub[MAXARR][5],rins[MAXARR][2];
EXTERN char  segid[MAXARR][5]; /*130507 JJH*/
EXTERN char  Anum[MAXARR][6]; /*Hybrid36 071001, replace int ncnt[MAXARR]*/
EXTERN int   num[MAXARR],base[MAXARR],idupl[MAXARR],isc[MAXARR];
EXTERN int   nextatm[MAXARR],atstyle[MAXARR]; /*atstyle, e.g. ghost 030920*/
EXTERN float x[MAXARR],y[MAXARR],z[MAXARR],o[MAXARR],B[MAXARR],Uval[MAXARR];
EXTERN char  element[MAXARR][3]; /*070520*/
EXTERN char  aspectstr[MAXARR][MAXaspects+3]; /*in parens: (XXXX)*/

/*mutant library residue's atom records*/
EXTERN char  mutmod[MAXARR][MAXmodchr]; /*000418*/
EXTERN char  mutatmcpy[6],mutname[MAXARR][7],rescode,mutrins[MAXARR][2];
EXTERN char  mutatom[MAXARR][6],mutres[MAXARR][4],mutsub[MAXARR][5]; /*070926*/
EXTERN char  mutAnum[MAXARR][6];/*Hybrid36 071001, replace int mutncnt[MAXARR]*/
EXTERN int   mutnum[MAXARR],mutbase[MAXARR];
EXTERN int   maxmut,nn;
EXTERN float mutx[MAXARR],muty[MAXARR],mutz[MAXARR];
EXTERN char  mutelement[MAXARR][3]; /*071222*/

EXTERN float bondangle[MAXANGLES+1];
/*rotator's name for bondrot lists*/
EXTERN char  rotatorname[16];
/*general global to pass double values*/
EXTERN double dblx,dbly,dblz;
/*residue's cys g atom records for ss bonds*/
EXTERN char  ssaspectstr[MAXARR][MAXaspects+3]; /*in parens: (XXXX)*/
EXTERN char  ssmod[MAXARR][MAXmodchr]; /*000418*/
EXTERN char  ssname[MAXARR][7],ssatom[MAXARR][6];
EXTERN char  ssres[MAXARR][4],sssub[MAXARR][5],ssrins[MAXARR][2]; /*070926*/
EXTERN char  ssAnum[MAXARR][6];/*Hybrid36 071001, replace int ssncnt[MAXARR]*/
EXTERN int   ssnum[MAXARR],ssscall[MAXARR];
EXTERN float ssx[MAXARR],ssy[MAXARR],ssz[MAXARR],ssBval[MAXARR];/*Bval 050704*/
EXTERN float sscax[MAXARR],sscay[MAXARR],sscaz[MAXARR],sscaBval[MAXARR];
EXTERN float sscbx[MAXARR],sscby[MAXARR],sscbz[MAXARR],sscbBval[MAXARR];
/*connecting atom at beginning of next residue 030515*/
EXTERN char  modnext[MAXmodchr];
EXTERN char  namenext[7];
EXTERN char  atomnext[6],resnext[4],subnext[2],rinsnext[2];
EXTERN char  Anumnext[6];/*Hybrid36 071001, replace int ncntnext*/
EXTERN int   numnext,Latomnext;
EXTERN float xnext,ynext,znext,onext,Bnext,Uvalnext;
EXTERN char  aspectstrnext[MAXaspects+3]; /*in parens: (XXXX)*/

/* possible overlap with earlier PREKIN variables */
EXTERN char word[256],texts[256],temps[256];
EXTERN char alertstr[256],alertstr2[256],alertstr3[256];
EXTERN char oldstr[256],olderstr[256],newstr[256],InfoStr[256],MolNameStr[16];
EXTERN char NameStr[256],OutfileStr[256],ScriptinStr[256],ShortNameStr[256];
EXTERN char DirStr[256],PDBfileStr[256],helpoutStr[256]; /*040425 helpout*/
EXTERN char codestr[MAXNAMECHAR+1]; /*051102*/
EXTERN int  Lcodestr;    /*051102*/
EXTERN char molecstr[MAXNAMECHAR+1]; /*051114*/
EXTERN int  Lmolecstr;    /*051114*/
EXTERN char  Scratchs[256],Scratchhbs[256],Scratchhets[256],ScriptoutStr[256];
EXTERN char  version[256],charin,kolor[20];
EXTERN char chainreport[256]; /*041031*/
EXTERN FILE  *fpinput,*fpoutput,*fpscriptin,*fpscriptout,*fpaalib;
EXTERN FILE  *fphelpout; /*040425*/
EXTERN int  Lhelpoutopen; /*040425*/
EXTERN int  atEOF,itext,itemp,ihitend,ihitaalibend,ataalibEOF,Laalibopen;
EXTERN int    thenLines,theviewLines,thenoldLines,thendiff;
EXTERN int   Lfpoutput; /*931128 flag for failure to write fpoutput*/
/*--hbond residue-by-residue variables*/
EXTERN int    tfc1,tfn2,tfca2,tfc2,tfo2,tfnh2;
EXTERN float  c1xyz[4],xyz[4],n2xyz[4],ca2xyz[4],c2xyz[4],o2xyz[4];
EXTERN float  lfac,dimhb,dlimhb,angfac,disthb,cosnho,nh2xyz[4];
EXTERN int    maxhb,cocnt,nhcnt,inumn2,inumo2;
/*--flow control pass to pass */
EXTERN int    Lnewpass,Lpassposs,npass;
EXTERN int    Lnewpassflow;

EXTERN int Lthispass,Nchainfirstpass,ichainthispass,Ltopout,Nmodels,Nresiduesin;
EXTERN int    icountres[MAXlevels];/*010530x*/
EXTERN int    icountresfirstpass[MAXlevels];/*010530x*/
EXTERN char   typechainfirstpass[MAXlevels]; /*991210*/
EXTERN char   kindchainfirstpass[MAXlevels]; /*060114 DNA vs RNA*/
EXTERN char   codechainfirstpass[MAXlevels][3]; /*010426,2char ChainID 070926*/
EXTERN int    LtypeP, LtypeN, LtypeH, LtypeW, LtypeU; /*051201*/
EXTERN char   residuetype,previousresiduetype; /*P,N,H,W,U, ,  061006*/

/*--flow control within a pass */
EXTERN int    Lfirstrecordinlevel;
EXTERN int    Lallaminoacids,iresiduecnt,Lallbases,Lnucleicacid,Lnucleicarrows;
EXTERN int    Lneedsubunits,Lrangestart,newkinemage;
EXTERN int   IOerr,Lerr,Lcancel,Lnewcoord,Lnewfile,Lgetfile,Lcontrols,Linein;
EXTERN int   Lin_stdin,Lout_stdout;/*970416*/
EXTERN int   Lgot_infilename,Lgot_outfilename;/*970416*/
EXTERN int   Lgot_infileopen,Lgot_outfileopen;/*970608*/
EXTERN int   Ldid_outfiletext;/*970610*/
EXTERN int   Ltestprint, Loption,Lscriptin,Lscriptout,Lcoordin,Lhetconnect;
EXTERN int   Lopenscriptout,Lscriptfileopen,LRCDialogInput;
EXTERN int   Lopenscriptin;
EXTERN int   Ltest, Limittext;
EXTERN int   Lreport;/*130324 made global*/
EXTERN long  maxtextchar;
EXTERN int  Lgetranges,Lrangecontrols,Lfinishranges;
EXTERN int  Ldetails;
EXTERN int  Lgetatommarker;
EXTERN int  Lgetribbonparam;
EXTERN int  Lfocusoption;
EXTERN int  Lfocusoptionend;
EXTERN int  Lfocusdistance;
EXTERN int  Lfocusdistanceend;
EXTERN int  Lconditions;
EXTERN int  Lstartarange;
EXTERN int  Lendarange;
EXTERN int  LBcolor;  /*971213*/
EXTERN int  LBvalue;
EXTERN int  Lkinmasters; /*030519*/
EXTERN int  Lstdoutreport,Lpperptobase,Lplotpperptobase; /*030502,15,19*/
EXTERN int  Ldumppperptobase,Lpperpoutliersonly; /*040202,040705*/
EXTERN int  Lpperptoc1nline; /*050125*/
EXTERN int  Loperptoc1nline; /*091129*/
EXTERN int Lquiet,Lcommanded,Lcommandsub,Lappend,Lanimate,Laltcontrol;
EXTERN int Loldpdb; /*071215*/
EXTERN int Lnobutton,Lresiduegroups; /*060120 multi-poses of ligand as ATOMs*/
EXTERN int Lonlygroups,Lonegroup,Lnogroups,Ngroups,Lallbuttons; /*010422*/
EXTERN int Lnosubgroups; /*051129*/
EXTERN int Lnotext; /*010422*/
EXTERN int LIOmage,LIOpipe,Nusermod; /*020514*/
EXTERN int Ldid_outfileheader,Lsingleout; /*040318*/
EXTERN int Llistcolordom; /*050705 for ribnhet option*/
/*-- prekin interface contols */
EXTERN short  startValue,endValue;
EXTERN short  lineHeight,textHeight,widMax,maxValue,minValue;
EXTERN int   nLines, viewLines, textLines, scrolLines,iword;
EXTERN int GHEIGHT,GWIDTH,maxwide,oldmaxwide,Lcursoractive;

/* varibles needed by Diamond's simple superposition subroutine*/
/* note dimension added up by 1 to keep fortran indexing in actual routine*/
EXTERN int    NN; /*actual number of corresponding points to be superimposed*/
EXTERN double X[4][MAXPTS+1], Y[4][MAXPTS+1]; /*MAXPTS defined above*/
EXTERN int    IM, JM, KM;
EXTERN double CRIT;
EXTERN double R[4][4], U[4], THETA, E, XBAR[4], YBAR[4];
EXTERN int    NCYCLE, ISTAT;
EXTERN float P[5][5];

EXTERN int UseSEGIDasChain; //jjh 130507

/*UNIX_X11  does typedef Cursor CursHandle */
/*PCWINDOWS does typedef Handle CursHandle */
/*MACINTOSH has  type           CursHandle */
EXTERN CursHandle   waitcursor,activecursor;

/*UNIX_X11  does typedef Widget MenuHandle */
/*PCWINDOWS does typedef Handle MenuHandle */
/*MACINTOSH has  type           MenuHandle */
EXTERN MenuHandle   fileMenu, textMenu, optiMenu, launchMenu, writeMenu;

/*UNIX_X11  does typedef Widget WindowPtr */
/*PCWINDOWS does typedef HWND   WindowPtr */
/*MACINTOSH has  type           WindowPtr */
EXTERN WindowPtr    DLOGreportBox,filedialogBox,textWindow,INITreportBox;
EXTERN WindowPtr    RangeStartDialogBox,KlugesDialogBox;
EXTERN WindowPtr    ExternalScriptDialogBox,BuiltinScriptsDialogBox;
EXTERN WindowPtr    RunConditionsDialogBox;
EXTERN WindowPtr    RangeControlDialogBox;
EXTERN WindowPtr    ScNumberDialogBox;
EXTERN WindowPtr    ScNumberControlDialogBox;
EXTERN WindowPtr    AtomMarkerDialogBox;
EXTERN WindowPtr    FocusPointDialogBox;
EXTERN WindowPtr    FocusParamDialogBox;
EXTERN WindowPtr    FocusOptionsDialogBox;
EXTERN WindowPtr    FocusResNumDialogBox;
EXTERN WindowPtr    RibbonDialogBox;
EXTERN WindowPtr    KludgesDialogBox;
EXTERN WindowPtr    ColorScaleDialogBox;

#ifdef MACINTOSH
  EXTERN StandardFileReply      outputfileinfo;  /*940811*/
  EXTERN TEHandle     theText, textText;
  EXTERN MenuHandle   appleMenu;
  EXTERN Handle       dialog;
  EXTERN ControlHandle  vtextScroll;
  EXTERN Rect      dragRect, windowBounds, boxBounds;
  EXTERN Rect      destRect, textdRect, viewRect, textvRect;
  EXTERN pascal Boolean DLOGFilter(),reportFilter();
  EXTERN ControlHandle      textvbar;
  EXTERN long mactextselect; /*holds old selEnd for a specific mac routine*/

#endif


#ifdef PCWINDOWS
  /* save the current directory in a global array so we can look for */
  /* the next file in it */
  EXTERN char szDirName[256];

  EXTERN HINSTANCE hInst;            /* current instance  */
  EXTERN HANDLE hFileContents
#ifdef MAGE_INIT
     = 0
#endif
    ;

  /* file stuff */
  EXTERN int pdbfile,kinfile,scriptinfile,scriptoutfile,focusfile;
/*  EXTERN int scratchfile,hbscratchfile,hetscratchfile;*/
  EXTERN OFSTRUCT pdbofstruct,kinofstruct,scriptinofstruct,scriptoutofstruct;
/*  EXTERN OFSTRUCT scratchofstruct,hbscratchofstruct,hetscratchofstruct;*/

  EXTERN char *filebuf;
  EXTERN char *pdbstart;
  EXTERN char *pdbend;

  /*THINGS FOR MICROSOFT PROGRAMMING FILE DIALOGS 920801*/
  EXTERN char FileName[128];        /* current filename */
  EXTERN char PathName[128];        /* current pathname */
  EXTERN char OpenName[128];        /* filename to open */
  EXTERN char  DefPath[128];        /* default path for list box */
  EXTERN char  DefSpec[13]         /* default search spec */
#ifdef MAGE_INIT
     = "*.*"
#endif
    ;

  EXTERN char  DefExt[]           /* default extension */
#ifdef MAGE_INIT
     = ".txt"
#endif
    ;

  EXTERN char  str[255];            /* string for sprintf() calls */


  EXTERN HWND PrekinWindow;

  EXTERN HACCEL PrekinAccel;
  EXTERN int DisplaySizeX;
  EXTERN int DisplaySizeY;
  EXTERN int fontheight;
  EXTERN int fontwidth;
  EXTERN int fontmaxwidth;
  EXTERN int fontverticalspace;


  /* extra window bytes of info for text displaying and scrolling */

#define TEXTLENGTH 0
#define TEXTLINESTOTAL 4
#define TEXTLINECURRENT 8
#define TEXTHANDLE 12
#define TEXTLINESTARTHANDLE 16

  EXTERN char ofilename[FILENAME_MAX];
  EXTERN char startpath[_MAX_PATH];

  EXTERN HDC cDC, DC;

  EXTERN int metafiling
#ifdef MAGE_INIT
      = 0
#endif
    ;

  EXTERN  LPSTR filearg
#ifdef MAGE_INIT
      = NULL
#endif
    ;


  EXTERN int ireturnflag;
#endif /*PCWINDOWS*/

#ifdef UNIX_MOTIF
  EXTERN  XmFontList fontList;
  EXTERN  XmFontList littlefontList;
  EXTERN  XmFontList mediumfontList;
  EXTERN  XmFontList largefontList;
#endif /*UNIX_MOTIF*/

#ifdef UNIX_X11
  EXTERN  XFontStruct *buttonFont;
  EXTERN  XFontStruct *littleFont;
  EXTERN  XFontStruct *mediumFont;
  EXTERN  XFontStruct *largeFont;
  EXTERN  int oldfontsize,littlefontsize,mediumfontsize,largefontsize;
  EXTERN XtAppContext app_context;
  EXTERN Visual* progvisual;
  EXTERN int progdepth;
  EXTERN Pixel windowbackground,windowborder;
  EXTERN Colormap colormap;
  EXTERN Display *dpy;
  EXTERN Widget  textwindow;
  EXTERN Window  drawWindow;
/*  EXTERN Cursor    waitcursor,activecursor;*/
  EXTERN  Arg args[100];
  EXTERN  int n;
  EXTERN  Widget topLevel, topgraf, toptext;
  EXTERN  Widget grafwindow, textwindow;
  EXTERN  Widget menuBar,fileButton;
  EXTERN  Widget openItem,aboutItem,launchItem,passItem,quitItem;
  EXTERN  Widget optiButton,optionsaboutItem,mutrotItem;
  EXTERN  Widget makehelphtmlItem; /*040425*/
  EXTERN  Widget reportform,reportbox,reportbutabort,report1,report2,report3;
  EXTERN  Widget reportbuts,reportbutOK;
  EXTERN  Widget fileoutdialog,scriptfiledialog;
#endif /*UNIX_X11*/

typedef struct{
    char* begin;
    char* end;
    char* next;
    char* cursor;
} textblock;

EXTERN textblock mainscratch, hbnscratch, hboscratch, hetscratch;
EXTERN textblock headerstuff,hypertextstuff,pdbinfostuff;
EXTERN int Lhypertextstuff;

typedef struct{
    char color[16];
    char origcolor[16];
    float value;
    float origvalue;
} colorscalestruct;

EXTERN colorscalestruct colorscale[11];

#include "PKINhdr.h"   /*many prototypes are at beginning of src files*/

