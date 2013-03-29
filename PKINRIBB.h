#ifdef  PKINRIBB
#undef  PKINRIBB
#define PKINRIBB
#else
#define PKINRIBB extern
#endif

PKINRIBB    int   Lribbonnew;  /*051025 new ribbon structures: PKINRIBB*/
PKINRIBB    int   Lnewribbonstart; /*090219 clear mind for new ribbon section*/
PKINRIBB    int   previousribbontype; /*090219 global: clear for new fragment*/

#define CORDER 4  /*order of the B-spline defined here*/

/*{{{struct ribpntstruct */
/*ribpntstruct* allocribpntstructure(ribresstruct* thisresptr )*/
typedef struct ribpntstruct {
   struct ribresstruct* ribresptr; /*parent*/
          /*members*/
   char   name[8]; /*pdb type atom name when storing atom coords*/
   float  x;
   float  y;
   float  z;
}ribpntstruct;
typedef ribpntstruct* ribpntptr;
/*}}}*/

/*{{{struct ribresstruct  */
/*ribresstruct* allocribresstructure(ribfragstruct* thisfragptr )*/
typedef struct ribresstruct {
   struct ribfragstruct* ribfragptr; /*parent*/
   struct ribresstruct*   previousptr; /*peers*/
   struct ribresstruct*   nextptr;
          /*members*/
   struct ribpntstruct*  middlptr;  /*midpoint, original placement*/
   struct ribpntstruct*  guideptr;  /*midpoint then offset for curvature*/
                                    /*not subject to user tweaks*/
   struct ribpntstruct*  axialptr;  /*midpoint, then moved to axis*/
   struct ribpntstruct*  avecaptr; /*unnormalized pt to pt vector along chain*/ 
   struct ribpntstruct*  cvecnptr; /*normalized vector perpendicular to plane*/
   struct ribpntstruct*  dvecnptr; /*normalized vector in plane, e.g. peptide*/
   struct ribpntstruct*  npeptptr;  
   struct ribpntstruct*  hnpepptr;  /*store or calc for mc Hbonds*/
   struct ribpntstruct*  calphptr;  
   struct ribpntstruct*  ccarbptr;  
   struct ribpntstruct*  ocarbptr;  
   struct ribpntstruct*  cbetaptr;  
   struct ribpntstruct*  pnuclptr;  
   struct ribpntstruct*  o5ribptr;  
   struct ribpntstruct*  c1ribptr;  
   struct ribpntstruct*  c3ribptr;  
   struct ribpntstruct*  o3ribptr;  
   struct ribpntstruct*  c4ribptr;  
   struct ribpntstruct*  cunknptr;/*unknown carbon in non-residue group 061011*/
   float  widthpossible; /*width factor from curvature calc at this residue*/
   float  width;         /*width factor locally consistant, so can apply */
   int    number;     /*position of residue along spline, incl padded ends*/
   int    numrib;     /*residue number, as given in pdb file*/
   char   resrib[4];  /*residue name*/
   char   subrib[3];  /*residue subunit code == chainID 2char 070926*/
   char   rinsrib[2]; /*residue insertion code*/
   float  Bvalrib;    /*typical B factor for this residue from a critical atom*/
   int    Lstub;      /*allows ribbon stub to cbeta */
   unsigned int    explicit;  /*type of ribbon at this residue (explicitrib)*/
   float  extend;/*curvature extendedness fraction between minimum and maximum*/
   int    COside;
   char   ribidentity[5]; /*identity col 10--13 SHEET record  060929*/
   float  azmangle; /*for Calpha+COazmuthalangle  100322*/
}ribresstruct;
PKINRIBB struct ribresstruct* thisribresptr;
/*}}}*/

/*{{{struct ribfragstruct */
/*ribfragstruct* allocribfragstructure(ribsectionstruct* thissectionptr )*/
typedef struct ribfragstruct {
   struct ribsectionstruct* ribsectionptr; /*parent*/
   struct ribresstruct*    firstribresptr; /*children==members*/
   struct ribresstruct*    lastribresptr;
   struct ribfragstruct*   previousptr; /*peers*/
   struct ribfragstruct*   nextptr;
   int    number; /*MAXRES residues incl dummies in splineset, alloc MAXRES+1*/
   struct spline* splineset;
   struct guide*  guideset;
   struct residue* residueset; /*array of pointers to residues of this frag*/
}ribfragstruct;
PKINRIBB struct ribfragstruct* thisribfragptr;
/*}}}*/

/*{{{struct ribsectionstruct  */
/*ribsectionstruct* allocribsectionstructure(ribmodelstruct* thismodelptr )*/
typedef struct ribsectionstruct {
   struct ribmodelstruct*  ribmodelptr; /*parent*/
   struct ribfragstruct*   firstribfragptr; /*children==members*/
   struct ribfragstruct*   lastribfragptr;
   struct ribsectionstruct* previousptr; /*peers*/
   struct ribsectionstruct* nextptr;
   char chainID[3]; /*IS THIS EVER USED ???? 070926 */
   char type;  /*Protein vs generic Nucleic acid*/
   char kind;  /*Nucleic acid: Dna or Rna*/
   int  number; /*MAXRES residues incl dummies in splineset, alloc MAXRES+1*/
}ribsectionstruct;
PKINRIBB struct ribsectionstruct* thisribsectionptr;
PKINRIBB struct ribsectionstruct* trialribsectionptr;
/*}}}*/

/*{{{struct ribmodelstruct  */
/*ribmodelstruct* allocribmodelstructure(char* modnum)*/
typedef struct ribmodelstruct {  /*no parent, global: this, first, last*/
   struct ribsectionstruct* firstribsectionptr; /*children==members*/
   struct ribsectionstruct* lastribsectionptr;
   struct ribmodelstruct*  previousptr; /*peers*/
   struct ribmodelstruct*  nextptr;
   char   modelstr[MAXmodchr]; /*as of 051025 is == nmrmodel[]*/
}ribmodelstruct;
PKINRIBB struct ribmodelstruct* thisribmodelptr;
PKINRIBB struct ribmodelstruct* firstribmodelptr;
PKINRIBB struct ribmodelstruct* lastribmodelptr;
/*}}}*/

/*{{{struct p  */
typedef struct p {
   float x;
   float y;
   float z;
}p;
/*}}}*/

/*{{{struct guide  */
typedef struct guide {
   struct p* m0;
   struct p* a1;
   struct p* a2;
   struct p* b3;
   struct p* b4;
   struct p* c5;
   struct p* c6;
   struct p* x7;   
}guide;
/*}}}*/

/*{{{struct chord  */
typedef struct chord {
   struct p* pt;
}chord;
/*}}}*/

/*{{{struct strand  */
typedef struct strand {
   struct chord* chordset;
}strand;
/*}}}*/

/*{{{struct spline  */
typedef struct spline {
   struct strand* m0strnd;
   struct strand* a1strnd;
   struct strand* a2strnd;
   struct strand* b3strnd;
   struct strand* b4strnd;
   struct strand* c5strnd;
   struct strand* c6strnd;
   struct strand* x7strnd;
}spline; /*splineset[]*/
/*}}}*/

/*{{{struct residue  */
typedef struct residue {
   struct ribresstruct* resptr;
}residue; /*residueset[]*/
/*}}}*/

/*{{{prototypes  */

struct ribpntstruct* allocribpntstructure(ribresstruct*); 
struct ribresstruct* allocribresstructure(ribfragstruct*); 
struct ribfragstruct* allocribfragstructure(ribsectionstruct*); 
struct ribsectionstruct* allocribsectionstructure(ribmodelstruct*);
struct ribmodelstruct* allocribmodelstructure(char*); 
                             /*models have no parent, exist as globals*/

int    loadribbonatoms(void); /*called for each residue*/

void   buildallguides(void);
void   buildsectionguides(ribsectionstruct*);
void   expandallguides(void);
void   expandsectionguides(ribsectionstruct*);
void   calculateallsplines(void);
void   calculatesectionsplines(ribsectionstruct*);
void   constructallribbons(void);
void   constructsectionribbons(ribsectionstruct*);

void   makeallpseudoHbonds(void);
void   makesectionpseudoHbonds(ribsectionstruct*);

float  distanribpntptrs(ribpntstruct*, ribpntstruct*);
float  distanpntptrs(p*, p*);

void splinecalc(float Tbone, int IMAX, int X[CORDER*2], float fg[CORDER][4], float spine[10][4]);
/*}}}*/
