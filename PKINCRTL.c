/*23456789_123456789_123456789_123456789_123456789_123456789_123456789_1234567*/
/*                           PKINCRTL.c                     */
#include "PKIN.h"

#define   PKINCRTL
#include "PKINCRTL.h"
#undef    PKINCRTL
#include "PKINDLOG.h"
#include "PKINRIBB.h"

int  queercompare(const void * astr, const void * bstr); /*050925*/

/***flowControl()**********************************************************/
void flowControl()
{
  /*enters here for each pass, aka Lthispass, so any globals that must */
  /*presist from the first pass, must not be reset here without protection*/

  int    i,j,k,maxcounter,iskip,inputline,iscan,Lperiod,LOK=1;
  static int  Loutput,Lallrescycle,Numberresiduetypes,LLfpoutput;
  static char oldmaxatmnamechar;
  static char residuereport[3][MAXlevels]; /*up from 128 040324,040504*/
  static int realresiduereport[MAXlevels]; /*up from 128 040324,040504*/
  static int iresiduereport,iteststring;

  static long ncountticks, accumticks[11],OldTickCounts[11];
  char resname[4],secstrcstr[256];
  static int nlevel=0;
  long   previousconect=0; /*050409*/
  int    IOK = 0,nN=0,nP=0; /*061001,061006*/

#ifdef GFPchromophore  /*070801*/ /*Perhaps do this as Lspecialresidue ???? */
  int  LCA1=0,LCA2=0,LCA3=0,LNN1=0,LNN2=0,LNN3=0; /*070801*/
  int  LCC1=0,LCC2=0,LCC3=0,LCO1=0,LCO2=0,LCO3=0; /*070801*/
#endif

accumticks[0] = 0;
accumticks[1] = 0;
accumticks[2] = 0;
accumticks[3] = 0;
accumticks[4] = 0;
accumticks[5] = 0;
accumticks[6] = 0;
accumticks[7] = 0;
accumticks[8] = 0;
accumticks[9] = 0;
accumticks[10] = 0;
OldTickCounts[0] = 0;
OldTickCounts[1] = 0;
OldTickCounts[2] = 0;
OldTickCounts[3] = 0;
OldTickCounts[4] = 0;
OldTickCounts[5] = 0;
OldTickCounts[6] = 0;
OldTickCounts[7] = 0;
OldTickCounts[8] = 0;
OldTickCounts[9] = 0;
OldTickCounts[10] = 0;
OldTickCounts[0] = TickCount();

/*#define PRINTINFORMATION*/   /*080915*/
#ifdef PRINTINFORMATION
fprintf(stderr,"mrange== %d\n",mrange);
fprintf(stderr,"j:mcxl,scxl,hbxl,hyxl,caxl,htxl,mcCPKl,scCPKl,htCPKl,atxl,ctxl,lbxl,cssl,waxl,waal\n");

    for(j=1 ; j<=mrange ; j++)
    {/*check each range*/
fprintf(stderr,"%d: %d,   %d,   %d,   %d,   %d,    %d,     %d,     %d,    %d,   %d,   %d,   %d,   %d,   %d,   %d, resnu1[%d]== %d, resnu2[%d]== %d\n"
,j,mcxl[j],scxl[j],hbxl[j],hyxl[j],caxl[j],htxl[j]
,mcCPKl[j],scCPKl[j],htCPKl[j],atxl[j],ctxl[j],lbxl[j],cssl[j],waxl[j],waal[j]
,j,resnu1[j],j,resnu2[j]);
    }
fprintf(stderr,"FOVEA %d, xyz %.3f, %.3f, %.3f"
", focussc %.2f, focusmc %.2f, focusca %.2f, focuswa %.2f, focusht %.2f\n"
,FOVEA,focus[1],focus[2],focus[3],focussc,focusmc,focusca,focuswa,focusht);
#endif
/*#define PRINTRANGES*/
#ifdef PRINTRANGES
printf("code r=1  2  3  4  5  6  7  8  9\n");
printf("mcxl  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,mcxl[1],mcxl[2],mcxl[3],mcxl[4],mcxl[5],mcxl[6],mcxl[7],mcxl[8],mcxl[9]);
printf("scxl  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,scxl[1],scxl[2],scxl[3],scxl[4],scxl[5],scxl[6],scxl[7],scxl[8],scxl[9]);
printf("hbxl  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,hbxl[1],hbxl[2],hbxl[3],hbxl[4],hbxl[5],hbxl[6],hbxl[7],hbxl[8],hbxl[9]);
printf("hyxl  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,hyxl[1],hyxl[2],hyxl[3],hyxl[4],hyxl[5],hyxl[6],hyxl[7],hyxl[8],hyxl[9]);
printf("caxl  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,caxl[1],caxl[2],caxl[3],caxl[4],caxl[5],caxl[6],caxl[7],caxl[8],caxl[9]);
printf("htxl  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,htxl[1],htxl[2],htxl[3],htxl[4],htxl[5],htxl[6],htxl[7],htxl[8],htxl[9]);
printf("waxl  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,waxl[1],waxl[2],waxl[3],waxl[4],waxl[5],waxl[6],waxl[7],waxl[8],waxl[9]);
printf("atxl  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,atxl[1],atxl[2],atxl[3],atxl[4],atxl[5],atxl[6],atxl[7],atxl[8],atxl[9]);
printf("lbxl  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,lbxl[1],lbxl[2],lbxl[3],lbxl[4],lbxl[5],lbxl[6],lbxl[7],lbxl[8],lbxl[9]);
printf("ctxl  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,ctxl[1],ctxl[2],ctxl[3],ctxl[4],ctxl[5],ctxl[6],ctxl[7],ctxl[8],ctxl[9]);
printf("mcal  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,mcal[1],mcal[2],mcal[3],mcal[4],mcal[5],mcal[6],mcal[7],mcal[8],mcal[9]);
printf("scal  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,scal[1],scal[2],scal[3],scal[4],scal[5],scal[6],scal[7],scal[8],scal[9]);
printf("htal  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,htal[1],htal[2],htal[3],htal[4],htal[5],htal[6],htal[7],htal[8],htal[9]);
printf("waal  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,waal[1],waal[2],waal[3],waal[4],waal[5],waal[6],waal[7],waal[8],waal[9]);
#endif /*PRINTRANGES*/

  numrotsc = -999;
  iresiduereport = 0;
  for(j=0;j<256;j++){realresiduereport[j]=0;}
  iteststring = 0;
  Lperiod = 0; /*allows keyboard . interrupt*/
  countxyz = 0;  /* number of xyz written to .kin */
  oldmaxatmnamechar = 'a'; /*presume start with ATOM records*/
  /*--define constants: array sizes and distance checks */
  lastat = (MAXARR - 1);
  ssmax = (MAXARR - 1);
  hetmax = 1000;
  /*  --allowed connection distances for s-s, hetatom hookups, and mainchain*/
  ssdist = 2.5;

  bondst = 2.5;
  /*--mainchain allowed sloppy for poorly determined structures  */
  /*ca-ca distance allowance: */
  cadist = 5.0;

  iresiduecnt = 0; /* for special cycles through all aminoacids */

  /*------define starting values of control flags */
  /*-- endfil: signels end of input file */
  endfil = 0;
/*    -- endsub: signels end of a subunit and another to come */
  endsub = 0;
/*    -- endchn: signals break in continuity */
  endchn = 0;
/*    -- discon: set when current residue is disconnected from previous one */
  discon = 0;
/*    -- endres: signels end of a residue, implies continuity if possible */
  endres = 0;
/*    -- first one shot declaration of very first piece of data on infile */
  first1 = 1;
/*    -- level relates to putting different subunits in different groups */
  level = 0;
  nlevel = 0;
/*    -- icountres[level]  number of . showing number of residues processed */
  for(j=0;j<MAXlevels;j++)
  {
     icountres[j] = 0; /*number of . showing number of residues processed*/
     ChainID[j][0] = '\0'; /*2char 070926*/
     ChainID[j][1] = '\0'; /*2char 070926*/
     ChainID[j][2] = '\0'; /*2char 070926*/
  }
  if(Lthispass==1)
  {/*initialize residue counter, values will NOT be recreated in later passes*/
     for(j=0;j<MAXlevels;j++)
     {
        icountresfirstpass[j] = 0;
     }
     LtypeP = 0;  /*051201*/
     LtypeN = 0;
     LtypeH = 0;
     LtypeW = 0;
     LtypeU = 0;
  }
/*    -- irpass serves as the level indicator when level starts .gt. 0 */
  irpass = 0;
/*    -- 910131 ksave defines which character of subnam changed for multi- */
/*    -- subunit names, ksave=0 is illegal, so forces subroutine subals to */
/*    -- construct a unique name, at whatever level subals is first called!*/
  ksave = 0;
/*   -- levmsg if 0 when morsub .true. will print message about other levels*/
  levmsg = 0;
/*    -- morsub: signels that there may be multiple subunits, */
  morsub = 1;  /* set 0 when done last or only subunit*/
/*    -- Ngroups: counts Number of groups outputed this PASS or this MODEL */
  Ngroups = 0;
/*    -- Ltopout: flags when masters, etc. written at top of kinemage*/
  Ltopout = 0;
/*    -- Nmodels: 0 for regular complex, ==1 for 1st member of nmr ensemble*/
/*                unfortunately, rolls over at end of each MODEL before*/
/*                processing of earlier one because prekin always reads*/
/*                into next valid coordinate record as its fool-proof way*/
/*                of really knowing where boundarys are. */
/*                This is a pre-PDB uniformity sanity check.*/
  Nmodels = 0;
/*    -- Nresiduesin: counts number of residues in each chain as they come in*/
  Nresiduesin = 0;

  stonex = 0;
  /*stonex signels that the 'texts' buffer has stored a possible next atom */
  icount = 0;  /*icount is total of individual residue sidechains read out */
  delnum = 0;
  delres = 0;
  delnam = 0;
  delsub = 0;
  oldsub[0] = ' ';
  oldsub[1] = ' ';
  oldsub[2] = '\0';
  nexsub[0] = ' ';
  nexsub[1] = ' ';
  nexsub[2] = '\0';
  oldmod[0] = '\0';
  nexmod[0] = '\0';

  atomin = 0;
  /*atomin set by a valid atom/hetatm read, so if 0: still in headers */

  atpdbEOF = FALSE;
  Lfoundresnum = 0; /*for fovea by residue number */
  ihitend = 0; /*stores EOF info, used if EOF ends valid line*/
  ihitaalibend = 0;

  nmrmodel[0] = '\0'; /*reset NULL, set to char str of MODEL # */
     /*this has to be before PDB MODEL record encountered*/
  Nusermod = 0; /*020514 for pdbinfostuff in Lthispass==1 */

  Lpointmasteralta = 0; /*030503*/
  Lpointmasteraltb = 0;
  Lpointmasteraltc = 0;

  Lribidentity = 0; /*default, no colorsets defined yet  060929*/
  if(Lthispass==1)
  {/*initialize ribidentity's, values will NOT be recreated in later passes*/
     for(j=0; j<MAXR; j++) {ribidentity[j][0] = '\0';} /*060929*/
  }

  /*------------------------------------------------------------------------*/
  sprintf(temps,CRLF CRLF"...program running... this pass == %d",Lthispass);
  pkintextinsert(temps);  /*____TEXT.C*/
  adjusttext(0);          /* 0: force text liines onto screen ____TEXT.C*/
  /*------------------------------------------------------------------------*/

  inputflag = 0;
  while(inputflag !=1)/*eventually inputflag==1 when 1st ATOM record reached*/
  {/*read HEADER */
     inputline = Getoneinputline(); /*PKININPT*/
     if(atpdbEOF) inputflag = 1;
     else if(inputline == 1)  /* new input may be in buffer */
     {/*checkline for atom record, interpret HEADER records*/
        if(LpdbIO) /*040304*/
        {/*just doing straight dump of pdb header to output*/
         /*and transform of coords then dump of modified ATOM, HETATM records*/
           inputflag = pdbInOutprocessing(texts); /*PKININPT*/ /*LpdbIO 040304*/
        }
        else
        {/*checkline for doing pre-kinemage work*/
           inputflag = checkline(texts); /*PKINCSUB*/
        }
     }
  }
  /*evidentually, 1st ATOM record has been reached...*/
  Lnewpassflow = 0; /* now do work for this pass*/

  if(!Lin_stdin)  rewind(fpinput);

  /*HEADER stuff has been examined for this pass*/
  MAXaspectcode = Naspectcode; /*aspects inputted*/
  Naspectnc = 0;
  Naspectabc = 0;
  if(Laspectnc || Laspectabc)
  {/*add internal aspects to any HEADER inputted ones*/
     /*HEADER inputted Naspectcode aspects*/
     if(Laspectnc)
     {
        MAXaspectcode++;
        Naspectnc = MAXaspectcode;
        sprintf(aspectid[MAXaspectcode],"N-->C");
     }
     if(Laspectabc)
     {
        MAXaspectcode++;
        Naspectabc = MAXaspectcode;
        sprintf(aspectid[MAXaspectcode],"Sec Str");
     }
  }

  if(Lplotpperptobase){Lkinmasters = 1;ballradius=(float)0.05;}  /*030519*/

  if(LpdbIO) /*040304*/
  {
     Lallrescycle = 0; /*no further work for LpdbIO processing of pdb file*/
  }
  else
  {
     Lallrescycle = 1; /*need at least one cycle for pre-kinemage work*/
  }
  if(Lallaminoacids) Numberresiduetypes = 21;
      /*21 since 1st pass does all ca-ca */
  if(Lallbases) Numberresiduetypes = 6;
      /*(naba) 6 since 1st pass does backbone, 4 types, plus 1 misc category*/
OldTickCounts[1] = TickCount();

  while(Lallrescycle)
  {/*Lallrescycle*/
    /*each cycle rewinds input file and goes through everything*/

    sprintf(temps,CRLF); /*c*/ /*extra carriage return between cycles*/
    pkintextinsert(temps);  /*PKMCTEXT.C PKPCTEXT.C*/
    adjusttext(1);          /*PKMCTEXT.C PKPCTEXT.C*/

    level = 0;
    nlevel = 0;
    atomin = 0; /*040406*/
    /*atomin set by a valid atom/hetatm read, so if 0: still in headers */

    if(   (Lallaminoacids||Lallbases)
       && (iresiduecnt < Numberresiduetypes)
       && (Lthispass>1)                    )  /*040406 Lthispass criterion*/
    {
        /*find special residue name for this cycle*/
        getresiduename();/*augments iresiduecnt, 1 for backbone/pseudobackbone*/
        sprintf(temps,"now look for:  %s"CRLF
                 ,resid[1]);
        pkintextinsert(temps);  /*PKMCTEXT.C PKPCTEXT.C*/
        adjusttext(1);          /*PKMCTEXT.C PKPCTEXT.C*/

        Lallrescycle = 1;
    }
    else if(Lresnumfovea)
    {
        Lallrescycle = 1; /*need 2nd cycle to apply distance to found residue*/
    }
    else
    {
        Lallrescycle = 0; /*on last cycle of Lthispass*/
    }



OldTickCounts[2] = TickCount();

  /************************************************************/
  /*---loop all subunits; while input file has data: */
  /*** cycle to here after process each subunit */
  while( morsub == 1 && endfil == 0)  /*100 continue*/
  {/*within molecular coordinates: morsub == 1 && endfil == 0 */

    if(level < MAXlevels-1) /*040406*/
    {
       level = level + 1;
       nlevel++;
    } /*040406*/
    else
    {
       nlevel++;
       sprintf(temps,"level would be %d, stuff now into max level %d"CRLF,nlevel,level);  /*040504*/
       pkintextinsert(temps);
       adjusttext(1);
#ifdef UNIX_X11
       fprintf(stderr,"level would be %d, stuff now into max level %d"CRLF,nlevel,level);  /*040504*/
#endif
    }
    icountres[level] = 0;
    Nresiduesin = 0;
    /* this defines level == subunit or molecule number */

    if(Lthispass>1)
    {
       Loutput = TRUE;
       if(Lribbon && Lribbonnew) /*051025 new ribbon code*/
       {/*Lribbonnew*/
         if(typechainfirstpass[level]=='P'||typechainfirstpass[level]=='N')
         {/*could have ribbon for this level*/

          /*define a new section for this level */
          if(thisribmodelptr == NULL) /*probably 1st time through...*/
          {
             thisribmodelptr = allocribmodelstructure(nmrmodel); /* global*/
            /*nmrmodel safe for incoming atoms, but not for output of a model*/
/*
fprintf(stderr,"PKINCRTL level:%d, ribl[n==%d]==%d alloc thisribmodelptr == %ld\n",level,n,ribl[n],thisribmodelptr);
*/
          }
          /*global thisribmodelptr set here or by MODEL record in PKINCSUB*/
          /*NOTE: as of 051025 PKINCSUB DOES NOT alloc ribmodelstruct */
          if(thisribmodelptr != NULL)
          {
             trialribsectionptr = allocribsectionstructure(thisribmodelptr);
             if(trialribsectionptr != NULL)
             {
                Lnewribbonstart = 1; /*090219 clear previousribbontype*/
                thisribsectionptr = trialribsectionptr;
/*
fprintf(stderr,"PKINCRTL level:%d, ribl[%d]==%d  alloc thisribsectionptr == %ld\n",level,n,ribl[n],thisribsectionptr);
*/
                thisribsectionptr->type = typechainfirstpass[level];
                thisribsectionptr->kind = kindchainfirstpass[level]; /*060114*/
                thisribfragptr = allocribfragstructure(thisribsectionptr);
                /*first fragment of this section*/
/*
fprintf(stderr,"PKINCRTL level:%d, ribl[%d]==%d  alloc thisribfragptr == %ld\n",level,n,ribl[n],thisribfragptr);
*/
             }
          }
         }/*could have ribbon for this level*/
         if(thisribfragptr == NULL)
         {
            Lribbonnew = 0; /*cannot do newribbon code */
            /*all hetatms, or not enough Prot or Nuc to do ribbon 090219*/
#ifdef UNIX_X11
            fprintf(stderr,"CANNOT do a ribbon for this file"CRLF);/*090219*/
            if(Lribnhet) {Lribbon = 0;} /*FAIL for RCSB PDB ribbon command*/
               /*so failure will be evident in batch processing mode! */
#endif

         }
       }/*Lribbonnew*/
       if(Lbestribbon)  /*011030*/
       {
        /*BEWARE: PREKIN confusion between chain and coordinate block 050923*/
        /*e.g. subunits vs models and mixtures of the two */
          if(Lexplicitribbonpossible[level] || Lmodel) /*Lmodel 050923*/
          {/*Lmodel flag is stupid patch to get ribbon ranges for all models*/
           /* this may cause problems with nmr struct with big hetatm molecs */
             Lexplicitribbon = 1;
             explicitribbontype = 3; /*COIL*/
             explicitribbonidentity[0] = '\0'; /*default: no ID 060929*/
             Lonestrandcoil = 1;
             Lfudgedwidth = 0;
             Lbetaarrows = 1;
             Lautoribbon = 1;
             Ledgedribbon = 1;
             Lcrosstie = 0;
             Lskeinedribbon = 0;
             ribwidcoil = 1;/*explicit range ribbon with beta arrows:*/
                            /* before 050206 was coil==0*/
          }
          else
          {
             Lexplicitribbon = 0;
             Lonestrandcoil = 0;
             Lfudgedwidth = 1;
             Lbetaarrows = 0;
             Lautoribbon = 0;
             Ledgedribbon = 1;
             Lcrosstie = 0;
             Lskeinedribbon = 0;
             ribwidcoil = 1.0;
          }
       }
    }
    else /*Lthispass==1*/
    {
       if(Nmodels < 3)
       {/*first pass Nmodels==0 for multi-subunit complex*/
        /*  and Nmodels never incremented*/
        /*first pass Nmodels==1 for 1st nmr MODEL ensemble member*/
        /*  incremented at end of 1st nmr MODEL  when run into 2nd*/
        /*  but this is before prekin is allowed to process first member*/
          Nchainfirstpass = Nchainfirstpass + 1;
       }
       Loutput = FALSE;
    }
/*
sprintf(temps,CRLF"PKINCRTL Lthispass %d, Nmodels %d, Nchainfirstpass %d, ichainthispass %d"
          ,Lthispass,Nmodels,Nchainfirstpass,ichainthispass);
pkintextinsert(temps);
adjusttext(1);
*/

    if(gflag[0] == ' ' || gflag[0] == '0' ) /*gflag[0]==1 means do all chains*/
    {/* not necessarily doing all subunits*/
      /*chains done as marked in the order as recognized by PREKIN*/
      /*This is set in subunit choice dialog box of GUI*/
      /*Different control than commandline: -chain A, handled by rangecontrol*/
      if(gflag[level] == '_') /* simulate end of data */
      {
        morsub = 0;
        endfil = 1;
        /*---------------------------------------------------------------*/
        sprintf(temps,"simulate data end at subunit or molecule %d"CRLF,level);
        pkintextinsert(temps);  /*PKMCTEXT.C PKPCTEXT.C*/
        adjusttext(1);          /*PKMCTEXT.C PKPCTEXT.C*/
        /*---------------------------------------------------------------*/
        goto loopend;  /* with morsub and endfil set to break out of loop */
      }
      if(gflag[level] == ' ' || gflag[level] == '0' )
      /* not doing this subunit*/
      {
        Loutput = FALSE;
        /*---------------------------------------------------------------*/
  sprintf(temps,"scanning but not outputting subunit or molecule %d"CRLF,level);
        pkintextinsert(temps);  /*PKMCTEXT.C PKPCTEXT.C*/
        adjusttext(1);          /*PKMCTEXT.C PKPCTEXT.C*/
        /*---------------------------------------------------------------*/
      }
    }/* not necessarily doing all subunits*/
    /*--------------within a subunit----------------------------------------*/
    /*--while within a subunit, cycle this while loop */
    /*set conditions for beginning of each real subunit loop */

   setsubunitconditions();/*CSUB this resets most counters for virtual subunit*/
    Lfirstrecordinlevel = 1; /*lurks for that first atom record*/

/*--------------------WORKING ON CHAIN PRINT STATEMENT-----------------------*/
       sprintf(temps,CRLF"working on coordinates for coordinate block %d  %s  "
       ,level,nmrmodel);
       pkintextinsert(temps);  /*____TEXT.C*/
       adjusttext(0);          /* 0: force text lines onto screen ____TEXT.C*/
/*---------------------------------------------------------------------------*/


OldTickCounts[3] = TickCount();

    /****************** while(endsub == 0) ***********************/
    while(endsub == 0)  /* 130 continue */
    {  /* within a subunit: endsub == 0 */
      /*loop all residues/blocks of atoms of current subunit */

OldTickCounts[4] = TickCount();

      Latomnext = 0; /*030515*/
      endres = 0;
      discon = 0;
      /* discon...nected residue, default is 0, connected to previous */
      if (endchn == 1)
      /*this residue flagged as discontinuous with previous one */
      {
        discon = 1;
        endchn = 0;
      }
      iskip = 0;
      n = 0;        /*atom counter started afresh for this next residue */
      atarraymax = 0; /* limited number of atoms in a residue */
      resinloop = 0; /*separate loop control so can look ahead in input file*/
      /*read through until hit a valid atom/hetatm or EOF past each residue*/

      /*set the vector tail(==base) to each atom to be itself*/
      /*avoid past historical values for the atom base[] if not set currently*/
      for(j=0; j<= lastat-8; j++) {base[j] = j;}  /*061001*/

      /*logicals concerned with ca-ca only */
      catrue = 0; /*set catrue = TRUE when encounter a ca (or c4',or p 061001)*/
      if(LpdbCACOpdb) {caonly = 0;} /*doing Calphas--> all mainchain 030202*/
      else {caonly = 1;}
      /*set caonly = FALSE when encounter something besides a ca*/
      /*note: caoutl controls whether only ca will be connected in mc output*/
      while( (resinloop == 0) && (atarraymax == 0) )
      {/*residue input, while resinloop == 0  */
      /*--------within a residue-----------------------------------------*/

        OldTickCounts[7] = TickCount(); /*input record*/

        n = n + 1;  /* atoms of this residue counter  */

        /*lastat reserved for possible connection atom in previous residue */
        /* lastat-1,lastat-2,lastat-3 for possible alternate conf: a,b,c */
        /* lastat-4,lastat-5,lastat-6 lastat-7 for ca - ca connections */
        if(n == (lastat-8) )  atarraymax = 1;
        /*array size limit to match expected max residue/group size ; */

        nnow = n;  /*nnow keeps value of n for use after do loop finished */

        delnum = 0;  /* residue ends at residue number change */
          /*delnum flagged normally by evalatomrecord(), */
          /*non-PDB char in number flagged by readbrkatmrecord()*/

        delres = 0;  /* residue ends at residue name change BUT */
        /*011208 sequence heterogeneity sc res change but same num*/
        /*so delres NOT robust*/
        /* not really needed anyway if number or insertion indicator changes*/

        /*check if texts buffer holds the next atom, i.e. the to be n==1 atom */
        /*when starting a new residue. */

        if ( n > 1 || stonex == 0)
        /*if valid atom being held in texts buffer, don't do this read section*/
        {/*input a record into texts buffer*/
          /*here there is no stored atom in texts, */
          /*so have to beware of changes */
          /*in name of record or name of subunit that will affect processing*/
          /*read in record as character string into 'texts' buffer */
          /*this is the only place where the input file is read! */

          inputline = Getoneinputline(); /*PKININPT*/
          if(atpdbEOF)
          {/*EOF*/
              endfil = 1; /*end of the file*/
              morsub = 0; /*no more subunits expected*/
              endchn = 1;
              endres = 1;
              endsub = 1;
              /*note that endsub means multiple subunits, */
              /*which may not be true here */
              resinloop = 1; /* declare at end of a residue */
          }/*EOF*/

          if(inputline == 1)  /* new input may be in buffer */
          {/*checkline for atom record*/
            inputflag = checkline(texts); /*PKINCSUB*/
            if( inputflag != 1) /* return != 1 if not atom, not hetatm*/
            {/*check out line without coord info, allow upper or lower case*/
               evalnonatmrecord();  /*PKINCSUB*/
                 /*uses inputflag as returned from checkline*/
               iskip = 1;
               /*prekin historically just keeps reading until new residue*/
               /*at which time it closes and processes the previous residue*/
               /*Then, if has flags set, it goes on to process the chain*/
               /*or, of course, if it hits an EOF*/
            }
            else  /* checkline returned == 1 */
            {/* valid atom or hetatm record */
               atomin = 1;
               iskip = 0;

              /*for ease and consistency upper case converted to lower*/
              /*convertcase(texts);*/   /*PKINCSUB*/
              /*do this inside of readbrkatmrecord()*/

            }/* valid atom or hetatm record */
          }/*checkline for atom record*/
          else  /* Getoneinputline returned 0 */
          {/*no current line*/
            resinloop = 1; /*declare at end of a residue*/
            iskip = 1;  /* no coordinate line to process */
            n = n - 1;  /* back off the counter so could keep reading */
            /*with return of 1, evalnonatmrecord() does n=n-1 */
          }/*no current line*/
        }/*input a record into texts buffer*/

ncountticks = TickCount();
accumticks[7] = accumticks[7] + (ncountticks - OldTickCounts[7]);

OldTickCounts[8] =  ncountticks;   /*valid record*/

        if(resinloop == 0 && iskip == 0)
        {/* valid record */

          /*now do a formated reread of the line in texts */
          readbrkatmrecord();  /*PKINCSUB.c*/

          /*valid read now means inside a chain, */
          /*with this atom also held in texts */
          stonex = 1;  /*so stonex is indeed true */
          evalatomrecord();  /*PKINCSUB.c*/

          if(resinloop == 1) break; /* break from readinloop */

          /*within a residue or block of atoms, */
          /* PREKIN: treat hetatm sets as residues */
          /* waters are individual residues */
          /* Metals(?): hetatm residue with one atom that is not a water */
          /* is given a 3-D Unpickable cross as well as a center dot */

          /*ca logicals */ /*munged in honor of GFPchromophore 070801*/
          if(   atom[n][1] =='c'&&atom[n][2] =='a'
             &&(atom[n][3] ==' '||isdigit(atom[n][3]))) catrue = 1; /*070801*/
          else                                          caonly = 0;
          if(   atom[n][1] =='c'&&atom[n][2] =='4'
             &&(atom[n][3] =='*'||atom[n][3] =='\''))   catrue = 1;
          if(   atom[n][1] =='p'&&atom[n][2] ==' '
             &&(atom[n][3] ==' '||atom[n][3] =='\''))   catrue = 1; /*061001*/

        }/* valid record*/ /* n  is atom counter: */
        if (iskip == 1 && atpdbEOF == 0 )
        {
            resinloop = 0;
            /* skip useless record, e.g. HEADER */
        }
ncountticks = TickCount();
accumticks[8] = accumticks[8] + (ncountticks - OldTickCounts[8]);

      }/* end-of residue input, while resinloop ==0 controlled */ /*goto 132*/

ncountticks = TickCount();
accumticks[4] = accumticks[4] + (ncountticks - OldTickCounts[4]);
OldTickCounts[5] = ncountticks;
OldTickCounts[9] = ncountticks;

      /*--------------------------------------------------------------------*/
      if( (resinloop == 0) && (atarraymax == 1) )
      {
        /*150 continue*/
        /*only get in here if at limit of atoms/residue, */
        /*and not hit an end to the residue */
/*c*/      sprintf(alertstr," %s %s %d has at least %d atoms, the array limit"
                ,res[n],sub[n],num[n],n);
/*m*/      DoReportDialog();
/*c*/      sprintf(alertstr," processing will continue, "
                            "but results may be wierd");
/*m*/      DoReportDialog();
      }
      /*--------------------------------------------------------------------*/
      /*brought in current residue*/
      Nresiduesin++;
      /*maxatm keeps track of how many atoms belong in this residue */
      maxatm = nnow - 1;
      /*920512 let evalatomrecord() treat EOF at end of valid line as a */
      /* TER on a phantom next line */
      /*it is n-1 since reads one atom into next residue each time! */
      /*nnow holds the last value of n of the above loop, last atom inputed */
      /*or 1 past last atom if hit a 'ter', and */
      /* 'texts' holds last record as char string */

      /*NB 070801 if NOT at end of chain, evalatomrecord() stored nnow th */
      /* as atomnext, xnext, ynext, znext, etcnext... */

      finishres = 0;
      /* set flag to process residue, reset later when finish this residue */

      /*maxatm = 0 means passing through here with no stored residue so */
      /*   skip any attempt to connect atoms in the stored residue! */

      if(maxatm == 0) finishres = 1;

/*----------------------------------------------------------------------------*/
      residuetype = ' '; /*not even a guess */
      if(maxatm > 0) /*guess residuetype: P,N,H,W,U, , 061006*/
      {

#ifdef GFPchromophore  /*070801*/ /*Perhaps do this as Lspecialresidue ???? */
         LGFPchromophore = 0; /* unset the flag */
         Lunknownresidue = 0;
         Lunuseableresidue = 0;
#endif /*GFPchromophore 070801*/

         nP = 0;
         nN = 0;
         if(maxatm == 1)
         {
            if(isawater()) {residuetype = 'W';}
            else if(   atom[1][1] == 'o'  && atom[1][2] == '3'
                    &&(atom[1][3] == '*' ||  atom[1][3] == '\'') )
                 {residuetype = 'N';} /*lone previous nucl O3*  070512*/
            else {residuetype = 'H';}
         }
         else
         {
            if(strstr(aaList, res[maxatm]) )      {residuetype = 'P';}
            else if(strstr(naList, res[maxatm]) ) {residuetype = 'N';}
            else if(atom[maxatm][0] == 'a')
            {/*atom record but residue name not in our lists, try to guess*/
               for(j=1; j<=maxatm; j++)
               {/*look for a subset of Protein and Nucleic type atom names*/
                  if     (  atom[j][1] == 'c' && atom[j][2] == 'a') {nP++;}
                  else if(  atom[j][1] == 'c' && atom[j][2] == ' ') {nP++;}
                  else if(  atom[j][1] == 'n' && atom[j][2] == ' ') {nP++;}
                  else if(  atom[j][1] == 'p' && atom[j][2] == ' ')  {nN++;}
                  else if(  atom[j][1] == 'c' && atom[j][3] == '\'') {nN++;}
               }
               if     (nP > nN) {residuetype = 'P';}
               else if(nP < nN) {residuetype = 'N';}
               else             {residuetype = 'U';}
            }

#ifdef GFPchromophore  /*070801*/ /*Perhaps do this as Lspecialresidue ????*/
            else if(Lthispass > 1) /*guess if this hetatm a GFPchromophore*/
            {
               if(typechainfirstpass[level]=='P')
               {
                  LCA1=0;LCA2=0;LCA3=0;LNN1=0;LNN2=0;LNN3=0;
                  LCC1=0;LCC2=0;LCC3=0;LCO1=0;LCO2=0;LCO3=0; /*070801*/
                  for(j=1; j<=maxatm; j++)
                  {/*look for GFPchromophore type atom names*/
                    /*unfortunately, other hets, e.g. 2INZ/NAP match... */
                    /*so have to be very detailed*/
                     if(atom[j][1]=='c' && atom[j][2]=='a')
                     {
                        if(atom[j][3]=='1') {LCA1=1;}
                        if(atom[j][3]=='2') {LCA2=1;}
                        if(atom[j][3]=='3') {LCA3=1;}
                     }
                     if(atom[j][1]=='n' && atom[j][3]==' ')
                     {
                        if(atom[j][2]=='1') {LNN1=1;}
                        if(atom[j][2]=='2') {LNN2=1;}
                        if(atom[j][2]=='3') {LNN3=1;}
                     }
                     if(atom[j][1]=='c' && atom[j][3]==' ')
                     {
                        if(atom[j][2]=='1') {LCC1=1;}
                        if(atom[j][2]=='2') {LCC2=1;}
                        if(atom[j][2]=='3') {LCC3=1;}
                     }
                     if(atom[j][1]=='o' && atom[j][3]==' ')
                     {
                        /*if(atom[j][2]=='1') {LCO1=1;} missing==link*/
                        if(atom[j][2]=='2') {LCO2=1;}
                        if(atom[j][2]=='3') {LCO3=1;}
                     }
                  }
                  if(  LCA1&&LCA2&&LCA3&&LNN1&&LNN2&&LNN3
                     &&LCC1&&LCC2&&LCC3      &&LCO2&&LCO3)
                  { /*very likely to be GFPchromophore embedded in Protein*/
                    /*alternate atoms can increase number of hits */
                     LGFPchromophore = 1; /*070801*/
                     residuetype = 'H';/*otherwise not set, use for ribnhet*/
                  }
                  if(LGFPchromophore) /*reset the hetatm to be atom records*/
                  {
                     for(j=1; j<=maxatm; j++)
                     {
                        sprintf(name[j],"atom  ");
                     }
                  }
               }
            }
#endif /*GFPchromophore 070801*/

            else
            {/*hetatm record, punt...*/
               residuetype = 'H';
            }
         }
      }

      if(Nresiduesin == 1)
      {/*first residue of chain: invent chain name*/
        /*sub[]: blank or 1 char chain identifier and/or nmrmodel(#) <=5 char*/

        if(sub[maxatm][1] == ' '){ChainID[level][1] = 'A';}/*right justified*/
        else {ChainID[level][0] = sub[maxatm][0];   /*case sensitive pdb v3.0*/
              ChainID[level][1] = sub[maxatm][1];}  /*070926*/

        if(sub[maxatm][1] == ' ' && mod[maxatm][0]=='\0')
        {/*nothing to use, topID == null*/
           topID[0] = '\0';
           modelnumber[0] = '\0'; /*050121 model associated with this chain*/
        }
        else if(mod[maxatm][0]!='\0')
        {/*there is a model number available*/
         /*use: mod[maxatm] for the current residue */
         /*instead of: nmrmodel which changes when MODEL delinates end of res */

           /*strcpy(modelnumber,nmrmodel);*/  /*050121*/
           strcpy(modelnumber,mod[maxatm]);  /*051128*/
              /*050121 model associated with this chain == most recent MODEL*/
           if(sub[maxatm][1] != ' ')
           {/*non-blank chain identifier, topID == chainID & model# */
             topID[0] = sub[maxatm][0];
             topID[1] = '_'; /*spacer*/
             for(j=1; j<MAXmodchr; j++) /*start at 1 to*/
             {/*drop leading blank of mod, 050121 keep char 0,1 of topID*/
                topID[j+1] = mod[maxatm][j];/*top group indicator for output*/
             }
           }/*non-blank chain identifier*/
           else
           {/*no one letter chainID in sub[][], topID == the model number*/
             for(j=1; j<MAXmodchr; j++) /*start at 1 to*/
             {/*drop leading blank of mod*/
                topID[j-1] = mod[maxatm][j]; /*top group indicator for output*/
             }
           }/*no one letter chainID in sub[][], topID == the model number*/
        }/*there is a model number available*/
        else if(sub[maxatm][0] != ' ')
        {/*there is only a chain identifier, topID == chain identifier*/
           topID[0] = sub[maxatm][1]; /*top group indicator for output*/
           topID[1] = '\0';
           modelnumber[0] = '\0'; /*050121 model associated with this chain*/
        }/*there is only a chain identifier, topID == chain identifier*/

/*-----------ADD ON TO WORKING ON CHAIN PRINT STATEMENT-----------------------*/
          sprintf(temps," ,chain code|%s|%s| ",sub[maxatm],mod[maxatm]);
          pkintextinsert(temps);
          adjusttext(1);
/*----------------------------------------------------------------------------*/

      }/*first residue of chain: invent chain name*/

      if(Lthispass>1 && Loutput)
      {/*Loutput*/
       while(finishres == 0)
       {/*working on a residue: finishres == 0 */

        /*in a residue: see if we want it if we are in range control */

/*fprintf(stderr,"PKINCRTL level==%d calls   rangecontrol() riboutl==%d, %s %s %s\n",level,riboutl,atom[maxatm],res[maxatm],sub[maxatm]); */ /*KEEP STDERR*/

        rangecontrol(); /*PKINCSUB*/
/*
fprintf(stderr,"PKINCRTL level==%d return  rangecontrol() riboutl==%d\n",level,riboutl);
*/
        if(  (mutoutl != 0 || supoutl != 0)
           ||(  (Lcbetadev||Ltaudev)&&typechainfirstpass[level]=='P'
               && CompArgStr(name[maxatm],"atom",4)
             ) /*011029 check for P & atom*/
             /* NB maxatm last of current residue, n is first of next residue*/
          )
        {/*011029 check for P protein*/
           mutateresidue(); /*PKINCSUB*/
        }
        /* FOVEA adds to rangecontrols, sets rangel and other logicals */
        if(FOVEA) checkfocus(); /*PKINCSUB*/

        if( rangel == 0 ) finishres = 1;
        if(finishres == 1) break;  /*----skip this residue*/

        /*PREKIN uses distances within a residue for connectivity, */
        /* not library connections */
        /*continuing with this residue---*/
        /*keep track of encounters of all ribbon guide atoms in this residue*/
        o3snew = 0; /*040229 for nucleic ribbon extention*/
        c1snew = 0;
        c3snew = 0;
        nupnew = 0;
        canew = 0;
        cbnew = 0; /*041205*/
        onew = 0;

        Lalts = 0; /*flag for alt conf in this residue*/
        altstd = 0;  /*standard conformation flag*/
        /*reset alternate conformation flags */
        /* these are only used to connect back to a previous residue*/
        altcfa[0] = 0;
        altcfb[0] = 0;
        altcfc[0] = 0;
        altcfa[1] = 0;
        altcfb[1] = 0;
        altcfc[1] = 0;

           /* reset mc h-bond flags for this residue */
           tfn2 = FALSE;tfca2 = FALSE;tfc2 = FALSE;tfo2 = FALSE;tfnh2 = FALSE;

        /* if h-bonds not being done, carbonyl not stored for next either*/
        if( !hboutl) tfc1 = FALSE;

        /*do all original atoms, */
        /* don't do created ones for ring closure, etc, on second pass*/
        maxcounter = maxatm;
        for(natom=1 ; natom <= maxcounter ; natom++) idupl[natom] = natom;
            /*set each atom duplicated flag to the atom itself */
            /* as atoms are duplicated at the end of the list, their */
            /* idupl flag is set to the original atom number so sorting*/
            /* can know who each duplicated atom really is. */

ncountticks = TickCount();
accumticks[9] = accumticks[9] + (ncountticks - OldTickCounts[9]);

OldTickCounts[6] = ncountticks;

/*putative slow step**********************************************************/
/*
fprintf(stderr,"putative: mcoutl %d, scoutl %d, waoutl %d, (htoutl %d, %c),(ssscoutl %d, %c%c%c)\n"
,mcoutl,scoutl,waoutl,htoutl,name[maxatm][0]
,ssscoutl,res[maxatm][0],res[maxatm][1],res[maxatm][2]);
*/
      if(    mcoutl
         ||  scoutl
         ||  waoutl                                  /*971031*/
         || (htoutl   && (name[maxatm][0]=='h' || residuetype=='H')) /*070801*/
         || (ssscoutl &&
             res[maxatm][0]=='c'&&res[maxatm][1]=='y'&&res[maxatm][2]=='s') )

      {/*speed filter*/

        if(htoutl && name[maxatm][0]=='a' && residuetype=='H') /*070801*/
        { /*special case, eg GFPchromophore, converted to atom but still het*/
           mcoutl = 1;
           scoutl = 1;  /*070801 output residue as if it were still a het*/
        }

        for(iscan = 1; iscan <= 2 ; iscan++) /*double pass*/
        {/*make connections for this residue, */
          /* 2 passes helps some non-standard order problems */
/*
sprintf(alertstr,"make base connection for natom= %d"
CRLF"%s|%s|%s|%s|%s|%d|%s|%f %f %f",natom,name[natom],Anum[natom],atom[natom]
,res[natom],sub[natom],num[natom],rins[natom],x[natom],y[natom],z[natom]);
DoReportDialog();
*/
/*
printf("make base connection for natom= %d\n",natom);
printf("%s|%|%s|%s|%s|%d|%s|\n"
     ,name[natom],Anum[natom],atom[natom]  Anum hybrid36 071001
     ,res[natom],sub[natom],num[natom],rins[natom]);
printf(" %f %f %f\n" ,x[natom] ,y[natom] ,z[natom]);
*/
          for(natom=1 ; natom <= maxcounter ; natom++)
          {/*current atom: sequentially looked at in this residue */
            if(iscan == 1) {base[natom] = 0;}
            /* initially set as NO vector drawn to this atom */

            /*make atmcpy, so can rearrange and */
            /*play with atom name character str. */
            for(j=0 ; j<=5 ; j++) {atmcpy[j] = atom[natom][j];}

#ifdef OLDCODE
          /*070520 unclear that this is needed, would need work for new names*/
          /*since now using isahydrogen() */
            /*if atom is labeled as a deuterium, rename the copy to be an h:*/
            if(atmcpy[1] == 'd' ) {atmcpy[1] = 'h';}
            /*if atom is labeled as a NMR pseudo atom, */
            /*rename the copy to be an h:*/
            if(atmcpy[1] == 'q' ) {atmcpy[1] = 'h';}
#endif

            /*screen for alternate conformation letters at end of atoms: */
/*s*/       screenaltconf();

            /*residue-by-residue storage of mc h-bond calculating info */
/*H*/       if( hboutl )
            {
              if(iscan == 1)
              {
                if( name[natom][0] == 'a' )
                {
/*ss*/             if( decidemainside(natom) )
                   {
/*H*/                 getmchbatoms();
                      /* subroutine checks for atom type itself */
                   }
                }
              }
            }
            /*do hydrogens based on closest distance to heavy atom*/
            /*if(  (atmcpy[1] == 'h')*/
            /*||(atmcpy[0] == 'h' &&isdigit(atmcpy[2])&&isdigit(atmcpy[3]) ) )*/
            if(isahydrogen(name[natom],atmcpy,element[natom]) ) /*070520*/
            {/*connecthydrogen for h and h_nn cns hydrogen names  030920*/
               if(iscan == 1)
               {
                  /*connecthydrogen(); superseded 130318*/  /*by distance*/
                  connecthydrogenbyname(); /*supersede 130318*/
                    /*PKINCSUB*/ /*do this on first pass only*/
               }
            }
            else /*if(!caonly)*/ /*930123*/ /* not a hydrogen atom so: */
            {/* not a hydrogen atom so make connections */
                /*if(mcoutl || scoutl || htoutl)*/ /*to limit working time*/
                {/*this may be needed no matter what is specified*/

                    connectheavyatom(iscan,maxcounter); /*PKINCSUB*/

                }
                /* store cys cg for possible -ss- bond formation */
                if(iscan == 1) /*only need to do this once */
                {/*first scan of atom list, store cys cg for possible -ss-*/
                    if( (scoutl || ssscoutl)  &&
                       res[natom][0]=='c' &&  res[natom][1]=='y' &&
                       res[natom][2]=='s' && atom[natom][1]=='s' &&
                       atom[natom][2]=='g' )
                    {/*cys cg*/
    if(Laltcontrol!=0)
    {/*see if this atom meets alternate conformation specification(s)*/
       if(Laltcontrol>0)
       {/*inclusion*/
          LOK = 0; /*presume not to be included*/
          for(k=1;k<=Laltcontrol;k++)
          {/*check for inclusion*/
             if(atom[natom][4]==altcontrol[k]) LOK = 1;
          }
       }/*inclusion*/
       else /*Laltcontrol is negative*/
       {/*exclusion*/
          LOK = 1; /*presume to be included*/
          for(k=1;k<=(-Laltcontrol);k++)
          {/*check for exclusion*/
             if(atom[natom][4]==altcontrol[k]) LOK = 0;
          }
       }/*exclusion*/
    }/*see if this atom meets alternate conformation specification(s)*/
    if(LOK)
    {/*do this atom*/
       storecyssg(maxcounter); /*PKINCSUB*/
    }/*do this atom*/
                    }/*cys cg*/
                }/*first scan of atom list, store cys cg for possible -ss-*/
            }/* not a hydrogen atom so make connections */
          }/*current atom sequentially looked at in this residue */
        }/*make connections for this residue, 2 passes*/

        iteststring++;

      }/*speed filter*/

/*putative slow step_________________________________________________________*/
ncountticks = TickCount();
accumticks[6] = accumticks[6] + (ncountticks - OldTickCounts[6]);
OldTickCounts[10] = ncountticks;

if(Ltestprint > 1)
{/*Ltestprint*/
/*c*/   sprintf(temps,CRLF"---------------------------------------------"CRLF);
        pkintextinsert(temps);  /*PKMCTEXT.C PKPCTEXT.C*/
        adjusttext(1);          /*PKMCTEXT.C PKPCTEXT.C*/
  for(natom=1 ; natom <= maxatm ; natom++)
  {
        j = base[natom];
        sprintf(temps,"base for %s %s %d is: %s %s %d"CRLF
           ,atom[natom],res[natom],num[natom],atom[j],res[j],num[j]);
        pkintextinsert(temps);  /*PKMCTEXT.C PKPCTEXT.C*/
        adjusttext(1);          /*PKMCTEXT.C PKPCTEXT.C*/
  }
}/*Ltestprint*/

        /* check if "ca-ca" by choice or input for ATOM records*/
        if(name[maxatm][0]=='a' && ((caonly&&mcoutl) || caoutl || tabloutl))
        {   /*call even for first res of a chain to connect nucleic p-c4'-...*/
/*
fprintf(stderr,"icountres[%d]:%d: about to connectcaca, res[maxatm]%s, num[maxatm]%d, n==%d, endchn==%d, caonly:%d, mcoutl:%d, caoutl:%d, tabloutl:%d\n",level,icountres[level],res[maxatm],num[maxatm],n,endchn,caonly,mcoutl,caoutl,tabloutl);
*/
            connectcaca(); /*this makes a new entry in atom list for Ca-Ca*/
        }

        /*check for possible connections back to earlier het residues */
        if(name[maxatm][0] == 'h' && Lhetconnect && notawater() )  /*s*/
        {/*make new entry in atom list for any back connection*/
            connecthetatms(); /*PKINCSUB.c*/
        }
        /*now have vectors, or dots, for each atom */
        /*working on a residue: finishres == 0 */

        if(maxatm == 1 && base[1] == 1 && htoutl)
        /* lone hetatm, Make first character Cap, so calcium = Ca, not ca*/
        {
           if( notawater() ) atom[1][0] = lctouc(atom[1][0]);
        }
        /*test here to avoid errant mc atoms written to ribnhets ???? 090219*/
        if(Lribnhet){mcoutl=0;}   /*090219*/
        writescratch(maxcounter); /*PKINCSBS*/

        if(lboutl) writelabelscratch();

        if(atoutl || waaoutl) {writeatomscratch(maxcounter);}

        if(maxatm == 1 && base[1] == 1 && htoutl)
        /* dot had been made for lone atom which is likely a hetatm 070512*/
        {
           if(   notawater()
              && !(   atom[1][1] == 'o'  && atom[1][2] == '3' /*lone O3* nucl*/
                   &&(atom[1][3] == '*' ||  atom[1][3] == '\'') ) )
           {
              makemetalsymbol();
           }
             /*sets Listht==1 and writes atom to scratch*/
        }

        /*Put mc h-bond atoms onto hbond scratch file */
        /*try to calculate nh atom coord for mc h-bonds for this residue */
        if( hboutl)
        {
           storemchbatoms(); /*PKINHBND.c*/
        }
        finishres = 1;
        /*if expect continuity to next residue, then find and store */
        /*carbonyl carbon or ribose o3* of this current residue */
/*
fprintf(stderr,"icountres[%d]:%d: about to storeconnectingatom, n==%d, endchn==%d\n",level,icountres[level],n,endchn);
*/
        /*if(!endchn)  061202 always store putative connecting atom info*/
        /* hetatm in midst of atom records flags an endchn, */
        /* connectresidue dump should report last know atom */
        {
            IOK = storeconnectingatom(0,1); /*rtn int and 2nd int arg 061001*/
          /*and store ca incase doing ca-ca hookup, or p-c4*-p hookup*/
            IOK = storeconnectingatom(1,1); /*j==1,ipass==1*/
            if(!IOK) /*try ca---ca or p---p hookup 061001*/
            {
               IOK = storeconnectingatom(1,2); /*j==1,ipass=2*/
            }
        }
        if(Lribbon && name[maxatm][0] == 'a')/*riboutl*/
        {/*look for atoms in ATOM records that produce ribbon guide points */

          if(thisribfragptr != NULL) /*residue must be part of a fragment*/
          {
             LOK = loadribbonatoms(); /*works on current inputed residue*/
             /*090219 LOK from loadribbonatoms() does not seem to be used*/
             /* so neither a problem with atoms in the residue */
             /* - which is attempted to punt around in the ribbon code */
             /* nor a problem allocating storage, are rrecognized out here.*/
          }
        }/*ribbon stuff*/
       }/*working on a residue: finishres == 0 */
       if(Lpperptobase && (typechainfirstpass[level]=='N') )  /*030515*/
       {/*only works for nucleic acids*/
          pperptobase(maxcounter); /*PKINCSBS 030515*/
       }
      }/*Loutput*/
/*---------------------------------------------------------------------*/

      /*961117 modify to store residue count info and report at end*/
      /*declare residuereport[3][128], indexed by iresiduereport*/
      /*seems to drop through to here on first atom, presumably because*/
      /*atom is a change from no atoms before. In any case, this causes*/
      /* a count of 1 residue of type blank before gets into real residue*/
      /*counting.  So that extra entry on output is a deep artifact 990314*/
      icountres[level]++;
      if(name[maxatm][0]=='a') {realresiduereport[level]++;} /*011109*/
      if(icountres[level] == 1) oldmaxatmnamechar = name[maxatm][0]; /*990314*/
      else if(name[maxatm][0] != oldmaxatmnamechar)
      {
         oldmaxatmnamechar = name[maxatm][0];
         if(icountres[level]>1)
         {
             /*icountres[level] = 1;*/ /*NOT reset 051116*/
                 /*e.g. reset if change from ATOM to HETATM */
             /*sprintf(temps,CRLF); */
             /*pkintextinsert(temps);*/  /*PKMCTEXT.C PKPCTEXT.C*/
             /*adjusttext(1); */         /*PKMCTEXT.C PKPCTEXT.C*/
             /*if(iresiduereport < 2) iresiduereport++;*/ /*allows 3 kinds*/
             /*above fails if HETATM mes mixed in with ATOM residues*/
             if(name[maxatm][0]=='a') {iresiduereport=0;} /*011109*/
             else if(name[maxatm][0]=='h') {iresiduereport=1;}
             else {iresiduereport=2;}
             /*ATOM, HETATM, ?????*/
             /* 051116 seems that residuereport[iresiduereport] counta */
             /*actual atom or hetatm or other(?) residues --- */
             /*used to set number of residues for colornc breaks */
             /* seems that icountres[level] only used to keep track of where*/
             /*one is along the residues of level(==chain) -- so */
             /*icountres should not start over when encounter embedded het res*/
         }
      }
      /*if(icountres[level] == 1 && IMAC) getmactextselect(); */  /*990314*/
                                 /*PKMCTEXT.C PKPCTEXT.C*/
          /*macintosh TE variable used in pkintextreplace(temps)*/
                         /*PC dummies and ignores this */
      /**tally vote for type of chain: Nprotein, Nnucleic, Nwater, Nhetgroup */
      /**atom record: check name for nucleic acid, else default to protein */
      if(name[maxatm][0]=='a')/*should use AAList and NAList? maybe not 050409*/
      {  /*ensure upper case*/
         resname[0] = toupper(res[maxatm][0]);
         resname[1] = toupper(res[maxatm][1]);
         resname[2] = toupper(res[maxatm][2]);
         resname[3] = '\0';
         if( strstr(NAList, resname) )  /*List in UPPER CASE */
         {
            Nnucleic++;
            if(LRNA) {NRNA++;}
            else     {NDNA++;}
            /*reset LRNA below...*/
         }
         else
         {
            Nprotein++;
         }
      }
      else /**hetatm record: check for water, else default to general het */
      {
         if(  (res[maxatm][0]=='h'&&res[maxatm][1]=='o'&&res[maxatm][2]=='h')
            ||(res[maxatm][0]=='d'&&res[maxatm][1]=='o'&&res[maxatm][2]=='d')
            ||(res[maxatm][0]=='h'&&res[maxatm][1]=='2'&&res[maxatm][2]=='o')
            ||(res[maxatm][0]=='d'&&res[maxatm][1]=='2'&&res[maxatm][2]=='o')
            ||(res[maxatm][0]=='w'&&res[maxatm][1]=='a'&&res[maxatm][2]=='t')
            ||(res[maxatm][0]=='t'&&res[maxatm][1]=='i'&&res[maxatm][2]=='p')
            ||(res[maxatm][0]=='t'&&res[maxatm][1]=='p'&&res[maxatm][2]=='3')
           ) Nwater++;
         else Nhetgroup++;
      }
      LRNA = 0; /*bullet proof place to reset this flag for next residue*/

      sprintf(residuereport[iresiduereport]
      ,"%d residues of type %s",icountres[level],name[maxatm]);
                  /* for each residue processed */
      if(icountres[level]==1 && (Ltestprint || Lthispass>1) )
      {
         sprintf(temps,CRLF);
         pkintextinsert(temps); /*resets current position for textreplace*/
         adjusttext(1);
      }
    if(Ltestprint)
    {sprintf(temps,CRLF"%d residues of type %s",icountres[level],name[maxatm]);}
    else if(Lthispass==1)
    {
         sprintf(temps,"first pass: %d residues of type %s, in seqment %d"
            ,icountres[level],name[maxatm],iresiduereport);
         /*typechainfirstpass[level] = name[maxatm][0];NOT DONE HERE 040406*/

         /*only honor chainID for ATOM records ... vote on this ????070926*/
         /* or just honor last ATOM record or first different than '  ' ??*/
         if(strcmp(name[maxatm],"atom  ")==0) /*last atom record sets chainID*/
         {
           codechainfirstpass[level][0] =sub[maxatm][0];/*2char chainID 070926*/
           codechainfirstpass[level][1] =sub[maxatm][1];/*2char chainID 070926*/
         }
         /*realresiduereport[level] counts just ATOM records 011109*/
         /*which misses things like HETATM mes records, but*/
         /*is mostly the set for which a ribbon could be made*/
         icountresfirstpass[level] = realresiduereport[level];
         /*used for ribbon and other N-->C coloration*/
    }
    else
    {
         sprintf(temps,"%d residues of type %s",icountres[level],name[maxatm]);
    }
      if(Ltestprint)        pkintextinsert(temps); /*____TEXT*/
/*020514 don't really need to see this stuff flash by for every pass*/
    /*  else if(Lthispass>1)  pkintextreplace(temps); */  /*____TEXT.C*/

/*------------------------------------------------------------------------*/
      Lperiod = 0;
      if( getkeydownstate() ) /*PKMCMAIN.C PKPCMAIN.C*/
      {
          Lperiod = 1;
          sprintf(temps,CRLF"--cancel this pass--"CRLF);
          pkintextinsert(temps);  /*PKMCTEXT.C PKPCTEXT.C*/
          adjusttext(2);          /*PKMCTEXT.C PKPCTEXT.C*/
      }
      if(Lperiod) goto loopend; /*bailout if keyboard interrupt*/

/*------------------------------------------------------------------------*/


ncountticks = TickCount();
accumticks[5] = accumticks[5] + (ncountticks - OldTickCounts[5]);
accumticks[10] = accumticks[10] + (ncountticks - OldTickCounts[10]);

      previousresiduetype = residuetype;  /*P,N,H,W,U, , 061006*/
      /*loop back for another residue */
      /*if not the end of a subunit or end of file */

    }/* within a subunit: endsub == 0 */
    /*if ( .not.endfil .and. .not.endsub) goto 130 */
    /*_________________ while(endsub == 0) ______________________*/

ncountticks = TickCount();
accumticks[3] = accumticks[3] + (ncountticks - OldTickCounts[3]);
#ifdef RESIDUEREPORT /*not needed ???? now that do interrupt to old flow cnt*/
   if(iresiduereport>0)
   {/*more than one kind of residue in this subunit/molecule*/
     for(j=0;j<=iresiduereport;j++)
     {

         sprintf(temps,"%s"CRLF,residuereport[j]);
         /*need carriage return after atom count*/
         pkintextinsert(temps);  /*PKMCTEXT.C PKPCTEXT.C*/
         adjusttext(1);          /*PKMCTEXT.C PKPCTEXT.C*/
     }
   }/* one taken care of by overwritten accummulative report*/
   iresiduereport = 0;
#endif /*RESIDUEREPORT*/

#ifdef REPORTSLOWSTEP
sprintf(temps,"entered putative slow step %d times"CRLF,iteststring);
pkintextinsert(temps);  /*PKMCTEXT.C PKPCTEXT.C*/
adjusttext(1);          /*PKMCTEXT.C PKPCTEXT.C*/
#endif
iteststring = 0;


    /*check for possible -ss- bonds and write them to scratch file */
    if(Lthispass>1 && Loutput && ssknt > 1)
    {
       sprintf(temps,CRLF" scanning for -SS- bonds"CRLF);
       pkintextinsert(temps);  /*____TEXT*/
       adjusttext(1);          /*____TEXT*/

       connectssbonds();

    }
    /*check for possible h-bonds and write them to scratch file */
    if(Lthispass>1 && Loutput && cocnt != 0 && nhcnt != 0)
    {
/*c*/  sprintf(temps," scanning for possible mc h-bonds"CRLF);
       pkintextinsert(temps);  /*PKMCTEXT.C PKPCTEXT.C*/
       adjusttext(1);          /*PKMCTEXT.C PKPCTEXT.C*/

/*H*/    connecthbonds();
    }
    /*check for possible ribbon and write it to scratch */

    if(Lthispass>1 && Loutput && riboutl && Lribbon && Lribbonnew) /*081120*/
    {
       if(typechainfirstpass[level]=='P'||typechainfirstpass[level]=='N')
       {/*could have ribbon for this level*/
          /*construct ribbon section by section as these are inputed*/
          /*kinemage output per chain then stays in sync with ribbons */
          /*PKINRIBB 051214 also rebuildribbonguides(), etc.*/
          buildsectionguides(thisribsectionptr);
/*
fprintf(stderr,"PKINCRTL level==%d about to call expandsectionguides, riboutl==%d\n",level,riboutl);
*/
          expandsectionguides(thisribsectionptr);
          calculatesectionsplines(thisribsectionptr);
          constructsectionribbons(thisribsectionptr);  /*PKINRIBB*/
          if(Lribbonhbonds){makesectionpseudoHbonds(thisribsectionptr);}

       }/*could have ribbon for this level*/
    }

    if(Lthispass==1 && iresiduecnt<=1) /*040406*/
    {/*Lthispass==1: just do this chain type assignment once*/ /*040406*/
#ifdef OLDCODE
      if(Nnucleic > Nprotein+Nwater+Nhetgroup) typechainfirstpass[level] = 'N';
      else if(Nprotein>Nnucleic+Nwater+Nhetgroup) typechainfirstpass[level]='P';
      else if(Nwater>0 && Nhetgroup==0) typechainfirstpass[level] = 'W';
      else if(Nhetgroup > 0 ) typechainfirstpass[level] = 'H';
      else typechainfirstpass[level] = 'U';
#endif /*OLDCODE*/
      /*protein and nucleic completely dominant over hets and water 011105*/
      /*and hets dominant over water*/
      /*051201 logicals to show existence of molec types in the file: */
      /*070801 unfortunately, this does not quite do the trick for case where*/
      /* e.g. protein chain A is followed by het chain A */
      /* even though TER separates, still stays the same level in prekin */

       if     (Nprotein>0 &&Nprotein>=Nnucleic)
       {
          typechainfirstpass[level]='P';
          LtypeP = 1;
       }
       else if(Nnucleic>0 &&Nnucleic> Nprotein)
       {
          typechainfirstpass[level]='N';
          LtypeN = 1;
          if(NDNA > 0 || NRNA > 0)
          {
             if(NRNA > NDNA) {kindchainfirstpass[level]='R';}
             else            {kindchainfirstpass[level]='D';}
             /*otherwise a blank space character*/
          }
       }
       else if(Nhetgroup > 0)
       {
          typechainfirstpass[level]='H';
          LtypeH = 1;
       }
       else if(Nwater > 0)
       {
          typechainfirstpass[level]='W';
          LtypeW = 1;
       }
       else
       {
          typechainfirstpass[level]='U';
          LtypeU = 1; /*for stupid completeness */
       }
       if(Nhetgroup > 0) {LtypeH = 1;} /*hets   sometimes part of a chain*/
       if(Nwater > 0)    {LtypeW = 1;} /*waters sometimes part of a chain*/
          /*water and hets masters should then be declared at beginning */

       word[0] = '\0';
       if(Nnucleic > 0) sprintf(word,"%d Nucleic Acid",Nnucleic);
       if(Nprotein > 0) sprintf(word,"%s, %d Protein",word,Nprotein);
       if(Nhetgroup > 0) sprintf(word,"%s, %d Het Group",word,Nhetgroup);
       if(Nwater > 0) sprintf(word,"%s, %d Water",word,Nwater);
       sprintf(temps,CRLF"processed molecule or subunit %d, %s"
                 ,level,word);

       /*sprintf(temps,CRLF"processed molecule or subunit %d"CRLF,level);*/

       pkintextinsert(temps);
       adjusttext(1);
    }/*Lthispass==1: just do this chain type assignment once*/ /*040406*/

    if(level == 1 && npass == 1 && newkinemage)
    {   /* needs this even if not doing output for this level */
         newkinemage = 0;  /*resets newkinemage */
    }

    if(Loutput && !Lresnumfovea && Lthispass>1)
    {/*output a section of molecule, e.g. chain, subunit, model...*/
/*#define PRINTOUTL*/
#ifdef PRINTOUTL
sprintf(temps,"rangel %d, mcoutl %d, scoutl %d, lboutl %d, croutl %d, htoutl %d, hyoutl %d, hboutl %d, caoutl %d, waoutl %d, atoutl %d, mcaoutl %d, htaoutl %d, scaoutl %d, waaoutl %d, ssscoutl %d, rotoutl %d, mutoutl %d, riboutl %d, extoutl %d, mccoutl %d, sccoutl %d, htcoutl %d, mcCPKoutl %d,scCPKoutl %d,htCPKoutl %d, Bcoloutl %d, Bvaloutl %d\n"
   ,rangel, mcoutl, scoutl, lboutl, croutl,
    htoutl, hyoutl, hboutl, caoutl, waoutl,
    atoutl, mcaoutl, htaoutl, scaoutl, waaoutl,
    ssscoutl, rotoutl, mutoutl, riboutl, extoutl,
    mccoutl, sccoutl, htcoutl,
    mcCPKoutl,scCPKoutl,htCPKoutl,
    Bcoloutl, Bvaloutl);
pkintextinsert(temps);
adjusttext(1);
#endif /*PRINTOUTL*/
         if(Lsingleout) /*040318*/
         {/*no output until all chains processed*/
            LLfpoutput = 0;
         }
         else
         {/*output for each chain...*/
            LLfpoutput = 1; /*local flag to handle write loop*/
         }
         while(LLfpoutput>0)
         {
             sprintf(temps,"   writing output..."CRLF);
             pkintextinsert(temps);  /*PKMCTEXT.C PKPCTEXT.C*/
             adjusttext(1);          /*PKMCTEXT.C PKPCTEXT.C*/
             writeoutput(); /*PKINCOUT*/

             Lcommanded = 0; /*020514*/
             if(Lfpoutput<0)
             {
                 sprintf(alertstr,"Prekin writing to %s has failed! "
                              "Try a new file",OutfileStr);
                 DoReportDialog();
                 LLfpoutput = 1;
             }
             else LLfpoutput = 0;
         }
    }/*output a section of molecule, e.g. chain, subunit, model...*/


    loopend:  ;

    disposetextblock(&mainscratch);
    disposetextblock(&hbnscratch);
    disposetextblock(&hboscratch);
    if(Lhetconnect)
    {
        disposetextblock(&hetscratch);
    }

        if(Lperiod) goto newpassplace; /*bailout if keyboard interrupt*/
        if(Lfpoutput<0) goto newpassplace; /*bailout if output file fails*/

  }/*within molecular coordinates: morsub == 1 && endfil == 0 */
/*-----------------------------END OF PASS------------------------------------*/

/*----------- END-OF-FIRST-PASS REPORT PRINT STATEMENT -----------------------*/

  if(Lthispass==1)
  {/*first pass report*/
     Nproteinchains=0; Nnucleicchains=0; /*041031*/
     for(j=1; j<=Nchainfirstpass; j++)
     {/*check through count of all chains recognized by prekin*/
        if     (typechainfirstpass[j]=='N')
        {
           sprintf(word,"Nucleic Acid");
           Nnucleicchains++;
        }
        else if(typechainfirstpass[j]=='P')
        {
           sprintf(word,"Protein");
           Nproteinchains++;
        }
        else if(typechainfirstpass[j]=='W') {sprintf(word,"Water");}
        else if(typechainfirstpass[j]=='H') {sprintf(word,"Het Group");}
        else sprintf(word,"unknown");

        /*BEWARE: PREKIN confusion between chain and coordinate block 050923*/
        /*e.g. subunits vs models and mixtures of the two */
        Lexplicitribbonpossible[j] = 0; /*initialize:*/
        secstrcstr[0] = '\0';
        if(typechainfirstpass[j]=='P'||typechainfirstpass[j]=='N')
        {/*040228 nucleic can have SHEET ranges*/
         /*protein chain can have pdb HELIX,SHEET header specifications*/
           /*for(k=1; k<MAXC; k++)*/
           for(k=1; k<Nribs; k++) /*look through all rib ranges and */
           {/*check all possible matches of HELIX,SHEET chain records*/

              if(   codechainfirstpass[j][0]==ribchain[k][0]/*2charchnID070926*/
                 && codechainfirstpass[j][1]==ribchain[k][1]/*2charchnID070926*/
                 && (ribAlpha[k] || ribBeta[k]) )
              {
                 Lexplicitribbonpossible[j] = 1;
                 sprintf(secstrcstr," with explicit ribbons possible");
                 break; /*only need one match to set condition*/
              }
           }
        }
        else
        {
           Lexplicitribbonpossible[j] = 0;
           secstrcstr[0] = '\0';
        }

        sprintf(temps,CRLF"chain #%d, ID: %s of type %s%s: %d \"residues\""
                   ,j,codechainfirstpass[j],word,secstrcstr,icountres[j]);
        pkintextinsert(temps);
        adjusttext(1);
        /*if(LIOmage) putonetextblockline(&pdbinfostuff,texts);*/ /*020514*/
        if(LIOmage) {putonetextblockline(&pdbinfostuff,temps);}

        if(Lbrowsedefault) {putonetextblockline(&headerstuff,temps);}

#ifdef UNIX_X11
        if(Lstdoutreport)
        {
           fprintf(stdout,"chain #%d, ID: %s of type %s%s: %d \"residues\"\n"
                   ,j,codechainfirstpass[j],word,secstrcstr,icountres[j]);
        }
#endif
     }/*check through count of all chains recognized by prekin*/
     if(Nproteinchains>0 && Nnucleicchains==0)
        {sprintf(chainreport,"%d protein chains",Nproteinchains);}
     else if(Nnucleicchains>0 && Nproteinchains==0)
        {sprintf(chainreport,"%d nucleic acid chains",Nnucleicchains);}
     else if(Nproteinchains>0 && Nnucleicchains>0) /*041031*/
     {
        sprintf(chainreport,"%d protein chains, %d nucleic acid chains"
           ,Nproteinchains,Nnucleicchains);
     }
     else {chainreport[0] = '\0';}

  }/*first pass report*/
/*----------------------------------------------------------------------------*/
ncountticks = TickCount();
accumticks[2] = accumticks[2] + (ncountticks - OldTickCounts[2]);


     if(!Lin_stdin) rewind(fpinput);
     if( (Lallaminoacids||Lallbases) && iresiduecnt < Numberresiduetypes )
     {
         morsub = 1;
         endfil = 0;
         atpdbEOF = 0;
         ihitend = 0;
         endsub = 0;
         endchn = 1;
         stonex = 0;
     }
     if( Lresnumfovea && Lthispass>1)
     {
       if(Lfoundresnum)
       {
         morsub = 1;
         endfil = 0;
         atpdbEOF = 0;
         ihitend = 0;
         endsub = 0;
         endchn = 1;
         stonex = 0;
         /*Lresnumfovea = 0;*/ /*go back to PKINRNGE.C to get focus distances*/
         Lresnumfovea = 0; /* PKINRNGE.C originally got focus distances 951208*/
         /*FOVEA stays set, so will check distance from found residue coords*/
       }
       else
       {
           sprintf(alertstr
             ,"never found residue %d to act as focus\n",resnumfovea);
           DoReportDialog();
           Lresnumfovea = 0;
           FOVEA = 0;
           Lallrescycle = 0; /*break from cycle*/
           Lerr = 1;
       }
     }
     else if (Lresnumfovea && Lthispass==1 && Lquiet)
     {
        Lallrescycle = 0; /*break from this cycle, do another pass*/
     }
 }/*Lallrescycle*/

ncountticks = TickCount();
accumticks[1] = accumticks[1] + (ncountticks - OldTickCounts[1]);



/* if(Lfoundresnum) return;*/ /*go back to PKINRNGE.C to get focus distances*/
/*951208 try to do this as 2 cycles, focus distance specified at beginning*/
      if(Lthispass>1)
      {/*report about output*/
         sprintf(temps,CRLF"total count of xyz triples = %d "CRLF,countxyz);
         pkintextinsert(temps);  /*PKMCTEXT.C PKPCTEXT.C*/
         adjusttext(1);          /*PKMCTEXT.C PKPCTEXT.C*/
      }
      newpassplace: ;

      Lnewpass = 0;
      Lpassposs = 1;
      Lgetfile = 0;  /*let a new file be selected*/
                     /*must reset to 1 if continue with new pass on same file*/
      Lgot_infilename = 0; /*ready for new set of files*/
      Lgot_outfilename = 0;
      Lgot_infileopen = 0; /*ready for new set of files*/
      Lgot_outfileopen = 0;

      if(Lquiet>0 && (Lthispass>1 || Lstdoutreport || LpdbIO) )  /*040304*/
      {/*commandline preformance, quit so shell script can proceed*/
         if(Lsingleout) /*040318*/
         {/*single output, e.g. for cbdevdumpmax value*/
            writeoutput(); /*PKINCOUT*/
         }
         if(fpinput!=0 && fpinput != stdin)
         {
             fclose(fpinput);
         }
         if(fpoutput!=0 && fpoutput != stdout)
         {
             fclose(fpoutput);
         }
         if(Lcanlaunchmage && Lbrowsedefault)
         {
             launchcurrentkinemage(); /*launches, then calls exit*/
         }
         else exit(0);
      }
      else if(Lthispass>1 && (Lappleevents || Lcanlaunchmage) )
      {
         if(Lbrowsedefault)
         {/*automatic MAGE launch*/
             if(fpinput!=0)
             {
                 fclose(fpinput);
             }
             if(fpoutput!=0)
             {
                 fclose(fpoutput);
             }
             launchcurrentkinemage(); /*launches, then calls exit*/
         }
         else
         {/*MAGE launch optional*/
             /*now have a current kinemage that could be launched*/
             Lmadekin = 1;
             sprintf(temps,
             CRLF"After reading these messages, go to the File menu and either"
             CRLF" select Quit, or New Pass, or Open a new file, "
             CRLF" or else Launch the kinemage as a Mage display.");
         }
      }
      else
      {
         Lmadekin = 0;  /*need appleevents to launch*/
/*c*/    sprintf(temps,
            CRLF"After reading these messages, go to the File menu and either"
            CRLF" select Quit, or New Pass, or Open a new file. ");
      }

       pkintextinsert(temps);  /*____TEXT.c*/
       adjusttext(0);          /* 0: force text lines onto screen ____TEXT.c*/
       AdjustMenus();          /*____MENU.c*/



accumticks[0] = TickCount() - OldTickCounts[0];
#ifdef TIMINGRUN
sprintf(temps,CRLF"TotalTicks %ld inc 1stTicks %ld inc 2ndTicks %ld"
" inc 3rdTicks %ld == (4thTicks %ld + 5thTicks %ld)"
,accumticks[0],accumticks[1],accumticks[2],accumticks[3]
,accumticks[4],accumticks[5]);

       pkintextinsert(temps);  /*____TEXT.c*/
       adjusttext(1);          /*____TEXT.c*/
sprintf(temps,CRLF"Total 4 Ticks %ld == "
"(7thTicks %ld input record + 8thTicks %ld valid record)"
,accumticks[4],accumticks[7],accumticks[8]);

       pkintextinsert(temps);  /*____TEXT.c*/
       adjusttext(1);          /*____TEXT.c*/
sprintf(temps,CRLF" 5 Ticks %ld ==( 9th %ld + slow 6th %ld + 10th %ld)"
,accumticks[5],accumticks[9],accumticks[6],accumticks[10]);

       pkintextinsert(temps);  /*____TEXT.c*/
       adjusttext(1);          /*____TEXT.c*/
#endif

       cleanuptextwindow();  /*____TEXT.c*/

/*fprintf(stderr,"Lthispass == %d, ncon== %d\n",Lthispass,ncon);*/
   if(Lthispass==1) /*991210*/
   {
       if(Lconect && ncon > 0)
       {/*CONECT atom pairs were identified*/
          /*sort the atom pairs...*/
          /*qsort(iconect, ncon, sizeof(long), strcmp );*/
          qsort(iconect, ncon, sizeof(long), queercompare );
/* void qsort( void *buf, size_t num, size_t size, int (*compare)(const void *, const void *) );*/
/**warning: passing arg 4 of `qsort' makes pointer from integer without a cast*/
/*
for(j=0; j<=ncon; j++)
fprintf(stderr,"CONECT %3d: %5ld  %5ld\n",j,iconect[j]/100000,iconect[j] - ( 100000*(iconect[j]/100000) ) );
*/
          /*remove duplicates...*/
          previousconect = iconect[0];
          for(j=1; j<=ncon; j++)
          {
             if(iconect[j] == previousconect) {iconect[j] = 0;}
             else {previousconect = iconect[j];}
          }
/*
for(j=0; j<=ncon; j++)
fprintf(stderr,"CONECT %3d: %5ld  %5ld\n",j,iconect[j]/100000,iconect[j] - ( 100000*(iconect[j]/100000) ) );
*/
          /*remove zeros, close up the array...*/
          k = 0;
          for(j=0; j<=ncon; j++)
          {/*j loop*/
             if(iconect[j] == 0)
             {
                /*look ahead for the next non-zero entry*/
                for(i=j+1;i<=ncon;i++)
                {/*i loop*/
                   if(iconect[i] != 0)
                   {
                      iconect[j] = iconect[i]; /*copy back to earlier position*/
                      k = j; /*non-zero entry at j, so far the last atom pair*/
                      iconect[i] = 0; /*zero this position*/
                      break; /*out of i loop, so as to revert to j loop*/
                   }
                }/*i loop*/
             }
             else
             {/*non-zero entry at j, so far the last atom pair...*/
                k = j; /*may never get to here if there are any zero entries*/
             }
          }/*j loop*/
          ncon = k;
/*
for(j=0; j<=ncon; j++)
fprintf(stderr,"CONECT %3d: %5ld  %5ld\n",j,iconect[j]/100000,iconect[j] - ( 100000*(iconect[j]/100000) ) );
*/
       }/*CONECT atom pairs were identified*/
       if(Lquiet)
       {
           if(Lresnumfovea)
           {
              Lnewpass = 1;
           }
           else
           {
              Lnewpass=0;
              Lgetranges=1;
           }
       }
       else
       {
           Lnewpass = 1;
       }
   }
   Lthispass++;
}
/*___flowControl()__________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****queercompare()***********************************************************/
int  queercompare(const void * astr, const void * bstr)
{
   int ifoo = 0;

   ifoo = (int)strcmp((const char *)astr, (const char *)bstr);

   return(ifoo);
}
/*___queercompare()__________________________________________________________*/


