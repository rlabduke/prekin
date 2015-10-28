/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*                       PKINCSUB.c                */
/*****************************************************************/
/* NOTICE: This is free software and the source code is freely   */
/* available. You are free to redistribute or modify under the   */
/* conditions that (1) this notice is not removed or modified    */
/* in any way and (2) any modified versions of the program are   */
/* also available for free.                                      */
/*               ** Absolutely no Warranty **                    */
/* Copyright (C) 2000 David C. Richardson                        */
/*****************************************************************/
#include "PKIN.h"
#include "PKINDLOG.h"
#include "PKINCRTL.h"
#include "PKINRIBB.h" /*090219*/

static double anglerot1=0,anglerot2=0,cbcbaxis[3]={0,0,0},cacbcalc[3]={0,0,0};
static double caxyz[3]={0,0,0};
/*to superimpose PDB records*/

int NA_BaseCategory(char[4]);
int threepointimpose(float[3],float[3],float[3],char);
int imposeontoideal(float[3]);
int midanglepoint(float[3],float[3],float[3],float[3]);

/****NA_BaseCategory()********************************************************/
int NA_BaseCategory(char resname[4]) /*resname expected to be uppercase*/
{/* hacked from Mike's naBaseCategory()*/
   ResidueAndAtomPair *pair;

   for (pair = NAbaseGrouping; pair->rlist; pair++)
   {
      /*char *strstr(cs,ct) return pointer to first occurrence of */
      /*string ct in cs, or NULL if not present*/
      if(strstr(pair->rlist, resname))
      {
         return pair->bits; /* bits contain the atom type */
      }
   }
   return baseOther; /* not in table */
}
/*___NA_BaseCategory()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****setsubunitconditions()**************************************************/
void   setsubunitconditions(void)
{
  int    j;

    /*--set conditions for beginning of each subunit loop */

    first1 = 1; endsub = 0; endchn = 0; endres = 0;

    /*note that stonex may = 1 : valid atom still held in brk buffer (texts)*/
    /*but oldatm stored from last residue is meaningless at subunit start*/
    /*oldatm signels a stored possible atom to connect to next residue */
    /*and 3 possible alternate conformations, mainchain and ca-ca */
    /* j == 0 for mainchain,  j == 1 for ca-ca connection and virtual nucl bb*/

    for(j=0 ; j<=1 ; j++)
    {
      oldatm[j] = 0; oldata[j] = 0; oldatb[j] = 0; oldatc[j] = 0;
    }
    /*--logicals for hbond: all set false, no atoms yet encountered */
    tfc1  = 0; tfn2  = 0; tfca2 = 0; tfc2  = 0; tfo2  = 0; tfnh2 = 0;

    /*--initialize counters */
    /*counters for number of main chain and side chain vectors, labels, etc.*/
    mccnt = 0; sccnt = 0; sscnt = 0; ssknt = 0;
    hcount = 0; hetcnt = 0; metcnt = 0; hetknt = 0;
    lbacnt = 0; lbncnt = 0; lbzcnt = 0;
    atmcnt = 0; atncnt = 0; atocnt = 0; atscnt = 0; atwcnt = 0; atccnt = 0;
    ribcnt = 0; nrib = 0; npept = 0; mutcnt = 0;

    for(j=0; j<MAXTYPES; j++) pointcnt[j] = 0;
    ntypes = 0; /*countpoints() will reload cntp and cntn as it goes 011202*/

    /*--hbond counters */
    cocnt = 0; nhcnt = 0;

    /*--sonofchaos & oup-msb vector counter */
    vccnt = 0;

    /*__open scratch "tapes" */
    inittextblock(&mainscratch);
    inittextblock(&hbnscratch);
    inittextblock(&hboscratch);
    if(Lhetconnect)
    {
        inittextblock(&hetscratch);
    }
    /*--rewind scratch "tapes", inplicit with inittextblock() */
    /*--allocation of space automatic when attempting write*/

    /*initial conditions for range control runs */
    /*------output control logicals, one for each type of output */
    /* in effect if (mrange > 0), initialized by rangecontrol() */

    caonly = 0;  /*ca only*/
    caoutl = 0;  /*ca-ca bonds */

    /*output list controls*/
    Listmc = FALSE;Listmcn = FALSE;Listmco = FALSE;Listmcs = FALSE;
    Listsc = FALSE;Listscn = FALSE;Listsco = FALSE;Listscs = FALSE;
    Listca = FALSE;
    Listlb = FALSE;Listlbn = FALSE;
    Listhb = FALSE;
    Listhy = FALSE;
    Listss = FALSE;
    Listht = FALSE;Listhtn = FALSE;Listhto = FALSE;Listhts = FALSE;
    Listwa = FALSE; Listwao = FALSE;
    Listat = FALSE;
    Listmut = FALSE;
    Listrot = FALSE;
    Listrot2 = FALSE;
    Listnum = FALSE;
    Listrib = FALSE;
    Listtaudev = FALSE;
    Listcbdev = FALSE; /*Listtaudev,Listcbdev set in PKINANGL/constructbeta()*/
      /*called from mutateresidue() when Lcbetadev==1 */
    Listsuppos = FALSE;
    Ldump = FALSE; /*011027*/
    /*Listext = FALSE;*/ /*extra output*/
       /*DO NOT DEFAULT HERE: contol with kludges on an individual pass basis*/

    Listmcc = FALSE;Listscc = FALSE;Listhtc = FALSE;/*960809*/
    Listmcv=0;Listmck=0;Listscv=0;Listsck=0;Listhtv=0;Listhtk=0; /*960815*/
    Listmckc=0;Listmcko=0;Listmckn=0;Listmcks=0;Listmckh=0; /*960815*/
    Listsckc=0;Listscko=0;Listsckn=0;Listscks=0;Listsckh=0; /*960815*/
    Listhtkc=0;Listhtko=0;Listhtkn=0;Listhtks=0;Listhtkh=0; /*960815*/
    Listtabl=0;/*991122*/
    Listscring=0;/*991128*/
    Nprotein = 0;
    Nnucleic = 0;
    Nwater = 0;
    Nhetgroup = 0;
    NDNA = 0; /*counts accum for a subunit*/
    NRNA = 0;
    LRNA = 0; /*flag could be set for each residue*/
    residuetype = ' '; /*best guess at P vs N, etc. type 061006*/
    previousresiduetype = ' '; /*061006*/

    /*auto ribbon setup moved to after first atom record of this subunit*/
    /*so the actual chain ID character will be known. */
}
/*__setsubunitconditions()__________________________________________________*/

#ifdef PRINTOUTL
printf("rangel %d, mcoutl %d, scoutl %d, lboutl %d, croutl %d, htoutl %d, hyoutl %d, hboutl %d, caoutl %d, waoutl %d, atoutl %d, mcaoutl %d, htaoutl %d, scaoutl %d, waaoutl %d, ssscoutl %d, rotoutl %d, mutoutl %d, riboutl %d, extoutl %d, mccoutl %d, sccoutl %d, htcoutl %d, mcCPKoutl %d,scCPKoutl %d,htCPKoutl %d, Bcoloutl %d, Bvaloutl %d\n"
   ,rangel, mcoutl, scoutl, lboutl, croutl,
    htoutl, hyoutl, hboutl, caoutl, waoutl,
    atoutl, mcaoutl, htaoutl, scaoutl, waaoutl,
    ssscoutl, rotoutl, mutoutl, riboutl, extoutl,
    mccoutl, sccoutl, htcoutl,
    mcCPKoutl,scCPKoutl,htCPKoutl,
    Bcoloutl, Bvaloutl);
#endif /*PRINTOUTL*/

/***rangecontrol()***********************************************************/
void   rangecontrol(void)
{
  int    j,Lgood,Lbreak,LresOK,k,m,i;
  int    Lnosidechain;
  static int oldnumber = -999;
  /*static int previousribbontype = 0; to PKINRIBB.h 090219*/
  int   Lcoilregion = 0;
  int   Lcoilend = 0;
  int   LAlpharegion = 0;
  int   LAlphaend = 0;
  int   LBetaregion = 0;
  int   LBetaend = 0;
  char  resname[4];
  char Cabc=' ',Cnc=' ';
  int  Newaspects=0;
  char thischain[3] = {' ',' ','\0'}; /*2char sub== chainID 070926 */
       /*sub[maxatm] defaults to \0 to take no space*/
       /*so need to match blank chain with blank, not NULL  050925*/
       /*but it looks as if sub[][0] can be read in as a space character!*/
       /*2007 preserve pdb format spacing in output,  2char chainID 070926*/

  if(Lnewribbonstart)
  {previousribbontype=0; explicitribbontype=0; Lnewribbonstart=0;} /*090219*/
  if(mrange == 0)
  {/*no range controls*/
        /*allow everything, except atom markers, labels, and h-bonds */
        rangel = 1; mcoutl = 1; scoutl = 1; lboutl = 0; croutl = 1;
        htoutl = 1; hyoutl = 1; hboutl = 0; caoutl = 1; waoutl = 1;
        atoutl = 0; mcaoutl = 0; htaoutl = 0; scaoutl = 0; waaoutl = 0;
        ssscoutl = 0; /*all cys sc atoms along with -ss- */
        rotoutl = 0; mutoutl = 0;
             /*mutations && rotations not standard things*/
        riboutl = 0; /*ribbons not quite in CHAOS form*/
        extoutl = 0; /*   extra stuff*/
        mccoutl = 0; sccoutl = 0; htcoutl = 0; /*960815*/
        mcCPKoutl = 0;scCPKoutl = 0;htCPKoutl = 0; /*960815*/
        Bcoloutl = 0; Bvaloutl = 0; /*971213*/
        tabloutl = 0; /*991122*/
        supoutl = 0; /*020227*/
        siteoutl = 0; /*050731*/
  }/*no range controls*/
  else
  {/*mrange!=0:expect range controls*/
   /*first prohibit everything, with exceptions... 090219 */
   rangel = 0; mcoutl = 0; scoutl = 0; lboutl = 0; croutl = 0;
   htoutl = 0; hyoutl = 0; hboutl = 0; caoutl = 0; waoutl = 0;
   atoutl = 0; mcaoutl = 0; htaoutl = 0; scaoutl = 0; waaoutl = 0;
   ssscoutl = 0; rotoutl = 0; mutoutl = 0;  extoutl = 0;
   mccoutl = 0; sccoutl = 0; htcoutl = 0; /*960815*/
   mcCPKoutl = 0;scCPKoutl = 0;htCPKoutl = 0; /*960815*/
   Bcoloutl = 0; Bvaloutl = 0; /*971213*/
   tabloutl = 0; /*991122*/
   supoutl = 0; /*020227*/
   siteoutl = 0; /*050731*/
   /*do not reset riboutl : signals ribbon after all residues, if any need it*/
   /*then scan all ranges to see if this residue is included*/
   /* finds range by inclusive residue number */
   /* exclusive mode has two sub modes*/
   Lbreak = 0; /*lets exclusion jump out once occurred*/
   Lcoilregion = 0;
   Lcoilend = 0;
   LAlpharegion = 0;
   LAlphaend = 0;
   LBetaregion = 0;
   LBetaend = 0;
   Lendribbon = 0; /*global*/

   /*working on a residue....*/
   /*    identifiers of residue taken as identifiers on the last atom*/

   if(sub[maxatm][0] == '\0') /*070926 legacy code...*/
   {
      thischain[0] = ' '; thischain[1] = ' '; /* 2char chainID 070926*/
      thischain[2] = '\0';
       /*sub[maxatm] used to default to \0 to take no space*/
       /*so need to match blank chain with blank, not NULL  050925*/
       /*but now preserve pdb format in pointID and 2char chainID 070926*/
   }
   else
   {
      thischain[0] = sub[maxatm][0]; /*070926*/
      thischain[1] = sub[maxatm][1]; /*070926*/
      thischain[2] = '\0';
   }

   /*selections.........*/
   /*special selections......*/
   if(Lallaminoacids) /*special selections */
   {
        if(iresiduecnt == 2) ssscoutl = 1; /*all cys sc atoms with -ss- */
        if(iresiduecnt == 4) lboutl = 1;  /* glycine labels */
   }
   /*general selections... working in rangecontrol()...*/
   Lnosidechain = 0; /*flag to exclude by res name*/
   Lgood = 1; /*flag to still consider this residue*/
   /*first check for possible NMR  models*/
   /*NMR models can contain multiple chains, so models take precedence*/
   /*NOTE: to use same commandline for both NMR and XL files */
   /*include "_" as a possible model code and "_" as a possible chain ID*/
   if(Nmodelcharcode > 0)
   {/*commandline has specified model ID as a selection criterion*/
      Lgood = 0; /*must match model ID to be selected*/
      for(j=0; j<Nmodelcharcode; j++)
      {/*check against all permitted model codes*/
         /*use: mod[maxatm] for the current residue */
         /*instead of: nmrmodel which changes at ends before this test */
         if(  (toupper(modelcharcode[j][0])==toupper(mod[maxatm][2]))
            &&(toupper(modelcharcode[j][1])==toupper(mod[maxatm][3]))
            &&(toupper(modelcharcode[j][2])==toupper(mod[maxatm][4])) )
         {/*Match of model IDs allowed to present model ID*/
            Lgood = 1;
            break; /*any one match allows this model*/
         }
      }/*check against all permitted model codes*/
   }/*commandline has specified model ID as a selection criterion*/
   if(Lgood)
   {/*passed model filter, check for chain ID*/
     if(Nchaincharcode > 0)
     {/*commandline has specified chain ID as a selection criterion*/
       Lgood = 0; /*must match chain ID to be selected*/
       for(j=0; j<Nchaincharcode; j++)
       { /*chainID case sensitive pdb v3.0    070926*/
         if(  (chaincharcode[j][0]==thischain[0])  /*2char 070926*/
            &&(chaincharcode[j][1]==thischain[1])) /*2char 070926*/
         {/*Match of chain IDs allowed to present chain ID*/
            Lgood = 1;
            break; /*any one match allows this chain*/
         }
       }
     }
   }/*passed model filter, check for chain ID*/

   if(Laspectabc) /*search actual ranges 050924*/
   {/*aspect by any inputed ALPHA or BETA HEADER records 010920*/
      /*050924 ???? FLAG NEEDED FOR EXISTANCE OF HELIX,SHEET RECORDS? */
      /*search through the separate rib ranges for sec str designation 050928*/
      Cabc = 'I'; /*sky  default coil*/

      for(j=1; j<=Nribs; j++)
      {
         if(    ribBeta[j]
             && thischain[0] == ribchain[j][0]  /*sub[maxatm][0] 2char 070926*/
             && thischain[1] == ribchain[j][1]  /*sub[maxatm][1] 2char 070926*/
             && num[maxatm] >= ribresnu1[j]
             && num[maxatm] <= ribresnu2[j]
           )
         {/*residue is in Beta SHEET*/
            Cabc = 'F'; /*green*/
            break; /*on first instance of matching Beta range*/
         }
         else if(    ribAlpha[j]
             && thischain[0] == ribchain[j][0]  /*sub[maxatm][0] 2char 070926*/
             && thischain[1] == ribchain[j][1]  /*sub[maxatm][1] 2char 070926*/
             && num[maxatm] >= ribresnu1[j]
             && num[maxatm] <= ribresnu2[j]
           )
         {/*residue is in Alpha HELIX*/
            Cabc = 'A'; /*red*/
            break; /*on first instance of matching Alpha range*/
         }

      }
   }/*aspect by any inputed ALPHA or BETA HEADER records */

   if(LcolorbyNtoC && icountresfirstpass[level] != 0)
   {/*011109 commandline request for pointcolor colorNtoC*/
       if(icountres[level] < icountresfirstpass[level]/10)
          sprintf(colorbyNtoC," blue");
       else if(icountres[level] < 2*(icountresfirstpass[level]/10))
          sprintf(colorbyNtoC," sky");
       else if(icountres[level] < 3*(icountresfirstpass[level]/10))
          sprintf(colorbyNtoC," cyan");
       else if(icountres[level] < 4*(icountresfirstpass[level]/10))
          sprintf(colorbyNtoC," sea");
       else if(icountres[level] < 5*(icountresfirstpass[level]/10))
          sprintf(colorbyNtoC," green");
       else if(icountres[level] < 6*(icountresfirstpass[level]/10))
          sprintf(colorbyNtoC," lime");
       else if(icountres[level] < 7*(icountresfirstpass[level]/10))
          sprintf(colorbyNtoC," yellow");
       else if(icountres[level] < 8*(icountresfirstpass[level]/10))
          sprintf(colorbyNtoC," gold");
       else if(icountres[level] < 9*(icountresfirstpass[level]/10))
          sprintf(colorbyNtoC," orange");
       else
          sprintf(colorbyNtoC," red");
   }/*011109 commandline request for pointcolor colorNtoC*/
   else
   {
      colorbyNtoC[0] = '\0'; /*null string*/
   }/*011109 NOT commandline request for pointcolor colorNtoC*/

   if(Laspectnc && icountresfirstpass[level] != 0)
   {/*010920 aspect by N-->C proportion*/
      Newaspects++;
       if(icountres[level] < icountresfirstpass[level]/10)
          Cnc = 'J'; /*blue*/
       else if(icountres[level] < 2*(icountresfirstpass[level]/10))
          Cnc = 'I'; /*sky*/
       else if(icountres[level] < 3*(icountresfirstpass[level]/10))
         Cnc = 'H'; /*cyan*/
       else if(icountres[level] < 4*(icountresfirstpass[level]/10))
          Cnc = 'G'; /*sea*/
       else if(icountres[level] < 5*(icountresfirstpass[level]/10))
          Cnc = 'F'; /*green*/
       else if(icountres[level] < 6*(icountresfirstpass[level]/10))
          Cnc = 'E'; /*lime*/
       else if(icountres[level] < 7*(icountresfirstpass[level]/10))
          Cnc = 'D'; /*yellow*/
       else if(icountres[level] < 8*(icountresfirstpass[level]/10))
          Cnc = 'C'; /*gold*/
       else if(icountres[level] < 9*(icountresfirstpass[level]/10))
          Cnc = 'B'; /*orange*/
       else
          Cnc = 'A'; /*red*/
   }/*010920 aspect by N-->C proportion*/

   if(MAXaspectcode > Naspectcode) /*Naspectcode: # inputted*/
   {/*add these new aspects to any inputed on PDB ATOM records*/
      for(j=1 ; j<=maxatm; j++)
      {/*scan over all atoms of this residue*/
         /*if(aspectstr[j][Naspectcode]=='\0') */
         /*{*/ /*need to initialize this string, Naspectcode should==0*/
         /*   aspectstr[j][Naspectcode]='(';*/ /*necessary starting char*/
         /*   aspectstr[j][Naspectcode+1]=')';*/  /*for completeness*/
         /*   aspectstr[j][Naspectcode+2]='\0';*/ /*for completeness*/
         /*}*/
         if(aspectstr[j][0]=='\0')
         {/*need to initialize this string, Naspectcode should==0*/
            aspectstr[j][0]='('; /*necessary starting char*/
            aspectstr[j][1]=')';  /*for completeness*/
            aspectstr[j][2]='\0'; /*for completeness*/
         }
         /*with some presumption that aspect index numbers are correct*/
         /* and that aspects input in header records are already in the str*/
         /* so add on to end, over-writing the trailing end-paren */
         if(Laspectnc)  {aspectstr[j][Naspectnc] =Cnc;}
         if(Laspectabc) {aspectstr[j][Naspectabc]=Cabc;}
         aspectstr[j][MAXaspectcode+1]=')'; /*necessary ending char*/
         aspectstr[j][MAXaspectcode+2]='\0'; /*regular str ending*/
      }
   }/*add these new aspects to any inputed on PDB ATOM records*/

   if(Lgood)
   {/*acceptable chain: check each range for this residue*/
    for(j=1 ; j<=mrange ; j++)
    {/*check each jth range*/
      Lgood = 0; /*controls for each range*/
                 /*rejects unless accepted*/
      if(Linclusive)
      {
        if((num[maxatm] >= resnu1[j]) && (num[maxatm] <= resnu2[j]) )
        {
           Lgood=1;
        }
      }
      else
      {
        Lgood = 1; /*accepts unless excluded*/
        if(Loverlap) /*lap into ranges, this gives extra at ends*/
        {
           if( (num[maxatm] > resnu1[j]) && (num[maxatm] < resnu2[j]) )
           {  /*exclude this residue*/
              Lgood = 0;
              rangel = 0;
              Lbreak = 1;
           }
        }
        else /*don't lap into ranges, this gives spaces on ca-ca*/
        {
           if( (num[maxatm] >= resnu1[j]) && (num[maxatm] <= resnu2[j]) )
           {  /*exclude this residue*/
              Lgood = 0;
              rangel = 0;
              Lbreak = 1;
           }
        }
      }
      if(Lbreak) break;

      if(Lgood)
      {/*Lgood range*/
        /*Now must see if this residue should have conditions checked*/

        LresOK = 1; /*presume acceptable and needing extra conditions*/

        /*Now do tests on this residue which can modify this presumption*/
        /*if Ldotlist has set coordinate triples for this range, */
        /*  an atom of this residue must have this coord to allow output*/
        if(xyzl[j])
        {/*using coord to identify residues for output*/
            LresOK = 0; /*not expected to match*/
            for(k=1;k<=maxatm;k++)
            {
                 if(   (x[k]>=fxyz[j][0]-.002 && x[k]<=fxyz[j][0]+.002)
                    && (y[k]>=fxyz[j][1]-.002 && y[k]<=fxyz[j][1]+.002)
                    && (z[k]>=fxyz[j][2]-.002 && z[k]<=fxyz[j][2]+.002)  )
                       LresOK = 1; /*accepted, need conditions*/
            }
        }


        /* resl[]: if invoked, there is exclusive choice by residue type */
        /* if residue name is not specified for this range, then it is a */
        /* valid range for any residue, but if the residue name is given,*/
        /* it must match the current residue for this range to be considered*/

        /*970419: resl[] now a positive number for inclusions by res name, */
        /*a negative number for exclusions by name, resid[][012,345,678,...]*/
        /*holds the sequence of characters of these names */
        /*Negative resl[] not only blocks this jth range from having a*/
        /*contribution from this residue, it sets a flag so the sidechain*/
        /*of this residue will not be shown */


        /*if mutl[j] set then resl[j] will have been set as well */
        /*  if no residue name given, then sc rotations are set up*/
        /*  if residue named, then this residue is to be mutated */
        /*   to the given one, then sc rotations set up */
 /*NOT HERE 051213 ???? SITE by number, not name, as well 050917*/
 /*maybe needed for fakesite from GUI ???? */
        if(resl[j]==0)
        {/*unnamed residue*/
           if(rotl[j]==1)
           {/*unnamed residue at this location is to have rotations*/
              resid[j][0] = res[maxatm][0];
              resid[j][1] = res[maxatm][1];
              resid[j][2] = res[maxatm][2];
              resl[j] = 1; /*now set to just do rotations*/
              rotoutl = j; /*set up rotations*/
              rangel = 1;  /* accept this residue as rotatable*/
              scoutl = 1;  /*set to get sidechain*/
              numrotsc = num[maxatm];
              /*NOT named, do rotations for whatever residue is here*/
              LresOK = 1; /*rotation can have specifications*/
              /*modelnumber==this chain's model, nmrmodel== MODEL record*/
              /*[0]=='\0' no modelnumber else: [0]==' ',[1]=='M',[2]=='#'...*/
             if(modelnumber[0] == ' ') /*leading space to print nicely 050121*/
             { /*bondrotation label has spaces for readability */
               sprintf(rotatorname,"%s %s %d %s"
                            ,resid[j],sub[maxatm],num[maxatm],modelnumber);
             }
             else
             {
               sprintf(rotatorname,"%s %s %d",resid[j],sub[maxatm],num[maxatm]);
             }
              /*rotatorname incl sub 030925*/
           }
        }/*unnamed residue*/
        else /*(resl[j] != 0)*/
        {/*residue by name*/ /*PREKIN IS SCHIZOPHRENIC WITH REGARD TO CASE*/
          /*MOST INPUT FILES, INC PDB, USE UPPER CASE */
          /*PREKIN CONVERTS ATOM RECORD STUFF INTO LOWER CASE*/
          /*PROBLEM IS MATCHING USER INPUT OF THINGS LIKE RESIDUE NAMES*/

          LresOK = 0; /*presume no match*/
          /*check for mutant ninja rotations, only possible for resl==1*/
          if(resl[j] ==1 && mutl[j] && name[maxatm][0]=='a') /*ATOM records*/
          {/*mutation and/or rotation*/
             rotoutl = j; /*set up rotations*/
             rangel = 1;  /* accept this residue to be mutated*/
             scoutl = 1;  /*set to get sidechain*/
             numrotsc = num[maxatm];

             resid[j][0] = tolower(resid[j][0]); /*011202*/
             resid[j][1] = tolower(resid[j][1]);
             resid[j][2] = tolower(resid[j][2]);

             mutoutl = j;
             /*Residue named: mutate to this resid[j] residue*/
             /*or rebuild the same one from a library residue*/
             LresOK = 1; /*rotation can have specifications*/
              /*modelnumber==this chain's model, nmrmodel== MODEL record*/
              /*[0]=='\0' no modelnumber else: [0]==' ',[1]=='M',[2]=='#'...*/
             if(modelnumber[0] == ' ') /*leading space to print nicely 050121*/
             { /*bondrotation label has spaces for readability */
               sprintf(rotatorname,"%s %s %d %s"
                            ,resid[j],sub[maxatm],num[maxatm],modelnumber);
             }
             else
             {
               sprintf(rotatorname,"%s %s %d",resid[j],sub[maxatm],num[maxatm]);
             }
             /*rotatorname incl sub 030925*/
          }/*mutation and/or rotation*/
          else
          {/*look for residue name match*/
            if(resl[j]<0)
            {
                m = -resl[j];
            }
            else
            {
                m =  resl[j];
            }
            for(k=1; k<=m; k++)
            {/* look for one or more residue name matches*/
              i = 3*(k-1);
              if(      res[maxatm][0] == tolower(resid[j][0+i])
                    && res[maxatm][1] == tolower(resid[j][1+i])
                    && res[maxatm][2] == tolower(resid[j][2+i])     )/*011202*/
              {
                 LresOK = 1;
                if(Lsite)
                {
                   Lsitefake = 1; /*add this named res along with site res*/
                   siteoutl = 1;  /*051213*/
                }
              }
              else if(Lallbases)
              {
                  resname[0] = toupper(res[maxatm][0]);
                  resname[1] = toupper(res[maxatm][1]);
                  resname[2] = toupper(res[maxatm][2]);
                  resname[3] = '\0';
                  if(strstr(NAList, resname))
                  {/*recognized as a Nucleic Acid*/
                     if(NA_BaseCategory(resname) == iresiduecnt)
                     {/*name matches cycle counter*/
                        LresOK = 1;
                     }
                     else
                     {
                        LresOK = 0;
                     }
                  }
                  else
                  {/*NOT recognized as a Nucleic Acid*/
                     LresOK = 0;
                  }
              }
            }/* look for one or more residue name matches*/
            if(resl[j]<0)
            {
              if(LresOK==1 && resl[j]<0)
              {/*exclude this sidechain*/
                Lnosidechain = 1;
              }/*exclude this sidechain*/
              LresOK = 1; /*since only excluding sidechain anyway*/
            }
          }/*look for residue name match*/
        }/*residue by name*/
        if(LresOK)
        {/*LresOK: get conditions*/
          rangel = 1;  /* turn on this residue for general use*/
          if(supl[j])
          {
             supoutl = 1; /*superposition of aa type onto ideal std*/
          }
          /*set logicals for only what is allowed as output in this range */
          /*these accummulate for this residue for all ranges that have this*/
          /* residue*/
          /* logicals act at the write-scratch-file place */
          if (mcxl[j]) mcoutl = 1;  /* mainchain */
          if(Lallbases && iresiduecnt>1) mcoutl = 0; /*override*/
          if (scxl[j]) scoutl = 1;  /* sidechain */
          if(Lallbases && iresiduecnt==1) scoutl = 0; /*override*/
          if (hbxl[j]) hboutl = 1;  /* mc h-bonds */
          if (hyxl[j]) hyoutl = 1;  /* hydrogens */
          if (caxl[j])
          {
              if(Ltable) tabloutl = 1; /*991122*/
              else       caoutl = 1;  /* ca-ca */
          }
          if (htxl[j]) htoutl = 1;  /* hetatms */

          if(  (mcxl[j]) && mcCPKl[j]) mcCPKoutl = 1; /*960815*/
          if(  (scxl[j]) && scCPKl[j]) scCPKoutl = 1; /*960815*/
          if(  (htxl[j]) && htCPKl[j]) htCPKoutl = 1; /*960815*/

          if (atxl[j]) atoutl = 1;  /* some atom markers */
          if (mcal[j]) mcaoutl = 1;  /* mainchain atom markers*/
              if(mcal[j] && ctxl[j]) mccoutl = 1; /*carbon atoms also 960815+*/
          if (scal[j]) scaoutl = 1;  /* sidechain atom markers*/
              if(scal[j] && ctxl[j]) sccoutl = 1; /*carbon atoms also 960815+*/
          if (htal[j]) htaoutl = 1;  /* het group atom markers*/
              if(htal[j] && ctxl[j]) htcoutl = 1; /*carbon atoms also 960815+*/

          if (Bcol[j]) Bcoloutl = 1;  /* color by B-values */ /*971213*/

          if (Bval[j]) Bvaloutl = 1;  /* pointID B-values & occupancies*/

          if (lbxl[j]) lboutl = 1;  /* residue labels */

          if (cssl[j]) ssscoutl = 1; /*all cys sc atoms with -ss- */

          if (waxl[j]) waoutl = 1;

          if(waoutl)  if (waal[j]) waaoutl = 1;  /* water atom markers*/

          if(sitel[j]) /*broken after 050925 to 051212, Nsite not implemented*/
          {/*051213 fix the 050731 site residue, e.g. for rib stud*/
             /*char sitechain[MAXR][3],siteinsert[MAXR]; int siteresnum[MAXR];*/
             for(k=1; k <= Nsite; k++) /*check residue against SITE records*/
             {/*search in separate site ranges*/
                if(    (num[maxatm] == siteresnum[k])
                   &&  (rins[maxatm][0] == siteinsert[k])  /* char comparison*/
                   &&  (thischain[0] == sitechain[k][0])   /* 2char 070926*/
                   &&  (thischain[1] == sitechain[k][1]) ) /* 2char 070926*/
                {
                   siteoutl = 1; /*this residue is a valid site residue*/
                   scoutl = 1;   /*051213*/
                }
             }
          }
          /*050928 ribl[j] is part of general ranges, flags search for */
          /* Alpha, Beta, Coil in separate ribbon sec str ranges */
          /*if(  (ribl[j] || Coil[j] || Alpha[j] || Beta[j])*/

          if( ((ribl[j])||(Best[j])) &&( (char)name[maxatm][0] != 'h') )
          {/*ribbon controls  NOT applied to hets, e.g. HOH of 1BOV.pdb 041106*/
             riboutl = 1;
             /*090219 this would fail if MSE or other het as last res of atoms*/
             /*  so riboutl should be a set-once per level logical*/
             /*setsubunitconditions() does reset riboutl, */
             /*  so rangecontrols should NOT clear riboutl when called 090219*/
             /*always look for secondary structure records HELIX, SHEET 051208*/
              for(k=1; k <= Nribs; k++)
              {/*search in separate ribbon sec str ranges 1--k--Nribs*/

                /*THIS ROUTINE REQUIRES CORRECT FILL OF THESE RANGES*/
                if(   (num[maxatm] >= ribresnu1[k])
                   && (num[maxatm] <= ribresnu2[k])
                   && ((thischain[0] == ribchain[k][0])||(ribchain[k][0]=='+'))
                   && ((thischain[1] == ribchain[k][1])||(ribchain[k][1]=='+')))
                {/*appropriate range where + is GUI selected range*/
                  /*prekin not dealing with residue insertion codes*/

                  /* ribbons: Alpha & Beta can overlap */
                  if((ribAlpha[k])||(ribBeta[k]))
                  {
                    if(ribAlpha[k])
                    {
                       LAlpharegion = 1;
                       Lcoilregion = 0;
                       if(num[maxatm] == ribresnu2[k])
                       {
                          if(num[maxatm] != oldnumber)
                          {
                             LAlphaend = 1;
                          }
                          else
                          {/*ribbon region ends on first of set of insertions*/
                             LAlpharegion = 0;
                          }
                       }
                    }
                    else
                    {
                       LAlpharegion = 0;
                    }
                    if(ribBeta[k])
                    {
                       LBetaregion = 1;
                       Lcoilregion = 0;
                       if(num[maxatm] == ribresnu2[k])
                       {
                          if(num[maxatm] != oldnumber)
                          {
                             LBetaend = 1;
                          }
                          else
                          {/*ribbon region ends on first of set of insertions*/
                             LBetaregion = 0;
                          }
                       }
                       /*just doing beta for nucleics now, 060929*/

                       strcpy(explicitribbonidentity,ribidentity[k]);
                       /*explicitribbonidentity,ribidentity SHEET 060929*/
                       Lribidentity = 1; /*define rib sec str colorsets*/

                    }
                    else
                    {
                       LBetaregion = 0;
                    }
                  }
                  /*if(ribCoil[k])*/ /*050928 default to coil*/
                  else
                  {
                     Lcoilregion = 1;
                     LAlpharegion = 0;
                     LBetaregion = 0;
                     if(num[maxatm] == ribresnu2[k])
                     {
                        if(num[maxatm] != oldnumber)
                        {
                           Lcoilend = 1;
                        }
                        else
                        {
                           Lcoilregion = 0;
                        }
                     }
                  }
                }/*appropriate range*/
                else
                {/*not in a defined range, default to coil*/
                   Lcoilregion = 1;
                }
              }/*search in separate ribbon sec str ranges 1--k--Nribs*/

          }/*ribbon controls*/
          if(Lnosidechain) scoutl = 0; /*override by res name exclusion*/

        }/*LresOK: get conditions*/
      }/*Lgood range*/
    }/*check each jth range*/
    if(Lcoilregion || LAlpharegion || LBetaregion)
    {/*explicit ribbon under construction*/
       if(LAlpharegion && LBetaregion)
       {/*double listed, try to decide between Alpha and Beta*/
          if(previousribbontype==BETA2D)
          {/*continuing Beta takes precedence over Alpha*/
             if(LBetaend) explicitribbontype = BETAARROW2D;
             else         explicitribbontype = BETA2D;
          }
          else if(previousribbontype==ALPHA2D)
          {
             if(LAlphaend) explicitribbontype = ALPHAARROW2D;
             else          explicitribbontype = ALPHA2D;
          }
          else
          {/*Beta takes precedence over Alpha*/
             if(LBetaend) explicitribbontype = BETAARROW2D;
             else         explicitribbontype = BETA2D;
          }
       }/*double listed, try to decide between Alpha and Beta*/
       else if(LBetaregion)
       {
          {/*Beta takes precedence over Alpha*/
             if(LBetaend) explicitribbontype = BETAARROW2D;
             else         explicitribbontype = BETA2D;
          }
       }
       else if(LAlpharegion)
       {
             if(LAlphaend) explicitribbontype = ALPHAARROW2D;
             else          explicitribbontype = ALPHA2D;
       }
       else
       {/*claims to be just coil, see if Alpha or Beta needs end arrow*/
          if(previousribbontype==BETA2D) /*ended would be BETAARROW2D*/
          {/*ending Beta takes precedence over ending Alpha*/
             explicitribbontype = BETAARROW2D;
          }
          else if(previousribbontype==ALPHA2D) /*ended would be ALPHAARROW2D*/
          {
             explicitribbontype = ALPHAARROW2D;
          }
          else
          {
             explicitribbontype = COIL2D;
          }
       }/*claims to be just coil, see if Alpha or Beta needs end arrow*/
       if(Lcoilend) Lendribbon = 1;
    }/*explicit ribbon under construction*/
    previousribbontype = explicitribbontype;
    oldnumber = num[maxatm];

   }/*acceptable chain: check each range for this residue*/
   if(Lribbonalso){rangel=1; riboutl=1; /*050206 continuous ribbon*/ }
  }/*mrange!=0:expect range controls*/
/*keep this as a horrible example of hets (HOH)  being included in ribbon calc
if(explicitribbontype!=savedvalue)fprintf(stderr,"level==%d, nrib==%d: res: %s %s %d: rangecontrol explicitribbontype: %d-->%d\n",level,nrib,name[maxatm],res[maxatm],num[maxatm],savedvalue,explicitribbontype);
*/
}
/*__rangecontrol()__________________________________________________________*/

/***checkline()**************************************************************/
int    checkline(char texts[256])
{  /* check of a line to see if it is a valid coord record  */

 static  int   Lcompnd = 0;
    int    ignore,j,k;
    char   chainID[3] = {' ',' ','\0'}; /*2char 070926*/
    int    ireturn = 0;
    int    naspect=0,Lnew=0;
    int    Lnumber = 1, nc=0;
    char   cnumber[256];
    int    ith=0,LOK=0,ithnum=0,ithnumr,ithnum0=0,ithnum1=0,ithnumt;
    int    identity=0; /*060929*/

  /* need to check for atom or hetatm key word */
  /* keyword  must be contiguous characters as a single word */
  /* this routine looks for atom and hetatm */
  ireturn = 0;  /* presume not a coordinate record line until proven */
    /* try ATOM or HETATM, CompArgStr iqnores case*/
    if(    CompArgStr(texts,"ATOM",4)     /*PUXMINIT*/
        || CompArgStr(texts,"HETATM",6) )
    {
       if(Lthispass==1 && inputflag == 0 && Nsite > 0 && CompArgStr(texts,"ATOM",4) )
       {
/*
fprintf(stderr,"checkline finds ATOM record: Nsite== %d\n",Nsite);
fprintf(stderr,"%s\n",texts);
*/
        /*050928 keep site ranges stored separately*/
        /*  computesiteranges();*/ /*050731*/
        /*  Nsite = 0;*/  /*050731 site info now stored in range info*/
       }
       ireturn = 1;  /* set flag for a coordinate record line */
    }
    else if(CompArgStr(texts,"TER",3) )    /*PUXMINIT*/
    {
       ireturn = 2;
    }
    else if(CompArgStr(texts,"END",3) )    /*PUXMINIT*/
    {
       ireturn = 3;
       if(CompArgStr(texts,"ENDMDL",6) )    /*PUXMINIT*/
          Ngroups = 0; /*Number of groups outputed reset for this MODEL*/
    }
    else if(CompArgStr(texts,"MOD",3) )    /*PUXMINIT*/
    {
/*
01234567890123...  index of texts[]
123456789_1234...  columns
MODEL       20                                                          2CBH9610
*/
       if(CompArgStr(texts,"MODEL",5) )
       {/*really is a MODEL record, remember putative MODEL # chars*/
/*
But, as one might expect, this number is not always placed in strict pdb format
so for robustness, try to find it...     030111
*/
         /*set off model# by "M" to distinguish from residue number*/
         /*this will be in addition to chain ID Alphabetic character*/
         nmrmodel[0] = ' '; /*leading blank for spacing on output line*/
         nmrmodel[1] = 'M'; /*000418*/
         nmrmodel[2] = '1'; /*  1's digit default*/
         nmrmodel[3] = '\0';
         nmrmodel[4] = '\0';
         nmrmodel[5] = '\0';
         j = strlen(texts); /*length exclusive of EOL, so EOL is indexed as j*/
         if( j>=14 && isdigit(texts[13]) )
         {/*most likely this is actually standard pdb format*/
          if(texts[11] != ' ')
          {/*hundreds*/
             nmrmodel[2] = texts[11];/*possible 100's digit*/
             nmrmodel[3] = texts[12]; /*possible  10's digit*/
             nmrmodel[4] = texts[13]; /*expected   1's digit*/

          }
          else if(texts[12] != ' ')
          {/*tens*/
             nmrmodel[2] = texts[12]; /*possible  10's digit*/
             nmrmodel[3] = texts[13]; /*expected   1's digit*/
          }
          else
          {/*only units*/
             nmrmodel[2] = texts[13]; /*  1's digit*/
          }
         }/*most likely this is actually standard pdb format*/
         else
         {/*NOT standard pdb format, try to find a model number*/
          /*take intact set of digits nearest MODEL*/
          Lnumber = 0;
          nc = -1; /*augment before storage, end digit indexed by nc*/
          for(k=5; k<j; k++)
          {
             if(isdigit(texts[k]))
             {
                Lnumber = 1;
                cnumber[++nc] = texts[k];
             }
             else
             {
                if(Lnumber) {break;}
             }
          }
          if(Lnumber)
          {/*found something*/
             if(nc>=2)
             {/*at least 3 digit number, take 3 digits*/
                nmrmodel[2] = cnumber[nc-2]; /*possible 100's digit*/
                nmrmodel[3] = cnumber[nc-1]; /*possible  10's digit*/
                nmrmodel[4] = cnumber[nc];   /*expected   1's digit*/
             }
             else if(nc==1)
             {/*tens*/
                nmrmodel[2] = cnumber[nc-1]; /*possible  10's digit*/
                nmrmodel[3] = cnumber[nc];   /*expected   1's digit*/
             }
             else
             {/*only units*/
                nmrmodel[2] = cnumber[nc];   /*  1's digit*/
             }
          }
         }/*NOT standard pdb format, try to find a model number*/
         Nmodels = Nmodels + 1;
         Lonegroup = 1; /*once encounter MODEL, then mode set*/
         Lnosubgroupmasters = 0; /*050925*/
         Ngroups = 0; /*Number of groups outputed reset for this MODEL*/
         ireturn = 4;/*moved to here 040406, distinguish MOD MODEL MODRES etc.*/
         /*NOTE: nmrmodel now set to NEXT MODEL # of the input file! */
         Lmodel = 1; /*at least one MODEL record exists  050922*/
         if(Lthispass > 1)
         {
            if(FirstModel[0]=='\0') {strcpy(FirstModel,nmrmodel);} /*initiate*/
            if( !strcmp(FirstModel,nmrmodel) )
            {
               Nthmodel = 1; /*still on first model*/
            }
            else
            {
               Nthmodel++; /*augment counter for MODEL records 050922*/
            }
         }
       }/*really is a MODEL record, remember putative MODEL # chars*/
    }

    if(Lnewpassflow)       /* try HEADER stuff*/
    {/*Lnewpassflow*/
       if(Lthispass==1)
       {/*Lthispass==1*/

          if(    Lallowaspectsin > 0
              && CompArgStr(texts,"USER",4)
              && CompArgStr(&texts[6],"ASPECT",6)
              && CompArgStr(&texts[13],"CODE",4)
            )
          {/*USER  ASPECT CODE # {label} #  */
             /*012345678901234567890*/
             /*parse for aspect-number, {aspect label}, col-of-char-code*/
             j = 18; /*position in pdb record*/
             naspect = -1;
             Lnew = 1;
             while(!getinteger(texts[j],Lnew,&naspect) )
             {
                j++;
                Lnew = 0;
                if(j>253){break;}
             }
             if(naspect >= 0 && naspect <= MAXaspects)
             {/*try to get an aspect identifier*/
                aspectid[naspect][0] = '\0';
                aspectcodecol[naspect] = -1;
                Lnew = 1;
                while(!getbracketed(texts[j],Lnew,MAXaspectchars,aspectid[naspect]) )
                {
                   j++;
                   Lnew = 0;
                   if(j>254){break;}
                }
                j++;
                Lnew = 1;
                while(!getinteger(texts[j],Lnew,&aspectcodecol[naspect]) )
                {
                   j++;
                   Lnew = 0;
                   if(j>255){break;}
                }
                if(aspectcodecol[naspect] > 0)
                {
                   Naspectcode++;
                }
             }
/*
sprintf(temps,CRLF"aspect: %d, {%s}, col: %d"
        ,naspect,aspectid[naspect],aspectcodecol[naspect]);
pkintextinsert(temps);
adjusttext(2);
*/
          }/*USER  ASPECT CODE # {label} #  */

       }/*Lthispass==1*/

    if(         CompArgStr(texts,"HEADER",6) /*PUXMINIT*/
             || CompArgStr(texts,"TITLE" ,5)
             || CompArgStr(texts,"KEYWDS",6)
             || CompArgStr(texts,"AUTHOR",6)
             || CompArgStr(texts,"EXPDTA",6) /*040318*/
             || CompArgStr(texts,"SITE"  ,4) /* also separate 050731*/
             || CompArgStr(texts,"REMARK   2 RES",14) /*040319*/
           )
    {
       /*REMARK   2 RESOLUTION. 1.4 ANGSTROMS.*/
       Lcompnd = 0;
       if(Lthispass==1) /*000621*/
       {
          putonetextblockline(&headerstuff,texts); /*PKINSCRTS*/
          if(LIOmage) putonetextblockline(&pdbinfostuff,texts); /*PKINSCRTS*/
       }
       ireturn = 0; /*OK for @text, but not grist for PREKIN mill*/
    }
    if(Lthispass==1 && inputflag==0 && Lsite && CompArgStr(texts,"SITE",4)) /*050731*/
    {
/*
fprintf(stderr,"checkline finds SITE record: Nsite== %d\n",Nsite);
*/
       accumsiteranges(texts);
    }
    else if(    CompArgStr(texts,"USER",4)   /*PUXMINIT*/
             && Lmutate==0)  /*071215 avoid for mutation */
    {
       Lcompnd = 0;
       if(Lthispass==1) /*000621*/
       {
          Nusermod++;
          if(Nusermod < 101) /*100 limit 040318*/
          {
             putonetextblockline(&headerstuff,texts); /*PKINSCRTS*/
             if(LIOmage && Nusermod<3 )
                putonetextblockline(&pdbinfostuff,texts); /*PKINSCRTS*/
                /*pdbinfostuff only used for LIOmage persistant pipe*/
          }
          else if(Nusermod == 101) /*100 limit 040318*/
          {
          /*putonetextblockline(&headerstuff,">100 USER MODS, no more shown");*/
          /*060531 static string fails in rare case where this text causes */
          /* resize of textblock where text is saved in a temp string, then */
          /*copied back into this text string.  Since text string is usually */
          /* a char[256] global (texts or temps) storage exists; however, */
          /* static "string" appearently is either too small or non-existant*/
            strcpy(texts,">100 USER MODS, no more shown"); /*060531*/
            putonetextblockline(&headerstuff,texts); /*PKINSCRTS*/
          }
       }
       ireturn = 0; /*OK for @text, but not grist for PREKIN mill*/
    }
    else if(   (     CompArgStr(texts,"COMPND",6)
                 &&  CompArgStr(&texts[9]," ",1)
                 && !CompArgStr(&texts[10],"MOL_ID",6))
           )
    {
       /*012345678901234567890*/
       /*COMPND    MOL_ID: 1;   */
       Lcompnd = 1;
       if(Lthispass==1) /*000621*/
       {
          putonetextblockline(&headerstuff,texts); /*PKINSCRTS*/
          if(LIOmage) putonetextblockline(&pdbinfostuff,texts); /*PKINSCRTS*/
       }
       ireturn = 0; /*OK for @text, but not grist for PREKIN mill*/
    }
    else if(   (     CompArgStr(texts,"COMPND",6)
                 &&  Lcompnd)
           )
    {
       if(Lthispass==1) /*000621*/
       {
          putonetextblockline(&headerstuff,texts); /*PKINSCRTS*/
          if(LIOmage) putonetextblockline(&pdbinfostuff,texts); /*PKINSCRTS*/
       }
       ireturn = 0; /*OK for @text, but not grist for PREKIN mill*/
    }

    /*050924 HELIX and SHEET records directly make ranges when Lthispass==1*/
    else if(    CompArgStr(texts,"HELIX",5) )
    {/*HELIX record*/
       Lcompnd = 0; /* ???? WHY KEEP SETTING THIS ???? */
       if(Lthispass == 1 )
       {/*process HELIX record during first pass*/
          if(texts[31] != texts[19]) /*char position -1 == char index*/
          {
            for(j=0;j<37;j++) word[j] = texts[j];
            word[j] = '\0';
            sprintf(temps,"Range %s spans subunits & will be ignored"CRLF,word);
            pkintextinsert(temps);  /*____TEXT.c*/
            adjusttext(1);          /*____TEXT.c*/
          }
          else
          {/*HELIX OK range*/
             if(Nribs < MAXR-2) /*can have another range*/
             {
                ++Nribs;
                chainID[0] = texts[18]; /*2char 070926*/
                chainID[1] = texts[19]; /*2char 070926*/
                 /* PREKIN preserves case for chainID 070926*/

                k=0;
                for(j=21;j<=24;j++) { word[k++] = texts[j]; }
                word[k] = '\0';
                ribresnu1[Nribs] = intfromstr(word);
                ribinsert1[Nribs] = texts[25];

                k=0;
                for(j=33;j<=36;j++) { word[k++] = texts[j]; }
                word[k] = '\0';
                ribresnu2[Nribs] = intfromstr(word);
                ribinsert2[Nribs] = texts[37];

                ribchain[Nribs][0] = chainID[0]; /* 2char 070926 */
                ribchain[Nribs][1] = chainID[1]; /* 2char 070926 */
                ribAlpha[Nribs] = 1; ribBeta[Nribs] = 0;
                /*there is no problem with duplicate Alpha ranges*/

             }
             /* chainID case now used in pdb v3.0 file*/
             sprintf(temps,
                  "CHAIN %s, HELIX from %d %c to %d %c"CRLF
                  ,chainID,ribresnu1[Nribs],ribinsert1[Nribs]
                  ,ribresnu2[Nribs],ribinsert2[Nribs] );
             pkintextinsert(temps);  /*____TEXT.c*/
             adjusttext(1);          /*____TEXT.c*/
             if(LIOmage) putonetextblockline(&pdbinfostuff,texts);
          }/*HELIX OK range*/
          ireturn = 0;
       }/*process HELIX record during first pass*/
    }/*HELIX record*/
/* use    Cccc these 1+3 char as ribidentity, cheat is to also use col 11
SHEET       H2 4     F  10      F  14 -1
01234567890123456789012345678901234567890123456789012345678901234567890123456789
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
HELIX_ iii ccc res C inumA res C inumAiicomment....................... lllll
HELIX       E3     F   75      F   77  0
          iden start       stop       helixclass comment               length
SHEET_ iii cccnn res CinumA res CinumA-+ atomres CinumA atomres CinumA
SHEET       H2 4     F  10      F  14 -1
          iden   start      stop      orientation&registration
          identity of sheet and nnumber of strands in sheet
*/
    else if(    CompArgStr(texts,"SHEET",5) )
    {/*SHEET record*/
       Lcompnd = 0; /* ???? WHY KEEP SETTING THIS ???? */
       if(Lthispass == 1 )
       {/*process SHEET record during first pass*/
          if(texts[32] != texts[21]) /*char position -1 == char index*/
          {
            for(j=0;j<39;j++) word[j] = texts[j];
            word[j] = '\0';
            sprintf(temps,"Range %s spans subunits & will be ignored"CRLF,word);
            pkintextinsert(temps);  /*____TEXT.c*/
            adjusttext(1);          /*____TEXT.c*/
          }
          else
          {/*SHEET OK range*/
             if(Nribs < MAXR-2) /*can have another range*/
             {/*add another Beta range*/
                ++Nribs;
                chainID[0] = texts[20]; /*2char 070926*/
                chainID[1] = texts[21]; /*2char 070926*/
                 /* PREKIN preserves case for chainID 070926*/

                identity = 0;
                k=0;
                for(j=10;j<=13;j++)
                {
                   if(texts[j] != ' ') {identity = 1;}
                   ribidentity[Nribs][k++] = texts[j]; /*060929*/
                }
                ribidentity[Nribs][k] = '\0';
                if(identity != 1) {ribidentity[Nribs][0] = '\0';} /*060929*/

                k=0;
                for(j=22;j<=25;j++) { word[k++] = texts[j]; }
                word[k] = '\0';
                ribresnu1[Nribs] = intfromstr(word);
                ribinsert1[Nribs] = texts[26];

                k=0;
                for(j=33;j<=36;j++) { word[k++] = texts[j]; }
                word[k] = '\0';
                ribresnu2[Nribs] = intfromstr(word);
                ribinsert2[Nribs] = texts[37];

                ribchain[Nribs][0] = chainID[0]; /* 2char 070926 */
                ribchain[Nribs][1] = chainID[1]; /* 2char 070926 */
                ribBeta[Nribs] = 1; ribAlpha[Nribs] = 0;

                /*BEWARE of strands that are subsets of strands: */
                ignore = 0;
                for(j=1; j<Nribs; j++)
                {/*test earlier ranges*/

                /*040323 protect against strands that are subsets of strands*/
                /*this was making wierd open arrows on the extra length strand*/
                /*strand not only can be repeated to define another sheet*/
                /*strand can be subset of another strand to define new sheet*/

                   if(  (ribchain[j][0] == ribchain[Nribs][0]) /*2char 070926*/
                      &&(ribchain[j][1] == ribchain[Nribs][1]) /*2char 070926*/
                      &&(ribresnu1[j] <= ribresnu1[Nribs])
                      &&(ribresnu2[j] >= ribresnu2[Nribs])
                     )
                   {/*new strand is a duplicate or a subset of a previous one*/
                      ignore = 1; /*ignore the new one*/
                   }

                   else if(  (ribchain[j][0] == ribchain[Nribs][0]) /*070926*/
                           &&(ribchain[j][1] == ribchain[Nribs][1]) /*070926*/
                           &&(ribresnu1[j] >= ribresnu1[Nribs])
                           &&(ribresnu2[j] <= ribresnu2[Nribs])
                     )
                   {/*a previous strand is a subset of the new one*/
                      /*replace earlier strand limits with the new limits*/
                      ribresnu1[j] = ribresnu1[Nribs];
                      ribresnu2[j] = ribresnu2[Nribs];
                     ignore = 1;/*ignore the new strand which is now redundant*/
                   }
                   if(ignore)
                   {
                      /*report to reader*/
                      /*prekin honors pdb 3.0 case sensitive chainID 070926*/
                      /*sprintf(temps,"Duplicate:ignoring: "*/
                      sprintf(temps,"Duplicate or subset: ignoring: "
                         "CHAIN %s, SHEET from %d %c to %d %c"CRLF
                         ,chainID,ribresnu1[Nribs],ribinsert1[Nribs]
                         ,ribresnu2[Nribs],ribinsert2[Nribs]);
                      pkintextinsert(temps);  /*____TEXT.c*/
                      adjusttext(1);          /*____TEXT.c*/
                      if(LIOmage) putonetextblockline(&pdbinfostuff,texts);
                      Nribs--; /*duplicate range, ignore*/
                      break; /*can only ignore current range once*/
                   }
                }/*test earlier ranges*/
             }/*add another Beta range*/
             /*prekin honors pdb 3.0 case sensitive chainID 070926*/
             sprintf(temps,
                  "CHAIN %s, SHEET from %d %c to %d %c"CRLF
                  ,chainID,ribresnu1[Nribs],ribinsert1[Nribs]
                  ,ribresnu2[Nribs],ribinsert2[Nribs] );
             pkintextinsert(temps);  /*____TEXT.c*/
             adjusttext(1);          /*____TEXT.c*/
             if(LIOmage) putonetextblockline(&pdbinfostuff,texts);
          }/*SHEET OK range*/
          ireturn = 0;
       }/*process SHEET record during first pass*/
    }/*SHEET record*/
  }/*Lnewpassflow*/
  if(Lthispass == 1) /*050409*/
  {/*firstpass*/
    if(    CompArgStr(texts,"CONECT",6) && Lconect ) /*050409*/
    {/*CONECT record*/
       /*do this at any pass since -quiet does not do a first pass!? */
         /*do not trust this: not sure when firstpass is done 050409 */
       /**prekin does only one residue at a time i.e. residue-name of atoms*/
       /** so has to remember an atom that will connect to a later one*/
       /** CONECT atom numbers are maintained in a list of pairs */
       /** where pair is earlier,later and the pairs are sorted and uniquified*/
       ith = 0;
       ithnum0 = 0;
       ithnum1 = 0;
       ithnumt = 0;
       LOK = 1;
       ncon++;
       while(LOK)
       {/*LOK to strobe out a CONECT record*/
          ithnum = 0;
          /*6 for CONECT, then each field is i5, zeroth is reference atom */
          if(isdigit(texts[6+(5*ith)+4])) /*right adjusted integers*/
          {/*isdigit*/
             k=0;
             for(j=(6+(5*ith));j<=(6+(5*ith)+5);j++)
             {
                word[k++] = texts[j];
             }
             word[k] = '\0';
             ithnum = intfromstr(word);
             if(ith==0)
             {
                ithnum0 = ithnum;
                ithnumr = ithnum; /*remember reference atom number*/
             }
             else if(ith==1)
             {
                ithnum1 = ithnum;
             }
             else
             {
                ncon++;
                ithnum1 = ithnum;
                ithnum0 = ithnumr; /*reestablish reference*/
             }
             if(ith > 0)
             {
                /*first make sure that smaller number is in zeroth place*/
                if(ithnum1 < ithnum0)
                {
                   ithnumt = ithnum1;
                   ithnum1 = ithnum0;
                   ithnum0 = ithnumt;
                }
                /*then construct single long int so sorting will be simple*/
                if(ncon < NCON)
                {
                   iconect[ncon] = (long)(ithnum0*100000 + ithnum1);
/*
fprintf(stderr,"CONECT %3d: %5ld  %5ld\n",ncon,iconect[ncon]/100000,iconect[ncon] - ( 100000*(iconect[ncon]/100000) ) );
*/
                }
             }
             ith++;
          }/*isdigit*/
          else
          {
             LOK = 0;
          }
       }/*LOK to strobe out a CONECT record*/
       if(ith < 2)
       {/*least valid case leaves ith==2*/
/*
fprintf(stderr,"no connection for ncon== %3d\n",ncon);
*/
          ncon--; /*reset so can use this slot for a valid CONECT */
       }

    }/*CONECT record*/
  }/*firstpass*/
  return(ireturn);
}  /* check of a line to see if it is a valid coord record  */
/*___checkline()____________________________________________________________*/
/****CompArgStr()***********jmw/utility***************************************/
/*int CompArgStr(char *str, char *arg, int min)*/

/****evalnonatmrecord()******************************************************/
void   evalnonatmrecord(void)
{  /* != ATOM or HETATM record --whatever it is, it isn't an atom */

  stonex = 0;
  /*--usually never find anything except TER in atom/hetatm section */
  /*--  but this would allow embedded remarks and "new model" seperators */
  if( inputflag == 2 ) /*inputflag set by checkline()*/
  {/* TER record */
    /*--finish this residue completely, give anything else a fresh start */
    endsub = 1; /*090219 now hets are mixed in with atoms, need new level*/
    endchn = 1; /*chain ends with TER, not with transition atom to hetatm*/
    endres = 1;
    /*--read through until hit a valid atom/hetatm or EOF */
    /*-- back off the counter so can keep reading, and goto read cycle */
    n = n - 1;
  }
  else if(inputflag == 3) /*inputflag set by checkline()*/
  {/* END record */
    /*--really serious about a division, declare as if subunit end, and */
    /*--finish this residue completely, --give anything else a fresh start */
    endsub = 1;
    endchn = 1;
    endres = 1;
    /*--read through until hit a valid atom/hetatm or EOF */
    /*-- back off the counter so can keep reading, and continue read cycle */
    n = n - 1;
    /* resinloop still == 0 */
  }
  else if(inputflag == 4) /*inputflag set by checkline()*/
  {/* MOD record */
    /*--check to see if any atoms yet encountered, otherwise just go on */
    if(atomin==1)
    {
      /*--starting a new MODEL, declare as if new subunit, and */
      /*--unless previous signel has already ended this "subunit"  */
      /*--finish this residue completely */
      /*--give anything else a fresh start */
      morsub = 1;
      endsub = 1;
      endchn = 1;
      endres = 1;
    }
    /*--read through until hit a valid atom or hetatm or EOF */
    /*-- back off the counter so can keep reading, and continue read cycle */
    n = n - 1;
  }
  else
  {
    /*--ne)the2 an atom nor ter, nor mod */
    /*back off the counter so can keep reading */
    n = n - 1;
  }
}    /* end of if NOT an ATOM or HETATM record */
/*__evalnonatmrecord()______________________________________________________*/

/***convertcase()************************************************************/
void   convertcase(char texts[256])
{
    /*convert upper case to lower case: an artifact of our getting so far */
    /* into unix that we did everything in lower case, including brk files! */
/*s*/  uctolctexts(texts);
    if(texts[0] != 'a' && texts[0] != 'h')
    {  /*--nasty error, should only get atom records to this point! */
/*c*/    sprintf(alertstr,"input data filters bad, line blows mind:"CRLF"%s"
                 ,texts);
/*Ms*/   DoReportDialog();
/*c*/    exit(0);
    }
}
/*__convertcase()___________________________________________________________*/

/****readbrkatmrecord()******************************************************/
void   readbrkatmrecord(void)
{
  /*****************************************************************/
  /*use of character array and C utility strstr() from select.c    */
  /*probeV2.3.5.source, Oct 14, 2000:  J. Michael Word             */
  /*  array lists now in PKINCRTL.h  061006                        */
  /*****************************************************************/
  /*char *strstr(cs,ct) return pointer to first occurrence of */
  /*string ct in cs, or NULL if not present*/

  char   frags[256],numbers[5],alphers[5];
  int    j,inumber,Lnumber,ialpher,Lalpher;
  int    igotit;
  int    chain_ctr=0, seg_ctr=0;
  static char oldalphers[5];

/*current input line is in global char str: texts[256] */
/*the \0 is in texts[itext], where itext is also a global */
/*current line from PKININPT/Getoneinputline(), itext is index of the CR or LF*/
/* so itext is the column number of the last actual character */

/* where n is the nth atom of the current residue*/
/*
name[n],Anum[n],atom[n],res[n],sub[n],&num[n],rins[n]
,&x[n],&y[n],&z[n],&o[n],&B[n],element[n]
*/
/*
55...60  : occ
61...66  : B
67...72  : Unusual
73...76 4_char_file_code
77,78 atom symbol
79,80 charge
82,83,84 : reduce:new
85...90  : beyond
*/
/*format(a6,i5,1x,a5,a3,1x,a1,i4,a1,3x,3f8.3,2f6.2)*/
/*  these next lines contain 80 char templates */
/*
0123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789
123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
atom      1  n   arg a   1      26.465  27.452  -2.490  1.00 25.18      4pti  89
atom      2  ca  arg a   1      25.497  26.862  -1.573  1.00 17.63      4pti  90
aaaahhsssss_nnnnArrr_cjjjjI___XXXX.XXXYYYY.YYYZZZZ.ZZZooo.ooBBB.BB######FFFFiiii
0123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789
aaaahhsssss_nnnnArrr_cjjjjI___XXXX.XXXYYYY.YYYZZZZ.ZZZooo.ooBBB.BB######FFFFccic
__________1_________2_________3_________4_________5_________6_________7_________
count from zero: (i.e. C not fortran)
aaaahh 0-5 name of record (char)
      sssss 6-10 Anum holds atom's number (as char[6])
           _ 11 blank, not read by prekin
            nnnn 12-15 atom name (char)
                A 16 alt conformation character, read as 5th char of atom name
                 rrr 17-19 res idue name (char)
                    _ 20 OLD blank, part of chainID PDF format v3.0 070926
                     c 21 sub unit indicator (char)
                      jjjj 22-25 num ber of residue (int)
                          I 26 r esidue ins ertion code: rins (char)
                           ___ 27-29 blank, not read by prekin
                              XXXX.XXX 30-37 x (float)
                                      YYYY.YYY 38-45 y (float)
                                              ZZZZ.ZZZ 46-53 z (float)
                         o ccupancy (float)           ooo.oo 54-59
                               B value (float)              BBB.BB 60-65
                                     U nusual val ue (float)      ###### 66-71
                                     original format              _###__ 66-71
                               73-75 (char) CODE not read by prekin     FFFF
                               76-79 (int) record #, not read by prekin     iiii
                               76-79 (char) new style element code          cccc
070520 read by prekin if there          76-77 right justified element name
   not read by prekin                   78-79 charge: e.g. 2+, 1-, etc.
*/
/*char  name[MAXARR][7],atom[MAXARR][6],res[MAXARR][4];*/
/*char  sub[MAXARR][2],rins[MAXARR][2];  sub[][3] 070926*/
/*char  Anum[MAXARR][6];*/
/*int    num[MAXARR],base[MAXARR];*/
/*float  x[MAXARR],y[MAXARR],z[MAXARR];*/
/*float  o[MAXARR],B[MAXARR];*/
/*model number gets abstracted for MOD record and stored separately*/
    /*for ease and consistency upper case converted to lower*/
    /*THIS WAS BAD IDEA, BUT STUCK: LOWER CASE IS PRESUMED THROUGHOUT PREKIN*/
    /*070926 partial patch for 2char chainID: preserve case for pdb v3.0 */

    sub[n][0] = texts[20]; /*2char chainID 070926*/
    sub[n][1] = texts[21];
    sub[n][2] = '\0';
    /* do BEFORE changing everything to lower case!!!! 070926*/
    /*change to lower case the minimal number of fields 080318*/

    /*convertcase(texts);*/     /*PKINCSUB*/

    /* do direct transfer since blanks are meaningful */
    igotit = 1;

    /*for(j=0 ; j<=5 ; j++) name[n][j] = texts[j]; */
    for(j=0 ; j<=5 ; j++) {name[n][j] = tolower(texts[j]);} /*080318*/
    name[n][6] = '\0';

    for(j=0 ; j<=4 ; j++) Anum[n][j] = texts[j+6];
    Anum[n][5] = '\0';

    if(Lleftadjatomname)
    {/*whole atomname set one char to left, note 1x field so atom[][0] ok*/
        for(j=0 ; j<=4 ; j++) atom[n][j] = texts[j+11];
        atom[n][5] = '\0';
    }
    else
    {/*standard Brk PDB: 2 char element right adjusted digits 1&2 of atom name*/
        for(j=0 ; j<=4 ; j++) atom[n][j] = texts[j+12];
        atom[n][5] = '\0';  /*NB this captures Alt character as 5th atom char*/
    }
    for(j=0 ; j<=4 ; j++)
      {atom[n][j] = tolower(atom[n][j]);} /*atom name components 080318*/

    /*for(j=0 ; j<=2 ; j++) res[n][j] = texts[j+17];*/
    for(j=0 ; j<=2 ; j++) {res[n][j] = tolower(texts[j+17]);} /*080318*/
    res[n][3] = '\0';

    /* readbrkatmrecord()... associate this atom with most recent model # */
    if(nmrmodel[0] != '\0') /*really want the actual characters to rule*/
    {/* nmr model allow for 2 (or 3) char in addition to sub==chainID designator */
       /*presumably, Lonegroup has been set by MODEL record */
       /*or this mode set in Kludges, in which case should use the real sub[]*/
       /*load this atom's mod character str with the current nmr model*/
       /*i.e. the most recent input MODEL # is held in nmrmodel*/
       /*this changes when MODEL # is the delineator for the next model*/
       /*but then the end-of-a-section code should deal with it*/
       /*so nmrmodel is safe for incoming atoms, but not for output of a model*/

       for(j=0; j< MAXmodchr; j++)
       {
          mod[n][j] = nmrmodel[j];
       }
       mod[n][MAXmodchr-1] = '\0'; /*safety*/
/*
sprintf(temps,CRLF"INPUT nmrmodel != NULL: mod[%d]==|%s|"CRLF
,n,mod[n]);
pkintextinsert(temps);
adjusttext(1);
*/
    }
    else
    {
       mod[n][0] = '\0'; /*safety*/
    }
/*
for(j=0; j< MAXmodchr; j++)
{
   if(mod[n][j] != '\0')
   {
sprintf(temps,CRLF"CURRENT mod != NULL: mod[%d][%d]==|%d|"CRLF
,n,j,(int)mod[n][j]);
pkintextinsert(temps);
adjusttext(1);
   }
}
*/

/****** residue number section **********************************************/
/*RESIDUE NUMBER LEGACY CODE, IN CONFLICT WITH COMING Hybrid36 decode/encode*/
      /*residue number sometimes has imbedded subunit indicator*/
      /*either before or after actual number, only accummulate number */
      /* from one contiguous block, but all alphabetics gathered together*/
      inumber = 0;
      ialpher = 0;
      Lnumber = 0;
      Lalpher = 0;
    /*NOT here!!!! if(n==1) for(j=0;j<=4;j++) oldalphers[j]='\0';990412*/

    for(j=0 ; j<=3 ; j++) /*should be up to 4 place integer*/
    {
        if(texts[j+22]==' ')   ; /*skip any blanks*/
        else if(  texts[j+22]=='0'||texts[j+22]=='1'||texts[j+22]=='2'
                ||texts[j+22]=='3'||texts[j+22]=='4'||texts[j+22]=='5'
                ||texts[j+22]=='6'||texts[j+22]=='7'||texts[j+22]=='8'
                ||texts[j+22]=='9'||texts[j+22]=='-'                  )
            {
            if(Lalpher)  /*already encountered alphabetic char */
            {
                if(Lnumber)
                {   /*doing alpha after number*/
                    alphers[ialpher] = texts[j+22];
                    ialpher++;
                }
                else  /*!Lnumber*/
                {   /*doing number after alph*/
                    numbers[inumber] = texts[j+22];
                    inumber++;
                }
            }
            else  /*!Lalpher*/
            {   /*doing number before any alphabetic char*/
                Lnumber = 1; /*only set this if number is first*/
                numbers[inumber] = texts[j+22];
                inumber++;
            }

        }
        else /* not a number */
        {
            Lalpher = 1; /*set this for any alphabetic char*/
            alphers[ialpher] = texts[j+22];
            ialpher++;
        }
    }
    alphers[ialpher] = '\0';
    numbers[inumber] = '\0';

/*c*/  if(inumber > 0) igotit = sscanf(numbers,"%d",&num[n]);
       else  num[n] = 0;
       if(ialpher > 0)
       {/*this residue number has embedded alphabetic characters*/
           if(sub[n][0] == ' ')
           {/* maybe case where sub indicator part of res num*/
                sub[n][0] = alphers[0];
           }
           else
           {/* already has sub indicator, */
              /* see if effective residue number has changed*/
              for(j=0;j<ialpher;j++)
              {
                  if(alphers[j] != oldalphers[j])
                  {
                      delnum = 1;
                  }
              }
           }
           for(j=0;j<=4;j++)
           {
               oldalphers[j] = alphers[j]; /*reset oldalphers*/
           }
       }/*this residue number has embedded alphabetic characters*/
       else
       {/*this residue number does NOT have embedded alphabetic characters*/
           for(j=0;j<=4;j++) oldalphers[j]='\0'; /*990412*/
       }
/*________end of residue number section _____________________________________*/

    /*INSERTION CODE*/
    rins[n][0] = texts[26];
    rins[n][1] = '\0';
    /*rins should be arbitrary and work despite case 080318 */

    /*X,Y,Z COORDINATES */
    for(j=0 ; j<=7 ; j++) frags[j] = texts[j+30];
    frags[8] = '\0';
    igotit =  sscanf(frags,"%8f",&x[n]);
    for(j=0 ; j<=7 ; j++) frags[j] = texts[j+38];
    frags[8] = '\0';
    igotit =  sscanf(frags,"%8f",&y[n]);
    for(j=0 ; j<=7 ; j++) frags[j] = texts[j+46];
    frags[8] = '\0';
    igotit =  sscanf(frags,"%8f",&z[n]);

    /*All ATOM and HETATM records should be complete to here*/
    /*but many research files will lack occ and B factors*/
    /*Getoneinputline() blanks unused char positions, test is for safety*/
    if(itext>=58)  /*occ %6.2f cols: 55-60  567.90 decimal pt in 58*/
    {
       for(j=0 ; j<=5 ; j++) frags[j] = texts[j+54];
       frags[6] = '\0';
       igotit =  sscanf(frags,"%6f",&o[n]);
       if(!igotit) { o[n] = 0; igotit = 1; } /*030223 for <= macOS9*/
    }
    else o[n] = (float)0.0;

    if(itext>=64)  /*B   %6.2f cols: 61-66  123.56 decimal pt in 64*/
    {
       for(j=0 ; j<=5 ; j++) frags[j] = texts[j+60];
       frags[6] = '\0';
       igotit =  sscanf(frags,"%6f",&B[n]);
       if(!igotit) { B[n] = 0; igotit = 1; } /*030223 for <= macOS9*/
    }
    else B[n] = (float)0.0;

    if(itext>=70)  /*Unusual   %6.2f cols: 67-72  123.56 decimal pt in 70*/
    {
       for(j=0 ; j<=5 ; j++) frags[j] = texts[j+66];
       frags[6] = '\0';
       igotit =  sscanf(frags,"%6f",&Uval[n]);
       if(!igotit) { Uval[n] = 0; igotit = 1; } /*030223 for <= macOS9*/
    }
    else Uval[n] = (float)0.0;

    if(itext>=76) /*segid in columns 73,74,75,76 (index 72,73,74,75)*/
    {
      for(j=0; j<=3 ; j++) {
        segid[n][j] = texts[j+72];
      }
      segid[n][4] = '\0';
    }
    else
    {
      segid[n][0] = '\0';
    }

    if(UseSEGIDasChain == 1) /*use segid instead of chainid*/
    {
      sub[n][0] = segid[n][0];
      sub[n][1] = segid[n][1];
      sub[n][2] = segid[n][2];
      sub[n][3] = segid[n][3];
      sub[n][4] = segid[n][4];
    }

    /*itext is the index of CR or LF, so is index of last actual character*/
    if(itext>=78) /*element name in columns 77,78 (index 76,77)  070520*/
    {
       for(j=0; j<=1; j++) {frags[j] = texts[j+76];}
       if(!isdigit(frags[0]) && !isdigit(frags[1]) && frags[1] != ' ')
       {
          element[n][0] = frags[0];
          element[n][1] = frags[1];
          element[n][2] = '\0';    /* NOT = frags[2] 070727*/
       }
       else {element[n][0] = '\0';}
    }
    else {element[n][0] = '\0';}

    atstyle[n] = 0; /*030920  e.g. may be a ghost*/
/*name[n],Anum[n],atom[n],res[n],sub[n],&num[n],rins[n],&x[n],&y[n],&z[n]*/
/*format(a6,a5,1x,a5,a3,a2,i4,a1,3x,3&8.3)*/
/*
0123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789
123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
atom      1  n   arg a   1      26.465  27.452  -2.490  1.00 25.18      4pti  89
atom      2  ca  arg a   1      25.497  26.862  -1.573  1.00 17.63      4pti  90
*/
/*THIS IS STUPID: RECORD NAME IS CALLED NAME !!!!, atom name is called atom*/
/*  --name*6 record designator: atom or hetatom recognized  */
/*  --Anum:a5 atom number: moved blindly to output file  */
/*  --atom*5 includes atomname*4 and alt-conformation-indicator*1,  */
/*  --res*3 residue name  */
/*  --sub*1 chain ID: subunit designator  */
/*  --num:i4 residue number  */
/*  --rins*1 inserted residue indicator  */
/*  --x,y,z coord of the atom  */
/*  --so pretty std brookhaven except atom name where there is lots of  */
/*  -- non-standard brookhaven around!  */
/*  --if this reading has an error inform and abort:  */
/*  --implies a non-standard format that must be dealt with some other way  */
/*
0123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789
123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
ATOM     23 1DH1 ARG     1      25.930  13.038 -12.118  1.00 34.80      5PTI  99
ATOM     24 2DH1 ARG     1      27.324  12.549 -11.100  1.00 34.71      5PTI 100
ATOM     25 1DH2 ARG     1      27.786  12.886 -15.076  1.00 33.93      5PTI 101
ATOM     26 2DH2 ARG     1      26.154  13.214 -14.269  1.00 34.53      5PTI 102
ATOM     27  N   PRO     2      33.738  14.173 -10.126  1.00 22.41      5PTI 103
ATOM     28  CA  PRO     2      34.897  13.603  -9.390  1.00 19.52      5PTI 104
ATOM     29  C   PRO     2      34.652  12.135  -9.374  1.00 17.70      5PTI 105
ATOM     30  O   PRO     2      33.583  11.537  -9.255  1.00 16.41      5PTI 106

123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
ATOM     42  C4    G A   2      18.394  24.893  21.964  1.00   .76      2BNA 129
ATOM     43  P     C A   3      24.155  24.981  19.297  1.00 12.43      2BNA 130
ATOM     44  O1P   C A   3      23.669  25.875  18.229  1.00 24.85      2BNA 131
ATOM     45  O2P   C A   3      25.623  24.813  19.359  1.00 25.42      2BNA 132
ATOM     46  O5*   C A   3      23.454  23.553  19.172  1.00  7.50      2BNA 133
ATOM   1783  C6    T C   2      32.489  59.119  12.719  1.00 17.52
ATOM   1784  P     A C   3      33.340  63.593   8.466  1.00 41.62
*/
 if ( !igotit )
 {
  sprintf(alertstr,"input data format problem, line blows mind:"CRLF"%s",texts);
  DoReportDialog(); /*need to have Ltest set to see this */ /*____DLOG*/
  exit(0);/*c*/
 }/*input failure*/

 else

 {/*special filter for difficult logic cases*/
     /* CompArgStr() in PUXMINIT is case insensitive*/
     /* re: isawater(): 1atom residue*/
     /*was: (CompArgStr(name[n],"atom",4)&&... ) */
     if(       CompArgStr(res[n],"hoh",3)
            || CompArgStr(res[n],"h2o",3)
            || CompArgStr(res[n],"wat",3)
            || CompArgStr(res[n],"dod",3)
            || CompArgStr(res[n],"d2o",3)
            || CompArgStr(res[n],"tip",3)  /*030920*/
            || CompArgStr(res[n],"tp3",3)  /*030920*/
       )
     {/*is a water...*/
        sprintf(name[n],"hetatm"); /*so over-rides any record type 050409*/
        residuetype = 'W'; /*061006*/
     }/*force water to be HETATM */

     else if( CompArgStr(name[n],"hetatm",6) )

     {/*suspect imbedded HETATM to be cryptic protein or nucleic atom*/
        /*char *strstr(cs,ct) return pointer to first occurrence of */
        /*string ct in cs, or NULL if not present*/

        /*unfortunately, prekin processes atoms as they come in, so this */
        /*decision is made atom-by-atom but is a residue thing!  */
/*
  sprintf(temps,CRLF"input atom %s res name: %s"CRLF,atom[n],res[n]);
  pkintextinsert(temps);
  adjusttext(2);
*/
        /*011028*/
        /*attempt at smart decision when to change embedded HETATM to ATOM */
        /* no name test, take anything of 050409, is not adequate,*/
        /*  fails miserably e.g. for the GFPs BUT decidemainside not fudged!*/
        /*050705 stay with special list of possibile residues names*/
        if(typechainfirstpass[level]=='P')
        {/*HETATM within chain identified as Protein */
           if(strstr(aaList, res[n]) )
           {/*recover unusual amino-acids*/
              sprintf(name[n],"atom  ");
           }
        }/*HETATM within chain identified as Protein */
        if(typechainfirstpass[level]=='N')
        {/*HETATM within chain identified as Nucleic Acid */
           if(strstr(naList, res[n]) )
           {/*recover unusual nucleotides*/
              sprintf(name[n],"atom  ");
           }
        }/*HETATM within chain identified as Nucleic Acid */

/*090223 DIRTY TRICK TO GET AROUND HETATM THAT SHOULD BE ATOM RECORDS 090223*/
        if(Lribnhet
           &&(typechainfirstpass[level]=='P' || typechainfirstpass[level]=='N'))
        {
           sprintf(name[n],"atom  ");
        }



        if(CompArgStr(res[n],"cyo",3))
        {/*special case, should use a general equivalence table*/
           /*for residues where res name different for only a few atoms*/
           sprintf(res[n],"cys");
        }
/*
  sprintf(temps,"now called %s res name: %s"CRLF,name[n],res[n]);
  pkintextinsert(temps);
  adjusttext(1);
*/
     }/*suspect imbedded HETATM to be cryptic protein or nucleic atom*/
 }/*special filter for difficult logic cases*/

 /*track nucleic acid O2prime existance to distinguish DNA vs RNA */
 if( strstr(naList, res[n]) ) /*060113*/
 {
    if(    atom[n][1]=='o' && atom[n][2]=='2'
        &&(atom[n][3]=='*' || atom[n][3]=='\'')  )
      {/* O2' defines RNA rather than DNA*/
         LRNA = 1; /*distinguish RNA vs DNA, later vote for this fragment*/
      }
 }

 /*element identification   070520: classic = instead of == caught 070727*/
 if(element[n][0] == '\0') /*not set by element name in cols 77,78 */
 {
  /*whatelement(char record[7], char atmname[6], char elem[3])*/ /*070520*/
    whatelement(name[n], atom[n], element[n]);  /*070520,070730*/
/*fprintf(stderr,"whatelement sets record== %s, atom== %s, element== %s\n",name[n],atom[n],element[n]);*/ /*070730*/
 }
/*else fprintf(stderr,"____element __is record== %s, atom== %s, element== %s\n",name[n],atom[n],element[n]);*/ /*070730*/

 /*-------------------ASPECT CODE SPECIAL MODIFIED RECORDS-------------------*/
 if(Naspectcode > 0 && Lallowaspectsin > 0)
 {/*expect aspect color code characters in particular columns*/
    aspectstr[n][0] = '(';
    for(j=1; j<=Naspectcode; j++)
    {
       aspectstr[n][j] = lctouc(texts[aspectcodecol[j]]);
    }
    aspectstr[n][j] = ')';
    aspectstr[n][j+1] = '\0';
 }
 else
 {
    aspectstr[n][0] = '\0'; /*so no aspects gives a null string*/
    /*a null string has no length as a %s field*/
 }
 /*___________________ASPECT CODE SPECIAL MODIFIED RECORDS___________________*/

/*
sprintf(temps,CRLF"input: n= %d"CRLF"%s"CRLF"interpreted:"
                 CRLF"%s|%s|%s|%s|%s|%d|%s|%f %f %f"CRLF
,n,texts,name[n],Anum[n],atom[n],res[n],sub[n],num[n],rins[n],x[n],y[n],z[n]);
  pkintextinsert(temps);
  adjusttext(5);
*/
}
/*___readbrkatmrecord()_____________________________________________________*/

/****evalatomrecord()********************************************************/
void   evalatomrecord(void)
{
  int  j=0;
  int  delmod=0;

  /*check for any change indicator that affects processing */
  /*set logicals */
  if (n == 1)
  {/*this may be the first atom of a residue */
    /*define name and subunit idicators of this first atom */
    for(j=0 ; j<=5 ; j++) oldnam[j] = name[n][j];
    {
       oldnam[6] = '\0';
    }
    oldsub[0] = sub[n][0];  /*char chainID  070926*/
    oldsub[1] = sub[n][1];  /*char chainID  070926*/
    oldsub[2] = '\0'; /*safety, does not impinge on nmrmodel#*/
    for(j=0 ; j< MAXmodchr ; j++) oldmod[j] = mod[n][j];


    delnam = 0;
    delsub = 0;
    if(first1 == 1) first1 = 0;
  }/*this may be the first atom of a residue */
  else
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
  {/*this may be another atom of a residue, or the first of the next residue */
    /*check for change of residue number or name or insertion code*/
    if(num[n] != num[n-1])
    {
       delnum = 1;
    }

    /*011208 sequence heterogeneity sc res change but same num*/
    /*if(     (res[n][0] != res[n-1][0]) || (res[n][1] != res[n-1][1]) */
    /*     || (res[n][2] != res[n-1][2]) ) */
    /*{ */
    /*   delres = 1; */
    /*} */

    if(rins[n][0] != rins[n-1][0])
    {
       delnum = 1;
    }
  }/*this may be another atom of a residue, or the first of the next residue */
  /*check if record type, atom or hetatm, has changed*/
  if(name[n][0] != oldnam[0])
  {
     delnam = 1 ; /*either atom or hetatm: a or h */
  }
  /*check if subunit code character has changed*/
  if(  (sub[n][0]  != oldsub[0])  /*char chainID  070926*/
     ||(sub[n][1]  != oldsub[1])) /*char chainID  080318*/
   /*&&(sub[n][1]  != oldsub[1]))*/ /*char chainID  070926*/
  {
     delsub = 1 ;
  }
  if(nmrmodel[0] != '\0') /*011029*/
  {/*check if MODEL code has changed*/
    for(j=0; j < MAXmodchr; j++)
    {  /*change of MOD # should trigger endsub anyway ???? */
      if(mod[n][j] != oldmod[j])
      {
        delsub = 1;
        delmod = 1;
        delmod = delmod; /*to avoid compiler error when print commented out*/
        break; /*one mismatch does it*/
      }
    }
  }/*check if MODEL code has changed*/
  /*check conditions from included pdb records*/
  /* if an END recoord had been reached just previous to this ATOM then */
  /* presume change to a new subunit of a multi-subunit molecule */
  if (endsub) delsub = 1;
  if(Lresiduegroups && (delres || delnum) ) /*060120 */
  {/*e.g. each ATOM residue in separate group for multi-poses of ligands */
     delsub = 1;
  }
  if (delsub)
  {
    morsub = 1;

    nexsub[0] =  sub[n][0];  /*2char chainID 070926*/
    nexsub[1] =  sub[n][1];  /*2char chainID 070926*/
    nexsub[2] = '\0';
    for(j=0 ; j< MAXmodchr ; j++) nexmod[j] = mod[n][j];

    endsub = 1;
    endchn = 1;
    endres = 1;
    resinloop = 1; /* declare at end of a residue */
  }
  if(resinloop == 1) return;/* & break from readinloop: effective goto 160*/

  /* if a TER record had been reached just previous to this ATOM then */
  /*-- presume chain break just before here */
  if (endchn == 1)
  {
     delnam = 1;
  }
  if (delnam == 1)
  {
    endchn = 1;
    endres = 1;
    resinloop = 1; /* declare at end of a residue */
  }
  if(resinloop == 1) return; /* & break from readinloop: effective goto 160 */
  if (delres || delnum)
  {
    endres = 1;
    resinloop = 1; /* declare at end of a residue */
  }
  if(resinloop == 1)
  {/*store last atom as atom of next residue & break from readinloop 030515*/
     Latomnext = 1;
     xnext = x[n];
     ynext = y[n];
     znext = z[n];
     onext = o[n];
     Bnext = B[n];
     Uvalnext = Uval[n];
     strcpy(Anumnext,Anum[n]); /*hybrid36 071001 replace ncntnext = ncnt[n]*/
     numnext  = num[n];
     for(j=0; j<= 5; j++){atomnext[j] = atom[n][j];}
     for(j=0; j<= 3; j++){resnext[j] = res[n][j];}
     for(j=0; j<= 6; j++){namenext[j] = name[n][j];}
     for(j=0; j<  3; j++){subnext[j] = sub[n][j];} /*2char 070926*/
     for(j=0; j<= 1; j++){rinsnext[j] = rins[n][j];}
     for(j=0; j< MAXmodchr; j++){modnext[j] = mod[n][j];}
     for(j=0; j< MAXaspectchars+3; j++){aspectstrnext[j] = aspectstr[n][j];}
  }
  return;
}
/*__evalatomrecord()________________________________________________________*/

/***screenaltconf()**********************************************************/
void   screenaltconf(void)
{
   int  ica;
/*--screen for alternate conformation letters at end of atoms: */
/*-- NOTE a non-std brk format uses this position for #s: this is weak here!*/
/*-- alternate conformations designation: set althed  */
/*--define alt residue flag for the current atom */
      if(atmcpy[4] != ' ') {Lalts = 1;} /*at least one alt conf in current res*/
      ica = 0; /*any atom but C-alpha*/  /*930123*/
      if(   atmcpy[0] == ' ' && atmcpy[1] == 'c'&& atmcpy[2] == 'a'
         && atmcpy[3] == ' ') ica = 1; /*C-alpha*/
      /*althed[0] = ' ';*/
      althed[0] = atmcpy[4];
      if     ( atmcpy[4] == ' ' ){ altstd = 1;althed[0] = atmcpy[4];}
      else if( atmcpy[4] == 'a' )
         { altcfa[ica] = 1;althed[0] = atmcpy[4]; Lpointmasteralta=1;}/*030503*/
      else if( atmcpy[4] == 'b' )
         { altcfb[ica] = 1;althed[0] = atmcpy[4]; Lpointmasteraltb=1;}
      else if( atmcpy[4] == 'c' )
         { altcfc[ica] = 1;althed[0] = atmcpy[4]; Lpointmasteraltc=1;}
      else if( atmcpy[4] != ' ' )
      {
        /*if(atmcpy[1] != 'h')*/  /* sometimes h have # in this field*/
        if(!isahydrogen(name[natom],atmcpy,element[natom])) /*070520*/
        {/*--more than three alternate conformations or something else here:*/
            /*FRODO has wierd name for eta, e.g. arg neh1 instead of nh1*/
            if(atmcpy[2] == 'e'&&atmcpy[3] == 'h')
            {/*presume FRODO name and correct it */
                atmcpy[2] = 'h';
                atmcpy[3] = atmcpy[4];
                atmcpy[4] = ' ';
                altstd = 1;/*score as standard since might be FRODO file*/
                atom[natom][2] = atmcpy[2];
                atom[natom][3] = atmcpy[3];
                atom[natom][4] = atmcpy[4];
            }
            else
            {
              /*sprintf(alertstr,"wierd alternate conf flag: %s %s %s %d"CRLF*/
              /*     ,name[natom],atom[natom],res[natom],num[natom]);*/
              /*pkintextinsert(alertstr);*/  /*PKMCTEXT.C PKPCTEXT.C*/
              /*adjusttext(1);           */  /*PKMCTEXT.C PKPCTEXT.C*/
                altstd = 1;/*score as standard anyway*/
                /*for hook-up to previous residue anyway since logic for that*/
                /* hook-up is convoluted enough with 3 possible conformations*/
                /*However, let it try for matched hook-up within a residue*/
                althed[0] = atmcpy[4];
            }
        }
        else /*for a hydrogen...*/
        {
          /* presume a # for multiple h on one heavy atom*/
          altstd = 1;
        }
      }
}
/*__screenaltconf()_________________________________________________________*/

/****checkfocus()************************************************************/
void   checkfocus(void)
{
  float dist,distatom,distca,distmc,xsum,ysum,zsum;
  int    j;

  /*five primary output controls for mc sc ca ht wa */
  /*mc sc ca handled here, ht and wa in separate routines*/
  /*These five control all output except for ribbons*/
  /*ribons are NOT limited to the focus distance*/

  /*normally AND with range controls*/
  /*normally expect to do another pass to do OR function*/

  if(mrange == 0)
  {
    /*if there is no other range controls in operation*/
    /*prohibit everything, except the primary outl's */
    /*and any desired sub-outl's to appear in pure focus output*/
    /*first prohibit everything mentioned in rangecontrol*/
    rangel = 0; mcoutl = 0; scoutl = 0; lboutl = 0; croutl = 0;
    htoutl = 0; hyoutl = 0; hboutl = 0; caoutl = 0; waoutl = 0;
    atoutl = 0; mcaoutl = 0; htaoutl = 0; scaoutl = 0; waaoutl = 0;
    ssscoutl = 0; rotoutl = 0; mutoutl = 0; riboutl = 0; extoutl = 0;
    mccoutl = 0; sccoutl = 0; htcoutl = 0;
    mcCPKoutl = 0;scCPKoutl = 0;htCPKoutl = 0;
    Bcoloutl = 0,Bvaloutl = 0;
    /*then allow the primary outl's*/
    mcoutl = 1; scoutl = 1; caoutl = 1; htoutl = 1; waoutl = 1;
    /*and the desired dependent secondary outl's*/
    /*none chosen 990627*/
  }
  if(Lresnumfovea && (name[maxatm][0] == 'a'))
  {/*lresnumfovea first pass to compute an x,y,z for selected residue*/
      /*check for residue number only for this pass*/
      if(   (resnumfovea == num[maxatm])
         && (  (focuschaincharcode[1]==' '          )/*chainID right justified*/
             ||(focuschaincharcode[0]==sub[maxatm][0]) /*2char 070926*/
             ||(focuschaincharcode[1]==sub[maxatm][1]) /*2char 070926*/
            )
        )
      {/*compute x,y,z for a FOVEA */
          /*found residue number, stop here no matter what*/
          Lfoundresnum = 1;
          finishres = 1;
          endfil = 1;
          endsub = 1;

          /*find average coord of this residue*/
          xsum = 0.0;
          ysum = 0.0;
          zsum = 0.0;
          for(j=1 ; j<=maxatm ; j++)
          {
               xsum = xsum + x[j];
               ysum = ysum + y[j];
               zsum = zsum + z[j];
          }
          focus[1] = xsum/maxatm;
          focus[2] = ysum/maxatm;
          focus[3] = zsum/maxatm;
      }
  }/*lresnumfovea*/
  else
  {/*actual fovea run*/
      /*-- use all atoms for distance checks*/
    dist   = 999.0; /*start way out of any range*/
    distmc = 999.0;
    distca = 999.0;

    for(j=1 ; j<=maxatm ; j++)
    {/*compute distance to focus point*/
        distatom = (float)distan(x[j],y[j],z[j],focus[1],focus[2],focus[3]);
           if( (name[maxatm][0] == 'h') && (distatom < dist) )
           {/*hetatm records*/
              if( isawater() )
              {/*special case for water, only use oxygen for distance*/
                 if(atom[j][1] == 'o') dist = distatom; /*should be only one*/
              }
              else
              {
                 dist = distatom; /*keep minimum distance*/
              }
           }
           else
           {/*atom records*/
              if( decidemainside(j) )
              {/*mainchain*/
                 if(distatom < distmc)
                 {
                    distmc = distatom; /*keep minimum distance*/
                 }
                 if(atom[j][1] == 'c' && atom[j][2] == 'a')
                 {
                    distca = distatom; /*keep ca distance*/
                 }
              }
              else
              {/*sidechain*/
                 if(distatom < dist) dist=distatom; /*keep minimum distance*/
              }
           }
    }/*compute distance to focus point*/

    if(name[maxatm][0] == 'a')
    {/*atom record*/
       if( (distmc <= focusmc) && mcoutl)
       {
          mcoutl=TRUE;
       }
       else
       {
          mcoutl=FALSE;
          mcaoutl=FALSE; /*just in case*/
       }
       if( (distca <= focusca) && (caoutl || caonly) ) caoutl=TRUE;
       else
       {
          caoutl=FALSE;
       }
       if( (dist   <= focussc) && scoutl)
       {
          scoutl=TRUE;
       }
       else
       {
          scoutl=FALSE;
          scaoutl=FALSE; /*just in case*/
       }
    }/*atom record*/
    else
    {/*some type of hetatm*/
       if( isawater() )
       {/*water*/
          if( (dist <= focuswa) && waoutl) waoutl=TRUE;
          else
          {
             waoutl=FALSE;
             waaoutl=FALSE; /*just in case*/
          }
       }/*water*/
       else
       {/*other het group, not a water*/
          if( (dist <= focusht) && htoutl)  htoutl=TRUE;
          else
          {
             htoutl=FALSE;
             htaoutl=FALSE; /*just in case*/
          }
       }/*other het group, not a water*/

    }/*some type of hetatm*/

    if(scoutl || mcoutl || caoutl || htoutl || waoutl) rangel = TRUE;

  }/*actual fovea run*/
}
/*___checkfocus()___________________________________________________________*/

/****connecthydrogenbyname()*************************************************/
void   connecthydrogenbyname(void) /*130318dcr alt and name atom match*/
{/*connecthydrogen rewritten 130318 to connect a hydrogen atom by name*/
  /*fall back to closest distance connect if names not recognized*/

  float  dist,hatdst,distminimum;
  int    match,theatm,ipass,LOK;
  char   positionchar;
  int    branchnumber;
  int    baseatm; /*remember theatm number for diagnostics after loop ends*/

  /*note: for historic reasons, prekin uses lower case atom names*/
  /*re: PKINCSUB/readbrkatmrecord() */
  /*standard Brk PDB: 2 char element right adjusted digits 1&2 of atom name*/
  /*NB this captures Alt character as 5th atom char; i.e. index 4 */
  /*position out side chain: (A,B,G,D,E,Z,H) is 3rd atom char, i.e. index 2*/
  /*--at least for heavy (not-a-hydrogen) atoms */
  /*For H: H in 2nd atom char, 3rd atom char is it's own branch==twig */
  /*       H in 1st atom char, 3rd atom char is parent atom's branch  */
  /*        and                4th atom char is it's own branch==twig */
  /*       only match parent branch !! */ 

  /*--start with reference distance longer than willing to count as match */
  hatdst = 2.0;
  distminimum = 0.7; /*hydrogen to parent minimum distance*/
  positionchar = ' ';
  branchnumber = 0;
  match = 0;
  baseatm = 0;
  LOK = 1; /*presume OK to make connection*/
  
  if(atom[natom][1] == 'q') /*should not be able to get here with this...*/
  {
    /*--make dot for NMR pseudo atoms, i.e. original atom name is 'q' */
    base[natom] = natom;
    LOK = 0;
  }

  if(LOK) /*atom[natom]==H, but determine branch vs twig number by position*/
  { 
    /*first check register of atom name in its character string */
    if((atom[natom][0] == 'h')||(atom[natom][0] == 'd'))
    {/*0123 H?## left adjusted, gives room for parent branch number*/
      /*parent branch at index 2, hydrogen's own twig number is at index 3 */
      positionchar = atom[natom][1];
      if(isdigit(atom[natom][2])) /*branch number of parent*/
          {branchnumber = atom[natom][2];}
      else{branchnumber = 0;}
      LOK = 1;
    }
    
    else if((atom[natom][1] == 'h')||(atom[natom][1] == 'd'))
    {/* index: 0123 _H## _HX# _H#_ branch/twig ambiguous number */
     /*--3 cases depend on residue and position on residue: */
     /*hydrogen mainchain, NO branch, hydrogen's own twig number is at index 2*/
     /*parent branch NOT given, hydrogen's own twig number is at index 3 */
     /*parent branch is given, hydrogen's parent branch number is at index 3 */
     /* case: nterm NH3: hydrogen branch is blank, like blank branch on mc N */
   /*Tyr _CD1,_CD2,_CE1,_CE2  vs _HD1,_HD2,_HE1,_HE2  */
   /*Trp _CD1,_CD2,_NE1,_CE2,_CE3,_CZ2,_CZ3,_CH2:_HD1,_HE1,_HE3,_HZ2,_HZ3,_HH2*/
   /*Arg ...*/

      if(  (isdigit(atom[natom][2]))&&(atom[natom][2]>0)
         &&(!isdigit(atom[natom][3])) )  /*i.e. digit,blank*/
      {/* [2] likely is the twig number for, e.g. mc hydrogen e.g. -NH3 */
         branchnumber = atom[natom][2]; /*where this is probably twig number*/
         positionchar = ' '; /*for, e.g. mainchain */  
      } 
      else if(  (isdigit(atom[natom][2]))&&(atom[natom][2]>0)
         &&(isdigit(atom[natom][3])) )  /*i.e. digit,digit*/
      {/* [2] likely is a parent number for, e.g. hetatom varient */
         branchnumber = atom[natom][3]; /*where this is probably twig number*/
         positionchar = atom[natom][2]; /* e.g. het atom position number */  
      } 
      else if( !isdigit(atom[natom][2]) ) 
      {/*index [2] has a non-digit character, likely parent position*/
      
         positionchar = atom[natom][2]; /*e.g. residue parent branch char*/ 
         if( isdigit(atom[natom][3]) )
         {
            branchnumber = atom[natom][3]; /*e.g. residue H twig number*/
         }
         else
         {
            branchnumber = 0; /*atom name pattern wierd*/
         }
      }
      /*since parent branch maybe NOT given... */
      /* if((isdigit(atom[natom][3]))&&(atom[natom][3]>0)) */
      /* {*//*hydrogen's own twig OR parent's branch*/
      /*    branchnumber = atom[natom][3];*/ /*in case parent has a branch*/
      /* }*/
      else /*give up...*/
      {
         positionchar = ' ';
         branchnumber = 0;
      }
      LOK = 1;
    }
  }

  if(LOK)
  {/*LOK*/
    ipass = 1;   /*970422*/
    for(ipass = 1;(ipass <= 5)&&(match==0); ipass++)
    {/*5 passes 130323, include Jane's at-least-match-alt-chars(if both given)*/
     /* 1 try to match alternate conformations of both H and parent*/
     /* 2 try to match H alternate conformations to non-alt parent atom*/
     /* 3 try to match non alt H conformations to alt A parent atom 130323*/
     /*   cases 1 & 2 & 3 are legal name combinations */
     /* 4 at least match H alt characters if both given*/
     /* 5 try just connect H to nearest heavy atom */
     /*   case 3 is OK by old rules, cases 4 & 5 are pathological */
      for( theatm=1 ; theatm <= maxatm ; theatm++ )
      {/*LOOP theatm = 1,maxatm*/
        /*--skip hydrogens and pseudo-hydrogens named 'q' */
        if(!isahydrogen(name[theatm],atom[theatm],element[theatm]))
        {/*connect h to non-hydrogen atoms*/
          /*--check distance */
/*s*/     dist = (float)distan(x[natom],y[natom],z[natom]
                              ,x[theatm],y[theatm],z[theatm]);
          if(dist <= hatdst)
          {/*close enough*/ 
            /*--check for alternate conformation designator */
            if(ipass==1)
            {/*pass 1: try to exactly match alternate conformation*/
              if(  (atom[theatm][4] == althed[0])
                 &&(atom[theatm][2] == positionchar))
              {/*alt indicators and position chars match*/
                /*good so far... check on possible branch...*/
                { 
                  match = 1;
                  if(  ( (isdigit(atom[theatm][3])) && (atom[theatm][3] > 0) )
                     &&(atom[theatm][3] != branchnumber) )
                  {/*if putative parent at correct position is wrong branch*/
                     match=0;
                  }
                  if(match)
                  {
                     base[natom] =  theatm;
                     baseatm = theatm;
                     hatdst = dist;
                     break; /*should be correct match by names and alt chars*/
                  }
                }
              }/*alt indicators and position chars match*/
            }/*pass 1: try to exactly match alternate conformation*/
            else if(ipass==2)
            {/*2: try to match alternate H to non-alt heavy atom at position*/
              if((atom[theatm][4] == ' ') && (atom[theatm][2] == positionchar))
              { 
                match = 2;
                base[natom] =  theatm;
                baseatm = theatm;
                hatdst = dist;
                break; /*as good as be can done by match by names*/
              }
            }/*2*/
            else if(ipass==3)
            {/*3: try to match non-alt H to alt A heavy atom at position*/
              if(  (atom[theatm][2] == positionchar)
                 &&(atom[theatm][4] == 'a')&&(althed[0]==' ') )
              { 
                match = 3;
                base[natom] =  theatm;
                baseatm = theatm;
                hatdst = dist;
                break; /*as good as be can done by match by names*/
              }
            }/*3*/
            else if(ipass==4) /*failed clean match*/
            {/*4: match-alt-chars if both given & closest but not too close*/
              if( (atom[theatm][4] == althed[0])&&(dist > distminimum) ) 
              { 
                match = 4;
                base[natom] =  theatm;
                baseatm = theatm;
                hatdst = dist;
                continue; /*cycle matching alt chars for closest above minimum*/
              }
            }/*4*/
            else if(ipass==5) /*desperate attempt...*/
            {/*5: try to match H to closest heavy atom*/
              { /*here a TOO CLOSE distance makes a problem more obvious*/
                match = 5; /*take each in turn ---*/
                base[natom] =  theatm;
                baseatm = theatm;
                hatdst = dist; /*--looking for closest---*/
                continue; /*--keep cycling to get shortest connection--*/
              }
            }/*5*/
          }/*close enough*/ 
        }/*connect h to non-hydrogen atoms*/
      }/*LOOP theatm = 1,maxatm*/ /*BEWARE theatm incremented when loop ends*/
      /*baseatm == theatm at time of match*/
    }/*5 passes*/
    if((!LOK)||(match == 0))
    {/*!OK*/
      /*--did not find a base for this hydrogen: print message and continue */
       sprintf(alertstr,"no base for hydrogen: %s %s %s %d"CRLF
               ,name[natom],atom[natom],res[natom],num[natom]);
       pkintextinsert(alertstr);  /*PKMCTEXT.C PKPCTEXT.C*/
       adjusttext(1);             /*PKMCTEXT.C PKPCTEXT.C*/
      /*--make a dot so have an entry in the output: */
      base[natom] = natom;
if(Lreport)
{
      fprintf(stderr,"ORPHANdot hydrogen:(#%d) %s %s %s %d\n"
              ,natom,name[natom],atom[natom],res[natom],num[natom]);
} 
   }/*!OK*/
    else if(match > 3)
    {/*report imperfect match to terminal using standard error */
     if(match == 4)
     {
if(Lreport)
{
      fprintf(stderr,"uncertain hydrogen:(#%d) %s %s %s %d\
  to parent:(#%d) %s %s %s %d\n"
              ,natom,name[natom],atom[natom],res[natom],num[natom]
              ,baseatm,name[baseatm],atom[baseatm],res[baseatm],num[baseatm]);
}
     }
     else /*report imperfect match to terminal using standard error */
     {
if(Lreport)
{
      fprintf(stderr,"desperate hydrogen:(#%d) %s %s %s %d\
  to parent:(#%d) %s %s %s %d\n"
              ,natom,name[natom],atom[natom],res[natom],num[natom]
              ,baseatm,name[baseatm],atom[baseatm],res[baseatm],num[baseatm]);
}
     }
    }
  }/*LOK*/
}
/*___connecthydrogenbyname()_________________________________________________*/      

/****connecthydrogen()*******************************************************/
void   connecthydrogen(void)
{              /*connect a hydrogen atom*/
  float  dist,hatdst;
  int    match,theatm,ipass;

  /*--start with reference distance longer than willing to count as match */
  hatdst = 2.0;
  match = 0;

  if(atom[natom][1] == 'q')
  {
    /*--make dot for NMR pseudo atoms, i.e. original atom name is 'q' */
    base[natom] = natom;
  }
  else
  {
    ipass = 1;   /*970422*/
    for(ipass = 1;(ipass <= 2)&&(match==0); ipass++)
    {/*two passes, one try to match alternate conformations, 2 just connect*/
      for( theatm=1 ; theatm <= maxatm ; theatm++ )
      {
        /*--skip hydrogens and pseudo-hydrogens named 'q' */
     /*if(atom[theatm][1] !='h'&&atom[theatm][1] !='d'&&atom[theatm][1] !='q')*/
        if(!isahydrogen(name[theatm],atom[theatm],element[theatm]))
        {/*connect h to non-hydrogen atoms*/
          /*--check distance */
/*s*/     dist = (float)distan(x[natom],y[natom],z[natom]
                              ,x[theatm],y[theatm],z[theatm]);
          if(dist <= hatdst)
          {
            /*--check for alternate conformation designator */
            if(ipass==1)
            {/*try to exactly match alternate conformation*/
              if(atom[theatm][4] == althed[0])
              {
                base[natom] =  theatm;
                match = 1;
                hatdst = dist;
                /*--keep cycling so get shortest connection */
              }
            }
            else if(ipass==2)
            {/*try to match alternate H to standard heavy atom conformation*/
              if(atom[theatm][4] == ' ')
              {
                base[natom] =  theatm;
                match = 1;
                hatdst = dist;
                /*--keep cycling so get shortest connection */
              }
            }
          }
        }/*connect h to non-hydrogen atoms*/
      }/*old  do 307 theatm = 1,maxatm  loop*/
    }/*two passes, one try to match alternate conformations, 2 just connect*/

    if (match == 0)
    {
      /*--did not find a base for this hydrogen: print message and continue */
/*c*/  sprintf(alertstr,"no base for hydrogen: %s %s %s %d"CRLF
               ,name[natom],atom[natom],res[natom],num[natom]);
       pkintextinsert(alertstr);  /*PKMCTEXT.C PKPCTEXT.C*/
       adjusttext(1);             /*PKMCTEXT.C PKPCTEXT.C*/
      /*--make a dot so have an entry in the output: */
      base[natom] = natom;
    }
  }
}
/*___connecthydrogen()______________________________________________________*/

/****connectheavyatom()******************************************************/
void   connectheavyatom(int iscan,int maxcounter)
{
  float  dist,unkdst,hethetdist,testdist,cnodist,mixdist,hetdist;
  int    match,j,k,itype,ipairok,jstart,jend,Ltrytoconnect,ica,Lneed,jj,nn;

  /*connect a heavy atom*/ /*--(scanning natom=1,maxatm)--*/
  /*only non-hydrogen atoms should get into this section */
  /*already have dealt with hydrogens and pseudo-hydrogens */

  /*generic distance based connector */
  /*doing  natom = 1,maxcounter, with atmcpy and althed defined */

  /*check if this atom should be connected to a previous residue */
  /*oldatm T only when an atom from previous residue stored in lastat */
  /* of arrays and endchn was not flagged when left that residue */
  /*NOTE 061001 endchn flag not robust for this purpose*/
  /*only try this once, and only for atoms, not hetatms*/
  /*NOTE 061001 flaw at chain start: some nucl chains start with O3'... */
  /*need to protect the P from connecting back to earlier chain res*/

  if(icountres[level] == 0 && natom == 1 ) /*061001*/
  {/*first atom of a new chain, no way to connect back, only thing to do is */
     base[natom] = natom; /*default to a dot */
  }
  /* only look back within an ATOM chain for certain named atoms 061001*/
  /*call oldcon even for first atom of first residue for report 061202*/
  /*  i.e. used to be else if( ... >0)... : */
  /*-------------------------------------------------------------------------
   * if  (  (icountres[level] >= 0 && iscan == 1 && name[natom][0] == 'a')
   *      &&(  (    atom[natom][0] == ' ' && atom[natom][1] == 'n'
   *             && atom[natom][2] == ' ' && atom[natom][3] == ' ')
   *         ||(    atom[natom][0] == ' ' && atom[natom][1] == 'p'
   *             && atom[natom][2] == ' ' && atom[natom][3] == ' ') ) )
   ------------------------------------------------------------------------*/
  /* 061202 also try to connect hetatm within P or N chains */
  /* 070801 GFPchromophore: try _n1_ ... */

  if    (  (icountres[level] >= 0 && iscan == 1)
         &&(  (name[natom][0] == 'a')
            ||(  (typechainfirstpass[level]=='P')
               ||(typechainfirstpass[level]=='N') ) )
         &&(  (   atom[natom][0] == ' ' && atom[natom][1] == 'n'
               && atom[natom][2] == ' ' && atom[natom][3] == ' ')
            ||(   atom[natom][0] == ' ' && atom[natom][1] == 'p'
               && atom[natom][2] == ' ' && atom[natom][3] == ' ')
            ||(   LGFPchromophore
               && atom[natom][0] == ' ' && atom[natom][1] == 'n'
               && atom[natom][2] == '1' && atom[natom][3] == ' ') ) ) /*070801*/
        /*so _n1_ e.g. GFPchromophore only if _n__ not yet encountered  070801*/
  {/*find nominal beginning atom of a residue: n or p */
    if(!supoutl) /*NOT super impose of residue type at 0,0,0*/
    {
       ica = 0; /* NOT ca-ca type pseudobackbone*/

       oldcon((int)natom,(float)bondst,(int)ica); /*PKINCSBS.c*/
         /*find base for new atom at natom at <= bondst distance, ica == 0 */
         /*bondst (distance limit) avoids aa n connection to tRNA o3' */
       /*--base defined or defaulted to dot in oldcon, loop goes to next atom */
    }
  }/*find nominal beginning atom of a residue: n or p */
  else
  {/*interior atom of residue (or first atom if not n or p)*/

    /*special case of protein residue attached at end of nucleic e.g. tRNA*/
    if(   residuetype=='P' && previousresiduetype=='N'
       && atom[natom][0] == ' ' && atom[natom][1] == 'c'
       && atom[natom][2] == ' ' && atom[natom][3] == ' ')
    {/*nucleic to protein residue, connect to extra carbonyl carbon  061006*/
       creatm(natom); /*duplicate at ++maxatm*/
       oldcon((int)maxatm,(float)bondst,(int)ica); /*PKINCSBS.c*/
    }

    /*first check against all atoms behind, never looks at self, */
    /*then check against all atoms ahead, refuse connection for out-of-order*/
    /* if there is a preferred order specified */

    /*to allow ring closure, have to allow multiple hits, not just make */
    /*connection to minimum distant atom */
    /*------- */
    /*set a distance above which atoms should not be bonded */
    /*a bond will be made to the shortest distance less than that */
    cnodist = 2.0;    /* e.g. for c-c, c-n, c-o */
       /*hetdist=(float)2.1;*/
    hetdist=(float)2.0;/*often het groups are not as well determined, esp. cho*/
                 /*991203*/
    mixdist = (float)2.2;    /* e.g. for Fe-N */
    hethetdist = 2.5; /* e.g. for Fe-S */
      /* e.g. heme: Fe-N is slightly > 2.0 ,     */
      /*   while close non-bonded c...c is 2.21  */
      /* unfortunately, this lets carboxyl oxygens connect ... */
      /* so no peroxides, etc. allowed for hetatms either! */
    if(name[natom][0] == 'a')   unkdst = cnodist;  /*atom: residue*/
    else            unkdst = hetdist;  /*hetatm*/
    testdist = unkdst; /*so this is the default for current type of group */

    match = 0;

    /*first atom where natom=1 is a special case, if it hasn't been given */
    /*something from a previous residue to connect to, it will come */
    /*through here, where it has nothing behind it to look at, so can only */
    /*scan for first atom looking ahead */

    if(iscan == 1 && natom == 1)
    {
      base[natom] = natom; /*default to a dot, it might find something later*/
                  /* dot will be overridden if find another connection */
    }
    else
    {/*scan*/
      k = 0;  /* counter for connections made to this atom */
      if(iscan == 1 && natom > 1)
      /* full specification protection from program mutations*/
      {
        jstart = 1;
        jend = natom-1;
        Ltrytoconnect = 1;
      }
      else if(iscan == 2 && natom < maxcounter)
      {
        jstart = natom + 1;
        jend = maxcounter;
        Ltrytoconnect = 1;
      }
      else
      {
        Ltrytoconnect = 0;
        match = 1;  /*fake a match so don't mess with this atom's base */
      }

      if(Ltrytoconnect == 1)
      {/*Ltrytoconnect==1*/
       for( j=jstart ; j <= jend ; j++ )
       {/*scan over j for base atom*/

#ifdef UNIX_X11
if(Ltestprint)
{
  fprintf(stderr,"atom #%d %s %s %d  looking at #%d %s %s %d\n"
,natom,atom[natom],res[natom],num[natom],j,atom[j],res[j],num[j]);
}
#endif
        /*do some name checking to avoid wierd hook-ups */
        /*if the coords are off */

        itype = 1; /* OK to proceed */

        /*hydrogens have already been screened out for prime atom */

        /*hydrogens not allowed to have anything bind TO them so */
        /*if(    atom[j][1] =='h' || atom[j][1] =='d' || atom[j][1] =='q' */
        /*   || (atom[j][0] =='h'&&isdigit(atom[j][2])&&isdigit(atom[j][3])) )*/
        if(isahydrogen(name[j],atom[j],element[j])) /*inside connectheavyatom*/
        {/*H incl cns H 030920*/
             itype = 0;
        }
        /* presume no O-O peroxide links in a protein, avoids bad carboxyls */
        if(name[natom][0]=='a' && atom[natom][1]=='o' && atom[j][1]=='o')
             itype = 0;

        /* until get rational atom radii, ... */
        if(name[natom][0]=='h' && atom[natom][1]=='o' && atom[j][1]=='o')
             itype = 0;

        /*mutated prolines get an extra NE at the coord of mc N */
        if(  name[natom][0]=='a'
           && atom[natom][1]=='n' && atom[natom][2]=='e'
           && atom[j][1]=='c' && atom[j][2]=='a') { itype = 0; }
        if(  name[natom][0]=='a'
           && atom[natom][1]=='c' && atom[natom][2]=='a'
           && atom[j][1]=='n' && atom[j][2]=='e') { itype = 0; }
        if(  name[natom][0]=='a'
           && atom[natom][1]=='n' && atom[natom][2]==' '
           && atom[j][1]=='n' && atom[j][2]=='e') { itype = 0; } /*030605*/
        if(  name[natom][0]=='a'
           && atom[natom][1]=='n' && atom[natom][2]=='e'
           && atom[j][1]=='n' && atom[j][2]==' ') { itype = 0; } /*030605*/
        if(  name[natom][0]=='a'
           && atom[natom][1]=='n' && atom[natom][2]=='e'
           && atom[j][1]=='n' && atom[j][2]=='e') { itype = 0; } /*030605*/
        if( base[j] == natom )
        {
          itype = 0;
          /* non-trivial connection already made in reverse order */
          /* which had been screened to be in the preferred order */
          /* if a preferred order is defined for this pair */
          match = 1;
        }

        if(itype == 1)
        {/*try to connect a heavy atom to a heavy atom */
          /*althed has been defined for the prime atom*/
          /*so only need to check that alt. atom designator of this one same*/
          /*or blank so alt conf atom can bind back to common root atom */

          if(  (atom[j][4] == atmcpy[4] || atom[j][4] == ' ' )
             ||(atmcpy[4] == ' ')   /*common connects back to all alts*/
            )
          {/*correct alternate if any*/
            /*--check distance*/

          dist=(float)distan((double)x[natom],(double)y[natom],(double)z[natom]
                            ,(double)x[j],(double)y[j],(double)z[j]);


            if(  atom[natom][1]!='c'&&atom[j][1]!='c'
               &&atom[natom][1]!='o'&&atom[j][1]!='o'
               &&atom[natom][1]!='n'&&atom[j][1]!='n') testdist = hethetdist;
            else if(    (  atom[natom][1]=='c'
                         ||atom[natom][1]=='o'
                         ||atom[natom][1]=='n')
                     && (  atom[j][1]!='c'
                         &&atom[j][1]!='o'
                         &&atom[j][1]!='n')
                   ) testdist = mixdist;
            else if(    (  atom[natom][1]!='c'
                         &&atom[natom][1]!='o'
                         &&atom[natom][1]!='n')
                     && (  atom[j][1]=='c'
                         ||atom[j][1]=='o'
                         ||atom[j][1]=='n')
                   ) testdist = mixdist;
            else testdist = unkdst;
                /*either cnodist or hetdist depending on group*/
            if(dist <= testdist)
            {/*dist ok*/

              ipairok = 1; /* presume a valid pair */
              /*use atom names to screen for preferred connection direction*/

              /*for atom type residues*/
              if(name[natom][0] == 'a') ipairok = (int)screenatompair(j);
                                                          /*PKINCSBS.c*/
              /*for hetatm type residues, do a more complicated screen*/
              if(name[natom][0] == 'h') ipairok = (int)screenhetpairs(j);
                                                          /*PKINCSBS.c*/
              if(ipairok == 1)
              {/*ipairok*/
                match = 1;
                k = k + 1;

                if(k == 1 && base[natom] == natom)  base[natom] = 0;
                   /* override a dot*/
                if(k == 1 && base[natom] > 0)
                   /* already had a base assigned on the previous cycle*/
                {
                     if(base[natom] == j) k = 0;
                     /* already had THIS base assigned on the previous cycle*/
                     else k = 2;
                        /* different, probably ring closure */
                }
                if(k > 1)
                {/*k>1*/
                  /*this atom already has a base vector, before creating a*/
                  /*duplicate to receive a new connection, make sure that*/
                  /*this pair of atoms is not already represented as a vector*/
                  /*This pair is atom# natom and atom# j, but there may be */
                  /*atoms tacked on beyond maxcounter that are duplicates of*/
                  /*one or both of these, with their own specific connections*/
                  Lneed = 1; /*presume need this connection*/
                  if(maxatm > maxcounter)
                  {/*there already are additional tacked on atoms*/

                    for(jj=maxcounter+1; jj<=maxatm; jj++)
                    {/*check through the tacked on atoms*/
                      if(  (idupl[jj] == natom && base[jj] == j)
                         ||(idupl[jj] == j && base[jj] == natom)) {Lneed = 0;}
                    }/*check through the tacked on atoms*/
                  }/*there already are additional tacked on atoms*/
                  if(Lneed)
                  {/*need this connection*/
                   /*all atoms must have a base, then all atoms will be the*/
                   /*L part of a P L sequence and all atoms must be in the*/
                   /*L set which Mage then can output as a complete set*/
                   /*The trick is to tell Mage a complete and unique set*/
                   /*so extra cases of an atom as an L member are ghosts*/
                    /*multiple connections, need to make multiple vectors */
                    /*make sure there is room in the atom array for */
                    /*  another vector */
                    if (maxatm+1 >= lastat-8)
                    {  /*--no more room */

                       sprintf(alertstr,"array exceeded, extra connection"
                                    " not made from "
                                    "%s %s %d to: %s %s %d"CRLF
                               ,atom[natom],res[natom],num[natom],atom[j]
                               ,res[j],num[j]);
                       pkintextinsert(alertstr);  /*PKMCTEXT.C PKPCTEXT.C*/
                       adjusttext(1);             /*PKMCTEXT.C PKPCTEXT.C*/
                    }/*--no more room */
                    else
                    {/* got room */
                       maxatm = maxatm + 1;
                       tacker(maxatm,natom); /*PKINCSBS.c*/
                       /*     nput   nget   */
                       base[maxatm] = j;
                       idupl[maxatm] = natom;
                       atstyle[maxatm] = GHOST; /*030920 extra crt Mage output*/
                       /*keeps track of who this atom really is*/
#ifdef UNIX_X11
if(Ltestprint)
{
  fprintf(stderr,"scan %d multi- connect new # %.2d %s %s %d back to: # %.2d %s %s %d\n",iscan,maxatm,atom[maxatm],res[maxatm],num[maxatm],j,atom[j],res[j],num[j]);
}
#endif

                    }/* got room */
                  }/*need this connection*/
                }/*k>1*/
                else if( k == 1)
                {/*k==1*/
                   base[natom] = j;
#ifdef UNIX_X11
if(Ltestprint)
{
  fprintf(stderr,"scan %d single connect old # %.2d %s %s %d back to: # %.2d %s %s %d\n"
              ,iscan,natom,atom[natom],res[natom],num[natom],j,atom[j],res[j],num[j]);
}
#endif
                }/*k==1*/
              }/*ipairok*/
              else
              {
                ;
              }
            }/*dist ok*/
          }/*correct alternate if any*/
        }/*try to connect a heavy atom to a heavy atom */

       }/*scan over j for base atom*/
      }/*Ltrytoconnect==1*/
    }/*scan*/
    if(iscan == 2 && Ltrytoconnect ==1 && itype == 1 && match == 0)
    {
      if(base[natom] == 0)  base[natom] = natom;
      {/* nothing found in loop to connect, */
         if(base[natom] == 0)  base[natom] = natom; /*so make a dot*/
      }
    }
  }/*interior atom of residue (or first atom if not n or p)*/
/*
  j = base[natom];
  fprintf(stderr,"connectheavy: # %.2d %s %s %d back to: # %.2d %s %s %d\n"
              ,natom,atom[natom],res[natom],num[natom],j,atom[j],res[j],num[j]);
*/
}
/*__connectheavyatom()______________________________________________________*/
/*
ATOM    283  N   HIS E  57       6.644  75.220  21.180  1.00 17.50      2PTC 430
ATOM    284  CA  HIS E  57       7.150  76.215  20.220  1.00 17.50      2PTC 431
ATOM    285  C   HIS E  57       6.885  75.820  18.776  1.00 17.50      2PTC 432
ATOM    286  O   HIS E  57       6.934  76.707  17.900  1.00 17.50      2PTC 433
ATOM    287  CB  HIS E  57       8.621  76.713  20.439  1.00 17.50      2PTC 434
ATOM    288  CG  HIS E  57       9.666  75.626  20.166  1.00 25.18      2PTC 435
ATOM    289  ND1 HIS E  57      10.064  74.734  21.177  1.00 25.18      2PTC 436
ATOM    290  CD2 HIS E  57      10.269  75.276  19.021  1.00 25.18      2PTC 437
ATOM    291  CE1 HIS E  57      10.922  73.781  20.615  1.00 25.18      2PTC 438
ATOM    292  NE2 HIS E  57      11.051  74.132  19.270  1.00 25.18      2PTC 439
*/
/****storecyssg()************************************************************/
void   storecyssg(int maxcounter)
{/*store cyssg for possible ssbonds*/
    int    j;

    if( (ssknt + 1) < ssmax)
    {
        ssknt = ssknt + 1;
        sstapler(ssknt,natom);

        if(ssscoutl)
        {
            ssscall[ssknt] = 1;
            for(j=1; j<=maxcounter; j++) /*find corresponding ca & cb*/
            {
                if( (atom[j][1] == 'c' && atom[j][2] == 'a') &&
                    (atom[j][4] == ' ' || atom[j][4] == atom[natom][4]) )
                {
                     sscax[ssknt] = x[j];
                     sscay[ssknt] = y[j];
                     sscaz[ssknt] = z[j];
     /*also store Bvalue...*/
     if(Ncolorscalebasis==3)
        sscaBval[ssknt] = Uval[j]; /*Unusual field in  PDB format: col 67...72*/
     else if(Ncolorscalebasis==2)
        sscaBval[ssknt] = o[j]; /*occupancy factor,  or PDB field: col 55...60*/
     else
        sscaBval[ssknt] = B[j]; /*temperature factor in PDB field: col 61...66*/
                }
                else if( (atom[j][1] == 'c' && atom[j][2] == 'b') &&
                         (atom[j][4] == ' ' || atom[j][4] == atom[natom][4]) )
                {
                     sscbx[ssknt] = x[j];
                     sscby[ssknt] = y[j];
                     sscbz[ssknt] = z[j];
     /*also store Bvalue...*/
     if(Ncolorscalebasis==3)
        sscbBval[ssknt] = Uval[j]; /*Unusual field in  PDB format: col 67...72*/
     else if(Ncolorscalebasis==2)
        sscbBval[ssknt] = o[j]; /*occupancy factor,  or PDB field: col 55...60*/
     else
        sscbBval[ssknt] = B[j]; /*temperature factor in PDB field: col 61...66*/
                }
            }
        }
        else
        {
            ssscall[ssknt] = 0;
        }
    }
    else
    {
          sprintf(alertstr,"No room in cys sg array for:"
                        CRLF"%s|%s|%s|%s|%s|%s|%f %f %f"
                 ,name[natom],Anum[natom],atom[natom],res[natom]
                 ,sub[natom],rins[natom],x[natom],y[natom],z[natom]);
/*Ms*/    DoReportDialog();
    }

}
/*___storecyssg()___________________________________________________________*/

/****connecthetatms()********************************************************/
void   connecthetatms(void)
{
    int   maxorig,j,hetatEOF;
/*name[n],Anum[n],atom[n],res[n],sub[n],&num[n],rins[n],&x[n],&y[n],&z[n]*/
/*residue's atom record*/
/*extra char space in all storage char strings for \0  */
 char  thisname[7],thisatom[6],thisres[4],thissub[3],thisrins[2];
 char  thismod[MAXmodchr];
   /*local, but should match global use of model#s*/ /* 010529 */
 char   thisAnum[6];
 int    thisnum;
 float  thisx,thisy,thisz,thiso,thisB;

    maxorig = maxatm;
    for(n=1 ; n <= maxorig ; n++)
    {/*search for connections back to earlier hetatms from each atom*/
       rewindtextblock(&hetscratch);
       hetatEOF = 0;
       while (!hetatEOF)
       {/*Not at EOF*/
          getonetextblockline(&hetscratch,temps); /*line in temps[256]*/
          if(temps[0]=='\0') hetatEOF=1;
          if( !hetatEOF )
          {/*atom*/
             for(j=0;j<=5;j++) thisatom[j]=' ';
             for(j=0;j<=3;j++) thisres[j]=' ';
             /*read stored het atom info, inc model# */
             sscanf(temps,"%s %s %s %s %s %d %s %s %f %f %f %f %f",
                           thisname,thisAnum,thisatom,thisres,thissub
                          ,&thisnum,thisrins,thismod,&thisx,&thisy,&thisz
                          ,&thiso,&thisB
                  );
/*s*/        if ( (float)distan( x[n],y[n],z[n]
                                  ,thisx,thisy,thisz) <= 1.8)
             {/*originally 1.8 */
                /*set up another vector */
                if (maxatm+1 >= lastat-8)
                {/*--no more room */
/*c*/              sprintf(alertstr,"array exceeded, extra connection"
                        " not made from %s %s %d back to: %s %s %d"CRLF
                       ,atom[n],res[n],num[n],thisatom,thisres,thisnum);
                   pkintextinsert(alertstr);  /*PKMCTEXT.C PKPCTEXT.C*/
                   adjusttext(1);             /*PKMCTEXT.C PKPCTEXT.C*/
                }/*--no more room */
                else
                {/*got room*/ /*tacker operation*/

                   maxatm = maxatm + 1;
                   x[maxatm] = thisx;
                   y[maxatm] = thisy;
                   z[maxatm] = thisz;
                   o[maxatm] = thiso;
                   B[maxatm] = thisB;
                   num[maxatm]  = thisnum;

                   /*pad all char strings to full length of PDB format item*/
                   for(j=0 ; j<= 4 ; j++)
                   {/*0,1,2,3,4,5==\0*/
                      if(thisatom[j]=='_' || thisatom[j]=='\0') thisatom[j]=' ';
                   }
                   thisatom[5]='\0';
                   for(j=0 ; j<= 5 ; j++) atom[maxatm][j] = thisatom[j];
               /*-----*/
                   for(j=0 ; j<= 4 ; j++)
                   {/*0,1,2,3,4,5==\0*/
                      if(thisAnum[j]=='_' || thisAnum[j]=='\0') thisAnum[j]=' ';
                   }
                   thisAnum[5]='\0'; /*071001*/
                   for(j=0 ; j<= 5 ; j++) Anum[maxatm][j] = thisAnum[j];
               /*-----*/
                   for(j=0 ; j<= 2 ; j++)
                   {/*0,1,2,3==\0*/
                       if( thisres[j]=='_' || thisres[j]=='\0') thisres[j]=' ';
                   }
                   thisres[3]='\0';
                   for(j=0 ; j<= 3 ; j++) res[maxatm][j] = thisres[j];
               /*-----*/
                   for(j=0 ; j<= 5 ; j++)
                   {/*0,1,2,3,4,5,6==\0*/
                     if( thisname[j]=='_' || thisname[j]=='\0') thisname[j]=' ';
                   }
                   thisname[6]='\0';
                   for(j=0 ; j<= 6 ; j++) name[maxatm][j] = thisname[j];
               /*-----*/
                   for(j=0 ; j< 2 ; j++) /*2char chainID 070926*/
                   {/*0,1,2==\0*/
                     if( thissub[j]=='_' || thissub[j]=='\0') thissub[j]=' ';
                   }
                   thissub[2]='\0'; /* was 1 071222 */
                   for(j=0 ; j<= 2 ; j++) sub[maxatm][j] = thissub[j];
               /*-----*/
                   for(j=0 ; j<= 1 ; j++)
                   {/*0,1==\0*/
                     if( thisrins[j]=='_' || thisrins[j]=='\0') thisrins[j]=' ';
                   }
                   thisrins[1]='\0';
                   for(j=0 ; j<= 1 ; j++) rins[maxatm][j] = thisrins[j];
               /*-----*/
                   for(j=0 ; j< MAXmodchr ; j++)
                   {/*0,1,2,3,4,5,6==\0*/
                       if( thismod[j]=='_' || thismod[j]=='\0') thismod[j]=' ';
                   }
                   thismod[MAXmodchr-1]='\0'; /* 010529 */
                   for(j=0 ; j< MAXmodchr ; j++) mod[maxatm][j] = thismod[j];
                      /*global limiting number MAXmodchr includes \0 */
               /*-----*/
                   base[maxatm] = n;
                }/*got room*/
             }
          }/*atom*/
       }/*Not at EOF*/
    }/*search for connections back to earlier hetatms from each atom*/
    for(n=1 ; n <= maxorig ; n++)
    {/*add this residue of hetatms to list of earlier hetatms*/
        /*write het atom info, inc putative model# to storage*/
        /*have to protect against blank character fields*/
        /*name has to be either ATOM or HETATM to have gotten here*/

        /*pad all char strings to full length of PDB format item with _ for blanks*/
        for(j=0 ; j<= 5 ; j++)  thisatom[j] = atom[n][j];
        for(j=0 ; j<= 4 ; j++)
        {/*0,1,2,3,4,5==\0*/
            if(thisatom[j]==' ' || thisatom[j]=='\0') thisatom[j]='_';
        }
        thisatom[5]='\0';
     /*-----*/
        for(j=0 ; j<= 5 ; j++)  thisAnum[j] = Anum[n][j];
        for(j=0 ; j<= 4 ; j++)
        {/*0,1,2,3,4,5==\0*/
            if(thisAnum[j]==' ' || thisAnum[j]=='\0') thisAnum[j]='_';
        }
        thisAnum[5]='\0'; /*071001*/
     /*-----*/
        for(j=0 ; j<= 3 ; j++) thisres[j] = res[n][j];
        for(j=0 ; j<= 2 ; j++)
        {/*0,1,2,3==\0*/
           if( thisres[j]==' ' || thisres[j]=='\0') thisres[j]='_';
        }
        thisres[3]='\0';
     /*-----*/
        for(j=0 ; j<= 6 ; j++) thisname[j] = name[n][j];
        for(j=0 ; j<= 5 ; j++)
        {/*0,1,2,3,4,5,6==\0*/
           if( thisname[j]==' ' || thisname[j]=='\0') thisname[j]='_';
        }
        thisname[6]='\0';
     /*-----*/
        for(j=0 ; j< 2 ; j++) thissub[j] = sub[n][j]; /*2char chainID 070926*/
        for(j=0 ; j< 2 ; j++)
        {/*0,1,2==\0*/
           if( thissub[j]==' ' || thissub[j]=='\0') thissub[j]='_';
        }
        thissub[2]='\0';
     /*-----*/
        for(j=0 ; j<= 1 ; j++) thisrins[j] = rins[n][j];
        for(j=0 ; j<= 1 ; j++)
        {/*0,1==\0*/
           if( thisrins[j]==' ' || thisrins[j]=='\0') thisrins[j]='_';
        }
        thisrins[1]='\0';
     /*-----*/
        for(j=0 ; j< MAXmodchr ; j++) thismod[j] = mod[n][j];   /* 010529 */
        for(j=0 ; j< MAXmodchr ; j++)
        {/*0,1,2,3,4,5,6==\0, where global MAXmodchr==7*/
           if( thismod[j]==' ' || thismod[j]=='\0') thismod[j]='_';
        }
        thismod[MAXmodchr-1]='\0';  /* 010529 */
     /*-----*/

        /*each field must have a non-blank entry, and no internal blanks*/
        /*blank character string doesn't get read in properly*/
        sprintf(temps,"%s %s %s %s %s %d %s %s %f %f %f %f %f"EOLO,
           name[n],Anum[n],thisatom,thisres,thissub,num[n],thisrins,thismod
           ,x[n],y[n],z[n],o[n],B[n]
        );
/*add Model Char to this set, and occ and Bval*/
/*name[n],Anum[n],atom[n],res[n],sub[n],&num[n],rins[n],&x[n],&y[n],&z[n]*/
/*format(a6,a5,1x,a5,a3,a2,i4,a1,3x,3&8.3)*/ /*inserted nmr model #*/
        putonetextblockline(&hetscratch, temps); /*PKINSCRT.c*/
    }
}
/*___connecthetatms()_______________________________________________________*/

/****connectcaca()***********************************************************/
void   connectcaca(void)
{          /*either caonly or caoutl*/
  int    match,maxorig,j,k;

  /*--check if a ca was present */
  if(catrue) /*if have ca of a protein or c4' of a nucleic acid*/
  {/*catrue*/ /*also set if just have p of a nucleic acid 061001 */
    /*--find the ca and see if can hook it up to previous residue */
    /*    (use c4* back to p for nucleic acids) */
    /*--this might get squirrelly */
    /*   if there really are mainchain alternate confs!*/
    /*--default: hook to itself as a dot */
    match = 0;
    maxorig = maxatm; /*930123*/
    for(natom=1 ; natom <= maxorig ; natom++)
    {/*search for (all) the ca */
      if(atom[natom][1] == 'c' && atom[natom][2] == 'a')
      {
         /*this ca atom has already gone through connectheavyatom()*/
         /*so the ca at natom has already been assigned a base*/
         /* create a new ca entry, and hook that one up! */
/*s*/    creatm(natom);  /*makes an identical entry at maxatm=maxatm+1 */
         if(atom[natom][3] == ' '||atom[natom][3] == '1') /*allows GFP...*/
         { /*connect back to earlier residue*/
/*s*/       oldcon(maxatm,cadist,1);/*find base for new atom  at maxatm*/
/*
fprintf(stderr,"connectcaca(oldconCa): level %d, res %d, atom %d %s <--%d copied to # %d: back connect to %d %s\n",level,icountres[level],natom,atom[natom],base[natom],maxatm,base[maxatm],atom[base[maxatm]]);
*/
               /*at <= cadist distance, ica == 1 */
               /*ica = 1; is a ca-ca typr pseudobackbone*/
               /*-- cadist here, not bondst ! */
         }
         else /*allows for GFPchromophore 070801, only specialResidue case now*/
         {
            if(atom[natom][3] == '2')
            {
               for(j=1 ; j <= maxorig ; j++)
               {/*search for earlier ca */
                  if(atom[j][1]=='c'&&atom[j][2]=='a'&&atom[j][3]=='1')
                  { /*make a new entry and a new connection*/
                     creatm(natom);  /*make this ca entry at maxatm=maxatm+1 */
                     base[maxatm] = j;
                  }
               }
            }
            else if(atom[natom][3] == '3')
            {
               for(j=1 ; j <= maxorig ; j++)
               {/*search for earlier ca */
                  if(atom[j][1]=='c'&&atom[j][2]=='a'&&atom[j][3]=='2')
                  { /*make a new entry and a new connection*/
                     creatm(natom);  /*make this ca entry at maxatm=maxatm+1 */
                     base[maxatm] = j;
                  }
               }
            }
         }
         match = 1; /*set since at least one ca found */
      }
      /*if(match == 1) break;*/
    }/*search for (all) the ca */
    if(match == 0)
    {/*try as if a nucleic acid*/
        maxorig = maxatm; /*930123*/
        for(natom=1 ; natom <= maxorig ; natom++)
        {/*search for p */
            if(   atom[natom][1] == 'p'
               && atom[natom][2] == ' '
               && atom[natom][3] == ' ' )
            {/*found p */
/*
fprintf(stderr,"connectcaca(found P): level %d, res %d, atom %d %s <--%d %s\n",level,icountres[level],natom,atom[natom],base[natom],atom[base[natom]]);
*/
                /*sprintf(alertstr,"found %s",atom[natom]); DoReportDialog();*/
                /*this atom has already gone through connectheavyatom()*/
                /*so the atom at natom has already been assigned a base*/
                /* create a new atom entry, and hook that one up! */
/*s*/           creatm(natom);  /*makes an identical entry at maxatm=maxatm+1*/
                /*p would connect back to previous residue*/
/*s*/           oldcon(maxatm,10.0,1);/*find base for new atom  at maxatm*/
/*
fprintf(stderr,"connectcaca(oldconP): level %d, res %d, atom %d %s <--%d copied to # %d: back connect to %d\n",level,icountres[level],natom,atom[natom],base[natom],maxatm,base[maxatm]);
*/
                   /*at <= 10A distance, ica == 1 */
                   /*-- 10A here, not bondst ! */
                   /*ica = 1; is a ca-ca type pseudobackbone*/
                match = 1; /*set to break from search, since p found */
            }/*found p */
        }/*search for p */
        /*find this residue's c4' */
        for(j = 1; j<=maxorig; j++)
        {/*search for c4' */
            if(   atom[j][1] == 'c'
                 && atom[j][2] == '4'
                 &&(atom[j][3] == '*'||atom[j][3] == '\'') )
            {/*found c4' */
                if(match)
                {/*also have a p */
                   /* create a new atom entry, and hook that one up! */
/*s*/              creatm(j);  /*makes an identical entry at maxatm=maxatm+1*/
                               /*for j atom == c4' */
/*s*/              base[maxatm] = natom; /* P */
/*
fprintf(stderr,"connectcaca(assignP): level %d, res %d, atom %d %s copied to # %d: back connect to %d\n",level,icountres[level],j,atom[j],maxatm,base[maxatm]);
*/
                }/*also have a p */
                /*also hook c4' to this residue's c1' */
                for(k = 1; k<=maxorig; k++)
                {/*search for c1' */
                   if(   atom[k][1] == 'c'
                      && atom[k][2] == '1'
                      &&(atom[k][3] == '*'||atom[k][3] == '\'') )
                   {/*found c1' */
                     /* create a new atom entry, and hook that one up! */
/*s*/                creatm(k);  /*makes an identical entry at maxatm=maxatm+1*/
                               /*for k atom == c1' */
/*s*/                base[maxatm] = j; /* c4' */
/*
fprintf(stderr,"connectcaca(assignC): level %d, res %d, atom %d %s copied to # %d: back connect to %d\n",level,icountres[level],k,atom[k],maxatm,base[maxatm]);
*/
                     break; /*only need to find one instance of c1' */
                   }/*found c1' */
               }/*search for c1' */
               Lnucleicacid = 1;
               break; /*only need to find one instance of c4' */
            }/*found c4' */
        }/*search for c4' */
    }/*try as if a nucliec acid*/
  }/*catrue*/
}
/*__connectcaca()___________________________________________________________*/
/*
ATOM     42  C4    G A   2      18.394  24.893  21.964  1.00   .76      2BNA 129
ATOM     43  P     C A   3      24.155  24.981  19.297  1.00 12.43      2BNA 130
ATOM     44  O1P   C A   3      23.669  25.875  18.229  1.00 24.85      2BNA 131
ATOM     45  O2P   C A   3      25.623  24.813  19.359  1.00 25.42      2BNA 132
ATOM     46  O5*   C A   3      23.454  23.553  19.172  1.00  7.50      2BNA 133
ATOM     47  C5*   C A   3      23.900  22.466  20.009  1.00  9.65      2BNA 134
ATOM     48  C4*   C A   3      22.779  21.457  20.039  1.00 20.96      2BNA 135
ATOM     49  O4*   C A   3      21.567  22.184  20.197  1.00  6.08      2BNA 136
ATOM     50  C3*   C A   3      22.569  20.659  18.763  1.00 -2.15      2BNA 137
ATOM     51  O3*   C A   3      23.360  19.464  18.717  1.00  9.95      2BNA 138
ATOM     52  C2*   C A   3      21.070  20.384  18.773  1.00 -3.42      2BNA 139
ATOM     53  C1*   C A   3      20.510  21.302  19.846  1.00 -7.07      2BNA 140
ATOM     54  N1    C A   3      19.444  22.185  19.351  1.00 -8.22      2BNA 141
ATOM     55  C2    C A   3      18.147  21.882  19.616  1.00 -8.65      2BNA 142
ATOM     56  O2    C A   3      17.900  20.819  20.191  1.00 -7.22      2BNA 143
ATOM     57  N3    C A   3      17.180  22.745  19.271  1.00 -8.01      2BNA 144
ATOM     58  C4    C A   3      17.493  23.894  18.654  1.00  1.54      2BNA 145
ATOM     59  N4    C A   3      16.516  24.772  18.306  1.00  3.07      2BNA 146
ATOM     60  C5    C A   3      18.806  24.219  18.340  1.00  5.69      2BNA 147
ATOM     61  C6    C A   3      19.803  23.328  18.715  1.00 -9.65      2BNA 148
ATOM     62  P     G A   4      23.590  18.696  17.329  1.00 23.99      2BNA 149
*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****storeconnectingatom()***************************************************/
int  storeconnectingatom(int j, int ipass) /*ipass 061001, GFP 070801 */
{
   /* j == 0 for mainchain,  j == 1 for ca-ca connection */
   /* increase reference string arrays so indexes match those of compared atom*/
   /* 070801 need another char for GFPchromophore */
static  char  conchr[6]={' ',' ',' ',' ',' ','\0'};
static  char  nucchr[6]={' ',' ',' ',' ',' ','\0'};
static  int    noldat,k;
  int ireturn=0;

  /* 070801 GFPchromophore try _c3_  and _ca3_ */

  /*two states specified by j=0,1 ; 2 different storages of oldatm, etc. */
  /*j==1,ipass==2 for p--p as well as p--c4'--p  061001 connect across endchn*/
  /*set oldatm[j] = FALSE, only j's, storage of j==0 & j==1 done separately*/
  oldatm[j] = FALSE; oldata[j] = FALSE; oldatb[j] = FALSE; oldatc[j] = FALSE;

  /*search for the c or o3* of residue just processed,*/
  /*  unless ca only, then use ca */
  /*presume clean atom names for connecting atoms, no need to check some char*/

  if( j == 0 && ipass == 1)
  {
    conchr[1] = 'c';
    conchr[2] = ' ';
    conchr[3] = ' '; /*070801 for GFPchromophore*/
    nucchr[1] = 'o';
    nucchr[2] = '3';
    nucchr[3] = '*';
    nucchr[4] = '\'';

    if(LGFPchromophore) {conchr[2] = '3';}

    k = 0;   /* offset to store atom in array */
  }
  else if (j == 1 && ipass == 1) /*caonly*/
  {
    conchr[1] = 'c';
    conchr[2] = 'a';
    conchr[3] = ' '; /*070801 for GFPchromophore*/
    nucchr[1] = 'c';
    nucchr[2] = '4';
    nucchr[3] = '*';
    nucchr[4] = '\'';

    if(LGFPchromophore) {conchr[3] = '3';}

    k = 4;   /* offset to store atom in array */
  }
  else if (j == 1 && ipass == 2) /*caonly, and P only  061001*/
  {
    conchr[1] = 'c';
    conchr[2] = 'a';
    conchr[3] = ' '; /*070801 for GFPchromophore*/
    nucchr[1] = 'p';
    nucchr[2] = ' ';
    nucchr[3] = ' ';  /*070801 V3UNSAFE ?? IS THIS P NAME OK FOR NEW V3.0 PDB*/
    nucchr[4] = '\'';

    if(LGFPchromophore) {conchr[3] = '3';}

    k = 4;   /* offset to store atom in array */
  }
  for(natom=1 ; natom<=maxatm ; natom++)
  {/*loop over natom */
    if(  (   atom[natom][1] == conchr[1] && atom[natom][2] == conchr[2]
          && atom[natom][3] == conchr[3] ) /*070801*/

       ||(   atom[natom][1] == nucchr[1] && atom[natom][2] == nucchr[2]
          &&(atom[natom][3] == nucchr[3] || atom[natom][3] == nucchr[4] ) )
      )
    {/*store old atom and set alternate conf flags */

      if(atom[natom][4] == ' ')
      {
        noldat = lastat - k;
        oldatm[j] = TRUE;
      }
      else if(atom[natom][4] == 'a')
      {
        noldat = lastat - k - 1;
        oldata[j] = TRUE;
      }
      else if(atom[natom][4] == 'b')
      {
        noldat = lastat - k - 2;
        oldatb[j] = TRUE;
      }
      else if(atom[natom][4] == 'c')
      {
        noldat = lastat - k - 3;
        oldatc[j] = TRUE;
      }
      else  /* use lastat unless this mainchain has an alt conf */
      {
        noldat = lastat - k;
        oldatm[j] = TRUE;
      }

      tacker(noldat,natom); /*PKINCSBS.c*/
           /*  nput,nget  */

/*
sprintf(alertstr,"stored %s in %d - %d",atom[natom],lastat,k);DoReportDialog();
fprintf(stderr,"stored %s in %d - %d\n",atom[natom],lastat,k);
*/
      ireturn = 1;  /*061001*/
    }/*store old atom and set alternate conf flags */
  }/*loop over natom */
  return(ireturn); /*061001*/
}
/*___storeconnectingatom()__________________________________________________*/

/****notawater()*************************************************************/
int    notawater(void)  /* res[MAXARR][4] && called with maxatm == 1 */
{
  if(res[1][0] == 'h' && res[1][1] == 'o' && res[1][2] == 'h') return(FALSE);
  if(res[1][0] == 'd' && res[1][1] == 'o' && res[1][2] == 'd') return(FALSE);
  if(res[1][0] == 'w' && res[1][1] == 'a' && res[1][2] == 't') return(FALSE);
  if(res[1][0] == 'h' && res[1][1] == '2' && res[1][2] == 'o') return(FALSE);
  if(res[1][0] == 'd' && res[1][1] == '2' && res[1][2] == 'o') return(FALSE);
  if(res[1][0] == 't' && res[1][1] == 'i' && res[1][2] == 'p') return(FALSE);
    /*some sort of calculations type of  model for water*/
  else return(TRUE);
}
/*___notawater()____________________________________________________________*/

/****isawater()**************************************************************/
int    isawater(void)  /* res[MAXARR][4] && called with maxatm == 1 */
{
  if(res[1][0] == 'h' && res[1][1] == 'o' && res[1][2] == 'h') return(TRUE);
  if(res[1][0] == 'd' && res[1][1] == 'o' && res[1][2] == 'd') return(TRUE);
  if(res[1][0] == 'w' && res[1][1] == 'a' && res[1][2] == 't') return(TRUE);
  if(res[1][0] == 'h' && res[1][1] == '2' && res[1][2] == 'o') return(TRUE);
  if(res[1][0] == 'd' && res[1][1] == '2' && res[1][2] == 'o') return(TRUE);
  if(res[1][0] == 't' && res[1][1] == 'i' && res[1][2] == 'p') return(TRUE);
  if(res[1][0] == 't' && res[1][1] == 'p' && res[1][2] == '3') return(TRUE);
    /*some sort of calculations type of  model for water*/
  else return(FALSE);
}
/*___isawater()_____________________________________________________________*/

/****isahydrogen()************************************************************/
int  isahydrogen(char record[7], char atmname[6], char elem[3])/*070520,070730*/
{
  if(    (elem[0]==' ')
      && ( (elem[1]=='H'||elem[1]=='h')||(elem[1]=='D'||elem[1]=='d') ) )
     {return(TRUE);}/*080326 H,h; 130415 D,d*/
  else if(elem[0] != '\0')                  {return(FALSE);}
  else /* elem field NOT present, work from atom name */
  {
   /*presumably the element name is within the first two non-blank characters*/
   /* avoid false positive at the potential expense of some false negatives*/
   /* for some HETATM record atoms */

    if(  ((atmname[0] == ' ')||(isdigit(atmname[0]) ) )         /* HN2,1HH1*/
       &&(atmname[1]=='h' ||atmname[1]=='d' ||atmname[1]=='q') )
         {return(TRUE);}

    else if(  (record[0]=='a')
       &&( (isdigit(atmname[2]) )&&(isdigit(atmname[3]) ) )  /*old HG12,HH11*/
       &&(atmname[0]=='h' || atmname[0]=='d' || atmname[0]=='q') )
         {return(TRUE);} /*this is an old (v. 2.3)  safe condition*/

    else if(  (record[0]=='a')      /*h 1st char of atom record name, new HOP2*/
       &&(atmname[0]=='h' ||atmname[0]=='d' ||atmname[0]=='q') )
         {return(TRUE);}  /*general case of previous condition */
                          /*safe, one hopes, for ATOM records anyway*/

    else {return(FALSE);} /*maybe fails for some HETATM record hydrogens ??  */
  }
}
/*___isahydrogen()___________________________________________________________*/

/****isahydrogenname()********************************************************/
int  isahydrogenname(char atmname[6])  /*071222*/
{
      /*original single char element name with blank or digit preceeding it*/
   if(  ((atmname[0] == ' ')||(isdigit(atmname[0]) ) )         /* HN2,1HH1*/
      &&(atmname[1]=='h' || atmname[1]=='d' || atmname[1]=='q') )
        {return(TRUE);}

     /*h 1st char of atom record name, old HG12, new HH11*/
   else if(  (isdigit(atmname[2]) )&&(isdigit(atmname[3]) )
           &&(atmname[0]=='h' || atmname[0]=='d' || atmname[0]=='q') )
        {return(TRUE);} /*this is an old (v. 2.3)  safe condition*/
                        /*and true for some new format hydrogens */
     /*h 1st char of atom name record, general case, e.g.  new HOP2*/
                        /*as it stands, includes above case...*/
   else if(atmname[0]=='h' ||atmname[0]=='d' ||atmname[0]=='q' )
        {return(TRUE);}  /*general case of previous condition */
                         /*safe, one hopes, for ATOM records anyway*/

   else {return(FALSE);} /*maybe fails for some HETATM record hydrogens ??  */
}
/*___isahydrogenname()_______________________________________________________*/

/****iselement()*************************************************************/
int  iselement(char elem1[3], char elem2[3]) /*070520*/
{
   if(elem1[0] == elem2[0] && elem1[1] == elem2[1]) {return(TRUE);}
   else {return(FALSE);}
}
/*___iselement()_____________________________________________________________*/

/****whatelement()************************************************************/
void whatelement(char record[7], char atmname[6], char elem[3])/*070520,070730*/
{
   elem[0] = '\0'; elem[1]= '\0'; elem[2] = '\0'; /*safety*/

   /*element name is always in the first 2 characters of the atom name*/
   /* this was always true in official old pdb (<= v 2.3) */
   /* seems to be safe, in as far as I can remember*/
   /* seems to be true in new (v 3.0) pdb files */
   /*presumably the element name is within the first two non-blank characters*/
   /* so if 1st character is blank, then one-char element name is in 2nd */
   if(atmname[0] == ' ') /*this also catches many hydrogens*/
   {
      elem[0]=' '; elem[1] = atmname[1];
   }
   /*hydrogens sometimes have their parent atom as part of their name, */
   /* identifying hydrogens first should disambuiguate the name*/
   /*int  isahydrogen(char record[7], char atmname[6], char elem[3])*/
   else if(isahydrogen(record,atmname,elem))
   {
      elem[0]=' '; elem[1]='H'; /*080326 migrate element to upper case*/
   }
   else if(atmname[0] != ' ' && atmname[1] != ' ')
   {
      elem[0] = atmname[0]; elem[1] = atmname[1]; /*element lowercase 080326*/
   }
   else
   {
      elem[0] = '\0'; /* redundant setting, equivalent to return(FALSE) */
   }
}
/*___whatelement()___________________________________________________________*/

/****connectssbonds()********************************************************/
void   connectssbonds(void)
{/*retrieve cyssg and check for ssbonds*/
static  char  sscntl[5]; /* 4 characters */
  int    j,k,LOK;
  float  dist;
  char   color[16];  /*050211 VRML*/
  char   cntl[5];    /*050208 for VRML*/
  char   whostr[32]; /*050211 VRML*/
  float  radius=0;   /*050211 VRML*/
  char   colr1[32],colr2[32];   /*050704 Bcol for ss*/
  char   Bstr1[20], Bstr2[20]; /*050704 for Bvaloutl*/

  sscntl[0] = 's';
  sscntl[1] = 's';
  sscntl[2] = ' ';
  sscntl[3] = ' ';
  sscntl[4] = '\0';

  sscnt = 0;   /* initialize counter for this subunit's -ss- bonds */
          /* no provision here for -ss- between subunits! */
  for(j=2 ; j<= ssknt ; j++)
  {/*loop on j*/
    for(k=1 ; k<j ; k++)
    {/*loop on k*/
      dist = (float)distan(ssx[j],ssy[j],ssz[j],ssx[k],ssy[k],ssz[k]);

      if(dist <= ssdist)
      {/*dist ok*/
       if(   (ssnum[j] != ssnum[k]) ) /*080410 only check resnum not chain*/
       /*   && (sssub[j][0] != sssub[k][0] || sssub[j][1] != sssub[k][1]) )*/
       /*080410 && to restore chainid check - but ss could be on same chain*/
       {/*not alternative conformations on same residue*/
        LOK=0;
        if(ssatom[j][4]==' '&&ssatom[k][4]==' ')
        {/*not an obvious alt conf case*/
           LOK=1;
           altstr[0]='\0'; /*null string, takes no space on output line*/
        }
        else
        {/*maybe alt conf, make sure SS is sensible combination*/
           /*altstr is the pointmaster designator */
           altstr[0]=' ';altstr[1]='\'';altstr[2]=' ';altstr[3]='\'';
           altstr[4]='\0';

           if(ssatom[j][4]!=' '&&ssatom[k][4]==' ')
              {LOK=1; altstr[2]=ssatom[j][4];}
           if(ssatom[j][4]==' '&&ssatom[k][4]!=' ')
              {LOK=1; altstr[2]=ssatom[k][4];}
           if(  (ssatom[j][4]!=' '&&ssatom[k][4]!=' ')
              &&(ssatom[j][4]==ssatom[k][4])  )
              {LOK=1; altstr[2]=ssatom[j][4];}
        }
        if(LOK)
        {/*if both are alt confs, both are same alt conformation*/
          if(ssscall[k] && ssscall[j])
          {/*do all cys sc atoms, both sides present, e.g. caca+ss+hets... */
           /* 050704 Colr1,Colr2,Bstr1,Bstr2 */
           /* 050705 altstr on P == that of following L type point (for KiNG)*/
              /*P-L-L-L-L-L CAk-CBk-SGk-SGj-CBj-CAj */
              sscnt = sscnt + 1;
              Listss = TRUE;
              if(Bcoloutl){Bcoloredoutput(colr2, sscbBval[k]);} /*050704*/
              else        {colr2[0] = '\0';}  /*050704*/
              if(Bvaloutl)
              {
                 sprintf(Bstr1," B%.2f",sscaBval[k]);
                 sprintf(Bstr2," B%.2f",sscbBval[k]);
              }
              else        {Bstr1[0] = '\0';Bstr2[0] = '\0';}
              sprintf(temps,"%s{ ca  %s%s%4d%s%s}P%s %.3f, %.3f, %.3f "
                              "{ cb  %s%s%4d%s%s}L%s%s %.3f, %.3f, %.3f"EOLO
                       ,sscntl
                       ,ssres[k],sssub[k],ssnum[k],ssrins[k],Bstr1
                       ,altstr,sscax[k],sscay[k],sscaz[k]
                       ,ssres[k],sssub[k],ssnum[k],ssrins[k],Bstr2
                       ,altstr,colr2,sscbx[k],sscby[k],sscbz[k]);
              putonetextblockline(&mainscratch, temps); /*PKINSCRT.c CAk--CBk*/
              ++countxyz; /*P point*/
              ++countxyz; /*L point*/
              if(Bcoloutl)
              {
                 Bcoloredoutput(colr1, ssBval[k]);
                 Bcoloredoutput(colr2, ssBval[j]);
              }/*separate routine 050704*/
              else        {colr1[0] = '\0';colr2[0] = '\0';}  /*050704*/
              if(Bvaloutl) /*050704*/
              {
                 sprintf(Bstr1," B%.2f",ssBval[k]);
                 sprintf(Bstr2," B%.2f",ssBval[j]);
              }
              else        {Bstr1[0] = '\0';Bstr2[0] = '\0';}  /*050704*/
              sprintf(temps,"%s{%s%s%s%4d%s%s}L%s%s %.3f, %.3f, %.3f "
                              "{%s%s%s%4d%s%s}L%s%s %.3f, %.3f, %.3f"EOLO
                   ,sscntl,ssatom[k],ssres[k],sssub[k],ssnum[k],ssrins[k],Bstr1
                   ,altstr,colr1,ssx[k],ssy[k],ssz[k]
                   ,ssatom[j],ssres[j],sssub[j],ssnum[j],ssrins[j],Bstr2
                   ,altstr,colr2,ssx[j],ssy[j],ssz[j]);
              putonetextblockline(&mainscratch, temps); /*PKINSCRT.c SGk--SGj*/
              ++countxyz; /*P point*/
              ++countxyz; /*L point*/
              if(Bcoloutl)
              {
                 Bcoloredoutput(colr1, sscbBval[j]);
                 Bcoloredoutput(colr2, sscaBval[j]);
              }/*separate routine 050704*/
              else        {colr1[0] = '\0';colr2[0] = '\0';}  /*050704*/
              if(Bvaloutl) /*050704*/
              {
                 sprintf(Bstr1," B%.2f",sscbBval[j]);
                 sprintf(Bstr2," B%.2f",sscaBval[j]);
              }
              else        {Bstr1[0] = '\0';Bstr2[0] = '\0';}  /*050704*/
              sprintf(temps,"%s{ cb  %s%s%4d%s%s}L%s%s %.3f, %.3f, %.3f "
                              "{ ca  %s%s%4d%s%s}L%s%s %.3f, %.3f, %.3f"EOLO
                       ,sscntl,ssres[j],sssub[j],ssnum[j],ssrins[j],Bstr1
                       ,altstr,colr1,sscbx[j],sscby[j],sscbz[j]
                       ,ssres[j],sssub[j],ssnum[j],ssrins[j],Bstr2
                       ,altstr,colr2,sscax[j],sscay[j],sscaz[j]);
              putonetextblockline(&mainscratch, temps); /*PKINSCRT.c CBj--CAj*/
              ++countxyz; /*P point*/
              ++countxyz; /*L point*/
          }
          else
          {/*just do the two -ss- atoms*/
             if(LdumpVRML && LvectorVRML) /*050208*/
             {
                sprintf(color," yellow");
                sprintf(cntl,"scv ");/*goes with sidechain vectors*/
                sprintf(whostr,"-ss-");
                radius = ribwidVRML/nstrnd;
                writeVRMLvectortoscratch(cntl,color,radius,whostr
                   ,ssx[k],ssy[k],ssz[k] ,ssx[j],ssy[j],ssz[j]);
             }
             else
             {
                sscnt = sscnt + 1;
                Listss = TRUE;
                if(Bcoloutl)
                {
                   Bcoloredoutput(colr2, ((ssBval[j]+ssBval[k])/2) );
                }/*separate routine 050704*/
                else        {colr2[0] = '\0';}  /*050704*/
                if(Bvaloutl) /*050704*/
                {
                   sprintf(Bstr1," B%.2f",ssBval[k]);
                   sprintf(Bstr2," B%.2f",ssBval[j]);
                }
                else        {Bstr1[0] = '\0';Bstr2[0] = '\0';}  /*050704*/
                sprintf(temps,"%s{%s%s%s%4d%s%s}P%s %.3f, %.3f, %.3f "
                                "{%s%s%s%4d%s%s}L%s%s %.3f, %.3f, %.3f"EOLO
                         ,sscntl,ssatom[k],ssres[k],sssub[k],ssnum[k]
                         ,ssrins[k],Bstr1,altstr,ssx[k],ssy[k],ssz[k]
                         ,ssatom[j],ssres[j],sssub[j],ssnum[j]
                         ,ssrins[j],Bstr2,altstr,colr2,ssx[j],ssy[j],ssz[j]);
                putonetextblockline(&mainscratch, temps); /*PKINSCRT.c*/
                ++countxyz; /*P point*/
                ++countxyz; /*L point*/
             }
          }
        }/*if both are alt confs, both are same alt conformation*/
       }/*not alternative conformations on same residue*/
      }/*dist ok*/
    }/*loop on k*/
  }/*loop on j*/
}
/*____connectssbonds()______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****getresiduename()********************************************************/
void   getresiduename(void)
{
      int   j;

      iresiduecnt++;
      j = 1; /*931128 used to be just this */
    for(j=1;j<=mrange;j++) /*931128 j over ranges new feature*/
    {
      /* do Calphas or backbone/pseudobackbone for first pass only */
      caxl[j] = 0; /*931128 had been 1 rather than j */
      resl[j] = 1; /* which was set to get here in the first place */
      scxl[j] = 1; /*set for all ranges for later passes */


      switch (iresiduecnt)
      {
        case  1: /*special case to do ca-ca */
          {resid[j][0] = 'a'; resid[j][1] = 'l'; resid[j][2] = 'l';
           caxl[j] = 1;scxl[j] = 0;resl[j] = 0;}
           /*set for all ranges for first pass */
        break;
        case  2:
          if(Lallaminoacids)
          {resid[j][0] = 'c'; resid[j][1] = 'y'; resid[j][2] = 's';
           kolor[0]='y';kolor[1]='e';kolor[2]='l';kolor[3]='l';kolor[4]='o';
           kolor[5]='w';kolor[6]='\0'; /*yellow*/
          }
          else /*Lallbases*/
          {resid[j][0] = 'a'; resid[j][1] = ' '; resid[j][2] = ' ';
           kolor[0]='p';kolor[1]='i';kolor[2]='n';kolor[3]='k';
           kolor[4]='\0'; /*A type: pink*/
          }
        break;
        case  3:
          if(Lallaminoacids)
          {resid[j][0] = 'p'; resid[j][1] = 'r'; resid[j][2] = 'o';
           kolor[0]='h';kolor[1]='o';kolor[2]='t';kolor[3]='p';kolor[4]='i';
           kolor[5]='n';kolor[6]='k';kolor[7]='\0'; /*pro: hotpink*/
          }
          else /*Lallbases*/
          {resid[j][0] = 't'; resid[j][1] = 'u'; resid[j][2] = ' ';
           /*nucleic acids use first letter, so second can be alternative*/
           kolor[0]='s';kolor[1]='k';kolor[2]='y';kolor[3]='\0';kolor[4]='\0';
           /*T,U type: sky*/
          }
        break;
        case  4:
          if(Lallaminoacids)
          {resid[j][0] = 'g'; resid[j][1] = 'l'; resid[j][2] = 'y';
           kolor[0]='h';kolor[1]='o';kolor[2]='t';kolor[3]='p';kolor[4]='i';
           kolor[5]='n';kolor[6]='k';kolor[7]='\0'; /*gly: hotpink*/
          }
          else /*Lallbases*/
          {resid[j][0] = 'g'; resid[j][1] = ' '; resid[j][2] = ' ';
           kolor[0]='s';kolor[1]='e';kolor[2]='a';kolor[3]='\0';kolor[4]='\0';
           /*G type: sea*/
          }
        break;
        case  5:
          if(Lallaminoacids)
          {resid[j][0] = 't'; resid[j][1] = 'y'; resid[j][2] = 'r';
           kolor[0]='s';kolor[1]='e';kolor[2]='a';kolor[3]='\0';kolor[4]='\0';
           /*tyr: sea*/
          }
          else /*Lallbases*/
          {resid[j][0] = 'c'; resid[j][1] = ' '; resid[j][2] = ' ';
           kolor[0]='y';kolor[1]='e';kolor[2]='l';kolor[3]='l';
           kolor[4]='o';kolor[5]='w';kolor[6]='\0';/*C type: yellow*/
          }
        break;
        case  6:
          if(Lallaminoacids)
          {resid[j][0] = 'p'; resid[j][1] = 'h'; resid[j][2] = 'e';
           kolor[0]='s';kolor[1]='e';kolor[2]='a';kolor[3]='\0';kolor[4]='\0';
           /*phe: sea*/
          }
          else /*Lallbases*/
          {resid[j][0] = ' '; resid[j][1] = ' '; resid[j][2] = ' ';
           kolor[0]='w';kolor[1]='h';kolor[2]='i';kolor[3]='t';
           kolor[4]='e';kolor[5]='\0';/*misc type: white*/
          }
        break;
        case  7:
          {resid[j][0] = 't'; resid[j][1] = 'r'; resid[j][2] = 'p';
           kolor[0]='s';kolor[1]='e';kolor[2]='a';kolor[3]='\0';kolor[4]='\0';
          }
        break;
        case  8:
          {resid[j][0] = 'l'; resid[j][1] = 'e'; resid[j][2] = 'u';
           kolor[0]='g';kolor[1]='o';kolor[2]='l';kolor[3]='d';kolor[4]='\0';
          }
        break;
        case  9:
          {resid[j][0] = 'i'; resid[j][1] = 'l'; resid[j][2] = 'e';
           kolor[0]='g';kolor[1]='o';kolor[2]='l';kolor[3]='d';kolor[4]='\0';
          }
        break;
        case  10:
          {resid[j][0] = 'v'; resid[j][1] = 'a'; resid[j][2] = 'l';
           kolor[0]='g';kolor[1]='o';kolor[2]='l';kolor[3]='d';kolor[4]='\0';
          }
        break;
        case 11:
          {resid[j][0] = 'm'; resid[j][1] = 'e'; resid[j][2] = 't';
           kolor[0]='g';kolor[1]='o';kolor[2]='l';kolor[3]='d';kolor[4]='\0';
          }
        break;
        case 12:
          {resid[j][0] = 'a'; resid[j][1] = 'l'; resid[j][2] = 'a';
           kolor[0]='g';kolor[1]='o';kolor[2]='l';kolor[3]='d';kolor[4]='\0';
          }
        break;
        case 13:
          {resid[j][0] = 's'; resid[j][1] = 'e'; resid[j][2] = 'r';
           kolor[0]='c';kolor[1]='y';kolor[2]='a';kolor[3]='n';kolor[4]='\0';
          }
        break;
        case 14:
          {resid[j][0] = 't'; resid[j][1] = 'h'; resid[j][2] = 'r';
           kolor[0]='c';kolor[1]='y';kolor[2]='a';kolor[3]='n';kolor[4]='\0';
          }
        break;
        case 15:
          {resid[j][0] = 'g'; resid[j][1] = 'l'; resid[j][2] = 'n';
           kolor[0]='c';kolor[1]='y';kolor[2]='a';kolor[3]='n';kolor[4]='\0';
          }
        break;
        case 16:
          {resid[j][0] = 'a'; resid[j][1] = 's'; resid[j][2] = 'n';
           kolor[0]='c';kolor[1]='y';kolor[2]='a';kolor[3]='n';kolor[4]='\0';
          }
        break;
        case 17:
          {resid[j][0] = 'l'; resid[j][1] = 'y'; resid[j][2] = 's';
           kolor[0]='s';kolor[1]='k';kolor[2]='y';kolor[3]='\0';kolor[4]='\0';
          }
        break;
        case 18:
          {resid[j][0] = 'a'; resid[j][1] = 'r'; resid[j][2] = 'g';
           kolor[0]='s';kolor[1]='k';kolor[2]='y';kolor[3]='\0';kolor[4]='\0';
          }
        break;
        case 19:
          {resid[j][0] = 'h'; resid[j][1] = 'i'; resid[j][2] = 's';
           kolor[0]='s';kolor[1]='k';kolor[2]='y';kolor[3]='\0';kolor[4]='\0';
          }
        break;
        case 20:
          {resid[j][0] = 'a'; resid[j][1] = 's'; resid[j][2] = 'p';
           kolor[0]='p';kolor[1]='i';kolor[2]='n';kolor[3]='k';kolor[4]='\0';
          }
        break;
        case 21:
          {resid[j][0] = 'g'; resid[j][1] = 'l'; resid[j][2] = 'u';
           kolor[0]='p';kolor[1]='i';kolor[2]='n';kolor[3]='k';kolor[4]='\0';
          }
        break;
      }
    }
}
/*___getresiduename()_______________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****readbrkaalibrecord()****************************************************/
void   readbrkaalibrecord(char temps[256]) /*neutral to atom name format*/
{
  char   frags[256],numbers[256],alphers[256];
  int    j,inumber,Lnumber,ialpher,Lalpher;
  int    igotit;

/*name[n],Anum[n],atom[n],res[n],sub[n],&num[n],rins[n],&x[n],&y[n],&z[n]*/
/*format(a6,a5,1x,a5,a3,a2,i4,a1,3x,3f8.3)*/
/*  these next lines are 84 char long, they contain 80 char templates */
/*
0123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789
123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
atom      1  n   arg a   1      26.465  27.452  -2.490  1.00 25.18      4pti  89
atom      2  ca  arg a   1      25.497  26.862  -1.573  1.00 17.63      4pti  90
aaaahhsssss_nnnnArrr_cjjjjI___XXXX.XXXYYYY.YYYZZZZ.ZZZooo.ooBBB.BB######FFFFiiii
aaaahhsssss_nnnnArrr_cjjjjI___XXXX.XXXYYYY.YYYZZZZ.ZZZooo.ooBBB.BB######FFFFccic
                                              76-77 right justified element name
*/
/*char  mutname[MAXARR][7],mutatom[MAXARR][6],mutres[MAXARR][4];*/
/*char  mutsub[MAXARR][3],mutrins[MAXARR][2];*/
/*char  mutAnum[MAXARR][6];*/
/*int   mutnum[MAXARR],mutbase[MAXARR];*/
/*float mutx[MAXARR],muty[MAXARR],mutz[MAXARR];*/
  /* do direct transfer since blanks are meaningful */
  igotit = 1;

    /*for(j=0 ; j<=5 ; j++) mutname[nn][j] = temps[j]; */
    for(j=0 ; j<=5 ; j++) mutname[nn][j] = tolower(temps[j]);  /*080318*/
    mutname[nn][6] = '\0';
    for(j=0 ; j<=4 ; j++) mutAnum[nn][j] = temps[j+6];
    mutAnum[nn][5] = '\0';

    /*for(j=0 ; j<=4 ; j++) mutatom[nn][j] = temps[j+12];*/
    for(j=0 ; j<=4 ; j++) mutatom[nn][j] = tolower(temps[j+12]); /*080318*/
    mutatom[nn][5] = '\0';

    /*for(j=0 ; j<=2 ; j++) mutres[nn][j] = temps[j+17];*/
    for(j=0 ; j<=2 ; j++) mutres[nn][j] = tolower(temps[j+17]); /*080318*/
    mutres[nn][3] = '\0';

    mutsub[nn][0] = temps[20]; /*2char chainID 070926*/
    mutsub[nn][1] = temps[21]; /*2char chainID 070926*/
    mutsub[nn][2] = '\0';

    /*for the record, clear the model# character string*/
    for(j=0 ; j< MAXmodchr ; j++) mutmod[nn][j] = '\0';
    /*also clear element field  071222*/
    mutelement[nn][0] = '\0';
    mutelement[nn][1] = '\0';
    mutelement[nn][2] = '\0';

    /*residue number sometimes has imbedded subunit indicator*/
    /*either before or after actual number, only accummulate number */
    /* from one contiguous block, but all alphabetics gathered together*/
    inumber = 0;
    ialpher = 0;
    Lnumber = 0;
    Lalpher = 0;
    for(j=0 ; j<=3 ; j++)
    {
        if(temps[j+22]==' ')   ; /*skip any blanks*/
        else if(  temps[j+22]=='0'||temps[j+22]=='1'||temps[j+22]=='2'
                ||temps[j+22]=='3'||temps[j+22]=='4'||temps[j+22]=='5'
                ||temps[j+22]=='6'||temps[j+22]=='7'||temps[j+22]=='8'
                ||temps[j+22]=='9'||temps[j+22]=='-'                  )
            {
            if(Lalpher)  /*already encountered alphabetic char */
            {
                if(Lnumber)
                {   /*doing alpha after number*/
                    alphers[ialpher] = temps[j+22];
                    ialpher++;
                }
                else  /*!Lnumber*/
                {   /*doing number after alph*/
                    numbers[inumber] = temps[j+22];
                    inumber++;
                }
            }
            else  /*!Lalpher*/
            {   /*doing number before any alphabetic char*/
                Lnumber = 1; /*only set this if number is first*/
                numbers[inumber] = temps[j+22];
                inumber++;
            }

        }
        else /* not a number */
        {
            Lalpher = 1; /*set this for any alphabetic char*/
            alphers[ialpher] = temps[j+22];
            ialpher++;
        }
    }
    alphers[ialpher] = '\0';
    numbers[inumber] = '\0';

    if(inumber > 0) igotit = sscanf(numbers,"%d",&mutnum[nn]);
    else  mutnum[nn] = 0;
    if(ialpher > 0 && mutsub[nn][0] == ' ')
    {
       mutsub[nn][0] = ' ';
       mutsub[nn][1] = alphers[0];
       mutsub[nn][2] = '\0';;
           /*does case where sub indicator part of res num*/
           /*ONE HOPES THAT THIS IS TRUELY OBSELETE */
    }
    mutrins[nn][0] = temps[26];
    mutrins[nn][1] = '\0';

    for(j=0 ; j<=23 ; j++) frags[j] = temps[j+30];
    frags[24] = '\0';
    igotit =  sscanf(frags,"%8f %8f %8f",&mutx[nn],&muty[nn],&mutz[nn]);

/*name[n],Anum[n],atom[n],res[n],sub[n],&num[n],rins[n],&x[n],&y[n],&z[n]*/
/*format(a6,a5,1x,a5,a3,a2,i4,a1,3x,3&8.3)*/
/*  these next lines are 84 char long, they contain 80 char templates */
/*
0123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789
123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
atom      1  n   arg a   1      26.465  27.452  -2.490  1.00 25.18      4pti  89
atom      2  ca  arg a   1      25.497  26.862  -1.573  1.00 17.63      4pti  90
*/
/*  --name*6 record designator: atom or hetatom recognized  */
/*  --Anum*5 atom number: moved blindly to output file  */
/*  --atom*5 includes atomname*4 and alt-conformation-indicator*1,  */
/*  --res*3 residue name  */
/*  --sub*2 chain ID: subunit designator  */
/*  --num:i4 residue number  */
/*  --rins*1 inserted residue indicator  */
/*  --x,y,z coord of the atom  */
/*  --so pretty std brookhaven except atom name where there is lots of  */
/*  -- non-standard brookhaven around!  */
/*  --if this reading has an error inform and abort:  */
/*  --implies a non-standard format that must be dealt with some other way  */
/*
0123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789
123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
ATOM     23  HB2 ARG     1      25.930  13.038 -12.118  1.00 34.80      5PTI H
ATOM     24  HB3 ARG     1      27.324  12.549 -11.100  1.00 34.71      5PTI H
ATOM     25 HH21 ARG     1      27.786  12.886 -15.076  1.00 33.93      5PTI H
ATOM     26 HH22 ARG     1      26.154  13.214 -14.269  1.00 34.53      5PTI H
ATOM     27  N   PRO     2      33.738  14.173 -10.126  1.00 22.41      5PTI N
ATOM     28  CA  PRO     2      34.897  13.603  -9.390  1.00 19.52      5PTI C
ATOM     29  C   PRO     2      34.652  12.135  -9.374  1.00 17.70      5PTI C
ATOM     30  O   PRO     2      33.583  11.537  -9.255  1.00 16.41      5PTI O
aaaahhsssss_nnnnArrr_cjjjjI___XXXX.XXXYYYY.YYYZZZZ.ZZZooo.ooBBB.BB######FFFFccic
                                              76-77 right justified element name
*/

  if ( !igotit )
  {
     sprintf(alertstr,"input data format problem, line:"CRLF" %s",temps);
     DoReportDialog();
     exit(0);
  }

  /*library may not have explicit element identified  071222*/
  if(mutelement[nn][0] == '\0') /*not set by element name in cols 77,78 */
  {  /*uses atom name to work out element, loads mutelement*/
     whatelement(mutname[nn], mutatom[nn], mutelement[nn]);  /*071222*/
  }

/*
sprintf(alertstr,"input: nn= %d"CRLF"%s"CRLF"interpreted:"
                 CRLF"%s|%s|%s|%s|%s|%d|%s|%f %f %f"
,nn,temps,mutname[nn],mutAnum[nn],mutatom[nn],mutres[nn],mutsub[nn],mutnum[nn]
,mutrins[nn],mutx[nn],muty[nn],mutz[nn]);
DoReportDialog();
*/
/*
sprintf(temps,"aalibrecord: nn= %d"CRLF"%s"CRLF"interpreted:"
                 CRLF"%s|%s|%s|%s|%s|%d|%s|%f %f %f"
,nn,temps,mutname[nn],mutAnum[nn],mutatom[nn],mutres[nn],mutsub[nn],mutnum[nn]
,mutrins[nn],mutx[nn],muty[nn],mutz[nn]);

          pkintextinsert(temps);
          adjusttext(3);
*/
}
/*___readbrkaalibrecord()___________________________________________________*/

/****mutateresidue()*********************************************************/
void   mutateresidue(void)
{
   int  inputline=0,igotres=0,iskip=0,j=0,nput=0,nget=0;
   int  loop=0,ataamax=0; /*controls for matching library aa to real aa*/
   char newresidue=' ';
   char oldresidue=' ';
   int   LOK = 1; /*presume everything OK until something fails*/
   float betaxyz[3],alphaxyz[3],nitexyz[3];
   int Lnew=0,Lnewbeta=0;
//   int Lline=0,Nangles=0,nth=0,index=0,k=0;
   char targetresidue[4];
//   char anglestr[256],hold[256];
//   int angle[5];
   char residuecode[4];
   char altchoice=' ';
   int  liblines[2]={0,0};


   if(Laltcontrol > 0)
   {/*presume one choice, take last specified as the choice when more than one*/
      altchoice = altcontrol[Laltcontrol];
   }
   if(Lcbetadev == 1 || Ltaudev==1 || Lsuperpos)
   {/*computing C-beta deviations from ideal*/
      mutoutl = 1; /*arbitrary*/
   }
   igotres = 0;
   nn = 0;   /*atom counter for this residue */
   ataamax = 0; /* limited number of atoms allowed per residue */
   loop = 0; /* loop control*/
   /*read and abstract atoms of desired residue, gly must also have an entry*/
   Lnew = 1;
   while( (loop == 0) && (ataamax == 0) )
   {/*residue input, while loop == 0  */
      nn = nn+1;
      if(nn >= (MAXARR-1) ){  ataamax = 1;}
      /*array size limit to match expected max residue size*/
      if(Loldpdb) /*was getonelibraryline() 071215 */
         inputline = getoldlibraryline(Lnew,0); /*PKINROTL.c rtn line in temps*/
      else
         inputline = getnewlibraryline(Lnew,0); /*PKINROTL.c rtn line in temps*/
      /*returns Nth index into the Library array of indeal aa records 040303*/
      Lnew = 0;
      /*PKINROTL.c builtin ideal aa library*/
      /*works on mutant residue arrays*/

      if(inputline > 0)  /* new input may be in buffer */
      {/*checkline for atom record*/
         inputflag = checkline(temps);
         if( inputflag != 1)
         {
            iskip = 1;
            /* return != 1 if not atom, not hetatm: ignore */
            nn = nn - 1; /*reset atom counter*/
         }
         else  /* checkline returned == 1 */
         {/* valid atom or hetatm record */
            atomin = 1;
            /*convertcase(temps); do only a few fields  080318*/
            /*for ease and consistency upper case converted to lower*/
            /*unfortunate decision to work in lower case*/
            /*keep needing to work around this to recover original PDB form*/
            iskip = 0;
         }
      }/*checkline for atom record*/
      else  /* Getoneaalibline returned inputline=0 */
      {/*no current line*/
         iskip = 1;  /* no coordinate line to process */
         nn = nn - 1; /*reset atom counter*/
      }/*no current line*/
      if(loop == 0 && iskip == 0)
      {/*valid record*/
         /*now do a formated reread of the line in texts */
         readbrkaalibrecord(temps);/*works on mutant residue arrays */

         /*returns disected temps pdb record in mutres[] arrays*/
         if(Lcbetadev == 1||Ltaudev==1)
         {/*computing C-beta deviations from ideal*/
            resid[mutoutl][0] = 'a';
            resid[mutoutl][1] = 'l';
            resid[mutoutl][2] = 'a';
         }
         /* else standard, simple mutation to target residue*/
         if(    mutres[nn][0] == resid[mutoutl][0]
             && mutres[nn][1] == resid[mutoutl][1]
             && mutres[nn][2] == resid[mutoutl][2] )
         {
            igotres = 1;
            /*once a residue found that matches target,*/
            /* then change from that signals end of mutant residue in library*/
            if(liblines[0] ==0) liblines[0] = inputline; /*Nth== library index*/
         }
         else
         {
            nn = nn - 1; /*ignore this atom, does not match target */
            if(igotres)
            {/*a matching residue had been found earlier*/
               liblines[1] = inputline - 1;
               loop = 1;
            }
         }
      }/*valid record*/
      if(loop == 1) break;
   }/*residue input, while loop == 0  */
   if(!igotres)
   {/*failed to find aa in file*/
#ifdef UNIX_X11
      printf("Did not find mutant aa in library array\n");
#endif
      if(!Lcommanded)
      {
         sprintf(alertstr,"Did not find amino acid in library array"
                      CRLF" mutation cannot be done");
         DoReportDialog();
      }
      LOK = 0;
   }/*failed to find aa in file*/
   else
   {/*got a residue that matched target*/
      /*now have nn atoms in mut array, this includes gly*/
      maxmut = nn;
      if(Ltestprint > 2)
      {
         for(j=1;j<=maxmut;j++)
         {
            sprintf(alertstr,"mutant %d: %s|%s|%s|%s|%s|%d|%s|%f %f %f"
               ,j,mutname[j],mutAnum[j],mutatom[j],mutres[j],mutsub[j],mutnum[j]
               ,mutrins[j],mutx[j],muty[j],mutz[j]);
            DoReportDialog();
         }
         for(j=1;j<=maxatm;j++)
         {
            sprintf(alertstr,"orignl %d: %s|%s|%s|%s|%s|%d|%s|%f %f %f"
               ,j,name[j],Anum[j],atom[j],res[j],sub[j],num[j]
               ,rins[j],x[j],y[j],z[j]);
            DoReportDialog();
         }
      }
      /*mutation to gly a special case, superposition NOT done */
      /* ideal geom library array residues should have hydrogens! */

      /*compute an idealized Cbeta position from an average of construction*/
      /*of Cbeta from both directions using idealized parameters*/
      /*for mutation to gly, computes HA2 */
      if(Lcbetadev||Ltaudev||Lsuperpos)
      {/*recompute the C-beta for the original residue type*/
         targetresidue[0] = res[maxatm][0];
         targetresidue[1] = res[maxatm][1];
         targetresidue[2] = res[maxatm][2];
         targetresidue[3] = '\0';
         /*when the OLD RESIDUE is gly, don't show a recomputed Calpha--Cbeta*/
         if(        res[maxatm][0] == 'g'
                 && res[maxatm][1] == 'l'
                 && res[maxatm][2] == 'y')
         {/*gly     orig*/
             oldresidue = 'G';
         }/*gly     orig*/
         else
         {
             oldresidue = ' ';
         }
      }
      else
      {/*compute the C-beta for the type of residue that is the new mutation*/
         targetresidue[0] = resid[mutoutl][0];
         targetresidue[1] = resid[mutoutl][1];
         targetresidue[2] = resid[mutoutl][2];
         targetresidue[3] = '\0';
      }

      Lnewbeta=constructbeta(betaxyz,targetresidue,alphaxyz,nitexyz,&altchoice);
      if(Lnewbeta)
      {/*Lnewbeta==1*/
         newresidue = ' ';
         /*the NEW RESIDUE*/
         if(        resid[mutoutl][0] == 'g'
                 && resid[mutoutl][1] == 'l'
                 && resid[mutoutl][2] == 'y')
         {/*gly        mut 1st   orig or mut 2nd*/
            newresidue = 'G';
         }/*gly        mut*/
         if(Lcbetadev==0 && Ltaudev==0 && newresidue != 'G')
         {/*all real cases except mutation to gly*/
          if(Lsuperpos)
          {
             LOK = imposeontoideal(betaxyz);
          }
          else
          {
             LOK = threepointimpose(alphaxyz,betaxyz,nitexyz,altchoice);
          }
          if(LOK)
          {/*superpositioning return seems OK*/
            if(Lpdbmutout)
            {
               mutatepdbrecords(liblines,(float)anglerot1,(float)anglerot2,cbcbaxis,cacbcalc,caxyz);
            }
            else if(Lcbetadev == 0 && Ltaudev==0 && Lsuperpos==0 )
            {/*add rotamer hypertext lines to text window*/
               residuecode[0] = resid[mutoutl][0];
               residuecode[1] = resid[mutoutl][1];
               residuecode[2] = resid[mutoutl][2];
               residuecode[3] = '\0';
               /*global rotatorname incl sub 030925*/
               rotamerhypertext(residuecode);
            }/*add rotamer hypertext lines to text window*/
          }/*superpositioning return seems OK*/

         }/*all real cases except mutation to gly*/
         else if(Lcbetadev==0  &&Ltaudev==0 &&Lsuperpos==0 &&newresidue == 'G')
         {/*mutation to gly*/
            if(Lnewbeta)
            {/*place hydrogen computed for Beta position*/
               /* treat as a sidechain atom if such hydrogens to be outputed*/
               maxmut = 1;
               mutx[maxmut] = betaxyz[0];
               muty[maxmut] = betaxyz[1];
               mutz[maxmut] = betaxyz[2];
               if(Loldpdb) /*071215*/
               {
                  mutatom[maxmut][0] = '2';
                  mutatom[maxmut][1] = 'h';
                  mutatom[maxmut][2] = 'a';
                  mutatom[maxmut][3] = ' ';
               }
               else
               {
                  mutatom[maxmut][0] = ' ';
                  mutatom[maxmut][1] = 'h';
                  mutatom[maxmut][2] = 'a';
                  mutatom[maxmut][3] = '2';
               }
            }
            else
            {
               maxmut = 0;
            }
            LOK = 1; /*this is an easy one*/
         }/*mutation to gly*/
         else if(Lcbetadev==1||Ltaudev==1)
         {/*C-beta deviation for ideal geometry analysis*/
            if(Lnewbeta && newresidue != 'G')
            {
               maxmut = 1;
               mutx[maxmut] = betaxyz[0];
               muty[maxmut] = betaxyz[1];
               mutz[maxmut] = betaxyz[2];
               mutatom[maxmut][0] = ' ';
               mutatom[maxmut][1] = 'c';
               mutatom[maxmut][2] = 'b';
               mutatom[maxmut][3] = ' ';
            }
            else
            {
               maxmut = 0;
            }
            LOK = 1; /*this is also an easy one*/
         }/*C-beta deviation for ideal geometry analysis*/

      }/*Lnewbeta==1*/
      if(Lcbetadev==1 && oldresidue=='G')
      {
         maxmut = 0;
      }
      if(LOK && Lcbetadev==0 && Ltaudev==0 && Lsuperpos==0)
      {/*reloadresidue()*********section that could be a separate routine*/
         n = 1;
         while(n <= maxatm)
         {/*throw away old side chain*/
            if( !decidemainside(n) ) /*works on regular residue arrays*/
            {/*sidechain of old real residue*/
               if(n!=maxatm)
               {/*compress array*/
                  for(nput=n; nput<maxatm; nput++)
                  {
                     nget=nput+1;
                     tacker(nput,nget);
                  }
               }
               maxatm=maxatm-1;
            }/*sidechain of old real residue*/
            else
            {/*mainchain of old real residue*/
               n = n + 1; /*keep this mc atom, test next n*/
            }
         }/*throw away old side chain*/
         nn = 1;
         while(nn <= maxmut)
         {/*add on new side chain*/
            if(!decidemutmainside(nn)) /*works on mutant residue arrays*/
            {
               nput = maxatm+1;
               nget = nn;
               mutadder(nput,nget);/*selected fields*/
               /*presumes maxatm always points to correct old residue*/
               maxatm=maxatm+1;
            }
            nn = nn + 1;
         }/*add on new side chain*/
      }/*reloadresidue()________section that could be a separate routine*/
      else if(LOK==0) /*LOK == 0*/
      {/*failed to do superposition*/
         LOK = 0; /*set flag*/
         sprintf(alertstr," mutation cannot be done ");
         DoReportDialog(); /*PKINDLOG.c*/
      }/*failed to do superposition*/
      else
      {/*Lcbetadev==1*/
         ; /*NOP*/
      }/*Lcbetadev==1*/

      if(Ltestprint > 2)
      {
         for(j=1;j<=maxatm;j++)
         {
            sprintf(alertstr,CRLF"built : %s|%s|%s|%s|%s|%d|%s|%f %f %f"
                            ,name[j],Anum[j],atom[j],res[j],sub[j],num[j]
                            ,rins[j],x[j],y[j],z[j]);
            pkintextinsert(alertstr);
            adjusttext(1);
            DoReportDialog();
         }
         sprintf(alertstr,CRLF"LOK== %d"CRLF,LOK);
         pkintextinsert(alertstr);
         adjusttext(1);
      }
   }/*got a residue*/
   if(!LOK || Lcbetadev || Ltaudev || Lsuperpos)
   {
      mutoutl = 0; /*no mutated output*/
      rotoutl = 0; /*no rotated output*/
      /*NB: cbetadev balls written to scratch in PKINANGL/constructbeta()*/
   }
}
/*___mutateresidue()________________________________________________________*/

/****getbracketed()***********************************************************/
int    getbracketed(char c,int Lnew,int MAXchar,char bstr[256])
{
   static int Lidn=0; /*remembers if in an identifier*/
   static int ibstr=0;
   int    ireturn;

    ireturn = 0;
    if(Lnew)
    {
        Lidn = 0;
    }
    if(c == '{' /*}*/)  /* start an identifier */
    {  /*balanced brackets keeps source editor happy*/
        Lidn = 1;
        ibstr = 0;
    }
    else if(/*{*/ c == '}' || ibstr==MAXchar)  /* end an identifier */
    {  /*balanced brackets keeps source editor happy*/
        bstr[ibstr] = '\0';
        Lidn = 0;
        ireturn = 1;
    }
    else if(Lidn == 1 && ibstr < MAXchar)
    {
        bstr[ibstr++] = c;
    }
    else
    {
        ;  /* skip anything not in brackets */
    }
    return(ireturn);
}
/*___getbracketed()__________________________________________________________*/

/****getinteger()*************************************************************/
int getinteger(int c,int Lnew,int* result)
{
    int        ireturn;
    static int iintstr=0;
    static char intstr[256];

    ireturn = 0;
    if(Lnew)
    {
        iintstr = 0;
        /* Lnew local, must reset in calling routine */
		memset(intstr,' ',256);   /* optimized */
        /*for(j=0 ; j<256 ; j++) intstr[j] = ' '; */
    }

    if(iintstr==0 && (c==' '||c==',') )
    {
        ;  /* skip beginning white spaces */
    }
    else if(c=='0'||c=='1'||c=='2'||c=='3'||c=='4'||c=='5'||
            c=='6'||c=='7'||c=='8'||c=='9'||c=='-'||c=='+')
    {
        intstr[iintstr] = c;
        iintstr++;

    }
    else  /* anything besides numerals ends a number*/
    {
        intstr[iintstr] = '\0';  /* ends a number */
        if(iintstr == 0)
        {
            *result = 0;
        }
        else
        {
            sscanf(intstr,"%d",result); /* need a pointer */
        }
        ireturn = 1;  /* some number was retrieved or invented*/
    }
    return(ireturn);
}
/*___getinteger()____________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/***threepointimpose()********************************************************/
int threepointimpose(float alphaxyz[3],float betaxyz[3],float nitexyz[3],char altchoice)
{
   /*superimpose target residue sidechain mutation onto original residue*/
   /*use old N, old CA, calc CB as three-point place to put mutated residue*/
   /*logic: put mutCA onto oldCA, aim mutCB at calcCB, turn mutN toward oldN*/
   /*original residue arrays of atom info are in common*/
   /*but need to use appropriate atoms for desired alt conformation, */
   /*which were determined in constructbeta() and passed as alphaxyz, nitexyz*/
   /*mutant   residue arrays of atom info are in common*/
   int ireturn=0,j=0;
   int LgotCB=0,LgotN=0,LgotCA=0;
   int jofmutN=0,jofmutCA=0,jofmutCB=0;
   double ax=0,ay=0,az=0,bx=0,by=0,bz=0,cx=0,cy=0,cz=0,cmag=0,dist=0;
   double axisbetas[3],xyz[3],dot=0,dmag=0,emag=0;
   double angle1=0,angle2=0;

   LgotN=0; LgotCA=0; LgotCB=0;
   /*mutateresidue() has already used */
   /*getonelibraryline(), one == old || new, and readbrkaalibrecord() */
   /*to load mutatom[][] ... mutz[][] arrays for target ideal residue*/
   for(j=1; j<=maxmut; j++)
   {/*search for atoms in new ideal library residue, which should have H's*/
               /*gly should never get here (ala may need to do H's)*/
         if(   (mutatom[j][1]=='n')
            && (mutatom[j][2]==' '))
         {
               LgotN = 1;
               jofmutN = j;
         }
         else if(   (mutatom[j][1]=='c')
                 && (mutatom[j][2]=='a'))
         {
               LgotCA = 1;
               jofmutCA = j;
         }
         else if(   (mutatom[j][1]=='c')
                 && (mutatom[j][2]=='b'))
         {
               LgotCB = 1;
               jofmutCB = j;
         }
         if(LgotN==1 && LgotCA==1 && LgotCB==1) {break;}
   }/*search for atoms in new ideal library residue */
   if(LgotN==1 && LgotCA==1 && LgotCB==1)
   {/*mutate and original residues OK: LgotN==1,LgotCA==1,LgotCB==1*/

      /*rotations, etc. operate around origin: conveniently 0,0,0 of CAmut*/
      /*(CBmut-CAmut) X (CBcalc-CAold) ==> axisbetas */
      /*(CBmut-CAmut) dot (CBcalc-CAold)  ==> cos(angle1) */
      /*doaxisrot(mutxyz, angle1, origin, axisbetas) , where origin is 0,0,0 */
      /*==> newmutxyz */

      /*dihedral4pt(newNmut,CAmut,newCBmut,Nold-CAold) ==> angle2*/
      /*doaxisrot(newmutxyz, angle2, origin, CBcalc-CAold) ==>newermutxyz */

      /*translate all newermutxyz points to origin at CAold*/



      /*(CBmut-CAmut) X (CBcalc-CAold) ==> axisbetas */
      ax = mutx[jofmutCB] - mutx[jofmutCA];
      ay = muty[jofmutCB] - muty[jofmutCA];
      az = mutz[jofmutCB] - mutz[jofmutCA];
      bx = betaxyz[0] - alphaxyz[0];
      by = betaxyz[1] - alphaxyz[1];
      bz = betaxyz[2] - alphaxyz[2];

      /* c = a x b */
      docrossproduct(ax,ay,az,bx,by,bz,&cx,&cy,&cz);/*return vector is c*/
      /*normalization factor*/
       cmag = sqrt( (cx)*(cx) + (cy)*(cy) + (cz)*(cz) ); /*c*/
      dist = 1;
      /*get components of c normalized*/
      if(cmag > 0.000001)
      {
         cx = cx*dist/cmag;
         cy = cy*dist/cmag;
         cz = cz*dist/cmag;
      }
      axisbetas[0] = cx;
      axisbetas[1] = cy;
      axisbetas[2] = cz;

      /*(CBmut-CAmut) dot (CBcalc-CAold) ==> cos(angle1) */
      dot = (ax*bx + ay*by + az*bz);
      /*normalization factors*/
      dmag = sqrt( (ax)*(ax) + (ay)*(ay) + (az)*(az) );
      emag = sqrt( (bx)*(bx) + (by)*(by) + (bz)*(bz) );
      if(dmag*emag <0.0001)
      {
          angle1 = 0.0;
      }
      else angle1 = acos( dot/(dmag*emag) );
      angle1 = angle1*360.0/(2*3.14159);

      /*doaxisrot(mutxyz, angle1, origin, axisbetas) , where origin is 0,0,0 */
      /*==> newmutxyz */
      for(j=1; j<=maxmut; j++)
      {
         xyz[0] = (double)mutx[j];
         xyz[1] = (double)muty[j];
         xyz[2] = (double)mutz[j];

         doaxisrot(&xyz[0],&xyz[1],&xyz[2],(float)angle1
                   ,0,0,0,axisbetas[0],axisbetas[1],axisbetas[2]);

         mutx[j] = (float)xyz[0];
         muty[j] = (float)xyz[1];
         mutz[j] = (float)xyz[2];
      }
      /*save rotation angle,axis in this file's commmon*/
      /*to be used if superimposed PDB record is needed  040303*/
      anglerot1 = angle1;
      cbcbaxis[0] = axisbetas[0];
      cbcbaxis[1] = axisbetas[1];
      cbcbaxis[2] = axisbetas[2];


      ax = nitexyz[0] - alphaxyz[0];
      ay = nitexyz[1] - alphaxyz[1];
      az = nitexyz[2] - alphaxyz[2];

      /*dihedral4pt(newNmut,CAmut,newCBmut,Nold-CAold) ==> angle2*/
      angle2 = dihedral4pt( mutx[jofmutN],muty[jofmutN],mutz[jofmutN]
                           ,mutx[jofmutCA],muty[jofmutCA],mutz[jofmutCA]
                           ,mutx[jofmutCB],muty[jofmutCB],mutz[jofmutCB]
                           ,ax,ay,az);

      bx = betaxyz[0] - alphaxyz[0];
      by = betaxyz[1] - alphaxyz[1];
      bz = betaxyz[2] - alphaxyz[2];


      /*doaxisrot(newmutxyz, angle2, origin, CBcalc-CAold) ==>newermutxyz */
      /*and translate all newermutxyz points to origin at CAold*/
      for(j=1; j<=maxmut; j++)
      {
         xyz[0] = (double)mutx[j];
         xyz[1] = (double)muty[j];
         xyz[2] = (double)mutz[j];
         doaxisrot(&xyz[0],&xyz[1],&xyz[2],(float)angle2
                   ,0,0,0,bx,by,bz);

         /*change origin to old CA position*/
         mutx[j] = (float)xyz[0] + alphaxyz[0];
         muty[j] = (float)xyz[1] + alphaxyz[1];
         mutz[j] = (float)xyz[2] + alphaxyz[2];
         /*insert alt conf character into 5th char of mut atom name*/
         mutatom[j][4] = altchoice; /*the original altchoice ...*/
      }
      for(j=1; j<=maxmut; j++)
      {

      }
      /*save rotation angle,axis,and center in this file's commmon*/
      /*to be used if superimposed PDB record is needed  040303*/
      anglerot2 = angle2;
      cacbcalc[0] = bx;
      cacbcalc[1] = by;
      cacbcalc[2] = bz;
      caxyz[0] = alphaxyz[0];
      caxyz[1] = alphaxyz[1];
      caxyz[2] = alphaxyz[2];

      ireturn = 1;
   }/*both orig and mut residue seem OK*/
   return(ireturn);
}
/*___threepointimpose()______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/***imposeontoideal()*********************************************************/
int imposeontoideal(float betaxyz[3])
{
   /*position original residue using ideal Cbeta into std orientation*/
   /*use old N, old CA of actual residue*/
   /*to calculate best compromise tauball for orienting residue*/
   /*superimpose old CA onto ideal CA, place CA--CB, then tauball direction*/
   /*original residue arrays of atom info are in common*/
   /*mutant   residue arrays of atom info are in common*/
   int j=0;
   int LgotCB=0,LgotN=0,LgotCA=0,LgotC=0,LOK=0;
   float npt[3],apt[3],tpt[3],cpt[3];
   double ax,ay,az,bx,by,bz,cx,cy,cz,cmag,dist,axisbetas[3],dmag,emag;
   double angle1,angle2,dot;
   double xyz[3];
   float newx[MAXARR],newy[MAXARR],newz[MAXARR];

   for(j=1; j<=maxatm; j++)
   {/*search for atoms in orig real structure residue */
       if(   (atom[j][1]=='n')
          && (atom[j][2]==' '))
       {
             LgotN = 1;
             npt[0] = x[j];
             npt[1] = y[j];
             npt[2] = z[j];
       }
       else if(   (atom[j][1]=='c')
               && (atom[j][2]=='a'))
       {
             LgotCA = 1;
             apt[0] = x[j];
             apt[1] = y[j];
             apt[2] = z[j];
       }
       else if(   (atom[j][1]=='c')
               && (atom[j][2]=='b'))
       {
             LgotCB = 1;
       }
       else if(   (atom[j][1]=='c')
               && (atom[j][2]==' '))
       {
             LgotC = 1;
             cpt[0] = x[j];
             cpt[1] = y[j];
             cpt[2] = z[j];
       }
       if(LgotN==1 && LgotCA==1 && LgotCB==1 && LgotC)
       {
          LOK = 1;
          break;
       }
   }/*search for atoms in orig real structure residue */
   if(LOK)
   {/*orig residue OK*/
     /*calc pseudo-atom at tau angle mid point*/
     LOK = midanglepoint(npt,apt,cpt,tpt); /*returns tpt*/
     {/*reposition orig residue*/
      /*rotations, etc. operate around origin: conveniently 0,0,0 of CA*/
      /*(CBideal-CA) X (CBstd-CAstd) ==> axisbetas */
      /*(CBideal-CA) dot (CBstd-CAstd)  ==> cos(angle1) */
      /*doaxisrot(xyz, angle1, origin, axisbetas) , where origin is 0,0,0 */
      /*==> newxyz for all orig atoms of residue plus pseudo-tau atom*/

      /*dihedral4pt(newT,CAstd,CBstd,Tstd) ==> angle2*/
      /*doaxisrot(newxyz, angle2, origin, CBstd-CAstd) ==>newermutxyz */

      /*results in translation of all newxyz points to origin at CA*/

      /*(CBideal-CA) X (CBstd-CAstd) ==> axisbetas */
      ax = betaxyz[0] - apt[0];
      ay = betaxyz[1] - apt[1];
      az = betaxyz[2] - apt[2];
      bx = 0; /* 0 - 0 */
      by = 1; /* 1 - 0 */
      bz = 0; /* 0 - 0 */

      /* c = a x b */
      docrossproduct(ax,ay,az,bx,by,bz,&cx,&cy,&cz);/*return vector is c*/
      /*normalization factor*/
       cmag = sqrt( (cx)*(cx) + (cy)*(cy) + (cz)*(cz) ); /*c*/
      dist = 1;
      /*get components of c normalized*/
      if(cmag > 0.000001)
      {
         cx = cx*dist/cmag;
         cy = cy*dist/cmag;
         cz = cz*dist/cmag;
      }
      axisbetas[0] = cx;
      axisbetas[1] = cy;
      axisbetas[2] = cz;

      /*(CBideal-CA) dot (CBstd-CAstd)  ==> cos(angle1) */
      dot = (ax*bx + ay*by + az*bz);
      /*normalization factors*/
      dmag = sqrt( (ax)*(ax) + (ay)*(ay) + (az)*(az) );
      emag = sqrt( (bx)*(bx) + (by)*(by) + (bz)*(bz) );
      if(dmag*emag <0.0001)
      {
          angle1 = 0.0;
      }
      else angle1 = acos( dot/(dmag*emag) );
      angle1 = angle1*360.0/(2*3.14159);

      /*doaxisrot(xyz, angle1, origin, axisbetas) , where origin is 0,0,0 */
      /*and xyz == orig xyz - orig CA*/
      /*==> newxyz */

      for(j=1; j<=maxatm; j++)
      {
         xyz[0] = (double)(x[j]-apt[0]);
         xyz[1] = (double)(y[j]-apt[1]);
         xyz[2] = (double)(z[j]-apt[2]);

         doaxisrot(&xyz[0],&xyz[1],&xyz[2],(float)angle1
                   ,0,0,0,axisbetas[0],axisbetas[1],axisbetas[2]);

         newx[j] = (float)xyz[0];
         newy[j] = (float)xyz[1];
         newz[j] = (float)xyz[2];
      }
      /*rotate tau pseudo-atom into maxatm+1 place in new coord array*/
      /*tauball already is referenced to origin of 0,0,0*/
      {
         xyz[0] = (double)(tpt[0]);
         xyz[1] = (double)(tpt[1]);
         xyz[2] = (double)(tpt[2]);

         doaxisrot(&xyz[0],&xyz[1],&xyz[2],(float)angle1
                   ,0,0,0,axisbetas[0],axisbetas[1],axisbetas[2]);

         tpt[0] = (float)xyz[0];
         tpt[1] = (float)xyz[1];
         tpt[2] = (float)xyz[2];
      }

      /*Tstd: 0,0,1  defined as std z axis*/
      /*dihedral4pt(newT,CAstd,CBstd,Tstd) ==> angle2*/
      angle2 = dihedral4pt( tpt[0],tpt[1],tpt[2]
                           ,0,0,0
                           ,0,1,0
                           ,0,0,1);
      /*doaxisrot(newxyz, angle2, origin, CBstd-CAstd) ==>newerxyz */
      /*newerxyz points are around origin at CAstd*/
      for(j=1; j<=(maxatm); j++)
      {
         xyz[0] = (double)newx[j];
         xyz[1] = (double)newy[j];
         xyz[2] = (double)newz[j];

         doaxisrot(&xyz[0],&xyz[1],&xyz[2],(float)angle2
                   ,0,0,0,  0,1,0);

         /*origin is std CA position, so no translation needed*/
         /*put back into atom arrays*/
         x[j] = (float)xyz[0];
         y[j] = (float)xyz[1];
         z[j] = (float)xyz[2];
      }
         xyz[0] = (double)tpt[0];
         xyz[1] = (double)tpt[1];
         xyz[2] = (double)tpt[2];

         doaxisrot(&xyz[0],&xyz[1],&xyz[2],(float)angle2
                   ,0,0,0,  0,1,0);

         /*origin is std CA position, so no translation needed*/
         /*put back into atom arrays*/
         tpt[0] = (float)xyz[0];
         tpt[1] = (float)xyz[1];
         tpt[2] = (float)xyz[2];
      /*reloadresidue inc mainchain*/
     }/*reposition orig residue*/
   }/*orig residue OK*/
   return(LOK);
}
/*___imposeontoideal()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****midanglepoint()**********************************************************/
int midanglepoint(float npt[3],float apt[3],float cpt[3],float tpt[3])
{
   double an[3],ac[3],anmag=0,acmag=0;
   int ireturn=0;

   an[0] = (double)npt[0] - (double)apt[0]; /*vector A-->N*/
   an[1] = (double)npt[1] - (double)apt[1];
   an[2] = (double)npt[2] - (double)apt[2];

   ac[0] = (double)cpt[0] - (double)apt[0]; /*vector A-->C*/
   ac[1] = (double)cpt[1] - (double)apt[1];
   ac[2] = (double)cpt[2] - (double)apt[2];
   /*normalize vectors*/
   anmag = sqrt( an[0]*an[0] + an[1]*an[1] + an[2]*an[2] );
   acmag = sqrt( ac[0]*ac[0] + ac[1]*ac[1] + ac[2]*ac[2] );
   if(anmag > 0.0001 && acmag > 0.0001)
   {
      an[0] = an[0]/anmag;
      an[1] = an[1]/anmag;
      an[2] = an[2]/anmag;
      ac[0] = ac[0]/acmag;
      ac[1] = ac[1]/acmag;
      ac[2] = ac[2]/acmag;
      tpt[0] = (float)((an[0] + ac[0])/2);
      tpt[1] = (float)((an[1] + ac[1])/2);
      tpt[2] = (float)((an[2] + ac[2])/2);
      ireturn = 1;
   }
   else
   {
      ireturn = 0;
   }
   return(ireturn);
}
/*___midanglepoint()_________________________________________________________*/

/****accumsiteranges()********************************************************/
void accumsiteranges(char texts[256]) /*050731*/
{/*accumulates ranges when checkline() finds a SITE record*/
    char   rstr[5];

/*four site residues per SITE record line
         1         2         3         4         5         6         7
1234567890123456789012345678901234567890123456789012345678901234567890
0123456789012345678901234567890123456789012345678901234567890
SITE     1 DTA  3 ASP A  25  THR A  26  GLY A  27
SITE     1 DTB  3 ASP B  25  THR B  26  GLY B  27
SITE     1   A  4   U A  44    C A  46    G A  61    U A 118
SITE     1 ZN1  5 CYS A  97  CYS A 100  CYS A 103  CYS A 111
SITE     2 ZN1  5  ZN A 375
                      a_i4_a     a_i4_a     a_i4_a     a_i4_a
                      chain       resnum         insertioncode
0123456789012345678901234567890123456789012345678901234567890
*/
      /*each site becomes a range, limit Nsite now, limit mrange later*/
      if(   (strlen(texts) >= 27) && (texts[26] != ' ')
         && (Nsite < (MAXR-1) ) )
      {
        rstr[0]=texts[23];rstr[1]=texts[24];rstr[2]=texts[25];rstr[3]=texts[26];
        rstr[4]= '\0';
          siteresnum[++Nsite] = intfromstr(rstr);
          sitechain[Nsite][0] = (texts[21]); /*case, 2char 070926*/
          sitechain[Nsite][1] = (texts[22]); /*case, 2char 070926*/
          if( strlen(texts) >= 28) {siteinsert[Nsite]=tolower(texts[27]);}
      }
      if(   (strlen(texts) >= 38) && (texts[37] != ' ')
         && (Nsite < (MAXR-1) ) )
      {
        rstr[0]=texts[34];rstr[1]=texts[35];rstr[2]=texts[36];rstr[3]=texts[37];
        rstr[4]= '\0';
          siteresnum[++Nsite] = intfromstr(rstr);
          sitechain[Nsite][0] = (texts[32]); /*case, 2char 070926*/
          sitechain[Nsite][1] = (texts[33]); /*case, 2char 070926*/
          if( strlen(texts) >= 39) {siteinsert[Nsite]=tolower(texts[38]);}
      }
      if(   (strlen(texts) >= 49) && (texts[48] != ' ')
         && (Nsite < (MAXR-1) ) )
      {
        rstr[0]=texts[45];rstr[1]=texts[46];rstr[2]=texts[47];rstr[3]=texts[48];
        rstr[4]= '\0';
          siteresnum[++Nsite] = intfromstr(rstr);
          sitechain[Nsite][0] = (texts[43]); /*case, 2char 070926*/
          sitechain[Nsite][1] = (texts[44]); /*case, 2char 070926*/
          if( strlen(texts) >= 40) {siteinsert[Nsite]=tolower(texts[39]);}
      }
      if(   (strlen(texts) >= 60) && (texts[59] != ' ')
         && (Nsite < (MAXR-1) ) )
      {
        rstr[0]=texts[56];rstr[1]=texts[57];rstr[2]=texts[58];rstr[3]=texts[59];
        rstr[4]= '\0';
          siteresnum[++Nsite] = intfromstr(rstr);
          sitechain[Nsite][0] = (texts[54]); /*case, 2char 070926*/
          sitechain[Nsite][1] = (texts[55]); /*case, 2char 070926*/
          if( strlen(texts) >= 61) {siteinsert[Nsite]=tolower(texts[60]);}
      }
}
/*___accumsiteranges()________________________________________________________*/

/*obseleted 050731*/
/****computesiteranges()*******************************************************/
void computesiteranges() /*050731 reaches here with Nsite > 0 */
{/*compute ranges from SITE records when checkline() finds first ATOM record*/
   int    k=0,n=0;

   for(k=1; k <= Nsite; k++)
   {
      if(mrange < (MAXR-2) )
      {
         n = ++mrange;
         resnu1[n] = resnu2[n] = siteresnum[k];
         /*chainID and insertion code NOT commandline inputable for ranges*/
         sitel[n] = 1;
         scxl[n] = 1;
      }
   }
}
/*___computesiteranges()______________________________________________________*/

