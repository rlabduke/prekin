/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*                      PKINCSBS.c           */
#include "PKIN.h"
#include "PKINDLOG.h"
#include "PKINCRTL.h" /*070829 for LGFPchromophore*/

  typedef struct resatomstruct {
     char subc[3];
     char resc[5];
     char numc[6];
     char rinsc[3];
     char atomc[7];
  }resatomstruct;
static  struct resatomstruct* resatomptr;
static  struct resatomstruct* resAtomOLDptr;
static  struct resatomstruct* resAtomNEWptr;

/*prototypes used entirely within this source file*/
void writesctoscratch(int,char*);
void tesselatescrings(char*);
struct resatomstruct* allocresatomstruct(void); /*061202*/
void decoderesatom(resatomstruct*, char sub[3],char res[4],int num,char rins[2],char atom[6]); /*061202*/

/****distan()****************************************************************/
float  distan(double xa,double ya,double za,double x1,double y1,double z1)
{
    double  x2,y2,z2;
    float   freturn;

    x2 = ( xa - x1 )*( xa - x1 );
    y2 = ( ya - y1 )*( ya - y1 );
    z2 = ( za - z1 )*( za - z1 );
    freturn = (float)sqrt( x2 + y2 + z2 );

    return(freturn);
}
/*__distan()________________________________________________________________*/

/****distanfa4()***************************************************************/
float  distanfa4(float xyza[4], float xyz1[4])
{
    double  xa=0,ya=0,za=0,x1=0,y1=0,z1=0,x2=0,y2=0,z2=0;
    float   freturn;

    xa = (double)xyza[1];
    ya = (double)xyza[2];
    za = (double)xyza[3];
    x1 = (double)xyz1[1];
    y1 = (double)xyz1[2];
    z1 = (double)xyz1[3];

    x2 = ( xa - x1 )*( xa - x1 );
    y2 = ( ya - y1 )*( ya - y1 );
    z2 = ( za - z1 )*( za - z1 );
    freturn = (float)sqrt( x2 + y2 + z2 );

    return(freturn);
}
/*__distanfa4()_______________________________________________________________*/

/****uctolctexts()***********************************************************/
void  uctolctexts(char texts[256])
{
  int  j,k,match;
static  char  uc[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static  char  lc[] = " abcdefghijklmnopqrstuvwxyz";

  /*converts upper case alphabetic char to lower case */
  for(k=0 ; k<80 ; k++)  /*test everything for alphabetic character */
  {
    match = 0;
    for(j=1 ; j<= 26 ; j++)
    {
      if(texts[k] == uc[j])
      {
        texts[k] = lc[j];
        match = 1;
      }
      if(match==1) break;
    }
  }
  return;
}
/*__uctolctexts()___________________________________________________________*/

/****lctouc()****************************************************************/
char lctouc(char charic)
{
  int  j,match;
static  char  uc[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static  char  lc[] = " abcdefghijklmnopqrstuvwxyz";

  /*converts lower case alphabetic char to upper case */
    match = 0;
    for(j=1 ; j<= 26 ; j++)
    {
      if(charic == lc[j])
      {
        charic = uc[j];
        match = 1;
      }
      if(match==1) break;
    }
  return(charic);
}
/*__lctouc()________________________________________________________________*/

/****uctolc()****************************************************************/
char uctolc(char charic)
{
  int  j,match;
static  char  uc[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static  char  lc[] = " abcdefghijklmnopqrstuvwxyz";

  /*converts upper case alphabetic char to lower case */
    match = 0;
    for(j=1 ; j<= 26 ; j++)
    {
      if(charic == uc[j])
      {
        charic = lc[j];
        match = 1;
      }
      if(match==1) break;
    }
  return(charic);
}
/*__uctolc()________________________________________________________________*/

/****allocresatomstruct()*****************************************************/
struct resatomstruct* allocresatomstruct(void)
{
   struct resatomstruct* thereturnptr = NULL;

   thereturnptr = (resatomstruct*)malloc(sizeof(resatomstruct));
   if(thereturnptr != NULL)
   {
      thereturnptr->subc[0]   = ' ';
      thereturnptr->subc[1]   = '\0';
      thereturnptr->resc[0]   = ' ';
      thereturnptr->resc[1]   = ' ';
      thereturnptr->resc[2]   = ' ';
      thereturnptr->resc[3]   = '\0';
      thereturnptr->numc[0]   = ' ';
      thereturnptr->numc[1]   = ' ';
      thereturnptr->numc[2]   = ' ';
      thereturnptr->numc[3]   = ' ';
      thereturnptr->numc[4]   = '\0';
      thereturnptr->rinsc[0]  = ' ';
      thereturnptr->rinsc[1]  = '\0';
      thereturnptr->atomc[0]  = ' ';
      thereturnptr->atomc[1]  = ' ';
      thereturnptr->atomc[2]  = ' ';
      thereturnptr->atomc[3]  = ' ';
      thereturnptr->atomc[4]  = ' ';
      thereturnptr->atomc[5]  = '\0';
   }
   return(thereturnptr);
}
/*___allocresatomstruct()____________________________________________________*/

/****decoderesatom()**********************************************************/
void decoderesatom(resatomstruct* resatomptr, char sub[3],char res[4],int num,char rins[2],char atom[6]) /*061202*/ /*071222*/
{
 int j=0;

 if(resatomptr != NULL)
 {
  /*convert to 2char subunit==chainID   071222*/
  if(  (sub[0] == '\0' || sub[0] == ' ')
     &&(sub[1] == '\0' || sub[1] == ' ') )
  {
      resatomptr->subc[0] = ' ';
      resatomptr->subc[1] = '_';
      resatomptr->subc[2] = '\0';
  }
  else
  {
      resatomptr->subc[0] = lctouc(sub[0]);
      resatomptr->subc[1] = lctouc(sub[1]);
      resatomptr->subc[2] = '\0';
  }

  sprintf(resatomptr->resc,"%s",res);
  for(j=0; j<3; j++){if(resatomptr->resc[j] == ' '){resatomptr->resc[j] = '_';}
                     else{resatomptr->resc[j] = lctouc(resatomptr->resc[j]);} }
  resatomptr->resc[3] = '\0';

  sprintf(resatomptr->numc,"%4d",num);
  for(j=0; j<4; j++){if(resatomptr->numc[j] ==' '){resatomptr->numc[j] ='_';} }
  resatomptr->numc[4] = '\0';

  if(rins[0] == '\0' || rins[0] == ' ')
       {resatomptr->rinsc[0] = '_'; resatomptr->rinsc[1] = '\0';}
  else {resatomptr->rinsc[0] = lctouc(rins[0]); resatomptr->rinsc[1] = '\0';}

  sprintf(resatomptr->atomc,"%s",atom);
  for(j=0; j<5; j++) {if(resatomptr->atomc[j] ==' '){resatomptr->atomc[j] ='_';}
                   else{resatomptr->atomc[j] = lctouc(resatomptr->atomc[j]);} }
  resatomptr->atomc[5] = '\0';
 }
}
/*___decoderesatom()_________________________________________________________*/

/*connect current residue back to previous residue*/
/****oldcon()****************************************************************/
void  oldcon(int jatom, float setdst, int ica)
{
  int     didit,noldat,k,ny;
  float   dist = -1; /*negative if not calculated*/
  char    YN[2][2]={ {'X','\0'},{'-','\0'},};
  char    subc[2],resc[4],numc[5],rinsc[2],atomc[6];
  int     j=0;
  int     Lstderr = 0;
  int     jconn = 0; /*atom for which base[jconn] = noldat*/
  static int Lneedconnectheader=1; /*need header even when first residue wierd*/

  /* set offset for mainchain or ca-ca connection */
  k = 0;                   /*mainchain*/
  if( ica != 0 ) {k = 4;}  /*ca-ca*/
  base[jatom] = jatom;     /*default to a dot */
  discon = TRUE;           /* default discon...nected flag */
  /*--check to see if alternate conf flags set */
  /*--atom[jatom][4] holds the alt conf character for atom jatom*/

  jconn = jatom; /*default*/

  if(atom[jatom][4] == ' ')
  {
    if(oldatm[ica])
    {
      noldat = lastat - k;
      dist = (float)distan(x[jatom],y[jatom],z[jatom]
                          ,x[noldat],y[noldat],z[noldat]);
      if(dist <= setdst && icountres[level] > 0)  /*061202*/
      {
          base[jatom] = noldat;
          jconn = jatom;
      }
    /*limited logic:  connect ' ' with ' ',  ... or */
    }
    else if(oldata[ica] && !altcfa[ica])
    {
      noldat = lastat - k - 1;
      dist = (float)distan(x[jatom],y[jatom],z[jatom]
                          ,x[noldat],y[noldat],z[noldat]);
      if(dist <= setdst)
      {
          base[jatom] = noldat;
          jconn = jatom;
      }
    }
    /*--connect ' ' to 'b' only if this residue doesn't have 'b' conf */
    if(oldatb[ica] && !altcfb[ica])
    {
      noldat = lastat - k - 2;
      /*--create extra atom, connect it to 'b', then ...*/
/*.*/    didit = creatm(natom);
      if(didit)
      {
/*.*/   dist = (float)distan(x[jatom],y[jatom],z[jatom]
                            ,x[noldat],y[noldat],z[noldat]);
        if(dist <= setdst)
        {
            base[maxatm] = noldat;
            jconn = maxatm;
        }
      }
    }
    if(oldatc[ica] && !altcfc[ica])
    {
      noldat = lastat - k - 3;
      /*--create extra atom, connect it to 'c', then ...*/
/*.*/    didit = creatm(natom);
      if(didit)
      {
/*.*/      dist = (float)distan(x[jatom],y[jatom],z[jatom]
                               ,x[noldat],y[noldat],z[noldat]);
        if(dist <= setdst)
        {
            base[maxatm] = noldat;
            jconn = maxatm;
        }
      }
    }
  }
  else if(atom[jatom][4] == 'a')
  {
    if(oldata[ica])
    {
      noldat = lastat - k - 1;
/*.*/    dist = (float)distan(x[jatom],y[jatom],z[jatom]
                             ,x[noldat],y[noldat],z[noldat]);
      if(dist <= setdst)
      {
          base[jatom] = noldat;
          jconn = jatom;
      }
    }
    else if(oldatm[ica])
    {
      /*-- use lastat unless previous mainchain had an alt conf */
      noldat = lastat - k;
/*.*/    dist = (float)distan(x[jatom],y[jatom],z[jatom]
                             ,x[noldat],y[noldat],z[noldat]);
      if(dist <= setdst)
      {
          base[jatom] = noldat;
          jconn = jatom;
      }
    }
  }
  else if(atom[jatom][4] == 'b')
  {
    if(oldatb[ica])
    {
      noldat = lastat - k - 2;
/*.*/    dist = (float)distan(x[jatom],y[jatom],z[jatom]
                             ,x[noldat],y[noldat],z[noldat]);
      if(dist <= setdst)
      {
           base[jatom] = noldat;
           jconn = jatom;
      }
    }
    else if(oldatm[ica])
    {
      /*-- use lastat unless previous mainchain had an alt conf */
      noldat = lastat - k;
/*.*/    dist = (float)distan(x[jatom],y[jatom],z[jatom]
                             ,x[noldat],y[noldat],z[noldat]);
      if(dist <= setdst)
      {
           base[jatom] = noldat;
           jconn = jatom;
      }
    }
  }
  else if(atom[jatom][4] == 'c')
  {
    if(oldatc[ica])
    {
      noldat = lastat - k - 3;
/*.*/    dist = (float)distan(x[jatom],y[jatom],z[jatom]
                             ,x[noldat],y[noldat],z[noldat]);
      if(dist <= setdst)
      {
          base[jatom] = noldat;
          jconn = jatom;
      }
    }
    else if(oldatm[ica])
    {
      /*-- use lastat unless previous mainchain had an alt conf */
      noldat = lastat - k;
/*.*/    dist = (float)distan(x[jatom],y[jatom],z[jatom]
                             ,x[noldat],y[noldat],z[noldat]);
      if(dist <= setdst)
      {
          base[jatom] = noldat;
          jconn = jatom;
      }
    }
  }

  /*--defaults had been set so: base(jatom) = jatom & discon = TRUE */
  if(base[jatom] != jatom) discon = FALSE;
  /*--i.e., something got connected! */

  if(Lconnectresiduesdump) /* 061202 in oldcon() */
  {/*Lconnectresiduesdump*/
     if(icountres[level] == 0 || Lneedconnectheader)
     {
        if(level == 1) /*header for dump...*/
        {
          if(Lstderr)
          {
fprintf(stderr,"typechainfirstpass[level]== %c\n",typechainfirstpass[level]);
           fprintf(stderr,"dump:%s: connect distance <= %.3f\n"
              ,NameStr,setdst);
          }
           sprintf(temps,"dump:%s: connect distance <= %.3f:"EOLO
              ,NameStr,setdst);
           putonetextblockline(&mainscratch,temps);
           Lneedconnectheader = 0;
        }
        if(resAtomNEWptr == NULL) {resAtomNEWptr = allocresatomstruct();}
        if(resAtomNEWptr != NULL)
        {
           decoderesatom(resAtomNEWptr,sub[jconn],res[jconn],num[jconn],rins[jconn],atom[jconn]);
          if(Lstderr)
          {
           fprintf(stderr,"%2d # %5d back :%s:%s:%s%s:%s:<E-:\n"
              ,level,icountres[level]
              ,resAtomNEWptr->subc,resAtomNEWptr->resc,resAtomNEWptr->numc
              ,resAtomNEWptr->rinsc,resAtomNEWptr->atomc
           );
          }
           sprintf(temps,"dump:%s:%s:%s%s:%s:<E-:"EOLO
              ,resAtomNEWptr->subc,resAtomNEWptr->resc,resAtomNEWptr->numc
              ,resAtomNEWptr->rinsc,resAtomNEWptr->atomc
           );
           putonetextblockline(&mainscratch,temps);
        }
     }
     else
     {/*in chain...*/
        if(dist >= 0)
        {/*distance was calculated, so noldat exists for this residue and atom*/
           if(base[jconn] == noldat) {ny = 1;} else {ny = 0;}

           if(resAtomOLDptr == NULL) {resAtomOLDptr = allocresatomstruct();}
           if(resAtomOLDptr != NULL)
           {
              decoderesatom(resAtomOLDptr,sub[noldat],res[noldat],num[noldat],rins[noldat],atom[noldat]);
           }
           if(resAtomNEWptr == NULL) {resAtomNEWptr = allocresatomstruct();}
           if(resAtomNEWptr != NULL)
           {
              decoderesatom(resAtomNEWptr,sub[jconn],res[jconn],num[jconn],rins[jconn],atom[jconn]);
           }

           if(resAtomNEWptr != NULL && resAtomOLDptr != NULL )
           {
             if(Lstderr)
             {
              fprintf(stderr,"%2d # %5d fore :%s:%s:%s%s:%s:-%s>:%.3f"
                 ,level, icountres[level]-1
                 ,resAtomOLDptr->subc,resAtomOLDptr->resc,resAtomOLDptr->numc
                 ,resAtomOLDptr->rinsc,resAtomOLDptr->atomc
                 ,YN[ny],dist
              );
              fprintf(stderr, ":%s:%s:%s%s:%s:\n\n"
                 ,resAtomNEWptr->subc,resAtomNEWptr->resc,resAtomNEWptr->numc
                 ,resAtomNEWptr->rinsc,resAtomNEWptr->atomc
              );

              fprintf(stderr,"%2d # %5d back :%s:%s:%s%s:%s:<%s-:%.3f"
                 ,level, icountres[level]
                 ,resAtomNEWptr->subc,resAtomNEWptr->resc,resAtomNEWptr->numc
                 ,resAtomNEWptr->rinsc,resAtomNEWptr->atomc
                 ,YN[ny],dist
              );
              fprintf(stderr, ":%s:%s:%s%s:%s:\n"
                 ,resAtomOLDptr->subc,resAtomOLDptr->resc,resAtomOLDptr->numc
                 ,resAtomOLDptr->rinsc,resAtomOLDptr->atomc
              );
             }
              sprintf(temps,"dump:%s:%s:%s%s:%s:-%s>:%.3f:%s:%s:%s%s:%s:"EOLO
                 ,resAtomOLDptr->subc,resAtomOLDptr->resc,resAtomOLDptr->numc
                 ,resAtomOLDptr->rinsc,resAtomOLDptr->atomc
                 ,YN[ny],dist
                 ,resAtomNEWptr->subc,resAtomNEWptr->resc,resAtomNEWptr->numc
                 ,resAtomNEWptr->rinsc,resAtomNEWptr->atomc
              );
              putonetextblockline(&mainscratch,temps);

              sprintf(temps,"dump:%s:%s:%s%s:%s:<%s-:%.3f:%s:%s:%s%s:%s:"EOLO
                 ,resAtomNEWptr->subc,resAtomNEWptr->resc,resAtomNEWptr->numc
                 ,resAtomNEWptr->rinsc,resAtomNEWptr->atomc
                 ,YN[ny],dist
                 ,resAtomOLDptr->subc,resAtomOLDptr->resc,resAtomOLDptr->numc
                 ,resAtomOLDptr->rinsc,resAtomOLDptr->atomc
              );
              putonetextblockline(&mainscratch,temps);
           }
        }/*distance was calculated, so noldat exists for this residue*/
        else
        {/*nothing known about connecting this residue*/
           if(resAtomNEWptr == NULL) {resAtomNEWptr = allocresatomstruct();}
           if(resAtomNEWptr != NULL)
           {
              decoderesatom(resAtomNEWptr,sub[jconn],res[jconn],num[jconn],rins[jconn],atom[jconn]);
             if(Lstderr)
             {
               fprintf(stderr, "%2d # %5d fore "
                          ":_:___:_____:_____:-X>:_:%s:%s:%s%s:%s:\n\n"
                ,level, icountres[level]-1
                ,resAtomNEWptr->subc,resAtomNEWptr->resc,resAtomNEWptr->numc
                ,resAtomNEWptr->rinsc,resAtomNEWptr->atomc
               );
               fprintf(stderr,"%2d # %5d back :%s:%s:%s%s:%s:<X-:\n"
                ,level, icountres[level]
                ,resAtomNEWptr->subc,resAtomNEWptr->resc,resAtomNEWptr->numc
                ,resAtomNEWptr->rinsc,resAtomNEWptr->atomc
               );
             }
             sprintf(temps, "dump:_:___:_____:_____:-X>:_:%s:%s:%s%s:%s:"EOLO
              ,resAtomNEWptr->subc,resAtomNEWptr->resc,resAtomNEWptr->numc
              ,resAtomNEWptr->rinsc,resAtomNEWptr->atomc
             );
             putonetextblockline(&mainscratch,temps);
             sprintf(temps,"dump:%s:%s:%s%s:%s:<X-:"EOLO
              ,resAtomNEWptr->subc,resAtomNEWptr->resc,resAtomNEWptr->numc
              ,resAtomNEWptr->rinsc,resAtomNEWptr->atomc
             );
             putonetextblockline(&mainscratch,temps);
           }
        }/*nothing known about connecting this residue*/

        if(endchn)
        {
           if(resAtomNEWptr == NULL) {resAtomNEWptr = allocresatomstruct();}
           if(resAtomNEWptr != NULL)
           {
              decoderesatom(resAtomNEWptr,sub[jconn],res[jconn],num[jconn],rins[jconn],atom[jconn]);
             if(Lstderr)
             {
              fprintf(stderr,"%2d # %5d fore :%s:%s:%s%s:_____:-E>:\n\n"
                 ,level,icountres[level]
                 ,resAtomNEWptr->subc,resAtomNEWptr->resc,resAtomNEWptr->numc
                 ,resAtomNEWptr->rinsc
              );
             }
              sprintf(temps,"dump:%s:%s:%s%s:_____:-E>:"EOLO
                 ,resAtomNEWptr->subc,resAtomNEWptr->resc,resAtomNEWptr->numc
                 ,resAtomNEWptr->rinsc
              );
              putonetextblockline(&mainscratch,temps);
           }
        }
     }/*in chain...*/
  }/*Lconnectresiduesdump*/
return;
}
/*__oldcon()________________________________________________________________*/

/****tacker()****************************************************************/
void   tacker(int nput,int nget)
{
  int    j;
  x[nput] = x[nget];
  y[nput] = y[nget];
  z[nput] = z[nget];
  o[nput] = o[nget];
  B[nput] = B[nget];
  Uval[nput] = Uval[nget];

  strcpy(Anum[nput],Anum[nget]); /*hybrid36 071001 ncnt[nput] = ncnt[nget] */
  num[nput]  = num[nget];
  for(j=0; j<= 5; j++){atom[nput][j] = atom[nget][j];}
  for(j=0; j<= 3; j++){res[nput][j] = res[nget][j];}
  for(j=0; j<= 6; j++){name[nput][j] = name[nget][j];}
  for(j=0; j<= 2; j++){sub[nput][j] = sub[nget][j];} /*2char 071222*/
  for(j=0; j<= 1; j++){rins[nput][j] = rins[nget][j];}
  for(j=0; j<= 2; j++){element[nput][j] = element[nget][j];} /*070520*/
  for(j=0; j< MAXmodchr; j++){mod[nput][j] = mod[nget][j];}
  /*residue's atom records inc mod[][MAXmodchr] for nmrmodel number str 0-99 */
  for(j=0; j< MAXaspectchars+3; j++){aspectstr[nput][j] = aspectstr[nget][j];}
  atstyle[nput] = atstyle[nget]; /*030920*/
  return;
}
/*__tacker()________________________________________________________________*/

/****muttacker()*************************************************************/
void   muttacker(int nput,int nget)
{
  int    j;
  mutx[nput] = mutx[nget];
  muty[nput] = muty[nget];
  mutz[nput] = mutz[nget];

  strcpy(mutAnum[nput],mutAnum[nget]); /*071001 mutncnt[nput] = mutncnt[nget] */
  mutnum[nput]  = mutnum[nget];
  for(j=0; j<= 5; j++){mutatom[nput][j] = mutatom[nget][j];}
  for(j=0; j<= 3; j++){mutres[nput][j] = mutres[nget][j];}
  for(j=0; j<= 6; j++){mutname[nput][j] = mutname[nget][j];}
  for(j=0; j<= 2; j++){mutsub[nput][j] = mutsub[nget][j];}  /*2char 071222*/
  for(j=0; j<= 1; j++){mutrins[nput][j] = mutrins[nget][j];}
  for(j=0; j< MAXmodchr; j++){mutmod[nput][j] = mutmod[nget][j];}
  return;
}
/*__muttacker()_____________________________________________________________*/

/****sstapler()**************************************************************/
void   sstapler(int nput,int nget)
{
  int    j;

  ssx[nput] = x[nget];
  ssy[nput] = y[nget];
  ssz[nput] = z[nget];
  strcpy(ssAnum[nput],ssAnum[nget]); /* 071001 ssncnt[nput] = ssncnt[nget] */
  ssnum[nput]  = num[nget];
  for(j=0; j<= 5; j++){ssatom[nput][j] = atom[nget][j];}
  for(j=0; j<= 3; j++){ssres[nput][j] = res[nget][j];}
  for(j=0; j<= 6; j++){ssname[nput][j] = name[nget][j];}
  for(j=0; j<= 2; j++){sssub[nput][j] = sub[nget][j];} /*2char 071222*/
  for(j=0; j<= 1; j++){ssrins[nput][j] = rins[nget][j];}
  for(j=0; j< MAXmodchr; j++){ssmod[nput][j] = mod[nget][j];}
  for(j=0; j< MAXaspectchars+3; j++){ssaspectstr[nput][j]=aspectstr[nget][j];}
  /*also store Bvalue...*/
  if(Ncolorscalebasis==3)
     ssBval[nput] = Uval[nget]; /*Unusual field in  PDB format: col 67...72*/
  else if(Ncolorscalebasis==2)
     ssBval[nput] = o[nget]; /*occupancy factor,  or PDB field: col 55...60*/
  else
     ssBval[nput] = B[nget]; /*temperature factor in PDB field: col 61...66*/
  return;
}
/*__sstapler()______________________________________________________________*/

/****creatm()****************************************************************/
int    creatm(int j)
{
  int  didit;

  /*--make sure there is room in the atom array for another vector */
  if(maxatm+1 >= lastat-8)
  {
    /*--no more room*/
/*c*/ sprintf(temps,"array exceeded, atom not duplicated: "

                    "%s %s %d %f %f %f"CRLF
             ,atom[j],res[j],num[j],x[j],y[j],z[j]);
        pkintextinsert(temps);  /*PKMCTEXT.C PKPCTEXT.C*/
        adjusttext(1);          /*PKMCTEXT.C PKPCTEXT.C*/
      didit = 0;
/*c*/ sprintf(alertstr,"array exceeded, atom not duplicated: "
                       CRLF"%s %s %d %f %f %f"
             ,atom[j],res[j],num[j],x[j],y[j],z[j]);
/*Ms*/ DoReportDialog();

  }
  else
  {
    maxatm = maxatm + 1;
/*.*/  tacker(maxatm,j);
    /*--   nput   nget   */
    didit = TRUE;
  }
  return(didit);
}
/*___creatm()_______________________________________________________________*/

/****mutadder()****************************************************************/
void   mutadder(int nput,int nget)
{
  int    j;
  /*coords, atom name, residue name, atom number  get replaced*/
  x[nput] = mutx[nget];
  y[nput] = muty[nget];
  z[nput] = mutz[nget];
  o[nput] = 1.0; /*occupancy artifcially set*/
  B[nput] = (float)99.99; /*Temperature parameter  artifcially set*/

  strcpy(Anum[nput],mutAnum[nget]); /*hybrid36 071001 ncnt[nput]=mutncnt[nget]*/
  for(j=0 ; j<= 5 ; j++) atom[nput][j] = mutatom[nget][j];
  for(j=0 ; j<= 3 ; j++) res[nput][j] = mutres[nget][j];

  /*record name, residue number, subunit indicator, insertion indicator  kept*/
  for(j=0 ; j<= 6 ; j++) name[nput][j] = name[maxatm][j];
  for(j=0 ; j<= 2 ; j++) sub[nput][j] = sub[maxatm][j]; /*2char 071222*/
  num[nput]  = num[maxatm];
  for(j=0 ; j<= 1 ; j++) rins[nput][j] = rins[maxatm][j];
  for(j=0 ; j< MAXmodchr ; j++) mod[nput][j] = mod[maxatm][j];
     /*nmr model # */
  for(j=0 ; j< MAXaspectchars+3 ; j++) aspectstr[nput][j] = aspectstr[nget][j];
  for(j=0 ; j<= 2 ; j++) element[nput][j] = mutelement[nget][j]; /*071222*/
  return;
}
/*name[n],Anum[n],atom[n],res[n],sub[n],&num[n],rins[n],&x[n],&y[n],&z[n]*/
/*
atom      2  ca  arg a   1      25.497  26.862  -1.573  1.00 17.63      4pti  90
*/
/*__mutadder()________________________________________________________________*/

/****screenatompair()********************************************************/
int    screenatompair(int j)
{
  int    itype;

  itype = 1; /*presume ok then test for exceptions */
        /* any pair in this direction will show up in the other direction */
        /* so can safely not connect in a particular direction */

  if(  atom[natom][1] == 'c' && atom[natom][2] == 'a'
    &&   atom[j][1] == 'c' && atom[j][2] == 'b') itype = 0;
      /*don't base ca on cb */

  if(  atom[natom][1] == 'c' && atom[natom][2] == ' '
    &&   atom[j][1] == 'o' && atom[j][2] == ' ') itype = 0;
      /*don't base c on o */

  if(  atom[natom][1] == 'c' && atom[natom][2] == 'a'
    &&   atom[j][1] == 'c' && atom[j][2] == ' ') itype = 0;
      /*don't base ca on c */

  if(  atom[natom][1] == 'n' && atom[natom][2] == ' '
    &&   atom[j][1] == 'c' && atom[j][2] == 'a') itype = 0;
      /*don't base n on ca */

  if(  atom[natom][2] == 'b' && atom[j][2] == 'g') itype = 0;
       /*don't base _b on _g */

  if(  atom[natom][2] == 'g' && atom[j][2] == 'd') itype = 0;
       /*don't base _g on _d */

  if(  atom[natom][2] == 'd' && atom[j][2] == 'e') itype = 0;
       /*don't base _d on _e */

  if(  atom[natom][2] == 'e' && atom[j][2] == 'z') itype = 0;
       /*don't base _e on _z */

  if(  atom[natom][2] == 'z' && atom[j][2] == 'h') itype = 0;
       /*don't base _z on _h */

  if(  atom[natom][2] == 'a' && atom[j][2] == 'e') itype = 0;
       /*don't base _a on _e proline with ne at position of n*/

  if(  atom[natom][2] == 'e' && atom[j][2] == 'a') itype = 0;
       /*don't base _e on _a proline with ne at position of n*/

  return(itype);
}
/*___screenatompair()_______________________________________________________*/

/****screenhetpairs()********************************************************/
int    screenhetpairs(int j)
{
  int    itype,k;

  itype = 1; /*presume ok then test for exceptions */
        /* any pair in this direction will show up in the other direction */
        /* so can safely not connect in a particular direction */


  /*Question being asked is j atom a base for natom atom?*/
  /* Is natom connected to j anywhere else? */
  /* for (1   ->   maxatom)  = k        */
  /*   look for other instances of j which may have been connected to natom */
  /*      if( atom[k] == atom[j]      */
  /*          does base[natom] == k  ? */
  /*          does base[k] == natom  ? */

  for( k=1 ; k <= maxatm ; k++ )
  {
    if(  atom[k][0] == atom[j][0] &&
      atom[k][1] == atom[j][1] &&
      atom[k][2] == atom[j][2] &&
      atom[k][3] == atom[j][3] &&
      atom[k][4] == atom[j][4]    )
    {
      if(base[natom] == k) itype = 0;
      if(base[k] == natom) itype = 0;
    }
  }
  return(itype);
}
/*___screenhetpairs()_______________________________________________________*/

/****decidemainside()********************************************************/
int    decidemainside(int j)
{
     int    Lncacos;

     Lncacos = FALSE;
     if( atom[j][1] == 'n' && atom[j][2] == ' ' ) Lncacos = TRUE;
else if( atom[j][1] == 'n' && atom[j][2] == 't' ) Lncacos = TRUE;
else if( atom[j][1] == 'c' && atom[j][2] == 'a' ) Lncacos = TRUE;
else if( atom[j][1] == 'c' && atom[j][2] == ' ' ) Lncacos = TRUE;
else if( atom[j][1] == 'o' && atom[j][2] == ' ' ) Lncacos = TRUE;
else if( atom[j][1] == 'o' && atom[j][2] == 'x' && atom[j][3] == 't')
         Lncacos = TRUE;

#ifdef GFPchromophore /*070731 this part works, BUT see PKINCSUB code... */
/*070731 code for GFP chromophore: flagged 070829*/
else if(  LGFPchromophore
        &&(  (atom[j][1]=='n'||atom[j][1]=='c'||atom[j][1]=='o')
           &&(atom[j][2]=='1'||atom[j][2]=='2'||atom[j][2]=='3')))Lncacos=TRUE;

else if(  LGFPchromophore
        &&(  (atom[j][1]=='c'&&atom[j][2]=='a')
           &&(atom[j][3]=='1'||atom[j][3]=='2'||atom[j][3]=='3')))Lncacos=TRUE;
#endif /*GFPchromophore 070731*/

#ifdef OLDCODE  /*pre 070728*/
/*070520 this still may be OK even with new hydrogen names ????*/
else if(  ( atom[j][1] == 'h' || atom[j][1] == 'd' || atom[j][1] == 'q') )
   if(    ( atom[j][2] == 'a' || atom[j][2] == ' ' )
        &&( atom[j][3] == ' ' && atom[j][0] != '2' ) ) Lncacos = TRUE;/*mc h*/
   if(    ( atom[j][3] == '*' || atom[j][3] == '\'') ) Lncacos = TRUE;
#endif
/*new code 070728...*/
else if(isahydrogen(name[j],atom[j],element[j]))
{
   if(   ( atom[j][3] == '*' || atom[j][3] == '\'') ) Lncacos = TRUE;/*nucleic*/
   else if(  (atom[j][2] == 'a' || atom[j][2] == ' ')
           &&(atom[j][3] == ' ' ) ) Lncacos = TRUE;/*mc h on ca and n*/
   else if(  (atom[j][2] == 'a' && atom[j][3] == ' ')
           &&(atom[j][0] == '1' || atom[j][0] == '2')) Lncacos = TRUE;/*oldgly*/
   else if(  (atom[j][2] == 'a' && atom[j][0] == ' ')
           &&(atom[j][3] == '3' || atom[j][3] == '2')) Lncacos = TRUE;/*newgly*/
   else if(  (atom[j][0] == ' ' && atom[j][3] == ' ')
           &&(isdigit(atom[j][2]))) Lncacos = TRUE; /*old&new Nterm*/
}
else if( atom[j][1] == 'p' && atom[j][2] == ' ' && atom[j][3] == ' ')
         Lncacos = TRUE;
/*pre 200707:*/
else if( atom[j][1] == 'o' && atom[j][2] == '1' && atom[j][3] == 'p')
         Lncacos = TRUE;
else if( atom[j][1] == 'o' && atom[j][2] == '2' && atom[j][3] == 'p')
         Lncacos = TRUE;
/*post 200707: Nucleic Mainchain OP1 and OP2  070726 */
else if( atom[j][1] == 'o' && atom[j][2] == 'p' && atom[j][3] == '1')
         Lncacos = TRUE;
else if( atom[j][1] == 'o' && atom[j][2] == 'p' && atom[j][3] == '2')
         Lncacos = TRUE;

else if( atom[j][1] == 'o' && atom[j][2] == '5' &&
        (atom[j][3] == '*' || atom[j][3] == '\'') ) Lncacos = TRUE;
else if( atom[j][1] == 'c' && atom[j][2] == '5' &&
        (atom[j][3] == '*' || atom[j][3] == '\'') ) Lncacos = TRUE;
else if( atom[j][1] == 'o' && atom[j][2] == '4' &&
        (atom[j][3] == '*' || atom[j][3] == '\'') ) Lncacos = TRUE;
else if( atom[j][1] == 'c' && atom[j][2] == '4' &&
        (atom[j][3] == '*' || atom[j][3] == '\'') ) Lncacos = TRUE;
else if( atom[j][1] == 'c' && atom[j][2] == '3' &&
        (atom[j][3] == '*' || atom[j][3] == '\'') ) Lncacos = TRUE;
else if( atom[j][1] == 'o' && atom[j][2] == '3' &&
        (atom[j][3] == '*' || atom[j][3] == '\'') ) Lncacos = TRUE;
else if( atom[j][1] == 'c' && atom[j][2] == '2' &&
        (atom[j][3] == '*' || atom[j][3] == '\'') ) Lncacos = TRUE;
else if( atom[j][1] == 'o' && atom[j][2] == '2' &&
        (atom[j][3] == '*' || atom[j][3] == '\'') ) Lncacos = TRUE;
else if( atom[j][1] == 'o' && atom[j][2] == '1' &&
        (atom[j][3] == '*' || atom[j][3] == '\'') ) Lncacos = TRUE;
else if( atom[j][1] == 'c' && atom[j][2] == '1' &&
        (atom[j][3] == '*' || atom[j][3] == '\'') ) Lncacos = TRUE;
return(Lncacos);
}
/*___decidemainside()_______________________________________________________*/

/****decidemutmainside()*****************************************************/
int    decidemutmainside(int j)
{
     int    Lncacos;

     Lncacos = FALSE;
     if( mutatom[j][1] == 'n' && mutatom[j][2] == ' ' ) Lncacos = TRUE;
else if( mutatom[j][1] == 'n' && mutatom[j][2] == 't' ) Lncacos = TRUE;
else if( mutatom[j][1] == 'c' && mutatom[j][2] == 'a' ) Lncacos = TRUE;
else if( mutatom[j][1] == 'c' && mutatom[j][2] == ' ' ) Lncacos = TRUE;
else if( mutatom[j][1] == 'o' && mutatom[j][2] == ' ' ) Lncacos = TRUE;
else if( mutatom[j][1] == 'o' && mutatom[j][2] == 'x' && mutatom[j][3] == 't')
         Lncacos = TRUE;
#ifdef OLDCODE
/*070520 this still may be OK even with new hydrogen names ????*/
/*no, gly was HA1 HA2 now HA2 HA3  071222 */
else if(  ( mutatom[j][1]== 'h' || mutatom[j][1]== 'd' || mutatom[j][1]== 'q')
        &&( mutatom[j][2]== 'a' || mutatom[j][2]== ' ') /*mainchain hydrogen*/
        &&( mutatom[j][3]== ' ' && mutatom[j][0]!= '2') ) Lncacos = TRUE;
#endif
else if(isahydrogenname(mutatom[j]))
{
   if(   ( mutatom[j][3] == '*' || mutatom[j][3] == '\'') ) Lncacos = TRUE;/*nucleic*/
   else if(  (mutatom[j][2] == 'a' || mutatom[j][2] == ' ')
           &&(mutatom[j][3] == ' ' ) ) Lncacos = TRUE;/*mc h on ca and n*/
   else if(  (mutatom[j][2] == 'a' && mutatom[j][3] == ' ')
           &&(mutatom[j][0] == '1' || mutatom[j][0] == '2')) Lncacos = TRUE;/*oldgly*/
   else if(  (mutatom[j][2] == 'a' && mutatom[j][0] == ' ')
           &&(mutatom[j][3] == '3' || mutatom[j][3] == '2')) Lncacos = TRUE;/*newgly*/
   else if(  (mutatom[j][0] == ' ' && mutatom[j][3] == ' ')
           &&(isdigit(mutatom[j][2]))) Lncacos = TRUE; /*old&new Nterm*/
}
else if( mutatom[j][1] == 'p' && mutatom[j][2] == ' ' && mutatom[j][3] == ' ')
         Lncacos = TRUE;
/*pre 200707:*/
else if( mutatom[j][1] == 'o' && mutatom[j][2] == '1' && mutatom[j][3] == 'p')
         Lncacos = TRUE;
else if( mutatom[j][1] == 'o' && mutatom[j][2] == '2' && mutatom[j][3] == 'p')
         Lncacos = TRUE;
/*post 200707: Nucleic Mainchain OP1 and OP2  070726 */
else if( mutatom[j][1] == 'o' && mutatom[j][2] == 'p' && mutatom[j][3] == '1')
         Lncacos = TRUE;
else if( mutatom[j][1] == 'o' && mutatom[j][2] == 'p' && mutatom[j][3] == '2')
         Lncacos = TRUE;

else if( mutatom[j][1] == 'o' && mutatom[j][2] == '5' &&
        (mutatom[j][3] == '*' || mutatom[j][3] == '\'') ) Lncacos = TRUE;
else if( mutatom[j][1] == 'c' && mutatom[j][2] == '5' &&
        (mutatom[j][3] == '*' || mutatom[j][3] == '\'') ) Lncacos = TRUE;
else if( mutatom[j][1] == 'o' && mutatom[j][2] == '4' &&
        (mutatom[j][3] == '*' || mutatom[j][3] == '\'') ) Lncacos = TRUE;
else if( mutatom[j][1] == 'c' && mutatom[j][2] == '4' &&
        (mutatom[j][3] == '*' || mutatom[j][3] == '\'') ) Lncacos = TRUE;
else if( mutatom[j][1] == 'c' && mutatom[j][2] == '3' &&
        (mutatom[j][3] == '*' || mutatom[j][3] == '\'') ) Lncacos = TRUE;
else if( mutatom[j][1] == 'o' && mutatom[j][2] == '3' &&
        (mutatom[j][3] == '*' || mutatom[j][3] == '\'') ) Lncacos = TRUE;
else if( mutatom[j][1] == 'c' && mutatom[j][2] == '2' &&
        (mutatom[j][3] == '*' || mutatom[j][3] == '\'') ) Lncacos = TRUE;
else if( mutatom[j][1] == 'o' && mutatom[j][2] == '2' &&
        (mutatom[j][3] == '*' || mutatom[j][3] == '\'') ) Lncacos = TRUE;
else if( mutatom[j][1] == 'o' && mutatom[j][2] == '1' &&
        (mutatom[j][3] == '*' || mutatom[j][3] == '\'') ) Lncacos = TRUE;
else if( mutatom[j][1] == 'c' && mutatom[j][2] == '1' &&
        (mutatom[j][3] == '*' || mutatom[j][3] == '\'') ) Lncacos = TRUE;
return(Lncacos);
}
/*___decidemutmainside()____________________________________________________*/
/*
sprintf(temps,CRLF"writesctoscratch(%d,%s) k==%d, xth:%c:"CRLF,j,cntl,k,xth);
pkintextinsert(temps);
adjusttext(1);
*/
/****writesctoscratch()******************************************************/
void   writesctoscratch(int j,char* cntl)
{
        char  colr[32];
  char  colr1[32],colr2[32];
        int   k,Lprint1,Lprint2,kj;
        float Avalue;
        char  extra[16],extra1[20],extra2[20];
        char  ballstr[32],hydrgstr[16];
static  char  xth = ' '; /*991128*/
        char rds[16],sty[16]; /*030920*/
        float radius=0;   /*050211 VRML*/
        char  whostr[32]; /*050211 VRML*/
        char  cs[MAXNAMECHAR+2]; /*051102*/

  k = base[j];

/*
    sprintf(temps,CRLF"sca %d: %s %s %s %d , isc== %d"
    ,j,atom[j],res[j],sub[j],num[j],isc[j]);
    pkintextinsert(temps);
    adjusttext(1);
*/
  if(lastsc == -1) /*true only on first call for this sidechain*/
  {
     if(Leachsclist)   /*991128*/
     {/*toggle output indicator for changed sidechain*/
        if(xth == '1') xth = '2';
        else           xth = '1';
     }
     else xth = ' '; /*for completeness*/
  }
  hydrgstr[0] = '\0';
  if(isc[j]==2) /*hydrogen got through to here*/
  {
      if(Leachsclist)
      {/*include hydrogens in sc list*/
         cntl[0]='s';cntl[1]='c';cntl[2]='v';cntl[3]=xth;
         Listhy = TRUE;Listsc = TRUE;typenow=typehy;
         sprintf(hydrgstr," gray! 'H'");
      }
      else
      {
         cntl[0]='h';cntl[1]='y';cntl[2]='s';cntl[3]='c';
         Listhy = TRUE;typenow=typehy;
      }
  }
  else
  {
      cntl[0]='s';cntl[1]='c';cntl[2]=' ';cntl[3]=xth;
      Listsc=TRUE;typenow=typesc;
  }

  if(Lalts) /*altstr is the pointmaster designator*/
  {
     altstr[0] = ' '; /*leading blank*/
     altstr[2] = atom[j][4];
  }
  Lprint1 = TRUE; /*print the base as P */
  Lprint2 = TRUE; /*print the atom as L */
  if(   (!(Lcbstubsrib && atom[k][1]=='c' && atom[k][2]=='b'))    /*050206*/
     && (lastsc == k) )
     /*cbstubsrib---CB vector, need explicitly both ends of CB---CG vector */
  {/*polyline*/
     Lprint1 = FALSE;
     if(Lalts && altstr[2]==' ')
     {/*may be join to primary conf, avoid line jumping previous atom*/
        altstr[2] = atom[k][4];
     }
  }
  if(Lalts && altstr[2]==' ')
  {/*try NO pointmaster for ' ' case*/
     altstr[0] = '\0';
  }

  lastsc = j;

  if( !scCPKoutl && !rotoutl)
  {
     Listscv = 1;
     if(isc[j]!=2)
     {
       cntl[0]='s';cntl[1]='c';cntl[2]='v';cntl[3] = xth;cntl[4] = '\0';
     }
  }
  else if(scCPKoutl)
  {
       /*else if(hyoutl && (atom[j][1] == 'h' || atom[j][1] == 'd'))*/
       if(hyoutl && isahydrogen(name[j],atom[j],element[j]) ) /*070520*/
       {
           Listsckh = 1;cntl[2] = 'k';cntl[3] = 'h';cntl[4] = '\0';
       }
       else if(atom[j][1] == 'c')
       {
           Listsckc = 1;cntl[2] = 'k';cntl[3] = 'c';cntl[4] = '\0';
       }
       else if(atom[j][1] == 'o')
       {
           Listscko = 1;cntl[2] = 'k';cntl[3] = 'o';cntl[4] = '\0';
       }
       else if(atom[j][1] == 'n')
       {
           Listsckn = 1;cntl[2] = 'k';cntl[3] = 'n';cntl[4] = '\0';
       }
       else
       {
           Listscks = 1;cntl[2] = 'k';cntl[3] = 's';cntl[4] = '\0';
       }
       Lprint1 = FALSE; /*just need unique atom of the vector pair*/
  }
  if(Lballonscpoints) /*050925 use ballradius which defaults to r=.2 */
  {
   if     (atom[j][1]=='o') sprintf(ballstr," B r=%.2f red! ",ballradius);
   else if(atom[j][1]=='n') sprintf(ballstr," B r=%.2f blue! ",ballradius);
   else if(atom[j][1]=='s') sprintf(ballstr," B r=%.2f yellow! ",ballradius);
   else ballstr[0] = '\0';
  }
  else  ballstr[0] = '\0';

  if(Latomradii)
  {
   /*if(atom[j][1]=='h') */
   if(isahydrogen(name[j],atom[j],element[j]) )  /*070520*/
   {
      if(atom[k][1]=='o'||atom[k][1]=='n')
      {
         sprintf(rds," r=%.2f ",polarHradius);
      }
      else
      {
         sprintf(rds," r=1.17 ");
      }
   }
   else if(atom[j][1]=='o') sprintf(rds," r=1.4 ");
   else if(atom[j][1]=='n') sprintf(rds," r=1.55 ");
   else if(atom[j][1]=='c') sprintf(rds," r=1.75 ");
   else if(atom[j][1]=='s') sprintf(rds," r=1.8 ");
   else if(atom[j][1]=='p') sprintf(rds," r=1.8 ");
   else sprintf(rds," r=2.0 "); /*e.g. metals,  was else rds[0] = '\0' 041017*/
  }
  else  rds[0] = '\0';

  if(atstyle[j] == 1)sprintf(sty," ghost");
  else sty[0] = '\0';

  if(Lprint1 || Lprint2) /*writesctoscratch()*/
  {/*output*/

    if(Bcoloutl)
    {/*setup to output individual color override as a funct of B*/
       if(Lprint1 && !Lprint2) kj = k;
       else                    kj = j;
       if(Ncolorscalebasis==3)
          Avalue = Uval[kj]; /*Unusual field in the PDB format: col 67...72*/
       else if(Ncolorscalebasis==2)
          Avalue = o[kj]; /*occupancy factor,  or PDB field: col 55...60*/
       else
          Avalue = B[kj]; /*temperature factor in PDB field: col 61...66*/

       Bcoloredoutput(colr, Avalue); /*separate routine 050704*/
    }/*setup to output individual color override as a funct of B*/
    else
    {
       colr[0] = '\0'; /*NULL string occupies NO space in %s output field*/
    }
    if(Bvaloutl && Lprint1)
    {/*occ&B for Lprint1*/
       /*whenever occ < 1.0 and when B != 0, include values in ptID*/ /*971213*/
       if(o[k] != 0.0 && o[k] != 1.0)
       {
          if(o[k] < 0.0) sprintf(extra," %2.2f",-o[k]);
          else           sprintf(extra," %2.2f", o[k]);
       }
       else extra[0] = '\0';
       /*Note Bcoloutl forces an entry for B even if it happens to be 0.0 */
       if(Bcoloutl || B[k] != 0.0)
       {
           if(extra[0] != '\0')sprintf(extra1,"%sB%.2f",extra,B[k]);
           else                sprintf(extra1," B%.2f",B[k]);
       }
       else sprintf(extra1,"%s",extra);
       /*Bcoloutl flag sets point color to B value scale*/
    }/*occ&B for Lprint1*/
    else extra1[0]='\0';
    if(Lsuperpos && ShortNameStr[0] != '\0')
    {
       sprintf(extra,"%s %s",extra1,ShortNameStr);
       sprintf(extra1,"%s",extra);
    }

    if(Bvaloutl && Lprint2)
    {/*occ&B for Lprint2*/
      /*whenever occ < 1.0 and when B != 0, include values in ptID*/ /*971213*/
      if(o[j] != 0.0 && o[j] != 1.0)
      {
         if(o[j] < 0.0) sprintf(extra," %2.2f",-o[j]);
         else           sprintf(extra," %2.2f", o[j]);
      }
      else extra[0] = '\0';
      /*Note Bcoloutl forces an entry for B even if it happens to be 0.0 */
      if(Bcoloutl || B[j] != 0.0)
      {
           if(extra[0] != '\0')sprintf(extra2,"%sB%.2f",extra,B[j]);
           else                sprintf(extra2," B%.2f",B[j]);
      }
      else sprintf(extra2,"%s",extra);
      /*Bcoloutl flag sets point color to B value scale*/
    }/*occ&B for Lprint2*/
    else extra2[0]='\0';
    if(Lsuperpos && ShortNameStr[0] != '\0')
    {
       sprintf(extra,"%s %s",extra2,ShortNameStr);
       sprintf(extra2,"%s",extra);
    }
    if(Lcodestr) /*051102*/
    {
       cs[0] = ' ';
       cs[1] = '\0';
       strcat(cs,codestr);
    }
    else
    {
       cs[0] = '\0';
    }
    /*writesctoscratch()----*/
    if(  ((!(Lcbstubsrib && atom[j][1]=='c' && atom[j][2]=='b'))    /*050206*/
       &&  !Lcbonly) 
       ||  (Lcbonly && atom[j][1]=='c' && atom[j][2]=='b')     )/*130121(...&&...)*/
       /*when Lcbstubsrib (rib---CB), do not also write the CA---CB vector*/
    {/*restricted output for sidechain*/
      if(Lsuperpos)
      {/*sc lumped together with sc for each residue in output*/
        Listsuppos = 1;
        sprintf(cntl,"supr");

        /*070731 fix format 1st 15 char of pointID==PDB %5s%3s %1s%4d%1s*/
        /* atom, res, sub, num, rins : NB atom incl alt as 5th character*/
        /*NOTE sub[] is now 2 char, extra char goes into preceeding space*/
        /* thus drop the blank-space field in pointID part of output 071222*/

        if(Lprint1)
        {
          /*050916 rds on P so balllists will have atomic radii*/
          /* 050705 altstr on P == that of following L type point (for KiNG)*/
          /* this may be wrong for this Lsuperpos section????*/
          if     (atom[k][1]=='o') sprintf(colr1," B r=.05 red! ");
          else if(atom[k][1]=='n') sprintf(colr1," B r=.05 blue! ");
          else if(atom[k][1]=='s') sprintf(colr1," B r=.05 yellow! ");
          else sprintf(colr1," cyan");
          sprintf(temps,"%s{%s%s%s%4d%s%s%s%s}%s%s P%s%s %.3f, %.3f, %.3f "EOLO
           ,cntl,atom[k],res[k],sub[k],num[k],rins[k],extra1,mod[k],cs
           ,aspectstr[k],colr1,altstr,rds,x[k],y[k],z[k]);
          ++countxyz;++pointcnt[typenow];countpoints(cntl,0);/*P point only*/
          putonetextblockline(&mainscratch, temps); /*mc to scratch*/
        }
        if(Lprint2)
        {
          if     (atom[j][1]=='o') sprintf(colr2," B r=.05 red! ");
          else if(atom[j][1]=='n') sprintf(colr2," B r=.05 blue! ");
          else if(atom[j][1]=='s') sprintf(colr2," B r=.05 yellow! ");
          else sprintf(colr2," cyan");
         sprintf(temps,"%s{%s%s%s%4d%s%s%s%s}%s%s L%s%s%s %.3f, %.3f, %.3f"EOLO
           ,cntl,atom[j],res[j],sub[j],num[j],rins[j],extra2,mod[j],cs
           ,aspectstr[j],colr2,altstr,rds,sty,x[j],y[j],z[j]);
          ++countxyz;++pointcnt[typenow];countpoints(cntl,0); /*L point only*/
          putonetextblockline(&mainscratch, temps); /*mc to scratch*/
        }
      }
      else
      {/*ususal division of mc, sc*/
        if(LdumpVRML && LvectorVRML) /*050208*/
        {
	  sprintf(cntl,"scv ");/*goes with sidechain vectors*/ //There was a merge problem with this sprintf statement - not sure if it should be in or not SML DCR 20151028
           sprintf(whostr," sc ");
           radius = ribwidVRML/nstrnd;
           writeVRMLvectortoscratch(cntl,colr2,radius,whostr
              ,x[k],y[k],z[k] ,x[j],y[j],z[j]);
        }
        else
        {
           if(Lprint1 && !Lprint2)
           {
            /*050916 rds on P so balllists will have atomic radii*/
            /* 050705 altstr on P == that of following L type point (for KiNG)*/
            /* on spec: when is Lprint1 true, Lprint2 false? Only for water?*/
      

             if(Lmagekinjas) /*101225*/ 
             {
              sprintf(temps,"%s{%s%s%s%4d%s%s%s%s}%s%s P,%s%s %.3f, %.3f, %.3f "
                ,cntl,atom[k],res[k],sub[k],num[k],rins[k],extra1,mod[k],cs
                  ,aspectstr[k],colr,altstr,rds,x[k],y[k],z[k]);
             }
             else
             {
              sprintf(temps,"%s{%s%s%s%4d%s%s%s%s}%s%s P%s%s %.3f, %.3f, %.3f "
                ,cntl,atom[k],res[k],sub[k],num[k],rins[k],extra1,mod[k],cs
                  ,aspectstr[k],colr,altstr,rds,x[k],y[k],z[k]);
             
             }


             putonetextblockline(&mainscratch,temps);
             ++countxyz;++pointcnt[typenow];countpoints(cntl,0);
           }
           else if(Lprint2 && !Lprint1)
           {
             /*maybe a continuation of a polyline*/
             if(Lmagekinjas) /*101225*/
             {
              sprintf(temps,"%s{%s%s%s%4d%s%s%s%s}%s%s L,%s%s%s%s%s "
                "%.3f, %.3f, %.3f"
                EOLO
                ,cntl,atom[j],res[j],sub[j],num[j],rins[j],extra2,mod[j],cs
                ,aspectstr[j],colr,ballstr,hydrgstr,altstr,rds,sty
                ,x[j],y[j],z[j]);
             }
             else
             {
              sprintf(temps,"%s{%s%s%s%4d%s%s%s%s}%s%s L%s%s%s%s%s "
                "%.3f, %.3f, %.3f"
                EOLO
                ,cntl,atom[j],res[j],sub[j],num[j],rins[j],extra2,mod[j],cs
                ,aspectstr[j],colr,ballstr,hydrgstr,altstr,rds,sty
                ,x[j],y[j],z[j]);
             }


             putonetextblockline(&mainscratch,temps);
                ++countxyz;++pointcnt[typenow];countpoints(cntl,0);
           }
           if(Lprint1 && Lprint2)
           {
             /*050705 altstr on P == that of following L type point (for KiNG)*/


             if(Lmagekinjas) /*101225*/
             {
              sprintf(temps,"%s{%s%s%s%4d%s%s%s%s} P,%s %.3f, %.3f, %.3f "
                EOLO
                ,cntl,atom[k],res[k],sub[k],num[k],rins[k],extra1,mod[k],cs
                ,altstr,x[k],y[k],z[k]);
             putonetextblockline(&mainscratch,temps);

              sprintf(temps,"%s{%s%s%s%4d%s%s%s%s}%s%s L,%s%s%s%s%s %.3f, %.3f, %.3f"
                EOLO
                ,cntl,atom[j],res[j],sub[j],num[j],rins[j],extra2,mod[j],cs
                ,aspectstr[j],colr,ballstr,hydrgstr,altstr,rds,sty
                ,x[j],y[j],z[j]); 
             putonetextblockline(&mainscratch,temps);
             }
             else
             {
              sprintf(temps,"%s{%s%s%s%4d%s%s%s%s} P%s %.3f, %.3f, %.3f "
                    "{%s%s%s%4d%s%s%s%s}%s%s L%s%s%s%s%s %.3f, %.3f, %.3f"
                EOLO
                ,cntl,atom[k],res[k],sub[k],num[k],rins[k],extra1,mod[k],cs
                ,altstr,x[k],y[k],z[k]
                ,atom[j],res[j],sub[j],num[j],rins[j],extra2,mod[j],cs
                ,aspectstr[j],colr,ballstr,hydrgstr,altstr,rds,sty
                ,x[j],y[j],z[j]);
             putonetextblockline(&mainscratch,temps);
             }

             ++countxyz;++pointcnt[typenow];countpoints(cntl,0);
             ++countxyz;++pointcnt[typenow];countpoints(cntl,0);
           }
        }
      }/*ususal division of mc, sc*/
    }/*restricted output for sidechain*/
  }/*output*/
}
/*___writesctoscratch()_____________________________________________________*/
/*
pkintextinsert(temps);
adjusttext(1);
*/
/*name[n],Anum[n],atom[n],res[n],sub[n],&num[n],rins[n],&x[n],&y[n],&z[n]*/
/*format(a6,a5,1x,a5,a3,1x,a1,i4,a1,3x,3&8.3)*/
/*  these next lines are 84 char long, they contain 80 char templates */
/*
0123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789
123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
atom      1  n   arg a   1      26.465  27.452  -2.490  1.00 25.18      4pti  89
atom      2  ca  arg a   1      25.497  26.862  -1.573  1.00 17.63      4pti  90
*/
/*  --name*6 record designator: atom or hetatom recognized  */
/*  --Anum*5 hybrid36 atom number: moved blindly to output file  */
/*  --atom*5 includes atomname*4 and alt-conformation-indicator*1,  */
/*  --res*3 residue name  */
/*  --sub*2 chain ID: subunit designator  */
/*  --num:i4 residue number  */
/*  --rins*1 inserted residue indicator  */
/*  --x,y,z coord of the atom  */
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****writescratch()**********************************************************/
void   writescratch(int maxcounter)
{
   /*actual mc and het write to scratch, writesctoscratch() called for sc */

  char  colr1[32],colr2[32];
static  char  cntl[5]; /* 4 blank characters */
  int  Lprint1,Lprint2;
  int      j,k,lastmc,ij,jj,LOK=1,LheadOK=1;
static  float  lastcax,lastcay,lastcaz;
  float    Avalue;
  char extra[16],extra1[20],extra2[20],cell[32];
  float Bvalmax=0;
  int Lnewresidue=0;
  char Bvalmaster[16];
  char rds[16],sty[16]; /*030920*/
  char  cs[MAXNAMECHAR+2]; /*051102*/

  cntl[0] = ' ';
  cntl[1] = ' ';
  cntl[2] = ' ';
  cntl[3] = ' ';
  cntl[4] = '\0'; /*equivalent to sprintf(cntl,"    "); */
  lastmc  = 0;
  lastsc  = 0;

  if(Lalts)
  {altstr[0]=' ';altstr[1]='\'';altstr[2]=' ';altstr[3]='\'';altstr[4]='\0';}
  else{altstr[0]='\0'; /*null string, takes no space on output line*/}

  if(Lsuperpos)
  {
     Lnewresidue = 1;
     Bvalmax = 0;
     for(j=1 ; j<=maxatm ; j++)
     {/*check over all atoms for sidechain B values, in writescratch() */
        if(   !decidemainside(j) && name[j][0] == 'a'
           && !isahydrogen(name[j],atom[j],element[j]) ) /*070520*/
           /*&& atom[j][1] != 'h' && atom[j][1] != 'd' && atom[j][1] != 'q' )*/
        {/*sidechain heavy atom*/
           if( B[j] > Bvalmax) Bvalmax = B[j];
        }/*sidechain heavy atom*/
     }/*check over all atoms for sidechain B values*/
     if(Bvalmax >= 40) sprintf(Bvalmaster,"B40-");
     else if(Bvalmax >= 30) sprintf(Bvalmaster,"B30-40");
     else if(Bvalmax >= 20) sprintf(Bvalmaster,"B20-30");
     else if(Bvalmax >= 10) sprintf(Bvalmaster,"B10-20");
     else sprintf(Bvalmaster,"B 0-10");
  }

  for(j=1 ; j<=maxatm ; j++)
  {/*loop over all atoms*/

    if(Laltcontrol!=0)
    {/*see if this atom meets alternate confromation specification(s)*/
       if(Laltcontrol>0)
       {/*inclusion*/
          LOK = 0; /*presume not to be included*/
          LheadOK = 0; /*presume not to be included*/
          /*see if head of possible vector is OK*/
            for(k=1;k<=Laltcontrol;k++)
            {/*check head for inclusion*/
              if(atom[j][4]==altcontrol[k]) LheadOK = 1;
            }
#ifdef OLD_ONLY_SAME_ALT /*031001*/
/*031001 rotations for mutations need whatever base an alt atom gets assigned*/
          if(LheadOK) /*still allowed*/
          {/*see if base is also OK*/
            for(k=1;k<=Laltcontrol;k++)
            {/*check base for inclusion*/
              if(atom[base[j]][4]==altcontrol[k]) LOK = 1;
            }
          }
#else
          if(LheadOK) LOK = 1;
#endif
       }/*inclusion*/
       else /*Laltcontrol is negative*/
       {/*exclusion*/
          LOK = 1; /*presume to be included*/
          LheadOK = 1; /*presume to be included*/
          /*see if head of possible vector is NOT OK*/
            for(k=1;k<=(-Laltcontrol);k++)
            {/*check for exclusion*/
              if(atom[j][4]==altcontrol[k]) LheadOK = 0;
            }
          if(LheadOK) /*not excluded yet*/
          {/*see if base excludes this vector*/
            for(k=1;k<=Laltcontrol;k++)
            {/*check base for inclusion*/
              if(atom[base[j]][4]==altcontrol[k]) LOK = 0;
            }
          }
       }/*exclusion*/
    }/*see if this atom meets alternate confromation specification(s)*/
    else LOK = 1; /*safety*/
#ifdef UNIX_X11
if(Ltestprint)
{
   fprintf(stderr,"Laltcontrol: %d: LOK==%d for atom[%d]==%s with base#%d==%s\n"
       ,Laltcontrol,LOK,j,atom[j],base[j],atom[base[j]]);
}
#endif
    if(LOK)
    {/*do this atom writescratch*/

    if(Lalts)
    {
       altstr[0] = ' '; /*leading blank*/
       altstr[2] = atom[j][4]; /*actual unique pointmaster character*/
       if(altstr[2]==' ')
       {/*try NO pointmaster of ' '*/
          altstr[0] = '\0';
       }
    }

    isc[j] = 0; /* determine later if this atom is sidechain */
    nextatm[j] = 0; /*determine later what atom follows this atom*/
    k = base[j];
    Lprint1 = FALSE;
    Lprint2 = FALSE;

    /*identify entry, check range controls, and write to scratch file*/
    if(name[j][0] == 'h') /*hetatm*/
    {/*hetatm*/
      if( (htoutl || waoutl) && (base[j] > 0) )
      /* only write out vectors fully specified */
      {
        if(  htoutl && notawater() )
        {/*NON-water hetatm*/
            if( !htCPKoutl  ) /*&& !(maxatm == 1 && base[1] == 1)*/
            {/*NOT isolated metal AND NOT trying to draw CPKs*/ /*990403*/
               /*if(hyoutl && (atom[j][1]=='h' || atom[j][1]=='d'))*//*030605*/
               /*if(hyoutl && isahydrogen(name[j],atom[j],element[j]) ) 070520*/
               if(isahydrogen(name[j],atom[j],element[j]) ) /*070730*/
               {
                  if(hyoutl) /*070730*/
                  {
                   cntl[0]='h';cntl[1]='y';cntl[2]='h';cntl[3]='t';cntl[4]='\0';
                   Listht = TRUE; Listhy = 1;typenow=typeht;
                   Lprint1 = TRUE;Lprint2 = TRUE;
                  }
               }
               else
               {
                  cntl[0]='h';cntl[1]='t';cntl[2]='v';cntl[3]=' ';cntl[4]='\0';
                  Listht = TRUE; Listhtv = 1;typenow=typeht;
                  Lprint1 = TRUE;Lprint2 = TRUE;
               }
            }/*NOT isolated metal AND NOT trying to draw CPKs*/
            else
            {/*CPKs*/  /*V3UNSAFE 070730, probably unsafe at any speed*/
                cntl[0] = 'h';cntl[1] = 't';
                Listht = TRUE;typenow=typeht; /*CPKs subset of het */

                /*else if(hyoutl && (atom[j][1] == 'h' || atom[j][1] == 'd'))*/
                if(hyoutl && isahydrogen(name[j],atom[j],element[j]) )/*070520*/
                {   Listhtkh = 1;cntl[2] = 'k';cntl[3] = 'h';cntl[4] = '\0';}
                else if(atom[j][1] == 'c')
                {   Listhtkc = 1;cntl[2] = 'k';cntl[3] = 'c';cntl[4] = '\0';}
                else if(atom[j][1] == 'o')
                {   Listhtko = 1;cntl[2] = 'k';cntl[3] = 'o';cntl[4] = '\0';}
                else if(atom[j][1]=='n'&&atom[j][2]!='i') /*e.g. Nickel offset*/
                {   Listhtkn = 1;cntl[2] = 'k';cntl[3] = 'n';cntl[4] = '\0';}
                else
                {   Listhtks = 1;cntl[2] = 'k';cntl[3] = 's';cntl[4] = '\0';}
                Lprint1 = FALSE;Lprint2 = TRUE;
                   /*just unique atom of the vector pair*/
            }/*CPKs*/
        }/*NON-water hetatm*/
        else if(waoutl && isawater() )
        {
           /*if( atom[j][1] == 'h' || atom[j][1] == 'd' || atom[j][1] == 'q')*/
           if(isahydrogen(name[j],atom[j],element[j]) ) /*070520*/
           {/*hydrogen*/
             if( hyoutl && (base[j] > 0) )
             {
               cntl[0] = 'h';cntl[1] = 'y';cntl[2] = 'w';cntl[3] = 'a';
               cntl[4] = '\0';/*960815*/
               Listhy = TRUE;typenow=typehy;
               Lprint1 = TRUE;Lprint2 = TRUE;
             }
           }
           else
           {
              cntl[0] = 'w';cntl[1] = 'a';cntl[2] = ' ';cntl[3] = ' ';
              cntl[4] = '\0';/*960815*/
              Listwa = TRUE;typenow=typewa;
              Lprint1 = TRUE;Lprint2 = FALSE;        /*990227*/
           }
        }
      }
    }/*hetatm*/
    else
    {/*atom*/
      if( decidemainside(j) ) /*s*/ /*, in writescratch() */
      {/*mainchain*/ /*no simple test to see if outputing mainchain atoms*/

       /*test for  ( mcoutl || caoutl || caonly )  as one goes along*/
        if(   (caonly || caoutl || tabloutl)
           && (j > maxcounter)  /*the extra, pseudo mainchain, connection*/
           && atom[j][1] == 'c'
           && atom[j][2] == 'a'
           && atom[k][1] == 'c'
           && atom[k][2] == 'a'    )  /*ca-ca*/
        {/*only ca--ca */
          /*j > maxcounter == limit of original atoms, connections back */
          /* to previous residue have j > maxcounter */
          if(Ltable) /*depends on Calpha-Calpha connection being made*/
          {
             Listtabl = 1; /*991122*/
             cntl[0] = 't';cntl[1] = 'a';cntl[2] = 'b';cntl[3] = 'l';
             cntl[4] = '\0';/*960815*/
             typenow=typeca;
             Lprint1 = 0;
             Lprint2 = TRUE;
             cell[0] = getresiduecode(res[j][0],res[j][1],res[j][2]);
                /*PKINROTL.c*/ /*returns 1-letter code*/
             cell[0] = toupper(cell[0]);
             cell[1] = '\0';
          }
          else
          {/*standard Calpha list*/
             cntl[0] = 'c';cntl[1] = 'a';cntl[2] = ' ';cntl[3] = ' ';
             cntl[4] = '\0';/*960815*/
             Listca = TRUE;typenow=typeca;
             Lprint1 = TRUE;Lprint2 = TRUE;
          }

          if(
            lastcax == x[k] &&
            lastcay == y[k] &&
            lastcaz == z[k]   ) Lprint1 = FALSE;

          lastcax = x[j];
          lastcay = y[j];
          lastcaz = z[j];

        }/*only ca--ca */
        else if
          (
             (  caoutl
              && (num[j]!=num[k] || rins[j][0]!=rins[k][0])
              && atom[k][1] == 'c' && atom[k][2] == '4'    /*set by oldcon*/
              && (atom[k][3] == '*' || atom[k][3] == '\'')/*from previous*/
              && atom[j][1] == 'p' && atom[j][2] == ' '   /*to present res*/
             )
           ||(  caoutl
              && num[j]==num[k]  && rins[j][0]==rins[k][0] /*within residue*/
              && atom[k][1] == 'p' && atom[k][2] == ' '   /*from*/
              && atom[j][1] == 'c' && atom[j][2] == '4'
              && (atom[j][3] == '*' || atom[j][3] == '\'')   /*to*/
             )
           ||(  caoutl
              && num[j]==num[k]  && rins[j][0]==rins[k][0] /*within residue*/
              && atom[k][1] == 'c' && atom[k][2] == '4'
              && (atom[k][3] == '*' || atom[k][3] == '\'')  /*from*/
              && atom[j][1] == 'c' && atom[j][2] == '1'     /*to*/
             )
           ||(  caoutl                               /* p--p only 061001*/
              && (num[j]!=num[k] || rins[j][0]!=rins[k][0])
              && atom[k][1] == 'p' && atom[k][2] == ' '    /*set by oldcon*/
              && (atom[k][3] == ' ' || atom[k][3] == '\'')/*from previous*/
              && atom[j][1] == 'p' && atom[j][2] == ' '   /*to present res*/
             )
           ||(  caoutl                               /* tRNA--AA 061006*/
              && (num[j]!=num[k] || rins[j][0]!=rins[k][0])
              && atom[k][1] == 'p' && atom[k][2] == ' '    /*set by oldcon*/
              && (atom[k][3] == ' ' || atom[k][3] == '\'')/*from previous*/
              && atom[j][1] == 'c' && atom[j][2] == 'a'   /*to present res*/
             )
           ||(  caoutl                               /* tRNA--AA 061006*/
              && (num[j]!=num[k] || rins[j][0]!=rins[k][0])
              && atom[k][1] == 'c' && atom[k][2] == '4'    /*set by oldcon*/
              && (atom[k][3] == '*' || atom[k][3] == '\'')/*from previous*/
              && atom[j][1] == 'c' && atom[j][2] == 'a'   /*to present res*/
             )
          )  /*nucleic acid equivalent of ca-ca: pseudo-backbone*/

        {
          cntl[0] = 'c';cntl[1] = 'a';cntl[2] = ' ';cntl[3] = ' ';
          cntl[4] = '\0';/*960815*/
          Listca = TRUE;typenow=typeca;
          Lprint1 = TRUE;Lprint2 = TRUE;
        }
        else if(   mcoutl && isahydrogen(name[j],atom[j],element[j]) )/*070520*/
            /*&&(atom[j][1] == 'h' || atom[j][1] == 'd' || atom[j][1] == 'q'))*/
        {/*putative mc hydrogen*/
          if( hyoutl && (base[j] > 0) )
          {/*functional hydrogen*/
            if( !mcCPKoutl )
            {
               cntl[0] = 'h';cntl[1] = 'y';cntl[2] = 'm';cntl[3] = 'c';
               cntl[4] = '\0';/*960815*/
               Listhy = TRUE;typenow=typehy;
               Lprint1 = TRUE;Lprint2 = TRUE;
            }
            else
            {/*doing CPK this pass*/
               Listmckh = 1;
               typenow=typehy;
               cntl[0]='m';cntl[1]='c';cntl[2]='k';cntl[3]='h';cntl[4]='\0';
               Lprint1 = FALSE; /*just need unique atom of the vector pair*/
               Lprint2 = TRUE;
            }
          }/*functional hydrogen*/
        }/*putative mc hydrogen*/
        else if( mcoutl && base[j] != 0)  /* mc && vector drawn to this atom*/
        {
          cntl[0] = 'm';cntl[1] = 'c';cntl[2] = ' ';cntl[3] = ' ';
          cntl[4] = '\0';/*960815*/
          Listmc = TRUE;typenow=typemc;
          Lprint1 = TRUE;Lprint2 = TRUE;
          if(lastmc == k) Lprint1 = FALSE;
          lastmc = j;
          if( !mcCPKoutl )
          {
              Listmcv = 1;
              cntl[0] = 'm';cntl[1] = 'c';cntl[2] = 'v';cntl[3] = ' ';
              cntl[4] = '\0';/*960815*/
          }
          else
          {
              /*else if(hyoutl && (atom[j][1] == 'h' || atom[j][1] == 'd'))*/
              if(hyoutl && isahydrogen(name[j],atom[j],element[j]) )
                      /*070520*/
              {   Listmckn = 1;cntl[2] = 'k';cntl[3] = 'h';cntl[4] = '\0';}
                 /*but logic doesn't let a hydrogen get to here ?????*/
              else if(atom[j][1] == 'c')
              {   Listmckc = 1;cntl[2] = 'k';cntl[3] = 'c';cntl[4] = '\0';}
              else if(atom[j][1] == 'o')
              {   Listmcko = 1;cntl[2] = 'k';cntl[3] = 'o';cntl[4] = '\0';}
              else if(atom[j][1] == 'n')
              {   Listmckn = 1;cntl[2] = 'k';cntl[3] = 'n';cntl[4] = '\0';}
              else
              {   Listmcks = 1;cntl[2] = 'k';cntl[3] = 's';cntl[4] = '\0';}
              Lprint1 = FALSE; /*just need unique atom of the vector pair*/
          }

        }
      }/*mainchain*/
      else  /*based on decidemainside(j) , in writescratch*/
      {/*sidechain*/
        if( scoutl )
        {/*doing sidechains*/
          /*if(atom[j][1] =='h' || atom[j][1] =='d' || atom[j][1] =='q')*/
          if(isahydrogen(name[j],atom[j],element[j]) )  /*070520*/
          {/*hydrogen*/
            if( hyoutl )
            {/*doing hydrogens*/
              if( !rotoutl)
              {/*NOT a rotation sidechain*/
                if( base[j] > 0)
                {/*do sc hydrogens here, unless doing a rotation residue*/
                  if( !scCPKoutl )
                  {/*NOT CPK*/
                     if(Leachsclist)
                     {/* sc hydrogens just flagged here*/
                        if( (base[j] > 0) )
                        isc[j] = 2; /*flag this atom as sidechain*/
                          /*2 for hydrogen*/
                      }/* sc hydrogens just flagged here*/
                      else
                      {/* sc hydrogens written to scratch here*/
                         cntl[0] ='h';cntl[1] ='y';cntl[2] ='s';cntl[3] ='c';
                         cntl[4] = '\0';
                         Listhy = TRUE;typenow=typehy;
                         Lprint1 = TRUE;Lprint2 = TRUE;
                      }/* sc hydrogens written to scratch here*/
                  }/*NOT CPK*/
                  else
                  {/*CPK hydrogens written to scratch here*/
                     cntl[0] = 's';cntl[1] = 'c';cntl[2] = 'k';cntl[3] = 'h';
                     cntl[4] = '\0';
                     Listsckh = TRUE;typenow=typehy;
                     Lprint1 = FALSE;Lprint2 = TRUE;
                  }/*CPK hydrogens written to scratch here*/
                }/*do sc hydrogens here, unless doing a rotation residue*/
              }/*NOT a rotation sidechain*/
              else
              {/* sc rotation hydrogens just flagged here*/
                if( (base[j] > 0) )
                   isc[j] = 2; /*flag this atom as sidechain*/
                          /*2 for hydrogen*/
              }/* sc rotation hydrogens just flagged here*/
            }/*doing hydrogens*/
          }/*hydrogen*/
          else
          {/* sc heavy atoms always just flagged here*/
#ifdef UNIX_X11
if(Ltestprint)
{
   fprintf(stderr,"sc atom[%d]==%s\n",j,atom[j]);
}
#endif
            if( (base[j] > 0) )
            {
               isc[j] = 1; /*flag this atom as sidechain*/
#ifdef UNIX_X11
if(Ltestprint)
{
   fprintf(stderr,"sc atom[%d]==%s has base#%d==%s\n"
      ,j,atom[j],base[j],atom[base[j]]);
}
#endif
            }
          }
        }/*doing sidechains*/
        /*NOTE: Lprint1 and Lprint2 are still FALSE for usual sidechain atoms*/
      }/*sidechain*/
    }/*atom*/

/*
67...72  : unused
73...76 4_char_file_code
77,78 atom symbol
80    charge
82,83,84 : reduce:new
85...90  : extra
colorscale->
colorscale->value;colorscale->origvalue;colorscale->color;colorscale->origcolor
for(j=1; j<= 9; j++)
   strcpy(colorscale[j]->color,colorscale[j]->origcolor);
*/
    if(Bcoloutl)
    {/*setup to output individual color override as a funct of B*/
      if( (Lprint1 && !Lprint2) || (Lhalfbonds) )
      {/*Lprint1*/
         if(Ncolorscalebasis==3)
            Avalue = Uval[k]; /*Unusual field in the PDB format: col 67...72*/
         else if(Ncolorscalebasis==2)
            Avalue = o[k]; /*occupancy factor,  or PDB field: col 55...60*/
         else
            Avalue = B[k]; /*temperature factor in PDB field: col 61...66*/
         Bcoloredoutput(colr1, Avalue); /*separate routine 050704*/
      }/*Lprint1*/
      if(Lprint2)
      {/*Lprint2*/
         if(Ncolorscalebasis==3)
            Avalue = Uval[j]; /*Unusual field in the PDB format: col 67...72*/
         else if(Ncolorscalebasis==2)
            Avalue = o[j]; /*occupancy factor,  or PDB field: col 55...60*/
         else
            Avalue = B[j]; /*temperature factor in PDB field: col 61...66*/
         Bcoloredoutput(colr2, Avalue); /*separate routine 050704*/
      }/*Lprint2*/

    }/*setup to output individual color override as a funct of B*/
    else if(LcolorbyNtoC) /*011109*/
    {
       if(name[j][0] == 'h') /*041029 het pt color colorbyNtoC set to white*/
       {
          sprintf(colr1,"white");
          sprintf(colr2,"white");
       }
       else
       {
          sprintf(colr1,"%s",colorbyNtoC);
          sprintf(colr2,"%s",colorbyNtoC);
       }
    }
    else
    {
       colr1[0] = '\0'; /*NULL string occupies NO space in %s output field*/
       colr2[0] = '\0';
    }

    if(Latomradii)
    {
       /*if(atom[j][1]=='h') */
       if(isahydrogen(name[j],atom[j],element[j]) ) /*070520*/
       {
          if(atom[k][1]=='o'||atom[k][1]=='n')
          {
             sprintf(rds," r=%.2f ",polarHradius);
          }
          else
          {
             sprintf(rds," r=1.17 ");
          }
       }
       else if(atom[j][1]=='o') sprintf(rds," r=1.4 ");
       else if(atom[j][1]=='n') sprintf(rds," r=1.55 ");
       else if(atom[j][1]=='c') sprintf(rds," r=1.75 ");
       else if(atom[j][1]=='s') sprintf(rds," r=1.8 ");
       else if(atom[j][1]=='p') sprintf(rds," r=1.8 ");
       else sprintf(rds," r=2.0 "); /*e.g. metals, was else rds[0]='\0' 041017*/
    }
    else  rds[0] = '\0';

    if(atstyle[j] == 1)sprintf(sty," ghost"); /*030920*/
    else sty[0] = '\0';



    if( (Bvaloutl && Lprint1) || (Bvaloutl && Lhalfbonds) )
    {/*occ&B for Lprint1*/
      /*whenever occ < 1.0 and when B != 0, include values in ptID*/ /*971213*/
      if((o[k] != 0.0 &&  o[k] != 1.0) || typenow==typewa)
      {
            if(o[k] < 0.0) sprintf(extra," %2.2f",-o[k]);
            else           sprintf(extra," %2.2f", o[k]);
      }
      else extra[0] = '\0';
      /*Note Bcoloutl forces an entry for B even if it happens to be 0.0 */
      if(Bcoloutl || B[k] != 0.0)
      {
           if(extra[0] != '\0')sprintf(extra1,"%s B%.2f",extra,B[k]);
           else                sprintf(extra1," B%.2f",B[k]);
      }
      else sprintf(extra1,"%s",extra);
      /*Bcoloutl flag sets point color to B value scale*/
    }/*occ&B for Lprint1*/
    else extra1[0]='\0';
    if(Lsuperpos && ShortNameStr[0] != '\0')
    {
       sprintf(extra,"%s %s",extra1,ShortNameStr);
       sprintf(extra1,"%s",extra);
    }

    if(Bvaloutl && Lprint2)
    {/*occ&B for Lprint2*/
      /*whenever occ < 1.0 and when B != 0, include values in ptID*/ /*971213*/
      if((o[j] != 0.0 &&  o[j] != 1.0) || typenow==typewa)
      {
            if(o[j] < 0.0) sprintf(extra," %2.2f",-o[j]);
            else           sprintf(extra," %2.2f", o[j]);
      }
      else extra[0] = '\0';
      /*Note Bcoloutl forces an entry for B even if it happens to be 0.0 */
      if(Bcoloutl || B[j] != 0.0)
      {
           if(extra[0] != '\0')sprintf(extra2,"%s B%.2f",extra,B[j]);
           else                sprintf(extra2," B%.2f",B[j]);
      }
      else sprintf(extra2,"%s",extra);
      /*Bcoloutl flag sets point color to B value scale*/
    }/*occ&B for Lprint2*/
    else extra2[0]='\0';
    if(Lsuperpos && ShortNameStr[0] != '\0')
    {
       sprintf(extra,"%s %s",extra2,ShortNameStr);
       sprintf(extra2,"%s",extra);
    }
    if(Lcodestr) /*051102*/
    {
       cs[0] = ' ';
       cs[1] = '\0';
       strcat(cs,codestr);
    }
    else
    {
       cs[0] = '\0';
    }
    /*writescratch()----*/

    /*FORMAT CONTROL: first 15 char of atom pointID == cols 13--27 inclusive PDB */
    /*{%s%s%s%4d%s...}  all character strings except resnum is integer*/
    /*atom 5 (incl alt), res 3, blank, sub==chainID 2, resnum 4, rins 1 */
    /*edit to make this standard  070731, 2char sub in preceeding space 071222*/

    if(Lprint1 || Lprint2) /*writescratch*/
    {/*actually write het and mc to scratch space*/
      if(Lsuperpos)
      {/*mc and sc lumped together for each residue in output*/
        Listsuppos = 1;
        sprintf(cntl,"supr");
        if(Lnewresidue)
        {
          sprintf(temps,"%s@vectorlist {%s %s %d%s %s scB %.2f} color= cyan nobutton master= {%s}"EOLO
           ,cntl,res[k],sub[k],num[k],rins[k],ShortNameStr,Bvalmax,Bvalmaster);
          putonetextblockline(&mainscratch, temps); /*header to scratch*/
          Lnewresidue = 0;
        }
        if(Lprint1)
        {
          /*050916 rds on P so balllists will have atomic radii*/
          /*050705 altstr on P == that of following L type point (for KiNG)*/
          /* this could be wrong for this Lsuperpos option? */
          if(colr1[0]=='\0') sprintf(colr1," white");
          sprintf(temps,"%s{%s%s%s%4d%s%s%s%s}%s%s P%s%s %.3f, %.3f, %.3f "
           EOLO
           ,cntl,atom[k],res[k],sub[k],num[k],rins[k],extra1,mod[k],cs
           ,aspectstr[k],colr1,altstr,rds,x[k],y[k],z[k]);
          ++countxyz;++pointcnt[typenow];countpoints(cntl,0);/*P point only*/
          putonetextblockline(&mainscratch, temps); /*mc to scratch*/
        }
        if(Lprint2)
        {
          if(colr2[0]=='\0') sprintf(colr2," white");
          sprintf(temps,"%s{%s%s%s%4d%s%s%s%s}%s%s L%s%s%s %.3f, %.3f, %.3f "
            EOLO
           ,cntl,atom[j],res[j],sub[j],num[j],rins[j],extra2,mod[j],cs
           ,aspectstr[j],colr2,altstr,rds,sty,x[j],y[j],z[j]);
          ++countxyz;++pointcnt[typenow];countpoints(cntl,0); /*L point only*/
          putonetextblockline(&mainscratch, temps); /*mc to scratch*/
        }
      }
      else
      {/*ususal division of mc, sc*/
       if(Lprint1 && !Lprint2)
       {
          /*050916 rds on P so balllists will have atomic radii*/
          /*050705 altstr on P == that of following L type point (for KiNG)*/
          /*but when does 1 but not 2 happen, and just what altstr is defined?*/


         if(Lmagekinjas) /*101225*/
         {
          sprintf(temps,"%s{%s%s%s%4d%s%s%s%s}%s%s P,%s%s %.3f, %.3f, %.3f "
           EOLO
           ,cntl,atom[k],res[k],sub[k],num[k],rins[k],extra1,mod[k],cs
           ,aspectstr[k],colr1,altstr,rds,x[k],y[k],z[k]);
         }
         else
         {
          sprintf(temps,"%s{%s%s%s%4d%s%s%s%s}%s%s P%s%s %.3f, %.3f, %.3f "
           EOLO
           ,cntl,atom[k],res[k],sub[k],num[k],rins[k],extra1,mod[k],cs
           ,aspectstr[k],colr1,altstr,rds,x[k],y[k],z[k]);
         }


          ++countxyz;++pointcnt[typenow];countpoints(cntl,0);/*P point only*/
          putonetextblockline(&mainscratch, temps); /*mc and het to scratch*/
       }
       else if(Lprint2 && !Lprint1 && !Ltable) /*991122*/
       {/*usual running next point of a polyline*/
        if(Lhalfbonds)
        {/*intermediate point divides vector into halves*/
         /*I do not remember that x[0] is ever used, so use it here*/
         x[0] = (x[j]+x[k])/2;
         y[0] = (y[j]+y[k])/2;
         z[0] = (z[j]+z[k])/2;
         sprintf(temps,"%s{%s%s%s%4d%s%s%s%s}%s%s L%s U %.3f, %.3f, %.3f "
                       "{%s%s%s%4d%s%s%s%s}%s%s L%s%s%s %.3f, %.3f, %.3f "EOLO
           ,cntl,atom[k],res[k],sub[k],num[k],rins[k],extra1,mod[k],cs
           ,aspectstr[k],colr1,altstr,x[0],y[0],z[0]
                ,atom[j],res[j],sub[j],num[j],rins[j],extra2,mod[j],cs
           ,aspectstr[j],colr2,altstr,rds,sty,x[j],y[j],z[j]);
         ++countxyz;++pointcnt[typenow];countpoints(cntl,0); /*L point*/
         ++countxyz;++pointcnt[typenow];countpoints(cntl,0); /*L point*/
         putonetextblockline(&mainscratch, temps); /*mc and het to scratch*/
        }/*intermediate point divides vector into halves*/
        else
        {/*vector ending on atom 2*/


          if(Lmagekinjas) /*101225*/
          {
           sprintf(temps,"%s{%s%s%s%4d%s%s%s%s}%s%s L,%s%s%s %.3f, %.3f, %.3f "
           EOLO
           ,cntl,atom[j],res[j],sub[j],num[j],rins[j],extra2,mod[j],cs
           ,aspectstr[j],colr2,altstr,rds,sty,x[j],y[j],z[j]);
          }
          else
          {
           sprintf(temps,"%s{%s%s%s%4d%s%s%s%s}%s%s L%s%s%s %.3f, %.3f, %.3f "
           EOLO
           ,cntl,atom[j],res[j],sub[j],num[j],rins[j],extra2,mod[j],cs
           ,aspectstr[j],colr2,altstr,rds,sty,x[j],y[j],z[j]);
          }


          ++countxyz;++pointcnt[typenow];countpoints(cntl,0); /*L point only*/
          putonetextblockline(&mainscratch, temps); /*mc and het to scratch*/
        }/*vector ending on atom 2*/

       }/*usual running next point of a polyline*/
       else if(Lprint2 && !Lprint1 && Ltable) /*991122*/
       {/*Ltable: row of seq numbers, row of residue names*/
          /*first do this residues name, then do its number*/
          /*the resulting scratch lists will be put out in reverse order*/


          if(pointcnt[typenow]==0)
          {/*create a zeroth cell for both lists: should have ROW title*/
             /*this is passed as the first point in wordlist*/
             if(MolNameStr[0] == '\0')
                  sprintf(extra,"ROW %d",level);
             else sprintf(extra,"%s %d",MolNameStr,level);
             cntl[3] = 'l'; /*tabl*/
             sprintf(temps,"%s{ROW} <%s> U %.3f, %.3f, %.3f"EOLO
              ,cntl
              ,extra
              ,x[j],y[j],z[j]);
             putonetextblockline(&mainscratch, temps); /*table*/
             ++countxyz;++pointcnt[typenow];countpoints(cntl,0);/*L point only*/
             cntl[3] = 'h'; /*tabh*/
             sprintf(temps,"%s{header} <%s> U %.3f, %.3f, %.3f"EOLO
              ,cntl
              ,extra
              ,x[j],y[j],z[j]);
             putonetextblockline(&mainscratch, temps); /*table*/
             ++countxyz;++pointcnt[typenow];countpoints(cntl,0);/*L point only*/
          }/*create a zeroth cell for both lists: should have ROW title*/

          /*cell with this residue's name*/
          cntl[3] = 'l'; /*tabl*/
          sprintf(temps,"%s{%s%s%s%4d%s%s%s%s} <%s> %.3f, %.3f, %.3f"EOLO
            ,cntl,atom[j],res[j],sub[j],num[j],rins[j],extra2,mod[j],cs
            ,cell
            ,x[j],y[j],z[j]);
          ++countxyz;++pointcnt[typenow];countpoints(cntl,0); /*L point only*/
          putonetextblockline(&mainscratch, temps); /*table*/

          /*cell with this residue's number*/
          cntl[3] = 'h'; /*tabh*/

          if(num[j]%10 != 0) {cell[0] = ' ';}
          else {sprintf(cell,"%d",num[j]);}
          sprintf(temps,"%s{%s%s%s%4d%s%s%s%s} <%s> %.3f, %.3f, %.3f"EOLO
             ,cntl,atom[j],res[j],sub[j],num[j],rins[j],extra2,mod[j],cs
             ,cell
             ,x[j],y[j],z[j]);
          ++countxyz;++pointcnt[typenow];countpoints(cntl,0); /*L point only*/
          putonetextblockline(&mainscratch, temps); /*table*/
       }/*Ltable*/
       else if(Lprint1 && Lprint2)
       {
        if(Lhalfbonds)
        {/*intermediate point divides vector into halves*/
         /*I do not remember that x[0] is ever used, so use it here*/
         x[0] = (x[j]+x[k])/2;
         y[0] = (y[j]+y[k])/2;
         z[0] = (z[j]+z[k])/2;
         /*050705 altstr on P == that of following L type point (for KiNG)*/
         sprintf(temps,"%s{%s%s%s%4d%s%s%s%s} P%s %.3f, %.3f, %.3f "
                         "{%s%s%s%4d%s%s%s%s}%s%s L%s U %.3f, %.3f, %.3f "
                         "{%s%s%s%4d%s%s%s%s}%s%s L%s%s%s %.3f, %.3f, %.3f"EOLO
           ,cntl,atom[k],res[k],sub[k],num[k],rins[k],extra1,mod[k],cs
           ,altstr,x[k],y[k],z[k]
                ,atom[k],res[k],sub[k],num[k],rins[k],extra1,mod[k],cs
           ,aspectstr[k],colr1,altstr,x[0],y[0],z[0]
                ,atom[j],res[j],sub[j],num[j],rins[j],extra2,mod[j],cs
           ,aspectstr[j],colr2,altstr,rds,sty,x[j],y[j],z[j]);
         ++countxyz;++pointcnt[typenow];countpoints(cntl,0); /*P point*/
         ++countxyz;++pointcnt[typenow];countpoints(cntl,0); /*L point*/
         ++countxyz;++pointcnt[typenow];countpoints(cntl,0); /*L point*/
         putonetextblockline(&mainscratch, temps); /*mc and het to scratch*/
        }/*intermediate point divides vector into halves*/
        else
        {/*vector from atom 1 to atom 2*/


         if(Lmagekinjas) /*101225*/
         {
          sprintf(temps,"%s{%s%s%s%4d%s%s%s%s} P,%s %.3f, %.3f, %.3f "
           EOLO
           ,cntl,atom[k],res[k],sub[k],num[k],rins[k],extra1,mod[k],cs
           ,altstr,x[k],y[k],z[k]);
         putonetextblockline(&mainscratch, temps); /*mc and het to scratch*/

          sprintf(temps,"%s{%s%s%s%4d%s%s%s%s}%s%s L,%s%s%s %.3f, %.3f, %.3f"
           EOLO
           ,cntl,atom[j],res[j],sub[j],num[j],rins[j],extra2,mod[j],cs
           ,aspectstr[j],colr2,altstr,rds,sty,x[j],y[j],z[j]);
         putonetextblockline(&mainscratch, temps); /*mc and het to scratch*/
         }
         else
         {
          sprintf(temps,"%s{%s%s%s%4d%s%s%s%s} P%s %.3f, %.3f, %.3f "
                         "{%s%s%s%4d%s%s%s%s}%s%s L%s%s%s %.3f, %.3f, %.3f"
           EOLO
           ,cntl,atom[k],res[k],sub[k],num[k],rins[k],extra1,mod[k],cs
           ,altstr,x[k],y[k],z[k]
                ,atom[j],res[j],sub[j],num[j],rins[j],extra2,mod[j],cs
           ,aspectstr[j],colr2,altstr,rds,sty,x[j],y[j],z[j]);
         putonetextblockline(&mainscratch, temps); /*mc and het to scratch*/
         }


         ++countxyz;++pointcnt[typenow];countpoints(cntl,0); /*P point*/
         ++countxyz;++pointcnt[typenow];countpoints(cntl,0); /*L point*/
        }/*vector from atom 1 to atom 2*/
       }
      }/*ususal division of mc, sc*/
    }/*actually write het and mc to scratch space*/

    }/*do this atom*/
  }/*loop over all atoms*/

  lastsc = -1; /*impossible value, so always get first atom's base */
  if(scoutl)
  {/*special section to sort sc output */

   if(rotoutl > 0)
   {/*rotation residue special case routine*/
       writescrotscratch(); /*calls getresiduerotations() which calls:*/
       /*findrotationaxis() and findrotationriders() actually write to scratch*/

       /*rotoutl now is number of rotation axes in the current residue*/
   }

   if(rotoutl == 0)/*getresiduerotations() can change rotoutl to 0 sans output*/
   {/*ordinary sidechain, output in a relatively efficient sequence*/
    /*   move this step into writesctoscratch(j);*/ /*PKINCSBS.c*/
    /*build paths through sidechain using nextatm to flag next atom */
    for(ij=1 ; ij<=maxatm ; ij++) /*note j moves on beyond ij */
    {/*2nd loop over all atoms for sc sorting*/
       if(isc[ij] > 0 && nextatm[ij] == 0)
       {/*only look at recognized sc that aren't yet in a path */
          j = ij;  /*start a new P L L ... run */
          /*write this sc to scratch file*/

          writesctoscratch(j,cntl); /*PKINCSBS.c*/

          for(jj=j+1; jj<=maxatm; jj++)
          {/*do run of sc atoms*/  /*allow hydrogens when in same list as sc*/
             /*Hydrogens can slip into here on failed rotation */
             if( (isc[jj] >= 1) && (base[jj] == j || base[jj] == idupl[j]) )
             {/*connected side chain atom*/
                if(base[j] == jj) /*one-before-this same as the-one-after*/
                {
                   nextatm[j] = -1; /* end run at jth atom */
                   isc[jj] = 0; /*jjth is extraneous, remove from sc list */
                }
                else
                {
                   nextatm[j] = jj;
                   j = jj;
                   /*write this next sc to scratch file*/

                   writesctoscratch(j,cntl); /*PKINCSBS.c*/

                }
             }
          }/*end run of sc atoms*/
          nextatm[j] = -1; /*last atom of run is j, flag it as part of a run*/
       }/*only look at recognized sc that aren't yet in a path */
    }/*2nd loop over all atoms for sc sorting*/
    if(Lfillscrings) tesselatescrings(cntl);
   }/*ordinary sidechain, output in a relatively efficient sequence*/

  }/*special section to sort sc output */
}
/*___writescratch()_________________________________________________________*/

/****tesselatescrings()******************************************************/
void tesselatescrings(char* cntl)
{
   int j,jj,tricnt=0,trimax=0,tri[11];
   char cth=' ',dth=' ',typth=' ';

   if     (res[maxatm][0]=='h' && res[maxatm][1]=='i' && res[maxatm][2]=='s')
      {trimax =  5;typth='P';}
   else if(res[maxatm][0]=='t' && res[maxatm][1]=='y' && res[maxatm][2]=='r')
      {trimax =  6;typth='P';}
   else if(res[maxatm][0]=='p' && res[maxatm][1]=='h' && res[maxatm][2]=='e')
      {trimax =  6;typth='P';}
   else if(res[maxatm][0]=='t' && res[maxatm][1]=='r' && res[maxatm][2]=='p')
      {trimax = 10;typth='P';}

   else if(res[maxatm][0]==' ' && res[maxatm][1]==' ' && res[maxatm][2]=='c')
      {trimax =  6;typth='N';}
   else if(res[maxatm][0]==' ' && res[maxatm][1]==' ' && res[maxatm][2]=='t')
      {trimax =  6;typth='N';}
   else if(res[maxatm][0]==' ' && res[maxatm][1]==' ' && res[maxatm][2]=='u')
      {trimax =  6;typth='N';}

   else if(res[maxatm][0]==' ' && res[maxatm][1]==' ' && res[maxatm][2]=='a')
      {trimax = 10;typth='N';}
   else if(res[maxatm][0]==' ' && res[maxatm][1]==' ' && res[maxatm][2]=='g')
      {trimax = 10;typth='N';}

/*
   else if(res[maxatm][0]=='p' && res[maxatm][1]=='r' && res[maxatm][2]=='o')
      trimax =  5;
*/
   else return; /*no rings*/

   for(jj=1; jj<= trimax; jj++)
   {/*find all atoms that define residue plane for tesselation*/
      /*atom nome 4 character positions: ath,bth,cth,dth*/
      /*element name in right adjusted in ath,bth*/
      /*so position in ring is by last 2 characters:  cth,dth*/
      /*first get specific code for this sidechain of order of ring atoms*/
      /*for doing the tesselation */
      if(typth == 'P')
      {/*Protein amino acid sidechain*/
         if     (jj==1) { cth = 'g'; dth = ' ';}
         else if(jj==2) { cth = 'd'; dth = '1';}
         else if(jj==3) { cth = 'd'; dth = '2';}
         else if(jj==4) { cth = 'e'; dth = '1';}
         else if(jj==5) { cth = 'e'; dth = '2';}
         else if(jj==6) { cth = 'z'; dth = ' ';} /*good for his,tyr,phe*/
         if(trimax > 6)
         {/*e.g. trp*/
            if     (jj== 6) { cth = 'd'; dth = '2';} /*fold over to other ring*/
            else if(jj== 7) { cth = 'z'; dth = '2';}
            else if(jj== 8) { cth = 'e'; dth = '3';}
            else if(jj== 9) { cth = 'h'; dth = '2';}
            else if(jj==10) { cth = 'z'; dth = '3';}
         }/*e.g. trp*/
      }
      else if(typth=='N')
      {/*Nucleic acid base*/
         if(trimax == 6)
         {/*pyrimadines*/
            if     (jj==1) { cth = '1'; dth = ' ';}
            else if(jj==2) { cth = '2'; dth = ' ';}
            else if(jj==3) { cth = '6'; dth = ' ';}
            else if(jj==4) { cth = '3'; dth = ' ';}
            else if(jj==5) { cth = '5'; dth = ' ';}
            else if(jj==6) { cth = '4'; dth = ' ';} /*good for c,u,t*/
         }
         if(trimax > 6)
         {/*e.g. purines*/
            if     (jj==1) { cth = '9'; dth = ' ';}
            else if(jj==2) { cth = '8'; dth = ' ';}
            else if(jj==3) { cth = '4'; dth = ' ';}
            else if(jj==4) { cth = '7'; dth = ' ';}
            else if(jj==5) { cth = '5'; dth = ' ';}
            else if(jj==6) { cth = '4'; dth = ' ';} /*second time*/
            else if(jj== 7) { cth = '6'; dth = ' ';}
            else if(jj== 8) { cth = '3'; dth = ' ';}
            else if(jj== 9) { cth = '1'; dth = ' ';}
            else if(jj==10) { cth = '2'; dth = ' ';}/*good for a,g*/
         }/*e.g. purines*/
      }
      /*then search out the atoms in the tesselation order*/
      for(j=1; j<=maxatm; j++)
      {/*scan over all atoms of this residue*/
         if(   (atom[j][1] == 'c' || atom[j][1] == 'n')
            && (atom[j][2] == cth)
            && (atom[j][3] == dth)
           )
         {
            tri[jj] = j;
            tricnt++;
            break;
         }

      }/*scan over all atoms of this residue*/
   }/*find all atoms that define residue plane for tesselation*/

   if(tricnt == trimax)
   {/*found the right number of atoms, write tesselation points*/
      /*Listscring=TRUE;*/  /*instead, add to previous list*/
      typenow=typerib; /*global output controls*/
      /*use last constructed cntl*/
      for(jj=1;jj<=trimax;jj++)
      {
         j = tri[jj];
         sprintf(temps,"%s{%s%s%s%4d%s} T%s %.3f, %.3f, %.3f"EOLO
           ,cntl,atom[j],res[j],sub[j],num[j],rins[j],altstr,x[j],y[j],z[j]);
         putonetextblockline(&mainscratch,temps); /*PKINSCRT.c*/
         ++countxyz;++pointcnt[typenow];countpoints(cntl,0);
      }
   }
}
/*___tesselatescrings()_____________________________________________________*/

/****writeatomscratch()******************************************************/
void   writeatomscratch(int maxcounter)
{/*only one, the jth, atom gets a mark*/

        char  colr[32];
static  char  cntl[5];/* 4 blank characters!*/
  int   Lprint2;
  int   i,j,k,LOK=1;
static  float  dx,dy,dz;
        float  Avalue;
  char extra[16],extra2[20];
  char rds[16]; /*050916*/
  char  cs[MAXNAMECHAR+2]; /*051102*/


  cntl[0] = ' ';
  cntl[1] = ' ';
  cntl[2] = ' ';
  cntl[3] = ' ';
  cntl[4] = '\0';

  for(j=1 ; j<=maxcounter ; j++)
  {/*loop over atoms in "residue"*/

    if(Laltcontrol!=0)
    {/*see if this atom meets alternate confromation specification(s)*/
       if(Laltcontrol>0)
       {/*inclusion*/
          LOK = 0; /*presume not to be included*/
          for(k=1;k<=Laltcontrol;k++)
          {/*check for inclusion*/
             if(atom[j][4]==altcontrol[k]) LOK = 1;
          }
       }/*inclusion*/
       else /*Laltcontrol is negative*/
       {/*exclusion*/
          LOK = 1; /*presume to be included*/
          for(k=1;k<=(-Laltcontrol);k++)
          {/*check for exclusion*/
             if(atom[j][4]==altcontrol[k]) LOK = 0;
          }
       }/*exclusion*/
    }/*see if this atom meets alternate confromation specification(s)*/
    if(LOK)
    {/*do this atom*/

    if(Lalts) /*050121*/
    {
       altstr[0] = ' '; /*leading blank*/
       altstr[2] = atom[j][4]; /*actual unique pointmaster character*/
       if(altstr[2]==' ')
       {/*try NO pointmaster of ' '*/
          altstr[0] = '\0';
       }
    }
    else
    {
       altstr[0] = '\0';
    }

    Lprint2 = FALSE;
    /*identify entry, check range controls, and write to scratch file*/
    if(name[j][0] == 'h')
    {/*hetatm*/
    /*if( (htaoutl || waaoutl) && (atom[j][1] != 'c') )*/
      if( (htaoutl || waaoutl) )  /*960809*/
      {
        if(  htaoutl && notawater()  )
        {
          cntl[0] = 'h';cntl[1] = 't';Listat = TRUE;

          /*else if (atom[j][1] == 'h')*/ /*041020 no balls on hydrogens*/
          if(isahydrogen(name[j],atom[j],element[j]) )/*070520*/
          {
             ; /*NOP*/
          }
          else if (atom[j][1] == 'n')
          {
              cntl[2] = 'n'; Listhtn = TRUE;typenow=typehtn;
              Lprint2 = TRUE;
          }
          else if (atom[j][1] == 'o')
          {
              cntl[2] = 'o'; Listhto = TRUE;typenow=typehto;
              Lprint2 = TRUE;
          }
          else if (atom[j][1] == 'c')
          {
              if( htcoutl )
              {
                  cntl[2] = 'c'; Listhtc = TRUE;typenow=typehtc;
                  Lprint2 = TRUE;
              }
          }
          /*pre 060208 all other atoms get colored like s */
          else if ((atom[j][1] == 's')||(atom[j][1] == 'p')) /*060208*/
          {
              cntl[2] = 's'; Listhts = TRUE;typenow=typehts;
              Lprint2 = TRUE;
          }
          else /*all other atoms get treated as metals 060208*/
          {
              cntl[2] = 'm'; Listhtm = TRUE;typenow=typehtm;
              Lprint2 = TRUE;
          }
        }
        else if(waaoutl && (atom[j][1] == 'o') && isawater() )
        {
          cntl[0] = 'w';cntl[1] = 'a';cntl[2] = 'o';
          Listat = TRUE;Listwao=TRUE;typenow=typewao;
          Lprint2 = TRUE;
        }
      }
    }/*hetatm*/
    else
    {/*atom*/
      if( decidemainside(j) )  /*s*/ /* in writeatomscratch() */
      {/*mainchain*/
      /*if( mcaoutl && (atom[j][1] != 'c') )*/
        if( mcaoutl )   /*960809*/
        {
          cntl[0] = 'm';cntl[1] = 'c';Listat = TRUE;

          if (atom[j][1] == 'n')
          {
              cntl[2] = 'n'; Listmcn = TRUE;typenow=typemcn;
              Lprint2 = TRUE;
          }
          else if (atom[j][1] == 'o')
          {
              cntl[2] = 'o'; Listmco = TRUE;typenow=typemco;
              Lprint2 = TRUE;
          }
          else if (atom[j][1] == 'c')
          {
              if (mccoutl )
              {
                  cntl[2] = 'c'; Listmcc = TRUE;typenow=typemcc;
                  Lprint2 = TRUE;
              }
          }
          else if (atom[j][1] == 'p') /*overload "s" */
          {   /*only n,o,c in protein mc, p in nucleic acid*/
              cntl[2] = 's'; Listmcs = TRUE;typenow=typemcs;
              Lprint2 = TRUE;
          }
        }
      }/*mainchain*/
      else
      {/*sidechain*/

       if(!Lcbonly || (Lcbonly && atom[j][1]=='c' && atom[j][2]=='b') )
       {/*restricted output: only Cbeta*/

        /*if( atom[j][1] == 'h' || atom[j][1] == 'd' || atom[j][1] == 'q')*/
        if(isahydrogen(name[j],atom[j],element[j]))  /*070520*/
        {/*hydrogen*/
          ;  /*skip*/
        }/*hydrogen*/
        /*else if( scaoutl && (atom[j][1] != 'c') )*/
        else if( scaoutl )  /*960809*/
        {/*NOT hydrogen*/
          cntl[0] = 's';cntl[1] = 'c';Listat = TRUE;

          if (atom[j][1] == 'n')
          {
              cntl[2] = 'n'; Listscn = TRUE;typenow=typescn;
              Lprint2 = TRUE;
          }
          else if (atom[j][1] == 'o')
          {
              cntl[2] = 'o'; Listsco = TRUE;typenow=typesco;
              Lprint2 = TRUE;
          }
          else if (atom[j][1] == 'c')
          {
            if( sccoutl)
            {
                cntl[2] = 'c'; Listscc = TRUE;typenow=typescc;
                Lprint2 = TRUE;
            }
          }
          else if (atom[j][1] == 'a' && atom[j][3] == '1')/*ASN,GLN o,n ??*/
          {
              cntl[2] = 'o'; Listsco = TRUE;typenow=typesco;
              Lprint2 = TRUE;
          }
          else if (atom[j][1] == 'a' && atom[j][3] == '2')/*ASN,GLN o,n ??*/
          {
              cntl[2] = 'n'; Listscn = TRUE;typenow=typescn;
              Lprint2 = TRUE;
          }
          else
          {   /*take care of sulfurs and make visible any wierd atoms */
              cntl[2] = 's'; Listscs = TRUE;typenow=typescs;
              Lprint2 = TRUE;
          }
        }/*NOT hydrogen*/

       }/*restricted output: only Cbeta*/

      }/*sidechain*/
    }/*atom*/
    cntl[3] = ' ';
    cntl[4] = '\0';

    if(Bcoloutl)
    {/*setup to output individual color override as a funct of B*/
       if(Ncolorscalebasis==3)
          Avalue = Uval[j]; /*Unusual field in the PDB format: col 67...72*/
       else if(Ncolorscalebasis==2)
          Avalue = o[j]; /*occupancy factor,  or PDB field: col 55...60*/
       else
          Avalue = B[j]; /*temperature factor in PDB field: col 61...66*/
       Bcoloredoutput(colr, Avalue); /*separate routine 050704*/
    }/*setup to output individual color override as a funct of B*/
    else
    {
       colr[0] = '\0'; /*NULL string occupies NO space in %s output field*/
    }

    if(Latomradii) /*050916*/
    {
      /*if(atom[j][1]=='h') */
      if(isahydrogen(name[j],atom[j],element[j]))  /*070520*/
      {
        k = base[j]; /*unclear when H would really get an atom-marker 061006*/
        if(atom[k][1]=='o'||atom[k][1]=='n')
        {
           sprintf(rds," r=%.2f ",polarHradius);
        }
        else
        {
           sprintf(rds," r=1.17 ");
        }
     }
     else if(atom[j][1]=='o') sprintf(rds," r=1.4 ");
     else if(atom[j][1]=='n') sprintf(rds," r=1.55 ");
     else if(atom[j][1]=='c') sprintf(rds," r=1.75 ");
     else if(atom[j][1]=='s') sprintf(rds," r=1.8 ");
     else if(atom[j][1]=='p') sprintf(rds," r=1.8 ");
     else sprintf(rds," r=2.0 "); /*e.g. metals*/
    }
    else  rds[0] = '\0';

    if(Lprint2)
    {/*output writeatomscratch()*/

      if(Bvaloutl)
      {/*occ&B for Lprint2*/
        /*whenever occ < 1.0 and when B != 0, include values in ptID*//*971213*/
        if((o[j] != 0.0 &&  o[j] != 1.0) || typenow==typewao)
        {
            if(o[j] < 0.0) sprintf(extra," %2.2f",-o[j]);
            else           sprintf(extra," %2.2f", o[j]);
        }
        else extra[0] = '\0';
        /*Note Bcoloutl forces an entry for B even if it happens to be 0.0 */
        if(Bcoloutl || B[j] != 0.0)
        {
           if(extra[0] != '\0')sprintf(extra2,"%sB%.2f",extra,B[j]);
           else                sprintf(extra2," B%.2f",B[j]);
        }
        else sprintf(extra2,"%s",extra);
        /*Bcoloutl flag sets point color to B value scale*/
      }/*occ&B for Lprint2*/
      else extra2[0]='\0';
      if(Lsuperpos && ShortNameStr[0] != '\0')
      {
         sprintf(extra,"%s %s",extra2,ShortNameStr);
         sprintf(extra2,"%s",extra);
      }
    if(Lcodestr) /*051102*/
    {
       cs[0] = ' ';
       cs[1] = '\0';
       strcat(cs,codestr);
    }
    else
    {
       cs[0] = '\0';
    }
      /*writeatomscratch()----*/
      if(Lcrosses)
      {/*cross atom marker*/
        /*zero length line at atom position*/
        sprintf(temps,"%s{%s%s%s%4d%s%s} P%s %.3f, %.3f, %.3f "
                        "{%s%s%s%4d%s%s%s%s}%s%s L%s%s %.3f, %.3f, %.3f"EOLO
         ,cntl,atom[j],res[j],sub[j],num[j],rins[j],extra2,altstr,x[j],y[j],z[j]
              ,atom[j],res[j],sub[j],num[j],rins[j],extra2,mod[j],cs
              ,aspectstr[j]
              ,colr,altstr,rds,x[j],y[j],z[j]);
        putonetextblockline(&mainscratch,temps); /*PKINSCRT.c*/
        ++countxyz; ++pointcnt[typenow];countpoints(cntl,0);
        ++countxyz; ++pointcnt[typenow];countpoints(cntl,0);
        for(i=1 ; i<=3 ; i++)
        {
          if(i==1) { dx = atmarker; dy = 0.0;      dz = 0.0;     }
          if(i==2) { dx = 0.0;      dy = atmarker; dz = 0.0;     }
          if(i==3) { dx = 0.0;      dy = 0.0;      dz = atmarker;}
          /*the cross arms*/
          sprintf(temps,"%s{%s%s%s%4d%s} P%s U %.3f, %.3f, %.3f "
                          "{%s%s%s%4d%s} L%s U %.3f, %.3f, %.3f"EOLO
     ,cntl,atom[j],res[j],sub[j],num[j],rins[j],altstr,x[j]-dx,y[j]-dy,z[j]-dz
          ,atom[j],res[j],sub[j],num[j],rins[j],altstr,x[j]+dx,y[j]+dy,z[j]+dz);
          putonetextblockline(&mainscratch,temps); /*PKINSCRT.c*/
          ++countxyz; ++pointcnt[typenow]; countpoints(cntl,0);
          ++countxyz; ++pointcnt[typenow]; countpoints(cntl,0);
        }
      }/*cross atom marker*/
      else
      {/*Single dot or ball*/
        if(LdumpVRML && LsphereVRML) /*050209*/
        {
           sprintf(temps,"%s %.3f %.3f %.3f "EOLO,cntl,x[j],y[j],z[j]);
           putonetextblockline(&mainscratch,temps); /*PKINSCRT.c*/
        }
        else /*Ldotsonly or Lballs*/
        {
           if(Lballs) {rds[0]='\0';} /*do not set point radius */
          sprintf(temps,"%s{%s%s%s%4d%s%s%s%s}%s%s P%s%s %.3f, %.3f, %.3f "EOLO
              ,cntl,atom[j],res[j],sub[j],num[j],rins[j],extra2,mod[j],cs
              ,aspectstr[j],colr,altstr,rds,x[j],y[j],z[j]);
           putonetextblockline(&mainscratch,temps); /*PKINSCRT.c*/
           ++countxyz; ++pointcnt[typenow]; countpoints(cntl,0);
        }
      }/*Single dot or ball*/
    }/*output*/
    }/*do this atom*/
  }/*loop over atoms in "residue"*/
}
/*___writeatomscratch()_____________________________________________________*/

/****writelabelscratch()*******************************************************/
void   writelabelscratch(void)
{
   char  colr[32];
   static  char  cntl[5]; /* 4 blank characters */
   char  lbl[4]; /* 3 blank characters */
   int   Lprint1;
   int   j,k,icalpha,LOK=1;
   float Avalue;


/*PC compiler has trouble with initial assignment if chr str is blank*/
  cntl[0] = ' ';
  cntl[1] = ' ';
  cntl[2] = ' ';
  cntl[3] = ' ';
  cntl[4] = '\0';
/*establish labels*/
 lbl[0]=' ';
 lbl[1]=' ';
 lbl[2]=' ';
 lbl[3]='\0';

 j = maxatm;
 if     (res[j][0] == 'a' && res[j][1] == 'l' && res[j][2] == 'a') lbl[1]='A';
 else if(res[j][0] == 'c' && res[j][1] == 'y' && res[j][2] == 's') lbl[1]='C';
 else if(res[j][0] == 'a' && res[j][1] == 's' && res[j][2] == 'p') lbl[1]='D';
 else if(res[j][0] == 'g' && res[j][1] == 'l' && res[j][2] == 'u') lbl[1]='E';
 else if(res[j][0] == 'p' && res[j][1] == 'h' && res[j][2] == 'e') lbl[1]='F';
 else if(res[j][0] == 'g' && res[j][1] == 'l' && res[j][2] == 'y') lbl[1]='G';
 else if(res[j][0] == 'h' && res[j][1] == 'i' && res[j][2] == 's') lbl[1]='H';
 else if(res[j][0] == 'i' && res[j][1] == 'l' && res[j][2] == 'e') lbl[1]='I';
 else if(res[j][0] == 'l' && res[j][1] == 'y' && res[j][2] == 's') lbl[1]='K';
 else if(res[j][0] == 'l' && res[j][1] == 'e' && res[j][2] == 'u') lbl[1]='L';
 else if(res[j][0] == 'm' && res[j][1] == 'e' && res[j][2] == 't') lbl[1]='M';
 else if(res[j][0] == 'a' && res[j][1] == 's' && res[j][2] == 'n') lbl[1]='N';
 else if(res[j][0] == 'p' && res[j][1] == 'r' && res[j][2] == 'o') lbl[1]='P';
 else if(res[j][0] == 'g' && res[j][1] == 'l' && res[j][2] == 'n') lbl[1]='Q';
 else if(res[j][0] == 'a' && res[j][1] == 'r' && res[j][2] == 'g') lbl[1]='R';
 else if(res[j][0] == 's' && res[j][1] == 'e' && res[j][2] == 'r') lbl[1]='S';
 else if(res[j][0] == 't' && res[j][1] == 'h' && res[j][2] == 'r') lbl[1]='T';
 else if(res[j][0] == 'v' && res[j][1] == 'a' && res[j][2] == 'l') lbl[1]='V';
 else if(res[j][0] == 't' && res[j][1] == 'r' && res[j][2] == 'p') lbl[1]='W';
 else if(res[j][0] == 't' && res[j][1] == 'y' && res[j][2] == 'r') lbl[1]='Y';
 else
 {
      lbl[0] = res[j][0];
      lbl[1] = res[j][1];
      lbl[2] = res[j][2];
 }
  icalpha = 0; /*flag for atom number of ca, */
               /*set later and use if can't find a cb*/
  for(j=1 ; j<=maxatm ; j++)
  {/*loop over all atoms, look for appropriate atom to which to attach label*/

    if(Laltcontrol!=0)
    {/*see if this atom meets alternate confromation specification(s)*/
       if(Laltcontrol>0)
       {/*inclusion*/
          LOK = 0; /*presume not to be included*/
          for(k=1;k<=Laltcontrol;k++)
          {/*check for inclusion*/
             if(atom[j][4]==altcontrol[k]) LOK = 1;
          }
       }/*inclusion*/
       else /*Laltcontrol is negative*/
       {/*exclusion*/
          LOK = 1; /*presume to be included*/
          for(k=1;k<=(-Laltcontrol);k++)
          {/*check for exclusion*/
             if(atom[j][4]==altcontrol[k]) LOK = 0;
          }
       }/*exclusion*/
    }/*see if this atom meets alternate confromation specification(s)*/
    if(LOK)
    {/*do this atom*/

    Lprint1 = FALSE; /*only cycle if still looking for a label place */

    /*identify entry, check range controls, and write to scratch file*/
    {
/*s*/ if( decidemainside(j) )  /* in writelabelscratch() */
      {/*mainchain*/
        if(atom[j][1] == 'c' && atom[j][2] == 'a' )
        {
            /*found a ca, remember this */
            icalpha = j;
            if( (caonly || lbl[1] == 'G')  )
            {
                /*use ca position since either only doing ca-ca*/
                /* or this is a glycine*/
                cntl[0] = 'l';cntl[1] = 'b';Listlb = TRUE;
                Lprint1 = TRUE;
            }
        }
      }
      else
      {/*sidechain*/
        if( lbl[1] != 'G' && atom[j][1] == 'c' && atom[j][2] == 'b' )
        {
          /*use cb position to label or simulate sidechain*/
          /* if this is not a glycine*/
          /* Actually, it would be better to calculate a cb position*/
          /* for glycines and put the label there! */
          cntl[0] = 'l';cntl[1] = 'b';Listlb = TRUE;
          Lprint1 = TRUE;
        }
      }
    }
/*
colorscale[1]->origvalue  =   4.0;
colorscale[2]->origvalue  =   8.0;
colorscale[3]->origvalue  =  11.0;
colorscale[4]->origvalue  =  16.0;
colorscale[5]->origvalue  =  22.0;
colorscale[6]->origvalue  =  30.0;
colorscale[7]->origvalue  =  40.0;
colorscale[8]->origvalue  =  60.0;
colorscale[9]->origvalue  = 100.0;
colorscale[10]->origvalue = 999.0;
for(j=1; j<=10; j++) colorscale[j]->value = colorscale[j]->origvalue;
strcpy(colorscale[1]->origcolor,"blue");
strcpy(colorscale[2]->origcolor,"purple");
strcpy(colorscale[3]->origcolor,"magenta");
strcpy(colorscale[4]->origcolor,"hotpink");
strcpy(colorscale[5]->origcolor,"red");
strcpy(colorscale[6]->origcolor,"orange");
strcpy(colorscale[7]->origcolor,"gold");
strcpy(colorscale[8]->origcolor,"yellow");
strcpy(colorscale[9]->origcolor,"yellowtint");
strcpy(colorscale[10]->origcolor,"white");
55...60  : occ
61...66  : B
67...72  : Unusual

*/

    if(Bcoloutl>0)
    {/*setup to output individual color override as a funct of A value*/
       if(Ncolorscalebasis==3)
          Avalue = Uval[j]; /*Unusual field in the PDB format: col 67...72*/
       else if(Ncolorscalebasis==2)
          Avalue = o[j]; /*occupancy factor,  or PDB field: col 55...60*/
       else
            Avalue = B[j]; /*temperature factor in PDB field: col 61...66*/
       Bcoloredoutput(colr, Avalue); /*separate routine 050704*/
    }/*setup to output individual color override as a funct of A value*/
    else
    {
       colr[0] = '\0'; /*NULL string occupies NO space in %s output field*/
    }


    if(Lprint1)
    {
      typenow = typelb;
      sprintf(temps,"%s{%s}%s%s %.3f, %.3f, %.3f"EOLO
          ,cntl,lbl,aspectstr[j],colr,x[j],y[j],z[j]);
      putonetextblockline(&mainscratch,temps);
      ++countxyz;++pointcnt[typenow];countpoints(cntl,0);
    }
    if(Lprint1) break; /*only need one */

    }/*do this atom*/
  }/*loop over all atoms*/

  if(!Lprint1) /*didn't find an appropriate atom, see if can use ca */
  {
      if(icalpha > 0)
      {
          j = icalpha;
          Lprint1 = 1;
          cntl[0] = 'l';cntl[1] = 'b';Listlb = TRUE;
      }
      if(Lprint1)
      {
        typenow = typelb;
          sprintf(temps,"%s{%s}%s%s %.3f, %.3f, %.3f"EOLO
            ,cntl,lbl,aspectstr[j],colr,x[j],y[j],z[j]);
        putonetextblockline(&mainscratch,temps);
        ++countxyz;++pointcnt[typenow];countpoints(cntl,0);
      }
  }
}
/*___writelabelscratch()____________________________________________________*/

/****pperptobase()************************************************************/
void pperptobase(int maxcounter)
{  /*050125 includes alternate of pperp to c1'--n line*/

   /*Nucleotides P-perpendiculars: perp 5'P(n), 3'P(n+1) */
   /* U n1 n3 c5 , C n1 n3 c5 , G n9 n3 n7 , A n9 n3 n7  */
   /*filtered for the only or the A alternate, 040202*/

   /*3 kinds of output for this residue (base)
     vectors from 5'P to base plane and from 3'P of next residue back to base.
     point in 5'P-->base, nextP-->base, delta_angle : this requires all 3 pts.
     colon delineated dump of :base ID: 5'P-->base: nextP-->base:
        where missing P gives 999 for distance and missing base gives 999:999.
   */

   float epsilon=999; /*default epsilon to impossible value  070521*/
   float dist1=999,dist2=999; /*defaults for missing information*/
   char atname[6][5];
   int j=0,ncnt=0,patm1=0;
   float atm[3][7],Bvalmax=0,delta=0;
   float deltatenth=0;
   char  colr[32],cntl[5],Bset=' ';
   float x1=0,y1=0,z1=0,x2=0,y2=0,z2=0,x3=0,y3=0,z3=0,x4=0,y4=0,z4=0;
   float A1[3] = {0,0,0}; /*050125 for Lpperptoc1nline*/
   float A2[3] = {0,0,0}; /*050125 for Lpperptoc1nline*/
   float B1[3] = {0,0,0}; /*050125 for Lpperptoc1nline*/
   float B2[3] = {0,0,0}; /*050125 for Lpperptoc1nline*/
   int   nofbase = 0, cofribose = 0; /*050125 for Lpperptoc1nline*/
   int   oofribose = 0; /*091129 for Loperptoc1nline*/
   float distoO3=999; /*091129 for Loperptoc1nline*/
   float disto3p=999;
   int   Lpperpoutlier=0; /*040705*/
   char  outc = ' ';      /*040705*/
   char  oute = ' ';      /*070521*/
   float P[3]={0,0,0},B[3]={0,0,0},N[3]={0,0,0};
   char  name[256];
   int   LRNA=0; /*070731*/
   char  ptmaster[4] = {'\0','\0','\0','\0'}; /*070731*/
   char  comnt[8] = {'\0','\0','\0','\0','\0','\0','\0','\0'}; /*070731*/

   /*SANITY CHECK: calc pperp for RNA only!, i.e. only if find o2' atom 070731*/
   LRNA = 0; /*presume not RNA unless find o2' atom */
   for(j=1 ; j<=maxcounter ; j++)
   {/*loop over atoms in "residue"*/
      if(   (atom[j][3]=='*' || atom[j][3]=='\'')
         && (atom[j][4]==' ' || atom[j][4]== 'a'))
      {/*ribose backbone, i.e. 3rd character * for prime*/
         if(atom[j][1]=='o' && atom[j][2]=='2')
         {/*is RNA */
            LRNA = 1; break;
         }
      }
   }/*loop over atoms in "residue"*/
   if(LRNA)       /*calc pperp*/
   {/*LRNA*/
      /* 070521 calculate epsilon as well as delta: need P of next residue*/
      /*define and set memory of indexes c4j c3j o3j pnj 070521*/
      int c4j=-1; int c3j=-1; int o3j=-1; int pnj=-1;

      /*070731 punt now, to do this right, use strstr() vs lists in PKINCRTL.h*/
      /*do simple check for pyrimidine by looking at 3rd character of name*/
      /*otherwise, most wierd bases seem to be purines, thus the default*/
      if(  res[1][2]=='u'||res[1][2]=='U'||res[1][2]=='c'||res[1][2]=='C'
         ||res[1][2]=='t'||res[1][2]=='T')
      {/*pyrimidine*/
         atname[1][1] = 'n'; atname[1][2] = '1';
         atname[2][1] = 'n'; atname[2][2] = '3';
         atname[3][1] = 'c'; atname[3][2] = '5';
         atname[4][1] = 'p'; atname[4][2] = ' ';
      }
      else/*if(res[1][2]=='g'||res[1][2]=='G'||res[1][2]=='a'||res[1][2]=='A')*/
      {/*default to purine*/
         atname[1][1] = 'n'; atname[1][2] = '9';
         atname[2][1] = 'n'; atname[2][2] = '3';
         atname[3][1] = 'n'; atname[3][2] = '7';
         atname[4][1] = 'p'; atname[4][2] = ' ';
      }
      Bvalmax = 0;
      /*find atoms in base, backbone, inc P */
      for(j=1 ; j<=maxcounter ; j++)
      {/*loop over atoms in "residue"*/
         if(atom[j][3]==' ' && (atom[j][4]==' ' || atom[j][4]=='a'))
         {/*find a specific atom in base or a P, i.e. 3rd field blank*/
            /*and 4th character either blank or A  for that alternate*/
            if(atom[j][1]==atname[1][1] && atom[j][2]==atname[1][2])
            {/*ribose-conecting n of the base*/
               atm[0][1] = x[j]; atm[1][1] = y[j]; atm[2][1] = z[j]; ncnt++;
               A2[0]=x[j];A2[1]=y[j];A2[2]=z[j];nofbase=1;/*Lpperptoc1nline*/
            }
            if(atom[j][1]==atname[2][1] && atom[j][2]==atname[2][2])
               { atm[0][2] = x[j]; atm[1][2] = y[j]; atm[2][2] = z[j]; ncnt++; }
            if(atom[j][1]==atname[3][1] && atom[j][2]==atname[3][2])
               { atm[0][3] = x[j]; atm[1][3] = y[j]; atm[2][3] = z[j]; ncnt++; }
            if(atom[j][1]==atname[4][1] && atom[j][2]==atname[4][2])
               { atm[0][4] = x[j]; atm[1][4] = y[j]; atm[2][4] = z[j];patm1=j;}

         }/*find a specific atom in base or a P, i.e. 3rd field blank*/

         /*delta for Lplotpperptobase calc from ribose backbone atoms*/
         /*also need o3prime for connection distance to next nucleotide*/
         /*find ribose backbone atoms*/
         /*Perpendicular to line of ribose c1'--n of base requires c1' 050125*/
         if(   (atom[j][3]=='*' || atom[j][3]=='\'')
            && (atom[j][4]==' ' || atom[j][4]== 'a'))
         {/*ribose backbone, i.e. 3rd character * for prime*/
           /*and 4th character either blank or A  for that alternate*/
            if(B[j] > Bvalmax) {Bvalmax = B[j];} /*maximum B value*/
            /*accummulate atom coords to calculate delta dihedral angle*/
            if(atom[j][1]=='c' && atom[j][2]=='5')
               {x1=x[j];y1=y[j];z1=z[j];}
            if(atom[j][1]=='c' && atom[j][2]=='4')
               {x2=x[j];y2=y[j];z2=z[j]; c4j = j;} /* c4j.c3j,o3j 070521...*/
            if(atom[j][1]=='c' && atom[j][2]=='3')
               {x3=x[j];y3=y[j];z3=z[j]; c3j = j;}
            if(atom[j][1]=='o' && atom[j][2]=='3')
               {x4=x[j];y4=y[j];z4=z[j]; o3j = j;oofribose=1;}/*operp 091129*/
            if(atom[j][1]=='c' && atom[j][2]=='1')
             {A1[0]=x[j];A1[1]=y[j];A1[2]=z[j];cofribose=1;}/*Lpperptoc1nline*/
         }

      }/*loop over atoms in "residue"*/
      if(   (!Lpperptoc1nline && ncnt == 3)
          ||(Lpperptoc1nline && (nofbase && cofribose)) )
      {/*base and sugar atoms found*/
         if(Lplotpperptobase)
         {
            if     (Bvalmax < 10) {Bset = 'p';}
            else if(Bvalmax < 20) {Bset = 'q';}
            else if(Bvalmax < 30) {Bset = 'r';}
            else if(Bvalmax < 40) {Bset = 's';}
            else if(Bvalmax < 50) {Bset = 't';}
            else if(Bvalmax < 60) {Bset = 'u';}
            else if(Bvalmax < 70) {Bset = 'v';}
            else if(Bvalmax < 80) {Bset = 'w';}
            else if(Bvalmax < 90) {Bset = 'x';}
            else if(Bvalmax <100) {Bset = 'y';}
            else                  {Bset = 'z';}
            sprintf(colr," %s",colorscale[Ncolorscale].color); 
               /*high value default*/ /*" %s", 130121*/
            for(j=1; j < Ncolorscale; j++)
            {
               if(Bvalmax< colorscale[j].value)
               {/*under this value*/
                  sprintf(colr," %s",colorscale[j].color); /*" %s", 130121*/
                 break;
               }
            }
            if(CompArgStr(colr,"default",7)) {colr[0]='\0';} /*040902*/
         }
         delta = (float)dihedral4pt(
                  (double)x1,(double)y1,(double)z1
                 ,(double)x2,(double)y2,(double)z2
                 ,(double)x3,(double)y3,(double)z3
                 ,(double)x4,(double)y4,(double)z4 );
         deltatenth = delta/10;
         Listext = 1;
         typenow = typeext;
         if(Lplotpperptobase) { sprintf(cntl,"extB");} /*extra output*/
         else {sprintf(cntl,"ext ");} /*extra output*/
         if(patm1)
         {/*compute first perpendicular from  5' */
            if(Lpperptoc1nline) /*050125*/
            {
               B1[0] = x[patm1]; B1[1] = y[patm1]; B1[2] = z[patm1];
               DoPerpendicularToLine(A1,A2,B1,B2);
               /*put point on line into index==5*/
               atm[0][5] = B2[0]; atm[1][5] = B2[1]; atm[2][5] = B2[2];
            }
            else
            {
              DoPerpendicularToPlane(atm);/*returns point in plane at index==5*/
            }
            if(Lplotpperptobase) /*includes Ldumppperptobase case*/
            {/*both plot and dump need distances from base to flanking P atoms*/
               dist1 = (float)sqrt( (atm[0][5]-x[patm1])*(atm[0][5]-x[patm1])
                                   +(atm[1][5]-y[patm1])*(atm[1][5]-y[patm1])
                                   +(atm[2][5]-z[patm1])*(atm[2][5]-z[patm1]) );
               /*wait for 2nd distance before output for plot or dump*/
            }
            else
            {/*show pperps in a kinemage*/
               if(!Lpperpoutliersonly ) /*when NOT restricted to 3'P outliers*/
               {/*show 5'P to base plane*/
                  sprintf(colr," green"); /*cntl=="ext "*/
                  sprintf(temps,"%s{%s%s%s%4d%s}%s P %.3f, %.3f, %.3f "
                                "{base: %s %s %d%s}"
                                "%s L '5' %.3f, %.3f, %.3f "EOLO
                  ,cntl,atom[patm1],res[patm1],sub[patm1],num[patm1],rins[patm1]
                     ,colr,x[patm1],y[patm1],z[patm1]
                                   ,res[patm1],sub[patm1],num[patm1],rins[patm1]
                     ,colr,atm[0][5],atm[1][5],atm[2][5]);
                  ++countxyz;++pointcnt[typenow];countpoints(cntl,0);
                  putonetextblockline(&mainscratch, temps);
               }
            }
         }/*compute first perpendicular*/
         else
         {
       /*NO initial 5'P, punt distance and hope that base ID defined by atom 1*/
            dist1 = 999; /*signel that 5'P not present*/
            patm1 = 1; /*dummy atom #1, presume it has name of next residue*/
         }
         /*if(Latomnext && atomnext[1]=='p' && (numnext == num[1]+1) ) 030527*/
       /*bug to be residue numbering order dependent, note hammerhead ribozyme*/
         dist2 = 999; /*signel that 3'P not accessible, usually overwrite this*/
         if(Latomnext && atomnext[1]=='p' && !endchn ) /*040222*/
         {
       /*compute second perpendicular from 3' P recognized in the next residue*/
            /*Latomnext: also need this P to calc epsilon  070521 */
         /*probably the only or the A alternate phosphorus of the next residue*/
            /*But first check that the next nucleotide is connected! */
            disto3p = (float)sqrt(  (x4-xnext)*(x4-xnext)
                                   +(y4-ynext)*(y4-ynext)
                                   +(z4-znext)*(z4-znext) );
            if(disto3p < 2)
            {/*close enough to connect*/

               /*----- insert calculation of epsilon ------  070521 */
               if(c4j > 0 && c3j > 0 && o3j > 0)
               {/*can calc epsilon*/
                  epsilon = (float)dihedral4pt(
                     (double)x[c4j], (double)y[c4j], (double)z[c4j],
                     (double)x[c3j], (double)y[c3j], (double)z[c3j],
                     (double)x[o3j], (double)y[o3j], (double)z[o3j],
                     (double)xnext , (double)ynext , (double)znext );
               }

               atm[0][4] = xnext;
               atm[1][4] = ynext;
               atm[2][4] = znext;
               if(Lpperptoc1nline) /*050125*/
               {
                  B1[0] = xnext; B1[1] = ynext; B1[2] = znext;
                  DoPerpendicularToLine(A1,A2,B1,B2);
                  /*put point on line into index==5*/
                  atm[0][5] = B2[0]; atm[1][5] = B2[1]; atm[2][5] = B2[2];
               }
               else
               {
                 DoPerpendicularToPlane(atm);/*rtns point in plane at index==5*/
               }
             /*both plot and dump need distances from base to flanking P atoms*/
               /*and display tobase needs distance to know and show outliers*/
               dist2 = (float)sqrt( (atm[0][5]-xnext)*(atm[0][5]-xnext)
                                   +(atm[1][5]-ynext)*(atm[1][5]-ynext)
                                   +(atm[2][5]-znext)*(atm[2][5]-znext) );
               Lpperpoutlier = pperpoutlier(delta,dist2); /*040705*/
               if(Lplotpperptobase) /*includes Ldumppperptobase case*/
               {
                  if(!Ldumppperptobase && patm1 && (dist1 < 999) ) /*040222*/
                  {/*plot dist1,dist2,delta: no plot if 5'P not present*/
                   /*plot delta,dist2,dist1   040229*/
                     sprintf(cntl,"ext "); /*extra output*/
                     /*NB spacing not standard*/
                     sprintf(temps,"%s{%s %s %s %d%s Bvalmax: %.2f}"
                                   "%s '%c' P %.3f, %.3f, %.3f "EOLO
                        ,cntl,MolNameStr
                        ,res[patm1],sub[patm1],num[patm1],rins[patm1],Bvalmax
                        ,colr,Bset,deltatenth,dist2,dist1);
                     /*MolNameStr as a file code identifier 040229*/
                     ++countxyz;++pointcnt[typenow];countpoints(cntl,0);
                     putonetextblockline(&mainscratch, temps);
                  }
               }
               else
               {/*show pperps in a kinemage, reworked 070731*/
                 if(!Lpperpoutliersonly)
                 {
                    sprintf(ptmaster,"'3'"); /*subclass of extra 070731*/
                 }
                 if(!Lpperpoutliersonly ||(Lpperpoutliersonly &&Lpperpoutlier))
                 {
                    /*if(Lpperpoutlier) {sprintf(colr," magenta");}*/ /*040705*/
                    /*else {sprintf(colr," gold");} */

                     if(Lpperpoutlier==0)
                     {sprintf(colr," gold"); comnt[0]='\0';}
                     else if(Lpperpoutlier==3)
                     {sprintf(colr," purple" ); sprintf(comnt," 3'?");}
                     else if(Lpperpoutlier==2)
                     {sprintf(colr," magenta"); sprintf(comnt," 2'?");}
                     else /*Lpperpoutlier==1*/
                     {sprintf(colr," white"  ); sprintf(comnt," ???");}
                     sprintf(cntl,"ext "); /*extra output*/
                     /*NB non-standard format: pt on base line*/

                     sprintf(temps,"%s{%s%s%s%4d%s%s}%s P %.3f, %.3f, %.3f "
                                     "{base: %s %s%4d%s%s}"
                                     "%s L %s %.3f, %.3f, %.3f "EOLO
                        ,cntl,atomnext,resnext,subnext,numnext,rinsnext,comnt
                        ,colr,xnext,ynext,znext
                        ,res[patm1],sub[patm1],num[patm1],rins[patm1],comnt
                        ,colr,ptmaster,atm[0][5],atm[1][5],atm[2][5]);
                     ++countxyz;++pointcnt[typenow];countpoints(cntl,0);
                     putonetextblockline(&mainscratch, temps);
                     if(Lpperpoutliersonly)
                     {
                        P[0]=xnext; P[1]=ynext; P[2]=znext;
                        B[0]=atm[0][5]; B[1]=atm[1][5]; B[2]=atm[2][5];
                        N[0]=atm[0][1]; N[1]=atm[1][1]; N[2]=atm[2][1];
                         sprintf(name," %s %s %d%s"
                            ,res[patm1],sub[patm1],num[patm1],rins[patm1]);
                       /*pperp foot==claw*/
                       drawclaw(&P[0],&B[0],&N[0],&name[0],colr);
                     }
                 }
               }
            }/*close enough to connect*/
         }/*compute second perpendicular*/
      }/*base and sugar atoms found*/
      else if(Ldumppperptobase)
      {/*missing atoms, presume residue/base identity attributes of 1st atom*/
         patm1 = 1; /*punt for dump info*/
      }
      if(Ldumppperptobase)
      {
         if(!Lpperpoutliersonly || (Lpperpoutliersonly && Lpperpoutlier))
         {
            if(Lpperpoutlier) {outc = 'X';} /*outlier to P of next residue*/
            else {outc = ' ';}
            /*epsilon going into next res*/
            if( epsilon<999 && epsilonoutlier(epsilon) ) {oute = 'X';}
            else {oute = ' ';}
            sprintf(cntl,"dump"); /*extra output, unset Listext, etc.*/
            Listext=0;/*so PKINCOUT/writeoutput() will not treat as a kinemage*/
            /*USED BY MOLPROBITY: correlate format, etc. */
            sprintf(temps,
          "%s: %s : %s : %s : %5d%s : %7.2f : %7.2f : %6.1f : %c : %6.1f : %c :"
                 EOLO   /*insertion code part of atom-number 070627*/
                 ,cntl,MolNameStr,res[patm1],sub[patm1],num[patm1],rins[patm1]
                 ,dist1,dist2,delta,outc,epsilon,oute);
               /*,cntl,topID,res[patm1],sub[patm1],num[patm1],dist1,dist2);*/
            ++countxyz;++pointcnt[typenow];countpoints(cntl,0);
            putonetextblockline(&mainscratch, temps);
         }
      }
   }/*LRNA*/
}
/*___pperptobase()___________________________________________________________*/

/****pperpoutlier()***********************************************************/
int  pperpoutlier(float delta, float dist3pPp) /*mod 060208, 130329 */
{/*outlier crt delta representing ribose pucker vs distance 3'P to base plane*/
/* S.J. 12/13/2014 - changes to make delta range consistent with suitename and RNA11, and removing the overlap of 2.9 - 3 ang range for both puckers*/
   /* 3' perp dist for C3' pucker (delta==  60° -- 100°) range: 2.9 -- 5.0 */
   /* 3' perp dist for C2' pucker (delta== 125° -- 165°) range: 0.0 -- 3.0 */
   
   /* original code
   float delta3pmin = (float)65,  delta3pmax = (float)104;
   float dist3pmin  = (float)2.9, dist3pmax  = (float)5.0;
   float delta2pmin = (float)129, delta2pmax = (float)162;
   float dist2pmin  = (float)0.0, dist2pmax  = (float)3.0; */
   
   float delta3pmin = (float)60,  delta3pmax = (float)105; /* changed by S.J.*/
   float dist3pmin  = (float)2.9, dist3pmax  = (float)6.0;
   float delta2pmin = (float)125, delta2pmax = (float)165;
   float dist2pmin  = (float)0.0, dist2pmax  = (float)2.9;
   int   Lout = 0; /*not an outlier*/

   if( (  (delta >= delta3pmin && delta <= delta3pmax)     /*OK 3' pucker*/
        &&(dist3pPp >= dist3pmin && dist3pPp <= dist3pmax)) /*OK for 3' pucker*/
     ||(  (delta >= delta2pmin && delta <= delta2pmax)     /*OK 2' pucker*/
        &&(dist3pPp >= dist2pmin && dist3pPp < dist2pmax)))/*OK for 2' pucker*/  /* changed from <= to <, 2.9 belongs to 3'pucker - S.J.*/
   { /*OK, not an outlier*/
      Lout = 0;
   }
   else /*otherwise, is an outlier*/
   {/* changed from 3 to 2.9 - S.J.*/
      if(dist3pPp < 2.9) /*"base-p distance indicates 2'-endo"*/ 
      {
         Lout = 2;
      }
      else /*dist3pPp >= 2.9 "base-p distance indicates 3'-endo"*/
      {
         Lout = 3;
      }
   }
   return(Lout);
}
/*___pperpoutlier()__________________________________________________________*/

#ifdef NICETRY
   if(delta >= delta3pmin && delta <= delta3pmax)
   { /*OK 3' delta*/
      if(dist3pPp >= dist3pmin && dist3pPp <= dist3pmax)
      { /*OK for 3' pucker*/
         Lout = 0;
      }
      else
      { /*magenta outlier, too short, unless wildly bad geometry*/
         Lout = 2;
      }
   }
   else if(delta >= delta2pmin && delta <= delta2pmax)
   { /*OK 2' delta*/
      if(dist3pPp >= dist2pmin && dist3pPp <= dist2pmax)
      { /*OK for 2' pucker*/
         Lout = 0;
      }
      else
      { /*purple outlier, too long, NB minimum is zero*/
         Lout = 3;
      }
   }
   else
   { /*otherwise, is a delta outlier*/
      Lout = 1;
   }
   return(Lout);

#endif

/****epsilonoutlier()*********************************************************/
int  epsilonoutlier(float epsilon) /*070521*/
{/*outlier of epsilon ranges means possible pucker error*/
   float epsilonmin = 155, epsilonmax = 310; /*suiteninit.h 070130*/
   /*epsilon is local...*/
   if(epsilon < 0){epsilon = 360 + epsilon;}
   if(epsilon < epsilonmin || epsilon > epsilonmax){return(1);}
   else {return(0);}
}
/*___epsilonoutlier()________________________________________________________*/

#ifdef OLDCODE
/****pperpoutlier()***********************************************************/
int  pperpoutlier(float delta, float dist3pPp)
{/*outlier crt delta representing ribose pucker vs distance 3'P to base plane*/
   /* 3' perp dist for C3' pucker (delta==  60° -- 100°) range: 2.9 -- 5.0 */
   /* 3' perp dist for C2' pucker (delta== 125° -- 165°) range: 0.0 -- 3.0 */
   float delta3pmin = (float)60,  delta3pmax = (float)100;
   float dist3pmin  = (float)2.9, dist3pmax  = (float)5.0;
   float delta2pmin = (float)125, delta2pmax = (float)165;
   float dist2pmin  = (float)0.0, dist2pmax  = (float)3.0;
   int   Lout = 0;

   if     (delta >= delta3pmin && delta <= delta3pmax)
   {/*3'pucker*/
      if(dist3pPp >= dist3pmin && dist3pPp <= dist3pmax) {Lout = 0;}
      else {Lout = 1;}
   }
   else if(delta >= delta2pmin && delta <= delta2pmax)
   {/*2'pucker*/
      if(dist3pPp >= dist2pmin && dist3pPp <= dist2pmax) {Lout = 0;}
      else {Lout = 1;}
   }
   return(Lout);
}
/*___pperpoutlier()__________________________________________________________*/
#endif
/****drawclaw()***************************************************************/
void drawclaw(float fp[3], float fb[3], float fn[3], char name[256], char* colr)
{
   double bp[3]={0,0,0},bn[3]={0,0,0},bx[3]={0,0,0};
   double bpm=0,bnm=0;
   float  btoe[3]={0,0,0},heel[3]={0,0,0},xtoe[3]={0,0,0},ytoe[3]={0,0,0};
   /*char   colr[32]; 070731 using param*/
   char   cntl[5]={'e','x','t',' ','\0'};
   char  ptmaster[4] = {'\0','\0','\0','\0'}; /*070731*/

   if(!Lpperpoutliersonly)
   {
      sprintf(ptmaster,"'3'"); /*subclass of extra 070731*/
   }
   bp[0] = (double)fp[0]-(double)fb[0];
   bp[1] = (double)fp[1]-(double)fb[1];
   bp[2] = (double)fp[2]-(double)fb[2];
   bn[0] = (double)fn[0]-(double)fb[0];
   bn[1] = (double)fn[1]-(double)fb[1];
   bn[2] = (double)fn[2]-(double)fb[2];
   bnm = sqrt(bn[0]*bn[0] + bn[1]*bn[1] + bn[2]*bn[2]);
   if(bnm > 0)
   {/*normalize*/
      bn[0] = bn[0]/bnm;
      bn[1] = bn[1]/bnm;
      bn[2] = bn[2]/bnm;
   }
   bpm = sqrt(bp[0]*bp[0] + bp[1]*bp[1] + bp[2]*bp[2]);
   if(bpm > 0)
   {/*normalize*/
      bp[0] = bp[0]/bpm;
      bp[1] = bp[1]/bpm;
      bp[2] = bp[2]/bpm;
   }
   /*compute bx, the perpendicular to basepoint--P and basepoint--N1or9 */
   crossproduct3array(&bp[0],&bn[0],&bx[0]);

   /*claw mark has long big toe toward N1or9 of actual base, heel away, */
   /*and cross wise toes perpendicular to big toe and ankle up to 3'P*/
   btoe[0] = fb[0] + (float)0.8*(float)bn[0];
   btoe[1] = fb[1] + (float)0.8*(float)bn[1];
   btoe[2] = fb[2] + (float)0.8*(float)bn[2];
   heel[0] = fb[0] - (float)0.4*(float)bn[0];
   heel[1] = fb[1] - (float)0.4*(float)bn[1];
   heel[2] = fb[2] - (float)0.4*(float)bn[2];
   xtoe[0] = fb[0] + (float)0.4*(float)bx[0];
   xtoe[1] = fb[1] + (float)0.4*(float)bx[1];
   xtoe[2] = fb[2] + (float)0.4*(float)bx[2];
   ytoe[0] = fb[0] - (float)0.4*(float)bx[0];
   ytoe[1] = fb[1] - (float)0.4*(float)bx[1];
   ytoe[2] = fb[2] - (float)0.4*(float)bx[2];

   /*sprintf(colr," magenta");*/ /*040705*/ /*using param, 070731*/
   /*cntl=="ext "*/  /* Unpickable 041001*/
   sprintf(temps,"%s{outlier:%s}%s P U %.3f, %.3f, %.3f "
      "{outlier:%s}%s L U %s width4 %.3f, %.3f, %.3f "EOLO
        ,cntl
        ,name,colr,heel[0],heel[1],heel[2]
        ,name,colr,ptmaster,btoe[0],btoe[1],btoe[2]);
   ++countxyz;++pointcnt[typenow];countpoints(cntl,0);
   putonetextblockline(&mainscratch, temps);
   sprintf(temps,"%s{outlier:%s}%s P U %.3f, %.3f, %.3f "
      "{outlier:%s}%s L U %s width4 %.3f, %.3f, %.3f "EOLO
        ,cntl
        ,name,colr,xtoe[0],xtoe[1],xtoe[2]
        ,name,colr,ptmaster,ytoe[0],ytoe[1],ytoe[2]);
   ++countxyz;++pointcnt[typenow];countpoints(cntl,0);
   putonetextblockline(&mainscratch, temps);
}
/*___drawclaw()______________________________________________________________*/

/****Bcoloredoutput()*********************************************************/
void Bcoloredoutput(char* color, float Avalue) /*separate routine 050704*/
{/*setup to output individual color override as a funct of B*/
 /**Bcoloredoutput() takes index of atom, returns color of atom*/
 /**usually Bvalue, Ncolorscalebasis selects Uval,occ,Bval */
 /*char color[32]*/
 int ncol=0,jth=0;

       /**default Scale by f-stop: 1,1.4,2,2.8,4,5.6,8,11,16,22,32,44,64 */
       /*" %s", 130121*/
       sprintf(color," %s",colorscale[Ncolorscale].color);/*high value default*/
       for(ncol=1; ncol < Ncolorscale; ncol++)
       {
          if(Avalue< colorscale[ncol].value)
          {/*under this value*/
             sprintf(color," %s",colorscale[ncol].color); /*" %s", 130121*/
             break;
          }
       }
       if(CompArgStr(color,"default",7)) {color[0]='\0';} /*040902*/
       for(jth=14; jth>=0; jth--)
       {
          color[jth+1] = color[jth];
       }
       color[0] = ' '; /*leading blank character spacer*/
}/*setup to output individual color override as a funct of B*/
/*___Bcoloredoutput()________________________________________________________*/

/****makemetalsymbol()*******************************************************/
void   makemetalsymbol(void)  /*990402 Metal Ball*/
{
static  char    cntl[5];  /* 4 blank characters */
    int      k;

    k = 1;
    cntl[0] = 'h';
    cntl[1] = 't';
    cntl[2] = 'm';
    cntl[3] = ' ';
    cntl[4] = '\0';

    Listht  = TRUE;
    Listhtm = TRUE;
    typenow = typehtm;

      sprintf(temps,"%s{%s%s%s%4d%s%s} %.3f, %.3f, %.3f"EOLO
          ,cntl
          ,atom[k],res[k],sub[k],num[k],rins[k],mod[k],x[k],y[k],z[k]);

      putonetextblockline(&mainscratch,temps); /*PKINSCRT.c*/

      ++countxyz;++pointcnt[typenow];countpoints(cntl,0); /*P point*/
}
/*___makemetalsymbol()______________________________________________________*/


