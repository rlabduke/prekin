/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
                                /*PKINRIBB.c*/
/*(jEdit options) :folding=explicit:collapseFolds=1:*/
#include "PKIN.h"
#include "PKINCRTL.h" /*070801*/
#define   PKINRIBB
#include "PKINRIBB.h"
#undef    PKINRIBB

/*{{{--NOTE--  version, etc. */
/*051025  for now, just work with one model, and let prekin's levels==sections*/
/*PKINCRTL: define a new section each time prekin increments its level */
/* Lribbonnew is conditional for the new ribbon code */
/*need to PKININIT the globals this...ptr = NULL; structures are malloc-ed*/
/*}}}*/

/*{{{--NOTE--  number of strands:  */
       /*this routine always computes side guide points for 7 strands*/
       /*middle: m0, flanking a1,a2, and b3,b4, and c5,c6 */

       /*for stranded ribbons, 3 use middle plus b at full width */
       /* while 5 use middle, a intermediate, b at full width */
       /* this lets beta arrowheads work both for explict beta */
       /* as well as stranded, e.g. for VRML 3D-printer ribbons */

       /*for explicit ribbons, width for a: alpha, b: beta, c: coil */
       /* except we've always used single fat strand for coil, not flat coil*/

       /*Faced curvature dependent ribbons using c at calc width */
       /* can exist along with explicit faced ribbons using a and b */

       /*the widen-factor for each strand is calc from appropriate width */
       /*The same curvature calc is used for both ribbon widen-factor and */
       /* displacement factor radially out in tightly curled regions */
       /* This confuses fudge for widen and fudge for displacement.*/

       /* Curvature Dependent Variable Width, Radially Displaced Ribbons */
       /*use "widen" for amount of width offset of coil toward alpha or beta*/
       /*use "widthfactor" for the percent fudge */
       /*for amount of radial offset of usual to curled guide point,*/
       /*use "radialfactor" for that percent fudge */

       /*Historically, prekin could do any number of strands up to 9 */
       /* 9 stranded ribbons were very elegant on PS300 vector graphics */
       /* but were probably never used later. Evens were very rare if ever */
       /* 3 are goood for low-load kinemages, 5 make good VRML 3D-constructs */
/*}}}*/

/*{{{--CONTROLS-- types of ribbons:  */
   /*see PKINDLOG/EndRibbonDialog() */
   /*Faced ribbons can be explicitly defined or have curvature-dependent-width*/
   /*Lskeinedribbon controls stranded vs faced */
   /*explicitribbontype is a residue's assigned secondary structure*/
   /*explicitribbonidentity residue's sec. str. identity in a PDB SHEET record*/
   /*stranded: nstrnd==3 and nstrnd==5 do curvature dependent variable width */
   /* additional control for beta-like arrowheads */
   /*faced: curvature dependent variable width always available, */
   /* explicit alpha, beta (fake beta for nucleics) done when 2D regions given*/
   /*20140520 redefined -spline vs -skeins, -spline now simple vector mid line*/
/*}}}*/

/*{{{****allocation of structures: model, section, frag, res, pnt ****/

/*{{{allocribmodelstructure()*************************************************/
ribmodelstruct* allocribmodelstructure(char* modnum)
{
   struct ribmodelstruct *newribmodelptr;
   int  ireturn = 0; /*if successful, will be set == 1 */

   newribmodelptr = (ribmodelstruct*)malloc(sizeof(struct ribmodelstruct));

   if(newribmodelptr != NULL)
   {/*successfull allocation*/
      if(firstribmodelptr == NULL) /*initiate this set of structures*/
      {
         firstribmodelptr = newribmodelptr;
         lastribmodelptr = newribmodelptr; /*new one is as-of-now the last one*/
         firstribmodelptr->previousptr = NULL;
         firstribmodelptr->nextptr = NULL;
      }
      else /*set references between last and new structures*/
      {
         lastribmodelptr->nextptr = newribmodelptr;
         newribmodelptr->previousptr = lastribmodelptr; /*old last one*/
      }

      /*reset who is current global*/
      /*thisribmodelptr = newribmodelptr;*/ /*identify as a global*/
      newribmodelptr->nextptr = NULL;   /*no others beyond this one*/


      newribmodelptr->firstribsectionptr = NULL; /*initialize members*/
      newribmodelptr->lastribsectionptr  = NULL;
      strcpy(newribmodelptr->modelstr,modnum); /*nmrmodel M###*/
      ireturn = 1;
   }
   /*free(newribmodelptr);*/
   /*newribmodelptr = NULL;*/
   return(newribmodelptr);
}
/*___allocribmodelstructure()_____________________________________________}}}*/

/*{{{allocribsectionstructure()***********************************************/
ribsectionstruct* allocribsectionstructure(ribmodelstruct* thismodelptr )
{
   struct ribsectionstruct *newribsectionptr;
   int  ireturn = 0; /*if successful, will be set == 1 */

  newribsectionptr = (ribsectionstruct*)malloc(sizeof(struct ribsectionstruct));

   if(newribsectionptr != NULL)
   {/*successfull allocation*/
      if(thismodelptr->firstribsectionptr == NULL)
      {/*initiate this set of structures*/
         thismodelptr->firstribsectionptr = newribsectionptr;
         thismodelptr->lastribsectionptr = newribsectionptr;
         newribsectionptr->previousptr = NULL; /*first of its peers*/
         newribsectionptr->nextptr = NULL;
         newribsectionptr->number = 0; /*090219*/
      }
      else /*set references between last and new structures*/
      {
         newribsectionptr->previousptr = thismodelptr->lastribsectionptr;
         thismodelptr->lastribsectionptr->nextptr = newribsectionptr;
         /*newribsectionptr->previousptr->nextptr = newribsectionptr;*/
                                                /*link previous*/
         newribsectionptr->nextptr = NULL;
         thismodelptr->lastribsectionptr = newribsectionptr;
           /*now last child of parent*/
      }
      /*reset who is current global, previous is known as previous*/
      /*thisribsectionptr = newribsectionptr;*/ /*identify as this... global*/
      /*reset current connections*/
      newribsectionptr->ribmodelptr = thismodelptr;  /*store parent*/

      newribsectionptr->firstribfragptr = NULL; /*initialize members*/
      newribsectionptr->lastribfragptr  = NULL;
      newribsectionptr->chainID[0] = ' '; /*2char 070926*/
      newribsectionptr->chainID[1] = ' '; /*2char 070926*/
      newribsectionptr->chainID[2] = '\0'; /*2char 070926*/
      newribsectionptr->type = 'U'; /*Uknown,Protein,Nucl*/
      newribsectionptr->kind = 'U'; /*Uknown,Dna,Rna*/
      ireturn = 1;
   }
   /*free(newribsectionptr);*/
   /*newribsectionptr = NULL;*/
   return(newribsectionptr); /*NULL if alloc failed*/
}
/*___allocribsectionstructure()________________________________________}}}*/

/*{{{allocribfragstructure()***********************************************/
ribfragstruct* allocribfragstructure(ribsectionstruct* thissectionptr )
{
   struct ribfragstruct *newribfragptr;
   int  ireturn = 0; /*if successful, will be set == 1 */

   newribfragptr = (ribfragstruct*)malloc(sizeof(struct ribfragstruct));

   if(newribfragptr != NULL)
   {/*successfull allocation*/
      if(thissectionptr->firstribfragptr == NULL)
      {/*initiate this set of structures*/
             /*register with parent*/
         thissectionptr->firstribfragptr = newribfragptr;
         thissectionptr->lastribfragptr = newribfragptr;
         newribfragptr->previousptr = NULL; /*first of its peers*/
         newribfragptr->nextptr = NULL;
      }
      else /*set references between last and new structures*/
      {
         newribfragptr->previousptr = thissectionptr->lastribfragptr;
         newribfragptr->previousptr->nextptr = newribfragptr; /*link previous*/
         newribfragptr->nextptr = NULL;
         thissectionptr->lastribfragptr = newribfragptr;
           /*now last child of parent*/
      }
      /*reset who is current global, previous is known as previous*/
      /*thisribfragptr = newribfragptr; */ /*identify as this... global*/

      /*reset current connections*/
      newribfragptr->ribsectionptr = thissectionptr;  /*store parent*/

      newribfragptr->firstribresptr = NULL; /*initialize members*/
      newribfragptr->lastribresptr  = NULL;
      /*initialize ribbon position number*/
      newribfragptr->number = 0;  /*number of ribresidues 051112*/
      newribfragptr->splineset=NULL;
      newribfragptr->guideset=NULL;
      newribfragptr->residueset=NULL;
         /*array of pointers to residues of this frag*/
      ireturn = 1;
   }
   /*free(newribfragptr);*/
   /*newribfragptr = NULL;*/
   return(newribfragptr); /*NULL if alloc failed*/
}
/*___allocribfragstructure()___________________________________________}}}*/


/*{{{allocribresstructure()************************************************/
ribresstruct* allocribresstructure(ribfragstruct* thisfragptr )
{
   struct ribresstruct *newribresptr;
   int  iok = 0; /*if successful, will be set == 1, and return ribresstruct*/

   newribresptr = (ribresstruct*)malloc(sizeof(struct ribresstruct));

   if(newribresptr != NULL)
   {/*successfull allocation*/
      if(thisfragptr->firstribresptr == NULL)/*initiate this set of structures*/
      {
         thisfragptr->firstribresptr = newribresptr;
         thisfragptr->lastribresptr = newribresptr; /*register with parent*/
         thisfragptr->number = 1; /*first position will be a dummy*/
         newribresptr->previousptr = NULL; /*first of its peers*/
         newribresptr->nextptr = NULL;
      }
      else
      {
         newribresptr->previousptr = thisfragptr->lastribresptr; /*set peers*/
         newribresptr->previousptr->nextptr = newribresptr; /*link previous*/
         newribresptr->nextptr = NULL;
         thisfragptr->lastribresptr = newribresptr;/*now last child of parent*/
         thisfragptr->number++; /*count of positions: dummy and real residues*/
      }
      newribresptr->ribfragptr = thisfragptr;  /*store parent*/

      (thisfragptr->ribsectionptr)->number++; /*090219 residues in section cnt*/
         
      newribresptr->guideptr = NULL; /*initialize members*/
      newribresptr->middlptr = NULL;
      newribresptr->axialptr = NULL;
      newribresptr->avecaptr = NULL;
      newribresptr->cvecnptr = NULL;
      newribresptr->dvecnptr = NULL;
      newribresptr->npeptptr = NULL;
      newribresptr->hnpepptr = NULL;
      newribresptr->calphptr = NULL;
      newribresptr->ccarbptr = NULL;
      newribresptr->ocarbptr = NULL;
      newribresptr->cbetaptr = NULL;
      newribresptr->pnuclptr = NULL;
      newribresptr->o5ribptr = NULL;
      newribresptr->c1ribptr = NULL;
      newribresptr->c3ribptr = NULL;
      newribresptr->o3ribptr = NULL;
      newribresptr->c4ribptr = NULL;
      /*NOTE the above are NOT allocated, must be when atoms brought in */
      newribresptr->widthpossible = 0;
      newribresptr->width = 0;
      newribresptr->number = thisfragptr->number;
         /* # of residue along spline, incl padded ends*/
         /*   this number belongs to this residue*/
      newribresptr->numrib = 0; /*residue number, as given in pdb file*/
      newribresptr->resrib[0] =  '\0';
      newribresptr->subrib[0] =  '\0';
      newribresptr->subrib[1] =  '\0';
      newribresptr->subrib[2] =  '\0';
      newribresptr->rinsrib[0] = '\0';
      newribresptr->Bvalrib = 0;
      newribresptr->Lstub = 0;
      newribresptr->explicit  = 0;
      newribresptr->extend = 0;
      newribresptr->COside = 0;
      newribresptr->azmangle = 0; /*for Calpha+COazmuthalangle  100322*/
         /*alloc the vectors needed to define the splines*/
      if(
            (newribresptr->avecaptr=allocribpntstructure(newribresptr))!=NULL
          &&(newribresptr->cvecnptr=allocribpntstructure(newribresptr))!=NULL
          &&(newribresptr->dvecnptr=allocribpntstructure(newribresptr))!=NULL
          &&(newribresptr->guideptr=allocribpntstructure(newribresptr))!=NULL
          &&(newribresptr->middlptr=allocribpntstructure(newribresptr))!=NULL
          &&(newribresptr->axialptr=allocribpntstructure(newribresptr))!=NULL
        ) {iok = 1;} /*still good*/
      else{iok = 0;} /*fault*/
      if(!iok){newribresptr=NULL;} /*ugh! garbage! should de-allocate*/
   }
   return(newribresptr); /*new ptr or NULL if any alloc failed*/
}
/*___allocribresstructure()_______________________________________________}}}*/

/*{{{allocribpntstructure()***************************************************/
ribpntstruct* allocribpntstructure(ribresstruct* thisresptr )
{
   struct ribpntstruct *newribpntptr;
   int  ireturn = 0; /*if successful, will be set == 1 */

   newribpntptr = (ribpntstruct*)malloc(sizeof(struct ribpntstruct));

   if(newribpntptr != NULL)
   {/*successfull allocation*/
      /*parent takes care of assigning this point to the appropriate atom*/

      newribpntptr->ribresptr = thisresptr;  /*store parent*/

      newribpntptr->name[0] = '\0'; /*initialize members*/
      newribpntptr->x = 0;
      newribpntptr->y = 0;
      newribpntptr->z = 0;
   }
   return(newribpntptr); /*new ptr or NULL if alloc failed*/
}
/*___allocribpntstructure()_______________________________________________}}}*/

/*}}}****allocation of structures: model, section, frag, res, pnt ****/

/*{{{loadribbonatoms()********************************************************/
int  loadribbonatoms()  /*of current residue*/
{
  /*{{{-- declarations and NOTES*/

   /*loadribbonatoms() called once per residue during general input*/
   /*loads specific atom info into ribres structures that are part of*/
   /* ribfrag structures, which in turn are part of ribsection structures*/
   /* prekin does a section of ribbon for each of its levels==chains*/
   /* these are under ribmodel structures, the top of this hierarchy*/

   /* after the residues of a level are in (prekin's idea of chains) */
   /* then buildsectionguides(thisribsectionptr);  */

   int    n=0; /* beware: n is a global counter for ranges*/
   float  distsq=0;
   int    Lgotatoms = 1; /*ireturn 010511, renamed Lgotatoms 090219*/
   int    discon=0;
   ribfragstruct* trialribfragptr=NULL;
   float  wc=0;
   float  fx=0,fy=0,fz=0;
   float  factor=0;
   int    IOK=0;    /*051214*/
   int    nitr=0,itr=0; /*070801*/
   char   Q=' '; /*070801 Q ...PQR...*/

   /* B spline order==4 uses 4 intervals to calculate spline over mid-interval*/
   /* nth spline calc using n-2,n-1,nth,n+1 guidepoints */
   /*guide points between residues, named by the first */
   /*construct 3 dummy residues to begin ribbon, then their guide points */
   /*are used to construct spline for the first real interval == residue*/
   /*similarly, at finish, a single dummy lets the nth be the last real one */
   /*so the spline covers beginning and end residues */

  /*}}}declarations and NOTES*/

  if(LGFPchromophore && typechainfirstpass[level]=='P')
       {nitr = 3;}
  else
       {nitr = 1;}

  for(itr=1; itr<=nitr; itr++) /*070801 for GFPchromophore*/
  {/*itr: iteration for multi-residue pseudo-hets like GFPchromophore*/
     if(LGFPchromophore)  /*070801*/
     {
        if     (itr==1) {Q='1';}
        else if(itr==2) {Q='2';}
        else if(itr==3) {Q='3';}
     }

  /*{{{-- define and open a new ribresstruct for this residue*/

   /*define a new global ribresstruct for this residue*/
   thisribresptr = allocribresstructure(thisribfragptr);

   if(thisribresptr != NULL && thisribresptr->number == 1 )/*start of fragment*/
   {/*pad beginning with dummy residues so ribbon can tail off nicely*/
      /*use original alloc above for 1st dummy residue*/
      /*original becomes n-3, 3 dummies needed since */
      /*spline uses n-3,n-2,n-1,nth to calc spline in interval n-1 */
      /*SO WHY DID OLD PREKIN GET AWAY WITH ONLY 2 BEGINNING DUMMIES ???? */

      /*thisribresptr->explicit == 0 unspecified, == 4 for coil */
      thisribresptr->explicit = 0; /*number 1*/
      thisribresptr = allocribresstructure(thisribfragptr);/*for 2nd dummy res*/
      if(thisribresptr != NULL )
      {
        thisribresptr->explicit = 0; /*number 2*/
         thisribresptr = allocribresstructure(thisribfragptr); /*3rd dummy*/
         if(thisribresptr != NULL )
         {
            thisribresptr->explicit = 0; /*number 3*/
            thisribresptr = allocribresstructure(thisribfragptr); /*1st real*/
            if(thisribresptr != NULL )
            {
               thisribresptr->explicit = 0; /*number 4, might be reset below*/
            }
         }
      }
      Lnewribbonstart = 1; /*090219 to clear previousribbontype for ranges*/
      previousribbontype = 0; /*090219 clear the d--m global */
   }/*pad beginning with dummy residues so ribbon can tail off nicely*/

   /*}}}define and open a new ribresstruct for this residue*/

   if(thisribresptr != NULL) /*4 and greater are real residues, until last pad*/
   {/*ribres allocated*/

      /*{{{-- loop over all atoms of this residue */

      for(n=1;n<=maxatm;n++)
      {/*loop over all atoms*/

        /*take first instance of an atom in the list*/
        /*so SHOULD ONLY DO ALT A atoms if there are any */

        /*{{{---- look for NUCLEIC atoms */

        if(   atom[n][1]=='p' && atom[n][2]==' '
           && thisribresptr->pnuclptr == NULL     )
        {/*P is usually the first atom of a nucleotide*/
           thisribresptr->pnuclptr = allocribpntstructure(thisribresptr);
           if(thisribresptr->pnuclptr != NULL)
           {
              (thisribresptr->pnuclptr)->x = x[n];
              (thisribresptr->pnuclptr)->y = y[n];
              (thisribresptr->pnuclptr)->z = z[n];
           }
        }
        if(    atom[n][1]=='o' && atom[n][2]=='5'
            && thisribresptr->o5ribptr == NULL     )
        {/*P is usually the first atom of a nucleotide, but sometimes missing*/
            /*so pick up the O5' as possible replacement,*/
            /*if never find a P, this will be used as beginning atom*/
           if(   (thisribresptr->o5ribptr = allocribpntstructure(thisribresptr))
              != NULL)
           {
              (thisribresptr->o5ribptr)->x = x[n];
              (thisribresptr->o5ribptr)->y = y[n];
              (thisribresptr->o5ribptr)->z = z[n];
           }
        }
        if(    atom[n][1]=='c' && atom[n][2]=='3'
            &&(atom[n][3]=='*' || atom[n][3]=='\'')
            && thisribresptr->c3ribptr == NULL     )
        {
           thisribresptr->c3ribptr = allocribpntstructure(thisribresptr);
           if(thisribresptr->c3ribptr != NULL)
           {
              (thisribresptr->c3ribptr)->x = x[n];
              (thisribresptr->c3ribptr)->y = y[n];
              (thisribresptr->c3ribptr)->z = z[n];
           }
        }
        if(    atom[n][1]=='o' && atom[n][2]=='3'
            &&(atom[n][3]=='*' || atom[n][3]=='\'')
            && thisribresptr->o3ribptr == NULL     )
        {/*hold O3' to use as an approximation to a next PO4 at chain breaks*/
           thisribresptr->o3ribptr = allocribpntstructure(thisribresptr);
           if(thisribresptr->o3ribptr != NULL)
           {
              (thisribresptr->o3ribptr)->x = x[n];
              (thisribresptr->o3ribptr)->y = y[n];
              (thisribresptr->o3ribptr)->z = z[n];
           }
        }
        if(    atom[n][1]=='c' && atom[n][2]=='4'
            &&(atom[n][3]=='*' || atom[n][3]=='\'')
            && thisribresptr->c4ribptr == NULL     )
        {/*hold C4' to use as intermediate end point for doubled region ribbon*/
           if(   (thisribresptr->c4ribptr = allocribpntstructure(thisribresptr))
              != NULL)
           {
              (thisribresptr->c4ribptr)->x = x[n];
              (thisribresptr->c4ribptr)->y = y[n];
              (thisribresptr->c4ribptr)->z = z[n];
           }
        }
        if(    atom[n][1]=='c' && atom[n][2]=='1'
            &&(atom[n][3]=='*' || atom[n][3]=='\'')
            && thisribresptr->c1ribptr == NULL     )
        {
           if(   (thisribresptr->c1ribptr = allocribpntstructure(thisribresptr))
              != NULL)
           {
              (thisribresptr->c1ribptr)->x = x[n];
              (thisribresptr->c1ribptr)->y = y[n];
              (thisribresptr->c1ribptr)->z = z[n];
             /*set only on c1* atom for nucleic acid*/
              thisribresptr->numrib = num[n];
              strcpy(thisribresptr->resrib, res[n]);
              strcpy(thisribresptr->subrib, sub[n]);
              strcpy(thisribresptr->rinsrib, rins[n]);
              thisribresptr->Bvalrib = B[n];
           }
        }

        /*}}}---- look for NUCLEIC atoms */

        /*{{{---- look for PROTEIN atoms*/

        /*--atoms for fudged protein ribbons-- store all mc atoms for Hbonds*/
        if(   atom[n][1]=='c' && atom[n][2]=='a' && atom[n][3]==Q
           && thisribresptr->calphptr == NULL     )
        {
           if(   (thisribresptr->calphptr = allocribpntstructure(thisribresptr))
              != NULL)
           {
              (thisribresptr->calphptr)->x = x[n];
              (thisribresptr->calphptr)->y = y[n];
              (thisribresptr->calphptr)->z = z[n];
             /*set only on ca atom for protein*/
              thisribresptr->numrib = num[n];
              strcpy(thisribresptr->resrib, res[n]);
              strcpy(thisribresptr->subrib, sub[n]);
              strcpy(thisribresptr->rinsrib, rins[n]);
              thisribresptr->Bvalrib = B[n];
           }
        }
        if(   atom[n][1]=='n' && atom[n][2]==Q
           && thisribresptr->npeptptr == NULL     )
        {
           if(   (thisribresptr->npeptptr = allocribpntstructure(thisribresptr))
              != NULL)
           {
              (thisribresptr->npeptptr)->x = x[n];
              (thisribresptr->npeptptr)->y = y[n];
              (thisribresptr->npeptptr)->z = z[n];
           }
        }
        if(   atom[n][1]=='h' && atom[n][2]==Q
           && thisribresptr->hnpepptr == NULL     )
        {
           if(   (thisribresptr->hnpepptr = allocribpntstructure(thisribresptr))
              != NULL)
           {
              (thisribresptr->hnpepptr)->x = x[n];
              (thisribresptr->hnpepptr)->y = y[n];
              (thisribresptr->hnpepptr)->z = z[n];
           }
        }
        if(   atom[n][1]=='c' && atom[n][2]==Q
           && thisribresptr->ccarbptr == NULL     )
        {
           if(   (thisribresptr->ccarbptr = allocribpntstructure(thisribresptr))
              != NULL)
           {
              (thisribresptr->ccarbptr)->x = x[n];
              (thisribresptr->ccarbptr)->y = y[n];
              (thisribresptr->ccarbptr)->z = z[n];
           }
        }
        if(   atom[n][1]=='o' && atom[n][2]==Q
           && thisribresptr->ocarbptr == NULL     )
        {
           if(   (thisribresptr->ocarbptr = allocribpntstructure(thisribresptr))
              != NULL)
           {
              (thisribresptr->ocarbptr)->x = x[n];
              (thisribresptr->ocarbptr)->y = y[n];
              (thisribresptr->ocarbptr)->z = z[n];
           }
        }
        if(    atom[n][1]=='c' && atom[n][2]=='b' && atom[n][3]==Q
           && thisribresptr->cbetaptr == NULL     )
        {
           if(   (thisribresptr->cbetaptr = allocribpntstructure(thisribresptr))
              != NULL)
           {
              (thisribresptr->cbetaptr)->x = x[n];
              (thisribresptr->cbetaptr)->y = y[n];
              (thisribresptr->cbetaptr)->z = z[n];

              if(   (Lsite && siteoutl) || (Lsitefake && siteoutl)  /*050928*/
                 || (!Lsite && !Lsitefake) )
              {
                 thisribresptr->Lstub = 1;  /*050731*/
              }
           }
        }
        if(   atom[n][1]=='c' && atom[n][2]!=' ' && atom[n][2]!='b'
           && atom[n][2]!='a'
           && thisribresptr->cunknptr == NULL     )
        {
           if(   (thisribresptr->cunknptr = allocribpntstructure(thisribresptr))
              != NULL)
           {/*clunker carbon, e.g. acyl at Nterm,...  061011*/
              (thisribresptr->cunknptr)->x = x[n];
              (thisribresptr->cunknptr)->y = y[n];
              (thisribresptr->cunknptr)->z = z[n];
           }
        }

        /*}}}look for PROTEIN atoms*/

      }/*loop over all atoms*/

      if(Lribbonalso && !scoutl)
      {/*override for only specified sc 050206*/
         thisribresptr->Lstub = 0;
      }

      /*}}}loop over all atoms of this residue*/

    /*{{{-- check completeness, do tricks for protein and nucleic acid*/

    /*Protein vs Nucleic acid set by typechainfirstpass[level] for ribsection*/

    if(thisribsectionptr->type == 'P')
    /*{{{---- PROTEIN tricks*/
    {/*PROTEIN tricks*/
      if(   thisribsectionptr->type == 'P'
         && thisribresptr->calphptr != NULL
         && thisribresptr->ocarbptr == NULL
         &&(thisribresptr->previousptr)->ocarbptr != NULL )
      {
         /*special patch for green fluoresent protein 040406*/
         /*interior of a continuous stretch, fake a carbonyl oxygen 040406*/
         if(   (thisribresptr->ocarbptr = allocribpntstructure(thisribresptr))
            != NULL)
         {
            (thisribresptr->ocarbptr)->x
               = ((thisribresptr->previousptr)->ocarbptr)->x;
            (thisribresptr->ocarbptr)->y
               = ((thisribresptr->previousptr)->ocarbptr)->y;
            (thisribresptr->ocarbptr)->z
               = ((thisribresptr->previousptr)->ocarbptr)->z;
            sprintf(temps,"fake a carbonyl oxygen at %s %d (O of res -1)"CRLF
              ,sub[n],num[n]);
            pkintextinsert(temps);
            adjusttext(1);
#ifdef UNIX_X11
            if(!Lquiet)
            {
               fprintf(stderr,"fake a carbonyl oxygen at %s %d (O of res -1)\n"
                 ,sub[n],num[n]);
            }
#endif
            Lgotatoms = 1;
         }

      }
      else if(   thisribsectionptr->type == 'P'
              && thisribresptr->calphptr != NULL
              && thisribresptr->ocarbptr == NULL )
      {
         /*maybe Calpha only file, fake oxygen 041105, can do a worm*/
         if(   (thisribresptr->ocarbptr = allocribpntstructure(thisribresptr))
            != NULL)
         {
            (thisribresptr->ocarbptr)->x = (thisribresptr->calphptr)->x;
            (thisribresptr->ocarbptr)->y = (thisribresptr->calphptr)->y;
            (thisribresptr->ocarbptr)->z = (thisribresptr->calphptr)->z;
            Lgotatoms = 2;
         }
      }
      else if(   thisribsectionptr->type == 'P'
              && thisribresptr->calphptr == NULL
              && thisribresptr->ccarbptr != NULL)  /*061011*/
      {
         /*No Calpha named, but have a carbonyl carbon atom 061011*/
         if(   (thisribresptr->calphptr = allocribpntstructure(thisribresptr))
            != NULL)
         {/*fake a Calpha*/
            (thisribresptr->calphptr)->x = (thisribresptr->ccarbptr)->x;
            (thisribresptr->calphptr)->y = (thisribresptr->ccarbptr)->y;
            (thisribresptr->calphptr)->z = (thisribresptr->ccarbptr)->z;
            Lgotatoms = 3;
         }
      }
      else if(   thisribsectionptr->type == 'P'
              && thisribresptr->calphptr == NULL
              && thisribresptr->ccarbptr != NULL   /*070801*/
              && thisribresptr->cunknptr != NULL)  /*061011*/
      {
         /*No Calpha named, but have a clunker carbon atom 061011*/
         if(   (thisribresptr->calphptr = allocribpntstructure(thisribresptr))
            != NULL)
         {/*fake a Calpha*/
            (thisribresptr->calphptr)->x = (thisribresptr->cunknptr)->x;
            (thisribresptr->calphptr)->y = (thisribresptr->cunknptr)->y;
            (thisribresptr->calphptr)->z = (thisribresptr->cunknptr)->z;
            Lgotatoms = 4;
         }
      }
      else if(   thisribsectionptr->type == 'P'
              && thisribresptr->calphptr == NULL
              && thisribresptr->ocarbptr != NULL)  /*061011*/
      {
         /*No Calpha named, but have a carbonyl oxygen atom 061011*/
         if(   (thisribresptr->calphptr = allocribpntstructure(thisribresptr))
            != NULL)
         {/*fake a Calpha, beware: ca--o now zero length vector...*/
            (thisribresptr->calphptr)->x = (thisribresptr->ocarbptr)->x;
            (thisribresptr->calphptr)->y = (thisribresptr->ocarbptr)->y;
            (thisribresptr->calphptr)->z = (thisribresptr->ocarbptr)->z;
            Lgotatoms = 5;
         }
      }

      if(   thisribsectionptr->type == 'P'
              && thisribresptr->calphptr != NULL
              && thisribresptr->ocarbptr != NULL )
      {/*enough real atoms to do a ribbon properly*/
         Lgotatoms = 6; /*enough to do a ribbon*/

         /*EXTRA stuff for Protein Ribbons... */
         if(   (thisribresptr->hnpepptr == NULL) /*H of peptide NOT found*/
            && (thisribresptr->resrib[0] != 'p') ) /*not do pro */
         {/*try to compute hnpep*/
            if(   thisribresptr->npeptptr != NULL
               && thisribresptr->calphptr != NULL
               && thisribresptr->previousptr->ccarbptr != NULL)
            {/*the necessary atoms are present*/
               /* calculate coords for the h (section from nhgen.f) */
               /* .4775 jsr fudged "halfway" point: */
               /*   c-n-ca define a plane, */
               /*   a point xyz nearly halfway between c & ca */
               /*   is opposite the nh position.  */
               /*A normal vector in the xyz - n2xyz direction */
               /* points one unit out from n toward the nh position.*/
               /* Scale this and add to n2xyz to get nhxyz. */
               /*    (n-h ~ 1.0, so scale=1.0)  */

                  fx = (   (thisribresptr->previousptr->ccarbptr->x)
                           + (float)0.4775*(  (thisribresptr->calphptr->x)
                                  - (thisribresptr->previousptr->ccarbptr->x)));
                  fy = (   (thisribresptr->previousptr->ccarbptr->y)
                           + (float)0.4775*(  (thisribresptr->calphptr->y)
                                  - (thisribresptr->previousptr->ccarbptr->y)));
                  fz = (   (thisribresptr->previousptr->ccarbptr->z)
                           + (float)0.4775*(  (thisribresptr->calphptr->z)
                                  - (thisribresptr->previousptr->ccarbptr->z)));

                  factor = (float)(1.0
                    /(sqrt(  (   (thisribresptr->npeptptr->x - fx)
                               * (thisribresptr->npeptptr->x - fx) )
                            +(   (thisribresptr->npeptptr->y - fy)
                               * (thisribresptr->npeptptr->y - fy) )
                            +(   (thisribresptr->npeptptr->z - fz)
                               * (thisribresptr->npeptptr->z - fz) ) ) ) );

                  if(thisribresptr->hnpepptr == NULL)
                     {thisribresptr->hnpepptr =
                        allocribpntstructure(thisribresptr);}
                  if(thisribresptr->hnpepptr != NULL)
                  {
                     thisribresptr->hnpepptr->x =   thisribresptr->npeptptr->x
                         + factor*(thisribresptr->npeptptr->x - fx);
                     thisribresptr->hnpepptr->y =   thisribresptr->npeptptr->y
                         + factor*(thisribresptr->npeptptr->y - fy);
                     thisribresptr->hnpepptr->z =   thisribresptr->npeptptr->z
                         + factor*(thisribresptr->npeptptr->z - fz);
                  }
            }/*the necessary atoms are present*/
         }/*try to compute hnpep*/
      }/*enough to do a ribbon*/
      else
      {/*PROTEIN: partial or fully missing requisite atoms to make a ribbon*/
         Lgotatoms = 0;
         /*as of 051025 still no graceful way of dealing with imperfect data*/
         /*130406 prune flawed segment end residue, avoid singleton N segfault*/
 
         discon=1; /*force disconnection at this flawed, unuseable, residue*/
      }
    }/*PROTEIN tricks*/
    /*}}}---- PROTEIN tricks*/
    else if(thisribsectionptr->type == 'N')
    /*{{{---- NUCLEIC tricks*/
    {/*NUCLEIC tricks*/
      if     (   thisribsectionptr->type == 'N'
              && thisribresptr->pnuclptr != NULL
              && thisribresptr->c1ribptr != NULL
              && thisribresptr->c4ribptr != NULL )
      {
         Lgotatoms = 7;
      }
      /* dummy always starts so cannot use thisribresptr->previousptr == NULL*/
      /* to know first real residue of frag*/
      else if(   thisribsectionptr->type == 'N'
              && thisribresptr->o5ribptr != NULL
              && thisribresptr->c1ribptr != NULL
              && thisribresptr->c4ribptr != NULL )
      {  /* substitute o5 for missing first p */
           thisribresptr->pnuclptr = allocribpntstructure(thisribresptr);
           if(thisribresptr->pnuclptr != NULL) /*allocation succeeded*/
           {
              (thisribresptr->pnuclptr)->x = (thisribresptr->o5ribptr)->x;
              (thisribresptr->pnuclptr)->y = (thisribresptr->o5ribptr)->y;
              (thisribresptr->pnuclptr)->z = (thisribresptr->o5ribptr)->z;
              Lgotatoms = 8;
           }
           else  /*allocation failed*/
           {
              Lgotatoms = 0;
           }
      }
      else
      {/*partial or fully missing requisite atoms to make a ribbon*/
         Lgotatoms = 0;
         /*as of 051025 still no graceful way of dealing with imperfect data*/
         /*090219 try to use discon for this*/
      }

    }/*NUCLEIC tricks*/
    /*}}}---- NUCLEIC tricks*/
    else /*neither P nor N, no notion of how to use this in a ribbon*/
    {
       Lgotatoms = 0;
    }
    /*}}}check completeness, do tricks for protein and nucleic acid*/

    if(Lgotatoms) /*still good*/
    {/*full number of atoms in to do a ribbon seqment*/
    /*{{{-- if still good, try to define explicit sec str type from header*/
        /*  nucleic recognized by theribsectionptr->type */
        /* and uses explicitribbontype == BETA2D ranges */
        /* if(Lexplicitribbon && Lnucleicacid && !Lnucleicarrows) */
        /* { */
        /*    explicitribbontype = NUCLEIC2D; */
        /* } */

         /*general range control: */
         /* 0 Not explicit, 1 Alpha, 2 Beta, 4 Coil, 8 NUCLEIC, 16++ ARROW*/
         thisribresptr->explicit = explicitribbontype; /*from PDB HEADER */
         strcpy(thisribresptr->ribidentity,explicitribbonidentity);
         explicitribbonidentity[0] = '\0'; /*forget so must be reset*/

            /*explicitribbonidentity from SHEET record 060929*/
         /*only need 1,2,3,4 for PKINRIBB code*/
/* ALPHA2D    1 */
/* BETA2D     2 */
/* COIL2D     4 */
/* NUCLEIC2D  8 */
/* ALL2D           ALPHA2D | BETA2D | COIL2D   */
/* ARROW2D   16 */
/* ALPHAARROW2D  17 */
/* BETAARROW2D   18  bit-wise logic imposed 061021*/
/* SEGMENTS   12  */ /*050204 for VRMLxSectionCircle */

       /*  bit-wise logic imposed 061021 keep explicit knowledge
         if(thisribresptr->explicit == ALPHAARROW2D)
           {thisribresptr->explicit =  ALPHA2D;}
         if(thisribresptr->explicit == BETAARROW2D)
           {thisribresptr->explicit =  BETA2D;}
         if(thisribresptr->explicit == NUCLEIC2D)
           {thisribresptr->explicit =  BETA2D;}
       */
    /*}}}-- if still good, try to define explicit sec str type from header*/

    /*{{{-- check for disconnection because residues too far apart */
       if(!LfakeCaCaspline) /*111106*/
       {/*really do check*/
         /*check for disconnected fragments at evey residue */
         if(thisribresptr->previousptr != NULL)
         {/*at least on 2nd residue of a fragment*/
            if(thisribsectionptr->type == 'N')
            {/*nucleic*/
               if(   thisribresptr->pnuclptr != NULL
                  && (thisribresptr->previousptr)->pnuclptr != NULL )
               {
                  distsq =
                    (   (thisribresptr->pnuclptr)->x
                     - ((thisribresptr->previousptr)->pnuclptr)->x)
                   *(   (thisribresptr->pnuclptr)->x
                     - ((thisribresptr->previousptr)->pnuclptr)->x)
                   +(   (thisribresptr->pnuclptr)->y
                     - ((thisribresptr->previousptr)->pnuclptr)->y)
                   *(   (thisribresptr->pnuclptr)->y
                     - ((thisribresptr->previousptr)->pnuclptr)->y)
                   +(   (thisribresptr->pnuclptr)->z
                     - ((thisribresptr->previousptr)->pnuclptr)->z)
                   *(   (thisribresptr->pnuclptr)->z
                     - ((thisribresptr->previousptr)->pnuclptr)->z);
                  if(distsq > 100.0) {discon=1;} /*10sq vs 7sq*/
               }
            }/*nucleic*/
            else if(thisribsectionptr->type == 'P')
            {/*protein*/
               if(   thisribresptr->calphptr != NULL
                  && (thisribresptr->previousptr)->calphptr != NULL )
               {
                  distsq =
                    (   (thisribresptr->calphptr)->x
                     - ((thisribresptr->previousptr)->calphptr)->x)
                   *(   (thisribresptr->calphptr)->x
                     - ((thisribresptr->previousptr)->calphptr)->x)
                   +(   (thisribresptr->calphptr)->y
                     - ((thisribresptr->previousptr)->calphptr)->y)
                   *(   (thisribresptr->calphptr)->y
                     - ((thisribresptr->previousptr)->calphptr)->y)
                   +(   (thisribresptr->calphptr)->z
                     - ((thisribresptr->previousptr)->calphptr)->z)
                   *(   (thisribresptr->calphptr)->z
                     - ((thisribresptr->previousptr)->calphptr)->z);
                  if(distsq > 36.0) {discon=1;} /*6sq vs 3.8 sq*/
               }
            }/*protein*/
         }/*at least on 2nd residue of a fragment*/
       }/*really do check*/
    /*}}}-- check for disconnection because residues too far apart */
    }/*full number of atoms in to do a ribbon seqment*/
    /*{{{-- disconnection, alloc new fragment*/

    if(discon || !Lgotatoms)  /*090219, 130406 routine rearranged*/
    {/*this residue disconnects from previous residue: backoff counters */
            /*reset current global thisribfragptr->lastribresptr  */
            /* to the previous ribresptr */

            thisribfragptr->lastribresptr = thisribresptr->previousptr;
            thisribfragptr->lastribresptr->nextptr = NULL; /*an end*/
            /*back-off residue count of working fragment*/

            thisribfragptr->number--;

            thisribsectionptr->number--;

            thisribresptr = NULL;

            previousribbontype = 4; /*090219 clear flag before next residue*/
            explicitribbontype = 4; /*4==COIL*/

      if(Lgotatoms)
      {/*ribbon could continue in a new fragment with the current residue*/
     	    /*  Ribbon continues!? try to create new fragment...*/
            /*  alloc a new global thisribfragptr */
            /*  and start it with the current, disconnected ribresptr */
            /*  if this current residue has sufficient atoms, */
            /*  else skip and expect next residue to start new fragment */

            thisribfragptr = allocribfragstructure(thisribsectionptr);
                     /* allocates global thisribfragptr  051214*/

            if(thisribfragptr != NULL)
            {
               /*re-entrant to start another fragment with current residue*/
               /*reloads a newly allocated ribresptr in this new fragment*/
               /*thisribresptr->previousptr == NULL, cannot get here next time*/

               loadribbonatoms(); /*works on current inputed residue*/
            }
      }/*ribbon could continue in a new fragment with the current residue*/
    }/*this residue disconnected from previous residue*/
    /*}}}-- disconnection, alloc new fragment*/

   }/*ribres allocated*/
  }/*itr: iteration for multi-residue pseudo-hets like GFPchromophore*/
 
  return(Lgotatoms);
}
/*___loadribbonatoms()____________________________________________________}}}*/

/*{{{buildsectionguides()*****************************************************/
void buildsectionguides(ribsectionstruct* theribsectionptr)
{
  /*{{{-- declarations--and definitions                                      */

  /*ribbon section structure has the specific residue atoms to specify the */
  /*guide points for the splines of a section (prekin's notion of chains).*/
  /*Loop through the ribbon residue structures of the*/
  /*ribbon fragment structures of the*/
  /*ribbon section structures of the current ribbon model structure.*/

  /*Guide points and Spline points are stored back into the ribres structures*/
  /* (with some extra dummy residues added as needed to the dummy residues */
  /*  that were anticipated early as the residues were loaded). */

  ribfragstruct*    theribfragptr=NULL;
  ribresstruct*     theribresptr=NULL;
  ribresstruct*     nthresptr=NULL;
  ribresstruct*     nm1resptr=NULL;
  ribresstruct*     nm2resptr=NULL;
  ribresstruct*     nm3resptr=NULL;
  ribresstruct*     nm4resptr=NULL;
  ribresstruct*     nm5resptr=NULL;
  ribresstruct*     nm6resptr=NULL;
  ribresstruct*     nm7resptr=NULL;
  ribresstruct*     nm8resptr=NULL;
    /*nth is current residue under consideration (e.g. nm1==n-1)*/
    /*four residues needed to define curvature, etc. nth,nm1,nm2,nm3 */
    /*range of 3 fully defined needed to establish consistency: nm4,nm3,nm2 */
    /*Simple array indexing would make this code easier to understand*/
    /* rather than C type structures with internal pointer references*/
  float thispnt[4]={0,0,0,0};
  float nextpnt[4]={0,0,0,0};
  float nearpnt[4]={0,0,0,0};
  float distpnt[4]={0,0,0,0};
  float midpnt[4]={0,0,0,0};
  float avec[4]={0,0,0,0};
  float bvec[4]={0,0,0,0};
  float cvec[4]={0,0,0,0};
  float dvec[4]={0,0,0,0};
  float ppnt[4]={0,0,0,0};
  float amag=0,bmag=0,cmag=0,dmag=0;
  float wc=0;
  float maxoff=0,minoff=0,maxoffset=0,dist1=0,dist2=0,distance=0,delta=0;
  float fraction=0,widthfactor=0,radialfactor=0;
        /*do not use fudge, old common percent offset, as a variable name*/
  float minlimit=0,maxlimit=0; /*of extendedness 051211*/
  float extended=0; /*curvature fractional extendedness 051211*/
  float woffa=0,woffb=0;
  int   Lcurled=0,nuke=0;
  float pmid[4]={0,0,0,0},direc[4]={0,0,0,0},center[4]={0,0,0,0};
  int   j=0;
  int   LOK = 0;  /*070801*/
  float azmangle=0; /*dihedral CA-1,CA,CA+1,O azmuthal to Carbonyl 100322*/
  float prevpnt[4]={0,0,0,0}; /*for dihedral: CA-1 100322*/

  /*wc--width in coil, no  sec. structure */
  wc = ribwidcoil;
  /*woffa--width offset,up to this amount added when width fudged for alpha*/
  woffa = ribwidalpha - ribwidcoil; /*old woff==1.1*/
  /*woffb--width offset, up to this amount added when width fudged for beta*/
  woffb = ribwidbeta - ribwidcoil; /*old woff==1.1*/
  /*However, if Lfudgedwidth==0 then, e.g., ribwidalpha in designated alpha*/
  /*ribwidnucleic used straight*/

    if(theribsectionptr->type == 'N') {Lnucleicacid = 1;}
    else                              {Lnucleicacid = 0;}
    if(Lnucleicacid)
    {
       if     (LdoRNAribbon) {Lribonucleic = 1;} /*forced*/
       else if(LdoDNAribbon) {Lribonucleic = 0;} /*forced*/
       else if(theribsectionptr->kind == 'R') {Lribonucleic = 1;} /*voted*/
       else if(theribsectionptr->kind == 'D') {Lribonucleic = 0;} /*voted*/
       /*otherwise, whatever Lribonucleic happens to be set as */
    }
  /*}}}-- declarations--and definitions*/

  /*{{{-- loop through fragments and residues                                */
  theribfragptr = theribsectionptr->firstribfragptr;
  while(theribfragptr != NULL)
  {/*frag*/
    nm8resptr = NULL;
    nm7resptr = NULL;
    nm6resptr = NULL;
    nm5resptr = NULL;
    nm4resptr = NULL; /*range of 3: n-2,n-3,n-4 needed for consistency check*/
    nm3resptr = NULL; /* four residues must be considered to fully*/
    nm2resptr = NULL; /* decide the interval between 2nd & 3rd */
    nm1resptr = NULL; /*initialize ptr for these four rolling positions*/
    nthresptr = NULL; /*interval (n minus 2) needs info from nm3 & nth */
    /*each fragment has 3 dummy residues 1,2,3 at the N (5prime) end */
    /*and one extra residue at the C (3prime) end: more to be added at end. */

    /*{{{---- loop through the residues, & dummies, tracking recent residues */

    theribresptr = theribfragptr->firstribresptr;
    while(theribresptr != NULL)
    {/*res*/
      /*update the rolling pointers to current and near previous residues*/
      nm8resptr = nm7resptr;
      nm7resptr = nm6resptr;
      nm6resptr = nm5resptr;
      nm5resptr = nm4resptr;
      nm4resptr = nm3resptr;
      nm3resptr = nm2resptr;
      nm2resptr = nm1resptr;
      nm1resptr = nthresptr;
      nthresptr = theribresptr;

      /*each fragment has dummy residues 1,2,3 at the N (5prime) end */
      if(theribresptr->number < 4 )
      {
         ; /*drop down to increment residue just before end of while loop*/
      }
      /* 4 is first real one possible*/

      /*{{{------  in midst of inputed resides                               */
      else if(theribresptr->nextptr != NULL) /*there is a next to draw to*/
      {/*in midst of inputed resides: there is a real next residue*/
        /*{{{-------- get current working residue's critical atom points     */
        LOK = 0; /*presumed NOT OK */
        if(Lnucleicacid)
        {
          if(theribresptr->pnuclptr != NULL) /*070801 safety*/
          {
           /* if no initial p then o5' already sub-ed for it in pnuclptr*/
           thispnt[1] = theribresptr->pnuclptr->x;
           thispnt[2] = theribresptr->pnuclptr->y;
           thispnt[3] = theribresptr->pnuclptr->z;
           if(theribresptr->nextptr->pnuclptr != NULL)
           {/*next residue at least has some atom assigned as a phosphate*/
              nextpnt[1] = theribresptr->nextptr->pnuclptr->x;
              nextpnt[2] = theribresptr->nextptr->pnuclptr->y;
              nextpnt[3] = theribresptr->nextptr->pnuclptr->z;
              LOK = 1;
           }
           else if(theribresptr->nextptr->calphptr != NULL)
           {/*e.g. at end of charged tRNA--AA the next res is protein! 061004 */
              nextpnt[1] = theribresptr->nextptr->calphptr->x;
              nextpnt[2] = theribresptr->nextptr->calphptr->y;
              nextpnt[3] = theribresptr->nextptr->calphptr->z;
              LOK = 1;
           }
           if(theribresptr->c3ribptr!=NULL && theribresptr->c1ribptr!=NULL)
           {
              nearpnt[1] = theribresptr->c3ribptr->x;
              nearpnt[2] = theribresptr->c3ribptr->y;
              nearpnt[3] = theribresptr->c3ribptr->z;
              distpnt[1] = theribresptr->c1ribptr->x;
              distpnt[2] = theribresptr->c1ribptr->y;
              distpnt[3] = theribresptr->c1ribptr->z;
              LOK=1;
           }
           else
           {
              LOK=0;
           }
          }
          else
          {
             LOK=0;
          }
        }
        else /*protein*/
        {
		
          if(   theribresptr->nextptr->calphptr == NULL  /*100322*/
             && theribresptr->ccarbptr!=NULL  ) /*100322*/
          {/*next residue is not really there, use this C instead 100322*/
fprintf(stderr,"NEXT IS NULL, this is %d\n",theribresptr->numrib);
             if( (  theribresptr->nextptr->calphptr 
                  = allocribpntstructure(theribresptr->nextptr))
                != NULL)
             {
              (theribresptr->nextptr->calphptr)->x = theribresptr->ccarbptr->x;
              (theribresptr->nextptr->calphptr)->y = theribresptr->ccarbptr->y;
              (theribresptr->nextptr->calphptr)->z = theribresptr->ccarbptr->z;
             }
          }
          if(   theribresptr->calphptr!=NULL && theribresptr->ocarbptr!=NULL
             && theribresptr->nextptr->calphptr != NULL )
          {/*this residue has Calpha and Ocarb atoms defined */
            /*and next residue at least has some atom assigned as a Calpha */
            thispnt[1] = theribresptr->calphptr->x;
            thispnt[2] = theribresptr->calphptr->y;
            thispnt[3] = theribresptr->calphptr->z;

            nextpnt[1] = theribresptr->nextptr->calphptr->x;
            nextpnt[2] = theribresptr->nextptr->calphptr->y;
            nextpnt[3] = theribresptr->nextptr->calphptr->z;

            nearpnt[1] = theribresptr->calphptr->x;
            nearpnt[2] = theribresptr->calphptr->y;
            nearpnt[3] = theribresptr->calphptr->z;
            distpnt[1] = theribresptr->ocarbptr->x;
            distpnt[2] = theribresptr->ocarbptr->y;
            distpnt[3] = theribresptr->ocarbptr->z;
            LOK = 1;
            if(   theribresptr->previousptr != NULL /*for azmangle 100322 */
               && theribresptr->previousptr->calphptr != NULL)
            {
               prevpnt[1] = theribresptr->previousptr->calphptr->x;
               prevpnt[2] = theribresptr->previousptr->calphptr->y;
               prevpnt[3] = theribresptr->previousptr->calphptr->z;
            }
            else if(theribresptr->npeptptr!=NULL) /*azmangle 100322*/
            {/*use N, beware poorly defined dihedral: azmangle 100322*/
               prevpnt[1] = theribresptr->npeptptr->x;
               prevpnt[2] = theribresptr->npeptptr->y;
               prevpnt[3] = theribresptr->npeptptr->z;
            }
            else /*reuse Calpha for an undefined dihedral=0, azmangle 100322*/
            {/*use N, beware poorly defined dihedral: azmangle 100322*/
               prevpnt[1] = thispnt[1];
               prevpnt[2] = thispnt[2];
               prevpnt[3] = thispnt[3];
            }                                       /*for azmangle 100322 */

          }
          else
          {
             LOK=0;
          }
        }
        /*}}}-------- get current working residue's critical atom points     */

        if(LOK)
        {
        /*{{{-------- define ribbon plane, and associated vectors            */

         /*{{{---------- vector A, vector B --> vectors C and D               */
        /*--define ribbon plane, and associated vectors */

        /*--vector A = ca+1 - ca  Protein      */
        /*--vector B = o - ca */
        /*--vector C = AxB normal to peptide plane ca,o,ca+1, */
        /*--points out of RH helix */
        /*--vector D = CxA parallel to peptide plane, perpendicular to A */

        /*--vector A = p+1 - p  Nucleic Acid */
        /*--vector B = c1' - c3' */
        /*--vector C = AxB normal to pseudo-plane p,c3',c1',p+1, */
        /*--vector D = CxA parallel to pseudo-plane, perpendicular to A */

        avec[1] = nextpnt[1] - thispnt[1];
        avec[2] = nextpnt[2] - thispnt[2];
        avec[3] = nextpnt[3] - thispnt[3];

        /*--vector B = c1' - c3' */
        /*--vector B = o   - ca  */

        if(Lca3ribbon) /*130121 rib plane use ca-1,ca,ca+1 perp instead of C=O*/
        {
           cvec[1] = thispnt[1] - prevpnt[1];
           cvec[2] = thispnt[2] - prevpnt[2];
           cvec[3] = thispnt[3] - prevpnt[3];
           /*crossproduct*/
           bvec[1] = avec[2]*cvec[3] - cvec[2]*avec[3];
           bvec[2] = avec[3]*cvec[1] - cvec[3]*avec[1];
           bvec[3] = avec[1]*cvec[2] - cvec[1]*avec[2];
        }
        else /*distpnt is Carbonyl Oxygen or surrogate*/
        {
           bvec[1] = distpnt[1] - nearpnt[1];
           bvec[2] = distpnt[2] - nearpnt[2];
           bvec[3] = distpnt[3] - nearpnt[3];
        }
        /*--vector C = AxB normal to pseudo-plane p,c3',c1',p+1, */
        /*--vector C = AxB normal to peptide plane ca,o,ca+1, */
        /*             points out of RH helix */
        cvec[1] = avec[2]*bvec[3] - bvec[2]*avec[3];
        cvec[2] = avec[3]*bvec[1] - bvec[3]*avec[1];
        cvec[3] = avec[1]*bvec[2] - bvec[1]*avec[2];

        /*--vector D = CxA parallel to pseudo-plane,  perpendicular to A*/
        /*--vector D = CxA parallel to peptide plane, perpendicular to A*/
        dvec[1] = cvec[2]*avec[3] - avec[2]*cvec[3];
        dvec[2] = cvec[3]*avec[1] - avec[3]*cvec[1];
        dvec[3] = cvec[1]*avec[2] - avec[1]*cvec[2];

        /*--normalize C & D */
        cmag = (float)(sqrt(  cvec[1]*cvec[1]
                            + cvec[2]*cvec[2]
                            + cvec[3]*cvec[3] ));
        if(cmag > 0.01 || theribresptr->number < 4)/*see else,was this,100322*/
        {
           cvec[1] = cvec[1]/cmag;
           cvec[2] = cvec[2]/cmag;
           cvec[3] = cvec[3]/cmag;
           dmag = (float)(sqrt(  dvec[1]*dvec[1]
                               + dvec[2]*dvec[2]
                               + dvec[3]*dvec[3] ) );
           dvec[1] = dvec[1]/dmag;
           dvec[2] = dvec[2]/dmag;
           dvec[3] = dvec[3]/dmag;
        }
        else /*cmag very small...*/
        {
          /*alternate conformation with the same mainchain */
          /* will make c-vector zero*/
          /*rather than figure out what really should be done, */
          /*just set the d-vector to an earlier value if cmag very small*/
            /*BEWARE: PREVIOUS NOT DEFINED FOR 1st residue == #3 */
           cvec[1] = theribresptr->previousptr->cvecnptr->x;/*was this,100322*/
           cvec[2] = theribresptr->previousptr->cvecnptr->y;/*was this,100322*/
           cvec[3] = theribresptr->previousptr->cvecnptr->z;/*was this,100322*/
           dvec[1] = theribresptr->previousptr->dvecnptr->x;/*was this,100322*/
           dvec[2] = theribresptr->previousptr->dvecnptr->y;/*was this,100322*/
           dvec[3] = theribresptr->previousptr->dvecnptr->z;/*was this,100322*/
        }
        if(LvectCOpdb) /*only Calpha,azimuth --> pdb format 100322*/
        {
            theribresptr->azmangle = (float)dihedral4pt( /*was this,100322*/
                (double)prevpnt[1]
               ,(double)prevpnt[2]
               ,(double)prevpnt[3]
               ,(double)thispnt[1]
               ,(double)thispnt[2]
               ,(double)thispnt[3]
               ,(double)nextpnt[1]
               ,(double)nextpnt[2]
               ,(double)nextpnt[3]
               ,(double)distpnt[1]
               ,(double)distpnt[2]
               ,(double)distpnt[3]
              );
fprintf(stderr,"CA %d: %8.3f,%8.3f,%8.3f "
,theribresptr->numrib,(double)thispnt[1],(double)thispnt[2],(double)thispnt[3]);
fprintf(stderr,"CA %d: %8.3f,%8.3f,%8.3f\n"
,theribresptr->numrib+1,(double)nextpnt[1],(double)nextpnt[2],(double)nextpnt[3]);
        }              /*only Calpha,azimuth --> pdb format 100322*/

        /*}}}--------- vector A, vector B --> vectors C and D                */

         /*{{{---------- adjust D vs C for nucleics, set guide ==midpoint of A*/
        if((Lnucleicacid && !Lribonucleic)||(Lpeptideperprib))
        {/*use c as guide point direction not d */
           dvec[1] = cvec[1];
           dvec[2] = cvec[2];
           dvec[3] = cvec[3];
        }

        /*-- guide point: midpoint of p---p+1   */
        /*-- guide point: midpoint of ca---ca+1 */
        midpnt[1] = ( thispnt[1] + nextpnt[1] )/2;
        midpnt[2] = ( thispnt[2] + nextpnt[2] )/2;
        midpnt[3] = ( thispnt[3] + nextpnt[3] )/2;


        /*-----------------------------*/
        /* b-spline guide points center on midpnt and go along D */
        /* to the side for a ribbon width */
        /* guidepnt == midpnt but  may be displaced for the geom-fudge */
        /*ribbon width factor to get actual g guide points */
        /* can also could be fudged by curvature */
        /*}}}---------- adjust D vs C for nucleics, set guide ==midpoint of A*/

         /*{{{---------- adjust flip state of ribbon for smoother twist       */
        if(theribresptr->number > 4) /*need successive real residues*/
        {/*smooth the ribbon twist*/
           /*optional: Mike Been: twist in Ribozymes may be meaningfull*/
           if(Lsmoothtwist) /* but smoothtwist is default, 010515*/
           {/*smooth out the twist along main chain*/
              if( (  theribresptr->previousptr->dvecnptr->x * dvec[1]
                   + theribresptr->previousptr->dvecnptr->y * dvec[2]
                   + theribresptr->previousptr->dvecnptr->z * dvec[3]) < 0 )
              {/*do a flip of the D vector */
                 dvec[1] = -dvec[1];
                 dvec[2] = -dvec[2];
                 dvec[3] = -dvec[3];
              }
           }
        }/*smooth the ribbon twist*/
        /*}}}adjust flip state of ribbon for smoother twist */

         /*{{{---------- annotate strand--peptide orientation                 */
        if(!Lnucleicacid)
        {
           /*vector B points from Calpha toward carbonyl Oxygen*/
           /*B dot D is positive if odd strands on carboyl side of peptide*/
           if((bvec[1])*(dvec[1])+(bvec[2])*(dvec[2])+(bvec[3])*(dvec[3]) >= 0)
                {theribresptr->COside = 1;} /*nh at b4, co at b3*/
           else {theribresptr->COside = 2;} /*nh at b3, co at b4*/

        }
        /*}}}annotate strand--peptide orientation*/

         /*{{{---------- load actual ribres-structure with vectors and points */
        theribresptr->avecaptr->x = avec[1];
        theribresptr->avecaptr->y = avec[2];
        theribresptr->avecaptr->z = avec[3];
        theribresptr->cvecnptr->x = cvec[1];
        theribresptr->cvecnptr->y = cvec[2];
        theribresptr->cvecnptr->z = cvec[3];
        theribresptr->dvecnptr->x = dvec[1];
        theribresptr->dvecnptr->y = dvec[2];
        theribresptr->dvecnptr->z = dvec[3];
        theribresptr->middlptr->x = midpnt[1];  /*orig middle pt, for TEST */
        theribresptr->middlptr->y = midpnt[2];
        theribresptr->middlptr->z = midpnt[3];
        theribresptr->axialptr->x = midpnt[1];  /*orig middle pt, for axis*/
        theribresptr->axialptr->y = midpnt[2];
        theribresptr->axialptr->z = midpnt[3];
        theribresptr->guideptr->x = midpnt[1];
        theribresptr->guideptr->y = midpnt[2];
        theribresptr->guideptr->z = midpnt[3];

        /*tentative width set at coil default */
        if(Lnucleicacid) wc = ribwidnucleic;
        else wc = ribwidcoil;

        theribresptr->widthpossible = wc;
        theribresptr->width = wc;
        /*}}}load actual ribres- structure with the vectors and points*/

         /*{{{---------- at first real residue, 3 dummy residues begins spline*/
        if(theribresptr->number == 4) /*fourth ribresptr is 1st real one*/
        {/*theribresptr->number == 4*/
         /*first dummy residues are different to tail ribbon in smoothly*/
           /*extend back to earlier (dummy) residues*/
           /*--extend back half distance of vector A */
           /* (back-extension has evolved to be essentially at 1st atom)*/
           /*nm1resptr == theribresptr->previousptr*/ /* was this... 100322*/
           nm1resptr->guideptr->x = midpnt[1] -avec[1]/2;
           nm1resptr->guideptr->y = midpnt[2] -avec[2]/2;
           nm1resptr->guideptr->z = midpnt[3] -avec[3]/2;
           nm1resptr->axialptr->x = nm1resptr->guideptr->x;
           nm1resptr->axialptr->y = nm1resptr->guideptr->y;
           nm1resptr->axialptr->z = nm1resptr->guideptr->z;

           nm1resptr->avecaptr->x = avec[1];
           nm1resptr->avecaptr->y = avec[2];
           nm1resptr->avecaptr->z = avec[3];
           nm1resptr->cvecnptr->x = cvec[1];
           nm1resptr->cvecnptr->y = cvec[2];
           nm1resptr->cvecnptr->z = cvec[3];
           nm1resptr->dvecnptr->x = dvec[1];
           nm1resptr->dvecnptr->y = dvec[2];
           nm1resptr->dvecnptr->z = dvec[3];
           nm1resptr->widthpossible = wc;
           nm1resptr->width = wc;
           /*nm1resptr->explicit    already set == 0 */
           nm1resptr->numrib = theribresptr->numrib;
           for(j=0;j<4;j++)
              {nm1resptr->resrib[j]=theribresptr->resrib[j];}
           for(j=0;j<3;j++) /*2char + endstr  070926*/
           {
              nm1resptr->subrib[j] = theribresptr->subrib[j];
              nm1resptr->rinsrib[j] = theribresptr->rinsrib[j];
           }
           nm1resptr->Bvalrib = theribresptr->Bvalrib;
           nm1resptr->Lstub = 0; /*not reasonable for CB-stub*/

           /*duplicate: dummy n-2 = copy of dummy n-1 */
           nm2resptr->guideptr->x = nm1resptr->guideptr->x;
           nm2resptr->guideptr->y = nm1resptr->guideptr->y;
           nm2resptr->guideptr->z = nm1resptr->guideptr->z;
           nm2resptr->axialptr->x = nm1resptr->axialptr->x;
           nm2resptr->axialptr->y = nm1resptr->axialptr->y;
           nm2resptr->axialptr->z = nm1resptr->axialptr->z;

           nm2resptr->avecaptr->x = nm1resptr->avecaptr->x;
           nm2resptr->avecaptr->y = nm1resptr->avecaptr->y;
           nm2resptr->avecaptr->z = nm1resptr->avecaptr->z;
           nm2resptr->cvecnptr->x = nm1resptr->cvecnptr->x;
           nm2resptr->cvecnptr->y = nm1resptr->cvecnptr->y;
           nm2resptr->cvecnptr->z = nm1resptr->cvecnptr->z;
           nm2resptr->dvecnptr->x = nm1resptr->dvecnptr->x;
           nm2resptr->dvecnptr->y = nm1resptr->dvecnptr->y;
           nm2resptr->dvecnptr->z = nm1resptr->dvecnptr->z;
           nm2resptr->widthpossible = nm1resptr->widthpossible;
           nm2resptr->width = nm1resptr->width;
           /*nm2resptr->explicit     already set == 0 */
           nm2resptr->numrib = nm1resptr->numrib;
           for(j=0;j<4;j++)
              {nm2resptr->resrib[j]=nm1resptr->resrib[j];}
           for(j=0;j<3;j++) /*2char + endstr  070926*/
           {
              nm2resptr->subrib[j] = nm1resptr->subrib[j];
              nm2resptr->rinsrib[j] = nm1resptr->rinsrib[j];
           }
           nm2resptr->Bvalrib = nm1resptr->Bvalrib;
           nm2resptr->Lstub = 0; /*not reasonable for CB-stub*/

           /*duplicate: dummy n-3 = copy of dummy n-1 */
           nm3resptr->guideptr->x = nm1resptr->guideptr->x;
           nm3resptr->guideptr->y = nm1resptr->guideptr->y;
           nm3resptr->guideptr->z = nm1resptr->guideptr->z;
           nm3resptr->axialptr->x = nm1resptr->axialptr->x;
           nm3resptr->axialptr->y = nm1resptr->axialptr->y;
           nm3resptr->axialptr->z = nm1resptr->axialptr->z;

           nm3resptr->avecaptr->x = nm1resptr->avecaptr->x;
           nm3resptr->avecaptr->y = nm1resptr->avecaptr->y;
           nm3resptr->avecaptr->z = nm1resptr->avecaptr->z;
           nm3resptr->cvecnptr->x = nm1resptr->cvecnptr->x;
           nm3resptr->cvecnptr->y = nm1resptr->cvecnptr->y;
           nm3resptr->cvecnptr->z = nm1resptr->cvecnptr->z;
           nm3resptr->dvecnptr->x = nm1resptr->dvecnptr->x;
           nm3resptr->dvecnptr->y = nm1resptr->dvecnptr->y;
           nm3resptr->dvecnptr->z = nm1resptr->dvecnptr->z;
           nm3resptr->widthpossible = nm1resptr->widthpossible;
           nm3resptr->width = nm1resptr->width;
           /*nm3resptr->explicit     already set == 0 */
           nm3resptr->numrib = nm1resptr->numrib;
           for(j=0;j<4;j++)
              {nm3resptr->resrib[j]=nm1resptr->resrib[j];}
           for(j=0;j<3;j++) /*2char + endstr  070926*/
           {
              nm3resptr->subrib[j] = nm1resptr->subrib[j];
              nm3resptr->rinsrib[j] = nm1resptr->rinsrib[j];
           }
           nm3resptr->Bvalrib = nm1resptr->Bvalrib;
           nm3resptr->Lstub = 0; /*not reasonable for CB-stub*/
        }/*theribresptr->number == 4*/
        /*}}}at first real residue, define first dummy residues*/

        /*}}}-------- define ribbon plane, and associated vectors            */

        /*{{{-------- calc curvature-dependent offsets and width variations  */

        /*geom fudge based on the distance of ca[n-4] -> ca[n]*/
        /*down to 5 A for alpha & turns, so over range of 7 -> 5*/
        /*distance up to 11 A for beta or extended chain, range 9 -> 11*/
        /*fudge width across D & displacement of guide point (midpnt)*/
        /* maxoff is the distance threshold for curvature offset */

        if(theribresptr->number >= 7 )/*forth ribresptr is 1st real one*/
        {/*number >= 7*/
          /*at least 4 real residues, with nth, can judge curvature at n-2*/

          /*{{{-------- nucleicacid extendedness                             */
          if(Lnucleicacid)
          {/*nucleic acid*/
             /* extendedness of nucleics is not a simple calculation*/
             /* it takes into account the complicated bended allowed by 7D */
             maxoff = (float)9.0;
             minoff = (float)7.0;
             maxoffset = (float)2.5;
             /*040421 look for P zigzag where p[2] is mid pt of P2---P3*/
             dist1 =
               distanribpntptrs(nm3resptr->pnuclptr,nm2resptr->guideptr);
             dist2 =
               distanribpntptrs(nthresptr->pnuclptr,nm2resptr->guideptr);
             /*CURVATURE DISTANCE*/
             distance = dist1; /*distance needs to be defined for each res*/
             if(dist1 >= maxoff && dist2 >= maxoff)
             {
                 Lcurled = 0;
                 nuke = 0;
             }
             else if(dist1 <= maxoff && dist2 <= maxoff)
             {
                 if(dist1 < dist2){distance = dist1; nuke = 1;}
                 else             {distance = dist2; nuke = 4;}
                 Lcurled = 1;
                 /*nuke specifies P1 or P4 as offset target direction*/
             }
             else if(dist1 <= maxoff)
             {
                 distance = dist1;
                 Lcurled = 1; nuke = 1;
             }
             else if(dist2 <= maxoff)
             {
                 distance = dist2;
                 Lcurled = 1; nuke = 4;
             }

             /*calc fractional extendedness 051211*/
             /*curvature judged on bending around P--sugar--P  */
             /* takes into account distance to nearest flanking neighbor P */
             minlimit = minoff; /* ~limit of most tightly curled*/
             maxlimit = maxoff; /* ~limit of most extended*/

             if(distance < minlimit) {extended = 0;}
             else if(distance > maxlimit) {extended = 1;}
             else {extended = (distance - minlimit)/(maxlimit - minlimit);}

             /* nm2resptr->explicit = NUCLEIC2D; would override ranges */
             if(Lcurled && Lc4primenucleics)
             {/*offset to be done, */
                /*calc max dist from mid P2--P3 to c4prime*/
                maxoffset =
                   distanribpntptrs(nm2resptr->c4ribptr,nm2resptr->guideptr);
                /* at extreme the guide point is put at c4prime atom position*/
                /* maximum offset is maxoffset = midpoint-to-c4prime distance*/
                maxoffset = maxoffset + 1; /*allow guide pt to go beyond c4' */

                /*fully extended nucleic: mid-point 2P----3P to 1P or 4P: ~9A,*/
                /* shortest approach puts P--midPP ~5A*/
                /*so range is 9 -- 5, question of when to get to extreme*/
                minoff = (float)7;
                delta = maxoff - distance;
                if(delta > maxoff - minoff){delta = maxoff - minoff;}
                radialfactor = delta/(maxoff-minoff);
                /*nucleics do not use a widthfactor, at least for now*/
                widthfactor = 0; /*i.e. no extra width beyond standard*/
             }
             if( Lcurled &&( (Lcurvednucleics && nuke) || Lc4primenucleics ) )
             {/*nucleic acid tightly curved*/
               /*may need shorter interval for nucl curvature, eg S motif*/
               if( Lc4primenucleics) /*040422*/
               {
                  pmid[1] = nm2resptr->c4ribptr->x;
                  pmid[2] = nm2resptr->c4ribptr->y;
                  pmid[3] = nm2resptr->c4ribptr->z;
                  maxoffset = -maxoffset;
                     /*pull toward c4prime rather than push from a mid point*/
               }
               else
               {
                  if(nuke==1)
                  {
                     pmid[1] = nm3resptr->pnuclptr->x;
                     pmid[2] = nm3resptr->pnuclptr->y;
                     pmid[3] = nm3resptr->pnuclptr->z;
                  }
                  else if(nuke==4)
                  {
                     pmid[1] = nthresptr->pnuclptr->x;
                     pmid[2] = nthresptr->pnuclptr->y;
                     pmid[3] = nthresptr->pnuclptr->z;
                  }
               }
             }/*nucleic acid tightly curved*/
             else
             {
                Lcurled = 0; /*nucleic and not subject to curled offset*/
             }
          }/*nucleic acid*/

          /*}}}-------- nucleicacid extendedness                             */

          /*{{{-------- protein extendedness                                 */
          else
          {/*protein*/
             /*CURVATURE DISTANCE*/

             distance  =
                distanribpntptrs(nthresptr->calphptr, nm3resptr->calphptr);
                /*distance used to delineate tightly curled regions where */
                /* both radial offset and width offset are done */
                /* and distance used to delineate extended regions where */
                /* only width offset is done */

             /*nm2resptr->explicit = COIL2D; don't override ranges */

             /*look for tightly curled region to see if radial offset needed*/

             /*maxoff is longest limit Calpha1---Calpha4 considered curled*/
             maxoff = (float)7.0; /* default for protein ribbons */
             maxoffset = (float)1.5; /*default for protein*/
             if(distance < maxoff){ Lcurled = 1;}
             else {Lcurled = 0;}
             if(Lcurled)
             {
               delta = maxoff - distance;
               if(delta < 0)             {delta = 0; fraction = 0;}
               else if(delta > maxoffset){delta = maxoffset; fraction = 1;}
               else                      {fraction = delta/maxoffset;}
               radialfactor = fraction;
               widthfactor = fraction;
             }
             if( Lcurled)
             {/*protein tightly curved*/
                /*treat as alpha or turn */
                /* CA1 to CA4 is measure of curvature and offset center*/
                pmid[1] = (nthresptr->calphptr->x + nm3resptr->calphptr->x)/2;
                pmid[2] = (nthresptr->calphptr->y + nm3resptr->calphptr->y)/2;
                pmid[3] = (nthresptr->calphptr->z + nm3resptr->calphptr->z)/2;
             }/*protein tightly curved*/

             /*calc fractional extendedness 051211*/
             /*curvature judged on Calpha1---Calpha4 distance */
             minlimit = (float)5; /* ~limit of most tightly curled protein*/
             maxlimit = (float)11; /* ~limit of most extended protein*/

             if(distance < minlimit) {extended = 0;}
             else if(distance > maxlimit) {extended = 1;}
             else {extended = (distance - minlimit)/(maxlimit - minlimit);}

          }/*protein*/
          /*}}}-------- protein extendedness                                 */

          /*{{{-------- apply extendedness factors                           */

          /* store fractional extendedness 051211 */
          nm2resptr->extend = extended;
             /*NOTE: this is extendedness of backbone chain, */
               /*  NOT curvature of ribbon splines*/

          if(Lcurled)
          {/*tightly curved*/
             /*radial direction offset from pmid -> guidept between 2&3 (n-2)*/
             direc[1] = nm2resptr->guideptr->x - pmid[1];
             direc[2] = nm2resptr->guideptr->y - pmid[2];
             direc[3] = nm2resptr->guideptr->z - pmid[3];
             center[1] = (nm2resptr->guideptr->x + pmid[1])/2;
             center[2] = (nm2resptr->guideptr->y + pmid[2])/2;
             center[3] = (nm2resptr->guideptr->z + pmid[3])/2;
             /*approx center of bend, use for last vector if end in helix*/
             /*normalize direc */
             cmag = (float)(sqrt(   direc[1]*direc[1]
                                  + direc[2]*direc[2]
                                  + direc[3]*direc[3] ) );
             direc[1] = direc[1]/cmag;
             direc[2] = direc[2]/cmag;  /*normalized radial offset direction */
             direc[3] = direc[3]/cmag;
             /*move guide point of n-2 by radialfactor*maxoffset*/
             if(!LNOfudge) /*050327*/
             {
                nm2resptr->guideptr->x =
                     nm2resptr->guideptr->x + radialfactor*maxoffset*direc[1];
                nm2resptr->guideptr->y =
                     nm2resptr->guideptr->y + radialfactor*maxoffset*direc[2];
                nm2resptr->guideptr->z =
                     nm2resptr->guideptr->z + radialfactor*maxoffset*direc[3];

                nm2resptr->axialptr->x =
                     nm2resptr->axialptr->x - radialfactor*maxoffset*direc[1];
                nm2resptr->axialptr->y =
                     nm2resptr->axialptr->y - radialfactor*maxoffset*direc[2];
                nm2resptr->axialptr->z =
                     nm2resptr->axialptr->z - radialfactor*maxoffset*direc[3];
                /*060115 guide points for axis spline */
             }
             if(!Lnucleicacid) /*040420*/
             {
               /*set up for width fudging, only apply if 3 in a row are fudged*/
               /* midpoint between Calphas is moved by offset for curvature*/
               /*d perpendicular to Calphas not moved, just used for direction*/
               if(Lfudgedwidth)
               {
                  nm2resptr->widthpossible = wc + widthfactor*woffa;
               }
               else if(LNOfudge) /*050327*/
               {
                  nm2resptr->widthpossible = wc;
               }
               else
               {
                  nm2resptr->widthpossible = wc + woffa;
               }
               /*nm2resptr->explicit = ALPHA2D; this would override ranges*/
             }
          }/*tightly curved*/
          if( distance > 9.0 && !Lnucleicacid )
          {/*low curvature: protein extended chain*/
             /*may be in beta, set up for fudged variable width */
             minoff = (float)9.0; /* default for protein ribbons */
             delta = (float)(distance - minoff);
             if (delta > 1.5)   {delta = 1.5;}
             else if(delta < 0) {delta = 0;}

             widthfactor = (float)(delta/1.5); /*fraction*/

             if(Lfudgedwidth)
             {
                nm2resptr->widthpossible = wc + widthfactor*woffb;
             }
             else if(LNOfudge) /*050327*/
             {
                nm2resptr->widthpossible = wc;
             }
             else
             {
                nm2resptr->widthpossible = wc + woffb;
             }
             /*nm2resptr->explicit = BETA2D; this would override ranges*/
          }/*low curvature: protein extended chain*/

          if(theribresptr->number >= 8)/*while processing all with number >= 6*/
          {/*number>=8: at least 6 real residues, so 3 real distances measured*/
             /*Once past the 7th real residue, have full knowledge about */
             /*n-4,n-1,n-2 and have set params except for continuity for width*/
             /*can widen 3 if n-4 & n-3 & n-2 all could be widened*/
             /*once three in a row widened, keep doing it for new wide ones*/
             /*easier to keep resetting all three then doing logical test*/
             if(    nm2resptr->widthpossible>wc   /*width res4 known at num==8*/
                 && nm3resptr->widthpossible>wc   /*width res3 */
                 && nm4resptr->widthpossible>wc ) /*width res2 */
             {
                nm2resptr->width = nm2resptr->widthpossible;
                nm3resptr->width = nm3resptr->widthpossible;
                nm4resptr->width = nm4resptr->widthpossible;
             }

/*
  1st known distance at resid4 nth==7, can calc width for resid2 at nth == 9
                               w5     w6     w7  at        nth==9
   n-8    n-7    n-6    n-5    n-4    n-3    n-2    n-1    nth==9
   n-7    n-6    n-5    n-4    n-3    n-2    n-1    nth==8
   n-6    n-5    n-4    n-3    n-2    n-1    nth
    1      2      3      4      5      6      7     .8.    .9.   number   ...
                         |      |      |      |      |      |      .
 dummy0 dummy1 dummy2 resid1 resid2 resid3 resid4   .5.N   .6. res-last dummy
    dup==  <-ext--|avec->|      |      |      |      |      |     ->
                         |------distance----->|
       |      |      |      |      |      |      |
                                 width2 width3 width4 width5
                                       |------distance----->|
                                                     |
                                            residueN's width, number==N+2
*/

             if((theribresptr->number == 9) /*only do this at 6th real residue*/
               && LNtermBetaFudge
               && (nm2resptr->width > wc)  ) /* n-2 interval is set wide*/
             {/*at number 9: adjust widths back to beginning of spline*/
               /*adjust widths back to beginning of spline*/
               /*At chain beginning: if chain begins in helix or beta: */
               /* then since width cannot be fudged at real 2--3 interval */
               /*   (i.e. witdthpossible set > wc)  */
               /* until 4th real point(distance  #4-#1 for #2),*/
               /* and local fudging only goes back to n-4, */
               /* beginning spline can never be widened except by special case*/

               /*--first, correct the tentative widths...*/
               /* dummy intervals are n-6, n-7, n-8  at nth==9*/
               /*n-4, res2: first known width, also fudge n-5 res1 */
               nm8resptr->widthpossible = nm4resptr->widthpossible;
               nm7resptr->widthpossible = nm4resptr->widthpossible;
               nm6resptr->widthpossible = nm4resptr->widthpossible;
               nm5resptr->widthpossible = nm4resptr->widthpossible;

               /* apply fudged widths to n-8, n-7, n-6, n-5  first 4 intervals*/
               nm8resptr->width = nm8resptr->widthpossible;
               nm7resptr->width = nm7resptr->widthpossible;
               nm6resptr->width = nm6resptr->widthpossible;
               nm5resptr->width = nm5resptr->widthpossible;

               /*--similarly for explicit designations--*/
               /*090219 seems nm4 is not the one to carry back...*/
               /*   nm8resptr->explicit = nm4resptr->explicit; */
               /*   nm7resptr->explicit = nm4resptr->explicit; */
               /*   nm6resptr->explicit = nm4resptr->explicit; */
               /*   nm5resptr->explicit = nm4resptr->explicit; */

               /* nm8resptr->explicit = nm5resptr->explicit;  */
               /* nm7resptr->explicit = nm5resptr->explicit;  */
               /* nm6resptr->explicit = nm5resptr->explicit;  */

             }/*at number 9: adjust widths back to beginning of spline*/
          }/*number>=9: at least 6 real residues, so 3 real distances measured*/
          /*}}}-------- apply extendedness factors                           */

        }/*number >= 7*/

        /*}}}-------- calc curvature-dependent offsets and width variations  */
        }/*LOK*/

      }/*in midst of inputed resides: there is a real next residue*/

      /*}}}------  in midst of inputed resides                               */

      /*{{{------  cycle, or at last residue, make ultimate dummy guidepoint */
      if(theribresptr == theribfragptr->lastribresptr)
      {/*at the last ribresptr*/

          /*alloc a final dummy residue to extend the spline*/
          theribresptr = allocribresstructure(theribfragptr);
             /* dummy lastribresptr, others alloc in loadribbonatoms() 100322*/
          if(theribresptr != NULL )
          {/*pad end with a dummy residue so ribbon can tail off nicely*/
            /*update the rolling pointers to current five residues*/
            nm4resptr = nm3resptr;
            nm3resptr = nm2resptr;
            nm2resptr = nm1resptr; /*penultimate residue: interval defined */
            nm1resptr = nthresptr; /*the last residue: guide not defined yet */
            nthresptr = theribresptr; /*which is now a C (3prime) end dummy*/

/*090223 if total of actual residues is less than 4, no info to tweak guides*/
/*so just skip this and hope for the best re: dirty trick DIRTY TRICK*/
/*  meanwhile, make sure that HETATMs not artificially fragmenting chain*/
/* i.e. deal with travesty of non-std residues being declared hets */
           if(theribresptr->number >= 8)/*3 start dummies + 4 actual + 1 end dummy 090223*/
           {/*at least 4 actual residues in this fragment 090223*/

            /*assign cross direction of penultimate guide to ultimate guide*/
            nm1resptr->dvecnptr->x = nm2resptr->dvecnptr->x;
            nm1resptr->dvecnptr->y = nm2resptr->dvecnptr->y;
            nm1resptr->dvecnptr->z = nm2resptr->dvecnptr->z;

            /*Ultimate guide point: extend beyond last guide point */
            /*Protein looks good extended half-distance along vector A */
            /*where vector A is CA---CA (P---P) so it's essentially on last CA*/
            /*For nucleic this is the P before all the last nucleotide atoms*/
            /*and not in-line with where the ribbon looks like it is going*/
            /*Try using the O3' as an approximation to a next PO4 */
            /*Using O3' itself makes a nice coil but much too enlongated arrow*/
            /*halfway between (average) last PO4 and O3' looks off to the side*/
            /*halfway between last guide point and O3' looks good but short*/
            /*Jane suggests 2/3 from last guide point to O3')*/

            if(Lnucleicacid && nm1resptr->o3ribptr != NULL)
            {/*new guide point 2/3 between last guide pt and o3prime atom*/
               /*i.e. average the points weighting o3 twice that of old guide*/
               nm1resptr->guideptr->x =
                  (2*(nm1resptr->o3ribptr->x) + nm2resptr->guideptr->x)/3;
               nm1resptr->guideptr->y =
                  (2*(nm1resptr->o3ribptr->y) + nm2resptr->guideptr->y)/3;
               nm1resptr->guideptr->z =
                  (2*(nm1resptr->o3ribptr->z) + nm2resptr->guideptr->z)/3;

            }
            else
            {
              /*extend forward half distance of vector A: ~at atom of last res*/
               nm1resptr->guideptr->x =
                  nm2resptr->guideptr->x + nm2resptr->avecaptr->x/2;
               nm1resptr->guideptr->y =
                  nm2resptr->guideptr->y + nm2resptr->avecaptr->y/2;
               nm1resptr->guideptr->z =
                  nm2resptr->guideptr->z + nm2resptr->avecaptr->z/2;
            }

            nm1resptr->axialptr->x = nm1resptr->guideptr->x; /*060115*/
            nm1resptr->axialptr->y = nm1resptr->guideptr->y;
            nm1resptr->axialptr->z = nm1resptr->guideptr->z;

            if(!Lnucleicacid)
            {/*protein tightly curled to the end, gets spline offset*/
               /*CURVATURE DISTANCE of last real residue back to its n-3*/
               distance  =
                 distanribpntptrs(nm1resptr->calphptr, nm4resptr->calphptr);

            /* but center[] has only been calc for those with high curvature*/
            /* however, that is the condition : distance < maxoff */
            /* and only for protein, so !Lnucleicacid */
            /*CURVATURE DISTANCE*/
              if(distance < maxoff )
              {/*short distance:treat as alpha radial offset penultimate point*/
                 /* CA1 to CA4 is measure of curvature and offset center*/
                 pmid[1] = (nm1resptr->calphptr->x + nm4resptr->calphptr->x)/2;
                 pmid[2] = (nm1resptr->calphptr->y + nm4resptr->calphptr->y)/2;
                 pmid[3] = (nm1resptr->calphptr->z + nm4resptr->calphptr->z)/2;
                 center[1] = (nm3resptr->guideptr->x + pmid[1])/2;
                 center[2] = (nm3resptr->guideptr->y + pmid[2])/2;
                 center[3] = (nm3resptr->guideptr->z + pmid[3])/2;
                 /* center[] is from last set of 4 pts, extend to a 5th*/
                 direc[1] = nm1resptr->guideptr->x - center[1];
                 direc[2] = nm1resptr->guideptr->y - center[2];
                 direc[3] = nm1resptr->guideptr->z - center[3];
                 /*normalize direc */
                 cmag = (float)(sqrt(   direc[1]*direc[1]
                                      + direc[2]*direc[2]
                                      + direc[3]*direc[3] ) );
                 direc[1] = direc[1]/cmag;
                 direc[2] = direc[2]/cmag;
                 direc[3] = direc[3]/cmag;
                 /*move guide of n-1 by fudge*maxoffset in direc direction*/
                 if(!LNOfudge) /*050327*/
                 {
                    nm1resptr->guideptr->x =
                       nm1resptr->guideptr->x + radialfactor*maxoffset*direc[1];
                    nm1resptr->guideptr->y =
                       nm1resptr->guideptr->y + radialfactor*maxoffset*direc[2];
                    nm1resptr->guideptr->z =
                       nm1resptr->guideptr->z + radialfactor*maxoffset*direc[3];

                    nm1resptr->axialptr->x =
                       nm1resptr->axialptr->x - radialfactor*maxoffset*direc[1];
                    nm1resptr->axialptr->y =
                       nm1resptr->axialptr->y - radialfactor*maxoffset*direc[2];
                    nm1resptr->axialptr->z =
                       nm1resptr->axialptr->z - radialfactor*maxoffset*direc[3];

                 }
              }/*short distance:treat as alpha, offset penultimate point*/
            }/*NOT Lnucleicacid*/
            /*duplicate last real residue's stuff at the dummy*/
            nthresptr->guideptr->x = nm1resptr->guideptr->x;
            nthresptr->guideptr->y = nm1resptr->guideptr->y;
            nthresptr->guideptr->z = nm1resptr->guideptr->z;
            nthresptr->dvecnptr->x = nm1resptr->dvecnptr->x;
            nthresptr->dvecnptr->y = nm1resptr->dvecnptr->y;
            nthresptr->dvecnptr->z = nm1resptr->dvecnptr->z;
            nthresptr->axialptr->x = nm1resptr->axialptr->x;
            nthresptr->axialptr->y = nm1resptr->axialptr->y;
            nthresptr->axialptr->z = nm1resptr->axialptr->z;

           /*use of default width factor bad if struct ends in helix or strand*/
           /*w[nrib] = wc;*/
           /*wp[nrib] = wc;*/
           /*extend width of previous interval,*/
           /* unfortunately, previous interval not yet evaluated!!!*/
           /*width is only defined up to what previously was interval n-2*/
           /*which has now been moved to n-3 position, extend at that width*/
            nm2resptr->widthpossible = nm3resptr->widthpossible;
            nm1resptr->widthpossible = nm2resptr->widthpossible;
            nthresptr->widthpossible = nm1resptr->widthpossible; /*extension*/
            nm2resptr->width = nm3resptr->widthpossible;
            nm1resptr->width = nm2resptr->widthpossible;
            nthresptr->width = nm1resptr->widthpossible; /*extension*/

           /*resetting explicit wrong for case where only last residue is coil*/
           /*try without this reset of explicit...  061021*/
         /*
            nm2resptr->explicit = nm3resptr->explicit;
            nm1resptr->explicit = nm2resptr->explicit;
            nthresptr->explicit = 0;
         */
                 /*so will trigger Lhead at finish end*/

           }/*at least 4 actual residues in this fragment 090223*/
          }/*pad end with a dummy residue so ribbon can tail off nicely*/
           
          theribresptr = NULL; /*now we can end this fragment*/
      }/*at the last ribresptr*/
      else
      {
         theribresptr = theribresptr->nextptr;
      }
      /*}}}------  cycle, or at last residue, make ultimate dummy guidepoint */

    }/*res*/ /*while(theribresptr != NULL)*/

    /*}}}loop through the residues, & dummies, tracking recent residues*/

    if(theribfragptr == theribsectionptr->lastribfragptr)
    {
       theribfragptr = NULL;
    }
    else
    {
       theribfragptr = theribfragptr->nextptr;
    }
  }/*frag*/ /*while(theribfragptr != NULL)*/

  /*}}}-- loop through fragments and residues                                */
}
/*___buildsectionguides()_________________________________________________}}}*/

/*{{{buildallguides()***************calls buildsectionguides()****************/
void buildallguides()  /*051214*/
{
  /*{{{-- loop through models to the sections, call build sectionguides*/

  ribmodelstruct*   theribmodelptr=NULL;
  ribsectionstruct* theribsectionptr=NULL;

  theribmodelptr = firstribmodelptr;
  while(theribmodelptr != NULL)
  {/*model*/
   theribsectionptr = theribmodelptr->firstribsectionptr;
   while(theribsectionptr != NULL)
   {/*section*/

    buildsectionguides(theribsectionptr); /*051214*/

    if(theribsectionptr == theribmodelptr->lastribsectionptr)
    {
       theribsectionptr = NULL;
    }
    else
    {
       theribsectionptr = theribsectionptr->nextptr;
    }
   }/*section*/ /*while(theribsectionptr != NULL)*/
   theribmodelptr = theribmodelptr->nextptr; /*==NULL if last model*/
  }/*model*/ /*while(theribmodelptr != NULL)*/

  /*}}}loop through models to the sections, call build sectionguides*/
}
/*___buildallguides()_____________________________________________________}}}*/

/*{{{expandsectionguides()****************************************************/
void expandsectionguides(ribsectionstruct* theribsectionptr)
{  /*safer allocations and tests, 130406 */
  /*{{{--declarations:                                                       */
  ribfragstruct*    theribfragptr=NULL;
  ribresstruct*     theribresptr=NULL;

  int   Lnucleic=0;
  int   mmaxres=0,num=0; /*Max number of all residues, num: specific res index*/
  float deldax=0,delday=0,deldaz=0; /*offset vectors fractions of dvecn*/
  float deldbx=0,deldby=0,deldbz=0; /* dvecn: d vector normalized */
  float deldcx=0,deldcy=0,deldcz=0; /* width direction of ribbon */
  float widena=0,widenb=0,widenc=0;
  int   j=0;
  int   LOK = 0; /*130406*/
  /*}}}*/

    /*{{{---- loop down through fragments to the residues                    */

    /*section*/

    if(theribsectionptr->type == 'N') {Lnucleic = 1;}
    else                              {Lnucleic = 0;}
    
    theribfragptr = theribsectionptr->firstribfragptr;
    while(theribfragptr != NULL)
    {/*frag*/

      if(theribfragptr->guideset == NULL)
      {/*alloc guideset for this ribfrag*/
        /*alloc the guide array to hold guide points of this frag*/
        /*protect from unforeseen as unneeded extra allocations*/

        if(theribfragptr->lastribresptr != NULL)
        {/*theribfragptr OK, initialize guideset and alloc residueset*/
           
           mmaxres = theribfragptr->lastribresptr->number;

           /*alloc the residue array of indexed residue pointers of this frag*/
        theribfragptr->guideset=(guide*)malloc(sizeof(guide)*(mmaxres+1));
           /*alloc the residue array of indexed residue pointers of this frag*/
        theribfragptr->residueset=(residue*)malloc(sizeof(residue)*(mmaxres+1));
           /* all real & dummy residues are in, now know maxres. 100322*/
           
           if(  (theribfragptr->guideset != NULL) 
              &&(theribfragptr->residueset != NULL))
           {/*guideset and residueset are allocated*/           
              
              for(j=0; j<mmaxres+1; j++) 
              { 
                 /*initialize members of each indexed guideset*/
                 /*guide members will be calculated below in this subroutine*/
                 theribfragptr->guideset[j].m0 = NULL;
                 theribfragptr->guideset[j].a1 = NULL;
                 theribfragptr->guideset[j].a2 = NULL;
                 theribfragptr->guideset[j].b3 = NULL;
                 theribfragptr->guideset[j].b4 = NULL;
                 theribfragptr->guideset[j].c5 = NULL;
                 theribfragptr->guideset[j].c6 = NULL;
                 theribfragptr->guideset[j].x7 = NULL; /*axis 060115*/
              
                 /*initialize the member of each indexed residueset*/
                 /*residue pointers point to existing residues of this frag*/
                 theribfragptr->residueset[j].resptr = NULL;
                 /* define residue-set array 100322*/
              }

              /*initialization avoid problem: first time through, */
              /*when allocating members, expect them to have been NULL.*/
              /*Later, when re-doing all, expect just to change values,*/
              /*BUT cannot trust uninitiated members to actually be NULL */
           }/*guideset and residueset are allocated*/
           LOK = 1;
        }/*theribfragptr OK, initialize guideset and alloc residueset*/
        else
        {/*NOT OK, do not work this fragment*/
           LOK = 0;	
        }
      }/*alloc guideset for this ribfrag*/
    /*}}}loop down through fragments to the residues*/

     /*{{{---- for each strand of each residue, calc width offset and apply  */
     if(LOK)
     {/*build guides for this fragment*/

     theribresptr = theribfragptr->firstribresptr;
     while(theribresptr != NULL)
     {/*res*/

       num = theribresptr->number;

       /*{{{----calc widena, widenb, widenc                                  */
       /* each type (a,b,c) placed out 1/2 of type-width */
       /* for  3 & 5 stranded: b out width/2; for 5 stranded: a out width/4 */
       /*widena, widenb, widenc are displacements! Already halved from orig */
       if(Lskeinedribbon)
       {
          widena = (theribresptr->width)/4 ;
          widenb = (theribresptr->width)/2 ;
       }
       else
       {
          widena = ribwidalpha/2;
          if(Lnucleic) {widenb = ribwidnucleic/2;}
          else         {widenb = ribwidbeta/2;}
          /*widenc will be the curvature-dependent width/2 at each residue*/
          /* (we've never used ribwidcoil for explicit ribbons) */
       }
       widenc = (theribresptr->width)/2 ; /* for both cases */
       /*}}}calc widena, widenb, widenc */

       /*{{{----calc offset delta and apply for each strand (a,b,c pairs)    */

       deldax = (theribresptr->dvecnptr->x)*((widena) );
       delday = (theribresptr->dvecnptr->y)*((widena) );
       deldaz = (theribresptr->dvecnptr->z)*((widena) );

       deldbx = (theribresptr->dvecnptr->x)*((widenb) );
       deldby = (theribresptr->dvecnptr->y)*((widenb) );
       deldbz = (theribresptr->dvecnptr->z)*((widenb) );

       deldcx = (theribresptr->dvecnptr->x)*((widenc) );
       deldcy = (theribresptr->dvecnptr->y)*((widenc) );
       deldcz = (theribresptr->dvecnptr->z)*((widenc) );

       if(theribfragptr->guideset[num].m0 == NULL)
         {theribfragptr->guideset[num].m0 = (p*)malloc(sizeof(p*));}
       theribfragptr->guideset[num].m0->x = theribresptr->guideptr->x;
       theribfragptr->guideset[num].m0->y = theribresptr->guideptr->y;
       theribfragptr->guideset[num].m0->z = theribresptr->guideptr->z;

       if(theribfragptr->guideset[num].a1 == NULL)
         {theribfragptr->guideset[num].a1 = (p*)malloc(sizeof(p*));}
       theribfragptr->guideset[num].a1->x = (theribresptr->guideptr->x)+deldax;
       theribfragptr->guideset[num].a1->y = (theribresptr->guideptr->y)+delday;
       theribfragptr->guideset[num].a1->z = (theribresptr->guideptr->z)+deldaz;

       if(theribfragptr->guideset[num].a2 == NULL)
         {theribfragptr->guideset[num].a2 = (p*)malloc(sizeof(p*));}
       theribfragptr->guideset[num].a2->x = (theribresptr->guideptr->x)-deldax;
       theribfragptr->guideset[num].a2->y = (theribresptr->guideptr->y)-delday;
       theribfragptr->guideset[num].a2->z = (theribresptr->guideptr->z)-deldaz;

       if(theribfragptr->guideset[num].b3 == NULL)
         {theribfragptr->guideset[num].b3 = (p*)malloc(sizeof(p*));}
       theribfragptr->guideset[num].b3->x = (theribresptr->guideptr->x)+deldbx;
       theribfragptr->guideset[num].b3->y = (theribresptr->guideptr->y)+deldby;
       theribfragptr->guideset[num].b3->z = (theribresptr->guideptr->z)+deldbz;

       if(theribfragptr->guideset[num].b4 == NULL)
         {theribfragptr->guideset[num].b4 = (p*)malloc(sizeof(p*));}
       theribfragptr->guideset[num].b4->x = (theribresptr->guideptr->x)-deldbx;
       theribfragptr->guideset[num].b4->y = (theribresptr->guideptr->y)-deldby;
       theribfragptr->guideset[num].b4->z = (theribresptr->guideptr->z)-deldbz;

       if(theribfragptr->guideset[num].c5 == NULL)
         {theribfragptr->guideset[num].c5 = (p*)malloc(sizeof(p*));}
       theribfragptr->guideset[num].c5->x = (theribresptr->guideptr->x)+deldcx;
       theribfragptr->guideset[num].c5->y = (theribresptr->guideptr->y)+deldcy;
       theribfragptr->guideset[num].c5->z = (theribresptr->guideptr->z)+deldcz;

       if(theribfragptr->guideset[num].c6 == NULL)
         {theribfragptr->guideset[num].c6 = (p*)malloc(sizeof(p*));}
       theribfragptr->guideset[num].c6->x = (theribresptr->guideptr->x)-deldcx;
       theribfragptr->guideset[num].c6->y = (theribresptr->guideptr->y)-deldcy;
       theribfragptr->guideset[num].c6->z = (theribresptr->guideptr->z)-deldcz;

       if(theribfragptr->guideset[num].x7 == NULL)
         {theribfragptr->guideset[num].x7 = (p*)malloc(sizeof(p*));}
       theribfragptr->guideset[num].x7->x = theribresptr->axialptr->x;
       theribfragptr->guideset[num].x7->y = theribresptr->axialptr->y;
       theribfragptr->guideset[num].x7->z = theribresptr->axialptr->z;

       /*residue information available by storing pointer to this residue*/
       if(theribfragptr->residueset[num].resptr == NULL)
       {
          theribfragptr->residueset[num].resptr =
                     (ribresstruct*)malloc(sizeof(ribresstruct*));
       }
       theribfragptr->residueset[num].resptr = theribresptr;
           /* ++++++ IMPORTANT: register theribresptr as member of set 100322*/
       
       /*}}}calc offset delta and apply for each strand (a,b,c pairs)*/

       /*now for this num: all theribresptr->guideset[num] members defined */
       if(theribresptr == theribfragptr->lastribresptr)
       {/*at the last ribresptr*/
          theribresptr = NULL; /*now we can end this fragment*/
       }/*at the last ribresptr*/
       else
       {
          theribresptr = theribresptr->nextptr;
       }

     }/*res*/ /*while(theribresptr != NULL)*/
     /*}}}for each strand of each residue, calc width offset and apply*/

     /*{{{---- loop back up through fragments                */

     if(theribfragptr == theribsectionptr->lastribfragptr)
         {  theribfragptr = NULL;}
     else{  theribfragptr = theribfragptr->nextptr;}

     }/*build guides for this fragment*/
    }/*frag*/ /*while(theribfragptr != NULL)*/
  /*}}}loop back up through fragments*/
}
/*___expandsectionguides()________________________________________________}}}*/

/*{{{expandallguides()**************calls expandsectionguides()***************/
void expandallguides()
{
  ribmodelstruct*   theribmodelptr=NULL;
  ribsectionstruct* theribsectionptr=NULL;

  /*{{{-- loop through models and sections, call expandsectionguides*/
  theribmodelptr = firstribmodelptr;
  while(theribmodelptr != NULL)
  {/*model*/
   theribsectionptr = theribmodelptr->firstribsectionptr;
   while(theribsectionptr != NULL)
   {/*section*/

    expandsectionguides(theribsectionptr); /*051214*/

    if(theribsectionptr == theribmodelptr->lastribsectionptr)
        {  theribsectionptr = NULL;}
    else{  theribsectionptr = theribsectionptr->nextptr;}

   }/*section*/ /*while(theribsectionptr != NULL)*/
   theribmodelptr = theribmodelptr->nextptr; /*==NULL if last model*/
  }/*model*/ /*while(theribmodelptr != NULL)*/
  /*}}}loop through models and sections, call expandsectionguides*/
}
/*___expandallguides()____________________________________________________}}}*/

/*{{{calculatesectionsplines()************************************************/
void calculatesectionsplines(ribsectionstruct* theribsectionptr)
{
  /*{{{--declarations:                                                       */
  ribfragstruct*    theribfragptr=NULL;
  ribresstruct*     theribresptr=NULL;
  chord*            thechord=NULL;
  spline*           thespline=NULL;
  strand*           thestrand=NULL;
  struct p* pptr=NULL;

  int  index=0;
  static    int   X[CORDER*2],IMAX,fgnumrib[CORDER+1];
  static    float Tbone,fg[CORDER][4];
  static    float spine[10][4];
  int  j=0,mn=0; /* n is a global counter for ranges*/
  int  mmaxres=0,nth=0,nm1=0;

  /*}}}*/

  /*{{{-- loop down through fragments to the residues                        */

    theribfragptr = theribsectionptr->firstribfragptr;
    while(theribfragptr != NULL)
    {/*frag*/
      if(theribfragptr->splineset == NULL)
      {
         /*declare a spline array to hold spline points for this frag*/
         /* member chord arrays declared later */
          mmaxres = theribfragptr->lastribresptr->number;
         theribfragptr->splineset = (spline*)malloc(sizeof(spline)*(mmaxres+1));
         /*080327 this is the only place where splineset is allocated*/
         /*enough room is alloc to hold mmaxres splines each with 8 strands*/
         if(theribfragptr->splineset != NULL)
         {
            for(j=0; j<=mmaxres; j++)
            {
               theribfragptr->splineset[j].m0strnd = NULL;
               theribfragptr->splineset[j].a1strnd = NULL;
               theribfragptr->splineset[j].a2strnd = NULL;
               theribfragptr->splineset[j].b3strnd = NULL;
               theribfragptr->splineset[j].b4strnd = NULL;
               theribfragptr->splineset[j].c5strnd = NULL;
               theribfragptr->splineset[j].c6strnd = NULL;
               theribfragptr->splineset[j].x7strnd = NULL;
            }
         }
         else
         {
            fprintf(stderr,"PKINRIBB failed to alloc splineset, crash?...\n");
         }

         theribfragptr->number = mmaxres; /*this should NOT change number!*/

      }

     index = -1; /*reset for next continuous ribbon, increases up to CORDER-1 */
       /*start at -1 since our ribbon residue numbers start at 1 and the */
       /*indexing for the spline calc has to line up with the residue numbers*/
       /*1st index==3: residue # == 4 and n-3 (index==0) is residue#==1 dummy*/
     /*initialize statics for a new ribbon fragment*/

     for(j=0;j<CORDER;j++) {X[j] = 0;} /* CORDER residues weigh in for spline*/
     for(j=CORDER; j<2*CORDER; j++) {X[j] = X[j-1]+1;} /* X knot weighting */
     Tbone = 0.0;
     IMAX = 2*CORDER-1;

     /*}}}loop down through fragments to the residues */

    /*{{{---- for each strand of each residue, calc splines*/

     /*{{{------ loop down to individual strand                              */
     theribresptr = theribfragptr->firstribresptr;
     while(theribresptr != NULL)
     {/*res*/
       /*side guide points for ribbons are now in theribresptr->guideset */
       /* as calculated by expandguides() */

       /*index == 0 at beginning of ribbon, at index== ORDER-1 (e.g.3)*/
       /*steady state where 4 intervals contibute to define the spline*/
       if(index == CORDER-1) /* CORDER index from 0 to CORDER-1 */
       {/*do a ribbon interval with points from I=0 to index == CORDER-1 */

         for(mn=0; mn<8; mn++)  /*axial x7 060115*/
         {/*loop over strands*/
            /*}}}loop down to individual strand*/

           /*{{{****load guide points of the four most recent residues       */
           nth = theribresptr->number; /*define for nth residue*/
           /*load fg[][] array which gets passed to splinecalc: */
           for(j=0; j<CORDER; j++)  /*only do this when index == CORDER-1*/
           {
              if     (mn==0){pptr = theribfragptr->guideset[nth-j].m0;}
              else if(mn==1){pptr = theribfragptr->guideset[nth-j].a1;}
              else if(mn==2){pptr = theribfragptr->guideset[nth-j].a2;}
              else if(mn==3){pptr = theribfragptr->guideset[nth-j].b3;}
              else if(mn==4){pptr = theribfragptr->guideset[nth-j].b4;}
              else if(mn==5){pptr = theribfragptr->guideset[nth-j].c5;}
              else if(mn==6){pptr = theribfragptr->guideset[nth-j].c6;}
              else if(mn==7){pptr = theribfragptr->guideset[nth-j].x7;}

              fg[index-j][1] = pptr->x;
              fg[index-j][2] = pptr->y;
              fg[index-j][3] = pptr->z;
           }
           /*}}}load guide points of the four most recent residues*/

           /*{{{----NOTES re splines, residues, calculating splines...       */
           /*tt is the number of spline points within a residue==interval*/
           /*ttt == 0 ... tt-1 */
           /*spline points for the interval are accumulated for each strand*/
           /* in spine[tt][xyz] */


           /*splinecalc needs to know current resnum (==Tbone), */
           /* and strand (m0,a1,a2,b3,b4,c5,c6) */
           /*  to look back for appropriate guide pts*/

           /*NOTE: CORDER-1 == 3 dummy residues at beginning (for CORDER==4), */
           /*splines begin at 1st real residue, the 4th by count */
           /* 1 dummy residue at final end, */
           /*splines end at last real residue, maxres-1 by count */

           /*offset guide points are in theribfragptr->guideset[nth]*/
           /*for prekin standard number of chords == 4 (CORDER == 4): */
           /*spline for the n-1 interval calc from n-3,n-2,n-1,nth guidepoints*/
           /*Tbone monotonically increasing, X knot weighting, also increasing*/
           /*}}}NOTES re splines, residues, calculating splines*/

           splinecalc(Tbone, IMAX, X, fg, spine); /*call from calculatesplines*/

           /*{{{------ one interval's spline vectors, check for last residue */
           /*spine now has one interval's spline vectors for this strand*/
           /* this is for n-1 residue, where nth is the residue of index.*/
           /* the very end does the n-1 residue, nth is the extra dummy at end*/
           /*at the very end, move the last spline pt out to the last guide pt*/
           if(theribresptr == theribfragptr->lastribresptr) /*last ribresptr*/
           {  /*set last spline point on last guide point*/
             spine[ichord-1][1] = fg[index][1]; /*guide pt of last (dummy) res*/
             spine[ichord-1][2] = fg[index][2];
             spine[ichord-1][3] = fg[index][3];
           }
           /*}}}one interval's spline vectors, check for last residue*/

           /*{{{****alloc the strands of this splineset for the n-1 residue  */
           nm1 = theribresptr->previousptr->number; /*define for nth-1 */

           if(mn==0)
           {
              if(theribfragptr->splineset[nm1].m0strnd==NULL)
              {
                 theribfragptr->splineset[nm1].m0strnd =
                   (strand*)malloc(sizeof(strand));
                 theribfragptr->splineset[nm1].m0strnd->chordset = NULL;
              }
              if(  (theribfragptr->splineset[nm1].m0strnd != NULL)
                 &&(theribfragptr->splineset[nm1].m0strnd->chordset == NULL) )
              {
                    theribfragptr->splineset[nm1].m0strnd->chordset =
                       (chord*)malloc(sizeof(chord*)*ichord);
                 for(j=0; j<ichord; j++)
                 {
                    theribfragptr->splineset[nm1].m0strnd->chordset[j].pt=NULL;
                 }
              }
              thestrand = theribfragptr->splineset[nm1].m0strnd;
           }
           else if(mn==1)
           {
              if(theribfragptr->splineset[nm1].a1strnd==NULL)
              {
                 theribfragptr->splineset[nm1].a1strnd =
                   (strand*)malloc(sizeof(strand));
                 theribfragptr->splineset[nm1].a1strnd->chordset = NULL;
              }
              if(  (theribfragptr->splineset[nm1].a1strnd != NULL)
                 &&(theribfragptr->splineset[nm1].a1strnd->chordset == NULL) )
              {
                    theribfragptr->splineset[nm1].a1strnd->chordset =
                        (chord*)malloc(sizeof(chord*)*ichord);
                 for(j=0; j<ichord; j++)
                 {
                    theribfragptr->splineset[nm1].a1strnd->chordset[j].pt=NULL;
                 }
              }
              thestrand = theribfragptr->splineset[nm1].a1strnd;
           }
           else if(mn==2)
           {
              if(theribfragptr->splineset[nm1].a2strnd==NULL)
              {
                 theribfragptr->splineset[nm1].a2strnd =
                   (strand*)malloc(sizeof(strand));
                 theribfragptr->splineset[nm1].a2strnd->chordset = NULL;
              }
              if(  (theribfragptr->splineset[nm1].a2strnd != NULL)
                 &&(theribfragptr->splineset[nm1].a2strnd->chordset == NULL) )
              {
                    theribfragptr->splineset[nm1].a2strnd->chordset =
                       (chord*)malloc(sizeof(chord*)*ichord);
                 for(j=0; j<ichord; j++)
                 {
                    theribfragptr->splineset[nm1].a2strnd->chordset[j].pt=NULL;
                 }
              }
              thestrand = theribfragptr->splineset[nm1].a2strnd;
           }
           else if(mn==3)
           {
              if(theribfragptr->splineset[nm1].b3strnd==NULL)
              {
                 theribfragptr->splineset[nm1].b3strnd =
                   (strand*)malloc(sizeof(strand));
                 theribfragptr->splineset[nm1].b3strnd->chordset = NULL;
              }
              if(  (theribfragptr->splineset[nm1].b3strnd != NULL)
                 &&(theribfragptr->splineset[nm1].b3strnd->chordset == NULL) )
              {
                    theribfragptr->splineset[nm1].b3strnd->chordset =
                       (chord*)malloc(sizeof(chord*)*ichord);
                 for(j=0; j<ichord; j++)
                 {
                    theribfragptr->splineset[nm1].b3strnd->chordset[j].pt=NULL;
                 }
              }
              thestrand = theribfragptr->splineset[nm1].b3strnd;
           }
           else if(mn==4)
           {
              if(theribfragptr->splineset[nm1].b4strnd==NULL)
              {
                 theribfragptr->splineset[nm1].b4strnd =
                   (strand*)malloc(sizeof(strand));
                 theribfragptr->splineset[nm1].b4strnd->chordset = NULL;
              }
              if(  (theribfragptr->splineset[nm1].b4strnd != NULL)
                 &&(theribfragptr->splineset[nm1].b4strnd->chordset == NULL) )
              {
                    theribfragptr->splineset[nm1].b4strnd->chordset =
                       (chord*)malloc(sizeof(chord*)*ichord);
                 for(j=0; j<ichord; j++)
                 {
                    theribfragptr->splineset[nm1].b4strnd->chordset[j].pt=NULL;
                 }
              }
              thestrand = theribfragptr->splineset[nm1].b4strnd;
           }
           else if(mn==5)
           {
              if(theribfragptr->splineset[nm1].c5strnd==NULL)
              {
                 theribfragptr->splineset[nm1].c5strnd =
                   (strand*)malloc(sizeof(strand));
                 theribfragptr->splineset[nm1].c5strnd->chordset = NULL;
              }
              if(  (theribfragptr->splineset[nm1].c5strnd != NULL)
                 &&(theribfragptr->splineset[nm1].c5strnd->chordset == NULL) )
              {
                    theribfragptr->splineset[nm1].c5strnd->chordset =
                       (chord*)malloc(sizeof(chord*)*ichord);
                 for(j=0; j<ichord; j++)
                 {
                    theribfragptr->splineset[nm1].c5strnd->chordset[j].pt=NULL;
                 }
              }
              thestrand = theribfragptr->splineset[nm1].c5strnd;
           }
           else if(mn==6)
           {
              if(theribfragptr->splineset[nm1].c6strnd==NULL)
              {
                 theribfragptr->splineset[nm1].c6strnd =
                   (strand*)malloc(sizeof(strand));
                 theribfragptr->splineset[nm1].c6strnd->chordset = NULL;
              }
              if(  (theribfragptr->splineset[nm1].c6strnd != NULL)
                 &&(theribfragptr->splineset[nm1].c6strnd->chordset == NULL) )
              {
                 theribfragptr->splineset[nm1].c6strnd->chordset =
                    (chord*)malloc(sizeof(chord*)*ichord);
                 for(j=0; j<ichord; j++)
                 {
                    theribfragptr->splineset[nm1].c6strnd->chordset[j].pt=NULL;
                 }
              }
              thestrand = theribfragptr->splineset[nm1].c6strnd;
           }
           else if(mn==7)
           {
              if(theribfragptr->splineset[nm1].x7strnd==NULL)
              {
                 theribfragptr->splineset[nm1].x7strnd =
                   (strand*)malloc(sizeof(strand));
                 theribfragptr->splineset[nm1].x7strnd->chordset = NULL;
              }
              if(  (theribfragptr->splineset[nm1].x7strnd != NULL)
                 &&(theribfragptr->splineset[nm1].x7strnd->chordset == NULL) )
              {
                 theribfragptr->splineset[nm1].x7strnd->chordset =
                    (chord*)malloc(sizeof(chord*)*ichord);
                 for(j=0; j<ichord; j++)
                 {
                    theribfragptr->splineset[nm1].x7strnd->chordset[j].pt=NULL;
                 }
              }
              thestrand = theribfragptr->splineset[nm1].x7strnd;
           }

           /*}}}alloc the strands of this splineset for the n-1 residue*/

           /*{{{****load the appropriate chord point with the spine coords   */

           for(j=0; j<ichord; j++)
           {/*loop over chords*/
              if(thestrand->chordset[j].pt == NULL)
              {
                   thestrand->chordset[j].pt = (p*)malloc(sizeof(p*));
              }
              thestrand->chordset[j].pt->x = spine[j][1];
              thestrand->chordset[j].pt->y = spine[j][2];
              thestrand->chordset[j].pt->z = spine[j][3];
           }/*loop over chords*/
           /*}}}load the appropriate chord point with the spine coords*/

      /*{{{------ loop back up from individual strand to end of residues     */
         }/*loop over strands*/
         /*finished with this interval*/
         for(j=0; j<IMAX; j++)
         {/*reset X[] knot value array for next interval*/
            X[j] = X[j+1];
         }
         X[IMAX] = X[IMAX]+1;
         Tbone++; /*set for next interval*/
       }/*do a ribbon interval with points from I=0 to index == CORDER-1 */
       else
       {
          index++; /*index not up to CORDER-1 yet */
       }
       if(theribresptr == theribfragptr->lastribresptr) /*at the last ribresptr*/
           {  theribresptr = NULL;} /*now we can end this fragment*/
       else{  theribresptr = theribresptr->nextptr;}
     }/*res*/ /*while(theribresptr != NULL)*/
     /*}}} loop back up from individual strand to end of residues*/
     /*}}} for each strand of each residue, calc splines*/

  /*{{{-- loop back up through fragments to section                          */

     if(theribfragptr == theribsectionptr->lastribfragptr)
         {  theribfragptr = NULL;}
     else{  theribfragptr = theribfragptr->nextptr;}
    }/*frag*/ /*while(theribfragptr != NULL)*/
  /*}}}loop back up through fragments to section*/
}
/*___calculatesectionsplines()____________________________________________}}}*/

/*{{{calculateallsplines()**********calls calculatesectionsplines()***********/
void calculateallsplines()
{
  ribmodelstruct*   theribmodelptr=NULL;
  ribsectionstruct* theribsectionptr=NULL;

  theribmodelptr = firstribmodelptr;
  while(theribmodelptr != NULL)
  {/*model*/

   theribsectionptr = theribmodelptr->firstribsectionptr;
   while(theribsectionptr != NULL)
   {/*section*/

    calculatesectionsplines(theribsectionptr);

    if(theribsectionptr == theribmodelptr->lastribsectionptr)
        {  theribsectionptr = NULL;}
    else{  theribsectionptr = theribsectionptr->nextptr;}
   }/*section*/ /*while(theribsectionptr != NULL)*/
   theribmodelptr = theribmodelptr->nextptr; /*==NULL if last model*/
  }/*model*/ /*while(theribmodelptr != NULL)*/
}
/*___calculateallsplines()________________________________________________}}}*/

/*{{{constructsectionribbons()***********incl TEST code***********************/
void constructsectionribbons(ribsectionstruct* theribsectionptr)
{           /*also has TEST guide pts and TEST splines */
 /*{{{-- declarations:  */
  ribfragstruct*    theribfragptr=NULL;
  spline*           thesplineset=NULL;
  residue*          theresidueset=NULL;
  struct p* pptr=NULL;
  struct p* qptr=NULL;

  struct chord* mstrand=NULL;  /*chordset of a strand, has pt as a member*/
  struct chord* pstrand=NULL;
  struct chord* qstrand=NULL;
  struct chord* ms=NULL;  /*chordset of a strand at this residue, pt is member*/
  struct chord* ps=NULL;  /*ps,qs either ALPHA, or BETA edges */
  struct chord* qs=NULL;  /*also edge strands of 3,5 multi-stranded ribbons*/
  struct chord* rs=NULL;  /*rs,ss intermediate skeins of 5 stranded ribbons*/
  struct chord* ss=NULL;
  struct chord* mspre=NULL;  /*chordset of a strand at previous residue*/
  struct chord* pspre=NULL;
  struct chord* qspre=NULL;
  struct chord* rspre=NULL;  /*for intermediate skeins of 5 stranded ribbons*/
  struct chord* sspre=NULL;
  struct chord* xs=NULL;     /*strand under consideration for skeined ribbon*/
  struct chord* xspre=NULL;

  float radius=0;   /* VRML */
  char  whostr[32]; /* VRML */

  int   Lhead=0,Ltail=0;
  int   j=0,mn=0,nmax=0,num=0,maxres=0; /*chords, strands, residues */
  int   numinsection = 0; /*number of a residue in a section of fragments*/
  char  gc=' '; /*guide character: m,a,b,c , use with nm strand number 0...6*/
  char  PL[5]={'\0','\0','\0','\0','\0' };  /*Point,Line (Move,Draw) */
  char  Ue[5]={'\0','\0','\0','\0','\0' }; /*Unpickable indicator when == U */
  char  W[10]; /*single strand width# */
  char  Z[10]; /*single strand width# black bkg 090622*/
  int   thick=0; /*single strand black bkg width <= 7, 090704*/
  char  strnd[3]={'\0','\0','\0' };
  char  cntl[5]={'\0','\0','\0','\0','\0' };
  int   ns=0;
  char  types[16];
  char  color[16];
  char  colorstr[16];
  char  setname[8]={'\0','\0','\0','\0','\0','\0','\0','\0' }; /*060929*/
        /*setname[8]: colorset name for explicit SHEET sec str identity 060929*/
  int   ncol=0;
  char  resID[32];
  char  resIDpre[32];
  float extended=0; /*fractional extendedness of chain at this residue*/
  int   type=0;
  int   cnt = 0, nnt=0;
  float   distance=0;
  float   tenth=0; /*061029 colorNtoC*/
  float angle=0; /*for Calphas+azmangle  100322*/
#define ALPHA 1
#define BETA  2
#define COIL  3
#define FUDGE 7
/*090704 use of cntl string...
rbgp Lribbonguidepts
extB spline points
rbsp spline chords
rbaf ALPHA face
rbbf BETA  face
rbcf COIL  face
rbcf FUDGE face
rbBf BETA  face, cap B signals {colorset name}
rbce midline thick line
rbae ALPHA faced ribbon edges
rbbe BETA  faced ribbon edges
rb1c odd-residue coil
rb2c even-residue coil
rb   default ribbon vectorlist
VR%2d VRML splines
VRML  VRML arrowhead
scv  sidechain stubs (attach to sc lists)
rbhb pseudo H-bonds

rbn  nucleic defined in PKINCOUT, not used PKINRIBB
rbne edge for nucleic "
rb   considered coil in PKINCOUT
rbc  considered coil in PKINCOUT

*/
  /*}}}*/

 /*{{{-- loop through section fragments, work with residue splinesets, TEST*/

    /*{{{---loop down through fragments to residue splinesets                */

    theribfragptr = theribsectionptr->firstribfragptr;
    numinsection = 0; /*position for NtoC color over section  090219*/
    while(theribfragptr != NULL)
    {/*frag*/

      if(theribfragptr->splineset != NULL && theribfragptr->number > 5)
      {/*splineset*/

         maxres = theribfragptr->number;
         thesplineset  = theribfragptr->splineset;
         theresidueset = theribfragptr->residueset;

         /*NOTE: CORDER-1 == 3 dummy residues at beginning (for CORDER==4), */
         /*splines begin at 1st real residue, the 4th by count */
         /* 1 dummy residue at final end, */
         /*splines end at last real residue, maxres-1 by count */
         /*guide points are defined for all residues, real and dummy*/
         /*but the first 2 dummy's guide points are on top of each other*/

    /*}}}loop down through this section's fragments to residue splinesets*/

         /*++++{{{TEST: output guide points of all residues                  */
         if(Lribbonguidepts)
         {/*output the guide points of all residues */
            for(num=1; num < maxres+1; num++)
            {/*loop over all residues, incl dummies on ends*/

               sprintf(cntl,"rbgp");

               for(mn=0; mn<7; mn++)  /*only do this when index == CORDER-1*/
               {/*loop over all strands*/
                 if     (mn==0)
                 {
                    pptr = theribfragptr->guideset[num].m0;
                    gc='m';
                    sprintf(color," gray width7");
                 }
                 else if(mn==1)
                 {
                    pptr = theribfragptr->guideset[num].a1;
                    gc='a';
                    sprintf(color," red width7");
                 }
                 else if(mn==2)
                 {
                    pptr = theribfragptr->guideset[num].a2;
                    gc='a';
                    sprintf(color," red width7");
                 }
                 else if(mn==3)
                 {
                    pptr = theribfragptr->guideset[num].b3;
                    gc='b';
                    sprintf(color," green width5");
                 }
                 else if(mn==4)
                 {
                    pptr = theribfragptr->guideset[num].b4;
                    gc='b';
                    sprintf(color," green width5");
                 }
                 else if(mn==5)
                 {
                    pptr = theribfragptr->guideset[num].c5;
                    gc='c';
                    sprintf(color," gold width3");
                 }
                 else if(mn==6)
                 {
                    pptr = theribfragptr->guideset[num].c6;
                    gc='c';
                    sprintf(color," gold width3");
                 }

                  sprintf(temps,"%s{%c%d GUIDE %d}P%s '%c' %.3f, %.3f, %.3f"EOLO
                      ,cntl,gc,mn,num,color,gc,pptr->x,pptr->y,pptr->z);
                    putonetextblockline(&mainscratch,temps);

                  Listrib = 1;
                  typenow=typerib; /*global output controls for old prekin*/
                  ++pointcnt[typenow];
               }
               if(1==1)
               {
                  if(theresidueset[num].resptr->COside == 1)
                  {
                     sprintf(color," sea width7");
                     pptr->x =   theribfragptr->guideset[num].b3->x
                               + theresidueset[num].resptr->dvecnptr->x;
                     pptr->y =   theribfragptr->guideset[num].b3->y
                               + theresidueset[num].resptr->dvecnptr->y;
                     pptr->z =   theribfragptr->guideset[num].b3->z
                               + theresidueset[num].resptr->dvecnptr->z;
                     gc = 'b'; mn = 3;
                  }
                  else if(theresidueset[num].resptr->COside == 2)
                  {
                     sprintf(color," orange width7");
                     pptr->x =   theribfragptr->guideset[num].b4->x
                               - theresidueset[num].resptr->dvecnptr->x;
                     pptr->y =   theribfragptr->guideset[num].b4->y
                               - theresidueset[num].resptr->dvecnptr->y;
                     pptr->z =   theribfragptr->guideset[num].b4->z
                               - theresidueset[num].resptr->dvecnptr->z;
                     gc = 'b'; mn = 4;
                  }
                  sprintf(temps,"%s{%c%d COside %d}P%s 'H' %.3f, %.3f, %.3f"EOLO
                      ,cntl,gc,mn,num,color,pptr->x,pptr->y,pptr->z);
                    putonetextblockline(&mainscratch,temps);

                  if(theresidueset[num].resptr->hnpepptr != NULL)
                  {
                     pptr->x = theresidueset[num].resptr->hnpepptr->x;
                     pptr->y = theresidueset[num].resptr->hnpepptr->y;
                     pptr->z = theresidueset[num].resptr->hnpepptr->z;
                     /*use same cntl as above*/
                     sprintf(color," gray width7");
                     sprintf(temps,"%s{Hnpept %d}P%s 'H' %.3f, %.3f, %.3f"EOLO
                         ,cntl,num,color,pptr->x,pptr->y,pptr->z);
                       putonetextblockline(&mainscratch,temps);
                  }
                  if(theresidueset[num].resptr->middlptr != NULL)
                  {
                     pptr->x = theresidueset[num].resptr->middlptr->x;
                     pptr->y = theresidueset[num].resptr->middlptr->y;
                     pptr->z = theresidueset[num].resptr->middlptr->z;
                     /*use same cntl as above*/
                     sprintf(color," hotpink width3");
                     sprintf(temps,"%s{mid-pt %d}P 'p' %s %.3f, %.3f, %.3f"EOLO
                         ,cntl,num,color,pptr->x,pptr->y,pptr->z);
                       putonetextblockline(&mainscratch,temps);
                  }
                  if(theresidueset[num].resptr->axialptr != NULL)
                  {
                     pptr->x = theresidueset[num].resptr->axialptr->x;
                     pptr->y = theresidueset[num].resptr->axialptr->y;
                     pptr->z = theresidueset[num].resptr->axialptr->z;
                     /*use same cntl as above*/
                     sprintf(color," white width7");
                     sprintf(temps,"%s{axis x7 %d}P 'X' %s %.3f, %.3f, %.3f"EOLO
                         ,cntl,num,color,pptr->x,pptr->y,pptr->z);
                       putonetextblockline(&mainscratch,temps);
                  }

               }/*1==1*/
            }/*loop over all residues, incl dummies on ends*/

         }/*output the guide points of all residues */
         /*}}}TEST: output guide points of all residues*/

         /*++++{{{TEST: output density of spline points for all residues     */
         if(Lribbondensity)
         {/*output the local spline density of all residues */

            /*density of spline points****************************************/
            Listext = 1;
            for(num=CORDER; num < maxres; num++)
            {/*loop over all residues*/
              cnt = 0;
              pptr = theribfragptr->splineset[num].x7strnd->chordset[0].pt;
              for(nnt=CORDER; nnt < maxres; nnt++)
              {/*loop over all residues*/
                 if(nnt < num-2 || nnt > num+2)
                 {
                  for(j=0; j<ichord; j++)
                  {/*loop over chords*/

                    distance =
                      distanpntptrs(pptr
                        ,theribfragptr->splineset[nnt].x7strnd->chordset[j].pt);
                    if(distance < 8){cnt++;}
                  }/*loop over chords*/
                 }
              }/*loop over all residues*/
                sprintf(cntl,"extB");
                sprintf(color," brown");
                sprintf(temps,"%s{  %d} r=%.2f %s %.3f, %.3f, %.3f"EOLO
                   ,cntl,cnt,(float)cnt/20,color,pptr->x,pptr->y,pptr->z);
                putonetextblockline(&mainscratch,temps);
            }/*loop over all residues*/
            /*density of spline points_______________________________________*/
         }/*output the local spline density of all residues */

         /*++++}}}TEST: output density of spline points for all residues*/

         /*++++{{{Calphas+COazmangle  100322 */
         if(LvectCOpdb)
         {/*only CA,azimuth to pdb format (the azmangle value)  100322*/
            for(num=1; num < maxres+1; num++)
            {/*loop over all residues, incl dummies on ends*/
               Listext = 1;
               extoutl = 1;
               sprintf(cntl,"pdb ");/*extra output*/
               
               /*angle = theribfragptr->ribresptr->azmangle;*/
               /*angle = theribfragptr->residueset[num].azmangle;*/
               angle = theresidueset[num].resptr->azmangle;
               if(angle < 0) angle = 360 + angle; /*130121 = */

               if(   theresidueset[num].resptr != NULL 
                  && theresidueset[num].resptr->calphptr != NULL)
               {/*Calpha exists for a residue...*/               
                sprintf(temps
,"%sATOM  %5d  CA  %s%s%4d%s   %8.3f%8.3f%8.3f           %7.1fVCCO%4d"EOLO
               ,cntl
               ,num
               ,theresidueset[num].resptr->resrib
               ,theresidueset[num].resptr->subrib
               ,theresidueset[num].resptr->numrib
               ,theresidueset[num].resptr->rinsrib
               ,theresidueset[num].resptr->calphptr->x
               ,theresidueset[num].resptr->calphptr->y
               ,theresidueset[num].resptr->calphptr->z
               ,angle
               ,num-1);

               putonetextblockline(&mainscratch,temps);
                  
               typenow=typeext; /*global output controls for old prekin*/
               ++pointcnt[typenow]; 

               }
            }/*loop over all residues, incl dummies on ends*/
         }
         /*++++}}}Calphas+COazmangle  100322 */
         
         /*{{{--- for each residue, calculate ribbon section, can do TEST splines*/
         /*--------------THE ACTUAL RIBBON CODE -----------------------------*/

         if(!LvectCOpdb)
         {/*The actual ribbon code  100322*/

         for(num=CORDER; num < maxres; num++) /*THE loop over residues*/
         {/*loop over residues in a fragment, num is residue position in frag*/
           /*{{{---- define residue IDs, ribidentity for this and previous one*/
           numinsection++; /*accumulates across fragments for NtoC 090219*/

           if(theresidueset[num].resptr->ribidentity[0] != '\0') /*060929*/
           {  /*ribbon identity from PDB SHEET record 060929 */
              sprintf(resID,"%s %s %d%s %s"
                        ,theresidueset[num].resptr->resrib
                        ,theresidueset[num].resptr->subrib
                        ,theresidueset[num].resptr->numrib
                        ,theresidueset[num].resptr->rinsrib
                        ,theresidueset[num].resptr->ribidentity );
              if(num > 4)
              {
                 sprintf(resIDpre,"%s %s%4d%s %s"
                        ,theresidueset[num-1].resptr->resrib
                        ,theresidueset[num-1].resptr->subrib
                        ,theresidueset[num-1].resptr->numrib
                        ,theresidueset[num-1].resptr->rinsrib
                        ,theresidueset[num-1].resptr->ribidentity );
              }
           }
           else
           {
              sprintf(resID,"%s %s%4d%s"
                        ,theresidueset[num].resptr->resrib
                        ,theresidueset[num].resptr->subrib
                        ,theresidueset[num].resptr->numrib
                        ,theresidueset[num].resptr->rinsrib );
              if(num > 4)
              {
                 sprintf(resIDpre,"%s %s%4d%s"
                        ,theresidueset[num-1].resptr->resrib
                        ,theresidueset[num-1].resptr->subrib
                        ,theresidueset[num-1].resptr->numrib
                        ,theresidueset[num-1].resptr->rinsrib );
              }
           }
           /*}}}define residue IDs, ribidentity for this and previous one*/

           /*++++{{{TEST: output of raw splines++++                          */
            if(Lribbonrawsplines)
            {/*Lribbonrawsplines*/

               /*++++{{{TEST: set color for secondary struct type            */

               /* ALPHA2D    1 */
               /* BETA2D     2 */
               /* COIL2D     4 */
               /* NUCLEIC2D  8 */
               /* ARROW2D   16 */
               /* ALPHAARROW2D  17 */
               /* BETAARROW2D   18  bit-wise logic imposed 061021*/
               /* SEGMENTS   12  */ /*050204 for VRMLxSectionCircle */
               /*change this section to bit-wise logic flags 061021*/
               /* double bit ARROWs thus have to be tested first*/
               if     (theresidueset[num].resptr->explicit & ALPHAARROW2D)
                    {sprintf(types,"ALPHAARROW2D");sprintf(color,"magenta");}
               else if(theresidueset[num].resptr->explicit & BETAARROW2D)
                    {sprintf(types,"BETAARROW2D");sprintf(color,"cyan");}
               else if(theresidueset[num].resptr->explicit & ALPHA2D)
                    {sprintf(types,"ALPHA2D");sprintf(color,"red");}
               else if(theresidueset[num].resptr->explicit & BETA2D)
                    {sprintf(types,"BETA2D");sprintf(color,"green");}
               else if(theresidueset[num].resptr->explicit & COIL2D)
                    {sprintf(types,"COIL2D");sprintf(color,"gray");}
               else if(theresidueset[num].resptr->explicit & NUCLEIC2D)
                    {sprintf(types,"NUCLEIC2D");sprintf(color,"brown");}
               else {sprintf(types,"UNKNOWN");sprintf(color,"yellow");}

               /*}}}TEST: set color for secondary struct type       */

               for(mn=0; mn<8; mn++) /*axial x7 060115*/
               {/*loop over strands*/
                  for(j=0; j<ichord; j++)
                  {/*loop over chords*/
                  /*{{{----get appropriate chord point to place calc chord   */
                  if(mn==0)
                  {/*pptr is a global*/   /*constructribbons()*/
                   pptr = theribfragptr->splineset[num].m0strnd->chordset[j].pt;
                   sprintf(strnd,"m0");
                   /*use color of sec-struc type*/
                   ns = 0;
                   gc = 'm';
                  }
                  else if(mn==1)
                  {
                   pptr = theribfragptr->splineset[num].a1strnd->chordset[j].pt;
                   sprintf(strnd,"a1");
                   sprintf(color,"red");
                   ns = 1;
                   gc = 'a';
                  }
                  else if(mn==2)
                  {
                   pptr = theribfragptr->splineset[num].a2strnd->chordset[j].pt;
                   sprintf(strnd,"a2");
                   sprintf(color,"red");
                   ns = 2;
                   gc = 'a';
                  }
                  else if(mn==3)
                  {
                   pptr = theribfragptr->splineset[num].b3strnd->chordset[j].pt;
                   sprintf(strnd,"b3");
                   sprintf(color,"green");
                   ns = 3;
                   gc = 'b';
                  }
                  else if(mn==4)
                  {
                   pptr = theribfragptr->splineset[num].b4strnd->chordset[j].pt;
                   sprintf(strnd,"b4");
                   sprintf(color,"green");
                   ns = 4;
                   gc = 'b';
                  }
                  else if(mn==5)
                  {
                   pptr = theribfragptr->splineset[num].c5strnd->chordset[j].pt;
                   sprintf(strnd,"c5");
                   sprintf(color,"gold");
                   ns = 5;
                   gc = 'c';
                  }
                  else if(mn==6)
                  {
                   pptr = theribfragptr->splineset[num].c6strnd->chordset[j].pt;
                   sprintf(strnd,"c6");
                   sprintf(color,"gold");
                   ns = 6;
                   gc = 'c';
                  }
                  else if(mn==7)
                  {
                   pptr = theribfragptr->splineset[num].x7strnd->chordset[j].pt;
                   sprintf(strnd,"x7");
                   sprintf(color,"white");
                   ns = 7;
                   gc = 'X';
                  }

                  /*}}}get appropriate chord point to place calc chord*/

                     sprintf(cntl,"rbsp");
                     if(j==0) {sprintf(PL," P ");}
                     else     {sprintf(PL," L ");}
                     sprintf(temps,"%s{SPLINE%s: %s %d %s %d}"
                                   "%s '%c' %s %.3f, %.3f, %.3f"EOLO
                       ,cntl,strnd
                       ,theresidueset[num].resptr->resrib
                       ,theresidueset[num].resptr->numrib
                       ,types,theresidueset[num].resptr->explicit
                       ,PL
                       ,gc /* pointmaster for this strnd */
                       ,color
                       ,pptr->x,pptr->y,pptr->z
                     );
                     putonetextblockline(&mainscratch,temps);
                     Listrib = 1;
                     typenow=typerib; /*global output controls for old prekin*/
                     ++pointcnt[typenow];
                  }/*loop over chords*/
               }/*loop over strands*/

            }/*Lribbonrawsplines*/
            /*++++}}}TEST: output of raw splines++++*/

           /*{{{----define strand_chordset for both faced and skeined ribbons*/

           ms = theribfragptr->splineset[num].m0strnd->chordset;
           if(num > 4)
           {
              mspre = theribfragptr->splineset[num-1].m0strnd->chordset;
           }
           if(  !Lskeinedribbon
              && (   !Lexplicitribbon
                  || theresidueset[num].resptr->explicit ==  COIL2D ) )
           {/*COIL or FUDGE, faced ribbon*/
              if(!Lexplicitribbon) {type = FUDGE;}
              else {type = COIL;}
              ps = theribfragptr->splineset[num].c5strnd->chordset;
              qs = theribfragptr->splineset[num].c6strnd->chordset;
              if(num > 4)
              {
                 pspre = theribfragptr->splineset[num-1].c5strnd->chordset;
                 qspre = theribfragptr->splineset[num-1].c6strnd->chordset;
              }
              if(type==FUDGE && num==4) {Ltail = 1;}
              else {Ltail = 0;}
              if(type==FUDGE && num==(maxres-1)) {Lhead = 1;}
              else {Lhead = 0;}
           }
           else if(   !Lskeinedribbon
                    && theresidueset[num].resptr->explicit & ALPHA2D )
           {/*ALPHA, faced ribbon*/
              type = ALPHA;
              ps = theribfragptr->splineset[num].a1strnd->chordset;
              qs = theribfragptr->splineset[num].a2strnd->chordset;
              if(num > 4)
              {
                 pspre = theribfragptr->splineset[num-1].a1strnd->chordset;
                 qspre = theribfragptr->splineset[num-1].a2strnd->chordset;
              }
           }
           else if(   !Lskeinedribbon
                    && theresidueset[num].resptr->explicit & BETA2D )
           {/*BETA, faced ribbon*/
              type = BETA;
              ps = theribfragptr->splineset[num].b3strnd->chordset;
              qs = theribfragptr->splineset[num].b4strnd->chordset;
              if(num > 4)
              {
                 pspre = theribfragptr->splineset[num-1].b3strnd->chordset;
                 qspre = theribfragptr->splineset[num-1].b4strnd->chordset;
              }
           }
           else if(Lskeinedribbon)
           {/*FUDGE, skeined ribbon*/
              type = FUDGE;
              ps = theribfragptr->splineset[num].b3strnd->chordset; /*BETAedge*/
              qs = theribfragptr->splineset[num].b4strnd->chordset;
              rs = theribfragptr->splineset[num].a1strnd->chordset; /*ALPHA*/
              ss = theribfragptr->splineset[num].a2strnd->chordset;

              if(num > 4)
              {
                 pspre = theribfragptr->splineset[num-1].b3strnd->chordset;
                 qspre = theribfragptr->splineset[num-1].b4strnd->chordset;
                 rspre = theribfragptr->splineset[num-1].a1strnd->chordset;
                 sspre = theribfragptr->splineset[num-1].a2strnd->chordset;
              }
           }
           if(Lexplicitribbon  /*071002*/
           &&((!Lskeinedribbon && theresidueset[num].resptr->explicit & ALPHA2D)
            ||(!Lskeinedribbon && theresidueset[num].resptr->explicit & BETA2D)
            ||(Lskeinedribbon) ) )
           {/*generic ARROWhead and tail placement 061021 */
              if(    (theresidueset[num-1].resptr->explicit & ALL2D)
                  != (theresidueset[num].resptr->explicit & ALL2D) )
              {
                 Ltail=1;
              }
              else if(   (   (theresidueset[num].resptr->explicit & ALPHA2D)
                           ||(theresidueset[num].resptr->explicit & BETA2D)
                           ||(theresidueset[num].resptr->explicit & NUCLEIC2D)
                         )
                       &&(theresidueset[num-1].resptr->explicit & ARROW2D) )
              {
                 Ltail=1;
              }
                  /*trigger Ltail on ribbon if previous is an ARROW 061021*/
              else {Ltail = 0;}

              if(theresidueset[num].resptr->explicit & ARROW2D)
              {
                 Lhead=1;
              }
              else if(   (theresidueset[num+1].resptr->explicit & ALL2D)
                      != (theresidueset[num].resptr->explicit & ALL2D) )
                 /*catch change from current residue type */
                 {
                    Lhead=1; /*and re-type this residue so look-back will work*/
                    theresidueset[num].resptr->explicit =
                       (theresidueset[num].resptr->explicit | ARROW2D);
                 }
              else if(strcmp(theresidueset[num+1].resptr->ribidentity,
                     theresidueset[num].resptr->ribidentity) != 0)
                 /*ARROW before differently named section*/
                 {
                    Lhead=1; /*and re-type this residue so look-back will work*/
                    theresidueset[num].resptr->explicit =
                       (theresidueset[num].resptr->explicit | ARROW2D);
                 } /*arrow head where ribidentity changes  061004*/
              else {Lhead = 0;}
           }

           /*}}}define strand_chordset for both faced and skeined ribbons*/
/*+++ FACED RIBBON FACES +++++++++++++++++++++++++++++++++++++++++++++++++++++*/
        /*{{{faced ribbon*/
          /*{{{faced ribbon faces: set pt-color,loop over chords,define points*/
           if(!Lskeinedribbon)
           {/*faced ribbon faces +++++++++++++++++++++++++++++++++++++++++*/
            Ue[0]='\0'; /*{sprintf(Ue," P ";}*/ /*Unpickable indicator*/
            Listrib = 1;
            typenow=typerib; /*global output controls for old prekin*/
            ++pointcnt[typenow];

            /*{{{------ set or use default pointcolor */
            /*Secondary Structure colorsets, ALPHA,BETA,COIL,... at list level*/
            /*point colors specific for current residue defined here...*/
            colorstr[0] = '\0'; /*default: NO pointcolor */

            if(LcolorbyNtoC) /*-colornc*/
            {/*num,maxres are counters: 3dummies --- residues --- 1dummy */
               /*(float)tenth needed for short sequences , e.g. 5bpti  061029*/
               /*tenth = ((float)maxres-4)/10; maxres of fragment*/
               tenth = (float)((thisribsectionptr->number)-4)/10; /*090219*/
               if     (numinsection < (3+1*(tenth))){sprintf(color," blue");}
               else if(numinsection < (3+2*(tenth))){sprintf(color," sky");}
               else if(numinsection < (3+3*(tenth))){sprintf(color," cyan");}
               else if(numinsection < (3+4*(tenth))){sprintf(color," sea");}
               else if(numinsection < (3+5*(tenth))){sprintf(color," green");}
               else if(numinsection < (3+6*(tenth))){sprintf(color," lime");}
               else if(numinsection < (3+7*(tenth))){sprintf(color," yellow");}
               else if(numinsection < (3+8*(tenth))){sprintf(color," gold");}
               else if(numinsection < (3+9*(tenth))){sprintf(color," orange");}
               else                                 {sprintf(color," red");}
            }
            else if(LcolorbyBval)
            {/*021213*/
               sprintf(color," %s",colorscale[Ncolorscale].color); /*130121 %s*/
                  /*high value default*/
               for(ncol=1; ncol < Ncolorscale; ncol++)
               {
                 if(theresidueset[num].resptr->Bvalrib < colorscale[ncol].value)
                 {/*under this value*/
                    sprintf(color," %s",colorscale[ncol].color);
                    break;
                 }
               }
            }
            else if(Lcolorbycurvature)
            {
               /*NOTE: this is extendedness of backbone chain, */
               /*  NOT curvature of ribbon*/
               extended = theresidueset[num].resptr->extend;
               if(extended < 0.2)      {sprintf(color," red");}
               else if(extended < 0.4) {sprintf(color," orange");}
               else if(extended < 0.6) {sprintf(color," yellow");}
               else if(extended < 0.8) {sprintf(color," lime");}
               else                    {sprintf(color," green");}
            }
            else
            {
               color[0] = '\0'; /*default: NO pointcolor */
            }
            strcpy(colorstr,color); /*store pointcolor string*/
            /*}}}set or use default pointcolor*/

            for(j=0; j<ichord; j++)
            {/*loop over chords to make face triangles and fat coil*/
             /**********face triangles: cntl = rb.f */
              /*reset color & cntl because tail can be single strand stub*/
              /*cntl rbce for vectorlist (uses coil colorset at list level)  */
              /*color by sec struct type, and specify ribbonlist for a,b,c */
              /*NOTE: Nucleic arrows treated as type==BETA */
              if(type==ALPHA){sprintf(cntl,"rbaf");}
              if(type==BETA) {sprintf(cntl,"rbbf");} /*NOTE rbBf below 060929*/
              if(type==COIL) {sprintf(cntl,"rbcf");}
              if(type==FUDGE){sprintf(cntl,"rbcf");}


              if(  (theresidueset[num].resptr->ribidentity[0] != '\0')/*060929*/
                 &&(type==BETA) )
              {  /*ribbon identity from PDB SHEET record 060929 */
                 sprintf(cntl,"rbBf"); /*NOTE cap B signals {colorset name} */
                 sprintf(setname,"{%s}",theresidueset[num].resptr->ribidentity);
              }
              else {setname[0] = '\0';} /*takes no space as a %s field*/

              strcpy(color,colorstr); /*reset in case coil or edge set color*/
              /*{{{-------- FACE CONTINUE */
              if(   (type!=COIL || type==FUDGE)
                 &&(   (j==0 && !Ltail )
                    || (j==1)
                    || (j==2 && !(Lhead && type==BETA))/*NOT at BETA arrowhead*/
                    || (j==3 && !Lhead) ) )
              {/*preceding in 2D, continue with crosstie for ribbon triangles*/
                 if(j==0 && num > 4) /*after the first real residue...*/
                 {/*repeat crosstie of previous residue to fill inbetween*/
                   sprintf(temps,"%s%s{R3 %s}P%s%s %.3f, %.3f, %.3f"EOLO
                       ,cntl,setname,resIDpre,Ue,color
                       ,pspre[3].pt->x,pspre[3].pt->y,pspre[3].pt->z);
                     putonetextblockline(&mainscratch,temps);
                   sprintf(temps,"%s%s{R3 %s}L%s%s %.3f, %.3f, %.3f"EOLO
                       ,cntl,setname,resIDpre,Ue,color
                       ,qspre[3].pt->x,qspre[3].pt->y,qspre[3].pt->z);
                     putonetextblockline(&mainscratch,temps);
                 }
                 sprintf(temps,"%s%s{R%d %s}P%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,setname,j,resID,Ue,color
                     ,ps[j].pt->x,ps[j].pt->y,ps[j].pt->z);
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s%s{R%d %s}L%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,setname,j,resID,Ue,color
                     ,qs[j].pt->x,qs[j].pt->y,qs[j].pt->z);
                   putonetextblockline(&mainscratch,temps);
              }
              /*}}}-------- FACE CONTINUE */
              /*{{{-------- FACE ARROWHEAD TIP */
              else if(j==3 && Lhead)
              {/*make a double point on the midline for arrowhead tip*/
                 sprintf(temps,"%s%s{R%d %s}%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,setname,j,resID,Ue,color
                     ,ms[j].pt->x,ms[j].pt->y,ms[j].pt->z);
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s%s{R%d %s}%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,setname,j,resID,Ue,color
                     ,ms[j].pt->x,ms[j].pt->y,ms[j].pt->z);
                   putonetextblockline(&mainscratch,temps);
              }
              /*}}}-------- FACE ARROWHEAD TIP */
              /*{{{-------- FACE TAIL */
              else if(j==0 && Ltail)
              {
                 /*side triangles to make a concave beginning*/
                 /*start in middle and swirl around, many dummy triangles*/
                 /*ribbonlist takes successive, rolling 3 points in pairs*/
                 /*(this needs a drawing here!!!!)*/
                 /* chord0   chord1               */
                 /*   6        5,8     strand q   */
                 /*          1,4,7     strand m   */
                 /*   2        3       strand p   */
                 /*triangles are made taking: 1,2,3 then 2,3,4 make a pair */
                 /*then 3,4,5 and 4,5,6  ...  */
                 /*pairs are given the same normal vector for lighting */

                 sprintf(temps,"%s%s{R1 %s}P%s%s %.3f, %.3f, %.3f"EOLO
                      ,cntl,setname,resID,Ue,color
                      ,ms[1].pt->x,ms[1].pt->y,ms[1].pt->z);  /* 1 m1*/
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s%s{R0 %s}L%s%s %.3f, %.3f, %.3f"EOLO
                      ,cntl,setname,resID,Ue,color
                      ,ps[0].pt->x,ps[0].pt->y,ps[0].pt->z);  /* 2 p0*/
                   putonetextblockline(&mainscratch,temps);

                 sprintf(temps,"%s%s{R1 %s}P%s%s %.3f, %.3f, %.3f"EOLO
                      ,cntl,setname,resID,Ue,color
                      ,ps[1].pt->x,ps[1].pt->y,ps[1].pt->z);  /* 3 p1*/
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s%s{R1 %s}L%s%s %.3f, %.3f, %.3f"EOLO
                      ,cntl,setname,resID,Ue,color
                      ,ms[1].pt->x,ms[1].pt->y,ms[1].pt->z);  /* 4 m1*/
                   putonetextblockline(&mainscratch,temps);

                 sprintf(temps,"%s%s{R1 %s}P%s%s %.3f, %.3f, %.3f"EOLO
                      ,cntl,setname,resID,Ue,color
                      ,qs[1].pt->x,qs[1].pt->y,qs[1].pt->z);  /* 5 q1*/
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s%s{R0 %s}L%s%s %.3f, %.3f, %.3f"EOLO
                      ,cntl,setname,resID,Ue,color
                      ,qs[0].pt->x,qs[0].pt->y,qs[0].pt->z);  /* 6 q0*/
                   putonetextblockline(&mainscratch,temps);

                 sprintf(temps,"%s%s{R1 %s}P%s%s %.3f, %.3f, %.3f"EOLO
                      ,cntl,setname,resID,Ue,color
                      ,ms[1].pt->x,ms[1].pt->y,ms[1].pt->z);  /* 7 m1*/
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s%s{R1 %s}L%s%s %.3f, %.3f, %.3f"EOLO
                      ,cntl,setname,resID,Ue,color
                      ,qs[1].pt->x,qs[1].pt->y,qs[1].pt->z);  /* 8 q1*/
                   putonetextblockline(&mainscratch,temps);
              /*__________face triangles: cntl = rb.f */
                 /*midline thick line*/

                 /*if(!LcolorbyNtoC && !LcolorbyBval && !Lcolorbycurvature)*/
                    /*{sprintf(color," white");}*/   /*jumper 090704*/
                 /*else use this region's point color*/
                 sprintf(W," width%d ",onestrandcoilwidth);
                 if((thick=(onestrandcoilwidth+2)) >7){thick = 7;} /*090704*/
                 sprintf(Z," width%d ",thick); /*090704*/
                 /*first make a thick black worm behind colored worm*/
                 if(num > 4)
                 {
                   /*reach back and connect from previous residue mid chord 3*/
                  sprintf(cntl,"rbce"); /*vectorlist rear black edge 090704*/
                  sprintf(temps,"%s{R3 %s}P%s deadblack %s %.3f, %.3f, %.3f"EOLO
                       ,cntl,resID,Ue,Z
                       ,mspre[3].pt->x,mspre[3].pt->y,mspre[3].pt->z);/*pre m3*/
                     putonetextblockline(&mainscratch,temps);
                  sprintf(temps,"%s{R0 %s}L%s deadblack %s %.3f, %.3f, %.3f"EOLO
                       ,cntl,resID,Ue,Z
                       ,ms[0].pt->x,ms[0].pt->y,ms[0].pt->z);  /* m0 */
                     putonetextblockline(&mainscratch,temps);
                 }
                 /*make a short stub in the first chord of this residue*/
                 sprintf(cntl,"rbce"); /*vectorlist rear black edge 090704*/
                 sprintf(temps,"%s{R0 %s}P%s deadblack %s %.3f, %.3f, %.3f"EOLO
                      ,cntl,resID,Ue,Z
                      ,ms[0].pt->x,ms[0].pt->y,ms[0].pt->z);  /* m0 */
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s{R1 %s}L%s deadblack %s %.3f, %.3f, %.3f"EOLO
                      ,cntl,resID,Ue,Z
                      ,ms[1].pt->x,ms[1].pt->y,ms[1].pt->z);  /* m1 */
                   putonetextblockline(&mainscratch,temps);
                 /*then make the colored worm*/
                 if(num > 4)
                 {
                   /*reach back and connect from previous residue mid chord 3*/
                   sprintf(cntl,"rbcc"); /*vectorlist fore inside black edge*/
                   sprintf(temps,"%s{R3 %s}P%s%s%s %.3f, %.3f, %.3f"EOLO
                       ,cntl,resID,Ue,color,W
                       ,mspre[3].pt->x,mspre[3].pt->y,mspre[3].pt->z);/*pre m3*/
                     putonetextblockline(&mainscratch,temps);
                   sprintf(temps,"%s{R0 %s}L%s%s%s %.3f, %.3f, %.3f"EOLO
                       ,cntl,resID,Ue,color,W
                       ,ms[0].pt->x,ms[0].pt->y,ms[0].pt->z);  /* m0 */
                     putonetextblockline(&mainscratch,temps);
                 }
                 /*make a short stub in the first chord of this residue*/
                 sprintf(cntl,"rbcc"); /*vectorlist fore inside black edge*/
                 sprintf(temps,"%s{R0 %s}P%s%s%s %.3f, %.3f, %.3f"EOLO
                      ,cntl,resID,Ue,color,W
                      ,ms[0].pt->x,ms[0].pt->y,ms[0].pt->z);  /* m0 */
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s{R1 %s}L%s%s%s %.3f, %.3f, %.3f"EOLO
                      ,cntl,resID,Ue,color,W
                      ,ms[1].pt->x,ms[1].pt->y,ms[1].pt->z);  /* m1 */
                   putonetextblockline(&mainscratch,temps);

              }
              /*}}}-------- FACE TAIL */
              /*{{{-------- COIL FAT LINE */
              else if(type==COIL) /*090622 target coil with black edge...*/
              {/*COIL*/
                 /*midline thick line*/
                 sprintf(W," width%d ",onestrandcoilwidth);
                 if((thick=(onestrandcoilwidth+2)) >7){thick = 7;} /*090704*/
                 sprintf(Z," width%d ",thick); /*090704*/
                if(j==0 && (num > 4)) /*num==4 is first real residue*/
                {
                 /*reach back and connect from previous residue's mid chord 3 */

                 sprintf(cntl,"rbce"); /*vectorlist rear for black edge 090704*/
                 sprintf(temps,"%s{R0 %s}P%s deadblack %s %.3f, %.3f, %.3f"EOLO
                      ,cntl,resID,Ue,Z
                      ,mspre[3].pt->x,mspre[3].pt->y,mspre[3].pt->z);
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s{R0 %s}L%s deadblack %s %.3f, %.3f, %.3f"EOLO
                      ,cntl,resID,Ue,Z
                      ,ms[0].pt->x,ms[0].pt->y,ms[0].pt->z);  /* m0 */
                   putonetextblockline(&mainscratch,temps);


                 sprintf(cntl,"rbcc"); /*vectorlist fore inside black edge*/
                 sprintf(temps,"%s{R0 %s}P%s%s%s %.3f, %.3f, %.3f"EOLO
                      ,cntl,resID,Ue,color,W
                      ,mspre[3].pt->x,mspre[3].pt->y,mspre[3].pt->z);
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s{R0 %s}L%s%s%s %.3f, %.3f, %.3f"EOLO
                      ,cntl,resID,Ue,color,W
                      ,ms[0].pt->x,ms[0].pt->y,ms[0].pt->z);  /* m0 */
                   putonetextblockline(&mainscratch,temps);
                }
                else if(j>0)  /*interior of a residue-spline region */
                {
                 /*connect the midline chords of this residue*/

                 sprintf(cntl,"rbce"); /*vectorlist rear for black edge 090704*/
                 sprintf(temps,"%s{R%d %s}P%s deadblack %s %.3f, %.3f, %.3f"EOLO
                      ,cntl,j,resID,Ue,Z
                      ,ms[j].pt->x,ms[j].pt->y,ms[j].pt->z);  /* ms */
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s{R%d %s}L%s deadblack %s %.3f, %.3f, %.3f"EOLO
                      ,cntl,j-1,resID,Ue,Z
                      ,ms[j-1].pt->x,ms[j-1].pt->y,ms[j-1].pt->z);  /* ms-1 */
                   putonetextblockline(&mainscratch,temps);

                 sprintf(cntl,"rbcc"); /*vectorlist fore inside black edge*/
                 sprintf(temps,"%s{R%d %s}P%s%s%s %.3f, %.3f, %.3f"EOLO
                      ,cntl,j,resID,Ue,color,W
                      ,ms[j].pt->x,ms[j].pt->y,ms[j].pt->z);  /* ms */
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s{R%d %s}L%s%s%s %.3f, %.3f, %.3f"EOLO
                      ,cntl,j-1,resID,Ue,color,W
                      ,ms[j-1].pt->x,ms[j-1].pt->y,ms[j-1].pt->z);  /* ms-1 */
                   putonetextblockline(&mainscratch,temps);
                }
              }/*COIL*/
              /*}}}-------- COIL FAT LINE */
              /*{{{-------- FACE BETA ARROWHEAD WINGS */
              /* ***extra here for BETA only*** */
              if(j==1 && Lhead && type==BETA)
              {/*j==1 and at the end of a BETA region*/

                 /*make wings for the arrowhead*/
                 sprintf(temps,"%s%s{R1 WING-P %s}P%s%s %.3f, %.3f, %.3f"EOLO
                         ,cntl,setname,resID,Ue,color
                         ,(ms[1].pt->x +(ps[1].pt->x - ms[1].pt->x)*2)
                         ,(ms[1].pt->y +(ps[1].pt->y - ms[1].pt->y)*2)
                         ,(ms[1].pt->z +(ps[1].pt->z - ms[1].pt->z)*2)
                          );
                 putonetextblockline(&mainscratch,temps);

                 sprintf(temps,"%s%s{R1 WING-Q %s}L%s%s %.3f, %.3f, %.3f"EOLO
                         ,cntl,setname,resID,Ue,color
                         ,(ms[1].pt->x +(qs[1].pt->x - ms[1].pt->x)*2)
                         ,(ms[1].pt->y +(qs[1].pt->y - ms[1].pt->y)*2)
                         ,(ms[1].pt->z +(qs[1].pt->z - ms[1].pt->z)*2)
                          );
                 putonetextblockline(&mainscratch,temps);

              }
              /*}}}-------- FACE BETA ARROWHEAD WINGS */
            }/*loop over chords to make face triangles and fat coil*/
           }/*faced ribbon faces ++++++++++++++++++++++++++++++++++++++++*/
           /*}}}faced ribbon faces: set color,loop over chords, define points*/
/*+++ FACED RIBBON FACES +++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++ FACED RIBBON EDGES +++++++++++++++++++++++++++++++++++++++++++++++++++++*/
           /*{{{faced ribbon edges: loop over chords, construct edge vectors  */
           if(!Lskeinedribbon)
           {/*faced ribbon edges*/

            sprintf(Ue," U "); /*{Ue[0]='\0';}*/ /*Unpickable indicator*/
            sprintf(color," deadblack"); /*edges all deadblack*/
            if(type==ALPHA){sprintf(cntl,"rbae");}
            else           {sprintf(cntl,"rbbe");}
            for(j=0; j<ichord; j++)
            {/*loop over chords to construct edge vectors*/
              /*{{{-------- EDGE CONTINUE */
              if(   type!=COIL
                 &&(   (j==0 && !Ltail )
                    || (j==1 && !Ltail )
                    || (j==2 && !(Lhead && type==BETA))/*NOT at BETA arrowhead*/
                    || (j==3 && !Lhead ) ) )
              {/*all chords: preceding in 2D, continue with edges*/
                if(j==0 && (num > 4)) /*num==4 is first real residue*/
                {
                /*reach back and connect from previous residue's edge chord 3 */
                 sprintf(temps,"%s{R3 %s}P%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,resIDpre,Ue,color
                     ,pspre[3].pt->x,pspre[3].pt->y,pspre[3].pt->z);
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s{R0 %s}L%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,resID,Ue,color
                     ,ps[0].pt->x,ps[0].pt->y,ps[0].pt->z);  /* m0 */
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s{R3 %s}P%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,resIDpre,Ue,color
                     ,qspre[3].pt->x,qspre[3].pt->y,qspre[3].pt->z);
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s{R0 %s}L%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,resID,Ue,color
                     ,qs[0].pt->x,qs[0].pt->y,qs[0].pt->z);  /* m0 */
                   putonetextblockline(&mainscratch,temps);
                }
                else if(j>0)  /*interior of a residue-spline region */
                {
                 /*connect the edge chords of this residue*/
                 sprintf(temps,"%s{R%d %s}P%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,j-1,resID,Ue,color
                     ,ps[j-1].pt->x,ps[j-1].pt->y,ps[j-1].pt->z);  /* n-1 */
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s{R%d %s}L%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,j,resID,Ue,color
                     ,ps[j].pt->x,ps[j].pt->y,ps[j].pt->z);  /* n */
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s{R%d %s}P%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,j-1,resID,Ue,color
                     ,qs[j-1].pt->x,qs[j-1].pt->y,qs[j-1].pt->z);  /* n-1 */
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s{R%d %s}L%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,j,resID,Ue,color
                     ,qs[j].pt->x,qs[j].pt->y,qs[j].pt->z);  /* n */
                   putonetextblockline(&mainscratch,temps);
                }
              }/*all chords: preceding in 2D, continue with edges*/
              /*}}}-------- EDGE CONTINUE */
              /*{{{-------- EDGE TAIL */
              else if(j==1 && Ltail) /*this works for both ALPHA and BETA*/
              {/*Ltail*/
                 /*side triangles make a concave beginning*/
                 /*outline their trailing edges with edge vectors */
                 /*so 2 edges filled from chord 0 to chord 1 */
                 sprintf(temps,"%s{R1 %s}P%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,resID,Ue,color
                     ,ms[1].pt->x,ms[1].pt->y,ms[1].pt->z);
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s{R0 %s}L%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,resID,Ue,color
                     ,ps[0].pt->x,ps[0].pt->y,ps[0].pt->z);
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s{R1 %s}L%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,resID,Ue,color
                     ,ps[1].pt->x,ps[1].pt->y,ps[1].pt->z);
                   putonetextblockline(&mainscratch,temps);

                 sprintf(temps,"%s{R1 %s}P%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,resID,Ue,color
                     ,ms[1].pt->x,ms[1].pt->y,ms[1].pt->z);
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s{R0 %s}L%s%s %.3f, %.3f, %.3f"EOLO
                    ,cntl,resID,Ue,color,qs[0].pt->x,qs[0].pt->y,qs[0].pt->z);
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s{R1 %s}L%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,resID,Ue,color
                     ,qs[1].pt->x,qs[1].pt->y,qs[1].pt->z);
                   putonetextblockline(&mainscratch,temps);
              }/*Ltail*/
              /*}}}-------- EDGE TAIL */
              /*{{{-------- EDGE BETA ARROWHEAD */
              if(j==1 && Lhead && type==BETA)
              {/*j==1 and at the end of a BETA region*/
                 /*outline wings of the arrowhead, out at chord 1, to mid at 3*/
                 sprintf(temps,"%s{b3R1 %s}P%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,resID,Ue,color
                     ,ps[1].pt->x,ps[1].pt->y,ps[1].pt->z);
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s{b3R1 %s}L%s%s %.3f, %.3f, %.3f"EOLO
                         ,cntl,resID,Ue,color
                         ,(ms[1].pt->x +(ps[1].pt->x - ms[1].pt->x)*2)
                         ,(ms[1].pt->y +(ps[1].pt->y - ms[1].pt->y)*2)
                         ,(ms[1].pt->z +(ps[1].pt->z - ms[1].pt->z)*2)
                        );
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s{m0R3 %s}L%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,resID,Ue,color
                     ,ms[3].pt->x,ms[3].pt->y,ms[3].pt->z);
                   putonetextblockline(&mainscratch,temps);

                 sprintf(temps,"%s{b4R1 %s}P%s%s %.3f, %.3f, %.3f"EOLO
                    ,cntl,resID,Ue,color
                    ,qs[1].pt->x,qs[1].pt->y,qs[1].pt->z);
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s{b4R1 %s}L%s%s %.3f, %.3f, %.3f"EOLO
                         ,cntl,resID,Ue,color
                         ,(ms[1].pt->x +(qs[1].pt->x - ms[1].pt->x)*2)
                         ,(ms[1].pt->y +(qs[1].pt->y - ms[1].pt->y)*2)
                         ,(ms[1].pt->z +(qs[1].pt->z - ms[1].pt->z)*2)
                          );
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s{m0R3 %s}L%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,resID,Ue,color
                     ,ms[3].pt->x,ms[3].pt->y,ms[3].pt->z);
                   putonetextblockline(&mainscratch,temps);
              }/*j==1 and at the end of a BETA region*/
              /*}}}-------- EDGE BETA ARROWHEAD */
              /*{{{-------- EDGE ALPHA ARROWHEAD */
              if(j==2 && Lhead && type==ALPHA)
              {/*j==2 and at the end of a ALPHA region*/
                 /*outline wings of the arrowhead, out at chord 1, to mid at 3*/
                 sprintf(temps,"%s{b3R2 %s}P%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,resID,Ue,color
                     ,ps[2].pt->x,ps[2].pt->y,ps[2].pt->z);
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s{m0R3 %s}L%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,resID,Ue,color
                     ,ms[3].pt->x,ms[3].pt->y,ms[3].pt->z);
                   putonetextblockline(&mainscratch,temps);

                 sprintf(temps,"%s{b4R2 %s}P%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,resID,Ue,color
                     ,qs[2].pt->x,qs[2].pt->y,qs[2].pt->z);
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s{m0R3 %s}L%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,resID,Ue,color
                     ,ms[3].pt->x,ms[3].pt->y,ms[3].pt->z);
                   putonetextblockline(&mainscratch,temps);
              }/*j==2 and at the end of a ALPHA region*/
              /*}}}-------- EDGE ALPHA ARROWHEAD */
            }/*loop over chords to construct edge vectors*/
           }/*faced ribbon edges*/
           /*}}}faced ribbon edges: loop over chords, construct edge vectors  */
         /*}}}faced ribbon*/
/*+++ FACED RIBBON EDGES +++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++ SKEINED RIBBON +++++++++++++++++++++++++++++++++++++++++++++++(140520)++*/
           /*{{{----skeined ribbon:loop over chords, construct strand vectors*/
           if(Lskeinedribbon)
           {/*skeined ribbon strands*/

            Ue[0]='\0'; /*{sprintf(Ue," U ");}*/ /*Unpickable indicator*/
            sprintf(color," gold");
            if  (Lbyresidue && num%2){sprintf(cntl,"rb1c");}/*odd-residue coil*/
            else if(Lbyresidue){sprintf(cntl,"rb2c");} /*even-residue coil*/
            else {sprintf(cntl,"rb  ");} /*all are in one vectorlist*/
            Listrib = 1;
            typenow=typerib; /*global output controls for old prekin*/
            ++pointcnt[typenow];
            if(nstrnd==1){nmax = 1;}
            else if(nstrnd < 4){nmax = 3;}
            else {nmax = 5;}

            /*{{{------ loop over strands*/
            for(mn=0; mn < nmax; mn++)
            {/*loop over strands*/
               /*xs is the working strand*/
               if     (mn==0){xs = ms;xspre = mspre;sprintf(strnd,"m0");}
               else if(mn==1){xs = ps;xspre = pspre;sprintf(strnd,"b3");}
               else if(mn==2){xs = qs;xspre = qspre;sprintf(strnd,"b4");}
               else if(mn==3){xs = rs;xspre = rspre;sprintf(strnd,"a1");}
               else if(mn==4){xs = ss;xspre = sspre;sprintf(strnd,"a2");}
               /*{{{-------- loop over chords*/
             for(j=0; j<ichord; j++)
             {/*loop over chords to construct strand vectors*/
               /*{{{--------- VRML splines*/
               if(LdumpVRML && LsplineVRML) /*050204*/
               {/*VRML*/
                  sprintf(cntl,"VR%2d",n);
                  sprintf(whostr,"spline");
                  radius = ribwidVRML/nstrnd;
                  if(j==0 && (num > 4)) /*num==4 is first real residue*/
                  {/*reach back and connect from previous residue's chord 3 */
                     writeVRMLvectortoscratch(cntl,color,radius,whostr
                        ,xspre[3].pt->x,xspre[3].pt->y,xspre[3].pt->z
                        ,xs[0].pt->x,xs[0].pt->y,xs[0].pt->z);
                  }
                  else if(j>0)  /*interior of a residue-spline region */
                  {/*interior chords*/
                     writeVRMLvectortoscratch(cntl,color,radius,whostr
                        ,xs[j-1].pt->x,xs[j-1].pt->y,xs[j-1].pt->z
                        ,xs[j].pt->x,xs[j].pt->y,xs[j].pt->z);
                  }
               }/*VRML*/
               /*}}}VRML splines*/
               else
               /*{{{--------- kin splines*/
               {/*kin*/
                  if(j==0 && (num > 4)) /*num==4 is first real residue*/
                  {/*reach back and connect from previous residue's chord 3 */
                   if(Lsplinevector) /*bare minimum vectorlist 140520*/
                   {/*bare minimum vectorlist 140520*/
                     sprintf(temps,"%s{%sR3 %s} %s%s %.3f, %.3f, %.3f"EOLO   
                         ,cntl,strnd,resIDpre,Ue,color
                         ,xspre[3].pt->x,xspre[3].pt->y,xspre[3].pt->z);  
                       putonetextblockline(&mainscratch,temps);
                   }/*bare minimum vectorlist 140520*/
                   else
                   {/*P,L per segment (140520)*/
                     sprintf(temps,"%s{%sR3 %s}P%s%s %.3f, %.3f, %.3f"EOLO
                         ,cntl,strnd,resIDpre,Ue,color
                         ,xspre[3].pt->x,xspre[3].pt->y,xspre[3].pt->z);
                       putonetextblockline(&mainscratch,temps);
                     sprintf(temps,"%s{%sR0 %s}L%s%s %.3f, %.3f, %.3f"EOLO
                         ,cntl,strnd,resID,Ue,color
                         ,xs[0].pt->x,xs[0].pt->y,xs[0].pt->z);  /* m0 */
                       putonetextblockline(&mainscratch,temps);
                   }/*P,L per segment (140520)*/
                  }
                  else if(j>0)  /*interior of a residue-spline region */
                  {/*interior chords*/
                   if(Lsplinevector) /*bare minimum vectorlist 140520*/
                   {/*bare minimum vectorlist 140520*/
                     sprintf(temps,"%s{%sR%d %s} %s%s %.3f, %.3f, %.3f"EOLO   
                         ,cntl,strnd,j-1,resID,Ue,color
                         ,xs[j-1].pt->x,xs[j-1].pt->y,xs[j-1].pt->z);  
                       putonetextblockline(&mainscratch,temps);
                   }/*bare minimum vectorlist 140520*/
                   else
                   {/*P,L per segment (140520)*/
                     sprintf(temps,"%s{%sR%d %s}P%s%s %.3f, %.3f, %.3f"EOLO
                         ,cntl,strnd,j-1,resID,Ue,color
                         ,xs[j-1].pt->x,xs[j-1].pt->y,xs[j-1].pt->z);
                       putonetextblockline(&mainscratch,temps);
                     sprintf(temps,"%s{%sR%d %s}L%s%s %.3f, %.3f, %.3f"EOLO
                         ,cntl,strnd,j,resID,Ue,color
                         ,xs[j].pt->x,xs[j].pt->y,xs[j].pt->z);
                       putonetextblockline(&mainscratch,temps);
                   }/*P,L per segment (140520)*/
                  }/*interior chords*/
               }/*kin*/
               /*}}}kin splines*/
               /*then check for arrowhead, both for VRML and kin*/
               /*{{{--------- arrowhead*/
               if(j==1 && Lhead && Larrowhead && (mn==1 || mn==2))
               {/*j==1 and at the end of a BETA/ALPHA region: make arrowhead*/
                 /*outline wings of arrowhead: out at chord 1, to mid at 3*/
                 /*{{{------------ VRML*/
                 if(LdumpVRML && LarrowVRML) /*050212 arrowhead*/
                 {/*VRML*/
                    /*draw out from n side the distance from center to edge*/
                    /* VRML originally drew from the centerline itself*/
                    sprintf(cntl,"VRML");
                    sprintf(whostr,"arrow");
                    radius = ribwidVRML/nstrnd;
                    writeVRMLvectortoscratch(cntl,color,radius,whostr
                       ,xs[1].pt->x,xs[1].pt->y,xs[1].pt->z
                       ,(ms[1].pt->x +(xs[1].pt->x - ms[1].pt->x)*2)
                       ,(ms[1].pt->y +(xs[1].pt->y - ms[1].pt->y)*2)
                       ,(ms[1].pt->z +(xs[1].pt->z - ms[1].pt->z)*2) );

                    /*draw from flange made at ttt-2 to centerline at ttt*/
                    sprintf(cntl,"VRML");
                    sprintf(whostr,"arrow");
                    radius = ribwidVRML/nstrnd;
                    writeVRMLvectortoscratch(cntl,color,radius,whostr
                       ,(ms[1].pt->x +(xs[1].pt->x - ms[1].pt->x)*2)
                       ,(ms[1].pt->y +(xs[1].pt->y - ms[1].pt->y)*2)
                       ,(ms[1].pt->z +(xs[1].pt->z - ms[1].pt->z)*2)
                       ,ms[3].pt->x,ms[3].pt->y,ms[3].pt->z);
                 }/*VRML*/
                 /*}}}VRML*/
                 else
                 /*{{{------------ kin*/
                 {/*kin*/
                    sprintf(temps,"%s{%sHead1 %s}P%s%s %.3f, %.3f, %.3f"EOLO
                         ,cntl,strnd,resID,Ue,color
                         ,xs[1].pt->x,xs[1].pt->y,xs[1].pt->z);
                      putonetextblockline(&mainscratch,temps);
                    sprintf(temps,"%s{%sHead1 %s}L%s%s %.3f, %.3f, %.3f"EOLO
                         ,cntl,strnd,resID,Ue,color
                         ,(ms[1].pt->x +(xs[1].pt->x - ms[1].pt->x)*2)
                         ,(ms[1].pt->y +(xs[1].pt->y - ms[1].pt->y)*2)
                         ,(ms[1].pt->z +(xs[1].pt->z - ms[1].pt->z)*2)
                          );
                      putonetextblockline(&mainscratch,temps);
                    sprintf(temps,"%s{%sHead3 %s}L%s%s %.3f, %.3f, %.3f"EOLO
                         ,cntl,strnd,resID,Ue,color
                         ,ms[3].pt->x,ms[3].pt->y,ms[3].pt->z);
                      putonetextblockline(&mainscratch,temps);
                 }/*kin*/
                 /*}}}kin*/
               }/*j==1 and at the end of a BETA/ALPHA region: make arrowhead*/
               /*}}}arrowhead*/
             }/*loop over chords to construct strand vectors*/
             /*}}}loop over chords*/
            }/*loop over strands*/
            /*}}}loop over strands*/
           }/*skeined ribbon strands*/
           /*}}}skeined ribbon: loop over chords, construct strand vectors*/

           /*{{{---- CBstubs (optional) */
           if(    Lcbstubsrib
               && (theresidueset[num].resptr->cbetaptr != NULL)
               && theresidueset[num].resptr->Lstub ) /*set when residue loaded*/
           {
#ifdef OLDCODE
              if(Lskeinedribbon && Lbyresidue && num%2)
                   {sprintf(cntl,"rb1c");}/*add to odd-residue coil*/
              else if(Lskeinedribbon && Lbyresidue)
                   {sprintf(cntl,"rb2c");} /*add to even-residue coil*/
              else if(Lskeinedribbon)
                   {sprintf(cntl,"rb  ");} /*add to the one vectorlist*/
              else {sprintf(cntl,"rbce");} /*coil edge vectorlist*/
#endif
              sprintf(cntl,"scv ");/*goes with sidechain vectors*/
              Listsc  = 1;
              Listscv = 1;

              Ue[0]='\0'; /*{sprintf(Ue," U ");}*/ /*Unpickable indicator*/
              sprintf(color," cyan");

              if(num == CORDER) {j = 0;} /*1st residue 1st guide pt at Calpha*/
              else if(num == maxres-1) {j = 3;} /*last res last guide at CA */
              else {j = 2;} /*interior residue CA near guide 2 */
              if(LdumpVRML && LvectorVRML)
              {
                 sprintf(whostr,"STUB");
                 radius = ribwidVRML/nstrnd;
                 writeVRMLvectortoscratch(cntl,color,radius,whostr
                    ,ms[j].pt->x,ms[j].pt->y,ms[j].pt->z  /* midline at j */
                    ,theresidueset[num].resptr->cbetaptr->x
                    ,theresidueset[num].resptr->cbetaptr->y
                    ,theresidueset[num].resptr->cbetaptr->z);  /*CBeta*/
              }
              else
              {
                 sprintf(temps,"%s{R%d %s}P%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,j,resID,Ue,color
                     ,ms[j].pt->x,ms[j].pt->y,ms[j].pt->z);  /* midline at j */
                   putonetextblockline(&mainscratch,temps);
                 sprintf(temps,"%s{CB %s}L%s%s %.3f, %.3f, %.3f"EOLO
                     ,cntl,resID,Ue,color
                     ,theresidueset[num].resptr->cbetaptr->x
                     ,theresidueset[num].resptr->cbetaptr->y
                     ,theresidueset[num].resptr->cbetaptr->z);  /*CBeta*/
                   putonetextblockline(&mainscratch,temps);
              }
           }
           /*}}}CBstubs (optional) */

         }/*loop over residues*/

	 }/*The actual ribbon code  100322*/ //DCR SML 20151028: if there is a compile error here, it's a mismatched curly brace related this line


    /*}}}for each residue, calculate the appropriate ribbon section*/

    /*{{{---loop up from residue splinesets to section's fragments           */

      }/*splineset*/

     if(theribfragptr == theribsectionptr->lastribfragptr)
     {
        theribfragptr = NULL;
     }
     else
     {
        theribfragptr = theribfragptr->nextptr;
     }
    }/*frag*/ /*while(theribfragptr != NULL)*/
    /*}}}loop up from residue splinesets to section's fragments */

 /*}}}loop through section fragments, work with residue splinesets*/
}
/*___constructsectionribbons()____________________________________________}}}*/

/*{{{constructallribbons()**********calls constructsectionribbons()***********/
void constructallribbons()
{
  ribmodelstruct*   theribmodelptr=NULL;
  ribsectionstruct* theribsectionptr=NULL;

  /*{{{-- loop through models and sections, call constructsectionribbons()*/

  theribmodelptr = firstribmodelptr;
  while(theribmodelptr != NULL)
  {/*model*/

   theribsectionptr = theribmodelptr->firstribsectionptr;
   while(theribsectionptr != NULL)
   {/*section*/

    constructsectionribbons(theribsectionptr);

    if(theribsectionptr == theribmodelptr->lastribsectionptr)
         {  theribsectionptr = NULL;}
    else {  theribsectionptr = theribsectionptr->nextptr;}
   }/*section*/ /*while(theribsectionptr != NULL)*/
   theribmodelptr = theribmodelptr->nextptr; /*==NULL if last model*/
  }/*model*/ /*while(theribmodelptr != NULL)*/
  /*}}}loop through models and sections, call constructsectionribbons()*/
}
/*___constructallribbons()________________________________________________}}}*/

/*{{{makesectionpseudoHbonds()************************************************/
void makesectionpseudoHbonds(ribsectionstruct* theribsectionptr)
{                /*051211 only works on mainchain within one fragment*/

  /*{{{--declarations  */
#define ALPHA 1
#define BETA  2

  ribfragstruct*    theribfragptr=NULL;
  spline*           thesplineset=NULL;
  residue*          theresidueset=NULL;
  struct p*         nhpt=NULL;
  struct p*         copt=NULL;
  char    numID[32];
  char    tryID[32];
  float   distance=0,numwiden=0,trywiden=0;
  int   j=0,mn=0,nmax=0,num=0,try=0,maxres=0; /*chords, strands, residues */
  char  cntl[5]={'\0','\0','\0','\0','\0' };
  char  Ue[5]={'\0','\0','\0','\0','\0' }; /*Unpickable indicator when == U */
  char  color[16];
  float cosnho=0,angfac=0,dimhb=0;dlimhb=0; /*for Hbond calc*/
  /*}}}declarations */

  /*{{{-- loop down through fragments to residue splinesets*/

    theribfragptr = theribsectionptr->firstribfragptr;
    while(theribfragptr != NULL)
    {/*frag*/

      if(theribfragptr->splineset != NULL && theribfragptr->number > 5)
      {/*splineset*/

         maxres = theribfragptr->number;
         thesplineset  = theribfragptr->splineset;
         theresidueset = theribfragptr->residueset;

  /*}}}loop down through fragments to residue splinesets*/

  /*{{{-- double loop over residues*/
         angfac = (float)2.3;
         dlimhb = (float)3.5;
         for(num = CORDER+1; num < maxres; num++) /*can't do HN at Nterm*/
         {/*loop over primary residue numbers*/
            for(try = CORDER; try < maxres-1 ; try++) /*can't do CO at Cterm*/
            {/*loop over secondary residue numbers*/
              if(   theresidueset[num].resptr->hnpepptr != NULL
                 && theresidueset[num].resptr->npeptptr != NULL
                 && theresidueset[try].resptr->ocarbptr != NULL
                 && (try < num-2 || try > num+2) )
              {/*HN here and CO not self or nearest sequence neighbor*/

      /*check distance for possible h-bond from this carbonyl o to working nh */
/*
         disthb = (float)(sqrt(  (nhxyz[1] - oxyz[1])*(nhxyz[1] - oxyz[1])
                       + (nhxyz[2] - oxyz[2])*(nhxyz[2] - oxyz[2])
                       + (nhxyz[3] - oxyz[3])*(nhxyz[3] - oxyz[3]) ) );
         cosnho = (nhxyz[1] - nxyz[1])*(oxyz[1] - nhxyz[1])/disthb
              +   (nhxyz[2] - nxyz[2])*(oxyz[2] - nhxyz[2])/disthb
              +   (nhxyz[3] - nxyz[3])*(oxyz[3] - nhxyz[3])/disthb ;
         dimhb = dlimhb - angfac + angfac*cosnho;
*/
               distance = distanribpntptrs(theresidueset[num].resptr->hnpepptr
                                          ,theresidueset[try].resptr->ocarbptr);
               cosnho = ( (  theresidueset[num].resptr->hnpepptr->x
                           - theresidueset[num].resptr->npeptptr->x)
                         *(  theresidueset[try].resptr->ocarbptr->x
                           - theresidueset[num].resptr->hnpepptr->x)
                         /distance)
                       +( (  theresidueset[num].resptr->hnpepptr->y
                           - theresidueset[num].resptr->npeptptr->y)
                         *(  theresidueset[try].resptr->ocarbptr->y
                           - theresidueset[num].resptr->hnpepptr->y)
                         /distance)
                       +( (  theresidueset[num].resptr->hnpepptr->z
                           - theresidueset[num].resptr->npeptptr->z)
                         *(  theresidueset[try].resptr->ocarbptr->z
                           - theresidueset[num].resptr->hnpepptr->z)
                         /distance);
               dimhb = dlimhb - angfac + angfac*cosnho;
               if(distance <= dimhb)
               {/*H-BOND*/

                  /*{{{---- edge num-NH & try-CO pts */

                  /* COside = 1: nh at b4, co at b3*/
                  /* COside = 2: nh at b3, co at b4*/
                  /* co near chord 3, nh near chord 0 of a residue*/
                  /* nh is ruled by preceding peptide. */
                  if(Lexplicitribbon || Lskeinedribbon)
                  {/*use B edges as outer edges of ribbon*/
                     if( (theresidueset[num-1].resptr->COside)==1)
                          {nhpt = thesplineset[num].b4strnd->chordset[0].pt;}
                     else {nhpt = thesplineset[num].b3strnd->chordset[0].pt;}

                     if( (theresidueset[try].resptr->COside)==1)
                          {copt = thesplineset[try].b3strnd->chordset[3].pt;}
                     else {copt = thesplineset[try].b4strnd->chordset[3].pt;}
                  }
                  else
                  {/*use C edges as outer edges of faced-fudged ribbon*/
                     if( (theresidueset[num-1].resptr->COside)==1)
                          {nhpt = thesplineset[num].c6strnd->chordset[0].pt;}
                     else {nhpt = thesplineset[num].c5strnd->chordset[0].pt;}

                     if( (theresidueset[try].resptr->COside)==1)
                          {copt = thesplineset[try].c5strnd->chordset[3].pt;}
                     else {copt = thesplineset[try].c6strnd->chordset[3].pt;}
                  }

                  /*}}}edge num-NH & try-CO pts */

                  /*{{{---- residueIDs */
                  sprintf(numID,"%s %s %d%s"
                         ,theresidueset[num].resptr->resrib
                         ,theresidueset[num].resptr->subrib
                         ,theresidueset[num].resptr->numrib
                         ,theresidueset[num].resptr->rinsrib );
                  sprintf(tryID,"%s %s %d%s"
                         ,theresidueset[try].resptr->resrib
                         ,theresidueset[try].resptr->subrib
                         ,theresidueset[try].resptr->numrib
                         ,theresidueset[try].resptr->rinsrib );
                  /*}}}residueIDs*/

                  Ue[0]='\0'; /*{sprintf(Ue," U ");}*/ /*Unpickable indicator*/
                  sprintf(color," magenta");
                  sprintf(cntl,"rbhb"); /*all are in one vectorlist*/
                  Listrib = 1;
                  typenow=typerib; /*global output controls for old prekin*/
                  ++pointcnt[typenow];

                  sprintf(temps,"%s{NH %s}P %s%s %.3f, %.3f, %.3f"EOLO
                       ,cntl,numID,Ue,color
                       ,nhpt->x,nhpt->y,nhpt->z);
                    putonetextblockline(&mainscratch,temps);

                  sprintf(temps,"%s{CO %s}L %s%s %.3f, %.3f, %.3f"EOLO
                       ,cntl,tryID,Ue,color
                       ,copt->x,copt->y,copt->z);
                    putonetextblockline(&mainscratch,temps);
               }
              }/*HN here and CO not self or nearest sequence neighbor*/
            }/*loop over secondary residues*/
         }/*loop over primary residues*/

  /*}}}double loop over residues*/

  /*{{{-- loop back up from splinesets through fragments to section*/

      }/*splineset*/

     if(theribfragptr == theribsectionptr->lastribfragptr)
     {
        theribfragptr = NULL;
     }
     else
     {
        theribfragptr = theribfragptr->nextptr;
     }
    }/*frag*/ /*while(theribfragptr != NULL)*/
  /*}}}loop back up from splinesets through fragments to section*/
}
/*___makesectionpseudoHbonds()____________________________________________}}}*/

/*{{{makeallpseudoHbonds()**********calls makesectionpseudoHbonds()***********/
void makeallpseudoHbonds()
{
  ribmodelstruct*   theribmodelptr=NULL;
  ribsectionstruct* theribsectionptr=NULL;

  /*{{{-- loop through models and sections, do fragment residue splinesets*/

  theribmodelptr = firstribmodelptr;
  while(theribmodelptr != NULL)
  {/*model*/

   theribsectionptr = theribmodelptr->firstribsectionptr;
   while(theribsectionptr != NULL)
   {/*section*/

    makesectionpseudoHbonds(theribsectionptr);

    if(theribsectionptr == theribmodelptr->lastribsectionptr)
    {
       theribsectionptr = NULL;
    }
    else
    {
       theribsectionptr = theribsectionptr->nextptr;
    }
   }/*section*/ /*while(theribsectionptr != NULL)*/
   theribmodelptr = theribmodelptr->nextptr; /*==NULL if last model*/
  }/*model*/ /*while(theribmodelptr != NULL)*/

  /*}}}loop through models and sections, do fragment residue splinesets*/
}
/*___makeallpseudoHbonds()________________________________________________}}}*/

/*{{{****utilities: distanribpntptrs(),distanpntptrs(),splinecalc() ****/

/*{{{distanribpntptrs()*******************************************************/
float  distanribpntptrs(ribpntstruct* xyza, ribpntstruct* xyz1)
{
    double  xa=0,ya=0,za=0,x1=0,y1=0,z1=0,x2=0,y2=0,z2=0;
    float   freturn=0;

    xa = (double)xyza->x;
    ya = (double)xyza->y;
    za = (double)xyza->z;
    x1 = (double)xyz1->x;
    y1 = (double)xyz1->y;
    z1 = (double)xyz1->z;

    x2 = ( xa - x1 )*( xa - x1 );
    y2 = ( ya - y1 )*( ya - y1 );
    z2 = ( za - z1 )*( za - z1 );
    freturn = (float)sqrt( x2 + y2 + z2 );

    return(freturn);
}
/*__distanribpntptrs()____________________________________________________}}}*/

/*{{{distanpntptrs()*******************************************************/
float  distanpntptrs(p* xyza, p* xyz1)
{
    double  xa=0,ya=0,za=0,x1=0,y1=0,z1=0,x2=0,y2=0,z2=0;
    float   freturn=0;

    xa = (double)xyza->x;
    ya = (double)xyza->y;
    za = (double)xyza->z;
    x1 = (double)xyz1->x;
    y1 = (double)xyz1->y;
    z1 = (double)xyz1->z;

    x2 = ( xa - x1 )*( xa - x1 );
    y2 = ( ya - y1 )*( ya - y1 );
    z2 = ( za - z1 )*( za - z1 );
    freturn = (float)sqrt( x2 + y2 + z2 );

    return(freturn);
}
/*__distanpntptrs()____________________________________________________}}}*/

/*{{{splinecalc()*************************************************************/
void splinecalc(float Tbone, int IMAX, int X[CORDER*2], float fg[CORDER][4], float spine[10][4])
{          /*does one interval based on CORDER residues*/

   float T,D,E,N[CORDER*2][CORDER+1];
   int   j=0,K=0,I=0,tt=0;
   float g[4]={0,0,0,0};

   /*Tbone: sequential number of the interval over which the spline is calc*/
   /*global ichord: number of spline segments per residue, 4 best for arrows */
   /* spline for the nth interval calc from n-2,n-1,nth,n+1 guidepoints */
   /*  except for very last interval where use  n-3,n-2,n-1,nth */


   for(T=Tbone,tt=0; T<Tbone+1.0-(float).5/ichord; T=T+(float)1.0/ichord,tt++)
   {/*T && tt over Tbone interval: THE SPLINE CALCULATION*/
      for(j=0; j<IMAX; j++)
      {/*set order 1 weighting factors*/
         if(X[j] <= T && T < X[j+1] ) N[j][1] = 1.0;
         else N[j][1] = 0.0;
      }
      N[IMAX][1] = 0; /*always*/

      for(K=2; K<=CORDER; K++)
      {
         N[CORDER][K-1] = 0;/*always, but tested at [I+1]*/
         for(I=0; I<CORDER; I++)
         {
            if(N[I][K-1] < 0.00001) D = 0.0;
            else D= ((T-X[I])*N[I][K-1])/(X[I+K-1]- X[I]);
            if(N[I+1][K-1] < 0.00001) E = 0.0;
            else E=((X[I+K]-T)*N[I+1][K-1])/(X[I+K]-X[I+1]);
            N[I][K] = D+E;
         }
      }
      g[1] = 0.0;
      g[2] = 0.0;
      g[3] = 0.0;

      for(I=0; I<CORDER; I++)
      {
         g[1] = g[1] + fg[I][1]*N[I][CORDER];
         g[2] = g[2] + fg[I][2]*N[I][CORDER];
         g[3] = g[3] + fg[I][3]*N[I][CORDER];
      }
      /*  T point + midline==zeroth strand*/
      spine[tt][1] = g[1];
      spine[tt][2] = g[2];
      spine[tt][3] = g[3];
   }/*T && tt over Tbone interval: THE SPLINE CALCULATION*/
}
/*___splinecalc()_________________________________________________________}}}*/
/*}}}*/
