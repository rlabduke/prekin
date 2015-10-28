/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*                     PKINCOUT.c           */
#include "PKIN.h"
#include "PKINDLOG.h"

/*needed in several routines in this source file:*/
static char topname[32]={"                               "};
//void writeVRMLvector(char[256]); /*050211*/
void writeVRMLvectorextrusion(char[256]); /*050208*/
void writeVRMLsphere(char[256],char[20]); /*050208*/
void VRMLxSectionCircle(float);  /*050204*/
int  LsplineVRMLout=0, LvectorVRMLout=0, LsphereVRMLout=0;/*050208*/
     /* local specific output control*/
static float VRMLx=0,VRMLy=0,VRMLz=0;

/****writeoutput()***********************************************************/
void   writeoutput()
{
  char  cntl[5];  /* 4 actual characters */
  char  kol[20],extra[256],aname[13];
  char  chain[32],atoms[32],cpks[32],hygen[32],hbond[32];
  char  subname[32],allstr[32];
  char  mastername[32]; /*051128*/
  char  domrec1[32],domrec2[32]; /*040406 for dominant or recessiveon*/
  char  alphcol[16],betacol[16],coilcol[16],nuclcol[16]; /*041029*/
static char  topnameold[32]={"                               "};
static char onlynameold[32]={"                               "}; /*050922*/
  int    j,k,Ldoit;
  char basetype[3];
static char onlyname[32]={"                               "};
  float ballradiushold=0;
  int  NMOD=0, Ncase=0; /*041106 colorset distinguish multiple subunits*/
  char ribidentityold[5]={'\0','\0','\0','\0','\0'}; /*060929*/
  char colsetstr[6][16]; /*060929*/
  int  numcolsets = 6;   /*060929*/
  int  indexribID = 0;   /*060929*/


   if(   Ldumppperptobase || Lcbdevdump || Lpdbmutout || Ldumpcispeptides
      || LdumpVRML || Lconnectresiduesdump)
   {
      Ldump = 1; /*generic control for dump mode not a kin*/
   }

/*Listca,Listmc,Listsc,Listhy,Listhb,Listss,Listlb,Listrot*/
/*mcoutl,scoutl,lboutl,croutl,htoutl,hyoutl,hboutl,cxoutl,caonly,caoutl*/
/*rotoutl,riboutl,extoutl*/

/*#define PRINTLISTS*/
#ifdef PRINTLISTS
fprintf(stderr,"Listmc %d, Listca %d, Listsc %d, Listss %d, Listhy %d, Listht %d, Listhb %d, Listwa %d, Listwao %d, Listat %d, Listlb %d, Listrot %d, Listrib %d, Listext %d, Listtabl %d, Listcbdev %d, Listtaudev %d, Ldump %d, Listsuppos %d, Listpdb %d\n"
,Listmc,Listca,Listsc,Listss,Listhy,Listht,Listhb,Listwa,Listwao,Listat,Listlb
,Listrot,Listrib,Listext,Listtabl,Listcbdev,Listtaudev,Ldump,Listsuppos,Listpdb);
#endif /*PRINTLISTS*/

  if(LIOpipe) /*020514*/
  {
     fprintf(fpoutput,EOLO"@start"EOLO); /*020514*/
  }
  /*There can be either kinemage format or colon:delineated format output*/
  /*check that some output has been requested, and points have been found*/
  /*as of 030520 do not correlate type of request with type of found points*/
  if(!Ldump
     &&(   Listmc
        || Listca
        || Listsc
        || Listss
        || Listhy
        || Listht
        || Listhb
        || Listwa
        || Listat
        || Listlb
        || Listrot
        || Listrib
        || Listext
        || Listtabl
        || Listcbdev
        || Listtaudev
        || Listsuppos /*uses typenow*/
       )
     &&(   pointcnt[typenull]
        || pointcnt[typemc]
        || pointcnt[typemcn]
        || pointcnt[typemco]
        || pointcnt[typemcs]
        || pointcnt[typemcc]
        || pointcnt[typesc]
        || pointcnt[typescn]
        || pointcnt[typesco]
        || pointcnt[typescs]
        || pointcnt[typescc]
        || pointcnt[typeca]
        || pointcnt[typerib]
        || pointcnt[typelb]
        || pointcnt[typelbn]
        || pointcnt[typehb]
        || pointcnt[typehy]
        || pointcnt[typess]
        || pointcnt[typeht]
        || pointcnt[typehtn]
        || pointcnt[typehto]
        || pointcnt[typehts]
        || pointcnt[typehtm]
        || pointcnt[typehtc]
        || pointcnt[typewa]
        || pointcnt[typewao]
        || pointcnt[typeext]
        || pointcnt[typecbda]
        || pointcnt[typecbdb]
        || pointcnt[typecbdc]
        || pointcnt[typecbdm]
        || pointcnt[typecbdv]
        || pointcnt[typecbdl]
        || pointcnt[typecbde]
        || pointcnt[typetau]
        || pointcnt[typetaub]
        || pointcnt[typetaus]
        || pointcnt[typero]
       )
    )
  {/*there is kinemage stuff to output*/

    if(Listsuppos)
    {/*trumps all other possible output this pass*/
       Listmc = 0; Listca = 0;  Listsc = 0; Listss = 0; Listhy  = 0;
       Listht = 0; Listhb = 0; Listwa = 0; Listat = 0; Listlb = 0;
       Listrot = 0; Listrib = 0; Listext = 0; Listtabl = 0; Listcbdev = 0;
       Listtaudev = 0;
    }
    if(!Ldid_outfiletext && !Ldump) /*set in ____OUTP/prepareoutputfile()*/
    {/*Do kinemage text and caption*/
        if(!Lappend)
        {/*NOT Lappend: presume a whole new kinemage*/

           if(!Lnotext)
           {/*write text for text window*/
#ifdef PCWINDOWS
             fprintf(fpoutput,"@text "EOLO" coordinates from file: %s "EOLO
                   ,dirstrip(NameStr));
#endif
#ifndef PCWINDOWS
             fprintf(fpoutput,"@text "EOLO" coordinates from file: %s "EOLO
                   ,NameStr);
#endif
             if(LIOmage)
             {
/*fprintf(stderr,"Prekin writing pdbinfostuff under @text\n");*/
                rewindtextblock(&pdbinfostuff);
                do
                {
                   getonetextblockline(&pdbinfostuff,word);
                   if(word[0]!='\0') fprintf(fpoutput,"%s",word);
                }while(word[0]!='\0');
             }
             else
             {
/*fprintf(stderr,"Prekin writing headerstuff under @text\n");*/
                rewindtextblock(&headerstuff);
                do
                {
                   getonetextblockline(&headerstuff,word);
                   if(word[0]!='\0') fprintf(fpoutput,"%s",word);
                }while(word[0]!='\0');
             }
           }/*write text for text window*/



           fprintf(fpoutput,EOLO"@kinemage 1 "EOLO); /*990118*/
           fprintf(fpoutput,EOLO"@prekin %.2f %s"EOLO,PREKINVERSION,PREKINDATE);
#ifdef PCWINDOWS
           fprintf(fpoutput,"@caption "EOLO" coordinates from file: %s "EOLO
                  ,dirstrip(NameStr));
#endif
#ifndef PCWINDOWS
           fprintf(fpoutput,"@caption "EOLO" coordinates from file: %s "EOLO
                  ,NameStr);
#endif
           /*if(fprintf(fpoutput,"@onewidth "EOLO)<0) Lfpoutput=-1;*/
           /*(fprintf(fpoutput,"@onewidth "EOLO)<0); fix warning 130507 JJH */
           fprintf(fpoutput,"@onewidth "EOLO); /*130121 remove (...<0) */
        }/*NOT Lappend: presume a whole new kinemage*/
        /*write file name so MAGE could call prekin or probe to read it*/
        fprintf(fpoutput,"@pdbfile {%s} "EOLO,ShortNameStr);

        if(Lpasskeyword) /*041107*/
        {
           if(passkeywordstr[0] != '@')
                {fprintf(fpoutput,"@%s"EOLO,passkeywordstr);} /*041107*/
           else {fprintf(fpoutput, "%s"EOLO,passkeywordstr);} /*041107*/
        }

        Ldid_outfiletext = 1;
    }/*Do kinemage text and caption*/

    if(Lhypertextstuff)
    {
        rewindtextblock(&hypertextstuff);
        do
        {
            getonetextblockline(&hypertextstuff,word);
            if(word[0]!='\0') fprintf(fpoutput,"%s",word);
        }while(word[0]!='\0');
        Lhypertextstuff = 0;
        disposetextblock(&hypertextstuff); /*PKINSCRT.c*/
    }

    if(Lallbuttons || Listcbdev  || Listtaudev || Listsuppos)
    {
       Lnogroups = 0;
       Lonlygroups = 0;
       Lonegroup = 0;
    }

    /*use: mod[maxatm] for actual model number of current piece of molecule */
    /*note nmrmodel changes at transition when next is another model*/
    /*050121 modelnumber is set to current model being processed*/


    if(nmrmodel[0] != '\0') /*nmrmodel set so presumably doing nmrmodels*/
    {
       if(Lcommanded && Listrot) /*050121 Lcommanded may not be robust...*/
       {/*e.g. mage command for rotatable sidechain invoked prekin*/
          sprintf(allstr," master= {rotation}");
       }
       else if(Listrot)
       {/*050121 seems prekin requested to make rotatable mutated sidechain*/
          sprintf(allstr," master= {rotation}");
       }
       else /*probably multiple nmr models so set a masters to control them*/
       {
          sprintf(allstr," animate master= {all}");
       }
    }
    else if(Lnobutton) /*060120*/
    {
       sprintf(allstr," animate nobutton master={ALL}"); /* < 32 char */
    }
    else if(Lanimate)
    {
       sprintf(allstr," animate");
    }
    else
    {
       allstr[0]='\0';
    }

    sprintf(hbond," master= {Hbonds}");
    sprintf(hygen," master= {H's}");
    sprintf(atoms," master= {atoms}");
    sprintf(cpks," master= {CPK}");

    /*derive names for whole molecule and subunits*/
    /*topID may have both subunit indicator and nmrmodel number*/
    /*050121 actually, topID does NOT seem to ever have nmrmodel number!*/
    /*not an nmr MODEL will have nmrmodel[0]=='\0', a null string 050121*/
    /*  null str will have null spaces in %s format conversion*/
    /* historically: Listrot inhibited showing chainID in group name*/
    /*note nmrmodel changes at transition when next is another model*/
    /*050121 modelnumber is set to current model being processed*/
    if(MolNameStr[0] == '\0')
    {
        sprintf(chain,"subunit%s",modelnumber); /*050121*/
        sprintf(topname,"molec%s%s",topID,modelnumber); /*050121*/
        sprintf(onlyname,"molec%s",modelnumber); /*050121*/
         /*e.g. for Lonegroup do not want chainID in group name*/
        /*sprintf(topname,"molec");*/
    }
    else
    {
      /*sprintf(chain,"%s%s",MolNameStr,modelnumber);*/
        sprintf(chain,"%s",MolNameStr);
        sprintf(topname,"%s%s%s",MolNameStr,topID,modelnumber); /*050121*/
        sprintf(onlyname,"%s%s",MolNameStr,modelnumber); /*050121*/
         /*e.g. for Lonegroup do not want chainID in group name*/
        /*sprintf(topname,"%s",MolNameStr);*/
    }
    if(Listrot && Lcommanded)
    {/*over-ride @group {topname...} with sidechain name*/
       /*e.g. mage command for just a rotatable sidechain*/
       sprintf(topname,"%s",rotatorname); /*050121 includes model*/
    }
/*
sprintf(temps,CRLF"output Lthispass %d, Ngroups %d, Nchainfirstpass %d, ichainthispass %d",Lthispass,Ngroups,Nchainfirstpass,ichainthispass);
       pkintextinsert(temps);
       adjusttext(1);
*/
    /*colorcoded aspects for individual atoms have labels*/
    /*either passed in pdb file header records for inputted aspects*/
    /*or generated for internally specified aspects*/
    if(MAXaspectcode >0 )
    {
       for(j=1; j<=MAXaspectcode; j++)
       {
          fprintf(fpoutput,"@%daspect {%s}"EOLO,j,aspectid[j]);
          /* which reads: @...%d...aspect : i.e. @0aspect, @1aspect, @2aspect, ...*/
       }
    }

    if(Lthispass == 2 &&!Ldump)
    {/*masters layout*/ /*general at 2nd pass 030503*/

       /*Prekin dry first pass to discover file structure, subunits, etc.*/
       /*Really needed if(Lonegroup||Lnogroups) when hierarchy hidden*/
       /*nmr MODEL invokes Lonegroup mode*/   /*991210*/
       if(   !Lnosubgroupmasters
          && Ltopout==0
          && Ngroups==0
          && (Lonegroup||Lnogroups) )
       {/*masters layout: only once, only in 2nd pass, only at first subunit*/
          /*can't do this if specify NO subgroup masters on commandline*/
          /* PKININIT default is Lnosubgroupmasters == 1 as of 011028 */
#ifdef OLDMASTERSETUP  /*051128 this is a mess with chains and subunits*/
          if(Nchainfirstpass>1 || Lnogroups)
          {/*set up masters for molecule and subunits*/
             if(   Nchainfirstpass == 2
                && (  typechainfirstpass[2] == 'H'
                    ||typechainfirstpass[2] == 'W')
               )
             {
                fprintf(fpoutput,"@master {%s}"EOLO,chain);
                if(Listht) {fprintf(fpoutput,"@master {%s hets}"EOLO,chain);}
             }
             else if(Nchainfirstpass >1)
             {
                for(j=1; j<=Nchainfirstpass; j++)
                {
                   Ldoit = 1; /*allow master specification for this chain*/
                   if(Nchaincharcode > 0)
                   {/*chain IDs specified by a command line:*/
                      /* make masters only for those*/
                      Ldoit = 0; /*inhibit master specification*/
                      for(k=0; k<Nchaincharcode; k++)
                      {  /*chainID case sensitive for pdb v3.0 070926*/
                         if(  (chaincharcode[k][0] ==codechainfirstpass[j][0])
                            &&(chaincharcode[k][1] ==codechainfirstpass[j][1]))
                           )
                         {/*Match of chain IDs allowed to present chain ID*/
                            Ldoit = 1;
                            break; /*any one match allows this chain*/
                         }
                      }
                   }
                   if(Ldoit)
                   {
                      if(typechainfirstpass[j]=='H')
                      {
                         fprintf(fpoutput,"@master {%s hets}"EOLO,chain);
                      }
                      else
                      {
                         fprintf(fpoutput,"@master {%s %s}"EOLO
                               ,chain,codechainfirstpass[j]);
                      }
                   }
                }
             }
             else
             {
                if(!Lnosubgroupmasters)
                   {fprintf(fpoutput,"@master {%s}"EOLO,chain);}
             }
          }/*set up masters for molecule and subunits*/
#endif  /*051128*/
          /*micromanage by type of output*/
          if(!Lallbases) /*which wreaks havoc with masters anyway*/
          {/*set up masters for components*/ /*only do this in the beginning*/
             /*mc,ca,sc,hy,even hb usually set as List.. in first level*/
             /*nt sometimes set in first level, but wa rarely. */
             /*?more robust to use ..outl so exist in a range setting */
             /*plus existence of a type: typechainfirstpass[level] */
             /* Ltype_ to anticipate need for ordered masters 051201*/
             if(mcoutl && (LtypeP || LtypeN) ) /*if(Listmc)*/
             {
                /*fprintf(fpoutput,"@master {mainchain} indent space"EOLO);*/
                fprintf(fpoutput,"@master {mainchain} indent"EOLO); /*060212*/
                  /*space not neede here, both*/
                  /*Mage and KiNG put a space between hierarchy and masters*/
             }
             if(caoutl && (LtypeP || LtypeN) ) /*pseudo-bb incl if(Listca)*/
             {
                if(LtypeN)
                {
                   fprintf(fpoutput,"@master {Virtual BB} indent"EOLO);
                }
                else
                {
                   fprintf(fpoutput,"@master {Calphas} indent"EOLO);
                }
             }
             if(scoutl && (LtypeP || LtypeN) ) /*bases incl if(Listsc)*/
             {
                fprintf(fpoutput,"@master {sidechain} indent"EOLO);
             }
             if(hyoutl && (LtypeP || LtypeN) ) /*Listhy*/
             {
                fprintf(fpoutput,"@master {H's} indent"EOLO);
                fprintf(fpoutput,"@pointmaster 'H' {H's}"EOLO);
             }
             if(hboutl && (LtypeP || LtypeN) ) /*Listhb*/
             {
                fprintf(fpoutput,"@master {Hbonds} indent"EOLO);
             }
             if(htoutl && LtypeH ) /*Listht*/
             {
                fprintf(fpoutput,"@master {hets} indent"EOLO);
             }
             if(waoutl && LtypeW ) /*Listwa*/
             {
                fprintf(fpoutput,"@master {water} indent"EOLO);
             }
          }/*set up masters for components*/
       }/*masters layout: only once, only in 2nd pass, only at first subunit*/
       else if(Lallbases)
       {/*Lallbases presumes mainchain, pseudo_mc, and sidechain at least!*/
          fprintf(fpoutput,"@master {mainchain}"EOLO);
          fprintf(fpoutput,"@master {pseudo_mc}"EOLO);
          fprintf(fpoutput,"@master {sidechain}"EOLO);
          if(Listhy) fprintf(fpoutput,"@master {H's}"EOLO);
       }
       /*041031 no cbetadev pointmasters*/
       if(Listcbdev)
       {/*this should be with the other master controls, */
        /*but cbdev is a special case hacked onto prekin's hack commands*/
        /*and the interaction of masters with groups is confuscated*/
        /*Remember: <...> means an ignored comment to Mage! */
             /*fprintf(fpoutput,"@master {dev_.05} indent"EOLO);*/
             /*fprintf(fpoutput,"@master {dev.05-.1} indent"EOLO);*/
             /*fprintf(fpoutput,"@master {dev.1-.2} indent"EOLO);*/
             /*fprintf(fpoutput,"@master {dev.2-.3} indent"EOLO);*/
             /*fprintf(fpoutput,"@master {dev.3-.4} indent"EOLO);*/
             /*fprintf(fpoutput,"@master {dev.4-.5} indent"EOLO);*/
             /*fprintf(fpoutput,"@master {dev_.5} indent"EOLO);*/
             /*fprintf(fpoutput,"@pointmaster 't' {dev_.05}"EOLO);*/
             /*fprintf(fpoutput,"@pointmaster 'u' {dev.05-.1}"EOLO);*/
             /*fprintf(fpoutput,"@pointmaster 'v' {dev.1-.2}"EOLO);*/
             /*fprintf(fpoutput,"@pointmaster 'w' {dev.2-.3}"EOLO);*/
             /*fprintf(fpoutput,"@pointmaster 'x' {dev.3-.4}"EOLO);*/
             /*fprintf(fpoutput,"@pointmaster 'y' {dev.4-.5}"EOLO);*/
             /*fprintf(fpoutput,"@pointmaster 'z' {dev_.5}"EOLO);*/
       }
       if(Lkinmasters && (Listsuppos || Lplotpperptobase) && !Lappend)/*030527*/
       {
          fprintf(fpoutput,"@master {B 0-10} indent"EOLO);
          fprintf(fpoutput,"@master {B10-20} indent"EOLO);
          fprintf(fpoutput,"@master {B20-30} indent"EOLO);
          fprintf(fpoutput,"@master {B30-40} indent"EOLO);
          fprintf(fpoutput,"@master {B40-50} indent"EOLO);
          fprintf(fpoutput,"@master {B50-60} indent"EOLO);
          fprintf(fpoutput,"@master {B60-70} indent"EOLO);
          fprintf(fpoutput,"@master {B70-80} indent"EOLO);
          fprintf(fpoutput,"@master {B80-90} indent"EOLO);
          fprintf(fpoutput,"@master {B90-100} indent"EOLO);
          fprintf(fpoutput,"@master {B100-} indent"EOLO);
       }
       if(Lkinmasters && Lplotpperptobase && !Lappend) /*030527*/
       {
          fprintf(fpoutput,"@pointmaster 'p' {B 0-10}"EOLO);
          fprintf(fpoutput,"@pointmaster 'q' {B10-20}"EOLO);
          fprintf(fpoutput,"@pointmaster 'r' {B20-30}"EOLO);
          fprintf(fpoutput,"@pointmaster 's' {B30-40}"EOLO);
          fprintf(fpoutput,"@pointmaster 't' {B40-50}"EOLO);
          fprintf(fpoutput,"@pointmaster 'u' {B50-60}"EOLO);
          fprintf(fpoutput,"@pointmaster 'v' {B60-70}"EOLO);
          fprintf(fpoutput,"@pointmaster 'w' {B70-80}"EOLO);
          fprintf(fpoutput,"@pointmaster 'x' {B80-90}"EOLO);
          fprintf(fpoutput,"@pointmaster 'y' {B90-100}"EOLO);
          fprintf(fpoutput,"@pointmaster 'z' {B100-}"EOLO);
          /*additional axes group*/
          fprintf(fpoutput,"@group {axes} dominant"EOLO);
          fprintf(fpoutput,"@vectorlist {axes} dominant"EOLO);
          fprintf(fpoutput,"{6,0,0}P 6,0,0 white 16, 0, 0"EOLO);
          fprintf(fpoutput,"{6,0,0}P 6,0,0 gold   6,10, 0"EOLO);
          fprintf(fpoutput,"{6,0,0}P 6,0,0 green  6, 0,10"EOLO);
          fprintf(fpoutput,"@labellist {axes} dominant"EOLO);
          fprintf(fpoutput,"{delta/10}  16, 0, 0"EOLO);
          fprintf(fpoutput,"{3'P} gold   6,10, 0"EOLO);
          fprintf(fpoutput,"{5'P} green  6, 0,10"EOLO);
          /*040620 axes permuted as should have been done 040229*/
       }
       if(Lpointmasteralta) /*030503*/
       {
          fprintf(fpoutput,"@pointmaster 'a' {alta} on"EOLO);
       }
       if(Lpointmasteraltb)
       {
          fprintf(fpoutput,"@pointmaster 'b' {altb} off"EOLO);
       }
       if(Lpointmasteraltc)
       {
          fprintf(fpoutput,"@pointmaster 'c' {altc} off"EOLO);
       }
       Lkinmasters = 0;
    }/*masters layout*/ /*general at 2nd pass 030503*/

/*@group line output***********************************************************/
    if( Lonlygroups || Lonegroup )
    {
       /*if(Lallaminoacids||Lallbases)*/
       if(Lallbases)
       {
          sprintf(extra," dominant"); /*040406*/
          sprintf(domrec1," dominant"); /*040406*/
          sprintf(domrec2," dominant"); /*040406*/
       }
       else if(Lallaminoacids)
       {
          sprintf(extra," recessiveon"); /*020614*/
          sprintf(domrec1," dominant"); /*040406*/
          sprintf(domrec2," recessiveon"); /*040406*/
       }
       else
       {
          sprintf(extra," dominant");
          sprintf(domrec1," dominant"); /*040406*/
          sprintf(domrec2," dominant"); /*040406*/
       }
    }
    else if(Listcbdev) /*041031 dominant group named cbetadev */
    {
       sprintf(extra," dominant");
       sprintf(domrec1," dominant");
       sprintf(domrec2," dominant");
       sprintf(topname,"cbetadev");
    }
    else
    {
       extra[0] = '\0';
       domrec1[0] = '\0'; /*040406*/
       domrec2[0] = '\0'; /*040406*/
    }

    if(!Lnogroups && !Ldump)
    {/*write @group...*/
      if(Ltable)   /*991122*/
      {/*exlusive pass to make a table group*/
         /*group as well as list line done in transferout()*/
         /*
         if(MolNameStr[0] == '\0')
              fprintf(fpoutput,"@group {ROW %d} table \n",level);
         else fprintf(fpoutput,"@group {%s %d} table \n",MolNameStr,level);
         */
      }
      else if(    ( (Lallaminoacids||Lallbases) && iresiduecnt == 1)  /*ca-ca*/
               || (!Lallaminoacids &&!Lallbases && morsub == 1 && level == 1)
               || (!Lallaminoacids &&!Lallbases && level > 1 )                )
      {/*expect more than one level*/
        if(Lonegroup) /*takes precedence*/
        {/*Lonegroup and button for this molecule or model*/
          if(Ngroups==0)
          {/*first group of this pass or MODEL*/
            /*not an nmr MODEL will have nmrmodel[0]=='\0', a null string*/
            /*or just use sub indicator anyway, which inc nmrmodel*/
            /*if(strcmp(topnameold,topname))*/
            if(strcmp(onlynameold,onlyname))   /*050922*/
            {/*not the same*/
               /*fprintf(fpoutput,"@group {%s}%s%s"EOLO,topname,extra,allstr);*/
               /*BUT group name should not have a chain ID*/
               fprintf(fpoutput,"@group {%s}%s%s"EOLO,onlyname,extra,allstr);
               /*strcpy(topnameold,topname);*/
               strcpy(onlynameold,onlyname); /*050922*/
               ichainthispass = 1;
               Ngroups = Ngroups + 1;
            }
          }/*first group of this pass or model*/
          /*write subgroup for this chain*/
          /*for Lonegroup there are masters on all lists*/
          if(   Nchainfirstpass > 1
             && !Lnosubgroupmasters
             && !Listrot
             && !Lnosubgroups) /*051129*/
          {/*set up subgroups with masters for subunits*/
            /*Listrot writes it's own subgroup line below  050121*/
          /*master subgroup if >1 chain, unless commandline -nosubgroupmasters*/
            if(   Nchainfirstpass == 2
               && (  typechainfirstpass[2] == 'H'
                   ||typechainfirstpass[2] == 'W')
              )
            {/*invent subname for one subunit and its hets*/
               if(ichainthispass == 1) /*first: subunit likely protein or nucl*/
               {
                  sprintf(subname,"%s",chain);
                  sprintf(mastername,"chain %s",ChainID[level]); /*051201*/
               }
               else /*second: hets or water, name accordingly   051201*/
               {
                  if(typechainfirstpass[2] == 'H')
                  {
                     sprintf(subname,"%s hets",chain);
                     sprintf(mastername,"hets");
                  }
                  else if(typechainfirstpass[2] == 'W')
                  {
                     sprintf(subname,"%s water",chain);
                     sprintf(mastername,"water");
                  }
                  else
                  {
                     sprintf(subname,"%s",chain);
                     sprintf(mastername,"%s",chain);
                  }
               }
            }
            else
            {/*invent subname for one of multiple subunits*/
             if(typechainfirstpass[ichainthispass]=='H')
             {
                sprintf(subname,"%s hets",chain);
                sprintf(mastername,"hets"); /*051128*/
             }
             if(typechainfirstpass[ichainthispass]=='W') /*051201*/
             {
                sprintf(subname,"%s water",chain);
                sprintf(mastername,"water");
             }
             else if(  (codechainfirstpass[ichainthispass][0] != ' ') /*070926*/
                     &&(codechainfirstpass[ichainthispass][1] != ' '))/*070926*/
             {
              sprintf(subname,"%s %s",chain,codechainfirstpass[ichainthispass]);
                sprintf(mastername,"chain %s",ChainID[level]); /*051128*/
             }
             else
             {
                sprintf(subname,"%s %d",chain,ichainthispass);
                sprintf(mastername,"chain %s",ChainID[level]); /*051128*/
             }
            }

            /* fprintf(fpoutput,"@subgroup {%s}%s master= {%s}"EOLO */
            /*    ,subname,extra,subname); */

            fprintf(fpoutput,"@subgroup {%s}%s master= {%s}"EOLO
               ,subname,extra,mastername); /*051128*/

            ichainthispass++;
          }/*set up subgroups with masters for subunits*/
          else if(!Listrot && !Lnosubgroups) /*051129*/
          {/*single chain, no separate subgroups, extra may be dominant*/
            /*Listrot writes it's own subgroup line below  050121*/
             fprintf(fpoutput,"@subgroup {%s}%s"EOLO,chain,extra);
          }
        }/*Lonegroup and button for this molecule or model*/
        else
        {/*no restrictions OR Lonlygroups where extra==dominant */
           if( (Lallaminoacids||Lallbases) && iresiduecnt == 1)
           {/*test level, whatever level is highest at this point*/
            /*040406 replace extra by domrec1*/
            if(typechainfirstpass[level]=='N' || typechainfirstpass[level]=='P')
            {
              fprintf(fpoutput,"@group {%s mc}%s%s"EOLO,topname,domrec1,allstr);
            }
            else if(typechainfirstpass[level]=='H')
            {
              fprintf(fpoutput,"@group {%s hets}%s%s"EOLO,topname,domrec1,allstr);
            }
            else if(typechainfirstpass[level]=='W')
            {
              fprintf(fpoutput,"@group {%s H2O}%s%s"EOLO,topname,domrec1,allstr);
            }
           }
           else
           {
               fprintf(fpoutput,"@group {%s}%s%s"EOLO,topname,extra,allstr);
           }
           strcpy(topnameold,topname);
           ichainthispass = 1;
           Ngroups = Ngroups + 1;
        }/*no restrictions OR Lonlygroups where extra==dominant */
      }/*expect more than one level*/
      else if(Lallaminoacids && iresiduecnt == 2 && morsub == 1 && level == 1)
      {/*special for sets of sidechain type*/
          fprintf(fpoutput,"@group {sidechains}%s"EOLO,extra);
          Ngroups = Ngroups + 1;
      }
      /*else if(Lallbases&&iresiduecnt >= 2 && morsub==1&&level==1)*//*001119*/
      else if(Lallbases&&iresiduecnt >= 2)/*040406*/
      {/*special for sets of base type (naba)*/
          if(iresiduecnt==2){ basetype[0]='A';basetype[1]='\0';}
          else if(iresiduecnt==3)
            { basetype[0]='T';basetype[1]='U';basetype[2]='\0';}
          else if(iresiduecnt==4){ basetype[0]='G';basetype[1]='\0';}
          else if(iresiduecnt==5){ basetype[0]='C';basetype[1]='\0';}
          else if(iresiduecnt==6){ basetype[0]='?';basetype[1]='\0';}
          fprintf(fpoutput,"@group {%s bases}%s"EOLO,basetype,domrec2);
            /*040406 replace extra by domrec2*/
          Ngroups = Ngroups + 1;
      }
      else if(!Lallaminoacids&&!Lallbases)
      {/*default for single chain and/or nothing fancy*/
        fprintf(fpoutput,"@group {%s}%s%s"EOLO,topname,extra,allstr);
        strcpy(topnameold,topname);
        ichainthispass = 1;
        Ngroups = Ngroups + 1;
      }
    }/*write @group...*/
/*@group line output__________________________________________________________*/

    Ltopout = 1; /*now past place for masters layout, etc. */

    /*Next section goes through the possible list outputs */
    /*master= {list type} generated and carried in variable called chain*/
    /*when it is appropriate to have a master on an individual list*/
    /*This can be over-ridden by speciallistmaster input from cmdline 040902*/
    /*Presumably only used when only one type of list is generated, but does*/
    /*provide for a different master button.  e.g. can thus distinguish list*/
    /*of bad rotamer sidechains from the list with all sidechains. */

    if(Lonlygroups||Lonegroup||Lallbases)
    {
       if(Lspeciallistmaster){sprintf(chain," master= {%s}",speciallistmaster);}
       else                  {sprintf(chain," master= {mainchain}");}
    }
    else chain[0] = '\0';

    if(Listmc)
    {/*mainchain*/
      /*for Lonegroup there are masters on all lists*/
      if(!Lonegroup && !Lnosubgroups) /*051129*/
      {
         if( (morsub==1&&level==1) || (level>1 && Lonesubunit == 0) )
         {
           if(Lnosubgroupmasters)
           {
             fprintf(fpoutput,"@subgroup {mainchain} dominant"EOLO);
           }
           else
           {
             fprintf(fpoutput,"@subgroup {mainchain} dominant"
                             " master= {subunit %d}"EOLO,level);
           }
         }
         else
         {
            fprintf(fpoutput,"@subgroup {mainchain}"EOLO);
         }
      }
      if(!Lballs && Listmcv)
      {/*do main chain vectors first, so dots or croses will overlay them*/
        sprintf(cntl,"mcv "); /*4 chars*/
        sprintf(aname,"mc");
        if(colormcvec[0]!='\0') {sprintf(kol,"%s",colormcvec);}
        else if(level==1||level==8||level==15||level==22) /*041031 re colorset*/
            {sprintf(kol,"white");}
        else if(level==2||level==9||level==16||level==23)
            {sprintf(kol,"yellowtint");}
        else if(level==3||level==10||level==17||level==24)
            {sprintf(kol,"peachtint");}
        else if(level==4||level==11||level==18||level==25)
            {sprintf(kol,"pinktint");}
        else if(level==5||level==12||level==19||level==26)
            {sprintf(kol,"lilactint");}
        else if(level==6||level==13||level==20||level==27)
            {sprintf(kol,"bluetint");}
        else if(level==7||level==14||level==21||level==28)
            {sprintf(kol,"greentint");}
        else  {sprintf(kol,"white");}
#ifdef OLDCODE
        else if(level==1||level==6||level==11||level>15)
            {sprintf(kol,"white");}
        else if(level==2||level==7||level==12)
            {sprintf(kol,"yellowtint");}
        else if(level==3||level==8||level==13)
            {sprintf(kol,"greentint");}
        else if(level==4||level==9||level==14)
            {sprintf(kol,"pinktint");}
        else if(level==5||level==10||level==15)
            {sprintf(kol,"bluetint");}
        else  {sprintf(kol,"white");}
#endif
        Lvector = 1;
        sprintf(extra,"%s%s",chain,offmc);
        transferout(aname,cntl,kol,extra); /*.*/
      }

      if(Listmcc)  /*960809*/
      {
        sprintf(cntl,"mcc "); /*4 chars*/
        sprintf(aname,"mc C");
        sprintf(kol,"sea");
        if(Ldotsonly) Lvector = 0;
        else if(Lballs) Lvector = 3;
        else Lvector = 1;
        sprintf(extra," nobutton%s%s",chain,atoms);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listmcn)
      {
        sprintf(cntl,"mcn "); /*4 chars*/
        sprintf(aname,"mc N");
        sprintf(kol,"sky");
        if(Ldotsonly) Lvector = 0;
        else if(Lballs) Lvector = 3;
        else Lvector = 1;
        sprintf(extra," nobutton%s%s",chain,atoms);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listmco)
      {
        sprintf(cntl,"mco "); /*4 chars*/
        sprintf(aname,"mc O");
        sprintf(kol,"red");
        if(Ldotsonly) Lvector = 0;
        else if(Lballs) Lvector = 3;
        else Lvector = 1;
        sprintf(extra," nobutton%s%s",chain,atoms);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listmcs)
      {
        sprintf(cntl,"mcs "); /*4 chars*/
        sprintf(aname,"mc P");
        sprintf(kol,"yellow");
        if(Ldotsonly) Lvector = 0;
        else if(Lballs) Lvector = 3;
        else Lvector = 1;
        sprintf(extra," nobutton%s%s",chain,atoms);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Lballs && Listmcv)
      {/*do main chain vectors after, so MAGE knows balls need lines shortened*/
        sprintf(cntl,"mcv "); /*4 chars*/
        sprintf(aname,"mc");
        if(colormcvec[0]!='\0') {sprintf(kol,"%s",colormcvec);}
        else if(level==1||level==8||level==15||level==22) /*041031 re colorset*/
            {sprintf(kol,"white");}
        else if(level==2||level==9||level==16||level==23)
            {sprintf(kol,"yellowtint");}
        else if(level==3||level==10||level==17||level==24)
            {sprintf(kol,"peachtint");}
        else if(level==4||level==11||level==18||level==25)
            {sprintf(kol,"pinktint");}
        else if(level==5||level==12||level==19||level==26)
            {sprintf(kol,"lilactint");}
        else if(level==6||level==13||level==20||level==27)
            {sprintf(kol,"bluetint");}
        else if(level==7||level==14||level==21||level==28)
            {sprintf(kol,"greentint");}
        else  {sprintf(kol,"white");}
#ifdef OLDCODE
        else if(level==1||level==6||level==11||level>15)
            {sprintf(kol,"white");}
        else if(level==2||level==7||level==12)
            {sprintf(kol,"yellowtint");}
        else if(level==3||level==8||level==13)
            {sprintf(kol,"greentint");}
        else if(level==4||level==9||level==14)
            {sprintf(kol,"pinktint");}
        else if(level==5||level==10||level==15)
            {sprintf(kol,"bluetint");}
        else  {sprintf(kol,"white");}
#endif
        Lvector = 1;
        sprintf(extra,"%s%s",chain,offmc);
        transferout(aname,cntl,kol,extra); /*.*/
      }

      if(Listmckc)
      {/*CPK*/ /*960815*/
        sprintf(cntl,"mckc"); /*4 chars*/
        sprintf(aname,"mc C");
        sprintf(kol,"green");
        Lvector = 4;
        if(Listmckh) sprintf(extra," radius= 1.74 nobutton%s%s",chain,cpks);
        else         sprintf(extra," radius= 1.9 nobutton%s%s",chain,cpks);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listmckn)
      {/*CPK*/ /*960815*/
        sprintf(cntl,"mckn"); /*4 chars*/
        sprintf(aname,"mc N");
        sprintf(kol,"sky");
        Lvector = 4;
        if(Listmckh) sprintf(extra," radius= 1.54 nobutton%s%s",chain,cpks);
        else         sprintf(extra," radius= 1.7 nobutton%s%s",chain,cpks);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listmcko)
      {/*CPK*/ /*960815*/
        sprintf(cntl,"mcko"); /*4 chars*/
        sprintf(aname,"mc O");
        sprintf(kol,"red");
        Lvector = 4;
        if(Listmckh) sprintf(extra," radius= 1.4 nobutton%s%s",chain,cpks);
        else         sprintf(extra," radius= 1.5 nobutton%s%s",chain,cpks);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listmcks)
      {/*CPK*/ /*051108 for PO4 of nucleic acids*/
        sprintf(cntl,"mcks"); /*4 chars*/
        sprintf(aname,"mc P");
        sprintf(kol,"yellow");
        Lvector = 4;
        if(Listmckh) sprintf(extra," radius= 1.9 nobutton%s%s",chain,cpks);
        else         sprintf(extra," radius= 1.9 nobutton%s%s",chain,cpks);
        transferout(aname,cntl,kol,extra); /*.*/
      }

      if(Listmckh)
      {/*CPK*/ /*960815*/
        sprintf(cntl,"mckh"); /*4 chars*/
        sprintf(aname,"mc H");
        sprintf(kol,"white");
        Lvector = 4;
        sprintf(extra," radius= 1.2 nobutton%s%s%s",chain,cpks,hygen);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listhy)
      {/*mc hydrogens*/
        sprintf(cntl,"hymc"); /*4 chars*/
        sprintf(aname,"mc H");
        if(colorhyvec[0] != '\0') sprintf(kol,"%s",colorhyvec);
        else sprintf(kol,"gray");
        Lvector = 1;
        if(Lhylens) sprintf(extra,"lens nobutton%s%s%s",chain,hygen,offhy);
        else sprintf(extra," nobutton%s%s%s",chain,hygen,offhy);
        transferout(aname,cntl,kol,extra); /*.*/
      }/*mc hydrogens*/

    }/*mainchain*/

    chain[0] = '\0'; /*set default first, then check for alternatives*/
    if(Lonlygroups || Lonegroup)
    {
       if(LtypeN)
       {
          sprintf(chain," master= {Virtual BB} ");
       }
       else
       {
          sprintf(chain," master= {Calphas} ");
       }
    }
    if(Lallbases)  sprintf(chain," master= {pseudo_mc} ");
    if(Lspeciallistmaster){sprintf(chain," master= {%s}",speciallistmaster);}

    if(Listca || Listtabl)
    {/*ca-ca*/
      /*if(Lnucleicacid) {sprintf(aname,"backbone");}*/
      if(LtypeN) {sprintf(aname,"Virtual BB");}
      else {sprintf(aname,"Calphas");}
      if(colorcavec[0] != '\0') {sprintf(kol,"%s",colorcavec);}
        else if(level==1||level==8||level==15||level==22) /*041031 re colorset*/
            {sprintf(kol,"white");}
        else if(level==2||level==9||level==16||level==23)
            {sprintf(kol,"yellowtint");}
        else if(level==3||level==10||level==17||level==24)
            {sprintf(kol,"peachtint");}
        else if(level==4||level==11||level==18||level==25)
            {sprintf(kol,"pinktint");}
        else if(level==5||level==12||level==19||level==26)
            {sprintf(kol,"lilactint");}
        else if(level==6||level==13||level==20||level==27)
            {sprintf(kol,"bluetint");}
        else if(level==7||level==14||level==21||level==28)
            {sprintf(kol,"greentint");}
      else  {sprintf(kol,"white");}
      if(Listca)
      {/*one pass for Calphas*/
         /*for Lonegroup there are masters on all lists*/
         if(!Lonegroup && !Lnosubgroups) /*051129*/
         {
            if(!Listmc) {fprintf(fpoutput,"@subgroup {mainchain} "EOLO);}
         }
         sprintf(cntl,"ca  "); /*4 chars*/
         Lvector = 1;
         if(Listmcv) {sprintf(extra," off  %s",chain);}
         else {sprintf(extra,"%s%s",chain,offca);} /*Calphas off if full mc vectors*/
         transferout(aname,cntl,kol,extra); /*.*/
      }
      else
      {/*Listtabl: 2 passes for Ltable control*/
         if(MolNameStr[0] == '\0')
              {fprintf(fpoutput,"@group {ROW %d} table header\n",level);}
         else {fprintf(fpoutput,"@group {%s %d} table header\n",MolNameStr,level);}
         sprintf(cntl,"tabh"); /*4 chars*/ /*991122*/
         sprintf(extra,"%s",chain);
         Lvector = 7; /*wordlist for table*/
         transferout(aname,cntl,kol,extra); /*.*/

         if(MolNameStr[0] == '\0')
              {fprintf(fpoutput,"@group {ROW %d} table \n",level);}
         else {fprintf(fpoutput,"@group {%s %d} table \n",MolNameStr,level);}
         sprintf(cntl,"tabl"); /*4 chars*/ /*991122*/
         sprintf(extra,"%s",chain);
         Lvector = 7; /*wordlist for table*/
         transferout(aname,cntl,kol,extra); /*.*/
      }
    }/*ca-ca*/

    if(Lonlygroups||Lonegroup||Lallbases)
    {
       if(Lspeciallistmaster){sprintf(chain," master= {%s}",speciallistmaster);}
       else                  {sprintf(chain," master= {mainchain}");}
    }
    else chain[0] = '\0';

    if(Listhb)
    {/*h-bonds*/
      /*for Lonegroup there are masters on all lists*/
      if(!Lonegroup && !Lnosubgroups) /*051129*/
      {
         if(!Listmc && !Listca)
         {
            fprintf(fpoutput,"@subgroup {mainchain} "EOLO);
         }
      }
      sprintf(cntl,"hb  "); /*4 chars*/
      sprintf(aname,"Hb");
      if(colorhbvec[0] != '\0') sprintf(kol,"%s",colorhbvec);
      else sprintf(kol,"brown");
      Lvector = 1;
      sprintf(extra," nobutton%s%s%s",chain,hbond,offhb);
      transferout(aname,cntl,kol,extra); /*.*/
    }/*h-bonds*/

    if(Lonlygroups||Lonegroup||Lallbases)
    {
       if(Lspeciallistmaster){sprintf(chain," master= {%s}",speciallistmaster);}
       else                  {sprintf(chain," master= {sidechain}");}
    }
    else {chain[0] = '\0';}

    if(Listsc)
    {/*sidechains*/
      sprintf(aname,"sc"); /*default*/
      if(Lallaminoacids||Lallbases)
      {/*Lallaminoacids||Lallbases: a lot of the name stuff for headers*/

        /*so complicated that Lnosubgroups is probably inappropriate 051129*/

        aname[0] = resid[1][0];
        aname[1] = resid[1][1];
        aname[2] = resid[1][2];
        aname[3] = '\0';
        if(Lallbases)
        {
           aname[0] = toupper(aname[0]);
           aname[1] = toupper(aname[1]);
           aname[2] = toupper(aname[2]);
        }

        if( (morsub==1&&level==1) || (level>1 && Lonesubunit == 0) )
        {
          if(!Lonegroup)  /*for Lonegroup there are masters on all lists*/
          {
            if(Lnosubgroupmasters)
            {
               fprintf(fpoutput,"@subgroup {%s} dominant"EOLO,aname);
            }
            else
            {
               fprintf(fpoutput,"@subgroup {%s} dominant "
                        "master= {subunit %d}"EOLO,aname,level);
            }
          }
        }
        else
        {
          if(!Lonegroup)  /*for Lonegroup there are masters on all lists*/
          {
            if(Lnosubgroupmasters)
            {
              fprintf(fpoutput,"@subgroup {%s} dominant %s"EOLO,aname,allstr);
            }
            else
            {
              fprintf(fpoutput,"@subgroup {%s} dominant %s%s"EOLO
                      ,aname,chain,allstr);
            }
          }
        }
      }/*Lallaminoacids||Lallbases: a lot of the name stuff for headers*/
      else
      {/*usual groupings*/
         /*for Lonegroup there are masters on all lists*/
         if(!Lonegroup && !Lnosubgroups )  /*051129*/
         {
           if(Lnosubgroupmasters)
           {
             fprintf(fpoutput,"@subgroup {sidechain}"EOLO);
           }
           else
           {
             fprintf(fpoutput,"@subgroup {sidechain} master= {subunit %d}"EOLO
                               ,level);
           }
         }
      }/*usual groupings*/
      if(!Lballs && Listscv)
      {/*do sidechain vectors first, so dots or croses will overlay them*/
         sprintf(cntl,"scv "); /*4 chars*/
        sprintf(aname,"sc");
        if(colorscvec[0] != '\0') sprintf(kol,"%s",colorscvec);
        else sprintf(kol,"cyan");
          /*default sidechain color*/
        if(Lallaminoacids||Lallbases)
        {
            aname[0] = resid[1][0];
            aname[1] = resid[1][1];
            aname[2] = resid[1][2];
            aname[3] = '\0';
            if(Lallbases)
            {
               aname[0] = toupper(aname[0]);
               aname[1] = toupper(aname[1]);
               aname[2] = toupper(aname[2]);
            }
            for(j=0; j<20; j++)
            {
                 kol[j] = kolor[j];
                 if(kolor[j] == '\0') break;
            }
            sprintf(allstr," master= {all}");
        }
         Lvector = 1;
         sprintf(extra,"%s%s",chain,offsc);
         transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listscc)
      {
        sprintf(cntl,"scc "); /*4 chars*/
        sprintf(aname,"sc C");
        sprintf(kol,"sea");
        if(Ldotsonly) Lvector = 0;
        else if(Lballs) Lvector = 3;
        else Lvector = 1;
        sprintf(extra," nobutton%s%s%s",chain,atoms,offsc);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listscn)
      {
        sprintf(cntl,"scn "); /*4 chars*/
        sprintf(aname,"sc N");
        sprintf(kol,"sky");
        if(Ldotsonly) Lvector = 0;
        else if(Lballs) Lvector = 3;
        else Lvector = 1;
        sprintf(extra," nobutton%s%s%s",chain,atoms,offsc);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listsco)
      {
        sprintf(cntl,"sco "); /*4 chars*/
        sprintf(aname,"sc O");
        sprintf(kol,"red");
        if(Ldotsonly) Lvector = 0;
        else if(Lballs) Lvector = 3;
        else Lvector = 1;
        sprintf(extra," nobutton%s%s%s",chain,atoms,offsc);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listscs)
      {
        sprintf(cntl,"scs "); /*4 chars*/
        sprintf(aname,"sc S");
        sprintf(kol,"yellow");
        if(Ldotsonly) Lvector = 0;
        else if(Lballs) Lvector = 3;
        else Lvector = 1;
        sprintf(extra," nobutton%s%s%s",chain,atoms,offsc);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Lballs && Listscv) /*969815*/
      {/*do sidechain vectors after, so MAGE knows balls need lines shortened*/
        /*sidechain vectors after balls*/
        /*have to repeat a lot of the name, color determining stuff*/

        if(colorscvec[0] != '\0') sprintf(kol,"%s",colorscvec);
        else sprintf(kol,"cyan"); /*default sidechain color*/

        sprintf(cntl,"scv "); /*4 chars*/
        sprintf(aname,"sc");
        if(Lallaminoacids||Lallbases)
        {
            aname[0] = resid[1][0];
            aname[1] = resid[1][1];
            aname[2] = resid[1][2];
            aname[3] = '\0';
            if(Lallbases)
            {
               aname[0] = toupper(aname[0]);
               aname[1] = toupper(aname[1]);
               aname[2] = toupper(aname[2]);
            }
            for(j=0; j<20; j++)
            {
                 kol[j] = kolor[j];
                 if(kolor[j] == '\0') break;
            }
        }
         Lvector = 1;
         sprintf(extra,"%s%s",chain,offsc);
         transferout(aname,cntl,kol,extra); /*.*/
      }/*sidechain vectors after balls*/

      if(Listsckc)/*969815*/ /*color and radius need specification*/
      {/*CPK*/
        Lvector = 4;
        sprintf(kol,"green");
        sprintf(cntl,"sckc"); /*4 chars*/
        sprintf(aname,"sc C");
        if(Listsckh) sprintf(extra," radius= 1.74 nobutton%s%s",chain,cpks);
        else         sprintf(extra," radius= 1.9 nobutton%s%s",chain,cpks);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listsckn)/*969815*/ /*color and radius need specification*/
      {/*CPK*/
        Lvector = 4;
        sprintf(kol,"sky");
        sprintf(cntl,"sckn"); /*4 chars*/
        sprintf(aname,"sc N");
        if(Listsckh) sprintf(extra," radius= 1.54 nobutton%s%s",chain,cpks);
        else         sprintf(extra," radius= 1.7 nobutton%s%s",chain,cpks);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listscko)/*969815*/ /*color and radius need specification*/
      {/*CPK*/
        Lvector = 4;
        sprintf(kol,"red");
        sprintf(cntl,"scko"); /*4 chars*/
        sprintf(aname,"sc O");
        if(Listsckh) sprintf(extra," radius= 1.4 nobutton%s%s",chain,cpks);
        else         sprintf(extra," radius= 1.5 nobutton%s%s",chain,cpks);
        transferout(aname,cntl,kol,extra); /*.*/
      }

      if(Listscks)/*969815*/ /*color and radius need specification*/
      {/*CPK*/
        Lvector = 4;
        sprintf(kol,"yellow");
        sprintf(cntl,"scks"); /*4 chars*/
        sprintf(aname,"sc S");
        if(Listsckh) sprintf(extra," radius= 1.85 nobutton%s%s",chain,cpks);
        else         sprintf(extra," radius= 2.0 nobutton%s%s",chain,cpks);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listsckh)
      {/*CPK*/ /*960815*/
        sprintf(cntl,"sckh"); /*4 chars*/
        sprintf(aname,"sc H");
        sprintf(kol,"white");
        Lvector = 4;
        sprintf(extra," radius= 1.2 nobutton%s%s%s",chain,cpks,hygen);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listhy && Listscv)
      {/*sc hydrogens*/
        sprintf(cntl,"hysc"); /*4 chars*/
        sprintf(aname,"sc H");
        if(colorhyvec[0] != '\0') sprintf(kol," %s",colorhyvec);
        else sprintf(kol,"gray");
        Lvector = 1;
        if(Lhylens) sprintf(extra,"lens nobutton%s%s%s",chain,hygen,offhy);
        else sprintf(extra," nobutton%s%s%s",chain,hygen,offhy);
        transferout(aname,cntl,kol,extra); /*.*/
      }/*sc hydrogens*/

      if(Listscring)
      {/*sc rings tesselation*/
          sprintf(cntl,"rbsc");
          sprintf(aname,"ring");
          sprintf(kol,"gold");
          Lvector = 8;
          sprintf(extra,"%s",chain);
          transferout(aname,cntl,kol,extra); /*.*/
      }/*sc rings tesselation*/

    }/*sidechains*/

    if(Listss)
    {/*disulfide vectors*/
        sprintf(extra,"%s",chain); /*chain==master={sidechain} move here051201*/
        /*for Lonegroup there are masters on all lists*/
        if(!Lonegroup && !Lnosubgroups )  /*051129*/
        {
           if(!Listsc) /*so likely no other sidechain except -SS- 051201 */
           {
              sprintf(extra," master= {-SS-}");

              if(Lnosubgroupmasters)
              {
                fprintf(fpoutput,"@subgroup {-SS-}"EOLO);
              }
              else
              {
                /*fprintf(fpoutput,"@subgroup {-SS-} %s"EOLO,chain);*/
                fprintf(fpoutput,"@subgroup {-SS-} master= {-SS-}"EOLO);
              }
           }
        }
        sprintf(cntl,"ss  "); /*4 chars*/
        sprintf(aname,"SS");
        kol[0]='y';kol[1]='e';kol[2]='l';kol[3]='l';kol[4]='o';kol[5]='w';
                 kol[6]='\0';
        Lvector = 1;
        transferout(aname,cntl,kol,extra); /*.*/
    }/*disulfide vectors*/

    if(Lonlygroups||Lonegroup||Lallbases)
    {
       if(Lspeciallistmaster){sprintf(chain," master= {%s}",speciallistmaster);}
       else                  {sprintf(chain," master= {labels}");}
    }
    else chain[0] = '\0';

    if(Listlb)
    {/*labels*/
       if(!Listsc && !Listss)
       {
         if(Lallaminoacids||Lallbases)
         {/*Lallaminoacids||Lallbases*/ /*Lnosubgroups not appropriate*/
           if( (morsub==1&&level==1) || (level>1 && Lonesubunit == 0) )
           {
             if(!Lonegroup)  /*for Lonegroup there are masters on all lists*/
             {
               if(Lnosubgroupmasters)
               {
                 fprintf(fpoutput,"@subgroup {%s} dominant"EOLO,resid[1]);
               }
               else
               {
                 fprintf(fpoutput,"@subgroup {%s} dominant "
                                  "master= {subunit %d}"EOLO,resid[1],level);
               }
             }
           }
           else
           {
             if(!Lonegroup)  /*for Lonegroup there are masters on all lists*/
             {
               if(Lnosubgroupmasters)
               {
                fprintf(fpoutput,"@subgroup {%s} dominant"EOLO,resid[1]);
               }
               else
               {
                fprintf(fpoutput,"@subgroup {%s} dominant %s"EOLO
                                  ,resid[1],chain);
               }
             }
           }
         }/*Lallaminoacids||Lallbases*/
         else
         {
            /*for Lonegroup there are masters on all lists*/
            if(!Lonegroup && !Lnosubgroups ) /*051129*/
            {
              if(Lnosubgroupmasters)
              {
                fprintf(fpoutput,"@subgroup {sidechain}"EOLO);
              }
              else
              {
                fprintf(fpoutput,"@subgroup {sidechain}%s"EOLO,chain);
              }
            }
         }
       }
       sprintf(cntl,"lb  "); /*4 chars*/
       sprintf(aname,"labels");
       if(colorlbvec[0] != '\0') sprintf(kol," %s",colorlbvec);
       else sprintf(kol,"cyan");
       Lvector = 2;
       sprintf(extra,"%s%s",chain,offlb);
       transferout(aname,cntl,kol,extra); /*.*/
    }/*labels*/

    if(Lonlygroups||Lonegroup)
    {
       if(Lspeciallistmaster){sprintf(chain," master= {%s}",speciallistmaster);}
       else                  {sprintf(chain," master= {hets}");}
    }
    else chain[0] = '\0';

    if(Listht)
    {/*hetatm*/
      /*for Lonegroup there are masters on all lists*/
      if(!Lonegroup && !Lnosubgroups)
      {
         if(Lnosubgroupmasters)
         {
            fprintf(fpoutput,"@subgroup {het groups}"EOLO);
         }
         else
         {
            fprintf(fpoutput,"@subgroup {het groups}%s"EOLO,chain);
         }
      }

      if(!Lballs && Listhtv)
      {
        sprintf(cntl,"htv "); /*4 chars*/
        sprintf(aname,"het");

        if(colorhtvec[0] != '\0') sprintf(kol,"%s",colorhtvec);
        else sprintf(kol,"pink");

        Lvector = 1;
        sprintf(extra,"%s%s",chain,offht);
        transferout(aname,cntl,kol,extra); /*.*/
      }

      if(Listhtc)
      {
        sprintf(cntl,"htc "); /*4 chars*/
        sprintf(aname,"het C");
        sprintf(kol,"sea");
        if(Ldotsonly) Lvector = 0;
        else if(Lballs) Lvector = 3;
        else Lvector = 1;
        sprintf(extra," nobutton%s%s",chain,atoms);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listhtn)
      {
        sprintf(cntl,"htn "); /*4 chars*/
        sprintf(aname,"het N");
        sprintf(kol,"sky");
        if(Ldotsonly) Lvector = 0;
        else if(Lballs) Lvector = 3;
        else Lvector = 1;
        sprintf(extra," nobutton%s%s",chain,atoms);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listhto)
      {
        sprintf(cntl,"hto "); /*4 chars*/
        sprintf(aname,"het O");
        sprintf(kol,"red");
        if(Ldotsonly) Lvector = 0;
        else if(Lballs) Lvector = 3;
        else Lvector = 1;
        sprintf(extra," nobutton%s%s",chain,atoms);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listhts)
      {
        sprintf(cntl,"hts "); /*4 chars*/
        sprintf(aname,"het S");
        sprintf(kol,"yellow");
        if(Ldotsonly) Lvector = 0;
        else if(Lballs) Lvector = 3;
        else Lvector = 1;
        sprintf(extra," nobutton%s%s",chain,atoms);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listhtm)
      {
        sprintf(cntl,"htm "); /*4 chars*/
        sprintf(aname,"het M");
        sprintf(kol,"gray");
        Lvector = 4; /*don't put master= {atoms} on these*/  /*991210*/
        sprintf(extra," radius= 0.5 nobutton%s",chain);
        transferout(aname,cntl,kol,extra); /*.*/
      }

      if(Lballs && Listhtv) /*960815*/
      {
        sprintf(cntl,"htv "); /*4 chars*/
        sprintf(aname,"het");

        if(colorhtvec[0] != '\0') sprintf(kol,"%s",colorhtvec);
        else sprintf(kol,"pink");

        Lvector = 1;
        sprintf(extra,"%s%s",chain,offht);
        transferout(aname,cntl,kol,extra); /*.*/
      }

      if(Listhtkc)/*969815*/ /*color and radius need specification*/
      {/*CPK*/
        Lvector = 4;
        sprintf(kol,"green");
        sprintf(cntl,"htkc"); /*4 chars*/
        sprintf(aname,"het C");
        if(Listhtkh) sprintf(extra," radius= 1.74 nobutton%s%s",chain,cpks);
        else         sprintf(extra," radius= 1.9 nobutton%s%s",chain,cpks);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listhtkn)/*969815*/ /*color and radius need specification*/
      {/*CPK*/
        Lvector = 4;
        sprintf(kol,"sky");
        sprintf(cntl,"htkn"); /*4 chars*/
        sprintf(aname,"het N");
        if(Listhtkh) sprintf(extra," radius= 1.54 nobutton%s%s",chain,cpks);
        else         sprintf(extra," radius= 1.7 nobutton%s%s",chain,cpks);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listhtko)/*969815*/ /*color and radius need specification*/
      {/*CPK*/
        Lvector = 4;
        sprintf(kol,"red");
        sprintf(cntl,"htko"); /*4 chars*/
        sprintf(aname,"het O");
        if(Listhtkh) sprintf(extra," radius= 1.4 nobutton%s%s",chain,cpks);
        else         sprintf(extra," radius= 1.5 nobutton%s%s",chain,cpks);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listhtks)/*969815*/ /*color and radius need specification*/
      {/*CPK*/
        Lvector = 4;
        sprintf(kol,"yellow");
        sprintf(cntl,"htks"); /*4 chars*/
        sprintf(aname,"het X");
        if(Listhtkh) sprintf(extra," radius= 2.0 nobutton%s%s",chain,cpks);
        else         sprintf(extra," radius= 1.85 nobutton%s%s",chain,cpks);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listhtkh)
      {/*CPK*/ /*960815*/
        sprintf(cntl,"htkh"); /*4 chars*/
        sprintf(aname,"het H");
        sprintf(kol,"white");
        Lvector = 4;
        sprintf(extra," radius= 1.2 nobutton%s%s%s",chain,cpks,hygen);
        transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listhy)
      {/*ht hydrogens*/
        sprintf(cntl,"hyht"); /*4 chars*/
        sprintf(aname,"ht H");
        if(colorhyvec[0] != '\0') sprintf(kol,"%s",colorhyvec);
        else sprintf(kol,"gray");
        Lvector = 1;
        if(Lhylens) sprintf(extra,"lens nobutton%s%s%s",chain,hygen,offhy);
        else sprintf(extra," nobutton%s%s%s",chain,hygen,offhy);
        transferout(aname,cntl,kol,extra); /*.*/
      }/*ht hydrogens*/
    }/*hetatm*/
    if(Listwa)
    {/*water*/

      if(Lonlygroups||Lonegroup)
      {
       if(Lspeciallistmaster){sprintf(chain," master= {%s}",speciallistmaster);}
       else                  {sprintf(chain," master= {water}");}
      }
      else chain[0] = '\0';

      /*for Lonegroup there are masters on all lists*/
      if(!Lonegroup && !Lnosubgroups ) /*051129*/
      {
         if(!Listht)
         {
            if(Lnosubgroupmasters)
            {
               fprintf(fpoutput,"@subgroup {waters} dominant"EOLO);
            }
            else
            {
               fprintf(fpoutput,"@subgroup {waters} dominant%s"EOLO,chain);
            }
         }
      }
      if(Listwao) /*water + atom markers*/
      {/*Marker on Oxygen, could be a ball if Lballs! */
        sprintf(cntl,"wao "); /*4 chars*/
        sprintf(aname,"water O");
        if(colorwavec[0] != '\0') sprintf(kol,"%s",colorwavec);
        else sprintf(kol,"peachtint");
        /*sprintf(extra," master= {water}");*/ /*set default*/
        sprintf(extra,"%s%s",chain,offwa);/*011024*/
        if(Ldotsonly) Lvector = 0;
        else if(Lballs)
        {
           Lvector = 6; /*balllist with extra radius specification*/
           /*sprintf(extra," radius= 0.15 master= {water}");*/
           sprintf(extra," radius= 0.15 %s%s",chain,offwa);/*011024*/
        }
        else Lvector = 1; /*vectors making cross-mark*/
        transferout(aname,cntl,kol,extra); /*.*/
      }
      else /*water without atom markers*/
      {/*No Balls on oxygen, so put a dot on the oxygen*/
        sprintf(cntl,"wa  "); /*4 chars*/
        sprintf(aname,"water");
        if(colorwavec[0] != '\0') sprintf(kol,"%s",colorwavec);
        else sprintf(kol,"peachtint");
        Lvector = 0; /*dotlist*/
        sprintf(extra,"%s%s",chain,offwa);
        transferout(aname,cntl,kol,extra); /*.*/
      }

      if(Listhy)
      {/*wa hydrogens*/
        sprintf(cntl,"hywa"); /*4 chars*/
        sprintf(aname,"wa H");
        if(colorhyvec[0] != '\0') sprintf(kol,"%s",colorhyvec);
        else sprintf(kol,"gray");
        Lvector = 1;
        /*sprintf(extra," master= {water}%s%s",hygen,offhy);*/
        sprintf(extra,"%s%s%s",chain,hygen,offhy);/*011024*/
        transferout(aname,cntl,kol,extra); /*.*/
      }/*wa hydrogens*/
    }/*water*/

    if(Listrot > 0)
    {/*something for which MAGE will set up rotations*/
      /*set up for at least one rotatable bond*/
      /*if(!Lonegroup)*/ /*always make separate subgroup for rotations*/
      /*for Lonegroup there are masters on all lists*/
      /*fprintf(fpoutput,"@subgroup {rotate %d} dominant master= {rotation}"*/

      if(Lemphasis) { fprintf(fpoutput,"@beginemphasis\n"); }
      if(!Lnosubgroups) /*051129*/
      {
         if(Lnosubgroupmasters)
         {
            fprintf(fpoutput,"@subgroup {%s} dominant"EOLO,rotatorname);
         }
         else
         {
            fprintf(fpoutput,"@subgroup {%s} dominant master= {rotation}"EOLO
                                ,rotatorname);
         }
      }
      sprintf(cntl,"ro1 "); /*4 chars*/
      sprintf(aname,"%s 1",rotatorname);
      sprintf(kol,"sea");
      sprintf(extra,"1bondrot %.1f",bondangle[1] );
      Lvector = 1;
      transferout(aname,cntl,kol,extra); /*.*/
      if(Listhy)
      {
         sprintf(cntl,"ro1h");
         sprintf(aname,"ro1H");
         sprintf(aname,"%s 1H",rotatorname);
         sprintf(kol,"gray");
         /*sprintf(extra," master= {rot H}%s",hygen);*/ /*011024*/
         sprintf(extra,"%s",hygen);/*hygen== master= {H's}*/
         Lvector = 1;
         transferout(aname,cntl,kol,extra); /*.*/
      }
      if(Listrot >= 2 ) /*just make changes to the above setup*/
      {
        sprintf(cntl,"ro2 "); /*4 chars*/
        sprintf(aname,"%s 2",rotatorname);
        sprintf(kol,"sea");
        if(Listrot2==2)
                sprintf(extra,"%cbondrot %.1f",branch,bondangle[2] );
        else            sprintf(extra,"2bondrot %.1f",bondangle[2] );
        Lvector = 1;
        transferout(aname,cntl,kol,extra); /*.*/
        if(Listhy)
        {
           sprintf(cntl,"ro2h");
           sprintf(aname,"%s 2H",rotatorname);
           sprintf(kol,"gray");
           /*sprintf(extra," master= {rot H}%s",hygen);*/ /*011024*/
           sprintf(extra,"%s",hygen);/*hygen== master= {H's}*/
           Lvector = 1;
           transferout(aname,cntl,kol,extra); /*.*/
        }

      }
      if(Listrot >= 3) /*just make changes to the above setup*/
      {
        sprintf(cntl,"ro3 ");
        sprintf(aname,"%s 3",rotatorname);
        sprintf(kol,"sea");
        if(Listrot2==3)
                sprintf(extra,"%cbondrot %.1f",branch,bondangle[3] );
        else            sprintf(extra,"3bondrot %.1f",bondangle[3] );
        Lvector = 1;
        transferout(aname,cntl,kol,extra); /*.*/
        if(Listhy)
        {
           sprintf(cntl,"ro3h");
           sprintf(aname,"%s 3H",rotatorname);
           sprintf(kol,"gray");
           /*sprintf(extra," master= {rot H}%s",hygen);*/ /*011024*/
           sprintf(extra,"%s",hygen);/*hygen== master= {H's}*/
           Lvector = 1;
           transferout(aname,cntl,kol,extra); /*.*/
        }

      }
      if(Listrot >= 4) /*just make changes to the above setup*/
      {
        sprintf(cntl,"ro4 ");
        sprintf(aname,"%s 4",rotatorname);
        sprintf(kol,"sea");
        if(Listrot2==4)
                sprintf(extra,"%cbondrot %.1f",branch,bondangle[4] );
        else            sprintf(extra,"4bondrot %.1f",bondangle[4] );
        Lvector = 1;
        transferout(aname,cntl,kol,extra); /*.*/
        if(Listhy)
        {
           sprintf(cntl,"ro4h");
           sprintf(aname,"%s 4H",rotatorname);
           sprintf(kol,"gray");
           /*sprintf(extra," master= {rot H}%s",hygen);*/ /*011024*/
           sprintf(extra,"%s",hygen);/*hygen== master= {H's}*/
           Lvector = 1;
           transferout(aname,cntl,kol,extra); /*.*/
        }

      }
      if(Listrot >= 5 ) /*just make changes to the above setup*/
      {
        sprintf(cntl,"ro5 ");
        sprintf(aname,"%s 5",rotatorname);
        sprintf(kol,"sea");
        sprintf(extra,"5bondrot %.1f",bondangle[5] );
        Lvector = 1;
        transferout(aname,cntl,kol,extra); /*.*/
        if(Listhy)
        {
           sprintf(cntl,"ro5h");
           sprintf(aname,"%s 5H",rotatorname);
           sprintf(kol,"gray");
           /*sprintf(extra," master= {rot H}%s",hygen);*/ /*011024*/
           sprintf(extra,"%s",hygen);/*hygen== master= {H's}*/
           Lvector = 1;
           transferout(aname,cntl,kol,extra); /*.*/
        }

      }

      fprintf(fpoutput,"@vectorlist {rotation} nobutton 0bondrot 0.0 "EOLO);
      if(Lemphasis) { fprintf(fpoutput,"@endemphasis\n"); }
    }/*something for which MAGE will set up rotations*/

    if(Lonlygroups||Lonegroup)
    {
       if(Lspeciallistmaster){sprintf(chain," master= {%s}",speciallistmaster);}
       else                  {sprintf(chain," master= {ribbon}");}
    }
    else chain[0] = '\0';

    if(Listrib)
    {/*ribbon*/
      /*for Lonegroup there are masters on all lists*/
      if(!Lonegroup && !Lnosubgroups ) /*051129*/
      {
         if(Lnosubgroupmasters)
         {
            fprintf(fpoutput,"@subgroup {ribbon}\n");
         }
         else
         {
            fprintf(fpoutput,"@subgroup {ribbon} %s\n",chain);
         }
      }

      if(Lexplicitribbon > 0 || Lbestribbon) /*041107 Lbestribbon*/
      {/*at least tried to get ribbon arrows from PDB SHEET & HELIX records*/
         /*set up color designators for putative alpha, beta, coil regions*/
         /*on list use:  color= {name}  instead of  color= color  */
         /*@colorset {name} color    used to use topname==molec+topID*/
         /*unique name and standard color as char strings*/
         /*031228: just use topID to keep colorset name <= 14 char for mage*/
         /*041106: colors to distinguish multiple chains, repeat by symmetry*/
         /*seven list color schemes, pointcolors could be colornc or colorBval*/
         /*protein nucleic acid complexes are not sorted out well as of 041106*/

         /*050705 use this for RCSB ribnhet option, but it messes up multicrit*/
         /*if(Nproteinchains > 1 || Nnucleicchains > 1)*/ /*041106listcolordom*/
         if(Llistcolordom &&(Nproteinchains > 1 || Nnucleicchains > 1))
         {/*distinguish chains by list colorscheme*/
            fprintf(fpoutput,"@listcolordominant\n");
         }
         NMOD = 7; /*max and default*/
         if(Nproteinchains > 1)  /*041106 distinguish by putative symmetry*/
         {/*distinguish by putative symmetry*/
            if     ( (Nproteinchains % 5) == 0 ) {NMOD = 5;}
            else if( (Nproteinchains % 6) == 0 ) {NMOD = 6;}
            else                                 {NMOD = 7;} /*max and default*/
         }
         Ncase = (level % NMOD);
         if(Ncase == 0) {Ncase = NMOD;} /*cases start with 1, go up to NMOD*/

         /*041106 commandline colormcvec wierd for ribbons, drop its use here*/
         /*if(colormcvec[0]!='\0') {sprintf(alphcol,"%s",colormcvec);}*/

         switch(Ncase)
         {/*switch on Ncase MOD level*/
            case 1:
               sprintf(alphcol,"red"); /*041031*/
               sprintf(betacol,"lime"); /*041031*/
               sprintf(coilcol,"white"); /*041031*/
               sprintf(nuclcol,"white"); /*041031*/
            break;
            case 2:
               sprintf(alphcol,"gold"); /*041029*/
               sprintf(betacol,"yellow"); /*041029*/
               sprintf(coilcol,"yellowtint"); /*041029*/
               sprintf(nuclcol,"yellowtint"); /*041029*/
            break;
            case 3:
               sprintf(alphcol,"orange");
               sprintf(betacol,"peach");
               sprintf(coilcol,"peachtint");
               sprintf(nuclcol,"peachtint");
            break;
            case 4:
               sprintf(alphcol,"hotpink");
               sprintf(betacol,"pink");
               sprintf(coilcol,"pinktint");
               sprintf(nuclcol,"pinktint");
            break;
            case 5:
               sprintf(alphcol,"purple");
               sprintf(betacol,"lilac");
               sprintf(coilcol,"lilactint");
               sprintf(nuclcol,"lilactint");
            break;
            case 6:
               sprintf(alphcol,"blue");
               sprintf(betacol,"sky");
               sprintf(coilcol,"bluetint");
               sprintf(nuclcol,"bluetint");
            break;
            case 7:
               sprintf(alphcol,"green");
               sprintf(betacol,"sea");
               sprintf(coilcol,"greentint");
               sprintf(nuclcol,"greentint");
            break;
            default:
               sprintf(alphcol,"red");
               sprintf(betacol,"lime");
               sprintf(coilcol,"white");
               sprintf(nuclcol,"white");
            break;
         }/*switch on Ncase MOD level*/

         fprintf(fpoutput,"@colorset {alph_%s} %s \n",topID,alphcol);
         fprintf(fpoutput,"@colorset {beta_%s} %s \n",topID,betacol);
         fprintf(fpoutput,"@colorset {coil_%s} %s \n",topID,coilcol);
         fprintf(fpoutput,"@colorset {nucl_%s} %s \n",topID,nuclcol);

         if(Lribidentity) /*define rib sec str colorsets 060929*/
         {
           sprintf(colsetstr[0],"yellowtint");
           sprintf(colsetstr[1],"peachtint");
           sprintf(colsetstr[2],"pinktint");
           sprintf(colsetstr[3],"lilactint");
           sprintf(colsetstr[4],"bluetint");
           sprintf(colsetstr[5],"greentint");
           numcolsets = 6;
           indexribID = -1;
           for(k=1; k<=Nribs; k++)
           {
             if(ribidentity[k][0] != '\0')
             {
               if(strcmp(ribidentityold,ribidentity[k]) != 0)
               {
                 indexribID++;
                 if(indexribID >= numcolsets) {indexribID = 0;}
                 fprintf(fpoutput,"@colorset {%s} %s\n"
                    ,ribidentity[k],colsetstr[indexribID]);
                 strcpy(ribidentityold,ribidentity[k]);
               }
             }
           }
         }

#ifdef OLDCODE
        else if(level==1||level==8||level==15||level>22)
        else if(level==2||level==9||level==16||level>23)
        else if(level==3||level==10||level==17||level>24)
        else if(level==4||level==11||level==18||level>25)
        else if(level==5||level==12||level==19||level>26)
        else if(level==6||level==13||level==20||level>27)
        else if(level==7||level==14||level==21||level>28)
         fprintf(fpoutput,"@colorset {alph_%s} gold \n",topID);
         fprintf(fpoutput,"@colorset {beta_%s} sea \n",topID);
         fprintf(fpoutput,"@colorset {coil_%s} white \n",topID);
         fprintf(fpoutput,"@colorset {nucl_%s} peach \n",topID);
#endif
         if(Lblackedge)
            {fprintf(fpoutput,"@colorset {edge_%s} deadblack \n",topID);}
      }
      else
      {/*Lexplicitribbon == 0 && !Lbestribbon*/
         /*fprintf(fpoutput,"@colorset {coil} gold \n");*/
         fprintf(fpoutput,"@colorset {coil_%s} gold \n",topID); /*040924*/
         if(Lblackedge)  /*040924*/
            {fprintf(fpoutput,"@colorset {edge_%s} deadblack \n",topID);}
      }
      if(Lribbonguidepts)
      {
         fprintf(fpoutput,"@colorset {guid_%s} sky \n",topID);/*050327*/
      }
      if(1 == 2)  /*put a real choice here some day*/
      {
          for(n=1;n<=nstrnd;n++)
          {
              sprintf(cntl,"rb%1d ",n);
              sprintf(aname,"rib%1d",n);
              sprintf(kol,"gold");
              Lvector = 1;
              sprintf(extra,"%s",chain);
              transferout(aname,cntl,kol,extra); /*.*/ /*Listrib*/
          }
      }
      else
      {
          sprintf(cntl,"rb  ");
          sprintf(aname,"ribbon");
          sprintf(kol,"gold");
          if(Lcrosstie && !Lnoface) Lvector = 5; /*Lnoface 040319*/
          else Lvector = 1;
          sprintf(extra,"%s",chain);
          transferout(aname,cntl,kol,extra); /*.*/ /*Listrib*/
      }

    }/*ribbon*/

/*deviations subgroup section*************************************************/

    if(Lonlygroups||Lonegroup)
    {
       if(Lspeciallistmaster){sprintf(chain," master= {%s}",speciallistmaster);}
       else                  {sprintf(chain," master= {deviations}");}
    }
    else chain[0] = '\0';

    if(Listtaudev)
    {/*tau angle deviation*/
      /*for Lonegroup there are masters on all lists*/
      if(!Lonegroup && !Lnosubgroups ) /*051129*/
      {
         if(Lnosubgroupmasters)
         {
            fprintf(fpoutput,"@subgroup {tau dev}\n");
         }
         else
         {
            fprintf(fpoutput,"@subgroup {tau dev} %s\n",chain);
         }
      }
        sprintf(cntl,"tau "); /*4 chars*/
        sprintf(aname,"tau dev");
        sprintf(kol,"hotpink"); /*tau deviation points are colored balls*/
        Lvector = 1; /*vector*/ /*vector part was for testing*/
        sprintf(extra," %s",chain);
        /*ballradius is a global*/
        ballradiushold = ballradius;
        ballradius = (float)0.002; /*minimal value to avoid line shortening*/
        transferout(aname,cntl,kol,extra); /*.*/
        ballradius = ballradiushold; /*restore*/

        sprintf(cntl,"taub"); /*4 chars*/
        sprintf(aname,"tau ball");
        sprintf(kol,"hotpink"); /*tau deviation points are colored balls*/
        Lvector = 3; /*ball*/
        sprintf(extra," %s",chain);
        /*ballradius is a global*/
        ballradiushold = ballradius;
        ballradius = (float)0.002; /*minimal value to avoid line shortening*/
        transferout(aname,cntl,kol,extra); /*.*/
        ballradius = ballradiushold; /*restore*/

        sprintf(cntl,"tauv"); /*4 chars*/
        sprintf(aname,"tau v");
        sprintf(kol,"hotpink"); /*tau deviation points are colored balls*/
        Lvector = 1; /*vector*/ /*vector part was for testing*/
        sprintf(extra," %s",chain);
        transferout(aname,cntl,kol,extra); /*.*/

        sprintf(cntl,"taus"); /*4 chars*/
        sprintf(aname,"tau ball2");
        sprintf(kol,"hotpink"); /*tau deviation points are colored balls*/
        Lvector = 3; /*ball*/
        sprintf(extra," %s",chain);
        /*ballradius is a global*/
        ballradiushold = ballradius;
        ballradius = (float)0.002; /*minimal value to avoid line shortening*/
        transferout(aname,cntl,kol,extra); /*.*/
        ballradius = ballradiushold; /*restore*/

        sprintf(cntl,"tauw"); /*4 chars*/
        sprintf(aname,"tau v2");
        sprintf(kol,"hotpink"); /*tau deviation points are colored balls*/
        Lvector = 1; /*vector*/ /*vector part was for testing*/
        sprintf(extra," %s",chain);
        transferout(aname,cntl,kol,extra); /*.*/
    }/*tau angle deviation*/

    if(Listcbdev)
    {/*C-beta deviation from ideal output*/
      /*for Lonegroup there are masters on all lists*/
      if(!Lonegroup && !Lnosubgroups ) /*051129*/
      {
         if(Lnosubgroupmasters)
         {
            fprintf(fpoutput,"@subgroup {Cbeta dev}\n");
         }
         else
         {
            fprintf(fpoutput,"@subgroup {Cbeta dev} %s\n",chain);
         }
      }
        sprintf(cntl,"cbda"); /*4 chars*/
        sprintf(aname,"CB dev Ball");
        sprintf(kol,"gold");
        Lvector = 3; /*balls*/
        sprintf(extra," %s",chain);
        ballradius = (float)0.002; /*minimal value to avoid line shortening*/
        transferout(aname,cntl,kol,extra); /*.*/
        ballradius = ballradiushold; /*restore*/

        sprintf(cntl,"cbdb"); /*4 chars*/
        sprintf(aname,"CB bdev Ball"); /*shorten for aname[13] 130508*/
        sprintf(kol,"pink");
        Lvector = 3; /*balls*/
        sprintf(extra," %s",chain);
        ballradius = (float)0.002; /*minimal value to avoid line shortening*/
        transferout(aname,cntl,kol,extra); /*.*/
        ballradius = ballradiushold; /*restore*/

        sprintf(cntl,"cbdc"); /*4 chars*/
        sprintf(aname,"CB cdev Ball"); /*shorten for aname[13] 130508*/
        sprintf(kol,"red");
        Lvector = 3; /*balls*/
        sprintf(extra," %s",chain);
        ballradius = (float)0.002; /*minimal value to avoid line shortening*/
        transferout(aname,cntl,kol,extra); /*.*/
        ballradius = ballradiushold; /*restore*/

        sprintf(cntl,"cbdm"); /*4 chars*/
        sprintf(aname,"CB mis-ball");
        sprintf(kol,"sky");
        Lvector = 3; /*balls*/
        sprintf(extra," off%s",chain);
        ballradius = (float)0.002; /*minimal value to avoid line shortening*/
        transferout(aname,cntl,kol,extra); /*.*/
        ballradius = ballradiushold; /*restore*/

        sprintf(cntl,"cbdv"); /*4 chars*/
        sprintf(aname,"CB mis-bar");
        sprintf(kol,"hotpink");
        Lvector = 1; /*vector*/
        sprintf(extra," off%s",chain);
        transferout(aname,cntl,kol,extra); /*.*/

        sprintf(cntl,"cbdl"); /*4 chars*/
        sprintf(aname,"CB label");
        sprintf(kol,"gold");
        Lvector = 2;
        sprintf(extra," off%s",chain);
        transferout(aname,cntl,kol,extra); /*.*/

        sprintf(cntl,"cbde"); /*4 chars*/
        sprintf(aname,"CA---CB");
        sprintf(kol,"peach");
        Lvector = 1; /*vector*/
        sprintf(extra," %s",chain);
        transferout(aname,cntl,kol,extra); /*.*/

        sprintf(cntl,"cbdf"); /*4 chars*/
        sprintf(aname,"CA---CB -b-");
        sprintf(kol,"peach");
        Lvector = 1; /*vector*/
        sprintf(extra," %s",chain);
        transferout(aname,cntl,kol,extra); /*.*/

        sprintf(cntl,"cbdg"); /*4 chars*/
        sprintf(aname,"CA---CB -c-");
        sprintf(kol,"peach");
        Lvector = 1; /*vector*/
        sprintf(extra," %s",chain);
        transferout(aname,cntl,kol,extra); /*.*/

    }/*C-beta deviation from ideal output*/

/*deviations subgroup section________________________________________________*/
    if(Listsuppos)
    {/*superposition of all type aa onto ideal std aa*/
      /*do not do a subgroup*/
      sprintf(cntl,"supr"); /*4 chars*/
      sprintf(aname,"superpos"); /*not used*/
      sprintf(kol,"cyan"); /*not used*/
      Lvector = -1; /*list header provided in scratch file*/
      sprintf(extra," %s nobutton",chain); /*not used*/
      transferout(aname,cntl,kol,extra); /*.*/
    }/*superposition of all type aa onto ideal std aa*/

    if(Listext)
    {/*extra output*/
      /*if(!Lonegroup)*/  /*for Lonegroup there are masters on all lists*/
      /*   fprintf(fpoutput,"@subgroup {extra} "EOLO);*/

      sprintf(cntl,"ext "); /*4 chars*/
      sprintf(aname,"ext");
      if(colorextra[0]!='\0') {sprintf(kol,"%s",colorextra);} /*041106*/
      else {sprintf(kol,"green");}
      Lvector = 1;/* 1==vectors, <0 then no @____list line*/
      if(Lpperptobase){sprintf(extra,"master= {base-P perp}");} /*070731*/
      else {sprintf(extra,"master= {extra}");}
      transferout(aname,cntl,kol,extra); /*.*/

      sprintf(cntl,"ext2"); /*4 chars*/  /*041102*/
      sprintf(aname,"ext");
      sprintf(kol,"green");
      Lvector = 1;/* 1==vectors, <0 then no @____list line*/
      sprintf(extra,"master= {extra2}");
      transferout(aname,cntl,kol,extra); /*.*/

      sprintf(cntl,"extL"); /*4 chars*/
      sprintf(aname,"ext");
      sprintf(kol,"green");
      Lvector = 2;/* 2==label, <0 then no @____list line*/
      sprintf(extra,"master= {extraL}");
      transferout(aname,cntl,kol,extra); /*.*/

      sprintf(cntl,"extB"); /*4 chars*/
      sprintf(aname,"ext");
      sprintf(kol,"green");
      Lvector = 3;/* 0==Dot,3==ball, <0 then no @____list line*/
      if(Ldump){Lvector = -1;}
      /*ballradius = 4.0; 030519 remove so can use extB for different things*/
      sprintf(extra,"master= {extraB}");
      transferout(aname,cntl,kol,extra); /*.*/

      sprintf(cntl,"pdb "); /*4 chars*/
      sprintf(aname,"ext");
      sprintf(kol,"green");
      Lvector = -1;/* <0 then no @____list line*/
      sprintf(extra,"master= {extrapdb}");
      transferout(aname,cntl,kol,extra); /*.*/
    }/*extra output*/

    fprintf(fpoutput,EOLO); /*trailing new line at end of a pass*/
  }/*there is kinemage stuff to output*/
  else if(Ldump && LdumpVRML)
  {/*there is VRML coordinates to dump to output*/
     if(!LheaderVRML)
     {/*do this only once at first pass*/
        /*3D printer can only do VRML 1.0 spheres, cylinders, cubes */
        /*but FreeWRL only accepts file with VRML 2.0 header */
        fprintf(fpoutput,"#VRML V2.0 utf8"EOLO); /*050212 VRML 1.0 ascii*/
        LheaderVRML=1;
        /*overall centering fixed at first pass, saved as static*/
        VRMLx = -((fxmax+fxmin)/2);
        VRMLy = -((fymax+fymin)/2);
        VRMLz = -((fzmax+fzmin)/2);
     }
     if(LsplineVRML)
     {/*one or more splines to sort into strands*/
        LsplineVRMLout=1; LvectorVRMLout=0;LsphereVRMLout=0;/*specific control*/
        for(j=1; j<=nstrnd; j++)
        {
           fprintf(fpoutput,"Transform {  # splines..."EOLO);
           fprintf(fpoutput,"translation %.3f %.3f %.3f "EOLO
              ,VRMLx,VRMLy,VRMLz);
           fprintf(fpoutput,"children [  # of spline"EOLO);

           sprintf(cntl,"VR%2d",j);
           Lvector = -1; /*no other header sort of thing*/
           aname[0] = '\0'; kol[0] = '\0'; extra[0] = '\0';
           transferout(aname,cntl,kol,extra); /*.*/

           fprintf(fpoutput,"]  # end of spline children"EOLO);
           fprintf(fpoutput,"}  # end of splines."EOLO); /* of Transform*/
        }
     }
     if(LvectorVRML)
     {/*e.g. sidechain vectors to be drawn*/
        LsplineVRMLout=0; LvectorVRMLout=1;LsphereVRMLout=0;/*specific control*/
           fprintf(fpoutput,"Transform {  # vectors..."EOLO);
           fprintf(fpoutput,"translation %.3f %.3f %.3f "EOLO
              ,VRMLx,VRMLy,VRMLz);
           fprintf(fpoutput,"children [  # vector children..."EOLO);

           sprintf(cntl,"scv ");
           Lvector = -1; /*no other header sort of thing*/
           aname[0] = '\0'; kol[0] = '\0'; extra[0] = '\0';
           transferout(aname,cntl,kol,extra); /*.*/

           fprintf(fpoutput,"]  # end of vector children."EOLO);
           fprintf(fpoutput,"}  # end of vectors."EOLO); /* of Transform*/
     }
     if(LarrowVRML)
     {/*e.g. ribbon arrowhead vectors to be drawn*/
        LsplineVRMLout=0; LvectorVRMLout=1;LsphereVRMLout=0;/*specific control*/
           fprintf(fpoutput,"Transform {  # arrowhead..."EOLO);
           fprintf(fpoutput,"translation %.3f %.3f %.3f "EOLO
              ,VRMLx,VRMLy,VRMLz);
           fprintf(fpoutput,"children [  # arrowhead children..."EOLO);

           sprintf(cntl,"VRML");
           Lvector = -1; /*no other header sort of thing*/
           aname[0] = '\0'; kol[0] = '\0'; extra[0] = '\0';
           transferout(aname,cntl,kol,extra); /*.*/

           fprintf(fpoutput,"]  # end of arrowhead children."EOLO);
           fprintf(fpoutput,"}  # end of arrowhead."EOLO); /* of Transform*/
     }
     if(LsphereVRML)
     {/*e.g. sidechain atom balls to be written piecemeal*/
        LsplineVRMLout=0; LvectorVRMLout=0;LsphereVRMLout=1;/*specific control*/
           fprintf(fpoutput,"Transform {  # spheres..."EOLO);
           fprintf(fpoutput,"translation %.3f %.3f %.3f "EOLO
              ,VRMLx,VRMLy,VRMLz);
           fprintf(fpoutput,"children [  # sphere children..."EOLO);

           Lvector = -1; /*no other header sort of thing*/
           aname[0] = '\0';
           extra[0] = '\0';

           sprintf(cntl,"scn ");
           sprintf(kol," .07  .46  .84"); /*sky*/
           transferout(aname,cntl,kol,extra); /*.*/
           sprintf(cntl,"sco ");
           sprintf(kol," 1.0  0.0  0.0"); /*red*/
           transferout(aname,cntl,kol,extra); /*.*/
           sprintf(cntl,"scs ");
           sprintf(kol," .95  .93  0.0"); /*yellow*/
           transferout(aname,cntl,kol,extra); /*.*/

           fprintf(fpoutput,"]  # end of sphere children."EOLO);/* of children*/
           fprintf(fpoutput,"}  # end of spheres."EOLO); /* of Transformation*/
     }
     /*possibility of another pass, e.g. to get arrowheads, reset options:*/
     LsplineVRML=0;
     LvectorVRML=0;
     LsphereVRML=0;
     LarrowVRML=0;
  }
  else if(Ldump)
  {/*there is colon delineated stuff to output*/
     sprintf(cntl,"dump"); /*4 chars*/
     sprintf(aname," ");
     sprintf(kol," ");
     Lvector = -1;/* <0 then no @____list line, but some dumps have their own*/
     sprintf(extra," ");
     transferout(aname,cntl,kol,extra); /*.*/
     /*fprintf(fpoutput,EOLO);*/ /*trailing new line at end of a pass*/
  }/*there is colon delineated stuff to output*/

  if(LIOpipe) /*020514*/
  {
     fprintf(fpoutput,EOLO"@finish"EOLO); /*020514*/
     fflush(fpoutput);
  }
  /*not even extra line if no other output  030111*/
}
/*___writeoutput()__________________________________________________________*/

/****transferout()***********************************************************/
void   transferout(char aname[13],char cntl[5],char kol[20],char extra[64])
{
   char   scrts[256];
   int    j=0,LOK=0;
   long   iexist;
   char   header[256];
   char   keyword[32],colorword[32],masterword[36]; /*990126 mod 991122*/
   char   zbufferword[32]; /*rear,fore: thick black edge behind worm 090704*/
   char   xth = ' '; /*991128*/
   /*int    npoints=0;*/ /*011202in 030125out see SIZEofLISTs*/
   char   sizestr[32]; /*011202*/
   char   last[5]; /*050327*/
   int    iname=-1; /*counter for {colorset name} before {ptID} 060929*/
   int    iscrt=4;  /*first content index in temps[], changed by iname */

   if(Lspeciallistname) /*051129*/
   {
      strcpy(aname,speciallistname);  /*051129*/
   }

   rewindtextblock(&mainscratch);
   atEOF = 0; /*set up here so can control output later 040318*/

   if(cntl[2] == '\0') cntl[2] = ' ';
   if(cntl[3] == '\0') cntl[3] = ' ';
   cntl[4] = '\0';
   /*  cntl: two to four characters */
   last[0] = cntl[0]; /*protection for a change back to orig cntl 050327*/
   last[1] = cntl[1];
   last[2] = cntl[2];
   last[3] = cntl[3];
   last[4] = '\0';

#ifdef SIZEofLISTs
/*size of lists would be useful for allocating display storage*/
/*BUT is very non-kinemage-like and very confusing while hand editing kins*/
/*since we never got around to coding array allocation to use list size*/
/*actually outputing size has been jumpered out  030125 */

   if( (npoints = countpoints(cntl,1) ) > 0 )
   {/*scratch flag recognized and has finite count of its points*/
      sprintf(sizestr," size= %d",npoints);
      if(cntl[0]=='r' && cntl[1]=='b')
      {/*ribbons get divided into lots of little lists, count is for all*/
         sizestr[0] = '\0'; /*count not appropriate for individual rib lists*/
      }
   }
   else
#endif
   {
      sizestr[0] = '\0'; /*null string, takes no space on output line*/
   }

   iexist = 0; /* see if there is an entry before starting a @---list*/
   if(Lvector == 1)
   {
       sprintf(header,"@vectorlist {%s} color= %s %s%s"EOLO
          ,aname,kol,extra,sizestr);
   }
   else if(Lvector == 0)
   {
       sprintf(header,"@dotlist {%s} color= %s %s"EOLO
          ,aname,kol,sizestr);
   }
   else if(Lvector == 2)
   {
       sprintf(header,"@labellist {%s} color= %s %s%s"EOLO
          ,aname,kol,extra,sizestr);
   }
   else if(Lvector == 3)
   {
       sprintf(header,"@balllist {%s} color= %s radius= %.4f %s%s"EOLO
          ,aname,kol,ballradius,extra,sizestr); /*ballradius is a global*/
   }
   else if(Lvector == 4)
   {
       sprintf(header,"@spherelist {%s} color= %s %s%s"EOLO
          ,aname,kol,extra,sizestr);
   }
   else if(Lvector == 5)
   {
       sprintf(header,"@ribbonlist {%s} color= %s %s%s"EOLO
          ,aname,kol,extra,sizestr);
   }
   else if(Lvector == 6)
   {/*extra can have special ball radius, e.g. for water*/
       sprintf(header,"@balllist {%s} color= %s %s%s"EOLO
          ,aname,kol,extra,sizestr);
   }
   else if(Lvector == 7)  /*Listtabl for Ltable */
   {
       sprintf(header,"@wordlist {%s} color= %s %s\n"
          ,aname,kol,sizestr);
   }
   else if(Lvector == 8)
   {
       sprintf(header,"@trianglelist {%s} color= %s %s%s"EOLO
          ,aname,kol,extra,sizestr);
   }
   else if(Lvector < 0)
   {/*special or set the flag indicating list header already outputted*/
      if(Lcbdevdump)
      {/*colon delineated stuff to output for cbeta deviation stuff*/
         if(Lcbdevdumpmax) /*only get here at very end of processing 040318*/
         {/*just dump the maximum value*/
            fprintf(fpoutput,"%s :%8.3f"EOLO,PDBfileStr,cbetadevmax);
            /*PDBfileStr == ShortNameStr truncated at first '.' , i.e. no ext*/
            /*ShortNameStr is input file name sans any directory path info*/
            atEOF = 1; /*simulate end of scratch file*/
         }
         else if(!Ldid_outfileheader)
         {/*write the header for the dump results to follow*/
            sprintf(header
             ,"pdb:alt:res:chainID:resnum:dev:dihedralNABB:Occ:ALT:"EOLO);
            Ldid_outfileheader = 1; /*only do this once per protein*/
         }
         else
         {
            iexist = 1; /*no more headers*/
         }
      }
      else if(Ldumppperptobase)
      {/*colon delineated stuff to output for perpendiculars to bases*/
         if(!Ldid_outfileheader)
         {/*write the header for the dump results to follow*/
            sprintf(header
             ,":pdb:res:chainID : resnum: 5'Pdist : 3'Pdist :delta:Outlier:epsilon:Outlier:"EOLO);
             /*delta:Outlier==X  040705*/
             /*epsilon:Outlier==X  070521*/
            Ldid_outfileheader = 1; /*only do this once per protein*/
         }
         else
         {
            iexist = 1; /*no more headers*/
         }
      }
      else if(Ldumpcispeptides) /*041023*/
      {/*cis peptide info stuff to output*/
         if(!Ldid_outfileheader)
         {/*write the header for the dump results to follow*/
            fprintf(fpoutput,"@text"EOLO"%s : cis-peptides"EOLO,PDBfileStr);
            Ldid_outfileheader = 1; /*only do this once per protein*/
            iexist = 1; /*no other headers*/
         }
         else
         {
            iexist = 1; /*no more headers*/
         }
      }
      else
      {
         iexist = 1; /*no header will preceed this set*/
      }
   }/*special or set the flag indicating list header already outputted*/

   while(!atEOF)
   {/*scan through scratch block*/
      getonetextblockline(&mainscratch,temps); /*PKINSCRT.c*/
      if(temps[0] != '\0')
      {/*entries still in scratch block*/

/*fprintf(stderr,"temps== %s\n",temps);*/

         if(   (temps[0]==last[0])  /*check all characters*/
             &&(temps[1]==last[1])  /*against the last cntl actually used*/
             &&(temps[2]==last[2])  /*use last instead of orig cntl 050327*/
             &&(temps[3]==last[3])  )
         {
            LOK = 1;/*this point belongs in this list*/
         }
         else if(     (temps[0]==last[0])  /*check first two characters*/
                   && (temps[1]==last[1])
                   &&((temps[2]!=last[2]) /*if last two characters change*/
                   || (temps[3]!=last[3]))
                )/*, may need new list*/
         {/*last 2 char mismatch*/
            iname = -1; /*reset here so new cntl type rules 060929*/
            iscrt = 4;
            if(   (temps[0]=='r')  /*check first 2 characters*/   /*990126*/
                &&(temps[1]=='b')
              )
            {/*is a ribbon*/   /*ribbon, may need new list   Listrib*/
               /*@colorset {name} color  */
               /*unique name and standard color as char strings*/
               /*on list use:  color= {name}  instead of  color= color  */
              /*ribbonlist interruption, needs new header== @ribbonlist line*/
              LOK = 1;/*presume this point belongs in this list*/
              zbufferword[0] = '\0'; /*clear special use header entry 090704*/
              if(  (temps[3]=='c')  /*single strand worm, nucleic coil,...*/
                 ||(temps[3]=='e')  /*black rearground for single strand worm*/
                 ||(temps[3]==' ')  /*old default*/
                /*coil edge is now rbce 090704*/
                )
              {
                 sprintf(keyword,"@vectorlist ");
              }
              else if(temps[3]=='f')  /*face*/
              {
                 if(Lnoface) { sprintf(keyword,"@vectorlist "); } /*040319*/
                 else        { sprintf(keyword,"@ribbonlist "); }
              }
              else if(temps[2]=='g')  /*guide point 050327*/
              {
                 { sprintf(keyword,"@dotlist "); } /*050327*/
              }
              else if(temps[2]=='s' || temps[2]=='h') /*splines,Hbonds 060113*/
              {
                 { sprintf(keyword,"@vectorlist "); }
              }
              else
              {
                 LOK = 0; /*wrong characters for a ribbon*/
              }
              if(LOK)
              {/*further define ribbon interruption*/
                if(Lexplicitribbon > 0)
                {/*ALPHA, BETA arrows*/
                   if(     temps[2]=='a')  /*alpha*/
                   {  /*topname is name of this group*/
                      /*topID has only chain and subunit info for short name*/
                      if(temps[3]=='e' && Lblackedge)
                         {sprintf(colorword,"{edge_%s} ",topID);}
                      else { sprintf(colorword,"{alph_%s} ",topID);}
                      if(Lribbonmasters) /*051128*/
                           {sprintf(masterword," master= {alpha}");}
                      else {masterword[0] = '\0';}
                   }
                   else if(temps[2]=='b')  /*beta*/
                   {
                      if(temps[3]=='e' && Lblackedge)
                         {sprintf(colorword,"{edge_%s} ",topID);}
                      else { sprintf(colorword,"{beta_%s} ",topID);}
                      if(Lribbonmasters) /*051128*/
                           {sprintf(masterword," master= {beta}");}
                      else {masterword[0] = '\0';}
                   }
                   else if(temps[2]=='B')  /*beta colorset={...}*/
                   {
                      /*{colorset name} preceeds {ptID} */
                      iname = -1; /*so increment can be before statement*/
                      do {iname++;colorword[iname]=temps[4+iname];}
                      while(colorword[iname] != '}');  /*ribidentity*/
                      colorword[iname+1] = '\0'; /*end of {ribidentity}*/
                      if(Lribbonmasters) /*051128*/
                           {sprintf(masterword," master= {beta}");}
                      else {masterword[0] = '\0';}
                   }
                   else if(temps[2]=='n')  /*nucleic acid*/
                   {
                      if(temps[3]=='e' && Lblackedge)
                         {sprintf(colorword,"{edge_%s} ",topID);}
                      else { sprintf(colorword,"{nucl_%s} ",topID); }
                      if(Lribbonmasters) /*051128*/
                           {sprintf(masterword," master= {nucleic}");}
                      else {masterword[0] = '\0';}
                   }
                   else if(temps[2]=='c' || temps[2]==' ')  /*coil*/
                   {
                      sprintf(colorword,"{coil_%s} ",topID);
                      if(Lribbonmasters) /*051128*/
                           {sprintf(masterword," master= {coil}");}
                      else {masterword[0] = '\0';}
                      if(temps[3]=='e') /*thick black in rear for edge 090704*/
                      {sprintf(zbufferword,"rear");}
                      else if(temps[3]=='c') /*fore colored worm 090704*/
                      {sprintf(zbufferword,"fore");}

                   }
                   else
                   {
                      LOK = 0; /*wrong characters for a ribbon*/
                   }
                }/*ALPHA, BETA arrows*/
                else
                {/*old, sooothed, single ribbon*/
                   masterword[0] = '\0';/*no master here for smoothed ribbon*/
                   if(temps[3]=='e' && Lblackedge)
                        {sprintf(colorword,"{edge_%s} ",topID);}
                   else {sprintf(colorword,"{coil_%s} ",topID);} /*040924*/
                   /*sprintf(colorword,"{coil} ");*/
                }
                if(  temps[2]=='g' && temps[3]=='p' ) /*guide points 050327*/
                {
                   /*override master= {ribbon} */
                   sprintf(extra," master= {guide pts}");
                   masterword[0] = '\0';
                   sprintf(colorword,"{guid_%s} ",topID);
                   LOK = 1;
                }
                else if(  temps[2]=='s' && temps[3]=='p' ) /*splines 060113*/
                {
                   /*override master= {ribbon} */
                   sprintf(extra," master= {splines}");
                   masterword[0] = '\0';
                   sprintf(colorword,"{guid_%s} ",topID);
                   LOK = 1;
                }
                else if(  temps[2]=='h' && temps[3]=='b' ) /*Hbonds 060113*/
                {
                   /*override master= {ribbon} */
                   sprintf(extra," master= {ribHbonds}");
                   masterword[0] = '\0';
                   sprintf(colorword,"{guid_%s} ",topID);
                   LOK = 1;
                }
                else if(Lribbonguidepts || Lribbonrawsplines || Lribbonhbonds)
                {/*override master that matches ribbon subgroup*/
                   sprintf(extra," master= {ribPlain}");
                   masterword[0] = '\0';
                }
              }/*further define ribbon interruption*/
              if(LOK)
              {/*finish defining ribbon interruption*/

                 sprintf(header
                   ,"%s {%s} color= %s %s %s nobutton%s\n"
                   ,keyword,aname,colorword,zbufferword    /*090704*/
                   ,extra,masterword);

                 iexist = 0; /*set for new header to be inserted*/
                 /*using this point, so reset last: */
                 last[0] = temps[0];
                 last[1] = temps[1];
                 last[2] = temps[2];
                 last[3] = temps[3];
              }/*finish defining ribbon interruption*/
              else
              {
                 LOK = 0;/*can't use this point in this list*/
              }
           }/*is a ribbon*/
           else if(   (temps[0]=='s') /*check first 2 characters*/ /*991128*/
                    &&(temps[1]=='c')
                  )
           {/*sidechain something*/
              if(temps[2]==cntl[2] && cntl[2]=='v') /* 3rd character: vector*/
              {/*sidechain vectors*/ /*check 4th character*/
                if(temps[3] != xth) /*both== ' ' for single list*/
                {/*Listscv may need new list*/
                  xth = temps[3]; /*reset indicator*/
                  iexist = 0; /*reset counter so get a new header== vectorlist*/
                  sprintf(header,"@vectorlist {%s} color= %s nobutton %s"EOLO
                     ,aname,kol,extra);
                }
                LOK = 1; /*point belongs in this list*/
              }/*sidechain vectors*/
              else
              {
                 LOK = 0;/*can't use this point in this list*/
              }
           }/*sidechain something*/
           else
           {/*NOT a recognized deviation, NOT a point in this list*/
              LOK = 0;
           }
         }/*last 2 char mismatch*/
         else
         {/*unrecognized mismatch, can't use this point in this list*/
            LOK = 0;
         }


         if(LOK)
         {/*this point belongs in this list*/
            if(iname != -1){iscrt = 4 + 1 + iname;} /*060929*/
            else {iscrt = 4;} /*first content index in temps[] 060929*/
            for( j=iscrt ; j<=255 ; j++) /*extract content from temps[] */
            {
                scrts[j-iscrt] = temps[j];
                if(temps[j] == '\0') break;
            }
            if(LdumpVRML) /*050208*/
            {
               if(LvectorVRMLout || LsplineVRMLout) /*050208,050211*/
               {/*scrts already in VRML format by writeVRMLvectortoscratch()*/
                  fprintf(fpoutput,"%s",scrts);
               }
               else if(LsphereVRMLout) /*050209*/
               {
                  writeVRMLsphere(scrts,kol);
               }
            }
            else
            {
              iexist++; /*increase counter*/
              if(iexist==1)
              {/*first occurance of point on this pass, DO need @---list*/
                 fprintf(fpoutput,"%s",header);
              }
              /*print point entry*/

              /*931128 this is the major output print statement*/
              /* so at least flag write error here: ever checked???*/
              /*if(fprintf(fpoutput,"%s",scrts) < 0) Lfpoutput = -1;*/

              fprintf(fpoutput,"%s",scrts);
            }
         }/*this point belongs in this list*/

     }/*entries still in scratch block*/
     else
     {/*reached end of scratch block*/
          atEOF = 1;
     }
   }/*scan through scratch block*/
}
/*___transferout()__________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****writeVRMLvectorextrusion()***********************************************/
void writeVRMLvectorextrusion(char scrts[256])
{  /*LvectorVRML: */
   /*cylinder with a same radius ball at end for smooth end or joint to next*/
   char sphstr[256];
   int  j=0,k=0,m=0,n=0,next=0;

           /*VRML 2.0 extrusion needs 3 spine points for correct end vectors*/
           /*so insert a midpoint */
           /* sprintf(temps,"%s %.3f  %.3f  %.3f "   */
           /*               ", %.3f  %.3f  %.3f "    */
           /*               ", %.3f  %.3f  %.3f"EOLO */
           /*      ,cntl                             */
           /*      ,x[k],y[k],z[k]                   */
           /*      ,(x[k]+x[j])/2,(y[k]+y[j])/2,(z[k]+z[j])/2 */
           /*     ,x[j],y[j],z[j]);                  */

   /*extrude a cylinder along the vector*/
      fprintf(fpoutput,"Extrusion {  # begin Extrusion of a vector"EOLO);
      fprintf(fpoutput,"    scale 1.0 1.0 "EOLO);
      fprintf(fpoutput,"    orientation 0.0 0.0 1.0 0.0 "EOLO);
      fprintf(fpoutput,"    solid TRUE "EOLO);
      fprintf(fpoutput,"    beginCap TRUE "EOLO);
      fprintf(fpoutput,"    endCap TRUE "EOLO);
      fprintf(fpoutput,"    ccw TRUE "EOLO);
      fprintf(fpoutput,"    creaseAngle 3.14 "EOLO);
      fprintf(fpoutput,"    crossSection ["EOLO);
      VRMLxSectionCircle( ribwidVRML/nstrnd ); /*cross section is a circle*/
      fprintf(fpoutput,"    ]"EOLO);
      fprintf(fpoutput,"    spine ["EOLO);
      fprintf(fpoutput,"%s",scrts); /*this vector*/
      fprintf(fpoutput,"    ]"EOLO);
      fprintf(fpoutput,"}  # end Extrusion of a vector"EOLO); /* of extrusion*/

      fprintf(fpoutput,"Transform {  # for capping ball"EOLO);
      /*parse out the end point of the vector, which is after the last comma*/
      n = strlen(scrts);
      m = 0;
      while(m<n){if(scrts[m++]==','){next=m;}} /*next index after last comma*/
      for(j=next; j<n; j++){sphstr[k++]=scrts[j];}
      sphstr[k] = '\0'; /*end the string*/
      fprintf(fpoutput,"translation %s "EOLO,sphstr); /*put a ball on the end*/
      fprintf(fpoutput,"children [  # of capping ball"EOLO);
      fprintf(fpoutput,"Shape {  # of capping ball"EOLO);
      fprintf(fpoutput,"geometry Sphere {"EOLO);
      fprintf(fpoutput,"    radius %.3f"EOLO,(ribwidVRML/nstrnd));
      fprintf(fpoutput,"    }"EOLO); /* of Sphere */
      fprintf(fpoutput,"}  # end Shape of capping ball"EOLO);
      fprintf(fpoutput,"]  # end children of capping ball"EOLO);
      fprintf(fpoutput,"}  # end Transform for capping ball"EOLO); /*Transform*/
}
/*___writeVRMLvectorextrusion________________________________________________*/

/****writeVRMLsphere()********************************************************/
void writeVRMLsphere(char scrts[256], char kol[20])
{  /*LsphereVRML: sphere as atom ball*/
   /*ballradius is a global*/

      fprintf(fpoutput,"Transform { "EOLO);
      /*parse out the end point of the vector, which is after the last comma*/
      fprintf(fpoutput,"translation %s "EOLO,scrts); /*put a ball here*/
      fprintf(fpoutput,"children [ "EOLO);
      fprintf(fpoutput,"Shape { "EOLO);

      fprintf(fpoutput,"appearance Appearance { "EOLO);
      fprintf(fpoutput,"material Material  { "EOLO);
      fprintf(fpoutput,"   diffuseColor %s"EOLO,kol);
      fprintf(fpoutput,"  } "EOLO);
      fprintf(fpoutput,"} "EOLO);

      fprintf(fpoutput,"geometry Sphere {"EOLO);
      fprintf(fpoutput,"    radius %.3f"EOLO,ballradius);
      fprintf(fpoutput,"    }"EOLO); /* of Sphere */
      fprintf(fpoutput,"} "EOLO);
      fprintf(fpoutput,"] "EOLO);
      fprintf(fpoutput,"} "EOLO); /*Transform*/
}
/*___writeVRMLsphere_________________________________________________________*/

/****VRMLxSectionCircle()*****************************************************/
void VRMLxSectionCircle(float radius)
{
   int  ierr = 0, j=0;

   float A[3],B[3],C[SEGMENTS][3];

   /*VRML circle for cross section is defined in XZ plane with axis along Y*/
   /*cross section is oriented s.t. its Y axis points along spine tangent*/
   A[0] =  0; A[1] = 10; A[2] =  0;
   B[0] =  0; B[1] =  0; B[2] =  0;
   ierr = constructcircle(C, A, B, radius);
   if(!ierr)
   {/*output xSectionCircle as 2D object in XZ plane*/
      for(j=0; j<SEGMENTS; j++)
      {
         fprintf(fpoutput,"  %.3f %.3f, "EOLO,C[j][0],C[j][2]);
      }
      /*and close the circle: */
         j=0;
         fprintf(fpoutput,"  %.3f %.3f, "EOLO,C[j][0],C[j][2]);
   }
}
/*___VRMLxSectionCircle()____________________________________________________*/


/****writeVRMLvectortoscratch()***********************************************/
void writeVRMLvectortoscratch(char cntl[5], char color[16], float radius, char whostr[32], float x1, float y1, float z1, float x2, float y2, float z2)
{  /*LvectorVRML: */
   /*cylinder with a same radius ball at end for smooth end or joint to next*/
   double ax=0,ay=0,az=0,bx=0,by=0,bz=0,cx=0,cy=0,cz=0,VRMLangle=0; /*050211*/
   double amag=0,bmag=0;
   double mx=0,my=0,mz=0; /*for midpoint of bond*/

   /*VRML cylinder with ball at head for smooth joint to next 050211*/
   /*VRML Transform node needs translation and rotation*/
   /*translate from origin to vector midpoint*/
   /*rotation is around crossproduct axis by dotproduct angle*/
   bx = ((double)x2 -(double)x1);
   by = ((double)y2 -(double)y1);
   bz = ((double)z2 -(double)z1);
   amag = bmag = sqrt(bx*bx + by*by + bz*bz); /*cylinder height*/
   /*midpoint of bond*/
   mx=(((double)x2+(double)x1)/2);
   my=(((double)y2+(double)y1)/2);
   mz=(((double)z2+(double)z1)/2);

   /*cylinder defined as along original y axis, centered on origin*/
   ax = (double)0;
   ay = bmag; /*cylinder height*/
   az = (double)0;
   docrossproduct(ax/amag,ay/amag,az/amag,bx/bmag,by/bmag,bz/bmag,&cx,&cy,&cz);
   /*cos(angle) == dotproduct of normalized vectors a and b */
   /*ax==0 and az==0, amag==ay, bmag==ay: so (ay/ay)*(by/ay)== by/ay */
   /*cos angle == (ax*bx + ay*by + az*bz)/(amag*bmag) == (by/ay) */
   VRMLangle = acos(by/ay);

   /*depend on transferout() to keep successive records in order for same cntl*/

   sprintf(temps,"%sTransform { translation %.3f %.3f %.3f "
      "rotation %.3f %.3f %.3f %.3f children [ "
      "Shape { "EOLO
      ,cntl ,mx,my,mz,cx,cy,cz,VRMLangle);
   putonetextblockline(&mainscratch,temps);

   /*sprintf(temps,"%s appearance Appearance { material Material {" */
   /*            " diffuseColor .70 .70 .90 } }"EOLO,cntl);         */
   sprintf(temps,"%s appearance Appearance { material Material {"
                 " diffuseColor 1.0 1.0 1.0 } }"EOLO,cntl);  /*050329*/
   putonetextblockline(&mainscratch,temps);

   sprintf(temps,"%s geometry Cylinder { radius %.3f height %.3f} } ] } #%s"EOLO
      ,cntl ,radius,bmag,whostr);
   putonetextblockline(&mainscratch,temps);

   /*place a ball at the end point*/
   sprintf(temps,"%s Transform { translation %.3f %.3f %.3f "
      "children [ "
      "Shape { "EOLO
      ,cntl ,x2,y2,z2);
   putonetextblockline(&mainscratch,temps);

   sprintf(temps,"%s appearance Appearance { material Material {"
                 " diffuseColor 1.0 1.0 1.0 } }"EOLO,cntl);
   putonetextblockline(&mainscratch,temps);

   sprintf(temps,"%s geometry Sphere { radius %.3f } } ] } #%s"EOLO
      ,cntl ,radius,whostr);
   putonetextblockline(&mainscratch,temps);

   if(mx > fxmax){fxmax = (float)mx;}
   if(mx < fxmin){fxmin = (float)mx;}
   if(my > fymax){fymax = (float)my;}
   if(my < fymin){fymin = (float)my;}
   if(mz > fzmax){fzmax = (float)mz;}
   if(mz < fzmin){fzmin = (float)mz;}
}
/*___writeVRMLvectortoscratch________________________________________________*/


